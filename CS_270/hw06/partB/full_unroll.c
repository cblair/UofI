#include "sassy.h"
#include "ddcf.h"
#include "error.h"

typedef struct out_xyz {
    TypeInfo ty;
    int is_const;
    char constval[256];
    int node;
    int port;
    int seq_next;
    struct out_xyz *link;
    } OutXyz;

int get_loop_dimensions (FuncGraph*, int, int*);
void srch_full_unroll (FuncGraph*, int);
int unroll_is_possible (FuncGraph*, int, int*, int*);
void full_unroll_forall (FuncGraph*, int, int, IntList**, int*);
int create_a_loop_body (FuncGraph*, int, OutXyz**);
void convert_simple_gens_to_arrayrefs (FuncGraph*, int);
void hook_up_outputs (FuncGraph*, int, int, int*, int*, IntList**, OutXyz*);
void hook_up_inputs (FuncGraph*, int, int, int*, int*, OutXyz*);
int hook_up_inputs_elegen (FuncGraph*, int, int, int*, int, int*, int, int);
int hook_up_inputs_slicegen (FuncGraph*, int, int, int*, int, int*, int, int);
int hook_up_inputs_windowgen (FuncGraph*, int, int, int*, int, int*, int, int);
int hook_up_inputs_scalargen (FuncGraph*, int, int, int*, int, int*, int, int);
void append_outxyz (OutXyz**, OutXyz*);
int find_nxt_out_port (OutXyz*, int);
void lift_graph (FuncGraph*, int, IntList**);
void insert_dummy_nodes (FuncGraph*, int);
void eleslicegen_index_list1 (FuncGraph*, int, int*, int, int*);
void eleslicegen_index_list2 (FuncGraph*, int, int*, int, int*);
void eleslicegen_index_list3 (FuncGraph*, int, int*, int, int*);
void windowgen_index_list1 (FuncGraph*, int, int*, int, int*);
void windowgen_index_list2 (FuncGraph*, int, int*, int, int*);
void windowgen_index_list3 (FuncGraph*, int, int*, int, int*);
void scalargen_index_list1 (FuncGraph*, int, int*, int, int*);
void scalargen_index_list2 (FuncGraph*, int, int*, int, int*);
void scalargen_index_list3 (FuncGraph*, int, int*, int, int*);
void loop_indices_index_list1 (FuncGraph*, int, int*, int, int*);
void loop_indices_index_list2 (FuncGraph*, int, int*, int, int*);
void loop_indices_index_list3 (FuncGraph*, int, int*, int, int*);
void loop_indices_index_list4 (FuncGraph*, int, int*, int, int*);
void loop_indices_index_list5 (FuncGraph*, int, int*, int, int*);
void loop_indices_index_list6 (FuncGraph*, int, int*, int, int*);
void loop_indices_index_list7 (FuncGraph*, int, int*, int, int*);
void loop_indices_index_list8 (FuncGraph*, int, int*, int, int*);
int assoc_reduction (FuncGraph*, int, int, int, int, int*, IntList**, DdcfType);
int hist_reduction (FuncGraph*, int, int, int, int, int*, IntList**);
int vals_at_mxxs_reduction (FuncGraph*, int, int, int, int, int*, int, IntList**, DdcfType);
int vals_at_one_mxx_reduction (FuncGraph*, int, int, int, int, int*, int, IntList**, DdcfType);
void free_oxyz_list (OutXyz**);
int hook_up_inputs_loop_indices (FuncGraph*, int, int, int*, int, int*, int);

void full_unroll ()
    {
    FuncGraph *fg;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
	    srch_full_unroll (fg, 0);
    }

void srch_full_unroll (FuncGraph *fg, int id)
    {
    IntList *it, *my_copy;
    int nd, szs[MAXRANK], i;
    int iter_cnt;

    /* we send a copy of this list, which can be modifed without affecting the original */
    copy_intlist (&my_copy, &(fg->nodes[id].My_nodes));

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;

	/* recursively search */
	if (is_compound (fg->nodes[nd].nodetype))
	    srch_full_unroll (fg, nd);

	if ((fg->nodes[nd].nodetype == ND_FORALL) || (fg->nodes[nd].nodetype == ND_FORNXT))
	    {
    	    for (i=0; i<MAXRANK; i++)
        	szs[i] = -1;

	    if (unroll_is_possible (fg, nd, &iter_cnt, szs))
	        {
		if (!quiet)
		    printf ("  full unroll (%d iterations) of loop on line %d, function '%s', file '%s'\n",
		    		iter_cnt, fg->nodes[nd].loc.line, fg->nodes[nd].loc.func, fg->nodes[nd].loc.file);

		full_unroll_forall (fg, nd, iter_cnt, &my_copy, szs);
		}
	    }
	}

    /* now replace the original list with the modified one */
    free_intlist (&(fg->nodes[id].My_nodes));
    fg->nodes[id].My_nodes = my_copy;
    }

int unroll_is_possible (FuncGraph *fg, int nd, int *iter_cnt, int *szs)
    {
    IntList *it;
    int rank, id, i, sizes_known;

    if (has_pragma (fg, nd, PRAG_NO_UNROLL))
	return FALSE;

    rank = get_loop_dimensions (fg, nd, szs);
    *iter_cnt = 1;
    sizes_known = TRUE;

    for (i=0; i<rank; i++)
        if (szs[i] < 0)
	    {
	    sizes_known = FALSE;
	    break;
	    }
	else
	    *iter_cnt *= szs[i];

    /* can't full unroll if loop extents not known */
    if (!sizes_known)
        return FALSE;

    if (*iter_cnt == 0)
        return FALSE;

    for (it=fg->nodes[nd].My_nodes; it!=NULL; it=it->link)
        {
	id = it->val;

	switch (fg->nodes[id].nodetype)
	    {
	    case ND_REDUCE_MEAN :
	    case ND_REDUCE_ST_DEV :
	    case ND_REDUCE_MODE :
	    case ND_REDUCE_PRODUCT :
	    case ND_CONSTRUCT_CONCAT :
	    case ND_CONSTRUCT_CONCAT_MASKED :
	    case ND_CONSTRUCT_TILE :
	        return FALSE;
	    case ND_CONSTRUCT_ARRAY :
		if (fg->nodes[id].inputs[0].ty.kind == Array)
	            return FALSE;
		break;
	    case ND_CROSS_PROD :
	    case ND_DOT_PROD :
		{
		IntList *itt;
		int idd;

		for (itt=fg->nodes[id].My_nodes; itt!=NULL; itt=itt->link)
		    {
		    idd = itt->val;

		    switch (fg->nodes[idd].nodetype)
		        {
			case ND_SCALAR_GEN :
			    for (i=0; i<fg->nodes[idd].num_inputs; i++)
			        if (!fg->nodes[idd].inputs[i].is_const)
				    return FALSE;
			    break;
			case ND_LOOP_INDICES :
			case ND_WINDOW_GEN :
			    for (i=1; i<fg->nodes[idd].num_inputs; i++)
			        if (!fg->nodes[idd].inputs[i].is_const)
				    return FALSE;
			    break;
	    		case ND_ELE_GEN :
			case ND_SLICE_GEN:
			    for (i=1; i<fg->nodes[idd].num_inputs; i++)
			        if (!fg->nodes[idd].inputs[i].is_const)
				    return FALSE;
			    break;
			default :
                            break;
			}
		    }
		}
	        break;
	    default :
                break;
	    }
	}

    return TRUE;
    }

/*  'nd'        - the id of the loop graph
 *  'iter_cnt'  - the number of iterations of the loop
 *  'parent'    - the id of the graph containing the loop
 *  'szs'       - the loop's extents
 */
void full_unroll_forall (FuncGraph *fg, int nd, int iter_cnt, IntList **parent_list, int *szs)
    {
    IntList *loc_list=NULL;
    OutXyz *out_stack=NULL;
    int i;
    int *bodies;

    is_stable = FALSE;

    SacMalloc (bodies, int*, iter_cnt * sizeof (int))

    /* create a loop body */
    bodies[0] = create_a_loop_body (fg, nd, &out_stack);
    add_to_list (bodies[0], &loc_list);

    /* make more copies of the loop body */
    for (i=1; i<iter_cnt; i++)
	{
        bodies[i] = copy_graph (fg, fg, bodies[0]);
        append_intlist (&loc_list, bodies[i]);
	}

    hook_up_inputs (fg, nd, iter_cnt, bodies, szs, out_stack);

    hook_up_outputs (fg, nd, iter_cnt, bodies, szs, parent_list, out_stack);

    /* replace the original loop node in the list with the new loop bodies */
    replace_val_with_list (nd, parent_list, &loc_list);

    /* lift the loop bodies up into this graph */
    for (i=0; i<iter_cnt; i++)
	lift_graph (fg, bodies[i], parent_list);

    /* destroy the original loop */
    for (i=0; i<fg->nodes[nd].num_inputs; i++)
        {
        remove_targets_to_this_port (fg, fg->nodes[nd].inputs[i].back_edges, nd, i);
	free_edge_list (&(fg->nodes[nd].inputs[i].back_edges));
	}

    for (i=0; i<fg->nodes[nd].num_outputs; i++)
        {
	remove_backedges_to_this_target (fg, fg->nodes[nd].outputs[i].targets, nd, i);
	free_edge_list (&(fg->nodes[nd].outputs[i].targets));
	}

    destroy_graph (fg, nd);

    SacFree (bodies)
    free_oxyz_list (&out_stack);
    }

/*  'nd' is the original loop node
 *  'iter_cnt' tell show many loop bodies there are
 *  'bodies' is the array of loop body ids
 *  'szs' is the array of extents of the unrolled loop
 *  'oxyz' is information about the output ports of the loop bodies
 *
 *  In the case of a FORALL loop, it's a matter of connecting all the inputs
 *  to the same sources. In the case of a FORNXT, the nextified inputs and
 *  outputs have to be threaded from one loop body to the next.
 */
