#include "sassy.h"
#include "ddcf.h"
#include "error.h"

int size_prop_changed;

int get_loop_dimensions (FuncGraph*, int, int*);
void dot_gen_fill (FuncGraph*, int, int*, int*);
int ele_gen_loop_dimensions (FuncGraph*, int, int, int*);
int slice_gen_loop_dimensions (FuncGraph*, int, int, int*);
int window_gen_loop_dimensions (FuncGraph*, int, int, int*);
int scalar_gen_loop_dimensions (FuncGraph*, int, int, int*);
void construct_array_loop_dimensions (FuncGraph*, int, int*);
void construct_concat_loop_dimensions (FuncGraph*, int, int*);
void construct_tile_loop_dimensions (FuncGraph*, int, int, int*);
void size_transfer (TypeInfo*, TypeInfo*);
void size_write (int*, int, int);
int get_int_const (char*);
void size_propagate_down ();
void size_propagate_up ();

void size_propagate ()
    {
    do
        {
	size_prop_changed = FALSE;
	/*
	printf ("calling 'size_propagate_down'\n"); fflush (stdout);
	*/
	size_propagate_down ();
	/*
	printf ("calling 'size_propagate_down'\n"); fflush (stdout);
	*/
	size_propagate_down ();
	/*
	printf ("calling 'size_propagate_up'\n"); fflush (stdout);
	*/
	size_propagate_up ();

	if (size_prop_changed)
	    is_stable = FALSE;
	}
    while (size_prop_changed);
    }

int get_loop_dimensions (FuncGraph *fg, int id, int *szs)
    {
    IntList *it;
    int gen_id, sgen_id, ret_id, rank;

    /*
    printf ("entering 'get_loop_dimensions'\n"); fflush (stdout);
    */

    /* find the generator graph */
    gen_id = find_generator_graph (fg, id);

    if (fg->nodes[gen_id].nodetype == ND_CROSS_PROD)
	for (rank=0, it=fg->nodes[gen_id].My_nodes; it!=NULL; it=it->link)
	    {
	    sgen_id = it->val;

	    switch (fg->nodes[sgen_id].nodetype)
	        {
		case ND_ELE_GEN :
		    rank = ele_gen_loop_dimensions (fg, sgen_id, rank, szs);
		    break;
		case ND_SLICE_GEN :
		    rank = slice_gen_loop_dimensions (fg, sgen_id, rank, szs);
		    break;
		case ND_WINDOW_GEN :
		    rank = window_gen_loop_dimensions (fg, sgen_id, rank, szs);
		    break;
		case ND_SCALAR_GEN :
		    rank = scalar_gen_loop_dimensions (fg, sgen_id, rank, szs);
		    break;
		default :
                    break;
        	}
	    }
    else
        {
	for (it=fg->nodes[gen_id].My_nodes; it!=NULL; it=it->link)
	    {
	    sgen_id = it->val;

	    switch (fg->nodes[sgen_id].nodetype)
	        {
		case ND_ELE_GEN :
		    rank = ele_gen_loop_dimensions (fg, sgen_id, 0, szs);
		    break;
		case ND_SLICE_GEN :
		    rank = slice_gen_loop_dimensions (fg, sgen_id, 0, szs);
		    break;
		case ND_WINDOW_GEN :
		    rank = window_gen_loop_dimensions (fg, sgen_id, 0, szs);
		    break;
		case ND_SCALAR_GEN :
		    rank = scalar_gen_loop_dimensions (fg, sgen_id, 0, szs);
		    break;
		default :
                    break;
        	}
	    }
	}

    /* find interesting return nodes */
    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
	{
	ret_id = it->val;
	switch (fg->nodes[ret_id].nodetype)
	    {
	    case ND_CONSTRUCT_ARRAY :
		construct_array_loop_dimensions (fg, ret_id, szs);
	        break;
	    case ND_CONSTRUCT_CONCAT :
		construct_concat_loop_dimensions (fg, ret_id, szs);
	        break;
	    case ND_CONSTRUCT_TILE :
		construct_tile_loop_dimensions (fg, ret_id, rank, szs);
	        break;
	    default :
                break;
	    }
	}

    /*
    printf ("leaving 'get_loop_dimensions'\n"); fflush (stdout);
    */

    return rank;
    }

