#include "sassy.h"
#include "ddcf.h"
#include "error.h"

void aref_elide_process_graphs_nodes (FuncGraph*, int);
void aref_elide_node (FuncGraph*, int, IntList**);
void dup_input (FuncGraph*, int, int, int, int);
int make_add (FuncGraph*, InputPort, InputPort, Location, IntList**, int);
int make_sub (FuncGraph*, InputPort, InputPort, Location, IntList**, int);
int make_mult (FuncGraph*, InputPort, InputPort, Location, IntList**, int);
int make_div (FuncGraph*, InputPort, InputPort, Location, IntList**, int);
int make_mult_add (FuncGraph*, InputPort, InputPort, InputPort, Location, IntList**, int);
int make_extent (FuncGraph *fg, InputPort, InputPort, InputPort, Location, IntList**, int);
void make_composite_type (TypeInfo*, TypeInfo*, Type*, int*, int*);
int bit_width (int);

void arrayref_elision ()
    {
    FuncGraph *fg;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
	    aref_elide_process_graphs_nodes (fg, 0);
    }

void aref_elide_process_graphs_nodes (FuncGraph *fg, int id)
    {
    IntList *it, *cpy;

    copy_intlist (&cpy, &(fg->nodes[id].My_nodes));

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        aref_elide_node (fg, it->val, &cpy);

    free_intlist (&(fg->nodes[id].My_nodes));
    fg->nodes[id].My_nodes = cpy;
    }

