#include "sassy.h"
#include "ddcf.h"
#include "error.h"

void remove_simple_node (FuncGraph*, int);
int is_simple_node (FuncGraph*, int);
int dead_code_node (FuncGraph*, int);
IntList *dead_code_process_list_reversed (FuncGraph*, IntList*);
void dead_code_graph (FuncGraph*, int);
void dead_code ();
void delete_node (FuncGraph*, int);
void delete_output (FuncGraph*, int, int);
void delete_dead_graph_inputs (FuncGraph*, int, IntList**);
void move_graph_input (FuncGraph*, int, int, int);
void delete_intlist_rev (FuncGraph*, IntList*);
void delete_from_int_array (int, int*, int);

void dead_code ()
    {
    FuncGraph *fg;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
            dead_code_graph (fg, 0);
    }

void dead_code_graph (FuncGraph *fg, int id)
    {
    IntList *deleted, *it;

    deleted = dead_code_process_list_reversed (fg, fg->nodes[id].My_nodes);

    for (it=deleted; it!=NULL; it=it->link)
	{
        delete_ele_from_intlist (it->val, &(fg->nodes[id].My_nodes));
	}
    
    free_intlist (&deleted);
    }

IntList *dead_code_process_list_reversed (FuncGraph *fg, IntList *lst)
    {
    IntList *deleted = NULL, *list_copy, *it;
    int nd;

    copy_intlist (&list_copy, &lst);
    reverse_intlist (&list_copy);

    for (it=list_copy; it!=NULL; it=it->link)
        {
	nd = it->val;
	if (dead_code_node (fg, nd))
	    add_to_list (nd, &deleted);
	}

    free_intlist (&list_copy);

    return deleted;
    }

int dead_code_node (FuncGraph *fg, int id)
    {
    int keep, i, g_id, port_num, deleted;

    /*
    printf ("entered 'dead_code_node', node %d\n", id); fflush (stdout);
    */

    deleted = FALSE;

    if (is_simple_node (fg, id))
        {
	keep = FALSE;

	switch (fg->nodes[id].nodetype)
	    {
	    case ND_G_OUTPUT :
	        g_id = fg->nodes[id].My_graph;
	        port_num = fg->nodes[id].Io_num;

	        if (fg->nodes[g_id].outputs[port_num].targets != NULL)
	            keep = TRUE;
		break;
	    case ND_PRINT :
	    case ND_ASSERT :
	    case ND_SWITCH_KEY :
	    case ND_SELECTORS :
	    case ND_NEXT :
	    case ND_SCALAR_GEN :
	    case ND_ELE_GEN :
	    case ND_SLICE_GEN :
	    case ND_WINDOW_GEN :
		/* these nodes never have outputs, but we don't want to
		 * eliminate them; ND_WHILE_PRED is not here because it's
		 * a compound, not a simple node;
		 */
	        return FALSE;
	    default :
	        for (i=0; i<fg->nodes[id].num_outputs; i++)
		    if (fg->nodes[id].outputs[i].targets != NULL)
		        {
		        keep = TRUE;
		        break;
		        }
	    }

	/* if it's not an input node, and it has no output targets, delete it */
	if ((!keep) && (fg->nodes[id].nodetype!=ND_G_INPUT) &&
				(fg->nodes[id].nodetype!=ND_G_INPUT_NEXT))
	    {
	    remove_simple_node (fg, id);
	    deleted = TRUE;
	    }
	}
    else
        {
	/* here we're processing a compound node */
	IntList *deleted_nodes, *it;

	/* first see if any of the outputs are dead */
	if (fg->nodes[id].nodetype != ND_WHILE_PRED)
	    for (i=0; i<fg->nodes[id].num_outputs; i++)
	        if (fg->nodes[id].outputs[i].targets == NULL)
	            delete_output (fg, id, i);

	/* remove dead code from the node's internal graph */
	/*
        deleted_nodes = dead_code_process_list_reversed (fg, fg->nodes[id].My_nodes);
	*/
        deleted_nodes = dead_code_process_list_reversed (fg, fg->nodes[id].My_nodes);

	/* here we delete any input nodes that have no outputs */
	delete_dead_graph_inputs (fg, id, &deleted_nodes);

        for (it=deleted_nodes; it!=NULL; it=it->link)
            delete_ele_from_intlist (it->val, &(fg->nodes[id].My_nodes));
        free_intlist (&deleted_nodes);

	/* if the node has no outputs, delete the whole thing */
	switch (fg->nodes[id].nodetype)
	    {
	    case ND_WHILE_PRED :
	    case ND_CROSS_PROD :
	    case ND_DOT_PROD :
	        break;
	    default :
		if (fg->nodes[id].nodetype != ND_WHILE_PRED)
	    	    if (fg->nodes[id].num_outputs == 0)
			{
	        	delete_node (fg, id);
			deleted = TRUE;
			}
	    }
	}

    /*
    printf ("leaving 'dead_code_node', node %d\n", id); fflush (stdout);
    */

    return deleted;
    }