void hook_up_inputs (FuncGraph *fg, int nd,
		int iter_cnt, int *bodies, int *szs, OutXyz *oxyz)
    {
    IntList *it;
    int gen_node, id, i, j, node, port, node1, port1, g_in;
    int pos=0, port_pos, nxt_cnt, in_port, out_port;

    /* first, duplicate the inputs from the original loop node */
    for (i=0; i<fg->nodes[nd].num_inputs; i++)
	{
	g_in = fg->nodes[nd].My_inputs[i];

	/*  if it's a nextified input, we connect only to the
	 *  first loop body; otherwise we connect it to every
	 *  loop body;
	 */
	if (fg->nodes[g_in].nodetype == ND_G_INPUT_NEXT)
	    {
	    node = fg->nodes[nd].inputs[i].back_edges->node;
	    port = fg->nodes[nd].inputs[i].back_edges->port;

	    if (fg->nodes[node].inputs[0].is_const)
	        {
		fg->nodes[bodies[0]].inputs[i].is_const = TRUE;
		fg->nodes[bodies[0]].inputs[i].constval = copy_to_const (fg->nodes[node].inputs[0].constval);
		copy_type_info (&(fg->nodes[bodies[0]].inputs[i].ty),
					&(fg->nodes[node].inputs[0].ty));
		}
	    else
	        {
	        node1 = fg->nodes[node].inputs[0].back_edges->node;
	        port1 = fg->nodes[node].inputs[0].back_edges->port;
	        add_edge (fg, node1, port1, bodies[0], i);
	        copy_type_info (&(fg->nodes[bodies[0]].inputs[i].ty),
	    				&(fg->nodes[node1].outputs[port1].ty));
		}
	    }
	else
	    {
	    node = fg->nodes[nd].inputs[i].back_edges->node;
	    port = fg->nodes[nd].inputs[i].back_edges->port;
            for (j=0; j<iter_cnt; j++)
	        {
	        add_edge (fg, node, port, bodies[j], i);
	        copy_type_info (&(fg->nodes[bodies[j]].inputs[i].ty),
	        			&(fg->nodes[node].outputs[port].ty));
	        }
	    }
	}

    /* if there are nextified inputs, the edges must thread from one body
     * to the next;
     */
    for (nxt_cnt=0, it=fg->nodes[nd].My_nodes; it!=NULL; it=it->link)
        {
	id = it->val;

	if (fg->nodes[id].nodetype == ND_NEXT)
	    {
	    g_in = fg->nodes[id].In_next_id;
	    in_port = fg->nodes[g_in].Io_num;
	    out_port = find_nxt_out_port (oxyz, nxt_cnt);

	    for (i=0; i<iter_cnt-1; i++)
		{
	        add_edge (fg, bodies[i], out_port, bodies[i+1], in_port);
		}

	    nxt_cnt++;
	    }
	}

    /* now put the constants on the additional inputs */
    port_pos = fg->nodes[nd].num_inputs;

    gen_node = find_generator_graph (fg, nd);

    /* find each of the simple generators and produce its index stream */
    for (it=fg->nodes[gen_node].My_nodes; it!=NULL; it=it->link)
	{
	id = it->val;

        switch (fg->nodes[id].nodetype)
	    {
	    case ND_ELE_GEN :
		port_pos = hook_up_inputs_elegen (fg, nd, iter_cnt, bodies,
			id, szs+pos, port_pos, fg->nodes[gen_node].nodetype==ND_CROSS_PROD);
		if (fg->nodes[gen_node].nodetype==ND_CROSS_PROD)
		  pos += fg->nodes[id].inputs[0].ty.dims;
	        break;
	    case ND_SLICE_GEN :
		port_pos = hook_up_inputs_slicegen (fg, nd, iter_cnt, bodies,
			id, szs+pos, port_pos, fg->nodes[gen_node].nodetype==ND_CROSS_PROD);
		if (fg->nodes[gen_node].nodetype==ND_CROSS_PROD)
		  pos += fg->nodes[id].inputs[0].ty.dims - fg->nodes[id].outputs[0].ty.dims;
	        break;
	    case ND_WINDOW_GEN :
		port_pos = hook_up_inputs_windowgen (fg, nd, iter_cnt, bodies,
			id, szs+pos, port_pos, fg->nodes[gen_node].nodetype==ND_CROSS_PROD);
		if (fg->nodes[gen_node].nodetype==ND_CROSS_PROD)
		    pos += fg->nodes[id].inputs[0].ty.dims;
	        break;
	    case ND_SCALAR_GEN :
		port_pos = hook_up_inputs_scalargen (fg, nd, iter_cnt, bodies,
			id, szs+pos, port_pos, fg->nodes[gen_node].nodetype==ND_CROSS_PROD);
		if (fg->nodes[gen_node].nodetype==ND_CROSS_PROD)
		    pos += fg->nodes[id].num_outputs;
	        break;
	    case ND_LOOP_INDICES :
		port_pos = hook_up_inputs_loop_indices (fg,
				nd, iter_cnt, bodies, id, szs, port_pos);
	        break;
	    default :
                break;
	    }
	}
    }

/*  search through the 'oxyz' list, looking for the 'n'th nextified output */
int find_nxt_out_port (OutXyz *oxyz, int nxt_cnt)
    {
    OutXyz *ox;
    int i;

    for (i=0, ox=oxyz; ox!=NULL; i++, ox=ox->link)
        if (ox->seq_next == nxt_cnt)
	    return i;

    assert (0);
    }

int hook_up_inputs_loop_indices (FuncGraph *fg, int nd,
		int iter_cnt, int *bodies, int id, int *szs, int port_pos)
    {
    int rank, i, j;
    int tot_sz, *indices, pidx, multiplier;

    rank = fg->nodes[id].num_outputs;

    /* get set to determine the stream of indices */
    for (i=0, tot_sz=1; i<rank; i++)
        tot_sz *= szs[i];

    multiplier = 1;
    SacMalloc (indices, int*, tot_sz * rank * multiplier * sizeof (int))

    /* get the stream of indices */
    switch (rank)
        {
	case 1 :
	    loop_indices_index_list1 (fg, id, szs, multiplier, indices);
	    break;
	case 2 :
	    loop_indices_index_list2 (fg, id, szs, multiplier, indices);
	    break;
	case 3 :
	    loop_indices_index_list3 (fg, id, szs, multiplier, indices);
	    break;
	case 4 :
	    loop_indices_index_list4 (fg, id, szs, multiplier, indices);
	    break;
	case 5 :
	    loop_indices_index_list5 (fg, id, szs, multiplier, indices);
	    break;
	case 6 :
	    loop_indices_index_list6 (fg, id, szs, multiplier, indices);
	    break;
	case 7 :
	    loop_indices_index_list7 (fg, id, szs, multiplier, indices);
	    break;
	case 8 :
	    loop_indices_index_list8 (fg, id, szs, multiplier, indices);
	    break;
	default :
            break;
	}

    /* go through all the loop bodies, and all the index inputs, and install
     * the appropriate constant value
     */
    for (i=0, pidx=0; i<iter_cnt; i++)
        {
	for (j=port_pos; j<port_pos+rank; j++)
	    {
	    int cval = indices[pidx];
	    char tmp[128];

	    fg->nodes[bodies[i]].inputs[j].is_const = TRUE;
	    sprintf (tmp, "%d", cval);
	    fg->nodes[bodies[i]].inputs[j].constval = copy_to_const (tmp);
	    fg->nodes[bodies[i]].inputs[j].ty.type = Uint;
	    fg->nodes[bodies[i]].inputs[j].ty.kind = Scalar;
	    fg->nodes[bodies[i]].inputs[j].ty.totsize = 32;
	    fg->nodes[bodies[i]].inputs[j].ty.fracsize = 0;

	    pidx++;
	    }

	/* to get wrap-around */
	pidx = pidx % (tot_sz*rank*multiplier);
	}

    SacFree (indices)

    return port_pos + rank;
    }

/*  'nd' is the original loop node;
 *  'iter_cnt' is the number of loop bodies
 *  'bodies' is the array of loop body ids
 *  'parent_list' is the node list of the parent grah containing the original loop
 *  'id' is a ND_WINDOW_GEN node
 *  'szs' is an array of the loop's extents
 *  'port_pos' tells which input port we're currently up to
 *  'is_cross' indicates whether this is inside of a cross product
 */
int hook_up_inputs_windowgen (FuncGraph *fg, int nd, int iter_cnt,
		int *bodies, int id, int *szs, int port_pos, int is_cross)
    {
    int rank, i, j, k;
    int tot_sz, *indices, pidx, multiplier, win_sizes[MAXRANK];

    rank = fg->nodes[id].inputs[0].ty.dims;

    /* get the window dimensions */
    for (j=0, i=1; j<rank; j++, i+=2)
        sscanf (fg->nodes[id].inputs[i].constval, "%d", &(win_sizes[j]));

    /* get set to determine the stream of indices */
    for (i=0, tot_sz=1; i<rank; i++)
        tot_sz *= szs[i];

    if (is_cross)
        for (i=rank, multiplier=1; szs[i]>=0; i++)
            multiplier *= szs[i];
    else
        multiplier = 1;

    SacMalloc (indices, int*, tot_sz * rank * multiplier * sizeof (int))

    /* get the stream of indices */
    switch (rank)
        {
	case 1 :
	    windowgen_index_list1 (fg, id, szs, multiplier, indices);
	    break;
	case 2 :
	    windowgen_index_list2 (fg, id, szs, multiplier, indices);
	    break;
	case 3 :
	    windowgen_index_list3 (fg, id, szs, multiplier, indices);
	    break;
	default :
	    assert (0);
	}

    /* go through all the loop bodies, and all the index inputs, and install
     * the appropriate constant values
     */
    for (i=0, pidx=0; i<iter_cnt; i++)
        {
	for (j=port_pos, k=0; j<port_pos+2*rank; j+=3, k++)
	    {
	    int cval = indices[pidx];
	    char tmp[128];

	    fg->nodes[bodies[i]].inputs[j].is_const = TRUE;
	    sprintf (tmp, "%d", cval);
	    fg->nodes[bodies[i]].inputs[j].constval = copy_to_const (tmp);
	    fg->nodes[bodies[i]].inputs[j].ty.type = Uint;
	    fg->nodes[bodies[i]].inputs[j].ty.kind = Scalar;
	    fg->nodes[bodies[i]].inputs[j].ty.totsize = 32;
	    fg->nodes[bodies[i]].inputs[j].ty.fracsize = 0;

	    fg->nodes[bodies[i]].inputs[j+1].is_const = TRUE;
	    sprintf (tmp, "%d", cval+win_sizes[k]-1);
	    fg->nodes[bodies[i]].inputs[j+1].constval = copy_to_const (tmp);
	    fg->nodes[bodies[i]].inputs[j+1].ty.type = Uint;
	    fg->nodes[bodies[i]].inputs[j+1].ty.kind = Scalar;
	    fg->nodes[bodies[i]].inputs[j+1].ty.totsize = 32;
	    fg->nodes[bodies[i]].inputs[j+1].ty.fracsize = 0;

	    fg->nodes[bodies[i]].inputs[j+2].is_const = TRUE;
	    sprintf (tmp, "%d", 1);
	    fg->nodes[bodies[i]].inputs[j+2].constval = copy_to_const (tmp);
	    fg->nodes[bodies[i]].inputs[j+2].ty.type = Uint;
	    fg->nodes[bodies[i]].inputs[j+2].ty.kind = Scalar;
	    fg->nodes[bodies[i]].inputs[j+2].ty.totsize = 32;
	    fg->nodes[bodies[i]].inputs[j+2].ty.fracsize = 0;

	    pidx++;
	    }

	/* to get wrap-around */
	pidx = pidx % (tot_sz*rank*multiplier);
	}

    SacFree (indices)

    return port_pos + 3*rank;
    }

