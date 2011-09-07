#include "sassy.h"
#include "error.h"
#include "ddcf.h"

/*  This optimization stripmines a window or element generator loop. If used with the
 *  'no_unroll' pragma, it can chunk a source array into pieces, each fed to the inner
 *  loop. If used without 'no_unroll' and with moderately small chunks, the inner loop
 *  will be unrolled, creating parallel execution of loop bodies.
 *
 *  Since there may be right and bottom fringes of the source array that are not touched
 *  by the stripmined loop, these fringes have to be computed separately and then con-
 *  catenated onto the result array.
 *
 *  The main implementation challenge is the computation of the step sizes for the new
 *  outer loop, and the computation of the fringe slice indices that are needed to compute
 *  the fringes.
 *
 *  The computation of the new loop's step size is straighforward:
 *
 *	// PRAGMA (stripmine (n))
 *	... window W[v] in A step (s) ...
 *
 *		produces
 *
 *	... window WT[n] in A step (t) {
 *	     for window W[v] in WT step (s) ...
 *
 *		where	t = ((n-v+s)/s)*s	using integer arithmetic
 *
 *  The proper specification of a fringe requires knowing where the first overshooting
 *  window would start if it hadn't overshot the source array. This index is computed
 *  by
 *
 *	start_idx = (e-(((e-n)%t)+n))+t		using integer arithmetic
 *	where 'e' is the extent of the source array
 *
 *  In the 2D case, each dimension is handled as above. However, the fringes have to be
 *  handled carefully:
 *
 *	+----------------------------+----+
 *      |                            |    |
 *      |                            |    |
 *      |                            |    |
 *      |                            |    |
 *      |                            |    |
 *      |                            |    |
 *      |                            |    |
 *      |                            |    |
 *      |                            |    |
 *      |                            |    |
 *      |                            |    |
 *      |                            |    |
 *      +----------------------------+    |
 *      |                            |    |
 *      |                            |    |
 *      +----------------------------+----+
 *
 *  The right fringe is easy. Its vertical indices are the same as the source array's,
 *  and the horizontal indices start with the 'start idx' computed as above and end
 *  with the source array's last horizontal index.
 *
 *  The bottom fringe is a bit trickier. It vertical indices begin with 'start idx'
 *  computed as above and end with the source array's last vertical index. The start
 *  index in the horizontal dimension is
 *
 *	'start idx' + (v-1-s)
 *
 *  and the ending index is the same as the source array's horizontal ending index.
 */

void wt_process_graph_node (FuncGraph*, int);
int can_be_window_tiled (FuncGraph*, int, Pragma*);
void do_window_tiling (FuncGraph*, int, IntList**, int, Pragma);
int create_tiled_loop (FuncGraph*, int, IntList**, PragCell*, int, int, int*, Pragma);
void excess_1d (FuncGraph*, int, int, int, int, IntList**, int*);
void excess_2d (FuncGraph*, int, int, int, int, int, int, int, int, IntList**, int*);
void clear_dim_sizes (TypeInfo*);
void clear_dim_sizes_reduction_node (FuncGraph*, int);
void clear_dim_sizes_win_gen (FuncGraph*, int);
void clear_dim_sizes_to_win_gen (FuncGraph*, int, int);
void clear_size_info_from_outputs (FuncGraph*, int);
void clear_size_info_from_loop (FuncGraph*, int);
IntList *leftover_range (FuncGraph*, int, int, int, int, int*, int*, int*, int*);
void reduce_1d (FuncGraph*, int, int, DdcfType, int, IntList**);
void reduce_2d (FuncGraph*, int, int, int, DdcfType, DdcfType, int, IntList**);
int generators_are_different (FuncGraph *fg, int g2, int g1);

void window_tile ()
    {
    FuncGraph *fg;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
	{
	if (fg->nodes_used == 0)
	    continue;

        wt_process_graph_node (fg, 0);
	}
    }

void wt_process_graph_node (FuncGraph *fg, int nd)
    {
    IntList *it, *list_copy;
    Pragma p;
    int id;

    for (it=fg->nodes[nd].My_nodes; it!=NULL; it=it->link)
	{
	id = it->val;

	if (!is_simple_node (fg, id))
	    wt_process_graph_node (fg, id);
	}

    copy_intlist (&list_copy, &(fg->nodes[nd].My_nodes));

    for (it=fg->nodes[nd].My_nodes; it!=NULL; it=it->link)
	{
	id = it->val;

        if (fg->nodes[id].nodetype == ND_FORALL)
	    if (can_be_window_tiled (fg, id, &p))
	        do_window_tiling (fg, id, &list_copy, id, p);
	}

    free_intlist (&(fg->nodes[nd].My_nodes));
    fg->nodes[nd].My_nodes = list_copy;

    // show_ddcf_func (fg, stdout, FALSE);
    }

int can_be_window_tiled (FuncGraph *fg, int nd, Pragma *p)
    {
    PragCell *pc;
    IntList *it;
    int gen_graph, win_gen, val, cnt, id, i, j;
    char str[128];

    /* we want to do the BLOCK before STRIPMINE, so check for it first */
    if ((pc = find_pragma (fg, nd, PRAG_BLOCK)) != NULL)
	{
	*p = PRAG_BLOCK;
	strcpy (str, "block");
	}
    else if ((pc = find_pragma (fg, nd, PRAG_STRIPMINE)) != NULL)
	{
        *p = PRAG_STRIPMINE;
	strcpy (str, "stripmine");
	}
    else
        {
	return FALSE;
	}

    /* generator graph must be dot prod */
    gen_graph = find_generator_graph (fg, nd);
    if (fg->nodes[gen_graph].nodetype != ND_DOT_PROD)
	{
        return FALSE;
	}

    /* only generators with identical sizes and step are allowed */
    win_gen = -1;
    for (it=fg->nodes[gen_graph].My_nodes; it!=NULL; it=it->link)
	{
	id = it->val;

        switch (fg->nodes[id].nodetype)
	    {
	    case ND_WINDOW_GEN :
	    case ND_ELE_GEN :
		if (win_gen == -1)
		    win_gen = id;
		else
		    {
		    if (generators_are_different (fg, win_gen, id))
		        return FALSE;
		    }
		break;
	    case ND_SCALAR_GEN :
	    case ND_LOOP_INDICES :
	        return FALSE;
	    default :
                break;
	    }
	}

    /* for now, we don't handle windows of more than rank two */
    if (fg->nodes[win_gen].inputs[0].ty.dims > 2)
	{
        return FALSE;
	}

    /* the window generator's sizes and steps must be const */
    if (fg->nodes[win_gen].nodetype == ND_WINDOW_GEN)
	{
        for (i=0, j=1; i<fg->nodes[win_gen].inputs[0].ty.dims; i++, j+=2)
	    {
            if (!fg->nodes[win_gen].inputs[j].is_const)
	        {
	        return FALSE;
	        }
	    if (!fg->nodes[win_gen].inputs[j+1].is_const)
	        {
	        return FALSE;
	        }
	    }
	}
    else if (fg->nodes[win_gen].nodetype == ND_ELE_GEN)
	{
	if (fg->nodes[win_gen].outputs[0].ty.kind != Scalar)
	    {
	    return FALSE;
	    }

        for (i=0, j=1; i<fg->nodes[win_gen].inputs[0].ty.dims; i++, j++)
	    {
	    if (!fg->nodes[win_gen].inputs[j].is_const)
	        {
	        return FALSE;
	        }
	    }
	}
    else
        assert (0);

    /* the generator should not have an 'at' spec */
    for (i=1; i<fg->nodes[win_gen].num_outputs; i++)
        if (fg->nodes[win_gen].outputs[i].targets != NULL)
	    {
	    return FALSE;
	    }

    /* count the pragma's dimensions */
    for (i=0; i<MAXRANK; i++)
        if (pc->vals[i] < 0)
	    break;

    /* make sure the params match the rank */
    if (i != fg->nodes[win_gen].inputs[0].ty.dims)
        {
	if (!quiet)
	    {
	    printf ("  pragma error: number of %s params %d does not match the window's rank %d\n",
	    					str, i, fg->nodes[win_gen].inputs[0].ty.dims);
            printf ("    %s of window on loop on line %d, function '%s', file '%s' did not take place\n",
				str, fg->nodes[nd].loc.line, fg->nodes[nd].loc.func, fg->nodes[nd].loc.file);
	    }
	return FALSE;
	}

    /* the window tile sizes in the pragma must be at least as big as the window sizes */
    if (fg->nodes[win_gen].nodetype == ND_WINDOW_GEN)
        for (i=0, j=1; i<fg->nodes[win_gen].inputs[0].ty.dims; i++, j+=2)
            {
	    sscanf (fg->nodes[win_gen].inputs[j].constval, "%d", &val);
	    if (pc->vals[i] < val)
	        {
	        if (!quiet)
	            {
	            printf ("  pragma error: %s pragma's size %d is smaller than the window's size %d;\n",
								str, pc->vals[i], val);
                    printf ("    %s of window on loop on line %d, function '%s', file '%s' did not take place\n",
				str, fg->nodes[nd].loc.line, fg->nodes[nd].loc.func, fg->nodes[nd].loc.file);
	            }
	        return FALSE;
	        }
	    }

    for (it=fg->nodes[nd].My_nodes; it!=NULL; it=it->link)
	{
	id = it->val;

        switch (fg->nodes[id].nodetype)
	    {
	    case ND_REDUCE_MAX :		/* because we don't have a ND_MAX to combine fringes */
	    case ND_REDUCE_MIN :		/* because we don't have a ND_MIN to combine fringes */
	    case ND_REDUCE_VAL_AT_FIRST_MAX :
	    case ND_REDUCE_VAL_AT_FIRST_MIN :
	    case ND_REDUCE_VAL_AT_LAST_MAX :
	    case ND_REDUCE_VAL_AT_LAST_MIN :
	    case ND_REDUCE_VAL_AT_MAXS :
	    case ND_REDUCE_VAL_AT_MINS :
	    case ND_REDUCE_MEAN :
	    case ND_REDUCE_ST_DEV :
	    case ND_REDUCE_MODE :
	    case ND_REDUCE_MEDIAN :
	    case ND_REDUCE_HIST :
	    case ND_CONSTRUCT_CONCAT_MASKED :
	    /*
	    case ND_CONSTRUCT_CONCAT :
	    case ND_CONSTRUCT_TILE :
	    */
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
	        return FALSE;
	    default :
                break;
	    }
	}

    return TRUE;
    }

