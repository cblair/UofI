#include "sassy.h"
#include "ddcf.h"
#include "error.h"

int ids_process_graphs_nodes (FuncGraph*, int);
int ids_node (FuncGraph*, int, IntList**);
int ids_add (FuncGraph*, int, IntList**);
int ids_sub (FuncGraph*, int, IntList**);
int ids_mul (FuncGraph*, int, IntList**);
int ids_div (FuncGraph*, int, IntList**);
int ids_and (FuncGraph*, int, IntList**);
int ids_or (FuncGraph*, int, IntList**);
int ids_eor (FuncGraph*, int, IntList**);
int ids_shifts (FuncGraph*, int, IntList**);
int ids_select (FuncGraph*, int, IntList**);
int ids_ch_width (FuncGraph*, int, IntList**);
int is_all_ones (FuncGraph*, int, int, int);
int is_zero_const (FuncGraph*, int, int);
int zero_const_w_type (Type, char*);
int is_one_const (FuncGraph*, int, int);
int is_neg_one_const (FuncGraph*, int, int);
void append_target_list (Edge**, Edge*);
int val_from_hex_string (char*);

void identities ()
    {
    FuncGraph *fg;
    int changed;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
	    {
            changed = ids_process_graphs_nodes (fg, 0);
	    if (changed)
	        is_stable = FALSE;
	    }
    }

int ids_process_graphs_nodes (FuncGraph *fg, int id)
    {
    IntList *it, *list_copy;
    int deleted, nd;
    int changed=FALSE;

    copy_intlist (&list_copy, &(fg->nodes[id].My_nodes));

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;

	if (is_simple_node (fg, nd))
            changed = ids_node (fg, nd, &list_copy) || changed;
	else
	    changed = ids_process_graphs_nodes (fg, nd) || changed;
	}

    free_intlist (&(fg->nodes[id].My_nodes));
    fg->nodes[id].My_nodes = list_copy;

    return changed;
    }

int ids_node (FuncGraph *fg, int id, IntList **itlist)
    {
    int changed = FALSE;

    switch (fg->nodes[id].nodetype)
        {
	case ND_ADD :
	    changed = ids_add (fg, id, itlist);
	    break;
	case ND_SUB :
	    changed = ids_sub (fg, id, itlist);
	    break;
	case ND_MUL :
	    changed = ids_mul (fg, id, itlist);
	    break;
	case ND_DIV :
	    changed = ids_div (fg, id, itlist);
	    break;
	case ND_BIT_OR :
	    changed = ids_or (fg, id, itlist);
	    break;
	case ND_BIT_AND :
	    changed = ids_and (fg, id, itlist);
	    break;
	case ND_BIT_EOR :
	    changed = ids_eor (fg, id, itlist);
	    break;
	case ND_LEFT_SHIFT_MACH :
	case ND_LEFT_SHIFT :
	case ND_RIGHT_SHIFT :
	    changed = ids_shifts (fg, id, itlist);
	    break;
        case ND_RC_SELECTOR :
	    changed = ids_select (fg, id, itlist);
	    break;
	case ND_CHANGE_WIDTH :
	    changed = ids_ch_width (fg, id, itlist);
	    break;
	default :
            break;
	}

    return changed;
    }

int ids_ch_width (FuncGraph *fg, int id, IntList **itlist)
    {
    char str[256];
    int changed = FALSE;
    int snd, spt;
    unsigned int val;

    sscanf (fg->nodes[id].inputs[1].constval, "%x", &val);

    if (val == fg->nodes[id].inputs[0].ty.totsize)
        {
	changed = TRUE;

	/* propagate the value (const or edge) to this node's targets */
        if (fg->nodes[id].inputs[0].is_const)
            {
    	    sscanf (fg->nodes[id].inputs[0].constval, "%x", &val);
            val &= mask_ones[fg->nodes[id].outputs[0].ty.totsize];
            sprintf (str, "0x%x", val);
            transfer_const_to_targets (fg, id, 0, str);
            free_edge_list (&(fg->nodes[id].outputs[0].targets));
            }
        else
            {
            snd = fg->nodes[id].inputs[0].back_edges->node;
            spt = fg->nodes[id].inputs[0].back_edges->port;
            remove_edge (fg, snd, spt, id, 0);
            retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, snd, spt);
            append_target_list (&(fg->nodes[snd].outputs[spt].targets), fg->nodes[id].outputs[0].targets);
            fg->nodes[id].outputs[0].targets = NULL;
            }
	}

    return changed;
    }