void aref_elide_node (FuncGraph *fg, int id, IntList **itlist)
    {
    InputPort *old_inputs;
    int snd, spt, i, j, k, m, n, old_in_cnt, cmp0, cmp1, cmp2, cast;
    int src_rank, mid_rank, out_rank, new_in_cnt, and, apt;
    char *old_pattern;

    if (!is_simple_node (fg, id))
        aref_elide_process_graphs_nodes (fg, id);
    else if (fg->nodes[id].nodetype == ND_ARRAYREF)
	{
	/* find its source node */
	assert (fg->nodes[id].inputs[0].back_edges != NULL);
	snd = fg->nodes[id].inputs[0].back_edges->node;
	spt = fg->nodes[id].inputs[0].back_edges->port;

	if (fg->nodes[snd].nodetype == ND_ARRAYREF)
	    {
	    is_stable = FALSE;

	    src_rank = fg->nodes[snd].inputs[0].ty.dims;
	    mid_rank = fg->nodes[snd].outputs[0].ty.dims;
	    if (fg->nodes[id].outputs[0].ty.kind == Array)
	        out_rank = fg->nodes[id].outputs[0].ty.dims;
	    else
	        out_rank = 0;

	    /* save the old inputs for reference */
	    old_inputs = fg->nodes[id].inputs;
	    old_pattern = fg->nodes[id].Reftypes;
	    old_in_cnt = fg->nodes[id].num_inputs;

	    /* determine the number of inputs needed */
	    new_in_cnt = 1 + src_rank + 2*out_rank;
	    fg->nodes[id].inputs = make_input_ports (new_in_cnt);
	    SacMalloc (fg->nodes[id].Reftypes, char*, (1+src_rank) * sizeof (char))

	    fg->nodes[id].num_inputs = new_in_cnt;

	    assert (fg->nodes[snd].inputs[0].back_edges != NULL);
	    and = fg->nodes[snd].inputs[0].back_edges->node;
	    apt = fg->nodes[snd].inputs[0].back_edges->port;

	    /* first input is fed by <and.apt> */
	    add_edge (fg, and, apt, id, 0);
	    copy_type_info (&(fg->nodes[id].inputs[0].ty), &(fg->nodes[and].outputs[apt].ty));

	    /*  'i' indexes into 'snd's inputs
	     *  'j' indexes into new 'id's inputs
	     *  'n' indexes into old 'id's inputs
	     *  'k' indexes along 'snd's dimensions
	     *  'm' indexes along old 'id's dimensions
	     */
	    for (i=1, j=1, n=1, k=0, m=0; k<src_rank; k++)
		if (fg->nodes[snd].Reftypes[k] == '_')
		    {
		    dup_input (fg, id, j, snd, i);
		    fg->nodes[id].Reftypes[k] = '_';
		    i++;
		    j++;
		    }
		else
		    {
		    if (old_pattern[m] == '_')
		        {
			cmp0 = make_mult_add (fg, old_inputs[n], fg->nodes[snd].inputs[i+2],
					fg->nodes[snd].inputs[i], fg->nodes[snd].loc, itlist, id);
			add_edge (fg, cmp0, 0, id, j);
			copy_type_info (&(fg->nodes[id].inputs[j].ty), &(fg->nodes[cmp0].outputs[0].ty));
		        fg->nodes[id].Reftypes[k] = '_';
			i += 3;
			j++;
			n++;
			}
		    else
		        {
			cmp0 = make_mult_add (fg, old_inputs[n], fg->nodes[snd].inputs[i+2],
						fg->nodes[snd].inputs[i], fg->nodes[snd].loc, itlist, id);
			add_edge (fg, cmp0, 0, id, j);
			copy_type_info (&(fg->nodes[id].inputs[j].ty), &(fg->nodes[cmp0].outputs[0].ty));

			cmp1 = make_mult_add (fg, old_inputs[n+1], fg->nodes[snd].inputs[i+2],
						fg->nodes[snd].inputs[i], fg->nodes[snd].loc, itlist, id);
			add_edge (fg, cmp1, 0, id, j+1);
			copy_type_info (&(fg->nodes[id].inputs[j+1].ty), &(fg->nodes[cmp1].outputs[0].ty));

			cmp2 = make_mult (fg, old_inputs[n+2],
						fg->nodes[snd].inputs[i+2], fg->nodes[snd].loc, itlist, id);
			add_edge (fg, cmp2, 0, id, j+2);
			copy_type_info (&(fg->nodes[id].inputs[j+2].ty), &(fg->nodes[cmp2].outputs[0].ty));

		        fg->nodes[id].Reftypes[k] = ':';

			i += 3;
			j += 3;
			n += 3;
			}
		    m++;
		    }

	    for (i=0; i<old_in_cnt; i++)
	        if (!old_inputs[i].is_const)
		    {
		    remove_targets_to_this_port (fg, old_inputs[i].back_edges, id, i);
		    free_edge_list (&(old_inputs[i].back_edges));
		    }

	    SacFree (old_inputs)
	    SacFree (old_pattern)
	    }
	}
    else if (fg->nodes[id].nodetype == ND_EXTENTS)
        {
	/* find its source node */
	assert (fg->nodes[id].inputs[0].back_edges != NULL);
	snd = fg->nodes[id].inputs[0].back_edges->node;
	spt = fg->nodes[id].inputs[0].back_edges->port;

	if (fg->nodes[snd].nodetype == ND_ARRAYREF)
	    {
	    is_stable = FALSE;

	    src_rank = fg->nodes[snd].inputs[0].ty.dims;

	    /*  'i' is source array dimension
	     *  'j' is ND_EXTENTS node's output index
	     *  'k' is index to the inputs of the source node 'snd'
	     */
	    for (i=0, j=0, k=1; i<src_rank; i++)
	        if (fg->nodes[snd].Reftypes[i] == ':')
		    {
		    cmp0 = make_extent (fg, fg->nodes[snd].inputs[k], fg->nodes[snd].inputs[k+1],
		    		fg->nodes[snd].inputs[k+2], fg->nodes[snd].loc, itlist, id);

		    if (! type_equal (&(fg->nodes[id].outputs[j].ty), &(fg->nodes[cmp0].outputs[0].ty)))
		        {
			cast = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[snd].loc);
			insert_val_after_in_intlist (cast, cmp0, itlist);
			add_edge (fg, cmp0, 0, cast, 0);
			copy_type_info (&(fg->nodes[cast].inputs[0].ty), &(fg->nodes[cmp0].outputs[0].ty));
			cmp0 = cast;
			}

		    copy_type_info (&(fg->nodes[cmp0].outputs[0].ty), &(fg->nodes[id].outputs[j].ty));
		    fg->nodes[cmp0].outputs[0].targets = fg->nodes[id].outputs[j].targets;
		    fg->nodes[id].outputs[j].targets = NULL;
		    retarget_backedges (fg, fg->nodes[cmp0].outputs[0].targets, id, j, cmp0, 0);
		    k += 3;
		    }
		else
		    k++;
		    
	    }
	}
    }