/*  'nd' is the original loop node;
 *  'iter_cnt' is the number of loop bodies
 *  'bodies' is the array of loop body ids
 *  'parent_list' is the node list of the parent grah containing the original loop
 *  'id' is a ND_ELE_GEN node
 *  'szs' is an array of the loop's extents
 *  'port_pos' tells which input port we're currently up to
 *  'is_cross' indicates whether this is inside of a cross product
 */
int hook_up_inputs_elegen (FuncGraph *fg, int nd, int iter_cnt,
		int *bodies, int id, int *szs, int port_pos, int is_cross)
    {
    int in_rank, out_rank, gen_rank, i, j, k;
    int tot_sz, *indices, pidx, multiplier;

    in_rank = fg->nodes[id].inputs[0].ty.dims;
    out_rank = 0;
    gen_rank = in_rank - out_rank;

    /* get set to determine the stream of indices */
    for (i=0, tot_sz=1; i<gen_rank; i++)
        tot_sz *= szs[i];

    if (is_cross)
        for (i=gen_rank, multiplier=1; szs[i]>=0; i++)
            multiplier *= szs[i];
    else
        multiplier = 1;

    SacMalloc (indices, int*, tot_sz * gen_rank * multiplier * sizeof (int))

    /* get the stream of indices */
    switch (gen_rank)
        {
	case 1 :
	    eleslicegen_index_list1 (fg, id, szs, multiplier, indices);
	    break;
	case 2 :
	    eleslicegen_index_list2 (fg, id, szs, multiplier, indices);
	    break;
	case 3 :
	    eleslicegen_index_list3 (fg, id, szs, multiplier, indices);
	    break;
	default :
	    assert (0);
	}

    /* go through all the loop bodies, and all the index inputs, and install
     * the appropriate constant value
     */
    for (i=0, pidx=0; i<iter_cnt; i++)
        {
	for (k=0, j=port_pos; k<in_rank; k++)
	  {
	    int cval = indices[pidx];
	    char tmp[128];
	    
	    fg->nodes[bodies[i]].inputs[j].is_const = TRUE;
	    sprintf (tmp, "%d", cval);
	    fg->nodes[bodies[i]].inputs[j].constval = copy_to_const (tmp);
	    fg->nodes[bodies[i]].inputs[j].ty.type = Uint;
	    fg->nodes[bodies[i]].inputs[j].ty.kind = Scalar;
	    fg->nodes[bodies[i]].inputs[j].ty.totsize = 32;
	    fg->nodes[bodies[i]].inputs[j].ty.fracsize = 0;
	    
	    pidx++;
	    j++;
	  }
	
	/* to get wrap-around */
	pidx = pidx % (tot_sz*gen_rank*multiplier);
	}

    SacFree (indices)

    return port_pos + gen_rank;
    }

/*  'nd' is the original loop node;
 *  'iter_cnt' is the number of loop bodies
 *  'bodies' is the array of loop body ids
 *  'parent_list' is the node list of the parent grah containing the original loop
 *  'id' is a ND_SLICE_GEN node
 *  'szs' is an array of the loop's extents
 *  'port_pos' tells which input port we're currently up to
 *  'is_cross' indicates whether this is inside of a cross product
 */
int hook_up_inputs_slicegen (FuncGraph *fg, int nd, int iter_cnt,
		int *bodies, int id, int *szs, int port_pos, int is_cross)
    {
    int in_rank, out_rank, gen_rank, i, j, k;
    int tot_sz, *indices, pidx, multiplier;

    in_rank = fg->nodes[id].inputs[0].ty.dims;
    out_rank = fg->nodes[id].outputs[0].ty.dims;
    gen_rank = in_rank - out_rank;

    /* get set to determine the stream of indices */
    for (i=0, tot_sz=1; i<gen_rank; i++)
        tot_sz *= szs[i];

    if (is_cross)
        for (i=gen_rank, multiplier=1; szs[i]>=0; i++)
            multiplier *= szs[i];
    else
        multiplier = 1;

    SacMalloc (indices, int*, tot_sz * gen_rank * multiplier * sizeof (int))

    /* get the stream of indices */
    switch (gen_rank)
        {
	case 1 :
	    eleslicegen_index_list1 (fg, id, szs, multiplier, indices);
	    break;
	case 2 :
	    eleslicegen_index_list2 (fg, id, szs, multiplier, indices);
	    break;
	case 3 :
	    eleslicegen_index_list3 (fg, id, szs, multiplier, indices);
	    break;
	default :
	    assert (0);
	}

    /* go through all the loop bodies, and all the index inputs, and install
     * the appropriate constant value
     */
    for (i=0, pidx=0; i<iter_cnt; i++)
        {
	for (k=0, j=port_pos; k<in_rank; k++)
	    if (fg->nodes[id].Reftypes[k] == '~')
	        {
	        int cval = indices[pidx];
		char tmp[128];

	        fg->nodes[bodies[i]].inputs[j].is_const = TRUE;
	        sprintf (tmp, "%d", cval);
		fg->nodes[bodies[i]].inputs[j].constval = copy_to_const (tmp);
	        fg->nodes[bodies[i]].inputs[j].ty.type = Uint;
	        fg->nodes[bodies[i]].inputs[j].ty.kind = Scalar;
	        fg->nodes[bodies[i]].inputs[j].ty.totsize = 32;
	        fg->nodes[bodies[i]].inputs[j].ty.fracsize = 0;

	        pidx++;
		j++;
	        }

	/* to get wrap-around */
	pidx = pidx % (tot_sz*gen_rank*multiplier);
	}

    SacFree (indices)

    return port_pos + gen_rank;
    }

/*  'nd' is the original loop node;
 *  'iter_cnt' is the number of loop bodies
 *  'bodies' is the array of loop body ids
 *  'id' is a ND_SCALAR_GEN node
 *  'szs' is an array of the loop's extents
 *  'port_pos' tells which input port we're currently up to
 *  'is_cross' indicates whether this is inside of a cross product
 */
int hook_up_inputs_scalargen (FuncGraph *fg, int nd, int iter_cnt,
		int *bodies, int id, int *szs, int port_pos, int is_cross)
    {
    int rank, i, j;
    int tot_sz, *indices, pidx, multiplier;

    /*
    printf ("entering 'hook_up_inputs_scalargen'\n"); fflush (stdout);
    */

    rank = fg->nodes[id].num_outputs;

    /* get set to determine the stream of indices */
    for (i=0, tot_sz=1; i<rank; i++)
        tot_sz *= szs[i];

    if (is_cross)
        for (i=rank, multiplier=1; szs[i]>=0; i++)
            multiplier *= szs[i];
    else
        multiplier = 1;

    SacMalloc (indices, int*, tot_sz * rank * multiplier * sizeof (int))

    /* get the stream of indices */
    switch (rank)
        {
	case 1 :
	    scalargen_index_list1 (fg, id, szs, multiplier, indices);
	    break;
	case 2 :
	    scalargen_index_list2 (fg, id, szs, multiplier, indices);
	    break;
	case 3 :
	    scalargen_index_list3 (fg, id, szs, multiplier, indices);
	    break;
	default :
	    assert (0);
	}

    /* go through all the loop bodies, and all the index inputs, and install
     * the appropriate constant value
     */
    for (i=0, pidx=0; i<iter_cnt; i++)
        {
	for (j=port_pos; j<port_pos+rank; j++)
	    {
	    int cval = indices[pidx];
	    char tmp[128];

	    fg->nodes[bodies[i]].inputs[j].is_const = TRUE;
	    sprintf (tmp, "%d", cval);
	    fg->nodes[bodies[i]].inputs[j].constval = copy_to_const (tmp);
	    fg->nodes[bodies[i]].inputs[j].ty.type = Uint;
	    fg->nodes[bodies[i]].inputs[j].ty.kind = Scalar;
	    fg->nodes[bodies[i]].inputs[j].ty.totsize = 32;
	    fg->nodes[bodies[i]].inputs[j].ty.fracsize = 0;

	    pidx++;
	    }

	/* to get wrap-around */
	pidx = pidx % (tot_sz*rank*multiplier);
	}

    SacFree (indices)

    /*
    printf ("leaving 'hook_up_inputs_scalargen'\n"); fflush (stdout);
    */

    return port_pos + rank;
    }

/*  'nd' is the original FORALL loop
 *  'iter_cnt' is the number of loop bodies
 *  'bodies' is the array of loop body ids
 *  'parent_list' is the node list of the loop's parent graph
 *  'oxyz' is information about the loop body outputs
 *
 *  Outputs of a loop body are of two kinds:
 *	- one kind corresponds to an input of a reduce/construct node in
 *	  the original loop
 *	- another kind corresponds to a loop-carried value (i.e. associated
 *	  with a nextified variable)
 *  The reduce/construct outputs come first, then the nextified values.
 */