/*  it's easier to let dead code elimination clean up after this one... */
int ids_select (FuncGraph *fg, int id, IntList **itlist)
    {
    char str[256];
    int changed = FALSE;
    unsigned int kval, val;
    int snd, spt;
    int i;

    if (fg->nodes[id].inputs[0].is_const)
        {
	changed = TRUE;

	/* find which input corresponds to the key value */
        sscanf (fg->nodes[id].inputs[0].constval, "%x", &kval);
        for (i=1; i<fg->nodes[id].num_inputs-1; i+=2)
            {
            assert (fg->nodes[id].inputs[i].is_const);
            sscanf (fg->nodes[id].inputs[i].constval, "%x", &val);
            if (val == kval)
                break;
            }

        if (i >= fg->nodes[id].num_inputs-1)
            i = fg->nodes[id].num_inputs-1;
        else
            i++;

	/* propagate the value (const or edge) to the SELECTOR's targets */
        if (fg->nodes[id].inputs[i].is_const)
            {
            val &= mask_ones[fg->nodes[id].outputs[0].ty.totsize];
            sprintf (str, "0x%x", val);
            transfer_const_to_targets (fg, id, 0, str);
            free_edge_list (&(fg->nodes[id].outputs[0].targets));
            }
        else
            {
            snd = fg->nodes[id].inputs[i].back_edges->node;
            spt = fg->nodes[id].inputs[i].back_edges->port;
            remove_edge (fg, snd, spt, id, i);
            retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, snd, spt);
            append_target_list (&(fg->nodes[snd].outputs[spt].targets), fg->nodes[id].outputs[0].targets);
            fg->nodes[id].outputs[0].targets = NULL;
            }
        }

    return changed;
    }

int ids_shifts (FuncGraph *fg, int id, IntList **itlist)
    {
    int changed = FALSE;
    int src_node, src_port;

    if (is_zero_const (fg, id, 1))
        {
	changed = TRUE;

	src_node = fg->nodes[id].inputs[0].back_edges->node;
	src_port = fg->nodes[id].inputs[0].back_edges->port;

	remove_targets_to_this_port (fg, fg->nodes[id].inputs[0].back_edges, id, 0);
	retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, src_node, src_port);
	append_target_list (&(fg->nodes[src_node].outputs[src_port].targets),
	    				fg->nodes[id].outputs[0].targets);
	free_edge_list (&(fg->nodes[id].inputs[0].back_edges));
	free_ddcf_node (fg, id, itlist);
	}

    return changed;
    }

int ids_add (FuncGraph *fg, int id, IntList **itlist)
    {
    int changed = FALSE;
    int src_node, src_port;

    if (is_zero_const (fg, id, 0))
        {
	changed = TRUE;

	if (type_equal (&(fg->nodes[id].inputs[1].ty), &(fg->nodes[id].outputs[0].ty)))
	    {
	    if (fg->nodes[id].inputs[1].is_const)
		assert (0);

	    src_node = fg->nodes[id].inputs[1].back_edges->node;
	    src_port = fg->nodes[id].inputs[1].back_edges->port;

	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[1].back_edges, id, 1);
	    retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, src_node, src_port);
	    append_target_list (&(fg->nodes[src_node].outputs[src_port].targets),
	    				fg->nodes[id].outputs[0].targets);
	    free_edge_list (&(fg->nodes[id].inputs[1].back_edges));
	    free_ddcf_node (fg, id, itlist);
	    }
	else
	    {
	    fg->nodes[id].nodetype = ND_CAST;
	    fg->nodes[id].inputs[0] = fg->nodes[id].inputs[1];
	    retarget (fg, fg->nodes[id].inputs[0].back_edges, id, 1, id, 0);
	    fg->nodes[id].num_inputs = 1;
	    }
	}
    else if (is_zero_const (fg, id, 1))
        {
	changed = TRUE;

	if (type_equal (&(fg->nodes[id].inputs[0].ty), &(fg->nodes[id].outputs[0].ty)))
	    {
	    src_node = fg->nodes[id].inputs[0].back_edges->node;
	    src_port = fg->nodes[id].inputs[0].back_edges->port;

	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[0].back_edges, id, 0);
	    retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, src_node, src_port);
	    append_target_list (&(fg->nodes[src_node].outputs[src_port].targets),
	    				fg->nodes[id].outputs[0].targets);
	    free_edge_list (&(fg->nodes[id].inputs[0].back_edges));
	    free_ddcf_node (fg, id, itlist);
	    }
	else
	    {
	    fg->nodes[id].nodetype = ND_CAST;
	    fg->nodes[id].num_inputs = 1;
	    }
	}

    return changed;
    }

