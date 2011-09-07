#include "sassy.h"
#include "ddcf.h"
#include "error.h"

/*
#define PRINT(s) {printf (s); fflush (stdout);}
*/
#define PRINT(s)

void size_propagate_node_up (FuncGraph*, int, int*);
void size_propagate_list_bkwds (FuncGraph*, IntList*, int*);
void transfer_through_outputs_up (FuncGraph*, int);
void transfer_through_inputs_up (FuncGraph*, int);
int size_propagate_elegen_up (FuncGraph*, int, int*, int);
int size_propagate_slicegen_up (FuncGraph*, int, int*, int);
int size_propagate_windowgen_up (FuncGraph*, int, int*, int);
void size_propagate_forall_up (FuncGraph*, int, int*);
void size_propagate_cross_prod_up (FuncGraph*, int, int*);
void size_propagate_dot_prod_up (FuncGraph*, int, int*);
void process_loop_returns_up (FuncGraph*, int, int, int*);
void propagate_upward (FuncGraph*, int, int);
void size_transfer (TypeInfo*, TypeInfo*);
void size_write (int*, int, int);
int get_int_const (char*);
int find_generator_graph (FuncGraph*, int);
int get_loop_dimensions (FuncGraph*, int, int*);

void size_propagate_up ()
    {
    FuncGraph *fg;
    TypeInfo *tt;
    int i;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
	{
	if (fg->nodes_used > 0)
	    {
	    transfer_through_outputs_up (fg, 0);
	    size_propagate_list_bkwds (fg, fg->nodes[0].My_nodes, NULL);
	    transfer_through_inputs_up (fg, 0);

	    for (tt=fg->params, i=0; tt!=NULL; tt=tt->link, i++)
		size_transfer (tt, &(fg->nodes[0].inputs[i].ty));
	    }
	}
    }

void size_propagate_list_bkwds (FuncGraph *fg, IntList *it, int *loop_extents)
    {
    if (it == NULL)
        return;

    size_propagate_list_bkwds (fg, it->link, loop_extents);
    size_propagate_node_up (fg, it->val, loop_extents);
    }

