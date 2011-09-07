#include "sassy.h"
#include "error.h"
#include "ddcf.h"

#define HASH_TABLE_SIZE 119

IntList *hash_table[HASH_TABLE_SIZE];

void process_graphs_nodes_cse (FuncGraph*, int);
void search_cse (FuncGraph*, int, IntList**, IntList**);
int cse_identical (FuncGraph*, int, int);
void fill_hash_table (FuncGraph*, IntList*);
int hash_inputs (FuncGraph*, int);
void free_hash_table ();
int node_is_cseable (FuncGraph*, int);

void cse ()
    {
    FuncGraph *fg;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
	    {
            process_graphs_nodes_cse (fg, 0);
	    }
    }

void process_graphs_nodes_cse (FuncGraph *fg, int id)
    {
    IntList *it, *list_copy;
    int i;

    /* first process the simple nodes that can be cse'd */
    copy_intlist (&list_copy, &(fg->nodes[id].My_nodes));
    fill_hash_table (fg, list_copy);

    for (i=0; i<HASH_TABLE_SIZE; i++)
        for (it=hash_table[i]; it!=NULL; it=it->link)
    	    search_cse (fg, it->val, &(it->link), &list_copy);

    free_hash_table ();
    free_intlist (&(fg->nodes[id].My_nodes));
    fg->nodes[id].My_nodes = list_copy;

    /* now recursively descend into any subgraphs */
    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        if (!is_simple_node (fg, it->val))
            process_graphs_nodes_cse (fg, it->val);
    }

void search_cse (FuncGraph *fg, int id, IntList **ppit, IntList **list_copy)
    {
    IntList **pit, *tmp;
    int i, nd;

    for (pit=ppit; (*pit)!=NULL; )
	{
	nd = (*pit)->val;

	if (cse_identical (fg, id, nd))
            {
	    is_stable = FALSE;

	    /*
	    printf ("cse: eliminating node %d\n", nd);
	    */

	    /* eliminate this node */
	    for (i=0; i<fg->nodes[nd].num_inputs; i++)
		if (!fg->nodes[nd].inputs[i].is_const)
		    {
	            remove_targets_to_this_port (fg, fg->nodes[nd].inputs[i].back_edges, nd, i);
		    free_edge_list (&(fg->nodes[nd].inputs[i].back_edges));
		    }

	    for (i=0; i<fg->nodes[nd].num_outputs; i++)
	        {
		retarget_backedges (fg, fg->nodes[nd].outputs[i].targets, nd, i, id, i);
		append_target_list (&(fg->nodes[id].outputs[i].targets), fg->nodes[nd].outputs[i].targets);
		}

	    free_ddcf_node (fg, nd, list_copy);

	    tmp = *pit;
	    *pit = tmp->link;
	    SacFree (tmp)
	    }
	else
	    pit=&((*pit)->link);
	}
    }

int cse_identical (FuncGraph *fg, int id, int nd)
    {
    int i, n0, p0, n1, p1;

    if (fg->nodes[id].nodetype != fg->nodes[nd].nodetype)
        return FALSE;

    if (fg->nodes[id].nodetype == ND_ARRAYREF)
	{
        for (i=0; i<fg->nodes[id].inputs[0].ty.dims; i++)
	    if (fg->nodes[id].Reftypes[i] != fg->nodes[nd].Reftypes[i])
	        return FALSE;
	}
    else if (fg->nodes[id].nodetype == ND_ARR_DEF)
        {
	for (i=0; i<fg->nodes[id].DefRank; i++)
	    if (fg->nodes[id].DefDims[i] != fg->nodes[nd].DefDims[i])
	        return FALSE;
	}

    if ((fg->nodes[id].num_inputs != fg->nodes[nd].num_inputs) ||
    			(fg->nodes[id].num_outputs != fg->nodes[nd].num_outputs))
	return FALSE;

    for (i=0; i<fg->nodes[id].num_outputs; i++)
        if (! type_equal (&(fg->nodes[id].outputs[i].ty), &(fg->nodes[nd].outputs[i].ty)))
	    return FALSE;

    for (i=0; i<fg->nodes[id].num_inputs; i++)
        {
	if (fg->nodes[id].inputs[i].is_const != fg->nodes[nd].inputs[i].is_const)
	    return FALSE;

	if (fg->nodes[id].inputs[i].is_const)
	    {
	    if (fg->nodes[id].inputs[i].ty.type != fg->nodes[nd].inputs[i].ty.type)
	        return FALSE;

	    if (fg->nodes[id].inputs[i].ty.fracsize != fg->nodes[nd].inputs[i].ty.fracsize)
	        return FALSE;

	    if (strcmp (fg->nodes[id].inputs[i].constval, fg->nodes[nd].inputs[i].constval) != 0)
	        return FALSE;
	    }
	else
	    {
	    if (! type_equal (&(fg->nodes[id].inputs[i].ty), &(fg->nodes[nd].inputs[i].ty)))
	        return FALSE;

	    assert (fg->nodes[id].inputs[i].back_edges != NULL);
	    assert (fg->nodes[nd].inputs[i].back_edges != NULL);

	    n0 = fg->nodes[id].inputs[i].back_edges->node;
	    p0 = fg->nodes[id].inputs[i].back_edges->port;
	    n1 = fg->nodes[nd].inputs[i].back_edges->node;
	    p1 = fg->nodes[nd].inputs[i].back_edges->port;

	    if ((n0 != n1) || (p0 != p1))
	        return FALSE;
	    }
	}

    return TRUE;
    }

