#include "sassy.h"
#include "ddcf.h"
#include "error.h"

void search_for_switches (FuncGraph*, int);
void dissolve_switch (FuncGraph*, int, int, int);
int equal_values (TypeInfo*, char*, char*);

void switch_const ()
    {
    FuncGraph *fg;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
	    search_for_switches (fg, 0);
    }

void search_for_switches (FuncGraph *fg, int id)
    {
    IntList **pit, *it, *is;
    int nd, key_node, deleted;

    for (pit=&(fg->nodes[id].My_nodes); (*pit)!=NULL; )
        {
	it = *pit;
	nd = it->val;
	deleted = FALSE;

	/* recursively search */
	if (is_compound (fg->nodes[nd].nodetype))
	    search_for_switches (fg, nd);

	if (fg->nodes[nd].nodetype == ND_SWITCH)
	    {
	    /* find the key node */
	    for (is=fg->nodes[nd].My_nodes; is!=NULL; is=is->link)
		{
		key_node = is->val;

	        if (fg->nodes[key_node].nodetype == ND_SWITCH_KEY)
		    break;
		}

	    /* see if the key is a constant */
	    if (fg->nodes[key_node].inputs[0].is_const)
		{
	        dissolve_switch (fg, nd, key_node, id);
		deleted = TRUE;
		}
	    }

	if (!deleted)
	    pit=&((*pit)->link);
	}
    }

/* 'nd' is a ND_SWITCH node */
void dissolve_switch (FuncGraph *fg, int nd, int key_node, int parent)
    {
    IntList *it, *is;
    Edge *tg;
    Edge *bk;
    int case_node, sel_node, default_case, sel_nd, i;
    int g_in, g_out, new_graph;
    char *sel_string;

    /*
    printf ("dissolve_switch\n");
    */

    is_stable = FALSE;

    sel_string = fg->nodes[key_node].inputs[0].constval;

    /*
    printf ("searching the CASE nodes\n");
    */

    for (it=fg->nodes[nd].My_nodes; it!=NULL; it=it->link)
	{
	case_node = it->val;

        if (fg->nodes[case_node].nodetype == ND_CASE)
	    {
	    for (is=fg->nodes[case_node].My_nodes; is!=NULL; is=is->link)
	        {
		sel_node = is->val;

		if (fg->nodes[sel_node].nodetype == ND_SELECTORS)
		    break;
		}

	    if (is == NULL)
	        default_case = case_node;
	    else
	        {
		for (i=0; i<fg->nodes[sel_node].num_inputs; i++)
		    if (equal_values (&(fg->nodes[sel_node].inputs[i].ty),
		    		fg->nodes[sel_node].inputs[i].constval, sel_string))
		        break;

		if (i != fg->nodes[sel_node].num_inputs)
		    break;
		}
	    }
	}

    /*  if 'it' reached NULL, then there was no match in the cases,
     *  so the default case is the one we keep
     */
    if (it == NULL)
        case_node = default_case;
	    
    new_graph = copy_graph (fg, fg, case_node);

    /* look at each SWITCH input port, and see if it feeds 'case_node' */
    for (i=0; i<fg->nodes[nd].num_inputs; i++)
        {
	g_in = fg->nodes[nd].My_inputs[i];

	for (tg=fg->nodes[g_in].outputs[0].targets; tg!=NULL; tg=tg->link)
	    if (tg->node == case_node)
		{
	        add_edge (fg, fg->nodes[nd].inputs[i].back_edges->node,
			fg->nodes[nd].inputs[i].back_edges->port, new_graph, tg->port);
		}

	remove_targets_to_this_port (fg, fg->nodes[nd].inputs[i].back_edges, nd, i);
        }

    /* look at each SWITCH output port, and see if it is fed by 'case_node' */
    for (i=0; i<fg->nodes[nd].num_outputs; i++)
        {
	g_out = fg->nodes[nd].My_outputs[i];

	for (bk=fg->nodes[g_out].inputs[0].back_edges; bk!=NULL; bk=bk->link)
	    if (bk->node == case_node)
	        for (tg=fg->nodes[nd].outputs[i].targets; tg!=NULL; tg=tg->link)
		    {
		    add_edge (fg, new_graph, bk->port, tg->node, tg->port);
		    }

	remove_backedges_to_this_target (fg, fg->nodes[nd].outputs[i].targets, nd, i);
	}

    replace_in_intlist (&(fg->nodes[parent].My_nodes), nd, new_graph);

    /* destroy the switch graph */
    destroy_graph (fg, nd);

    /* find the ND_SELECTORS node in the copied case graph */
    for (it=fg->nodes[new_graph].My_nodes; it!=NULL; it=it->link)
	{
	sel_nd = it->val;
        if (fg->nodes[sel_nd].nodetype == ND_SELECTORS)
	    break;
	}

    /* if a ND_SELECTORS was found, remove it */
    if (it != NULL)
        free_ddcf_node (fg, sel_nd, &(fg->nodes[new_graph].My_nodes));

    lift_graph (fg, new_graph, &(fg->nodes[parent].My_nodes));
    }

int equal_values (TypeInfo *ty, char *s0, char *s1)
    {
    int v0, v1;

    switch (ty->type)
        {
	case Uint :
	case Int :
	    sscanf (s0, "%d", &v0);
	    sscanf (s1, "%d", &v1);
	    return v0==v1;
	case Bool :
	    if (strcmp (s0, s1) == 0)
	        return TRUE;
	    else
	        return FALSE;
	default :
            break;
	}

    return FALSE;
    }