void hook_up_outputs (FuncGraph *fg, int nd, int iter_cnt, int *bodies, int *szs, IntList **parent_list, OutXyz *oxyz)
    {
    IntList *it;
    Edge *tg;
    int out_port=0, red_node, i, m, new_red_node, g_out;
    int nxt_cnt, nxt_node, node, port, new_out_port, loop_out_port, vsz;;

    /* first do the outputs associated with reduce/construct inputs */
    for (m=0; m<fg->nodes[nd].num_outputs; m++)
        {
	g_out = fg->nodes[nd].My_outputs[m];
	red_node = fg->nodes[g_out].inputs[0].back_edges->node;

	if (is_reduction (fg->nodes[red_node].nodetype))
	    {
	    switch (fg->nodes[red_node].nodetype)
	        {
	        case ND_REDUCE_VAL_AT_FIRST_MAX :
		    vsz = fg->nodes[red_node].VecSize;
		    new_red_node = vals_at_one_mxx_reduction (fg, iter_cnt, nd, red_node,
		    			out_port, bodies, vsz, parent_list, ND_REDUCE_VAL_AT_FIRST_MAX_MACRO);
		    out_port += vsz+2;
		    break;
	        case ND_REDUCE_VAL_AT_FIRST_MIN :
		    vsz = fg->nodes[red_node].VecSize;
		    new_red_node = vals_at_one_mxx_reduction (fg, iter_cnt, nd, red_node,
		    			out_port, bodies, vsz, parent_list, ND_REDUCE_VAL_AT_FIRST_MIN_MACRO);
		    out_port += vsz+2;
		    break;
	        case ND_REDUCE_VAL_AT_LAST_MAX :
		    vsz = fg->nodes[red_node].VecSize;
		    new_red_node = vals_at_one_mxx_reduction (fg, iter_cnt, nd, red_node,
		    			out_port, bodies, vsz, parent_list, ND_REDUCE_VAL_AT_LAST_MAX_MACRO);
		    out_port += vsz+2;
		    break;
	        case ND_REDUCE_VAL_AT_LAST_MIN :
		    vsz = fg->nodes[red_node].VecSize;
		    new_red_node = vals_at_one_mxx_reduction (fg, iter_cnt, nd, red_node,
		    			out_port, bodies, vsz, parent_list, ND_REDUCE_VAL_AT_LAST_MIN_MACRO);
		    out_port += vsz+2;
		    break;
	        case ND_REDUCE_VAL_AT_MAXS :
		    vsz = fg->nodes[red_node].VecSize;
		    new_red_node = vals_at_mxxs_reduction (fg, iter_cnt, nd, red_node,
		    			out_port, bodies, vsz, parent_list, ND_REDUCE_VAL_AT_MAXS_MACRO);
		    out_port += vsz+2;
		    break;
	        case ND_REDUCE_VAL_AT_MINS :
		    vsz = fg->nodes[red_node].VecSize;
		    new_red_node = vals_at_mxxs_reduction (fg, iter_cnt, nd, red_node,
		    			out_port, bodies, vsz, parent_list, ND_REDUCE_VAL_AT_MINS_MACRO);
		    out_port += vsz+2;
		    break;
	        case ND_REDUCE_SUM :
		    new_red_node = assoc_reduction (fg, iter_cnt, nd, red_node,
		    			out_port, bodies, parent_list, ND_REDUCE_SUM_MACRO);
		    out_port += 2;
		    break;
	        case ND_REDUCE_HIST :
		    new_red_node = hist_reduction (fg, iter_cnt, nd, red_node,
		    			out_port, bodies, parent_list);
		    out_port += 2;
		    break;
	        case ND_REDUCE_PRODUCT :
		    new_red_node = assoc_reduction (fg, iter_cnt, nd, red_node,
		    			out_port, bodies, parent_list, ND_REDUCE_PRODUCT_MACRO);
		    out_port += 2;
		    break;
	        case ND_REDUCE_MIN :
		    new_red_node = assoc_reduction (fg, iter_cnt, nd, red_node,
		    			out_port, bodies, parent_list, ND_REDUCE_MIN_MACRO);
		    out_port += 2;
		    break;
	        case ND_REDUCE_MAX :
		    new_red_node = assoc_reduction (fg, iter_cnt, nd, red_node,
		    			out_port, bodies, parent_list, ND_REDUCE_MAX_MACRO);
		    out_port += 2;
		    break;
	        case ND_REDUCE_AND :
		    new_red_node = assoc_reduction (fg, iter_cnt, nd, red_node,
		    			out_port, bodies, parent_list, ND_REDUCE_AND_MACRO);
		    out_port += 2;
		    break;
	        case ND_REDUCE_OR :
		    new_red_node = assoc_reduction (fg, iter_cnt, nd, red_node,
		    			out_port, bodies, parent_list, ND_REDUCE_OR_MACRO);
		    out_port += 2;
		    break;
	        case ND_REDUCE_MEDIAN :
		    new_red_node = assoc_reduction (fg, iter_cnt, nd, red_node,
		    			out_port, bodies, parent_list, ND_REDUCE_MEDIAN_MACRO);
		    out_port += 2;
		    break;
		case ND_CONSTRUCT_ARRAY :
		    {
		    int rank;
		    new_red_node = new_ddcf_node (fg, ND_ARR_DEF, iter_cnt, 1, fg->nodes[red_node].loc);
		    rank = fg->nodes[red_node].outputs[0].ty.dims;
		    fg->nodes[new_red_node].DefRank = rank;

		    SacMalloc (fg->nodes[new_red_node].DefDims, int*, rank * sizeof (int))

		    for (i=0; i<rank; i++)
		        fg->nodes[new_red_node].DefDims[i] = szs[i];

		    copy_type_info (&(fg->nodes[new_red_node].outputs[0].ty),
		    			&(fg->nodes[red_node].outputs[0].ty));
		    for (i=0; i<iter_cnt; i++)
		        {
			add_edge (fg, bodies[i], out_port, new_red_node, i);
			copy_type_info (&(fg->nodes[new_red_node].inputs[i].ty),
					&(fg->nodes[bodies[i]].outputs[out_port].ty));
			}
		    out_port++;

	    	    insert_val_after_in_intlist (new_red_node, nd, parent_list);
		    }
		    break;
	        default :
		    assert (0);
	        }

	    copy_target_list (&(fg->nodes[new_red_node].outputs[0].targets),
	    				fg->nodes[nd].outputs[m].targets);
	    install_back_edges_for_node (fg, new_red_node);
	    }
	}

    /* now do the outputs associated with nextified values */
    for (nxt_cnt=0, it=fg->nodes[nd].My_nodes; it!=NULL; it=it->link)
        {
	nxt_node = it->val;

	if (fg->nodes[nxt_node].nodetype == ND_NEXT)
	    {
	    if (!fg->nodes[nxt_node].inputs[0].is_const)
	        {
		node = fg->nodes[nxt_node].inputs[0].back_edges->node;
		port = fg->nodes[nxt_node].inputs[0].back_edges->port;

		for (tg=fg->nodes[node].outputs[port].targets; tg!=NULL; tg=tg->link)
		    {
		    g_out = tg->node;

		    if (fg->nodes[g_out].nodetype == ND_G_OUTPUT)
		        {
			loop_out_port = fg->nodes[g_out].Io_num;
			new_out_port = find_nxt_out_port (oxyz, nxt_cnt);
			fg->nodes[bodies[iter_cnt-1]].outputs[new_out_port].targets =
			    		fg->nodes[nd].outputs[loop_out_port].targets;
			fg->nodes[nd].outputs[loop_out_port].targets = NULL;
			retarget_backedges (fg,
			    fg->nodes[bodies[iter_cnt-1]].outputs[new_out_port].targets, nd,
			    loop_out_port, bodies[iter_cnt-1], new_out_port);
			}
		    }
		}

	    nxt_cnt++;
	    }
	}
    }


int vals_at_mxxs_reduction (FuncGraph *fg, int iter_cnt, int nd, int red_node,
		int out_port, int *bodies, int vsz, IntList **parent_list, DdcfType reduc_type)
    {
    int new_red_node, i, j, k, m;

    new_red_node = new_ddcf_node (fg, reduc_type, (2+vsz)*iter_cnt, 1, fg->nodes[nd].loc);
    fg->nodes[new_red_node].VecSize = vsz;
    copy_type_info (&(fg->nodes[new_red_node].outputs[0].ty),
		    		&(fg->nodes[red_node].outputs[0].ty));
		
    for (i=0, j=0; i<iter_cnt; i++)
        {
	k = out_port;
        add_edge (fg, bodies[i], k, new_red_node, j);
	copy_type_info (&(fg->nodes[new_red_node].inputs[j].ty),
					&(fg->nodes[bodies[i]].outputs[k].ty));
	j++;
	k++;

	for (m=0; m<vsz; m++)
	    {
            add_edge (fg, bodies[i], k, new_red_node, j);
	    copy_type_info (&(fg->nodes[new_red_node].inputs[j].ty),
					&(fg->nodes[bodies[i]].outputs[k].ty));
	    j++;
	    k++;
	    }

        add_edge (fg, bodies[i], k, new_red_node, j);
	copy_type_info (&(fg->nodes[new_red_node].inputs[j].ty),
					&(fg->nodes[bodies[i]].outputs[k].ty));

	j++;
	k++;
        }

    insert_val_after_in_intlist (new_red_node, nd, parent_list);

    return new_red_node;
    }

int vals_at_one_mxx_reduction (FuncGraph *fg, int iter_cnt, int nd, int red_node,
		int out_port, int *bodies, int vsz, IntList **parent_list, DdcfType reduc_type)
    {
    int new_red_node, new_arr_def_node, i, j, k, m;

    new_red_node = new_ddcf_node (fg, reduc_type, (2+vsz)*iter_cnt, vsz, fg->nodes[nd].loc);
    fg->nodes[new_red_node].VecSize = vsz;
    insert_val_after_in_intlist (new_red_node, nd, parent_list);

    new_arr_def_node = new_ddcf_node (fg, ND_ARR_DEF, vsz, 1, fg->nodes[nd].loc);
    fg->nodes[new_arr_def_node].DefRank = 1;
    SacMalloc (fg->nodes[new_arr_def_node].DefDims, int*, sizeof (int))
    fg->nodes[new_arr_def_node].DefDims[0] = vsz;
    insert_val_after_in_intlist (new_arr_def_node, new_red_node, parent_list);

    copy_type_info (&(fg->nodes[new_arr_def_node].outputs[0].ty),
		    		&(fg->nodes[red_node].outputs[0].ty));
		
    for (i=0, j=0; i<iter_cnt; i++)
        {
	k = out_port;
        add_edge (fg, bodies[i], k, new_red_node, j);
	copy_type_info (&(fg->nodes[new_red_node].inputs[j].ty),
					&(fg->nodes[bodies[i]].outputs[k].ty));
	j++;
	k++;

	for (m=0; m<vsz; m++)
	    {
            add_edge (fg, bodies[i], k, new_red_node, j);
	    copy_type_info (&(fg->nodes[new_red_node].inputs[j].ty),
					&(fg->nodes[bodies[i]].outputs[k].ty));
	    j++;
	    k++;
	    }

        add_edge (fg, bodies[i], k, new_red_node, j);
	copy_type_info (&(fg->nodes[new_red_node].inputs[j].ty),
					&(fg->nodes[bodies[i]].outputs[k].ty));

	j++;
	k++;
        }

    for (i=0; i<vsz; i++)
        {
	add_edge (fg, new_red_node, i, new_arr_def_node, i);
	copy_type_info (&(fg->nodes[new_red_node].outputs[i].ty), &(fg->nodes[new_arr_def_node].outputs[0].ty));
	fg->nodes[new_red_node].outputs[i].ty.kind = Scalar;
	fg->nodes[new_red_node].outputs[i].ty.dims = 0;
	fg->nodes[new_red_node].outputs[i].ty.dim_sizes[0] = -1;
	fg->nodes[new_red_node].outputs[i].ty.dim_sizes[1] = -1;
	copy_type_info (&(fg->nodes[new_arr_def_node].inputs[i].ty), &(fg->nodes[new_red_node].outputs[i].ty));
	}

    return new_arr_def_node;
    }