void dot_gen_fill (FuncGraph *fg, int rank, int *szs, int *loc_sizes)
    {
    int i;

    for (i=0; i<rank; i++)
        if (loc_sizes[i] >= 0)
	    if (szs[i] >= 0)
		size_write (szs, loc_sizes[i], i);
    }

int ele_gen_loop_dimensions (FuncGraph *fg, int sgen_id, int i, int *szs)
    {
    int j, k, step, hi, val;

    for (j=0, k=1; j<fg->nodes[sgen_id].inputs[0].ty.dims; j++)
      {
	if ((fg->nodes[sgen_id].inputs[0].ty.dim_sizes[j] >= 0) &&
	    (fg->nodes[sgen_id].inputs[k].is_const))
	  {
	    hi = fg->nodes[sgen_id].inputs[0].ty.dim_sizes[j] - 1;
	    step = get_int_const (fg->nodes[sgen_id].inputs[k].constval);
	    val = hi<0?0:1+hi/step;
	    size_write (szs, val, i);
	  }
	i++;
	k++;
      }
    
    return i;
    }

int slice_gen_loop_dimensions (FuncGraph *fg, int sgen_id, int i, int *szs)
    {
    int j, k, step, hi, val;

    for (j=0, k=1; j<fg->nodes[sgen_id].inputs[0].ty.dims; j++)
	if (fg->nodes[sgen_id].Reftypes[j] != ':')
            {
	    if ((fg->nodes[sgen_id].inputs[0].ty.dim_sizes[j] >= 0) &&
	    		(fg->nodes[sgen_id].inputs[k].is_const))
		{
		hi = fg->nodes[sgen_id].inputs[0].ty.dim_sizes[j] - 1;
		step = get_int_const (fg->nodes[sgen_id].inputs[k].constval);
		val = hi<0?0:1+hi/step;
		size_write (szs, val, i);
		}
	    i++;
	    k++;
	    }

    return i;
    }

int window_gen_loop_dimensions (FuncGraph *fg, int sgen_id, int i, int *szs)
    {
    int j, k, ext, wsize, step, val;

    for (j=0, k=1; j<fg->nodes[sgen_id].inputs[0].ty.dims; j++)
	{
        if ((fg->nodes[sgen_id].inputs[0].ty.dim_sizes[j] >= 0) &&
			(fg->nodes[sgen_id].inputs[k].is_const) &&
			(fg->nodes[sgen_id].inputs[k+1].is_const))
	    {
	    ext = fg->nodes[sgen_id].inputs[0].ty.dim_sizes[j];
	    wsize = get_int_const (fg->nodes[sgen_id].inputs[k].constval);
	    assert (wsize != 0);
	    step = get_int_const (fg->nodes[sgen_id].inputs[k+1].constval);
	    assert (step != 0);
	    val = ext-wsize<0?0:(ext-wsize)/step+1;
	    size_write (szs, val, i);
	    }
	i++;
	k+=2;
	}
    
    return i;
    }

int scalar_gen_loop_dimensions (FuncGraph *fg, int sgen_id, int i, int *szs)
    {
    int j, k, lo, hi, step, val;

    for (j=0, k=0; j<fg->nodes[sgen_id].num_outputs; j++)
        {
	if ((fg->nodes[sgen_id].inputs[k].is_const) &&
			(fg->nodes[sgen_id].inputs[k+1].is_const) &&
			(fg->nodes[sgen_id].inputs[k+2].is_const))
	    {
	    lo = get_int_const (fg->nodes[sgen_id].inputs[k].constval);
	    hi = get_int_const (fg->nodes[sgen_id].inputs[k+1].constval);
	    step = get_int_const (fg->nodes[sgen_id].inputs[k+2].constval);
	    val = hi-lo<0?0:1+(hi-lo)/step;
	    size_write (szs, val, i);
	    }
	i++;
	k += 3;
	}

    return i;
    }

/*  we can infer a loop extent if we have information at the output of the
 *  ND_CONSTRUCT_ARRAY node; the output rank is guaranteed to be larger than
 *  the input rank;
 */