void do_window_tiling (FuncGraph *fg, int nd, IntList **list_copy, int loop, Pragma p)
    {
    PragCell *pc;
    IntList *it;
    int tiled_loop, gen_graph, win_gen;
    int tile_size0, win_size0;
    int tile_size1, win_size1, win_step1;
    int loop_copy0, loop_copy1;
    int outer_steps[8];

    if (!quiet)
	{
	if (p == PRAG_BLOCK)
            printf ("  blocking window of loop on line %d, function '%s', file '%s'\n",
			fg->nodes[nd].loc.line, fg->nodes[nd].loc.func, fg->nodes[nd].loc.file);
	else
            printf ("  stripmining window of loop on line %d, function '%s', file '%s'\n",
			fg->nodes[nd].loc.line, fg->nodes[nd].loc.func, fg->nodes[nd].loc.file);
        }

    if (p == PRAG_BLOCK)
        pc = find_pragma (fg, nd, PRAG_BLOCK);
    else
        pc = find_pragma (fg, nd, PRAG_STRIPMINE);
    assert (pc != NULL);

    /* find generator graph and first window gen node */
    gen_graph = find_generator_graph (fg, nd);
    win_gen = -1;
    for (it=fg->nodes[gen_graph].My_nodes; it!=NULL; it=it->link)
        if (fg->nodes[it->val].nodetype == ND_WINDOW_GEN)
	    {
	    if (win_gen == -1)
	        win_gen = it->val;
	    }
	else if (fg->nodes[it->val].nodetype == ND_ELE_GEN)
	    {
	    if (win_gen == -1)
	        win_gen = it->val;
	    elegen_to_window (fg, it->val, gen_graph, loop);
	    }

    assert (win_gen != -1);

    clear_size_info_from_outputs (fg, nd);
    clear_size_info_from_loop (fg, nd);

    switch (fg->nodes[win_gen].inputs[0].ty.dims)
        {
	case 1:
	    tile_size0 = pc->vals[0];
	    win_size0 = fg->nodes[win_gen].outputs[0].ty.dim_sizes[0];
    	    loop_copy0 = copy_graph (fg, fg, nd);
	    add_pragma (&(fg->nodes[loop_copy0].pragmas), PRAG_NO_DFG, NULL);
	    add_pragma (&(fg->nodes[loop_copy0].pragmas), PRAG_NO_UNROLL, NULL);
	    eliminate_pragma (fg, loop_copy0, p);
	    eliminate_pragma (fg, loop_copy0, PRAG_STRIPMINE);
    	    tiled_loop = create_tiled_loop (fg, nd, list_copy, pc, gen_graph, win_gen, outer_steps, p);
	    excess_1d (fg, tiled_loop, loop_copy0, tile_size0, win_size0, list_copy, outer_steps);
	    break;
	case 2:
	    tile_size0 = pc->vals[0];
	    tile_size1 = pc->vals[1];
	    win_size0 = fg->nodes[win_gen].outputs[0].ty.dim_sizes[0];
	    win_size1 = fg->nodes[win_gen].outputs[0].ty.dim_sizes[1];
	    sscanf (fg->nodes[win_gen].inputs[4].constval, "%d", &win_step1);
    	    loop_copy0 = copy_graph (fg, fg, nd);
	    add_pragma (&(fg->nodes[loop_copy0].pragmas), PRAG_NO_DFG, NULL);
	    add_pragma (&(fg->nodes[loop_copy0].pragmas), PRAG_NO_UNROLL, NULL);
	    eliminate_pragma (fg, loop_copy0, p);
	    eliminate_pragma (fg, loop_copy0, PRAG_STRIPMINE);
    	    loop_copy1 = copy_graph (fg, fg, nd);
	    add_pragma (&(fg->nodes[loop_copy1].pragmas), PRAG_NO_DFG, NULL);
	    add_pragma (&(fg->nodes[loop_copy1].pragmas), PRAG_NO_UNROLL, NULL);
	    eliminate_pragma (fg, loop_copy1, p);
	    eliminate_pragma (fg, loop_copy1, PRAG_STRIPMINE);
    	    tiled_loop = create_tiled_loop (fg, nd, list_copy, pc, gen_graph, win_gen, outer_steps, p);
	    excess_2d (fg, tiled_loop, loop_copy0, loop_copy1,
	    		tile_size0, tile_size1, win_size0, win_size1, win_step1, list_copy, outer_steps);
	    break;
	default :
	    assert (0);
	}
    }

