#include "sassy.h"
#include "error.h"
#include "ddcf.h"

int code_motion_pass ();
int code_motion_graph (FuncGraph*, int);
int is_invariant (FuncGraph*, int, int);
void hoist_node (FuncGraph*, int, int, int, IntList**, IntList**);

void code_motion ()
    {
    int changed;

    do {
	changed = code_motion_pass ();
        } while (changed);

    dead_code ();
    }

int code_motion_pass ()
    {
    FuncGraph *fg;
    int chng, changed = FALSE;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
            {
            chng = code_motion_graph (fg, 0);
	    if (chng)
		{
	        changed = TRUE;

		/* signal the global variable used in the optimization cycle */
		is_stable = FALSE;
		}
            }

    return changed;
    }

int code_motion_graph (FuncGraph *fg, int id)
    {
    IntList *lparent, *lchild, *it, *itt;
    int nd, ndd;
    int changed = FALSE;

    copy_intlist (&lparent, &(fg->nodes[id].My_nodes));

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;

	if (!is_simple_node (fg, nd))
	    {
	    code_motion_graph (fg, nd);

	    copy_intlist (&lchild, &(fg->nodes[nd].My_nodes));

	    for (itt=fg->nodes[nd].My_nodes; itt!=NULL; itt=itt->link)
	        {
		ndd = itt->val;

		if (is_invariant (fg, ndd, nd))
		    {
		    hoist_node (fg, ndd, nd, id, &lchild, &lparent);
		    changed = TRUE;
		    }
		}

	    free_intlist (&(fg->nodes[nd].My_nodes));
	    fg->nodes[nd].My_nodes = lchild;
	    }
	}

    free_intlist (&(fg->nodes[id].My_nodes));
    fg->nodes[id].My_nodes = lparent;

    return changed;
    }

void hoist_node (FuncGraph *fg, int id, int graph, int parent, IntList **glist, IntList **parlist)
    {
    InputPort *new_inputs;
    int i, j, cur_input_count, new_input_count, *new_input_ar;
    int node, port, gin, input_num;
    
    /* allocate new inputs */
    cur_input_count = fg->nodes[graph].num_inputs;
    new_input_count = cur_input_count + fg->nodes[id].num_outputs;
    new_inputs = make_input_ports (new_input_count);
    SacMalloc (new_input_ar, int*, new_input_count * sizeof (int))

    for (i=0; i<fg->nodes[graph].num_inputs; i++)
	{
        new_inputs[i] = fg->nodes[graph].inputs[i];
	new_input_ar[i] = fg->nodes[graph].My_inputs[i];
	}

    SacFree (fg->nodes[graph].inputs)
    fg->nodes[graph].inputs = new_inputs;
    fg->nodes[graph].num_inputs = new_input_count;
    SacFree (fg->nodes[graph].My_inputs)
    fg->nodes[graph].My_inputs = new_input_ar;

    for (i=cur_input_count, j=0; i<new_input_count; i++, j++)
        {
	gin = new_ddcf_node (fg, ND_G_INPUT, 0, 1, fg->nodes[graph].loc);

	fg->nodes[gin].Io_num = i;
	fg->nodes[gin].My_graph = graph;
	fg->nodes[graph].My_inputs[i] = gin;

	fg->nodes[gin].outputs[0] = fg->nodes[id].outputs[j];
	fg->nodes[id].outputs[j].targets = NULL;
	retarget_backedges (fg, fg->nodes[gin].outputs[0].targets, id, j, gin, 0);

	add_edge (fg, id, j, graph, i);
	copy_type_info (&(fg->nodes[graph].inputs[i].ty), &(fg->nodes[id].outputs[j].ty));

	add_to_list (gin, glist);
	}

    delete_ele_from_intlist (id, glist);
    insert_val_before_in_intlist (id, graph, parlist);

    /* deal with the node's inputs */
    for (i=0; i<fg->nodes[id].num_inputs; i++)
	if (! fg->nodes[id].inputs[i].is_const)
            {
	    assert (fg->nodes[id].inputs[i].back_edges != NULL);
	    gin = fg->nodes[id].inputs[i].back_edges->node;
	    assert (fg->nodes[gin].nodetype == ND_G_INPUT);
	    input_num = fg->nodes[gin].Io_num;
	    assert (fg->nodes[graph].inputs[input_num].back_edges != NULL);

	    remove_edge (fg, gin, 0, id, i);
	    node = fg->nodes[graph].inputs[input_num].back_edges->node;
	    port = fg->nodes[graph].inputs[input_num].back_edges->port;
	    add_edge (fg, node, port, id, i);
	    }
    }

int is_invariant (FuncGraph *fg, int id, int graph_id)
    {
    int node, i;
    
    /* we don't lift code out of these graphs */
    switch (fg->nodes[graph_id].nodetype)
        {
	case ND_CASE :
	case ND_WHILE :
	case ND_FUNC :
	case ND_WHILE_PRED :
	    return FALSE;
	default :
            break;
	}

    /* we don't lift these nodes */
    switch (fg->nodes[id].nodetype)
        {
	case ND_CASE :
	case ND_WHILE :
	case ND_FORNXT :
	case ND_FUNC :
	case ND_CROSS_PROD :
	case ND_DOT_PROD :
	case ND_WHILE_PRED :
	case ND_SCALAR_GEN :
	case ND_ELE_GEN :
	case ND_SLICE_GEN:
	case ND_WINDOW_GEN :
	case ND_LOOP_INDICES :
	case ND_REDUCE_SUM :
	case ND_REDUCE_MIN :
	case ND_REDUCE_MAX :
	case ND_REDUCE_AND :
	case ND_REDUCE_OR :
	case ND_REDUCE_VAL_AT_FIRST_MAX :
	case ND_REDUCE_VAL_AT_FIRST_MIN :
	case ND_REDUCE_VAL_AT_LAST_MAX :
	case ND_REDUCE_VAL_AT_LAST_MIN :
	case ND_REDUCE_VAL_AT_MAXS :
	case ND_REDUCE_VAL_AT_MINS :
	case ND_REDUCE_PRODUCT :
	case ND_REDUCE_MEAN :
	case ND_REDUCE_ST_DEV :
	case ND_REDUCE_MODE :
	case ND_REDUCE_MEDIAN :
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
	case ND_REDUCE_SUM_MACRO :
	case ND_REDUCE_PRODUCT_MACRO :
	case ND_REDUCE_AND_MACRO :
	case ND_REDUCE_OR_MACRO :
	case ND_REDUCE_MIN_MACRO :
	case ND_REDUCE_MAX_MACRO :
	case ND_REDUCE_HIST_MACRO :
	case ND_G_INPUT :
	case ND_G_INPUT_NEXT :
	case ND_G_OUTPUT :
	case ND_SWITCH_KEY :
	case ND_SELECTORS :
	case ND_NEXT :
	case ND_PRINT :
	case ND_ASSERT :
	case ND_VOIDED :
	case ND_FEED_NEXT :
	case ND_GRAPH :
    	    return FALSE;
	default :
            break;
	}

    for (i=0; i<fg->nodes[id].num_inputs; i++)
        if (!fg->nodes[id].inputs[i].is_const)
	    {
	    assert (fg->nodes[id].inputs[i].back_edges != NULL);
	    node = fg->nodes[id].inputs[i].back_edges->node;
	    if (fg->nodes[node].nodetype != ND_G_INPUT)
		{
	        return FALSE;
		}
	    }

    return TRUE;
    }