int hist_reduction (FuncGraph *fg, int iter_cnt, int nd, int red_node,
			int out_port, int *bodies, IntList **parent_list)
    {
    int new_red_node, i, j, k;

    new_red_node = new_ddcf_node (fg, ND_REDUCE_HIST_MACRO, 2*iter_cnt+1, 1, fg->nodes[nd].loc);
    copy_type_info (&(fg->nodes[new_red_node].outputs[0].ty),
		    		&(fg->nodes[red_node].outputs[0].ty));
		
    for (i=0, j=0; i<iter_cnt; i++)
        {
	k = out_port;
        add_edge (fg, bodies[i], k, new_red_node, j);
	copy_type_info (&(fg->nodes[new_red_node].inputs[j].ty),
					&(fg->nodes[bodies[i]].outputs[k].ty));
	j++;
	k++;

        add_edge (fg, bodies[i], k, new_red_node, j);
	copy_type_info (&(fg->nodes[new_red_node].inputs[j].ty),
					&(fg->nodes[bodies[i]].outputs[k].ty));

	j++;
	k++;
        }

    insert_val_after_in_intlist (new_red_node, nd, parent_list);

    /* handle the result-array-size input */
    if (fg->nodes[red_node].inputs[2].is_const)
	{
	fg->nodes[new_red_node].inputs[2*iter_cnt].is_const = TRUE;
        fg->nodes[new_red_node].inputs[2*iter_cnt].constval = fg->nodes[red_node].inputs[2].constval;
	copy_type_info (&(fg->nodes[new_red_node].inputs[2*iter_cnt].ty),
					&(fg->nodes[red_node].inputs[2].ty));
	}
    else
        {
	int snd, spt, io_num;
	fg->nodes[new_red_node].inputs[2*iter_cnt].is_const = FALSE;
	assert (fg->nodes[red_node].inputs[2].back_edges != NULL);
	snd = fg->nodes[red_node].inputs[2].back_edges->node;
	assert (fg->nodes[snd].nodetype == ND_G_INPUT);
	io_num = fg->nodes[snd].Io_num;
	assert (fg->nodes[nd].inputs[io_num].back_edges != NULL);
	snd = fg->nodes[nd].inputs[io_num].back_edges->node;
	spt = fg->nodes[nd].inputs[io_num].back_edges->port;
	add_edge (fg, snd, spt, new_red_node, 2*iter_cnt);
	copy_type_info (&(fg->nodes[new_red_node].inputs[2*iter_cnt].ty),
					&(fg->nodes[snd].outputs[spt].ty));
	}

    return new_red_node;
    }

int assoc_reduction (FuncGraph *fg, int iter_cnt, int nd, int red_node,
			int out_port, int *bodies, IntList **parent_list, DdcfType reduc_type)
    {
    int new_red_node, i, j, k;

    new_red_node = new_ddcf_node (fg, reduc_type, 2*iter_cnt, 1, fg->nodes[nd].loc);
    copy_type_info (&(fg->nodes[new_red_node].outputs[0].ty),
		    		&(fg->nodes[red_node].outputs[0].ty));
		
    for (i=0, j=0; i<iter_cnt; i++)
        {
	k = out_port;
        add_edge (fg, bodies[i], k, new_red_node, j);
	copy_type_info (&(fg->nodes[new_red_node].inputs[j].ty),
					&(fg->nodes[bodies[i]].outputs[k].ty));
	j++;
	k++;

        add_edge (fg, bodies[i], k, new_red_node, j);
	copy_type_info (&(fg->nodes[new_red_node].inputs[j].ty),
					&(fg->nodes[bodies[i]].outputs[k].ty));

	j++;
	k++;
        }

    insert_val_after_in_intlist (new_red_node, nd, parent_list);

    return new_red_node;
    }

int create_a_loop_body (FuncGraph *fg, int nd, OutXyz **out_stack)
    {
    OutXyz *oxyz;
    IntList *it, **pit;
    int loop_body, gen_node, red_node, nxt_node, i, j, out_cnt=0, id;
    int g_out, nxt_cnt;

    loop_body = copy_graph (fg, fg, nd);

    /* nextified inputs become normal inputs */
    for (it=fg->nodes[loop_body].My_nodes; it!=NULL; it=it->link)
        {
	id = it->val;

	if (fg->nodes[id].nodetype == ND_G_INPUT_NEXT)
	    fg->nodes[id].nodetype = ND_G_INPUT;
	}

    insert_dummy_nodes (fg, loop_body);

    /* find the loop's generator graph and lift it to the loop body level */
    gen_node = find_generator_graph (fg, loop_body);
    lift_graph (fg, gen_node, &(fg->nodes[loop_body].My_nodes));

    /* find the loop reduce/construct nodes and save info about them */
    for (i=0; i<fg->nodes[loop_body].num_outputs; i++)
        {
	OutXyz *tmp;

	g_out = fg->nodes[loop_body].My_outputs[i];
	red_node = fg->nodes[g_out].inputs[0].back_edges->node;

	/*  if it's a recution/construction, we will create an output for
	 *  each of its inputs; if it's not a reduction/construction, then
	 *  it's a loop 'final' value that comes from a nextified variable;
	 *  we can ignore it for now, since the nextified variables are
	 *  handled a bit later;
	 */
	if (is_reduction (fg->nodes[red_node].nodetype))
	    for (j=0; j<fg->nodes[red_node].num_inputs; j++)
		{
		SacMalloc (tmp, OutXyz*, sizeof (OutXyz))

		if (fg->nodes[red_node].inputs[j].is_const)
		    {
		    tmp->is_const = TRUE;
		    strcpy (tmp->constval, fg->nodes[red_node].inputs[j].constval);
		    tmp->ty = fg->nodes[red_node].inputs[j].ty;
		    }
		else
		    {
		    tmp->is_const = FALSE;
		    tmp->node = fg->nodes[red_node].inputs[j].back_edges->node;
		    tmp->port = fg->nodes[red_node].inputs[j].back_edges->port;
		    }

		tmp->seq_next = -1;
		tmp->link = NULL;
		append_outxyz (out_stack, tmp);
		out_cnt++;
		}
	}

    /* find the NEXT nodes and save info about them */
    for (nxt_cnt=0, it=fg->nodes[loop_body].My_nodes; it!=NULL; it=it->link)
        {
	nxt_node = it->val;

	if (fg->nodes[nxt_node].nodetype == ND_NEXT)
	    {
	    OutXyz *tmp;

	    SacMalloc (tmp, OutXyz*, sizeof (OutXyz))

	    if (fg->nodes[nxt_node].inputs[0].is_const)
	        {
		tmp->is_const = TRUE;
		strcpy (tmp->constval, fg->nodes[nxt_node].inputs[0].constval);
		tmp->ty = fg->nodes[nxt_node].inputs[0].ty;
		}
	    else
	        {
		tmp->is_const = FALSE;
	        tmp->node = fg->nodes[nxt_node].inputs[0].back_edges->node;
	        tmp->port = fg->nodes[nxt_node].inputs[0].back_edges->port;
		}

	    tmp->seq_next = nxt_cnt++;
	    tmp->link = NULL;
	    append_outxyz (out_stack, tmp);
	    out_cnt++;
	    }
	}

    /* destroy reduce/construct and NEXT nodes */
    for (pit=&(fg->nodes[loop_body].My_nodes); (*pit)!=NULL; )
        {
	red_node = (*pit)->val;

	if (is_reduction (fg->nodes[red_node].nodetype) ||
				    fg->nodes[red_node].nodetype == ND_NEXT)
	    {
	    for (i=0; i<fg->nodes[red_node].num_inputs; i++)
		{
	        remove_targets_to_this_port (fg,
				fg->nodes[red_node].inputs[i].back_edges, red_node, i);
		free_edge_list (&(fg->nodes[red_node].inputs[i].back_edges));
		}

	    for (i=0; i<fg->nodes[red_node].num_outputs; i++)
		{
	        remove_backedges_to_this_target (fg, 
				fg->nodes[red_node].outputs[i].targets, red_node, i);
		free_edge_list (&(fg->nodes[red_node].outputs[i].targets));
		}

	    free_ddcf_node (fg, red_node, &(fg->nodes[loop_body].My_nodes));
	    }
	else
	    pit=&((*pit)->link);
	}

    /* destroy the ND_G_OUTPUT nodes */
    for (i=0; i<fg->nodes[loop_body].num_outputs; i++)
        {
	g_out = fg->nodes[loop_body].My_outputs[i];
	remove_targets_to_this_port (fg, fg->nodes[g_out].inputs[0].back_edges, g_out, 0);
	free_edge_list (&(fg->nodes[g_out].inputs[0].back_edges));
	free_ddcf_node (fg, g_out, &(fg->nodes[loop_body].My_nodes));
	}

    SacFree (fg->nodes[loop_body].outputs)
    SacFree (fg->nodes[loop_body].My_outputs)

    /* for each item in 'out_stack', create an output */
    fg->nodes[loop_body].num_outputs = out_cnt;
    fg->nodes[loop_body].outputs = make_output_ports (out_cnt);
    fg->nodes[loop_body].My_outputs = make_io_array (out_cnt);

    for (oxyz=*out_stack, i=0; oxyz!=NULL; oxyz=oxyz->link, i++)
	{
        g_out = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, fg->nodes[nd].loc);
	fg->nodes[g_out].My_graph = loop_body;
	fg->nodes[g_out].Io_num = i;
	fg->nodes[loop_body].My_outputs[i] = g_out;
	fg->nodes[loop_body].outputs[i].targets = NULL;
	if (oxyz->is_const)
	    {
	    fg->nodes[g_out].inputs[0].is_const = TRUE;
	    /*
	    strcpy (fg->nodes[g_out].inputs[0].constval, oxyz->constval);
	    */
	    fg->nodes[g_out].inputs[0].constval = copy_to_const (oxyz->constval);
	    copy_type_info (&(fg->nodes[g_out].inputs[0].ty), &(oxyz->ty));
	    copy_type_info (&(fg->nodes[loop_body].outputs[i].ty), &(oxyz->ty));
	    }
	else
	    {
	    add_edge (fg, oxyz->node, oxyz->port, g_out, 0);
	    copy_type_info (&(fg->nodes[g_out].inputs[0].ty),
				&(fg->nodes[oxyz->node].outputs[oxyz->port].ty));
	    copy_type_info (&(fg->nodes[loop_body].outputs[i].ty),
				&(fg->nodes[oxyz->node].outputs[oxyz->port].ty));
	    }
	}

    fg->nodes[loop_body].nodetype = ND_GRAPH;

    convert_simple_gens_to_arrayrefs (fg, loop_body);

    return loop_body;
    }