void excess_1d (FuncGraph *fg, int tiled_loop, int loop_copy, int tile_size,
				int win_size, IntList **list_copy, int *outer_steps)
    {
    Edge *tg;
    IntList *itblob;
    int snd, spt, src_nd, src_pt, res_lo_nd, res_lo_pt, res_hi_nd, res_hi_pt;
    int cast_node, extents_node, aref_node;
    int i, gen_id, gin, gout;
    char str[128];

    gen_id = find_generator_graph (fg, loop_copy);
    for (i=0; i<fg->nodes[loop_copy].num_inputs; i++)
        {
	gin = fg->nodes[loop_copy].My_inputs[i];
	for (tg=fg->nodes[gin].outputs[0].targets; tg!=NULL; tg=tg->link)
	    if (tg->node == gen_id)
	        {
    		/* clear any sizes on the input that feeds the window */
    		clear_dim_sizes_to_win_gen (fg, loop_copy, i);

    		assert (fg->nodes[tiled_loop].inputs[i].back_edges != NULL);
    		src_nd = fg->nodes[tiled_loop].inputs[i].back_edges->node;
    		src_pt = fg->nodes[tiled_loop].inputs[i].back_edges->port;

    		extents_node = new_ddcf_node (fg, ND_EXTENTS, 1, 1, fg->nodes[tiled_loop].loc);
    		add_edge (fg, src_nd, src_pt, extents_node, 0);
    		copy_type_info (&(fg->nodes[extents_node].inputs[0].ty),
					&(fg->nodes[src_nd].outputs[src_pt].ty));
    		set_type_to_uint (&(fg->nodes[extents_node].outputs[0].ty), 32);
    		cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[tiled_loop].loc);
    		set_type_to_uint (&(fg->nodes[cast_node].inputs[0].ty), 32);
    		set_type_to_int (&(fg->nodes[cast_node].outputs[0].ty), 32);
    		add_edge (fg, extents_node, 0, cast_node, 0);
    		itblob = leftover_range (fg, cast_node, 0, tile_size, outer_steps[0],
    					&res_lo_nd, &res_lo_pt, &res_hi_nd, &res_hi_pt);

    		insert_val_after_in_intlist (extents_node, src_nd, list_copy);
    		insert_val_after_in_intlist (cast_node, extents_node, list_copy);
    		insert_val_after_in_intlist (itblob->val, cast_node, list_copy);
    		insert_val_after_in_intlist (itblob->link->val, itblob->val, list_copy);

    		aref_node = new_ddcf_node (fg, ND_ARRAYREF, 4, 1, fg->nodes[tiled_loop].loc);
    		SacMalloc (fg->nodes[aref_node].Reftypes, char*, 2)
    		strcpy (fg->nodes[aref_node].Reftypes, ":");
    		add_edge (fg, src_nd, src_pt, aref_node, 0);
    		add_edge (fg, res_lo_nd, res_lo_pt, aref_node, 1);
    		add_edge (fg, res_hi_nd, res_hi_pt, aref_node, 2);
    		copy_type_info (&(fg->nodes[aref_node].inputs[0].ty),
					&(fg->nodes[src_nd].outputs[src_pt].ty));
    		copy_type_info (&(fg->nodes[aref_node].inputs[1].ty),
					&(fg->nodes[res_lo_nd].outputs[res_lo_pt].ty));
    		copy_type_info (&(fg->nodes[aref_node].inputs[2].ty),
					&(fg->nodes[res_hi_nd].outputs[res_hi_pt].ty));
    		fg->nodes[aref_node].inputs[3].is_const = TRUE;
    		strcpy (str, "1");
    		fg->nodes[aref_node].inputs[3].constval = copy_to_const (str);
    		set_type_to_uint (&(fg->nodes[aref_node].inputs[3].ty), 32);
    		insert_val_after_in_intlist (aref_node, itblob->link->val, list_copy);
    		free_intlist (&itblob);

    		copy_type_info (&(fg->nodes[aref_node].outputs[0].ty),
					&(fg->nodes[aref_node].inputs[0].ty));
    		fg->nodes[aref_node].outputs[0].ty.dim_sizes[0] = -1;

	        add_edge (fg, aref_node, 0, loop_copy, i);
	        copy_type_info (&(fg->nodes[loop_copy].inputs[i].ty),
					&(fg->nodes[aref_node].outputs[0].ty));
		}
	    else
	        {
	        assert (fg->nodes[tiled_loop].inputs[i].back_edges != NULL);
	        snd = fg->nodes[tiled_loop].inputs[i].back_edges->node;
	        spt = fg->nodes[tiled_loop].inputs[i].back_edges->port;
	        add_edge (fg, snd, spt, loop_copy, i);
	        copy_type_info (&(fg->nodes[loop_copy].inputs[i].ty),
					&(fg->nodes[tiled_loop].inputs[i].ty));
	        }
	}

    insert_node_in_intlist (fg, loop_copy, list_copy);

    /* handle the output of the tiled loop and the loop copy */
    for (i=0; i<fg->nodes[tiled_loop].num_outputs; i++)
        {
	gout = fg->nodes[loop_copy].My_outputs[i];
	assert (fg->nodes[gout].inputs[0].back_edges != NULL);
	snd = fg->nodes[gout].inputs[0].back_edges->node;
	spt = fg->nodes[gout].inputs[0].back_edges->port;
	switch (fg->nodes[snd].nodetype)
	    {
	    case ND_CONSTRUCT_CONCAT :
	    case ND_CONSTRUCT_TILE :
	    case ND_CONSTRUCT_ARRAY :
		reduce_1d (fg, tiled_loop, loop_copy, ND_ARR_CONCAT, i, list_copy);
		break;
	    case ND_REDUCE_SUM :
		reduce_1d (fg, tiled_loop, loop_copy, ND_ADD, i, list_copy);
		break;
	    case ND_REDUCE_PRODUCT :
		reduce_1d (fg, tiled_loop, loop_copy, ND_MUL, i, list_copy);
		break;
	    case ND_REDUCE_AND :
		reduce_1d (fg, tiled_loop, loop_copy, ND_AND, i, list_copy);
		break;
	    case ND_REDUCE_OR :
		reduce_1d (fg, tiled_loop, loop_copy, ND_OR, i, list_copy);
		break;
	    /*
	    case ND_REDUCE_MIN :
		reduce_1d (fg, tiled_loop, loop_copy, ND_MIN, i, list_copy);
		break;
	    case ND_REDUCE_MAX :
		reduce_1d (fg, tiled_loop, loop_copy, ND_MAX, i, list_copy);
		break;
	    */
	    default :
	        assert (0);
	    }
	}
    }

void reduce_1d (FuncGraph *fg, int tiled_loop, int loop_copy, DdcfType nty, int i, IntList **list_copy)
    {
    int new_red;

    new_red = new_ddcf_node (fg, nty, 2, 1, fg->nodes[tiled_loop].loc);
    fg->nodes[new_red].outputs[0] = fg->nodes[tiled_loop].outputs[i];
    fg->nodes[tiled_loop].outputs[i].targets = NULL;
    retarget_backedges (fg, fg->nodes[new_red].outputs[0].targets, tiled_loop, i, new_red, 0);
    add_edge (fg, tiled_loop, i, new_red, 0);
    copy_type_info (&(fg->nodes[new_red].inputs[0].ty), &(fg->nodes[tiled_loop].outputs[i].ty));
    add_edge (fg, loop_copy, i, new_red, 1);
    copy_type_info (&(fg->nodes[new_red].inputs[1].ty), &(fg->nodes[loop_copy].outputs[i].ty));
    copy_type_info (&(fg->nodes[new_red].outputs[0].ty), &(fg->nodes[tiled_loop].outputs[i].ty));

    insert_node_in_intlist (fg, new_red, list_copy);
    }