int ids_sub (FuncGraph *fg, int id, IntList **itlist)
    {
    int changed = FALSE;
    int src_node, src_port;

    if (is_zero_const (fg, id, 1))
        {
	changed = TRUE;

	if (type_equal (&(fg->nodes[id].inputs[0].ty), &(fg->nodes[id].outputs[0].ty)))
	    {
	    src_node = fg->nodes[id].inputs[0].back_edges->node;
	    src_port = fg->nodes[id].inputs[0].back_edges->port;

	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[0].back_edges, id, 0);
	    retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, src_node, src_port);
	    append_target_list (&(fg->nodes[src_node].outputs[src_port].targets),
	    				fg->nodes[id].outputs[0].targets);
	    free_edge_list (&(fg->nodes[id].inputs[0].back_edges));
	    free_ddcf_node (fg, id, itlist);
	    }
	else
	    {
	    fg->nodes[id].nodetype = ND_CAST;
	    fg->nodes[id].num_inputs = 1;
	    }
	}

    return changed;
    }

int ids_div (FuncGraph *fg, int id, IntList **itlist)
    {
    int changed = FALSE;
    int src_node, src_port;

    if (is_one_const (fg, id, 1))
        {
	changed = TRUE;

	if (type_equal (&(fg->nodes[id].inputs[0].ty), &(fg->nodes[id].outputs[0].ty)))
	    {
	    src_node = fg->nodes[id].inputs[0].back_edges->node;
	    src_port = fg->nodes[id].inputs[0].back_edges->port;

	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[0].back_edges, id, 0);
	    retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, src_node, src_port);
	    append_target_list (&(fg->nodes[src_node].outputs[src_port].targets),
	    				fg->nodes[id].outputs[0].targets);
	    free_edge_list (&(fg->nodes[id].inputs[0].back_edges));
	    free_ddcf_node (fg, id, itlist);
	    }
	else
	    {
	    fg->nodes[id].nodetype = ND_CAST;
	    fg->nodes[id].num_inputs = 1;
	    }
	}

    return changed;
    }