void size_propagate_node_up (FuncGraph *fg, int id, int *loop_extents)
    {
    int i;

    switch (fg->nodes[id].nodetype)
        {
	case ND_CASE :
	    transfer_through_outputs_up (fg, id);
	    size_propagate_list_bkwds (fg, fg->nodes[id].My_nodes, loop_extents);
	    transfer_through_inputs_up (fg, id);

	    /*  here we *don't* propagate up to the sources, because
	     *  an input size for one case is not necessarily the same
	     *  as it is for another case
	     */
	    break;
	case ND_FORALL :
	    size_propagate_forall_up (fg, id, loop_extents);
	    break;
	case ND_FUNC :
	case ND_WHILE_PRED :
	case ND_SWITCH :
	case ND_WHILE :
	    transfer_through_outputs_up (fg, id);
	    size_propagate_list_bkwds (fg, fg->nodes[id].My_nodes, loop_extents);
	    transfer_through_inputs_up (fg, id);

	    for (i=0; i<fg->nodes[id].num_inputs; i++)
		if (fg->nodes[id].inputs[i].ty.kind == Array)
	            propagate_upward (fg, id, i);
	    break;
	case ND_FORNXT :
	    transfer_through_outputs_up (fg, id);
	    size_propagate_list_bkwds (fg, fg->nodes[id].My_nodes, fg->nodes[id].dim_sizes);
	    transfer_through_inputs_up (fg, id);

	    for (i=0; i<fg->nodes[id].num_inputs; i++)
		if (fg->nodes[id].inputs[i].ty.kind == Array)
	            propagate_upward (fg, id, i);
	    break;
	case ND_DOT_PROD :
	    size_propagate_dot_prod_up (fg, id, loop_extents);
	    break;
	case ND_CROSS_PROD :
	    size_propagate_cross_prod_up (fg, id, loop_extents);
	    break;
	case ND_ADD :
	case ND_SUB :
	case ND_MUL :
	case ND_DIV :
	case ND_MOD :
	case ND_COMPLEX :
	case ND_LT :
	case ND_GT :
	case ND_LE :
	case ND_GE :
	case ND_NEQ :
	case ND_EQ :
	case ND_AND :
	case ND_OR :
	case ND_BIT_AND :
	case ND_BIT_OR :
	case ND_BIT_EOR :
	case ND_LEFT_SHIFT_MACH :
	case ND_LEFT_SHIFT :
	case ND_RIGHT_SHIFT :
	case ND_SCALAR_GEN :
	case ND_REAL :
	case ND_IMAG :
	case ND_NOT :
	case ND_NEG :
	case ND_INTRINCALL :
	case ND_SWITCH_KEY :
	case ND_SELECTORS :
	case ND_PRINT :
	case ND_ASSERT :
	case ND_LOOP_INDICES :
	case ND_REDUCE_SUM :
	case ND_REDUCE_MIN :
	case ND_REDUCE_MAX :
	case ND_REDUCE_AND :
	case ND_REDUCE_OR :
	case ND_REDUCE_PRODUCT :
	case ND_REDUCE_MEAN :
	case ND_REDUCE_ST_DEV :
	case ND_REDUCE_MEDIAN :
	case ND_EXTENTS :
	    break;
	case ND_ELE_GEN :
	case ND_SLICE_GEN :
	case ND_WINDOW_GEN :
	    break;
	case ND_REDUCE_VAL_AT_FIRST_MAX :
	case ND_REDUCE_VAL_AT_FIRST_MIN :
	case ND_REDUCE_VAL_AT_LAST_MAX :
	case ND_REDUCE_VAL_AT_LAST_MIN :
	case ND_REDUCE_VAL_AT_MAXS :
	case ND_REDUCE_VAL_AT_MINS :
	case ND_REDUCE_MODE :
	case ND_REDUCE_HIST :
	case ND_CONSTRUCT_ARRAY :
	case ND_CONSTRUCT_CONCAT :
	case ND_CONSTRUCT_CONCAT_MASKED :
	case ND_CONSTRUCT_TILE :
	case ND_ACCUM_SUM :
	case ND_ACCUM_MIN :
	case ND_ACCUM_MAX :
	case ND_ACCUM_AND :
	case ND_ACCUM_OR :
	case ND_ACCUM_PRODUCT :
	case ND_ACCUM_MEAN :
	case ND_ACCUM_ST_DEV :
	case ND_ACCUM_MEDIAN :
	case ND_ACCUM_HIST :
	    break;
	case ND_G_INPUT_NEXT :
	case ND_G_INPUT :
	    break;
	case ND_G_OUTPUT :
	    if (fg->nodes[id].inputs[0].ty.kind == Array)
	        propagate_upward (fg, id, 0);
	    break;
	case ND_ARR_DEF :
	    break;
	case ND_CAST :
	case ND_FEED_NEXT :
	    size_transfer (&(fg->nodes[id].inputs[0].ty),
	    			&(fg->nodes[id].outputs[0].ty));
	    propagate_upward (fg, id, 0);
	    break;
	case ND_ARR_CONCAT :
	    {
	    int sz;

	    for (i=0; i<fg->nodes[id].outputs[0].ty.dims-1; i++)
	        if (fg->nodes[id].outputs[0].ty.dim_sizes[i] >= 0)
		    {
		    size_write (fg->nodes[id].inputs[0].ty.dim_sizes,
					fg->nodes[id].outputs[0].ty.dim_sizes[i], i);
		    size_write (fg->nodes[id].inputs[1].ty.dim_sizes,
					fg->nodes[id].outputs[0].ty.dim_sizes[i], i);
		    }

	    if (fg->nodes[id].outputs[0].ty.dim_sizes[i] >= 0)
	        {
		if (fg->nodes[id].inputs[0].ty.dim_sizes[i] >= 0)
		    {
		    sz = fg->nodes[id].outputs[0].ty.dim_sizes[i] -
		    		fg->nodes[id].inputs[0].ty.dim_sizes[i];
		    size_write (fg->nodes[id].inputs[1].ty.dim_sizes, sz, i);
		    }
		else if (fg->nodes[id].inputs[1].ty.dim_sizes[i] >= 0)
		    {
		    sz = fg->nodes[id].outputs[0].ty.dim_sizes[i] -
		    		fg->nodes[id].inputs[1].ty.dim_sizes[i];
		    size_write (fg->nodes[id].inputs[0].ty.dim_sizes, sz, i);
		    }
		}

	    propagate_upward (fg, id, 0);
	    propagate_upward (fg, id, 1);
	    }
	    break;
	case ND_FCALL :
	    for (i=0; i<fg->nodes[id].num_inputs; i++)
	        if (fg->nodes[id].inputs[i].ty.kind == Array)
		    propagate_upward (fg, id, i);
	    break;
	case ND_ARR_CONPERIM :
	    if (fg->nodes[id].inputs[1].is_const)
		{
		int val = get_int_const (fg->nodes[id].inputs[1].constval);

	        for (i=0; i<fg->nodes[id].outputs[0].ty.dims; i++)
		    if (fg->nodes[id].outputs[0].ty.dim_sizes[i] >= 0)
			{
			int sz = fg->nodes[id].outputs[0].ty.dim_sizes[i] - 2*val;

			if (sz < 0)
		    	    handle_error (ErrSizePropConPerimLessThanZero,
			    				Fatal, 0, NULL, NULL);
			else
			    {
			    size_write (fg->nodes[id].inputs[0].ty.dim_sizes, sz, i);
			    }
			}
		}

	    propagate_upward (fg, id, 0);
	    break;
	case ND_ARRAYREF :
	    break;
	case ND_NEXT :
	    break;
	default :
            break;
	}
    }