void excess_2d (FuncGraph *fg, int tiled_loop, int loop_copy0, int loop_copy1, int tile_size0,
	int tile_size1, int win_size0, int win_size1, int win_step1, IntList **list_copy, int *outer_steps)
    {
    Edge *tg;
    IntList *itblob0, *itblob1;
    int snd, spt, src_nd, src_pt, res_lo_nd_0, res_lo_pt_0, res_hi_nd_0, res_hi_pt_0;
    int res_lo_nd_1, res_lo_pt_1, res_hi_nd_1, res_hi_pt_1;
    int cast_node0, cast_node1, extents_node, aref_node0, aref_node1, add_node;
    int i, gen_id, gin, gout;
    char str[128];

    gen_id = find_generator_graph (fg, loop_copy0);
    for (i=0; i<fg->nodes[loop_copy0].num_inputs; i++)
        {
	gin = fg->nodes[loop_copy0].My_inputs[i];
	for (tg=fg->nodes[gin].outputs[0].targets; tg!=NULL; tg=tg->link)
	    if (tg->node == gen_id)
	        {
    		/* clear any sizes on the input that feeds the window */
    		clear_dim_sizes_to_win_gen (fg, loop_copy0, i);
    		clear_dim_sizes_to_win_gen (fg, loop_copy1, i);

    		assert (fg->nodes[tiled_loop].inputs[i].back_edges != NULL);
    		src_nd = fg->nodes[tiled_loop].inputs[i].back_edges->node;
    		src_pt = fg->nodes[tiled_loop].inputs[i].back_edges->port;

    		extents_node = new_ddcf_node (fg, ND_EXTENTS, 1, 2, fg->nodes[tiled_loop].loc);
    		add_edge (fg, src_nd, src_pt, extents_node, 0);
    		copy_type_info (&(fg->nodes[extents_node].inputs[0].ty),
					&(fg->nodes[src_nd].outputs[src_pt].ty));
    		set_type_to_uint (&(fg->nodes[extents_node].outputs[0].ty), 32);
    		set_type_to_uint (&(fg->nodes[extents_node].outputs[1].ty), 32);
    		cast_node0 = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[tiled_loop].loc);
    		set_type_to_uint (&(fg->nodes[cast_node0].inputs[0].ty), 32);
    		set_type_to_int (&(fg->nodes[cast_node0].outputs[0].ty), 32);
    		add_edge (fg, extents_node, 0, cast_node0, 0);
    		cast_node1 = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[tiled_loop].loc);
    		set_type_to_uint (&(fg->nodes[cast_node1].inputs[0].ty), 32);
    		set_type_to_int (&(fg->nodes[cast_node1].outputs[0].ty), 32);
    		add_edge (fg, extents_node, 1, cast_node1, 0);

    		itblob0 = leftover_range (fg, cast_node0, 0, tile_size0,
    				outer_steps[0], &res_lo_nd_0, &res_lo_pt_0, &res_hi_nd_0, &res_hi_pt_0);
    		itblob1 = leftover_range (fg, cast_node1, 0, tile_size1,
    				outer_steps[1], &res_lo_nd_1, &res_lo_pt_1, &res_hi_nd_1, &res_hi_pt_1);

    		insert_val_after_in_intlist (extents_node, src_nd, list_copy);
    		insert_val_after_in_intlist (cast_node0, extents_node, list_copy);
    		insert_val_after_in_intlist (cast_node1, extents_node, list_copy);
    		insert_val_after_in_intlist (itblob0->val, cast_node0, list_copy);
    		insert_val_after_in_intlist (itblob0->link->val, cast_node0, list_copy);
    		insert_val_after_in_intlist (itblob1->val, cast_node1, list_copy);
    		insert_val_after_in_intlist (itblob1->link->val, cast_node1, list_copy);

    		/* take a slice for the bottom strip */
    		aref_node0 = new_ddcf_node (fg, ND_ARRAYREF, 7, 1, fg->nodes[tiled_loop].loc);
    		SacMalloc (fg->nodes[aref_node0].Reftypes, char*, 3)
    		strcpy (fg->nodes[aref_node0].Reftypes, "::");
    		add_edge (fg, src_nd, src_pt, aref_node0, 0);
    		add_edge (fg, res_lo_nd_0, res_lo_pt_0, aref_node0, 1);
    		add_edge (fg, res_hi_nd_0, res_hi_pt_0, aref_node0, 2);
    		copy_type_info (&(fg->nodes[aref_node0].inputs[0].ty),
					&(fg->nodes[src_nd].outputs[src_pt].ty));
    		copy_type_info (&(fg->nodes[aref_node0].inputs[1].ty),
					&(fg->nodes[res_lo_nd_0].outputs[res_lo_pt_0].ty));
    		copy_type_info (&(fg->nodes[aref_node0].inputs[2].ty),
					&(fg->nodes[res_hi_nd_0].outputs[res_hi_pt_0].ty));
    		fg->nodes[aref_node0].inputs[3].is_const = TRUE;
    		strcpy (str, "1");
    		fg->nodes[aref_node0].inputs[3].constval = copy_to_const (str);
    		set_type_to_uint (&(fg->nodes[aref_node0].inputs[3].ty), 32);
    		fg->nodes[aref_node0].inputs[4].is_const = TRUE;
    		strcpy (str, "0");
    		fg->nodes[aref_node0].inputs[4].constval = copy_to_const (str);
    		set_type_to_uint (&(fg->nodes[aref_node0].inputs[4].ty), 32);
    		fg->nodes[aref_node0].inputs[6].is_const = TRUE;
    		strcpy (str, "1");
    		fg->nodes[aref_node0].inputs[6].constval = copy_to_const (str);
    		set_type_to_uint (&(fg->nodes[aref_node0].inputs[6].ty), 32);

    		/* transfer input type to output type, but remove size info */
    		copy_type_info (&(fg->nodes[aref_node0].outputs[0].ty),
					&(fg->nodes[aref_node0].inputs[0].ty));
    		fg->nodes[aref_node0].outputs[0].ty.dim_sizes[0] = -1;
    		fg->nodes[aref_node0].outputs[0].ty.dim_sizes[1] = -1;

    		/* take a slice for the right strip */
    		aref_node1 = new_ddcf_node (fg, ND_ARRAYREF, 7, 1, fg->nodes[tiled_loop].loc);
    		SacMalloc (fg->nodes[aref_node1].Reftypes, char*, 3)
    		strcpy (fg->nodes[aref_node1].Reftypes, "::");
    		add_edge (fg, src_nd, src_pt, aref_node1, 0);
    		copy_type_info (&(fg->nodes[aref_node1].inputs[0].ty),
					&(fg->nodes[src_nd].outputs[src_pt].ty));
    		fg->nodes[aref_node1].inputs[1].is_const = TRUE;
    		strcpy (str, "0");
    		fg->nodes[aref_node1].inputs[1].constval = copy_to_const (str);
    		set_type_to_uint (&(fg->nodes[aref_node1].inputs[1].ty), 32);
    		add_edge (fg, res_hi_nd_0, res_hi_pt_0, aref_node1, 2);
    		copy_type_info (&(fg->nodes[aref_node1].inputs[2].ty),
					&(fg->nodes[res_hi_nd_1].outputs[res_hi_pt_1].ty));
    		fg->nodes[aref_node1].inputs[3].is_const = TRUE;
    		strcpy (str, "1");
    		fg->nodes[aref_node1].inputs[3].constval = copy_to_const (str);
    		set_type_to_uint (&(fg->nodes[aref_node1].inputs[3].ty), 32);
    		add_edge (fg, res_lo_nd_1, res_lo_pt_1, aref_node1, 4);
    		copy_type_info (&(fg->nodes[aref_node1].inputs[4].ty),
					&(fg->nodes[res_lo_nd_1].outputs[res_lo_pt_1].ty));
    		add_edge (fg, res_hi_nd_1, res_hi_pt_1, aref_node1, 5);
    		copy_type_info (&(fg->nodes[aref_node1].inputs[5].ty),
					&(fg->nodes[res_hi_nd_1].outputs[res_hi_pt_1].ty));
    		fg->nodes[aref_node1].inputs[6].is_const = TRUE;
    		strcpy (str, "1");
    		fg->nodes[aref_node1].inputs[6].constval = copy_to_const (str);
    		set_type_to_uint (&(fg->nodes[aref_node1].inputs[6].ty), 32);

    		/* transfer input type to output type, but remove size info */
    		copy_type_info (&(fg->nodes[aref_node1].outputs[0].ty),
					&(fg->nodes[aref_node1].inputs[0].ty));
    		fg->nodes[aref_node1].outputs[0].ty.dim_sizes[0] = -1;
    		fg->nodes[aref_node1].outputs[0].ty.dim_sizes[1] = -1;

    		add_node = new_ddcf_node (fg, ND_ADD, 2, 1, fg->nodes[tiled_loop].loc);
    		add_edge (fg, res_lo_nd_1, res_lo_pt_1, add_node, 0);
    		copy_type_info (&(fg->nodes[add_node].inputs[0].ty),
					&(fg->nodes[res_lo_nd_1].outputs[res_lo_pt_1].ty));
    		sprintf (str, "%d", win_size1-1-win_step1);
    		fg->nodes[add_node].inputs[1].is_const = TRUE;
    		fg->nodes[add_node].inputs[1].constval = copy_to_const (str);
    		set_type_to_int (&(fg->nodes[add_node].inputs[1].ty), 32);
    		set_type_to_int (&(fg->nodes[add_node].outputs[0].ty), 32);
    		add_edge (fg, add_node, 0, aref_node0, 5);
    		copy_type_info (&(fg->nodes[aref_node0].inputs[5].ty), &(fg->nodes[add_node].outputs[0].ty));
    		insert_val_after_in_intlist (add_node, res_lo_nd_1, list_copy);

    		insert_node_in_intlist (fg, aref_node0, list_copy);
    		insert_node_in_intlist (fg, aref_node1, list_copy);
    		free_intlist (&itblob0);
    		free_intlist (&itblob1);

	    	add_edge (fg, aref_node0, 0, loop_copy0, i);
	    	copy_type_info (&(fg->nodes[loop_copy0].inputs[i].ty),
				&(fg->nodes[aref_node0].outputs[0].ty));
	    	add_edge (fg, aref_node1, 0, loop_copy1, i);
	    	copy_type_info (&(fg->nodes[loop_copy1].inputs[i].ty),
				&(fg->nodes[aref_node1].outputs[0].ty));
		}
	    else
	        {
	    	assert (fg->nodes[tiled_loop].inputs[i].back_edges != NULL);
	    	snd = fg->nodes[tiled_loop].inputs[i].back_edges->node;
	    	spt = fg->nodes[tiled_loop].inputs[i].back_edges->port;
	    	add_edge (fg, snd, spt, loop_copy0, i);
	    	copy_type_info (&(fg->nodes[loop_copy0].inputs[i].ty),
					&(fg->nodes[tiled_loop].inputs[i].ty));
	    	add_edge (fg, snd, spt, loop_copy1, i);
	    	copy_type_info (&(fg->nodes[loop_copy1].inputs[i].ty),
					&(fg->nodes[tiled_loop].inputs[i].ty));
		}
	}

    insert_node_in_intlist (fg, loop_copy0, list_copy);
    insert_node_in_intlist (fg, loop_copy1, list_copy);

    /* handle the output of the tiled loop and the loop copy */
    for (i=0; i<fg->nodes[tiled_loop].num_outputs; i++)
        {
	gout = fg->nodes[loop_copy0].My_outputs[i];
	assert (fg->nodes[gout].inputs[0].back_edges != NULL);
	snd = fg->nodes[gout].inputs[0].back_edges->node;
	spt = fg->nodes[gout].inputs[0].back_edges->port;
	switch (fg->nodes[snd].nodetype)
	    {
	    case ND_CONSTRUCT_CONCAT :
	    case ND_CONSTRUCT_TILE :
	    case ND_CONSTRUCT_ARRAY :
		reduce_2d (fg, tiled_loop, loop_copy0, loop_copy1,
						ND_ARR_VERT_CONCAT, ND_ARR_CONCAT, i, list_copy);
		break;
	    case ND_REDUCE_SUM :
		reduce_2d (fg, tiled_loop, loop_copy0, loop_copy1, ND_ADD, ND_ADD, i, list_copy);
		break;
	    case ND_REDUCE_PRODUCT :
		reduce_2d (fg, tiled_loop, loop_copy0, loop_copy1, ND_MUL, ND_MUL, i, list_copy);
		break;
	    case ND_REDUCE_AND :
		reduce_2d (fg, tiled_loop, loop_copy0, loop_copy1, ND_AND, ND_AND, i, list_copy);
		break;
	    case ND_REDUCE_OR :
		reduce_2d (fg, tiled_loop, loop_copy0, loop_copy1, ND_OR, ND_OR, i, list_copy);
		break;
	    /*
	    case ND_REDUCE_MIN :
		reduce_2d (fg, tiled_loop, loop_copy0, loop_copy1, ND_MIN, ND_MIN, i, list_copy);
		break;
	    case ND_REDUCE_MAX :
		reduce_2d (fg, tiled_loop, loop_copy0, loop_copy1, ND_MAX, ND_MAX, i, list_copy);
		break;
	    */
	    default :
	        assert (0);
	    }
	}
    }