void convert_simple_gens_to_arrayrefs (FuncGraph *fg, int loop_body)
    {
    IntList *it, **pit;
    InputPort *new_ins, *new_ele_gen_inputs;
    Edge *tg;
    int i, j, k, m, rank, gen_rank, cur_cnt, new_in_cnt, tot_cnt, id, g_in;
    int *new_in_arr, cast_node, cnt, extent_srcs[MAXRANK], extents_node, add_node;

    /* count how many new inputs must be added */
    for (new_in_cnt=0, it=fg->nodes[loop_body].My_nodes; it!=NULL; it=it->link)
        {
	id = it->val;

	switch (fg->nodes[id].nodetype)
	    {
	    case ND_ELE_GEN :
	      gen_rank = fg->nodes[id].inputs[0].ty.dims;
	      new_in_cnt += gen_rank;
	      break;
	    case ND_SLICE_GEN :
	      gen_rank = fg->nodes[id].inputs[0].ty.dims - fg->nodes[id].outputs[0].ty.dims;

	      new_in_cnt += gen_rank;
	      break;
	    case ND_SCALAR_GEN :
	        new_in_cnt += fg->nodes[id].num_outputs;
		break;
	    case ND_WINDOW_GEN :
	        new_in_cnt += 3*fg->nodes[id].inputs[0].ty.dims;
		break;
	    case ND_LOOP_INDICES :
		new_in_cnt += fg->nodes[id].num_outputs;
	        break;
	    default :
                break;
	    }
	}

    cur_cnt = fg->nodes[loop_body].num_inputs;
    tot_cnt = cur_cnt + new_in_cnt;

    /* allocate the new inputs array */
    new_ins = make_input_ports (tot_cnt);
    new_in_arr = make_io_array (tot_cnt);

    /* copy the old inputs to the new inputs */
    for (i=0; i<fg->nodes[loop_body].num_inputs; i++)
	{
        new_ins[i] = fg->nodes[loop_body].inputs[i];
	new_in_arr[i] = fg->nodes[loop_body].My_inputs[i];
	}

    /* for now, set all the new inputs as constants; when the inputs
     * are connected later, the ones that aren't constants will be
     * set appropriately
     */
    for (i=fg->nodes[loop_body].num_inputs; i<tot_cnt; i++)
        {
	new_ins[i].is_const = TRUE;
	new_ins[i].constval = copy_to_const ("");
	new_ins[i].ty.type = Uint;
	new_ins[i].ty.kind = Scalar;
	new_ins[i].ty.totsize = 32;
	new_ins[i].ty.fracsize = 0;
	new_ins[i].back_edges = NULL;

	g_in = new_ddcf_node (fg, ND_G_INPUT, 0, 1, fg->nodes[loop_body].loc);
	fg->nodes[g_in].My_graph = loop_body;
	fg->nodes[g_in].Io_num = i;
	add_to_list (g_in, &(fg->nodes[loop_body].My_nodes));
	new_in_arr[i] = g_in;
	copy_type_info (&(fg->nodes[g_in].outputs[0].ty), &(new_ins[i].ty));
	}

    /* discard the old inputs and install the new ones */
    SacFree (fg->nodes[loop_body].inputs)
    fg->nodes[loop_body].inputs = new_ins;
    SacFree (fg->nodes[loop_body].My_inputs)
    fg->nodes[loop_body].My_inputs = new_in_arr;
    fg->nodes[loop_body].num_inputs = tot_cnt;

    /* find and process each of the simple generator nodes */
    for (i=cur_cnt, pit=&(fg->nodes[loop_body].My_nodes); (*pit)!=NULL; )
        {
	it = *pit;
	id = it->val;

	switch (fg->nodes[id].nodetype)
	    {
	    case ND_ELE_GEN :
		rank = fg->nodes[id].inputs[0].ty.dims;

		/*  count the number of inputs this ND_ELE_GEN node needs when it turns
		 *  into a ND_ARRAYREF node
		 */
		for (cnt=1, k=0; k<rank; k++)
		  cnt++;

		/* create the new input array for the ND_ELE_GEN node */
		new_ele_gen_inputs = make_input_ports (cnt);
		new_ele_gen_inputs[0] = fg->nodes[id].inputs[0];
		SacFree (fg->nodes[id].inputs)
		fg->nodes[id].inputs = new_ele_gen_inputs;
		fg->nodes[id].num_inputs = cnt;


		/*  'k' indexes in the rank position
		 *  'i' indexes for the input ports of the loop body graph
		 *  'j' indexes for the input ports of the ND_ELE_GEN node
		 *  'm' indexes for the output ports of the ND_ELE_GEN node
		 */
		for (k=0, j=1, m=1; k<rank; k++)
		  {
		        int node, port, ionum;

			fg->nodes[id].Reftypes[k] = '_';

		        fg->nodes[id].inputs[j].is_const = FALSE;
		        g_in = fg->nodes[loop_body].My_inputs[i];
		        add_edge (fg, g_in, 0, id, j);
		        copy_type_info (&(fg->nodes[id].inputs[j].ty),
					&(fg->nodes[g_in].outputs[0].ty));

		        /* if output 'm' is targeting anything, then 'g_in's
		         * output must instead target it
		         */
		        if (fg->nodes[id].outputs[m].targets != NULL) {
		            for (tg=fg->nodes[id].outputs[m].targets; tg!=NULL; tg=tg->link)
		                {
			        node = tg->node;
			        port = tg->port;
			        add_edge (fg, g_in, 0, node, port);
			        }

		            remove_backedges_to_this_target (fg,
		    			fg->nodes[id].outputs[m].targets, id, j);

			    free_edge_list (&(fg->nodes[id].outputs[m].targets));

			    copy_type_info (&(fg->nodes[g_in].outputs[0].ty),
						&(fg->nodes[node].inputs[port].ty));
			    ionum = fg->nodes[g_in].Io_num;
			    copy_type_info (&(fg->nodes[loop_body].inputs[ionum].ty),
						&(fg->nodes[node].inputs[port].ty));
			    }

			j++;
			m++;
			i++;
		  }

	        fg->nodes[id].nodetype = ND_ARRAYREF;
		fg->nodes[id].num_outputs = 1;

		pit = &((*pit)->link);

		break;
	    case ND_SLICE_GEN :
		rank = fg->nodes[id].inputs[0].ty.dims;

		/*  count the number of inputs this ND_SLICE_GEN node needs when it turns
		 *  into a ND_ARRAYREF node
		 */
		for (cnt=1, k=0; k<rank; k++)
		    if (fg->nodes[id].Reftypes[k] == '~')
		        cnt++;
		    else
		        cnt += 3;

		/* create the new input array for the ND_ELE_GEN node */
		new_ele_gen_inputs = make_input_ports (cnt);
		new_ele_gen_inputs[0] = fg->nodes[id].inputs[0];
		SacFree (fg->nodes[id].inputs)
		fg->nodes[id].inputs = new_ele_gen_inputs;
		fg->nodes[id].num_inputs = cnt;

		/* we create a ND_EXTENTS node */
		{
		  int src_nd, src_pt;
		  
		  assert (fg->nodes[id].inputs[0].back_edges != NULL);
		  src_nd = fg->nodes[id].inputs[0].back_edges->node;
		  src_pt = fg->nodes[id].inputs[0].back_edges->port;
		  
		  extents_node = new_ddcf_node (fg, ND_EXTENTS, 1,
						fg->nodes[id].inputs[0].ty.dims, fg->nodes[id].loc);
		  fg->nodes[extents_node].inputs[0].is_const = FALSE;
		  add_edge (fg, src_nd, src_pt, extents_node, 0);
		  copy_type_info (&(fg->nodes[extents_node].inputs[0].ty),
				  &(fg->nodes[src_nd].outputs[src_pt].ty));
		  insert_val_after_in_intlist (extents_node, src_nd, &(fg->nodes[loop_body].My_nodes));
		  
		  for (k=0; k<fg->nodes[id].inputs[0].ty.dims; k++)
		    {
		      fg->nodes[extents_node].outputs[k].ty.kind = Scalar;
		      fg->nodes[extents_node].outputs[k].ty.type = Uint;
		      fg->nodes[extents_node].outputs[k].ty.totsize = 32;
		      fg->nodes[extents_node].outputs[k].ty.fracsize = 0;
		      
		      add_node = new_ddcf_node (fg, ND_ADD, 2, 1, fg->nodes[id].loc);
		      fg->nodes[add_node].inputs[0].is_const = FALSE;
		      add_edge (fg, extents_node, k, add_node, 0);
		      copy_type_info (&(fg->nodes[add_node].inputs[0].ty),
				      &(fg->nodes[extents_node].outputs[k].ty));
		      fg->nodes[add_node].inputs[1].ty.kind = Scalar;
		      fg->nodes[add_node].inputs[1].ty.type = Int;
		      fg->nodes[add_node].inputs[1].ty.totsize = 32;
		      fg->nodes[add_node].inputs[1].ty.fracsize = 0;
		      fg->nodes[add_node].inputs[1].is_const = TRUE;
		      /*
			strcpy (fg->nodes[add_node].inputs[1].constval, "-1");
		      */
		      fg->nodes[add_node].inputs[1].constval = copy_to_const ("-1");
		      
		      fg->nodes[add_node].outputs[0].ty.kind = Scalar;
		      fg->nodes[add_node].outputs[0].ty.type = Int;
		      fg->nodes[add_node].outputs[0].ty.totsize = 32;
		      fg->nodes[add_node].outputs[0].ty.fracsize = 0;
		      
		      insert_val_after_in_intlist (add_node, extents_node, &(fg->nodes[loop_body].My_nodes));
		      
		      extent_srcs[k] = add_node;
		    }
		}

		/*  'k' indexes in the rank position
		 *  'i' indexes for the input ports of the loop body graph
		 *  'j' indexes for the input ports of the ND_SLICE_GEN node
		 *  'm' indexes for the output ports of the ND_SLICE_GEN node
		 */
		for (k=0, j=1, m=1; k<rank; k++)
		    if (fg->nodes[id].Reftypes[k] == '~') {
		        int node, port, ionum;

			fg->nodes[id].Reftypes[k] = '_';

		        fg->nodes[id].inputs[j].is_const = FALSE;
		        g_in = fg->nodes[loop_body].My_inputs[i];
		        add_edge (fg, g_in, 0, id, j);
		        copy_type_info (&(fg->nodes[id].inputs[j].ty),
					&(fg->nodes[g_in].outputs[0].ty));

		        /* if output 'm' is targeting anything, then 'g_in's
		         * output must instead target it
		         */
		        if (fg->nodes[id].outputs[m].targets != NULL) {
		            for (tg=fg->nodes[id].outputs[m].targets; tg!=NULL; tg=tg->link)
		                {
			        node = tg->node;
			        port = tg->port;
			        add_edge (fg, g_in, 0, node, port);
			        }

		            remove_backedges_to_this_target (fg,
		    			fg->nodes[id].outputs[m].targets, id, j);

			    free_edge_list (&(fg->nodes[id].outputs[m].targets));

			    copy_type_info (&(fg->nodes[g_in].outputs[0].ty),
						&(fg->nodes[node].inputs[port].ty));
			    ionum = fg->nodes[g_in].Io_num;
			    copy_type_info (&(fg->nodes[loop_body].inputs[ionum].ty),
						&(fg->nodes[node].inputs[port].ty));
			    }

			j++;
			m++;
			i++;
		        }
		    else {
			fg->nodes[id].inputs[j].is_const = TRUE;
			/*
			strcpy (fg->nodes[id].inputs[j].constval, "0");
			*/
			fg->nodes[id].inputs[j].constval = copy_to_const ("0");
			fg->nodes[id].inputs[j].ty.type = Uint;
			fg->nodes[id].inputs[j].ty.kind = Scalar;
			fg->nodes[id].inputs[j].ty.totsize = 32;
			fg->nodes[id].inputs[j].ty.fracsize = 0;
			j++;

			/* here hook up to the extents node... */
			fg->nodes[id].inputs[j].is_const = FALSE;
			add_edge (fg, extent_srcs[k], 0, id, j);
			copy_type_info (&(fg->nodes[id].inputs[j].ty),
						&(fg->nodes[extent_srcs[k]].outputs[0].ty));
			j++;

			fg->nodes[id].inputs[j].is_const = TRUE;
			fg->nodes[id].inputs[j].constval = copy_to_const ("1");
			fg->nodes[id].inputs[j].ty.type = Uint;
			fg->nodes[id].inputs[j].ty.kind = Scalar;
			fg->nodes[id].inputs[j].ty.totsize = 32;
			fg->nodes[id].inputs[j].ty.fracsize = 0;
			j++;
			}

	        fg->nodes[id].nodetype = ND_ARRAYREF;
		fg->nodes[id].num_outputs = 1;

		pit = &((*pit)->link);

		break;
	    case ND_WINDOW_GEN :
		{
		InputPort *new_inputs;

		rank = fg->nodes[id].inputs[0].ty.dims;
		new_inputs = make_input_ports (1 + 3*rank);
		new_inputs[0] = fg->nodes[id].inputs[0];

		SacFree (fg->nodes[id].inputs)
		fg->nodes[id].inputs = new_inputs;
		fg->nodes[id].num_inputs = 1 + 3*rank;

		/*  'j' indexes into the 'new_inputs' array
		 *  'k' indexes into the 'outputs' array
		 *  'm' is a loop counter
		 */
		for (m=0, k=1, j=1; m<rank; m++, k++, j+=3, i+=3)
		    {
		    int node, port, ionum;

		    new_inputs[j].is_const = FALSE;
		    g_in = fg->nodes[loop_body].My_inputs[i];
		    add_edge (fg, g_in, 0, id, j);
		    copy_type_info (&(new_inputs[j].ty),
					&(fg->nodes[g_in].outputs[0].ty));

		    /* if output 'k' is targeting anything, then 'g_in's
		     * output must instead target it
		     */
		    if (fg->nodes[id].outputs[k].targets != NULL)
		        {
		        for (tg=fg->nodes[id].outputs[k].targets; tg!=NULL; tg=tg->link)
		            {
			    node = tg->node;
			    port = tg->port;
			    add_edge (fg, g_in, 0, node, port);
			    }

		        remove_backedges_to_this_target (fg,
		    			fg->nodes[id].outputs[k].targets, id, k);

			free_edge_list (&(fg->nodes[id].outputs[k].targets));

			copy_type_info (&(fg->nodes[g_in].outputs[0].ty),
						&(fg->nodes[node].inputs[port].ty));
			ionum = fg->nodes[g_in].Io_num;
			copy_type_info (&(fg->nodes[loop_body].inputs[ionum].ty),
						&(fg->nodes[node].inputs[port].ty));
			}

		    new_inputs[j+1].is_const = FALSE;
		    g_in = fg->nodes[loop_body].My_inputs[i+1];
		    add_edge (fg, g_in, 0, id, j+1);
		    copy_type_info (&(new_inputs[j+1].ty),
					&(fg->nodes[g_in].outputs[0].ty));

		    new_inputs[j+2].is_const = FALSE;
		    g_in = fg->nodes[loop_body].My_inputs[i+2];
		    add_edge (fg, g_in, 0, id, j+2);
		    copy_type_info (&(new_inputs[j+2].ty),
					&(fg->nodes[g_in].outputs[0].ty));
		    }

	        fg->nodes[id].nodetype = ND_ARRAYREF;
		fg->nodes[id].num_outputs = 1;

		SacMalloc (fg->nodes[id].Reftypes, char*, rank);

		for (j=0; j<rank; j++)
		    fg->nodes[id].Reftypes[j] = ':';

		pit = &((*pit)->link);
		}
		break;
	    case ND_SCALAR_GEN :
	    case ND_LOOP_INDICES :
		rank = fg->nodes[id].num_outputs;

		for (j=0; j<rank; j++, i++)
		    {
		    cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[id].loc);
		    copy_type_info (&(fg->nodes[cast_node].outputs[0].ty),
		    				&(fg->nodes[id].outputs[j].ty));
		    fg->nodes[cast_node].outputs[0].targets =
		    				fg->nodes[id].outputs[j].targets;
		    fg->nodes[id].outputs[j].targets = NULL;
		    retarget_backedges (fg,
		    	fg->nodes[cast_node].outputs[0].targets, id, j, cast_node, 0);
		    g_in = fg->nodes[loop_body].My_inputs[i];
		    add_edge (fg, g_in, 0, cast_node, 0);
		    copy_type_info (&(fg->nodes[cast_node].inputs[0].ty),
					&(fg->nodes[cast_node].outputs[0].ty));
		    copy_type_info (&(fg->nodes[g_in].outputs[0].ty),
					&(fg->nodes[cast_node].inputs[0].ty));
		    copy_type_info (&(fg->nodes[loop_body].inputs[i].ty),
					&(fg->nodes[g_in].outputs[0].ty));
		    insert_val_after_in_intlist (cast_node, id, &(fg->nodes[loop_body].My_nodes));
		    }

		free_ddcf_node (fg, id, &(fg->nodes[loop_body].My_nodes));

	        break;
	    default :
	        pit = &((*pit)->link);
		break;
	    }
	}
    }