int hash_inputs (FuncGraph *fg, int nd)
    {
    int ac, acx, i;
    char *cp;

    for (i=0, ac=0; i<fg->nodes[nd].num_inputs; i++)
        {
	if (fg->nodes[nd].inputs[i].is_const)
	    for (acx=0, cp=fg->nodes[nd].inputs[i].constval; (*cp)!='\0'; cp++)
	        acx += *cp;
	else
	    acx = 501 * fg->nodes[nd].inputs[i].back_edges->node +
	          211 * fg->nodes[nd].inputs[i].back_edges->port;

	ac += acx * (i+1);
	}

    ac += (fg->nodes[nd].nodetype + 1) * 899;

    return ac%HASH_TABLE_SIZE;
    }

void fill_hash_table (FuncGraph *fg, IntList *itlist)
    {
    int i, hval;
    IntList *it;

    for (i=0; i<HASH_TABLE_SIZE; i++)
        hash_table[i] = NULL;

    for (it=itlist; it!=NULL; it=it->link)
	if (node_is_cseable (fg, it->val))
            {
	    hval = hash_inputs (fg, it->val);
	    add_to_list (it->val, &(hash_table[hval]));
	    }

    for (i=0; i<HASH_TABLE_SIZE; i++)
        reverse_intlist (&(hash_table[i]));
    }

void free_hash_table ()
    {
    int i;

    for (i=0; i<HASH_TABLE_SIZE; i++)
        free_intlist (&(hash_table[i]));
    }

int node_is_cseable (FuncGraph *fg, int nd)
    {
    if (is_simple_node (fg, nd))
	switch (fg->nodes[nd].nodetype)
	    {
	    case  ND_SCALAR_GEN :
	    case  ND_ELE_GEN :
	    case  ND_SLICE_GEN :
	    case  ND_WINDOW_GEN :
	    case  ND_LOOP_INDICES :
	    case  ND_REDUCE_SUM :
	    case  ND_REDUCE_MIN :
	    case  ND_REDUCE_MAX :
	    case  ND_REDUCE_AND :
	    case  ND_REDUCE_OR :
	    case  ND_REDUCE_VAL_AT_FIRST_MAX :
	    case  ND_REDUCE_VAL_AT_FIRST_MIN :
	    case  ND_REDUCE_VAL_AT_LAST_MAX :
	    case  ND_REDUCE_VAL_AT_LAST_MIN :
	    case  ND_REDUCE_VAL_AT_MAXS :
	    case  ND_REDUCE_VAL_AT_MINS :
	    case  ND_REDUCE_PRODUCT :
	    case  ND_REDUCE_MEAN :
	    case  ND_REDUCE_ST_DEV :
	    case  ND_REDUCE_MODE :
	    case  ND_REDUCE_MEDIAN :
	    case  ND_REDUCE_HIST :
	    case  ND_CONSTRUCT_ARRAY :
	    case  ND_CONSTRUCT_CONCAT :
	    case  ND_CONSTRUCT_CONCAT_MASKED :
	    case  ND_CONSTRUCT_TILE :
	    case  ND_ACCUM_SUM :
	    case  ND_ACCUM_MIN :
	    case  ND_ACCUM_MAX :
	    case  ND_ACCUM_AND :
	    case  ND_ACCUM_OR :
	    case  ND_ACCUM_PRODUCT :
	    case  ND_ACCUM_MEAN :
	    case  ND_ACCUM_ST_DEV :
	    case  ND_ACCUM_MEDIAN :
	    case  ND_ACCUM_HIST :
	    case  ND_G_INPUT :
	    case  ND_G_INPUT_NEXT :
	    case  ND_G_OUTPUT :
	    case  ND_FCALL :
	    case  ND_INTRINCALL :
	    case  ND_SWITCH_KEY :
	    case  ND_SELECTORS :
	    case  ND_NEXT :
	    case  ND_PRINT :
	    case  ND_ASSERT :
	    case  ND_FEED_NEXT :
	        return FALSE;
	    default :
	        return TRUE;
	    }

    return FALSE;
    }