void dup_input (FuncGraph *fg, int tgt_nd, int tgt_pt, int src_nd, int src_pt)
    {
    int xnd, xpt;

    if (fg->nodes[src_nd].inputs[src_pt].is_const)
        {
	fg->nodes[tgt_nd].inputs[tgt_pt].is_const = TRUE;
	fg->nodes[tgt_nd].inputs[tgt_pt].constval = copy_to_const (fg->nodes[src_nd].inputs[src_pt].constval);
	}
    else
        {
	assert (fg->nodes[src_nd].inputs[src_pt].back_edges != NULL);
	fg->nodes[tgt_nd].inputs[tgt_pt].is_const = FALSE;
	xnd = fg->nodes[src_nd].inputs[src_pt].back_edges->node;
	xpt = fg->nodes[src_nd].inputs[src_pt].back_edges->port;
	add_edge (fg, xnd, xpt, tgt_nd, tgt_pt);
	}
    copy_type_info (&(fg->nodes[tgt_nd].inputs[tgt_pt].ty), &(fg->nodes[src_nd].inputs[src_pt].ty));
    }

int make_add (FuncGraph *fg, InputPort in0, InputPort in1, Location loc, IntList **itlist, int ref)
    {
    int res, cast, snd, spt, val0, val1, width0, width1, width;
    Type ty;
    int tsz, fsz;

    /*  first determine the width of the output; its width will be one plus
     *  the max of the input widths; one of the two inputs must be cast this
     *  wide to force the add to have sufficient bits
     */
    if (in0.is_const)
        {
	sscanf (in0.constval, "%d", &val0);
	width0 = bit_width (val0);
	if (in0.ty.type == Int)
	    width0++;
	}
    else
	width0 = in0.ty.totsize;

    if (in1.is_const)
        {
	sscanf (in1.constval, "%d", &val1);
	width1 = bit_width (val1);
	if (in1.ty.type == Int)
	    width1++;
	}
    else
	width1 = in1.ty.totsize;

    width = 1 + (width0>width1 ? width0 : width1);
    width = width>32 ? 32 : width;

    /* create a CAST and hook input 0 to it */
    cast = new_ddcf_node (fg, ND_CAST, 1, 1, loc);

    if (in0.is_const)
        {
	fg->nodes[cast].inputs[0].is_const = TRUE;
	fg->nodes[cast].inputs[0].constval = copy_to_const (in0.constval);
	}
    else
        {
	assert (in0.back_edges != NULL);
	snd = in0.back_edges->node;
	spt = in0.back_edges->port;
	add_edge (fg, snd, spt, cast, 0);
	}
    copy_type_info (&(fg->nodes[cast].inputs[0].ty), &(in0.ty));
    copy_type_info (&(fg->nodes[cast].outputs[0].ty), &(in0.ty));
    fg->nodes[cast].outputs[0].ty.totsize = width;

    res = new_ddcf_node (fg, ND_ADD, 2, 1, loc);

    add_edge (fg, cast, 0, res, 0);
    copy_type_info (&(fg->nodes[res].inputs[0].ty), &(fg->nodes[cast].outputs[0].ty));

    if (in1.is_const)
        {
	fg->nodes[res].inputs[1].is_const = TRUE;
	fg->nodes[res].inputs[1].constval = copy_to_const (in1.constval);
	}
    else
        {
	assert (in1.back_edges != NULL);
	snd = in1.back_edges->node;
	spt = in1.back_edges->port;
	add_edge (fg, snd, spt, res, 1);
	}
    copy_type_info (&(fg->nodes[res].inputs[1].ty), &(in1.ty));
    fg->nodes[res].inputs[1].ty.totsize = width1;
    copy_type_info (&(fg->nodes[res].outputs[0].ty), &(fg->nodes[res].inputs[0].ty));
    make_composite_type (&(fg->nodes[res].inputs[0].ty), &(fg->nodes[res].inputs[1].ty), &ty, &tsz, &fsz);
    fg->nodes[res].outputs[0].ty.type = ty;
    fg->nodes[res].outputs[0].ty.totsize = tsz;
    fg->nodes[res].outputs[0].ty.fracsize = fsz;

    insert_val_before_in_intlist (res, ref, itlist);
    insert_val_before_in_intlist (cast, res, itlist);

    return res;
    }

