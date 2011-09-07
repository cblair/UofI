#include "sassy.h"
#include "ddcf.h"
#include "error.h"

int acp_node (FuncGraph*, int, IntList**);
int int_from_string (char*);
void acp_process_graphs_nodes (FuncGraph*, int);
int getvalstr (FuncGraph*, int, int, int*, char*, TypeInfo*, int*, int*, int*);
int find_array_source (FuncGraph*, int, int, int*, int*, int*);
void const_array_slice (FuncGraph*, int, IntList**);
void make_index_list (FuncGraph*, int, int*);
int *mk_indices (int, int*, int*, int, int*, int*);
int flat_index (int, int*, int*);

void array_const_prop ()
    {
    FuncGraph *fg;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
	    {
	    acp_process_graphs_nodes (fg, 0);
	    }
    }

void acp_process_graphs_nodes (FuncGraph *fg, int id)
    {
    IntList **pit;
    int deleted;

    for (pit=&(fg->nodes[id].My_nodes); (*pit)!=NULL; )
	{
        deleted = acp_node (fg, (*pit)->val, &(fg->nodes[id].My_nodes));

	if (!deleted)
	    pit=&((*pit)->link);
	}
    }

int acp_node (FuncGraph *fg, int id, IntList **itlist)
    {
    TypeInfo ty;
    char str[256];
    int keep, in_node, i;
    int deleted = FALSE;
    int idxs[MAXRANK], indices_known;
    int nd, pt, crossed;

    switch (fg->nodes[id].nodetype)
        {
	case ND_ARRAYREF :
	    if (fg->nodes[id].outputs[0].ty.kind == Scalar)
	        {
		indices_known = TRUE;

		/* see if all the indices are constants, and collect them in 'idxs' */
		for (i=1; i<fg->nodes[id].num_inputs; i++)
		    if (fg->nodes[id].inputs[i].is_const)
			{
		        sscanf (fg->nodes[id].inputs[i].constval, "%d", &(idxs[i-1]));
			}
		    else
		        {
			indices_known = FALSE;
			break;
			}

		/* if all indices are known, go to the array's source and return the
		 * value string if that input is a constant; if string comes back
		 * as "" then the value couldn't be determined;
		 */
		if (indices_known)
		    {
		    crossed = FALSE;
		    if (getvalstr (fg, id, 0, idxs, str, &ty, &nd, &pt, &crossed) == TRUE)
			{
			if ((strcmp (str, "") != 0) || !crossed)
			    {
			    is_stable = FALSE;

			    fg->nodes[id].nodetype = ND_CAST;
			    fg->nodes[id].num_inputs = 1;
			    remove_targets_to_this_port (fg, fg->nodes[id].inputs[0].back_edges, id, 0);
			    free_edge_list (&(fg->nodes[id].inputs[0].back_edges));

			    if (strcmp (str, "") != 0)
			        {
			        fg->nodes[id].inputs[0].is_const = TRUE;
			        fg->nodes[id].inputs[0].constval = copy_to_const (str);
			        }
			    else
			        {
			        add_edge (fg, nd, pt, id, 0);
			        }

			    copy_type_info (&(fg->nodes[id].inputs[0].ty), &ty);
			    }
			}
		    }
		}
	    else
	        {
		indices_known = TRUE;

		/* see if all the indices are constants */
		for (i=1; i<fg->nodes[id].num_inputs; i++)
		    if (!fg->nodes[id].inputs[i].is_const)
		        {
			indices_known = FALSE;
			break;
			}

		for (i=0; i<fg->nodes[id].outputs[0].ty.dims; i++)
		    if (fg->nodes[id].outputs[0].ty.dim_sizes[i] < 0)
		        {
			indices_known = FALSE;
			break;
			}

		if (indices_known)
		    const_array_slice (fg, id, itlist);
		}
	    break;
	case ND_SWITCH :
	case ND_CASE :
	case ND_WHILE :
	case ND_FORALL :
	case ND_FORNXT :
	case ND_FUNC :
	case ND_CROSS_PROD :
	case ND_DOT_PROD :
	case ND_WHILE_PRED :
	    for (i=0; i<fg->nodes[id].num_inputs; i++)
	        if (fg->nodes[id].inputs[i].is_const)
		    {
		    strcpy (str, fg->nodes[id].inputs[i].constval);
		    in_node = fg->nodes[id].My_inputs[i];
		    keep = transfer_const_to_targets (fg, in_node, 0, str);

		    if (! keep)
		        {
		        SacFree (fg->nodes[in_node].inputs)
		        free_edge_list (&(fg->nodes[in_node].outputs[0].targets));
		        SacFree (fg->nodes[in_node].outputs)
		        fg->nodes[in_node].nodetype = ND_VOIDED;

		        /* remove the node from the graph's list of nodes */
		        delete_ele_from_intlist (in_node, &(fg->nodes[id].My_nodes));
			deleted = TRUE;
		        }

		    eliminate_graph_input (fg, id, i);
		    }

	    acp_process_graphs_nodes (fg, id);

	    break;
	default :
            break;
	}

    return deleted;
    }