/* 'id' is the ND_DOT_PROD node */
void size_propagate_dot_prod_up (FuncGraph *fg, int id, int *loop_extents)
    {
    IntList *it;
    int pos, i;

    transfer_through_outputs_up (fg, id);

    for (it=fg->nodes[id].My_nodes, pos=0; it!=NULL; it=it->link)
        switch (fg->nodes[it->val].nodetype)
	    {
	    case ND_ELE_GEN :
		size_propagate_elegen_up (fg, it->val, loop_extents, 0);
	        break;
	    case ND_SLICE_GEN :
		size_propagate_slicegen_up (fg, it->val, loop_extents, 0);
	        break;
	    case ND_WINDOW_GEN :
		size_propagate_windowgen_up (fg, it->val, loop_extents, 0);
	        break;
	    default :
                break;
	    }

    size_propagate_list_bkwds (fg, fg->nodes[id].My_nodes, loop_extents);
    transfer_through_inputs_up (fg, id);

    for (i=0; i<fg->nodes[id].num_inputs; i++)
	if (fg->nodes[id].inputs[i].ty.kind == Array)
            propagate_upward (fg, id, i);
    }

/* 'id' is the ND_CROSS_PROD node */
void size_propagate_cross_prod_up (FuncGraph *fg, int id, int *loop_extents)
    {
    IntList *it;
    int pos, i;

    transfer_through_outputs_up (fg, id);

    for (it=fg->nodes[id].My_nodes, pos=0; it!=NULL; it=it->link)
        switch (fg->nodes[it->val].nodetype)
	    {
	    case ND_ELE_GEN :
		pos = size_propagate_elegen_up (fg, it->val, loop_extents, pos);
	        break;
	    case ND_SLICE_GEN :
		pos = size_propagate_slicegen_up (fg, it->val, loop_extents, pos);
	        break;
	    case ND_WINDOW_GEN :
		pos = size_propagate_windowgen_up (fg, it->val, loop_extents, pos);
	        break;
	    case ND_SCALAR_GEN :
	        pos += fg->nodes[it->val].num_outputs;
		break;
	    default :
                break;
	    }

    size_propagate_list_bkwds (fg, fg->nodes[id].My_nodes, loop_extents);
    transfer_through_inputs_up (fg, id);

    for (i=0; i<fg->nodes[id].num_inputs; i++)
	if (fg->nodes[id].inputs[i].ty.kind == Array)
            propagate_upward (fg, id, i);
    }

/*  'nd' is a ND_ELE_GEN node; for each dimension of the input array,
 *  we can infer its size providing:
 *	- if the step size is 1, the input extent comes
 *	  from the loop extent
 */
int size_propagate_elegen_up (FuncGraph *fg, int nd, int *loop_extents, int pos)
    {
    int rank, i, j, k, val;

    /*  'i' is the index into the input extents
     *  'j' is the index into the step inputs
     *  'k' is the index into output extents
     *  'pos' is the index into the loop's extents
     */

    assert (loop_extents != NULL);

    rank = fg->nodes[nd].inputs[0].ty.dims;

    for (i=0, j=1, k=0; i<rank; i++)
      {
	if (fg->nodes[nd].inputs[j].is_const)
	  {
	    val = get_int_const (fg->nodes[nd].inputs[j].constval);
	    if ((val == 1) && (loop_extents[pos] >= 0))
	      {
		size_write (fg->nodes[nd].inputs[0].ty.dim_sizes, loop_extents[pos], i);
	      }
	  }
	j++;
	pos++;
      }
    
    propagate_upward (fg, nd, 0);

    return pos;
    }

/*  'nd' is a ND_SLICE_GEN node; for each dimension of the input array,
 *  we can infer its size providing:
 *	- if it's a ':' dimension, the input extent comes from the output extent
 *	- if it's a '~' dimension and the step size is 1, the input extent comes
 *	  from the loop extent
 */