int ids_mul (FuncGraph *fg, int id, IntList **itlist)
    {
    Edge *tg;
    int changed = FALSE;
    int src_node, src_port, node, port;

    if (is_one_const (fg, id, 0))
        {
	changed = TRUE;

	if (type_equal (&(fg->nodes[id].inputs[1].ty), &(fg->nodes[id].outputs[0].ty)))
	    {
	    src_node = fg->nodes[id].inputs[1].back_edges->node;
	    src_port = fg->nodes[id].inputs[1].back_edges->port;

	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[1].back_edges, id, 1);
	    retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, src_node, src_port);
	    append_target_list (&(fg->nodes[src_node].outputs[src_port].targets),
	    				fg->nodes[id].outputs[0].targets);
	    free_edge_list (&(fg->nodes[id].inputs[1].back_edges));
	    free_ddcf_node (fg, id, itlist);
	    }
	else
	    {
	    fg->nodes[id].nodetype = ND_CAST;
	    fg->nodes[id].inputs[0] = fg->nodes[id].inputs[1];
	    retarget (fg, fg->nodes[id].inputs[0].back_edges, id, 1, id, 0);
	    fg->nodes[id].num_inputs = 1;
	    }
	}
    else if (is_one_const (fg, id, 1))
        {
	changed = TRUE;

	if (type_equal (&(fg->nodes[id].inputs[0].ty), &(fg->nodes[id].outputs[0].ty)))
	    {
	    src_node = fg->nodes[id].inputs[0].back_edges->node;
	    src_port = fg->nodes[id].inputs[0].back_edges->port;

	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[0].back_edges, id, 0);
	    retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, src_node, src_port);
	    append_target_list (&(fg->nodes[src_node].outputs[src_port].targets),
	    				fg->nodes[id].outputs[0].targets);
	    free_edge_list (&(fg->nodes[id].inputs[0].back_edges));
	    free_ddcf_node (fg, id, itlist);
	    }
	else
	    {
	    fg->nodes[id].nodetype = ND_CAST;
	    fg->nodes[id].num_inputs = 1;
	    }
	}
    else if (is_zero_const (fg, id, 0))
        {
	changed = TRUE;

	if (type_equal (&(fg->nodes[id].inputs[0].ty), &(fg->nodes[id].outputs[0].ty)))
	    {
	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[1].back_edges, id, 1);
	    free_edge_list (&(fg->nodes[id].inputs[1].back_edges));

	    remove_backedges_to_this_target (fg, fg->nodes[id].outputs[0].targets, id, 0);

	    for (tg=fg->nodes[id].outputs[0].targets; tg!=NULL; tg=tg->link)
	        {
		node = tg->node;
		port = tg->port;
		fg->nodes[node].inputs[port].is_const = TRUE;
		/*
		strcpy (fg->nodes[node].inputs[port].constval, fg->nodes[id].inputs[0].constval);
		*/
		fg->nodes[node].inputs[port].constval = copy_to_const (fg->nodes[id].inputs[0].constval);
		}

	    free_edge_list (&(fg->nodes[id].outputs[0].targets));
	    free_ddcf_node (fg, id, itlist);
	    }
	else
	    {
	    fg->nodes[id].nodetype = ND_CAST;
	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[1].back_edges, id, 1);
	    free_edge_list (&(fg->nodes[id].inputs[1].back_edges));
	    fg->nodes[id].num_inputs = 1;
	    }
	}
    else if (is_zero_const (fg, id, 1))
        {
	changed = TRUE;

	if (type_equal (&(fg->nodes[id].inputs[1].ty), &(fg->nodes[id].outputs[0].ty)))
	    {
	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[0].back_edges, id, 0);
	    free_edge_list (&(fg->nodes[id].inputs[0].back_edges));

	    remove_backedges_to_this_target (fg, fg->nodes[id].outputs[0].targets, id, 0);

	    for (tg=fg->nodes[id].outputs[0].targets; tg!=NULL; tg=tg->link)
	        {
		node = tg->node;
		port = tg->port;
		fg->nodes[node].inputs[port].is_const = TRUE;
		/*
		strcpy (fg->nodes[node].inputs[port].constval, fg->nodes[id].inputs[1].constval);
		*/
		fg->nodes[node].inputs[port].constval = copy_to_const (fg->nodes[id].inputs[1].constval);
		}

	    free_edge_list (&(fg->nodes[id].outputs[0].targets));
	    free_ddcf_node (fg, id, itlist);
	    }
	else
	    {
	    fg->nodes[id].nodetype = ND_CAST;
	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[0].back_edges, id, 0);
	    free_edge_list (&(fg->nodes[id].inputs[0].back_edges));
	    fg->nodes[id].inputs[0] = fg->nodes[id].inputs[1];
	    fg->nodes[id].num_inputs = 1;
	    }
	}
    else if (is_neg_one_const (fg, id, 0))
        {
	changed = TRUE;

	fg->nodes[id].nodetype = ND_NEG;
	fg->nodes[id].inputs[0] = fg->nodes[id].inputs[1];
	fg->nodes[id].num_inputs = 1;
	retarget (fg, fg->nodes[id].inputs[0].back_edges, id, 1, id, 0);
	}
    else if (is_neg_one_const (fg, id, 1))
        {
	changed = TRUE;

	fg->nodes[id].nodetype = ND_NEG;
	fg->nodes[id].num_inputs = 1;
	}

    return changed;
    }