int make_sub (FuncGraph *fg, InputPort in0, InputPort in1, Location loc, IntList **itlist, int ref)
    {
    int res, cast, snd, spt, val0, val1, width0, width1, width;
    Type ty;
    int tsz, fsz;

    /*  first determine the width of the output; its width will be one plus
     *  the max of the input widths; one of the two inputs must be cast this
     *  wide to force the add to have sufficient bits
     */
    if (in0.is_const)
        {
	sscanf (in0.constval, "%d", &val0);
	width0 = bit_width (val0);
	if (in0.ty.type == Int)
	    width0++;
	}
    else
	width0 = in0.ty.totsize;

    if (in1.is_const)
        {
	sscanf (in1.constval, "%d", &val1);
	width1 = bit_width (val1);
	if (in1.ty.type == Int)
	    width1++;
	}
    else
	width1 = in1.ty.totsize;

    width = 1 + (width0>width1 ? width0 : width1);
    width = width>32 ? 32 : width;

    /* create a CAST and hook input 0 to it */
    cast = new_ddcf_node (fg, ND_CAST, 1, 1, loc);

    if (in0.is_const)
        {
	fg->nodes[cast].inputs[0].is_const = TRUE;
	fg->nodes[cast].inputs[0].constval = copy_to_const (in0.constval);
	}
    else
        {
	assert (in0.back_edges != NULL);
	snd = in0.back_edges->node;
	spt = in0.back_edges->port;
	add_edge (fg, snd, spt, cast, 0);
	}
    copy_type_info (&(fg->nodes[cast].inputs[0].ty), &(in0.ty));
    copy_type_info (&(fg->nodes[cast].outputs[0].ty), &(in0.ty));
    fg->nodes[cast].outputs[0].ty.totsize = width;

    res = new_ddcf_node (fg, ND_SUB, 2, 1, loc);

    add_edge (fg, cast, 0, res, 0);
    copy_type_info (&(fg->nodes[res].inputs[0].ty), &(fg->nodes[cast].outputs[0].ty));

    if (in1.is_const)
        {
	fg->nodes[res].inputs[1].is_const = TRUE;
	fg->nodes[res].inputs[1].constval = copy_to_const (in1.constval);
	}
    else
        {
	assert (in1.back_edges != NULL);
	snd = in1.back_edges->node;
	spt = in1.back_edges->port;
	add_edge (fg, snd, spt, res, 1);
	}
    copy_type_info (&(fg->nodes[res].inputs[1].ty), &(in1.ty));
    fg->nodes[res].inputs[1].ty.totsize = width1;
    copy_type_info (&(fg->nodes[res].outputs[0].ty), &(fg->nodes[res].inputs[0].ty));
    make_composite_type (&(fg->nodes[res].inputs[0].ty), &(fg->nodes[res].inputs[1].ty), &ty, &tsz, &fsz);
    fg->nodes[res].outputs[0].ty.type = ty;
    fg->nodes[res].outputs[0].ty.totsize = tsz;
    fg->nodes[res].outputs[0].ty.fracsize = fsz;

    insert_val_before_in_intlist (res, ref, itlist);
    insert_val_before_in_intlist (cast, res, itlist);

    return res;
    }