/*  put a dummy node (CAST with same input and output types) above each
 *  reduction/construction node; this makes sure that we don't have an
 *  empty loop body, or an edge that goes directly from an input to an
 *  output; another optimization pass will clean these nodes up later;
 */
void insert_dummy_nodes (FuncGraph *fg, int loop_body)
    {
    IntList *it;
    Edge *tg;
    Edge *bk;
    int i, nd, src_node, src_port, cast_node;

    for (it=fg->nodes[loop_body].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;

	if (is_reduction (fg->nodes[nd].nodetype))
	    for (i=0; i<fg->nodes[nd].num_inputs; i++)
	        {
	        cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[loop_body].loc);

		if (fg->nodes[nd].inputs[i].is_const)
		    {
		    fg->nodes[nd].inputs[i].is_const = FALSE;

		    fg->nodes[cast_node].inputs[0].is_const = TRUE;
		    fg->nodes[cast_node].inputs[0].constval = copy_to_const (fg->nodes[nd].inputs[i].constval);

		    tg = malloc_target (nd, i, NULL);
		    fg->nodes[cast_node].outputs[0].targets = tg;

		    bk = malloc_backedge (cast_node, 0, NULL);
		    fg->nodes[nd].inputs[i].back_edges = bk;
		    }
		else
		    {
		    fg->nodes[cast_node].inputs[0].is_const = FALSE;

		    src_node = fg->nodes[nd].inputs[i].back_edges->node;
		    src_port = fg->nodes[nd].inputs[i].back_edges->port;

		    bk = malloc_backedge (src_node, src_port, NULL);
		    fg->nodes[cast_node].inputs[0].back_edges = bk;

		    retarget (fg, fg->nodes[cast_node].inputs[0].back_edges, nd, i, cast_node, 0);

		    tg = malloc_target (nd, i, NULL);
		    fg->nodes[cast_node].outputs[0].targets = tg;

		    retarget_backedges (fg, fg->nodes[cast_node].outputs[0].targets,
					src_node, src_port, cast_node, 0);
		    }

		copy_type_info (&(fg->nodes[cast_node].inputs[0].ty),
		    				&(fg->nodes[nd].inputs[i].ty));
		copy_type_info (&(fg->nodes[cast_node].outputs[0].ty),
		    				&(fg->nodes[nd].inputs[i].ty));
		insert_val_before_in_intlist (cast_node,
				nd, &(fg->nodes[loop_body].My_nodes));
		}
	}
    }
    
void loop_indices_index_list1 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int i0, j;

    for (i0=0; i0<szs[0]; i0++)
	for (j=0; j<multiplier; j++)
	    *(rets++) = i0;
    }