int ids_and (FuncGraph *fg, int id, IntList **itlist)
    {
    Edge *tg;
    int changed = FALSE;
    int src_node, src_port, node, port;

    if (is_all_ones (fg, id, 0, fg->nodes[id].outputs[0].ty.totsize))
        {
	changed = TRUE;

	if (fg->nodes[id].inputs[1].ty.totsize == fg->nodes[id].outputs[0].ty.totsize)
	    {
	    src_node = fg->nodes[id].inputs[1].back_edges->node;
	    src_port = fg->nodes[id].inputs[1].back_edges->port;

	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[1].back_edges, id, 1);
	    retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, src_node, src_port);
	    append_target_list (&(fg->nodes[src_node].outputs[src_port].targets),
	    				fg->nodes[id].outputs[0].targets);
	    free_edge_list (&(fg->nodes[id].inputs[1].back_edges));
	    free_ddcf_node (fg, id, itlist);
	    }
	else
	    {
	    fg->nodes[id].inputs[0] = fg->nodes[id].inputs[1];
	    retarget (fg, fg->nodes[id].inputs[0].back_edges, id, 1, id, 0);
	    fg->nodes[id].inputs[1].back_edges = NULL;
	    fg->nodes[id].num_inputs = 1;
	    fg->nodes[id].nodetype = ND_CAST;
	    }
	}
    else if (is_all_ones (fg, id, 1, fg->nodes[id].outputs[0].ty.totsize))
        {
	changed = TRUE;

	if (fg->nodes[id].inputs[0].ty.totsize == fg->nodes[id].outputs[0].ty.totsize)
	    {
	    src_node = fg->nodes[id].inputs[0].back_edges->node;
	    src_port = fg->nodes[id].inputs[0].back_edges->port;

	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[0].back_edges, id, 0);
	    retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, src_node, src_port);
	    append_target_list (&(fg->nodes[src_node].outputs[src_port].targets),
	    				fg->nodes[id].outputs[0].targets);
	    free_edge_list (&(fg->nodes[id].inputs[0].back_edges));
	    free_ddcf_node (fg, id, itlist);
	    }
	else
	    {
	    fg->nodes[id].num_inputs = 1;
	    fg->nodes[id].nodetype = ND_CAST;
	    }
	}
    else if (is_zero_const (fg, id, 0))
        {
	changed = TRUE;

	remove_targets_to_this_port (fg, fg->nodes[id].inputs[1].back_edges, id, 1);
	free_edge_list (&(fg->nodes[id].inputs[1].back_edges));

	remove_backedges_to_this_target (fg, fg->nodes[id].outputs[0].targets, id, 0);

	for (tg=fg->nodes[id].outputs[0].targets; tg!=NULL; tg=tg->link)
	    {
	    node = tg->node;
	    port = tg->port;
	    fg->nodes[node].inputs[port].is_const = TRUE;
	    fg->nodes[node].inputs[port].constval = copy_to_const (fg->nodes[id].inputs[0].constval);
	    }

	free_edge_list (&(fg->nodes[id].outputs[0].targets));
	free_ddcf_node (fg, id, itlist);
	}
    else if (is_zero_const (fg, id, 1))
        {
	changed = TRUE;

	remove_targets_to_this_port (fg, fg->nodes[id].inputs[0].back_edges, id, 0);
	free_edge_list (&(fg->nodes[id].inputs[0].back_edges));

	remove_backedges_to_this_target (fg, fg->nodes[id].outputs[0].targets, id, 0);

	for (tg=fg->nodes[id].outputs[0].targets; tg!=NULL; tg=tg->link)
	    {
	    node = tg->node;
	    port = tg->port;
	    fg->nodes[node].inputs[port].is_const = TRUE;
	    fg->nodes[node].inputs[port].constval = copy_to_const (fg->nodes[id].inputs[1].constval);
	    }

	free_edge_list (&(fg->nodes[id].outputs[0].targets));
	free_ddcf_node (fg, id, itlist);
	}

    return changed;
    }