int make_mult (FuncGraph *fg, InputPort in0, InputPort in1, Location loc, IntList **itlist, int ref)
    {
    int res, cast, snd, spt, val0, val1, width0, width1, width;
    Type ty;
    int tsz, fsz;

    /*  first determine the width of the output; its width will be the sum
     *  of the input widths; one of the two inputs must be cast this wide
     *  to force the multiply to have sufficient bits
     */
    if (in0.is_const)
        {
	sscanf (in0.constval, "%d", &val0);
	width0 = bit_width (val0);
	if (in0.ty.type == Int)
	    width0++;
	}
    else
	width0 = in0.ty.totsize;

    if (in1.is_const)
        {
	sscanf (in1.constval, "%d", &val1);
	width1 = bit_width (val1);
	if (in1.ty.type == Int)
	    width1++;
	}
    else
	width1 = in1.ty.totsize;

    width = width0 + width1;
    width = width>32 ? 32 : width;

    /* create a CAST and hook input 0 to it */
    cast = new_ddcf_node (fg, ND_CAST, 1, 1, loc);

    if (in0.is_const)
        {
	fg->nodes[cast].inputs[0].is_const = TRUE;
	fg->nodes[cast].inputs[0].constval = copy_to_const (in0.constval);
	}
    else
        {
	assert (in0.back_edges != NULL);
	snd = in0.back_edges->node;
	spt = in0.back_edges->port;
	add_edge (fg, snd, spt, cast, 0);
	}
    copy_type_info (&(fg->nodes[cast].inputs[0].ty), &(in0.ty));
    copy_type_info (&(fg->nodes[cast].outputs[0].ty), &(in0.ty));
    fg->nodes[cast].outputs[0].ty.totsize = width;

    res = new_ddcf_node (fg, ND_MUL, 2, 1, loc);

    add_edge (fg, cast, 0, res, 0);
    copy_type_info (&(fg->nodes[res].inputs[0].ty), &(fg->nodes[cast].outputs[0].ty));

    if (in1.is_const)
        {
	fg->nodes[res].inputs[1].is_const = TRUE;
	fg->nodes[res].inputs[1].constval = copy_to_const (in1.constval);
	}
    else
        {
	assert (in1.back_edges != NULL);
	snd = in1.back_edges->node;
	spt = in1.back_edges->port;
	add_edge (fg, snd, spt, res, 1);
	}
    copy_type_info (&(fg->nodes[res].inputs[1].ty), &(in1.ty));
    fg->nodes[res].inputs[1].ty.totsize = width1;
    copy_type_info (&(fg->nodes[res].outputs[0].ty), &(fg->nodes[res].inputs[0].ty));
    make_composite_type (&(fg->nodes[res].inputs[0].ty), &(fg->nodes[res].inputs[1].ty), &ty, &tsz, &fsz);
    fg->nodes[res].outputs[0].ty.type = ty;
    fg->nodes[res].outputs[0].ty.totsize = tsz;
    fg->nodes[res].outputs[0].ty.fracsize = fsz;

    insert_val_before_in_intlist (res, ref, itlist);
    insert_val_before_in_intlist (cast, res, itlist);

    return res;
    }