void loop_indices_index_list2 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int i0, i1, j;

    for (i0=0; i0<szs[0]; i0++)
        for (i1=0; i1<szs[1]; i1++)
	    for (j=0; j<multiplier; j++)
		{
	        *(rets++) = i0;
	        *(rets++) = i1;
		}
    }

void loop_indices_index_list3 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int i0, i1, i2, j;

    for (i0=0; i0<szs[0]; i0++)
        for (i1=0; i1<szs[1]; i1++)
            for (i2=0; i2<szs[2]; i2++)
	        for (j=0; j<multiplier; j++)
		    {
	            *(rets++) = i0;
	            *(rets++) = i1;
	            *(rets++) = i2;
		    }
    }

void loop_indices_index_list4 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int i0, i1, i2, i3, j;

    for (i0=0; i0<szs[0]; i0++)
        for (i1=0; i1<szs[1]; i1++)
            for (i2=0; i2<szs[2]; i2++)
                for (i3=0; i3<szs[3]; i3++)
	            for (j=0; j<multiplier; j++)
		        {
	                *(rets++) = i0;
	                *(rets++) = i1;
	                *(rets++) = i2;
	                *(rets++) = i3;
		        }
    }

void loop_indices_index_list5 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int i0, i1, i2, i3, i4, j;

    for (i0=0; i0<szs[0]; i0++)
        for (i1=0; i1<szs[1]; i1++)
            for (i2=0; i2<szs[2]; i2++)
                for (i3=0; i3<szs[3]; i3++)
                    for (i4=0; i4<szs[4]; i4++)
	                for (j=0; j<multiplier; j++)
		            {
	                    *(rets++) = i0;
	                    *(rets++) = i1;
	                    *(rets++) = i2;
	                    *(rets++) = i3;
	                    *(rets++) = i4;
		            }
    }

void loop_indices_index_list6 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int i0, i1, i2, i3, i4, i5, j;

    for (i0=0; i0<szs[0]; i0++)
        for (i1=0; i1<szs[1]; i1++)
            for (i2=0; i2<szs[2]; i2++)
                for (i3=0; i3<szs[3]; i3++)
                    for (i4=0; i4<szs[4]; i4++)
                        for (i5=0; i5<szs[5]; i5++)
	                    for (j=0; j<multiplier; j++)
		                {
	                        *(rets++) = i0;
	                        *(rets++) = i1;
	                        *(rets++) = i2;
	                        *(rets++) = i3;
	                        *(rets++) = i4;
	                        *(rets++) = i5;
		                }
    }

void loop_indices_index_list7 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int i0, i1, i2, i3, i4, i5, i6, j;

    for (i0=0; i0<szs[0]; i0++)
        for (i1=0; i1<szs[1]; i1++)
            for (i2=0; i2<szs[2]; i2++)
                for (i3=0; i3<szs[3]; i3++)
                    for (i4=0; i4<szs[4]; i4++)
                        for (i5=0; i5<szs[5]; i5++)
                            for (i6=0; i6<szs[6]; i6++)
	                        for (j=0; j<multiplier; j++)
		                    {
	                            *(rets++) = i0;
	                            *(rets++) = i1;
	                            *(rets++) = i2;
	                            *(rets++) = i3;
	                            *(rets++) = i4;
	                            *(rets++) = i5;
	                            *(rets++) = i6;
		                    }
    }

void loop_indices_index_list8 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int i0, i1, i2, i3, i4, i5, i6, i7, j;

    for (i0=0; i0<szs[0]; i0++)
        for (i1=0; i1<szs[1]; i1++)
            for (i2=0; i2<szs[2]; i2++)
                for (i3=0; i3<szs[3]; i3++)
                    for (i4=0; i4<szs[4]; i4++)
                        for (i5=0; i5<szs[5]; i5++)
                            for (i6=0; i6<szs[6]; i6++)
                                for (i7=0; i7<szs[7]; i7++)
	                            for (j=0; j<multiplier; j++)
		                        {
	                                *(rets++) = i0;
	                                *(rets++) = i1;
	                                *(rets++) = i2;
	                                *(rets++) = i3;
	                                *(rets++) = i4;
	                                *(rets++) = i5;
	                                *(rets++) = i6;
	                                *(rets++) = i7;
		                        }
    }

void scalargen_index_list1 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int st0, lo0, i0, idx0, j;

    sscanf (fg->nodes[nd].inputs[0].constval, "%d", &lo0);
    sscanf (fg->nodes[nd].inputs[2].constval, "%d", &st0);

    for (i0=0, idx0=lo0; i0<szs[0]; i0++, idx0+=st0)
	for (j=0; j<multiplier; j++)
	    {
	    *(rets++) = idx0;
	    }
    }

void scalargen_index_list2 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int st0, lo0, i0, idx0, st1, lo1, i1, idx1, j;

    sscanf (fg->nodes[nd].inputs[0].constval, "%d", &lo0);
    sscanf (fg->nodes[nd].inputs[2].constval, "%d", &st0);
    sscanf (fg->nodes[nd].inputs[3].constval, "%d", &lo1);
    sscanf (fg->nodes[nd].inputs[5].constval, "%d", &st1);

    for (i0=0, idx0=lo0; i0<szs[0]; i0++, idx0+=st0)
        for (i1=0, idx1=lo1; i1<szs[1]; i1++, idx1+=st1)
	    for (j=0; j<multiplier; j++)
	        {
	        *(rets++) = idx0;
	        *(rets++) = idx1;
	        }
    }

void scalargen_index_list3 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int st0, lo0, i0, idx0, st1, lo1, i1, idx1, st2, lo2, i2, idx2, j;

    sscanf (fg->nodes[nd].inputs[0].constval, "%d", &lo0);
    sscanf (fg->nodes[nd].inputs[2].constval, "%d", &st0);
    sscanf (fg->nodes[nd].inputs[3].constval, "%d", &lo1);
    sscanf (fg->nodes[nd].inputs[5].constval, "%d", &st1);
    sscanf (fg->nodes[nd].inputs[6].constval, "%d", &lo2);
    sscanf (fg->nodes[nd].inputs[8].constval, "%d", &st2);

    for (i0=0, idx0=lo0; i0<szs[0]; i0++, idx0+=st0)
        for (i1=0, idx1=lo1; i1<szs[1]; i1++, idx1+=st1)
            for (i2=0, idx2=lo2; i2<szs[2]; i2++, idx2+=st2)
	        for (j=0; j<multiplier; j++)
	            {
	            *(rets++) = idx0;
	            *(rets++) = idx1;
	            *(rets++) = idx2;
	            }
    }

void windowgen_index_list1 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int st0, i0, idx0, j;

    sscanf (fg->nodes[nd].inputs[2].constval, "%d", &st0);

    for (i0=0, idx0=0; i0<szs[0]; i0++, idx0+=st0)
	for (j=0; j<multiplier; j++)
	    *(rets++) = idx0;
    }

void windowgen_index_list2 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int st0, st1, i0, i1, idx0, idx1, j;

    sscanf (fg->nodes[nd].inputs[2].constval, "%d", &st0);
    sscanf (fg->nodes[nd].inputs[4].constval, "%d", &st1);

    for (i0=0, idx0=0; i0<szs[0]; i0++, idx0+=st0)
        for (i1=0, idx1=0; i1<szs[1]; i1++, idx1+=st1)
	    for (j=0; j<multiplier; j++)
	        {
	        *(rets++) = idx0;
	        *(rets++) = idx1;
	        }
    }

void windowgen_index_list3 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int st0, st1, st2, i0, i1, i2, idx0, idx1, idx2, j;

    sscanf (fg->nodes[nd].inputs[2].constval, "%d", &st0);
    sscanf (fg->nodes[nd].inputs[4].constval, "%d", &st1);
    sscanf (fg->nodes[nd].inputs[6].constval, "%d", &st2);

    for (i0=0, idx0=0; i0<szs[0]; i0++, idx0+=st0)
        for (i1=0, idx1=0; i1<szs[1]; i1++, idx1+=st1)
            for (i2=0, idx2=0; i2<szs[2]; i2++, idx2+=st2)
	        for (j=0; j<multiplier; j++)
	            {
	            *(rets++) = idx0;
	            *(rets++) = idx1;
	            *(rets++) = idx2;
	            }
    }

void eleslicegen_index_list1 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int st0, i0, idx0, j;

    /* pick up step size */
    sscanf (fg->nodes[nd].inputs[1].constval, "%d", &st0);

    for (i0=0, idx0=0; i0<szs[0]; i0++, idx0+=st0)
	for (j=0; j<multiplier; j++)
	    *(rets++) = idx0;
    }

void eleslicegen_index_list2 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int st0, st1, i0, i1, idx0, idx1, j;

    /* pick up step sizes */
    sscanf (fg->nodes[nd].inputs[1].constval, "%d", &st0);
    sscanf (fg->nodes[nd].inputs[2].constval, "%d", &st1);

    for (i0=0, idx0=0; i0<szs[0]; i0++, idx0+=st0)
        for (i1=0, idx1=0; i1<szs[1]; i1++, idx1+=st1)
	    for (j=0; j<multiplier; j++)
	        {
	        *(rets++) = idx0;
	        *(rets++) = idx1;
	        }
    }

void eleslicegen_index_list3 (FuncGraph *fg, int nd, int *szs, int multiplier, int *rets)
    {
    int st0, st1, st2, i0, i1, i2, idx0, idx1, idx2, j;

    /* pick up step sizes */
    sscanf (fg->nodes[nd].inputs[1].constval, "%d", &st0);
    sscanf (fg->nodes[nd].inputs[2].constval, "%d", &st1);
    sscanf (fg->nodes[nd].inputs[3].constval, "%d", &st2);

    for (i0=0, idx0=0; i0<szs[0]; i0++, idx0+=st0)
        for (i1=0, idx1=0; i1<szs[1]; i1++, idx1+=st1)
            for (i2=0, idx2=0; i2<szs[2]; i2++, idx2+=st2)
	        for (j=0; j<multiplier; j++)
	            {
	            *(rets++) = idx0;
	            *(rets++) = idx1;
	            *(rets++) = idx2;
	            }
    }

void append_outxyz (OutXyz **list, OutXyz *tmp)
    {
    while ((*list) != NULL)
        list = &((*list)->link);

    *list = tmp;
    }

void free_oxyz_list (OutXyz **ptg)
    {
    OutXyz *tmp, *tg;

    tg = *ptg;
    while (tg != NULL)
        {
        tmp = tg;
        tg = tg->link;
        SacFree (tmp)
        }

    *ptg = NULL;
    }