int ids_or (FuncGraph *fg, int id, IntList **itlist)
    {
    Edge *tg;
    int changed = FALSE;
    int src_node, src_port, node, port;
    char str[256];

    if (is_zero_const (fg, id, 0))
        {
	changed = TRUE;

	if (fg->nodes[id].inputs[1].ty.totsize == fg->nodes[id].outputs[0].ty.totsize)
	    {
	    src_node = fg->nodes[id].inputs[1].back_edges->node;
	    src_port = fg->nodes[id].inputs[1].back_edges->port;

	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[1].back_edges, id, 1);
	    retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, src_node, src_port);
	    append_target_list (&(fg->nodes[src_node].outputs[src_port].targets),
	    				fg->nodes[id].outputs[0].targets);
	    free_edge_list (&(fg->nodes[id].inputs[1].back_edges));
	    free_ddcf_node (fg, id, itlist);
	    }
	else
	    {
	    fg->nodes[id].inputs[0] = fg->nodes[id].inputs[1];
	    retarget (fg, fg->nodes[id].inputs[0].back_edges, id, 1, id, 0);
	    fg->nodes[id].inputs[1].back_edges = NULL;
	    fg->nodes[id].num_inputs = 1;
	    fg->nodes[id].nodetype = ND_CAST;
	    }
	}
    else if (is_zero_const (fg, id, 1))
        {
	changed = TRUE;

	if (fg->nodes[id].inputs[0].ty.totsize == fg->nodes[id].outputs[0].ty.totsize)
	    {
	    src_node = fg->nodes[id].inputs[0].back_edges->node;
	    src_port = fg->nodes[id].inputs[0].back_edges->port;

	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[0].back_edges, id, 0);
	    retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, src_node, src_port);
	    append_target_list (&(fg->nodes[src_node].outputs[src_port].targets),
	    				fg->nodes[id].outputs[0].targets);
	    free_edge_list (&(fg->nodes[id].inputs[0].back_edges));
	    free_ddcf_node (fg, id, itlist);
	    }
	else
	    {
	    fg->nodes[id].num_inputs = 1;
	    fg->nodes[id].nodetype = ND_CAST;
	    }
	}
    else if (is_all_ones (fg, id, 0, fg->nodes[id].outputs[0].ty.totsize))
        {
	changed = TRUE;

	remove_targets_to_this_port (fg, fg->nodes[id].inputs[1].back_edges, id, 1);
	free_edge_list (&(fg->nodes[id].inputs[1].back_edges));

	remove_backedges_to_this_target (fg, fg->nodes[id].outputs[0].targets, id, 0);

	for (tg=fg->nodes[id].outputs[0].targets; tg!=NULL; tg=tg->link)
	    {
	    node = tg->node;
	    port = tg->port;
	    fg->nodes[node].inputs[port].is_const = TRUE;
	    fg->nodes[node].inputs[port].constval = copy_to_const (fg->nodes[id].inputs[0].constval);
	    }

	free_edge_list (&(fg->nodes[id].outputs[0].targets));
	free_ddcf_node (fg, id, itlist);
	}
    else if (is_all_ones (fg, id, 1, fg->nodes[id].outputs[0].ty.totsize))
        {
	changed = TRUE;

	remove_targets_to_this_port (fg, fg->nodes[id].inputs[0].back_edges, id, 0);
	free_edge_list (&(fg->nodes[id].inputs[0].back_edges));

	remove_backedges_to_this_target (fg, fg->nodes[id].outputs[0].targets, id, 0);

	for (tg=fg->nodes[id].outputs[0].targets; tg!=NULL; tg=tg->link)
	    {
	    node = tg->node;
	    port = tg->port;
	    fg->nodes[node].inputs[port].is_const = TRUE;
	    fg->nodes[node].inputs[port].constval = copy_to_const (fg->nodes[id].inputs[1].constval);
	    }

	free_edge_list (&(fg->nodes[id].outputs[0].targets));
	free_ddcf_node (fg, id, itlist);
	}

    return changed;
    }

int ids_eor (FuncGraph *fg, int id, IntList **itlist)
    {
    int changed = FALSE;
    int src_node, src_port;

    if (is_zero_const (fg, id, 0))
        {
	changed = TRUE;

	if (fg->nodes[id].inputs[1].ty.totsize == fg->nodes[id].outputs[0].ty.totsize)
	    {
	    src_node = fg->nodes[id].inputs[1].back_edges->node;
	    src_port = fg->nodes[id].inputs[1].back_edges->port;

	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[1].back_edges, id, 1);
	    retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, src_node, src_port);
	    append_target_list (&(fg->nodes[src_node].outputs[src_port].targets),
	    				fg->nodes[id].outputs[0].targets);
	    free_edge_list (&(fg->nodes[id].inputs[1].back_edges));
	    free_ddcf_node (fg, id, itlist);
	    }
	else
	    {
	    fg->nodes[id].inputs[0] = fg->nodes[id].inputs[1];
	    retarget (fg, fg->nodes[id].inputs[0].back_edges, id, 1, id, 0);
	    fg->nodes[id].inputs[1].back_edges = NULL;
	    fg->nodes[id].num_inputs = 1;
	    fg->nodes[id].nodetype = ND_CAST;
	    }
        }
    else if (is_zero_const (fg, id, 1))
        {
	changed = TRUE;

	if (fg->nodes[id].inputs[0].ty.totsize == fg->nodes[id].outputs[0].ty.totsize)
	    {
	    src_node = fg->nodes[id].inputs[0].back_edges->node;
	    src_port = fg->nodes[id].inputs[0].back_edges->port;

	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[0].back_edges, id, 0);
	    retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, src_node, src_port);
	    append_target_list (&(fg->nodes[src_node].outputs[src_port].targets),
	    				fg->nodes[id].outputs[0].targets);
	    free_edge_list (&(fg->nodes[id].inputs[0].back_edges));
	    free_ddcf_node (fg, id, itlist);
	    }
	else
	    {
	    fg->nodes[id].num_inputs = 1;
	    fg->nodes[id].nodetype = ND_CAST;
	    }
	}

    return changed;
    }