int make_div (FuncGraph *fg, InputPort in0, InputPort in1, Location loc, IntList **itlist, int ref)
    {
    int res, snd, spt, val, width;
    Type ty;
    int tsz, fsz;

    res = new_ddcf_node (fg, ND_DIV, 2, 1, loc);

    if (in0.is_const)
        {
	fg->nodes[res].inputs[0].is_const = TRUE;
	fg->nodes[res].inputs[0].constval = copy_to_const (in0.constval);
	}
    else
        {
	assert (in0.back_edges != NULL);
	snd = in0.back_edges->node;
	spt = in0.back_edges->port;
	add_edge (fg, snd, spt, res, 0);
	}
    copy_type_info (&(fg->nodes[res].inputs[0].ty), &(in0.ty));

    if (in1.is_const)
        {
	fg->nodes[res].inputs[1].is_const = TRUE;
	fg->nodes[res].inputs[1].constval = copy_to_const (in1.constval);
	}
    else
        {
	assert (in1.back_edges != NULL);
	snd = in1.back_edges->node;
	spt = in1.back_edges->port;
	add_edge (fg, snd, spt, res, 1);
	}
    copy_type_info (&(fg->nodes[res].inputs[1].ty), &(in1.ty));

    if (in0.is_const && in1.is_const)
        {
	sscanf (in0.constval, "%d", &val);
	width = bit_width (val);
	if (val >= 0)
	    fg->nodes[res].inputs[0].ty.type = Uint;
	else
	    {
	    fg->nodes[res].inputs[0].ty.type = Int;
	    width++;
	    }
	fg->nodes[res].inputs[0].ty.totsize = width;

	sscanf (in1.constval, "%d", &val);
	width = bit_width (val);
	if (val >= 0)
	    fg->nodes[res].inputs[1].ty.type = Uint;
	else
	    {
	    fg->nodes[res].inputs[1].ty.type = Int;
	    width++;
	    }
	fg->nodes[res].inputs[1].ty.totsize = width;
	}

    fg->nodes[res].outputs[0].ty.kind = Scalar;
    make_composite_type (&(in0.ty), &(in1.ty), &ty, &tsz, &fsz);
    fg->nodes[res].outputs[0].ty.type = ty;
    fg->nodes[res].outputs[0].ty.totsize = tsz;
    fg->nodes[res].outputs[0].ty.fracsize = fsz;

    insert_val_before_in_intlist (res, ref, itlist);

    return res;
    }

int make_mult_add (FuncGraph *fg, InputPort in0, InputPort in1, InputPort in2, Location loc, IntList **itlist, int ref)
    {
    InputPort tmp;
    int res, mlt;

    mlt = make_mult (fg, in0, in1, loc, itlist, ref);

    tmp.is_const = FALSE;
    tmp.back_edges = malloc_backedge (mlt, 0, NULL);
    copy_type_info (&(tmp.ty), &(fg->nodes[mlt].outputs[0].ty));

    res = make_add (fg, tmp, in2, loc, itlist, ref);

    free_edge_list (&(tmp.back_edges));

    return res;
    }

int make_extent (FuncGraph *fg, InputPort in0, InputPort in1, InputPort in2, Location loc, IntList **itlist, int ref)
    {
    InputPort tmp, tmp1;
    Edge edg;
    int res, dif, sum;

    dif = make_sub (fg, in1, in0, loc, itlist, ref);

    tmp.is_const = FALSE;
    edg.node = dif;
    edg.port = 0;
    edg.link = NULL;
    tmp.back_edges = &edg;
    copy_type_info (&(tmp.ty), &(fg->nodes[dif].outputs[0].ty));

    tmp1.is_const = TRUE;
    tmp1.back_edges = NULL;
    tmp1.constval = copy_to_const ("1");
    tmp1.ty.kind = Scalar;
    tmp1.ty.type = Int;
    tmp1.ty.totsize = 32;
    tmp1.ty.fracsize = 0;

    sum = make_add (fg, tmp, tmp1, loc, itlist, ref);

    edg.node = sum;
    copy_type_info (&(tmp.ty), &(fg->nodes[sum].outputs[0].ty));

    res = make_div (fg, tmp, in2, loc, itlist, ref);

    return res;
    }