void reduce_2d (FuncGraph *fg, int tiled_loop, int loop_copy0,
		int loop_copy1, DdcfType nty0, DdcfType nty1, int i, IntList **list_copy)
    {
    int new_red, new_red1;

    new_red = new_ddcf_node (fg, nty0, 2, 1, fg->nodes[tiled_loop].loc);
    new_red1 = new_ddcf_node (fg, nty1, 2, 1, fg->nodes[tiled_loop].loc);
    fg->nodes[new_red1].outputs[0] = fg->nodes[tiled_loop].outputs[i];
    fg->nodes[tiled_loop].outputs[i].targets = NULL;
    retarget_backedges (fg, fg->nodes[new_red1].outputs[0].targets, tiled_loop, i, new_red1, 0);

    add_edge (fg, tiled_loop, i, new_red, 0);
    copy_type_info (&(fg->nodes[new_red].inputs[0].ty), &(fg->nodes[tiled_loop].outputs[i].ty));
    add_edge (fg, loop_copy0, i, new_red, 1);
    copy_type_info (&(fg->nodes[new_red].inputs[1].ty), &(fg->nodes[loop_copy0].outputs[i].ty));
    copy_type_info (&(fg->nodes[new_red].outputs[0].ty), &(fg->nodes[tiled_loop].outputs[i].ty));

    add_edge (fg, new_red, 0, new_red1, 0);
    copy_type_info (&(fg->nodes[new_red1].inputs[0].ty), &(fg->nodes[new_red].outputs[0].ty));
    add_edge (fg, loop_copy1, i, new_red1, 1);
    copy_type_info (&(fg->nodes[new_red1].inputs[1].ty), &(fg->nodes[loop_copy1].outputs[i].ty));

    insert_node_in_intlist (fg, new_red, list_copy);
    insert_node_in_intlist (fg, new_red1, list_copy);
    }