void construct_array_loop_dimensions (FuncGraph *fg, int ret_id, int *szs)
    {
    int in_rank, out_rank, i, val;

    if (fg->nodes[ret_id].inputs[0].ty.kind == Array)
        in_rank = fg->nodes[ret_id].inputs[0].ty.dims;
    else
        in_rank = 0;

    out_rank = fg->nodes[ret_id].outputs[0].ty.dims;

    for (i=0; i<out_rank-in_rank; i++)
        if (fg->nodes[ret_id].outputs[0].ty.dim_sizes[i] >= 0)
	    {
	    val = fg->nodes[ret_id].outputs[0].ty.dim_sizes[i];
	    size_write (szs, val, i);
	    }
    }

/*  for a ND_CONSTRUCT_CONCAT, we're guaranteed that the loop rank is one;
 *  we can infer its size if we have a rightmost extent on both input and
 *  output; if the input extent is zero, we can't infer anything about the
 *  loop extent;
 */
void construct_concat_loop_dimensions (FuncGraph *fg, int ret_id, int *szs)
    {
    int in_rank, out_rank;
    int in_extent, out_extent, loop_extent;

    in_rank = fg->nodes[ret_id].inputs[0].ty.dims;
    out_rank = fg->nodes[ret_id].outputs[0].ty.dims;

    in_extent = fg->nodes[ret_id].inputs[0].ty.dim_sizes[in_rank-1];
    out_extent = fg->nodes[ret_id].outputs[0].ty.dim_sizes[out_rank-1];

    if ((in_extent >= 0) && (out_extent >= 0))
	{
	if (in_extent == 0)
	    {
	    if (out_extent > 0)
	        handle_error (ErrSizePropSizeMismatch, Fatal, 0, NULL, NULL);
	    }
	else if (out_extent % in_extent > 0)
	    handle_error (ErrSizePropSizeMismatch, Fatal, 0, NULL, NULL);
	else
	    {
	    loop_extent = out_extent / in_extent;
	    size_write (szs, loop_extent, 0);
	    }
	}
    }

void construct_tile_loop_dimensions (FuncGraph *fg, int ret_id, int loop_rank, int *szs)
    {
    int in_rank, out_rank, i, j, k;
    int in_extent, out_extent, loop_extent;

    in_rank = fg->nodes[ret_id].inputs[0].ty.dims;
    out_rank = fg->nodes[ret_id].outputs[0].ty.dims;

    for (i=in_rank-1, j=out_rank-1, k=loop_rank-1; i>=0 && j>=0 && k>=0; i--, j--, k--)
        {
	in_extent = fg->nodes[ret_id].inputs[0].ty.dim_sizes[i];
	out_extent = fg->nodes[ret_id].outputs[0].ty.dim_sizes[j];

	if ((in_extent >= 0) && (out_extent >= 0))
	    {
	    if (in_extent == 0)
	        {
		if (out_extent > 0)
		    handle_error (ErrSizePropSizeMismatch, Fatal, 0, NULL, NULL);
		}
	    else if (out_extent % in_extent > 0)
	        handle_error (ErrSizePropSizeMismatch, Fatal, 0, NULL, NULL);
	    else
	        {
	        loop_extent = out_extent / in_extent;
		size_write (szs, loop_extent, k);
	        }
	    }
	}
    }

void size_transfer (TypeInfo *t0, TypeInfo *t1)
    {
    int i;

    for (i=0; i<t1->dims; i++)
        if (t1->dim_sizes[i] >= 0)
	    size_write (t0->dim_sizes, t1->dim_sizes[i], i);
    }

void size_write (int *ds0, int val, int i)
    {
    if (ds0[i] == -1)
	{
	size_prop_changed = TRUE;
        ds0[i] = val;
	}
    else if (ds0[i] != val)
        {
	handle_error (ErrSizePropSizeMismatchVals, Fatal, 0, NULL, NULL, i, ds0[i], val);
	}
    }

int get_int_const (char *s)
    {
    int val;

    sscanf (s, "%d", &val);
    return val;
    }