void const_array_slice (FuncGraph *fg, int id, IntList **itlist)
    {
    TypeInfo ty;
    int *def_dims, *flat_index_list, out_sz, out_rank, src_nd, src_pt, i, cast_node, snd, spt, f_idx;
    int crosses, bailout;

    crosses = FALSE;
    if (!find_array_source (fg, id, 0, &src_nd, &src_pt, &crosses))
        return;

    ty = fg->nodes[id].inputs[0].ty;
    ty.kind = Scalar;
    for (i=0; i<MAXRANK; i++)
        ty.dim_sizes[i] = -1;

    out_rank = fg->nodes[id].outputs[0].ty.dims;
    SacMalloc (def_dims, int*, out_rank * sizeof (int))

    for (out_sz=1, i=0; i<out_rank; i++)
	{
        out_sz *= fg->nodes[id].outputs[0].ty.dim_sizes[i];
	def_dims[i] = fg->nodes[id].outputs[0].ty.dim_sizes[i];
	}

    SacMalloc (flat_index_list, int*, out_sz * sizeof (int))

    make_index_list (fg, id, flat_index_list);

    /* if the path from source array to the array ref node crosses any graph
     * boundaries, then we can only do this optimization if all the inputs
     * we care about are constants; check that here;
     */
    bailout = FALSE;
    if (crosses)
        {
        for (i=0; i<out_sz; i++)
            {
	    f_idx = flat_index_list[i];
	    if (!fg->nodes[src_nd].inputs[f_idx].is_const)
		{
	        bailout = TRUE;
		break;
		}
	    }
	}

    if (bailout)
        {
	SacFree (def_dims);
	SacFree (flat_index_list);
	return;
	}

    is_stable = FALSE;

    remove_targets_to_this_port (fg, fg->nodes[id].inputs[0].back_edges, id, 0);
    free_edge_list (&(fg->nodes[id].inputs[0].back_edges));
    SacFree (fg->nodes[id].inputs)
    fg->nodes[id].inputs = make_input_ports (out_sz);
    fg->nodes[id].num_inputs = out_sz;

    for (i=0; i<out_sz; i++)
        {
	f_idx = flat_index_list[i];
	cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[id].loc);
	insert_val_before_in_intlist (cast_node, id, itlist);
	add_edge (fg, cast_node, 0, id, i);
	copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), &ty);
	copy_type_info (&(fg->nodes[id].inputs[i].ty), &ty);

	if (fg->nodes[src_nd].inputs[f_idx].is_const)
	    {
	    fg->nodes[cast_node].inputs[0].is_const = TRUE;
	    fg->nodes[cast_node].inputs[0].constval = copy_to_const (fg->nodes[src_nd].inputs[f_idx].constval);
	    }
	else
	    {
	    fg->nodes[cast_node].inputs[0].is_const = FALSE;
	    snd = fg->nodes[src_nd].inputs[f_idx].back_edges->node;
	    spt = fg->nodes[src_nd].inputs[f_idx].back_edges->port;
	    add_edge (fg, snd, spt, cast_node, 0);
	    }

	copy_type_info (&(fg->nodes[cast_node].inputs[0].ty), &(fg->nodes[src_nd].inputs[f_idx].ty));
	}

    SacFree (flat_index_list)

    fg->nodes[id].nodetype = ND_ARR_DEF;
    fg->nodes[id].DefRank = out_rank;
    fg->nodes[id].DefDims = def_dims;
    }