int size_propagate_slicegen_up (FuncGraph *fg, int nd, int *loop_extents, int pos)
    {
    int rank, i, j, k, val;

    /*  'i' is the index into the input extents
     *  'j' is the index into the step inputs
     *  'k' is the index into output extents
     *  'pos' is the index into the loop's extents
     */

    assert (loop_extents != NULL);

    rank = fg->nodes[nd].inputs[0].ty.dims;

    for (i=0, j=1, k=0; i<rank; i++)
        if (fg->nodes[nd].Reftypes[i] == ':')
	    {
	    val = fg->nodes[nd].outputs[0].ty.dim_sizes[k];
	    if (val >= 0)
		{
	        size_write (fg->nodes[nd].inputs[0].ty.dim_sizes, val, i);
		}
	    k++;
	    }
	else
	    {
	    if (fg->nodes[nd].inputs[j].is_const)
		{
		val = get_int_const (fg->nodes[nd].inputs[j].constval);
		if ((val == 1) && (loop_extents[pos] >= 0))
		    {
		    size_write (fg->nodes[nd].inputs[0].ty.dim_sizes, loop_extents[pos], i);
		    }
		}
	    j++;
	    pos++;
	    }

    propagate_upward (fg, nd, 0);

    return pos;
    }

/*  we can infer an input extent only if:
 *	- window size is const, and
 *	- step size == 1, and
 *	- loop extent > 0
 */
int size_propagate_windowgen_up (FuncGraph *fg, int nd, int *loop_extents, int pos)
    {
    int i, j, rank, step, ext;

    rank = fg->nodes[nd].inputs[0].ty.dims;

    for (i=0, j=2; i<rank; i++, j+=2, pos++)
        if (fg->nodes[nd].outputs[0].ty.dim_sizes[i] >= 0)
	    if (fg->nodes[nd].inputs[j].is_const)
	        {
		step = get_int_const (fg->nodes[nd].inputs[j].constval);
		if ((step == 1) && (loop_extents[pos] > 0))
		    {
		    ext = loop_extents[pos] + fg->nodes[nd].outputs[0].ty.dim_sizes[i] - 1;
		    size_write (fg->nodes[nd].inputs[0].ty.dim_sizes, ext, i);
		    }
		}

    propagate_upward (fg, nd, 0);

    return pos;
    }

/* for graph nodes only: transfer size info from the outputs of ND_G_INPUT
 * nodes to the input ports of the enclosing graph
 */
void transfer_through_inputs_up (FuncGraph *fg, int id)
    {
    int i, in_node;

    for (i=0; i<fg->nodes[id].num_inputs; i++)
	if (fg->nodes[id].inputs[i].ty.kind == Array)
	    {
	    in_node = fg->nodes[id].My_inputs[i];
	    size_transfer (&(fg->nodes[id].inputs[i].ty),
	    		&(fg->nodes[in_node].outputs[0].ty));
	    }
    }

/* for graph nodes only: transfer size info from the output ports of
 * the graph to the inputs of the ND_G_OUTPUT nodes of the graph,
 * and then up to the sources of the edges feeding the ND_G_OUTPUT nodes
 */
void transfer_through_outputs_up (FuncGraph *fg, int id)
    {
    int i, out_node;

    for (i=0; i<fg->nodes[id].num_outputs; i++)
        if (fg->nodes[id].outputs[i].ty.kind == Array)
	    {
	    out_node = fg->nodes[id].My_outputs[i];
	    size_transfer (&(fg->nodes[out_node].inputs[0].ty),
	    		&(fg->nodes[id].outputs[i].ty));
            propagate_upward (fg, out_node, 0);
	    }
    }

void size_propagate_forall_up (FuncGraph *fg, int id, int *loop_extents)
    {
    int i, rank;

    transfer_through_outputs_up (fg, id);

    /* determine loop dimensions where possible */
    rank = get_loop_dimensions (fg, id, fg->nodes[id].dim_sizes);

    process_loop_returns_up (fg, id, rank, fg->nodes[id].dim_sizes);

    size_propagate_list_bkwds (fg, fg->nodes[id].My_nodes, fg->nodes[id].dim_sizes);

    transfer_through_inputs_up (fg, id);

    for (i=0; i<fg->nodes[id].num_inputs; i++)
        if (fg->nodes[id].inputs[i].ty.kind == Array)
            propagate_upward (fg, id, i);
    }