int is_all_ones (FuncGraph *fg, int nd, int pt, int width)
    {
    int ival;

    if (!fg->nodes[nd].inputs[pt].is_const)
        return FALSE;

    sscanf (fg->nodes[nd].inputs[pt].constval, "%x", &ival);

    return (mask_ones[width] == ival);
    }

int is_zero_const (FuncGraph *fg, int nd, int pt)
    {

    if (!fg->nodes[nd].inputs[pt].is_const)
        return FALSE;

    return zero_const_w_type (fg->nodes[nd].inputs[pt].ty.type, fg->nodes[nd].inputs[pt].constval);
    }

int zero_const_w_type (Type ty, char *str)
    {
    int ival;
    float fval;
    double dval;

    switch (ty)
        {
	case Uint :
	case Int :
	    sscanf (str, "%d", &ival);
	    return (ival == 0);
	case Ufix :
	case Fix :
	    ival = val_from_hex_string (str);
	    return (ival == 0);
	case Float :
	    sscanf (str, "%f", &fval);
	    return (fval == 0.0);
	case Double :
	    sscanf (str, "%lf", &dval);
	    return (dval == 0.0);
	case Bits :
	    sscanf (str, "%x", &ival);
	    return (ival == 0);
	default :
	    assert (0);
	}
    }

int is_neg_one_const (FuncGraph *fg, int nd, int pt)
    {
    int ival;
    float fval;
    double dval;

    if (!fg->nodes[nd].inputs[pt].is_const)
        return FALSE;

    switch (fg->nodes[nd].inputs[pt].ty.type)
        {
	case Int :
	    sscanf (fg->nodes[nd].inputs[pt].constval, "%d", &ival);
	    return (ival == -1);
	case Fix :
	    {
	    int neg_one_val = (0xffffffff <<  fg->nodes[nd].inputs[pt].ty.fracsize) &
	    				mask_ones[fg->nodes[nd].inputs[pt].ty.totsize];
	    ival = val_from_hex_string (fg->nodes[nd].inputs[pt].constval);
	    return (ival == neg_one_val);
	    }
	case Float :
	    sscanf (fg->nodes[nd].inputs[pt].constval, "%f", &fval);
	    return (fval == -1.0);
	case Double :
	    sscanf (fg->nodes[nd].inputs[pt].constval, "%lf", &dval);
	    return (dval == -1.0);
	case Uint :
	case Ufix :
	    return FALSE;
	default :
	    assert (0);
	}
    }

int is_one_const (FuncGraph *fg, int nd, int pt)
    {
    int ival;
    float fval;
    double dval;

    if (!fg->nodes[nd].inputs[pt].is_const)
        return FALSE;

    switch (fg->nodes[nd].inputs[pt].ty.type)
        {
	case Uint :
	case Int :
	    sscanf (fg->nodes[nd].inputs[pt].constval, "%d", &ival);
	    return (ival == 1);
	case Ufix :
	case Fix :
	    {
	    int one_val = 1 << fg->nodes[nd].inputs[pt].ty.fracsize;
	    ival = val_from_hex_string (fg->nodes[nd].inputs[pt].constval);
	    return (ival == one_val);
	    }
	case Float :
	    sscanf (fg->nodes[nd].inputs[pt].constval, "%f", &fval);
	    return (fval == 1.0);
	case Double :
	    sscanf (fg->nodes[nd].inputs[pt].constval, "%lf", &dval);
	    return (dval == 1.0);
	case Bits :
	    sscanf (fg->nodes[nd].inputs[pt].constval, "%x", &ival);
	    return (ival == 1);
	default :
	    assert (0);
	}
    }