void make_index_list (FuncGraph *fg, int id, int *idxs)
    {
    int i, j, in_rank, *ranges, *rg;
    int indices[MAXRANK];

    in_rank = fg->nodes[id].inputs[0].ty.dims;

    SacMalloc (ranges, int*, 3 * in_rank * sizeof (int))

    for (rg=ranges, j=1, i=0; i<in_rank; i++)
        if (fg->nodes[id].Reftypes[i] == ':')
	    {
	    assert (fg->nodes[id].inputs[j].constval != NULL);
	    sscanf (fg->nodes[id].inputs[j++].constval, "%d", rg++);
	    assert (fg->nodes[id].inputs[j].constval != NULL);
	    sscanf (fg->nodes[id].inputs[j++].constval, "%d", rg++);
	    assert (fg->nodes[id].inputs[j].constval != NULL);
	    sscanf (fg->nodes[id].inputs[j++].constval, "%d", rg++);
	    }
	else
	    {
	    assert (fg->nodes[id].inputs[j].constval != NULL);
	    sscanf (fg->nodes[id].inputs[j].constval, "%d", rg++);
	    sscanf (fg->nodes[id].inputs[j++].constval, "%d", rg++);
	    *(rg++) = 1;
	    }

    mk_indices (0, ranges, fg->nodes[id].inputs[0].ty.dim_sizes, in_rank, indices, idxs);

    SacFree (ranges)
    }

int *mk_indices (int place, int *ranges, int *szs, int rank, int *idxs, int *rets)
    {
    int lo, hi, step, i;

    if (place == rank)
        *(rets++) = flat_index (rank, szs, idxs);
    else
        {
	lo = ranges[3*place];
	hi = ranges[3*place+1];
	step = ranges[3*place+2];

	for (i=lo; i<=hi; i+=step)
	    {
	    idxs[place] = i;
	    rets = mk_indices (place+1, ranges, szs, rank, idxs, rets);
	    }
	}
    
    return rets;
    }

int flat_index (int rank, int *szs, int *idxs)
    {
    int ac, i;

    ac = idxs[0];
    for (i=1; i<rank; i++)
        ac = ac * szs[i] + idxs[i];
    return ac;
    }

int find_array_source (FuncGraph *fg, int nd, int pt, int *rnd, int *rpt, int *crosses)
    {
    int parent_node, parent_port;
    int g_out, graph, in_port;
    int fnd;

    /* if it reaches the input port of the ND_FUNC graph, back_edges==NULL
     * and there's no value to be retrieved
     */
    if (fg->nodes[nd].inputs[pt].back_edges==NULL)
	return FALSE;

    /* there should be exactly one back edge on this port */
    assert (fg->nodes[nd].inputs[pt].back_edges->link == NULL);

    parent_node = fg->nodes[nd].inputs[pt].back_edges->node;
    parent_port = fg->nodes[nd].inputs[pt].back_edges->port;

    switch (fg->nodes[parent_node].nodetype)
	{
        case ND_SWITCH :
	    /* we don't follow this up into a SWITCH */
	    return FALSE;
	case ND_ARR_DEF :
	    *rnd = parent_node;
	    *rpt = 0;
	    return TRUE;
	case ND_WHILE :
	case ND_FORALL :
	case ND_FORNXT :
	case ND_FUNC :
	case ND_CROSS_PROD :
	case ND_DOT_PROD :
	    g_out = fg->nodes[parent_node].My_outputs[parent_port];
	    fnd = find_array_source (fg, g_out, 0, rnd, rpt, crosses);
	    *crosses = TRUE;
	    return fnd;
	case ND_G_INPUT:
	    graph = fg->nodes[parent_node].My_graph;
	    in_port = fg->nodes[parent_node].Io_num;
	    fnd = find_array_source (fg, graph, in_port, rnd, rpt, crosses);
	    *crosses = TRUE;
	    return fnd;
	case ND_CASE :
	case ND_WHILE_PRED :
	    assert (0);
	default :
	    return FALSE;
	}
    }