void process_loop_returns_up (FuncGraph *fg, int id, int loop_rank, int *loop_extents)
    {
    IntList *it;
    int nd, i, j;
    int in_rank, out_rank;
    int in_extent, out_extent, loop_extent;
    int val;

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
	{
	nd = it->val;

        switch (fg->nodes[nd].nodetype)
	    {
	    case ND_CONSTRUCT_ARRAY :
		in_rank = fg->nodes[nd].inputs[0].ty.dims;
		out_rank = fg->nodes[nd].outputs[0].ty.dims;

		if (fg->nodes[nd].inputs[0].ty.kind == Array)
		    for (i=loop_rank, j=0; i<out_rank; i++, j++)
		        {
		        out_extent = fg->nodes[nd].outputs[0].ty.dim_sizes[i];

		        if (out_extent >= 0)
			    {
			    size_write (fg->nodes[nd].inputs[0].ty.dim_sizes, out_extent, j);
			    }
		        }

		propagate_upward (fg, nd, 0);
	        break;
	    case ND_CONSTRUCT_CONCAT :
		/*  we can't propagate the rightmost extent upward because component
		 *  sizes can vary from iteration to iteration in the CONCAT operator
		 */ 

		in_rank = fg->nodes[nd].inputs[0].ty.dims;
		out_rank = fg->nodes[nd].outputs[0].ty.dims;

		for (i=0; i<in_rank-1; i++)
		    {
		    out_extent = fg->nodes[nd].outputs[0].ty.dim_sizes[i];

		    if (out_extent >= 0)
			{
		        size_write (fg->nodes[nd].inputs[0].ty.dim_sizes, out_extent, i);
			}
		    }

		propagate_upward (fg, nd, 0);
	        break;
	    case ND_CONSTRUCT_TILE :
		in_rank = fg->nodes[nd].inputs[0].ty.dims;
		out_rank = fg->nodes[nd].outputs[0].ty.dims;

		if (loop_rank == out_rank)
		    {
		    for (i=in_rank-1, j=out_rank-1; i>=0; i--, j--)
		        {
			in_extent = fg->nodes[nd].inputs[0].ty.dim_sizes[i];
			out_extent = fg->nodes[nd].outputs[0].ty.dim_sizes[j];
			loop_extent = loop_extents[j];

			if ((loop_extent > 0) && (out_extent >= 0))
			    {
			    if (out_extent % loop_extent != 0)
				handle_error (ErrSizePropSizeMismatch, 
				    				Fatal, 0, NULL, NULL);
			    else
			        {
				val = out_extent / loop_extent;
				size_write (fg->nodes[nd].inputs[0].ty.dim_sizes, val, i);
				}
			    }
		        }

		    for (i=0; i<out_rank - in_rank; i++)
		        if (loop_extents[i] >= 0)
			    {
			    size_write (fg->nodes[nd].outputs[0].ty.dim_sizes, loop_extents[i], i);
			    }
		    }
		else
		    {
		    for (i=in_rank-1, j=loop_rank-1; j>=0; i--, j--)
		        {
			in_extent = fg->nodes[nd].inputs[0].ty.dim_sizes[i];
			out_extent = fg->nodes[nd].outputs[0].ty.dim_sizes[i];
			loop_extent = loop_extents[j];

			if ((loop_extent > 0) && (out_extent >= 0))
			    {
			    if (out_extent % loop_extent != 0)
				handle_error (ErrSizePropSizeMismatch, 
				    				Fatal, 0, NULL, NULL);
			    else
			        {
				val = out_extent / loop_extent;
				size_write (fg->nodes[nd].inputs[0].ty.dim_sizes, val, i);
				}
			    }
		        }

		    for (i=0; i<out_rank - loop_rank; i++)
		        if (fg->nodes[nd].outputs[0].ty.dim_sizes[i] >= 0)
			    {
			    size_write (fg->nodes[nd].inputs[0].ty.dim_sizes,
			    		fg->nodes[nd].outputs[0].ty.dim_sizes[i], i);
			    }
		    }

		propagate_upward (fg, nd, 0);
	        break;
	    default :
                break;
	    }
	}
    }


/* follow back edges from an input port to the sources, propagating size
 * info to those sources
 */
void propagate_upward (FuncGraph *fg, int id, int inport)
    {
    Edge *bk;
    int node, port;

    for (bk=fg->nodes[id].inputs[inport].back_edges; bk!=NULL; bk=bk->link)
	{
	node = bk->node;
	port = bk->port;
	size_transfer (&(fg->nodes[node].outputs[port].ty),
    				&(fg->nodes[id].inputs[inport].ty));
	}
    }