int create_tiled_loop (FuncGraph *fg, int nd, IntList **list_copy,
				PragCell *pc, int gen_graph, int win_gen, int *outer_steps, Pragma p)
    {
    IntList *it;
    char str[128];
    int rank, new_win, loop_node, new_dot, new_reduc, i, j, w_step;
    int num_inputs, num_outputs, in_node, out_node;
    int gin, gout, snd, spt, val, tnd, tpt;

    rank = fg->nodes[win_gen].inputs[0].ty.dims;
    num_inputs = fg->nodes[nd].num_inputs;
    num_outputs = fg->nodes[nd].num_outputs;

    loop_node = new_ddcf_node (fg, ND_FORALL, num_inputs, num_outputs, fg->nodes[nd].loc);
    fg->nodes[loop_node].pragmas = copy_pragmas (fg->nodes[nd].pragmas);
    fg->nodes[loop_node].My_inputs = make_io_array (num_inputs);
    fg->nodes[loop_node].My_outputs = make_io_array (num_outputs);

    replace_in_intlist (list_copy, nd, loop_node);
    add_to_list (nd, &(fg->nodes[loop_node].My_nodes));

    new_dot = copy_graph (fg, fg, gen_graph);
    insert_val_before_in_intlist (new_dot, nd, &(fg->nodes[loop_node].My_nodes));

    /* find and adjust the window generators */
    for (it=fg->nodes[new_dot].My_nodes; it!=NULL; it=it->link)
        if (fg->nodes[it->val].nodetype == ND_WINDOW_GEN)
	    {
	    new_win = it->val;

            /* adjust the input values of the new window generator */
            for (i=0, j=1; i<rank; i++, j+=2)
        	{
		sprintf (str, "%d", pc->vals[i]);
		fg->nodes[new_win].inputs[j].constval = copy_to_const (str);
		set_type_to_uint (&(fg->nodes[new_win].inputs[j].ty), 32);

		fg->nodes[new_win].outputs[0].ty.dim_sizes[i] = pc->vals[i];
		tnd = fg->nodes[new_win].outputs[0].targets->node;
		tpt = fg->nodes[new_win].outputs[0].targets->port;
		fg->nodes[tnd].inputs[tpt].ty.dim_sizes[i] = pc->vals[i];
		fg->nodes[new_dot].outputs[0].ty.dim_sizes[i] = pc->vals[i];

		sscanf (fg->nodes[win_gen].inputs[j].constval, "%d", &val);
		sscanf (fg->nodes[win_gen].inputs[j+1].constval, "%d", &w_step);
		val = (pc->vals[i] - val + w_step) / w_step * w_step;
		sprintf (str, "%d", val);
		fg->nodes[new_win].inputs[j+1].constval = copy_to_const (str);
		set_type_to_uint (&(fg->nodes[new_win].inputs[j+1].ty), 32);

		outer_steps[i] = val;
		}
	    }

    /* move all edges targeting the old loop, to target the new loop */
    for (i=0; i<num_inputs; i++)
        {
	in_node = new_ddcf_node (fg, ND_G_INPUT, 0, 1, fg->nodes[nd].loc);
	fg->nodes[in_node].My_graph = loop_node;
	fg->nodes[in_node].Io_num = i;
	fg->nodes[loop_node].My_inputs[i] = in_node;
	add_to_list (in_node, &(fg->nodes[loop_node].My_nodes));
	fg->nodes[loop_node].inputs[i] = fg->nodes[nd].inputs[i];
	retarget (fg, fg->nodes[loop_node].inputs[i].back_edges, nd, i, loop_node, i);
	fg->nodes[nd].inputs[i].back_edges = NULL;
	}

    /* move all edges coming from the old loop, to come from the new loop */
    for (i=0; i<num_outputs; i++)
        {
	out_node = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, fg->nodes[nd].loc);
	fg->nodes[out_node].My_graph = loop_node;
	fg->nodes[out_node].Io_num = i;
	fg->nodes[loop_node].My_outputs[i] = out_node;
	fg->nodes[loop_node].outputs[i] = fg->nodes[nd].outputs[i];
	retarget_backedges (fg, fg->nodes[loop_node].outputs[i].targets, nd, i, loop_node, i);
	fg->nodes[nd].outputs[i].targets = NULL;
	}

    for (i=0; i<num_inputs; i++)
        {
	in_node = fg->nodes[nd].My_inputs[i];
	gin = fg->nodes[loop_node].My_inputs[i];

	if (fg->nodes[in_node].outputs[0].targets != NULL)
	    {
	    snd = fg->nodes[in_node].outputs[0].targets->node;
	    spt = fg->nodes[in_node].outputs[0].targets->port;

	    if (fg->nodes[snd].nodetype == ND_DOT_PROD)
	        {
	        add_edge (fg, gin, 0, new_dot, spt);
	        copy_type_info (&(fg->nodes[gin].outputs[0].ty), &(fg->nodes[new_dot].inputs[spt].ty));
	        add_edge (fg, new_dot, spt, nd, i);
	        }
	    else
	        {
	        add_edge (fg, gin, 0, nd, i);
	        copy_type_info (&(fg->nodes[gin].outputs[0].ty), &(fg->nodes[nd].inputs[i].ty));
	        }
	    }
        }

    /* create the reduction nodes for the output loop, based on the reduction node
     * type of the inner loop;
     */
    for (i=0; i<num_outputs; i++)
        {
	gout = fg->nodes[nd].My_outputs[i];
	assert (fg->nodes[gout].inputs[0].back_edges != NULL);
	snd = fg->nodes[gout].inputs[0].back_edges->node;

	switch (fg->nodes[snd].nodetype)
	    {
	    case ND_CONSTRUCT_CONCAT :
		new_reduc = new_ddcf_node (fg, ND_CONSTRUCT_CONCAT, 1, 1, fg->nodes[nd].loc);
		break;
	    case ND_CONSTRUCT_TILE :
	    case ND_CONSTRUCT_ARRAY :
		new_reduc = new_ddcf_node (fg, ND_CONSTRUCT_TILE, 1, 1, fg->nodes[nd].loc);
		break;
	    case ND_REDUCE_SUM :
		new_reduc = new_ddcf_node (fg, ND_REDUCE_SUM, 2, 1, fg->nodes[nd].loc);
		fg->nodes[new_reduc].inputs[1].is_const = TRUE;
		SacMalloc (fg->nodes[new_reduc].inputs[1].constval, char*, 5 * sizeof (char))
		strcpy (fg->nodes[new_reduc].inputs[1].constval, "true");
		set_type_to_bool (&(fg->nodes[new_reduc].inputs[1].ty));
		break;
	    case ND_REDUCE_PRODUCT :
		new_reduc = new_ddcf_node (fg, ND_REDUCE_PRODUCT, 2, 1, fg->nodes[nd].loc);
		fg->nodes[new_reduc].inputs[1].is_const = TRUE;
		SacMalloc (fg->nodes[new_reduc].inputs[1].constval, char*, 5 * sizeof (char))
		strcpy (fg->nodes[new_reduc].inputs[1].constval, "true");
		set_type_to_bool (&(fg->nodes[new_reduc].inputs[1].ty));
		break;
	    case ND_REDUCE_MIN :
		new_reduc = new_ddcf_node (fg, ND_REDUCE_MIN, 2, 1, fg->nodes[nd].loc);
		fg->nodes[new_reduc].inputs[1].is_const = TRUE;
		SacMalloc (fg->nodes[new_reduc].inputs[1].constval, char*, 5 * sizeof (char))
		strcpy (fg->nodes[new_reduc].inputs[1].constval, "true");
		set_type_to_bool (&(fg->nodes[new_reduc].inputs[1].ty));
		break;
	    case ND_REDUCE_MAX :
		new_reduc = new_ddcf_node (fg, ND_REDUCE_MAX, 2, 1, fg->nodes[nd].loc);
		fg->nodes[new_reduc].inputs[1].is_const = TRUE;
		SacMalloc (fg->nodes[new_reduc].inputs[1].constval, char*, 5 * sizeof (char))
		strcpy (fg->nodes[new_reduc].inputs[1].constval, "true");
		set_type_to_bool (&(fg->nodes[new_reduc].inputs[1].ty));
		break;
	    case ND_REDUCE_AND :
		new_reduc = new_ddcf_node (fg, ND_REDUCE_AND, 2, 1, fg->nodes[nd].loc);
		fg->nodes[new_reduc].inputs[1].is_const = TRUE;
		SacMalloc (fg->nodes[new_reduc].inputs[1].constval, char*, 5 * sizeof (char))
		strcpy (fg->nodes[new_reduc].inputs[1].constval, "true");
		set_type_to_bool (&(fg->nodes[new_reduc].inputs[1].ty));
		break;
	    case ND_REDUCE_OR :
		new_reduc = new_ddcf_node (fg, ND_REDUCE_OR, 2, 1, fg->nodes[nd].loc);
		fg->nodes[new_reduc].inputs[1].is_const = TRUE;
		SacMalloc (fg->nodes[new_reduc].inputs[1].constval, char*, 5 * sizeof (char))
		strcpy (fg->nodes[new_reduc].inputs[1].constval, "true");
		set_type_to_bool (&(fg->nodes[new_reduc].inputs[1].ty));
		break;
	    default :
	        assert (0);
	    }

	gout = fg->nodes[loop_node].My_outputs[i];
	add_edge (fg, nd, i, new_reduc, 0);
	copy_type_info (&(fg->nodes[new_reduc].inputs[0].ty), &(fg->nodes[nd].outputs[i].ty));
	add_edge (fg, new_reduc, 0, gout, 0);
	copy_type_info (&(fg->nodes[new_reduc].outputs[0].ty), &(fg->nodes[nd].outputs[i].ty));
	copy_type_info (&(fg->nodes[gout].inputs[0].ty), &(fg->nodes[new_reduc].outputs[0].ty));
	insert_val_after_in_intlist (new_reduc, nd, &(fg->nodes[loop_node].My_nodes));

	switch (fg->nodes[snd].nodetype)
	    {
	    case ND_CONSTRUCT_ARRAY :
	    case ND_CONSTRUCT_TILE :
	    case ND_CONSTRUCT_CONCAT :
	        clear_dim_sizes_reduction_node (fg, new_reduc);
		break;
	    default :
	        break;
	    }
	}

    for (it=fg->nodes[new_dot].My_nodes; it!=NULL; it=it->link)
        if (fg->nodes[it->val].nodetype == ND_WINDOW_GEN)
	    {
	    new_win = it->val;
    	    clear_dim_sizes_win_gen (fg, new_win);
	    }

    if (p == PRAG_STRIPMINE)
        {
        eliminate_pragma (fg, nd, PRAG_STRIPMINE);
        eliminate_pragma (fg, loop_node, PRAG_STRIPMINE);
        eliminate_pragma (fg, nd, PRAG_NO_UNROLL);

	add_pragma (&(fg->nodes[loop_node].pragmas), PRAG_NO_UNROLL, NULL);
	}
    else
        {
	eliminate_pragma (fg, nd, PRAG_BLOCK);
	eliminate_pragma (fg, loop_node, PRAG_BLOCK);

	/* eliminate a STRIPMINE pragma, if it exists, from the outer loop */
        eliminate_pragma (fg, loop_node, PRAG_STRIPMINE);

	add_pragma (&(fg->nodes[nd].pragmas), PRAG_NO_UNROLL, NULL);
	}

    return loop_node;
    }

void clear_dim_sizes_reduction_node (FuncGraph *fg, int reduc)
    {
    int nd, pt, tnd;

    clear_dim_sizes (&(fg->nodes[reduc].inputs[0].ty));
    assert (fg->nodes[reduc].inputs[0].back_edges != NULL);
    nd = fg->nodes[reduc].inputs[0].back_edges->node;
    pt = fg->nodes[reduc].inputs[0].back_edges->port;
    clear_dim_sizes (&(fg->nodes[nd].outputs[pt].ty));

    assert (fg->nodes[nd].nodetype == ND_FORALL);
    tnd = fg->nodes[nd].My_outputs[pt];
    assert (fg->nodes[tnd].nodetype == ND_G_OUTPUT);
    clear_dim_sizes (&(fg->nodes[tnd].inputs[0].ty));

    assert (fg->nodes[tnd].inputs[0].back_edges != NULL);
    nd = fg->nodes[tnd].inputs[0].back_edges->node;
    pt = fg->nodes[tnd].inputs[0].back_edges->port;
    clear_dim_sizes (&(fg->nodes[nd].outputs[pt].ty));
    }