void delete_node (FuncGraph *fg, int id)
    {
    int i;

    is_stable = FALSE;

    if (is_simple_node (fg, id))
        {
	for (i=0; i<fg->nodes[id].num_outputs; i++)
	    free_edge_list (&(fg->nodes[id].outputs[i].targets));
	for (i=0; i<fg->nodes[id].num_inputs; i++)
	    if (!fg->nodes[id].inputs[i].is_const)
		{
	        remove_targets_to_this_port (fg, fg->nodes[id].inputs[i].back_edges, id, i);
	        free_edge_list (&(fg->nodes[id].inputs[i].back_edges));
		}
	SacFree (fg->nodes[id].outputs)
	fg->nodes[id].outputs = NULL;
	SacFree (fg->nodes[id].inputs)
	fg->nodes[id].inputs = NULL;
	fg->nodes[id].nodetype = ND_VOIDED;
	}
    else
        {
	for (i=0; i<fg->nodes[id].num_outputs; i++)
	    {
	    delete_node (fg, fg->nodes[id].My_outputs[i]);
	    free_edge_list (&(fg->nodes[id].outputs[i].targets));
	    }

	delete_intlist_rev (fg, fg->nodes[id].My_nodes);

	for (i=0; i<fg->nodes[id].num_inputs; i++)
	    if (!fg->nodes[id].inputs[i].is_const)
		{
	        remove_targets_to_this_port (fg, fg->nodes[id].inputs[i].back_edges, id, i);
	        free_edge_list (&(fg->nodes[id].inputs[i].back_edges));
		}

	free_intlist (&(fg->nodes[id].My_nodes));
	SacFree (fg->nodes[id].My_outputs)
	fg->nodes[id].My_outputs = NULL;
	SacFree (fg->nodes[id].My_inputs)
	fg->nodes[id].My_inputs = NULL;
	SacFree (fg->nodes[id].outputs)
	fg->nodes[id].outputs = NULL;
	SacFree (fg->nodes[id].inputs)
	fg->nodes[id].inputs = NULL;
	fg->nodes[id].nodetype = ND_VOIDED;
	}
    }

void delete_intlist_rev (FuncGraph *fg, IntList *it)
    {
    if (it == NULL)
        return;

    delete_intlist_rev (fg, it->link);
    delete_node (fg, it->val);
    }

/* delete output port 'port' from graph 'id' */
void delete_output (FuncGraph *fg, int id, int port)
    {
    int g_out, i;

    is_stable = FALSE;

    g_out = fg->nodes[id].My_outputs[port];

    remove_simple_node (fg, g_out);

    for (i=port; i<fg->nodes[id].num_outputs-1; i++)
        {
	fg->nodes[id].My_outputs[i] = fg->nodes[id].My_outputs[i+1];
	g_out = fg->nodes[id].My_outputs[i];
	fg->nodes[g_out].Io_num = i;
	fg->nodes[id].outputs[i] = fg->nodes[id].outputs[i+1];
	retarget_backedges (fg, fg->nodes[id].outputs[i].targets, id, i+1, id, i);
	}

    (fg->nodes[id].num_outputs)--;
    }

void remove_simple_node (FuncGraph *fg, int id)
    {
    int i;

    is_stable = FALSE;

    for (i=0; i<fg->nodes[id].num_inputs; i++)
	if (!fg->nodes[id].inputs[i].is_const)
	    {
	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[i].back_edges, id, i);
	    free_edge_list (&(fg->nodes[id].inputs[i].back_edges));
	    }

    if (fg->nodes[id].inputs != NULL)
        SacFree (fg->nodes[id].inputs)
    if (fg->nodes[id].outputs != NULL)
        SacFree (fg->nodes[id].outputs)
    fg->nodes[id].nodetype = ND_VOIDED;
    }

void delete_dead_graph_inputs (FuncGraph *fg, int id, IntList **deleted)
    {
    int i, j, g_in, nd, pt, cnt, ndd;
    IntList *it, *kept_inputs = NULL;

    /*
    printf ("entering 'delete_dead_graph_inputs', id = %d\n", id); fflush (stdout);
    */

    for (i=0, j=0; i<fg->nodes[id].num_inputs; i++)
        {
	g_in = fg->nodes[id].My_inputs[i];
	if (fg->nodes[g_in].outputs[0].targets == NULL)
	    {
	    is_stable = FALSE;

	    /* if this is a dead nextified input, we have to track down its
	     * associated ND_NEXT node and delete it as well
	     */
	    if (fg->nodes[g_in].nodetype == ND_G_INPUT_NEXT)
	        for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
		    {
		    ndd = it->val;
		    if ((fg->nodes[ndd].nodetype == ND_NEXT) && (fg->nodes[ndd].In_next_id == g_in))
			{
			remove_simple_node (fg, ndd);
			add_to_list (ndd, deleted);
			break;
			}
		    }

	    SacFree (fg->nodes[g_in].outputs)
	    fg->nodes[g_in].nodetype = ND_VOIDED;
	    add_to_list (g_in, deleted);

	    if (fg->nodes[id].inputs[i].back_edges != NULL)
	        {
	        nd = fg->nodes[id].inputs[i].back_edges->node;
	        pt = fg->nodes[id].inputs[i].back_edges->port;
	        remove_edge (fg, nd, pt, id, i);
		}
	    }
	else
	    {
	    append_intlist (&kept_inputs, g_in);
	    move_graph_input (fg, id, i, j);
	    j++;
	    }
	}

    for (it=kept_inputs, cnt=0; it!=NULL; it=it->link, cnt++)
        fg->nodes[id].My_inputs[cnt] = it->val;

    free_intlist (&kept_inputs);
    fg->nodes[id].num_inputs = cnt;
    }