/* follow backedges up, looking for a constant string; return empty
 * string if constant can't be found
 */
int getvalstr (FuncGraph *fg, int nd, int pt, int *idxs, char *str,
				TypeInfo *ty, int *rnd, int *rpt, int *crossed)
    {
    int parent_node, parent_port;
    int *extents;
    int ac, i, g_out, graph, in_port;
    int fnd;

    /* if it reaches the input port of the ND_FUNC graph, back_edges==NULL
     * and there's no value to be retrieved
     */
    if (fg->nodes[nd].inputs[pt].back_edges==NULL)
	return FALSE;

    /* there should be exactly one back edge on this port */
    assert (fg->nodes[nd].inputs[pt].back_edges->link == NULL);

    parent_node = fg->nodes[nd].inputs[pt].back_edges->node;
    parent_port = fg->nodes[nd].inputs[pt].back_edges->port;

    switch (fg->nodes[parent_node].nodetype)
	{
        case ND_SWITCH :
	    /* we don't follow this up into a SWITCH */
	    return FALSE;
	case ND_ARR_DEF :
	    extents = fg->nodes[parent_node].DefDims;
	    ac = idxs[0];
	    for (i=1; i<fg->nodes[parent_node].DefRank; i++)
	        ac = ac * extents[i] + idxs[i];
	    
	    if (ac >= fg->nodes[parent_node].num_inputs)
	        {
		handle_error (ErrArrayConstProp, Deadly, fg->nodes[nd].loc.line,
				fg->nodes[nd].loc.file, fg->nodes[nd].loc.func, ac);
		}

	    if (fg->nodes[parent_node].inputs[ac].is_const)
		{
		*rnd = -1;
		*rpt = -1;
	        strcpy (str, fg->nodes[parent_node].inputs[ac].constval);
		}
	    else
		{
		*rnd = fg->nodes[parent_node].inputs[ac].back_edges->node;
		*rpt = fg->nodes[parent_node].inputs[ac].back_edges->port;
		strcpy (str, "");
		}

	    copy_type_info (ty, &(fg->nodes[parent_node].inputs[ac].ty));
	    return TRUE;
	case ND_WHILE :
	case ND_FORALL :
	case ND_FORNXT :
	case ND_FUNC :
	case ND_CROSS_PROD :
	case ND_DOT_PROD :
	    g_out = fg->nodes[parent_node].My_outputs[parent_port];
	    fnd = getvalstr (fg, g_out, 0, idxs, str, ty, rnd, rpt, crossed);
	    *crossed = TRUE;
	    return fnd;
	case ND_G_INPUT:
	    graph = fg->nodes[parent_node].My_graph;
	    in_port = fg->nodes[parent_node].Io_num;
	    fnd = getvalstr (fg, graph, in_port, idxs, str, ty, rnd, rpt, crossed);
	    *crossed = TRUE;
	    return fnd;
	case ND_CASE :
	case ND_WHILE_PRED :
	    assert (0);
	default :
	    return FALSE;
	}
    }