/* follow the output of this window gen to the input of the interior window gen,
 * clearing any array size info; this is needed because the array sizes are no
 * longer correct (since this came from a copy of the other dot gen graph)
 */
void clear_dim_sizes_win_gen (FuncGraph *fg, int new_win)
    {
    int nd, pt, tnd, tpt;

    assert (fg->nodes[new_win].outputs[0].targets != NULL);
    nd = fg->nodes[new_win].outputs[0].targets->node;
    pt = fg->nodes[new_win].outputs[0].targets->port;
    clear_dim_sizes (&(fg->nodes[nd].inputs[pt].ty));

    assert (fg->nodes[nd].nodetype == ND_G_OUTPUT);
    tnd = fg->nodes[nd].My_graph;
    tpt = fg->nodes[nd].Io_num;
    clear_dim_sizes (&(fg->nodes[tnd].outputs[tpt].ty));

    assert (fg->nodes[tnd].outputs[tpt].targets != NULL);
    nd = fg->nodes[tnd].outputs[tpt].targets->node;
    pt = fg->nodes[tnd].outputs[tpt].targets->port;
    clear_dim_sizes (&(fg->nodes[nd].inputs[pt].ty));

    assert (fg->nodes[nd].nodetype == ND_FORALL);
    tnd = fg->nodes[nd].My_inputs[pt];
    assert (fg->nodes[tnd].nodetype == ND_G_INPUT);
    clear_dim_sizes (&(fg->nodes[tnd].outputs[0].ty));

    assert (fg->nodes[tnd].outputs[0].targets != NULL);
    nd = fg->nodes[tnd].outputs[0].targets->node;
    pt = fg->nodes[tnd].outputs[0].targets->port;
    clear_dim_sizes (&(fg->nodes[nd].inputs[pt].ty));

    assert (fg->nodes[nd].nodetype == ND_DOT_PROD);
    tnd = fg->nodes[nd].My_inputs[pt];
    assert (fg->nodes[tnd].nodetype == ND_G_INPUT);
    clear_dim_sizes (&(fg->nodes[tnd].outputs[0].ty));

    assert (fg->nodes[tnd].outputs[0].targets != NULL);
    nd = fg->nodes[tnd].outputs[0].targets->node;
    pt = fg->nodes[tnd].outputs[0].targets->port;
    clear_dim_sizes (&(fg->nodes[nd].inputs[pt].ty));
    }

/* 'nd' is a FORALL loop; start at its port 'pt' and clear the dim sizes
 * down to the input of the window generator;
 */
void clear_dim_sizes_to_win_gen (FuncGraph *fg, int nd, int pt)
    {
    Edge *tg;
    int gin, tnd, tpt;

    assert (fg->nodes[nd].nodetype == ND_FORALL);
    clear_dim_sizes (&(fg->nodes[nd].inputs[pt].ty));
    gin = fg->nodes[nd].My_inputs[pt];
    clear_dim_sizes (&(fg->nodes[gin].outputs[0].ty));
    for (tg=fg->nodes[gin].outputs[0].targets; tg!=NULL; tg=tg->link)
        {
	tnd = tg->node;
	tpt = tg->port;
	clear_dim_sizes (&(fg->nodes[tnd].inputs[tpt].ty));

	if (fg->nodes[tnd].nodetype == ND_DOT_PROD)
	    {
	    gin = fg->nodes[tnd].My_inputs[tpt];
	    clear_dim_sizes (&(fg->nodes[gin].outputs[0].ty));
	    assert (fg->nodes[gin].outputs[0].targets != NULL);
	    tnd = fg->nodes[gin].outputs[0].targets->node;
	    tpt = fg->nodes[gin].outputs[0].targets->port;
	    clear_dim_sizes (&(fg->nodes[tnd].inputs[tpt].ty));
	    }
	}
    }

void clear_dim_sizes (TypeInfo *ty)
    {
    int i;

    for (i=0; i<MAXRANK; i++)
        ty->dim_sizes[i] = -1;
    }

IntList *leftover_range (FuncGraph *fg, int ext_nd, int ext_pt, int tile_size,
		int outer_step, int *res_lo_nd, int *res_lo_pt, int *res_hi_nd, int *res_hi_pt)
    {
    IntList *its = NULL;
    int switch_node, gin, gout, cgin, cgout, cmp_node, case_node;
    int key_node, sw_sel, sub_node, mod_node, add_node, ssub_node, aadd_node;
    char str[128];
    
    /* create the switch node and its inputs and outputs */
    switch_node = new_ddcf_node (fg, ND_SWITCH, 1, 1, fg->nodes[ext_nd].loc);
    fg->nodes[switch_node].My_inputs = make_io_array (1);
    gin = new_ddcf_node (fg, ND_G_INPUT, 0, 1, fg->nodes[ext_nd].loc);
    fg->nodes[gin].Io_num = 0;
    fg->nodes[gin].My_graph = switch_node;
    fg->nodes[switch_node].My_inputs[0] = gin;
    fg->nodes[switch_node].My_outputs = make_io_array (1);
    gout = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, fg->nodes[ext_nd].loc);
    fg->nodes[gout].Io_num = 0;
    fg->nodes[gout].My_graph = switch_node;
    fg->nodes[switch_node].My_outputs[0] = gout;
    add_to_list (switch_node, &its);
    add_to_list (gin, &(fg->nodes[switch_node].My_nodes));
    set_type_to_int (&(fg->nodes[gin].outputs[0].ty), 32);
    set_type_to_int (&(fg->nodes[gout].inputs[0].ty), 32);
    set_type_to_int (&(fg->nodes[switch_node].outputs[0].ty), 32);
    set_type_to_int (&(fg->nodes[switch_node].inputs[0].ty), 32);

    /* create the key */
    cmp_node = new_ddcf_node (fg, ND_GE, 2, 1, fg->nodes[ext_nd].loc);
    add_edge (fg, gin, 0, cmp_node, 0);
    fg->nodes[cmp_node].inputs[1].is_const = TRUE;
    sprintf (str, "%d", tile_size);
    fg->nodes[cmp_node].inputs[1].constval = copy_to_const (str);
    key_node = new_ddcf_node (fg, ND_SWITCH_KEY, 1, 0, fg->nodes[ext_nd].loc);
    add_edge (fg, cmp_node, 0, key_node, 0);
    append_intlist (&(fg->nodes[switch_node].My_nodes), cmp_node);
    append_intlist (&(fg->nodes[switch_node].My_nodes), key_node);
    set_type_to_int (&(fg->nodes[cmp_node].inputs[0].ty), 32);
    set_type_to_int (&(fg->nodes[cmp_node].inputs[1].ty), 32);
    set_type_to_int (&(fg->nodes[cmp_node].outputs[0].ty), 32);
    set_type_to_int (&(fg->nodes[key_node].inputs[0].ty), 32);

    /* create the true case */
    case_node = new_ddcf_node (fg, ND_CASE, 1, 1, fg->nodes[ext_nd].loc);
    fg->nodes[case_node].My_inputs = make_io_array (1);
    cgin = new_ddcf_node (fg, ND_G_INPUT, 0, 1, fg->nodes[ext_nd].loc);
    fg->nodes[cgin].Io_num = 0;
    fg->nodes[cgin].My_graph = case_node;
    fg->nodes[case_node].My_inputs[0] = cgin;
    fg->nodes[case_node].My_outputs = make_io_array (1);
    cgout = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, fg->nodes[ext_nd].loc);
    fg->nodes[cgout].Io_num = 0;
    fg->nodes[cgout].My_graph = case_node;
    fg->nodes[case_node].My_outputs[0] = cgout;
    append_intlist (&(fg->nodes[case_node].My_nodes), cgin);
    append_intlist (&(fg->nodes[switch_node].My_nodes), case_node);
    set_type_to_int (&(fg->nodes[case_node].inputs[0].ty), 32);
    set_type_to_int (&(fg->nodes[case_node].outputs[0].ty), 32);
    set_type_to_int (&(fg->nodes[cgin].outputs[0].ty), 32);
    set_type_to_int (&(fg->nodes[cgout].inputs[0].ty), 32);

    sw_sel = new_ddcf_node (fg, ND_SELECTORS, 1, 0, fg->nodes[ext_nd].loc);
    fg->nodes[sw_sel].inputs[0].is_const = TRUE;
    strcpy (str, "true");
    fg->nodes[sw_sel].inputs[0].constval = copy_to_const (str);
    append_intlist (&(fg->nodes[case_node].My_nodes), sw_sel);
    set_type_to_bool (&(fg->nodes[sw_sel].inputs[0].ty));

    sub_node = new_ddcf_node (fg, ND_SUB, 2, 1, fg->nodes[ext_nd].loc);
    add_edge (fg, cgin, 0, sub_node, 0);
    fg->nodes[sub_node].inputs[1].is_const = TRUE;
    sprintf (str, "%d", tile_size);
    fg->nodes[sub_node].inputs[1].constval = copy_to_const (str);
    append_intlist (&(fg->nodes[case_node].My_nodes), sub_node);
    set_type_to_int (&(fg->nodes[sub_node].inputs[0].ty), 32);
    set_type_to_int (&(fg->nodes[sub_node].inputs[1].ty), 32);
    set_type_to_int (&(fg->nodes[sub_node].outputs[0].ty), 32);

    mod_node = new_ddcf_node (fg, ND_MOD, 2, 1, fg->nodes[ext_nd].loc);
    add_edge (fg, sub_node, 0, mod_node, 0);
    fg->nodes[mod_node].inputs[1].is_const = TRUE;
    sprintf (str, "%d", outer_step);
    fg->nodes[mod_node].inputs[1].constval = copy_to_const (str);
    append_intlist (&(fg->nodes[case_node].My_nodes), mod_node);
    set_type_to_int (&(fg->nodes[mod_node].inputs[0].ty), 32);
    set_type_to_int (&(fg->nodes[mod_node].inputs[1].ty), 32);
    set_type_to_int (&(fg->nodes[mod_node].outputs[0].ty), 32);

    add_node = new_ddcf_node (fg, ND_ADD, 2, 1, fg->nodes[ext_nd].loc);
    add_edge (fg, mod_node, 0, add_node, 0);
    fg->nodes[add_node].inputs[1].is_const = TRUE;
    sprintf (str, "%d", tile_size);
    fg->nodes[add_node].inputs[1].constval = copy_to_const (str);
    append_intlist (&(fg->nodes[case_node].My_nodes), add_node);
    set_type_to_int (&(fg->nodes[add_node].inputs[0].ty), 32);
    set_type_to_int (&(fg->nodes[add_node].inputs[1].ty), 32);
    set_type_to_int (&(fg->nodes[add_node].outputs[0].ty), 32);

    ssub_node = new_ddcf_node (fg, ND_SUB, 2, 1, fg->nodes[ext_nd].loc);
    add_edge (fg, cgin, 0, ssub_node, 0);
    add_edge (fg, add_node, 0, ssub_node, 1);
    append_intlist (&(fg->nodes[case_node].My_nodes), ssub_node);
    set_type_to_int (&(fg->nodes[ssub_node].inputs[0].ty), 32);
    set_type_to_int (&(fg->nodes[ssub_node].inputs[1].ty), 32);
    set_type_to_int (&(fg->nodes[ssub_node].outputs[0].ty), 32);

    aadd_node = new_ddcf_node (fg, ND_ADD, 2, 1, fg->nodes[ext_nd].loc);
    add_edge (fg, ssub_node, 0, aadd_node, 0);
    fg->nodes[aadd_node].inputs[1].is_const = TRUE;
    sprintf (str, "%d", outer_step);
    fg->nodes[aadd_node].inputs[1].constval = copy_to_const (str);
    append_intlist (&(fg->nodes[case_node].My_nodes), aadd_node);
    set_type_to_int (&(fg->nodes[aadd_node].inputs[0].ty), 32);
    set_type_to_int (&(fg->nodes[aadd_node].inputs[1].ty), 32);
    set_type_to_int (&(fg->nodes[aadd_node].outputs[0].ty), 32);
    add_edge (fg, aadd_node, 0, cgout, 0);

    add_edge (fg, gin, 0, case_node, 0);
    add_edge (fg, case_node, 0, gout, 0);

    /* create the false case */
    case_node = new_ddcf_node (fg, ND_CASE, 0, 1, fg->nodes[ext_nd].loc);
    fg->nodes[case_node].My_outputs = make_io_array (1);
    cgout = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, fg->nodes[ext_nd].loc);
    fg->nodes[cgout].Io_num = 0;
    fg->nodes[cgout].My_graph = case_node;
    fg->nodes[case_node].My_outputs[0] = cgout;
    fg->nodes[cgout].inputs[0].is_const = TRUE;
    strcpy (str, "0");
    fg->nodes[cgout].inputs[0].constval = copy_to_const (str);
    append_intlist (&(fg->nodes[switch_node].My_nodes), case_node);
    set_type_to_int (&(fg->nodes[case_node].outputs[0].ty), 32);
    set_type_to_int (&(fg->nodes[cgout].inputs[0].ty), 32);

    add_edge (fg, case_node, 0, gout, 0);

    /* connect the external input of the switch node */
    add_edge (fg, ext_nd, ext_pt, switch_node, 0);
    *res_lo_nd = switch_node;
    *res_lo_pt = 0;

    /* create the subtract for the 'hi' value */
    sub_node = new_ddcf_node (fg, ND_SUB, 2, 1, fg->nodes[ext_nd].loc);
    add_edge (fg, ext_nd, ext_pt, sub_node, 0);
    fg->nodes[sub_node].inputs[1].is_const = TRUE;
    strcpy (str, "1");
    fg->nodes[sub_node].inputs[1].constval = copy_to_const (str);
    add_to_list (sub_node, &its);
    set_type_to_int (&(fg->nodes[sub_node].inputs[0].ty), 32);
    set_type_to_int (&(fg->nodes[sub_node].inputs[1].ty), 32);
    set_type_to_int (&(fg->nodes[sub_node].outputs[0].ty), 32);
    *res_hi_nd = sub_node;
    *res_hi_pt = 0;

    return its;
    }

/* remove any size info from a loop's outputs */
void clear_size_info_from_outputs (FuncGraph *fg, int nd)
    {
    int i, gout, snd, spt;

    for (i=0; i<fg->nodes[nd].num_outputs; i++)
        {
	clear_dim_sizes (&(fg->nodes[nd].outputs[i].ty));
	gout = fg->nodes[nd].My_outputs[i];
	clear_dim_sizes (&(fg->nodes[gout].inputs[0].ty));
	snd = fg->nodes[gout].inputs[0].back_edges->node;
	spt = fg->nodes[gout].inputs[0].back_edges->port;
	clear_dim_sizes (&(fg->nodes[snd].outputs[spt].ty));
	}
    }

void clear_size_info_from_loop (FuncGraph *fg, int nd)
    {
    int i;

    for (i=0; i<MAXRANK; i++)
        fg->nodes[nd].dim_sizes[i] = -1;
    }

int generators_are_different (FuncGraph *fg, int g0, int g1)
    {
    int i, snd, spt;

    if (fg->nodes[g0].num_inputs != fg->nodes[g1].num_inputs)
        return TRUE;

    /* all but input[0] must be compared */
    for (i=1; i<fg->nodes[g0].num_inputs; i++)
        if (fg->nodes[g0].inputs[i].is_const)
	    {
	    if (fg->nodes[g1].inputs[i].is_const)
		{
	        if (strcmp (fg->nodes[g0].inputs[i].constval,
				fg->nodes[g1].inputs[i].constval) != 0)
		    return TRUE;
		}
	    else
	        return TRUE;
	    }
	else
	    {
	    snd = fg->nodes[g0].inputs[i].back_edges->node;
	    spt = fg->nodes[g0].inputs[i].back_edges->port;

	    if (fg->nodes[g1].inputs[i].back_edges->node != snd)
	        return TRUE;
	    if (fg->nodes[g1].inputs[i].back_edges->port != spt)
	        return TRUE;
	    }

    return FALSE;
    }
