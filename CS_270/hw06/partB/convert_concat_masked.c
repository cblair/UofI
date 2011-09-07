#include "sassy.h"
#include "error.h"
#include "ddcf.h"
#include <limits.h>
#include <float.h>

void convert_concat_masked_graph (FuncGraph *fg, int id);
void convert_this_concat_masked (FuncGraph *fg, int nd, int snd0, int snd1, IntList **list_copy);

void convert_concat_masked ()
    {
    FuncGraph *fg;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
	    convert_concat_masked_graph (fg, 0);
    }

void convert_concat_masked_graph (FuncGraph *fg, int id)
    {
    IntList *it, *list_copy;
    int nd, snd0, snd1;

    copy_intlist (&list_copy, &(fg->nodes[id].My_nodes));

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
	{
	nd = it->val;

	if (is_compound (fg->nodes[nd].nodetype))
	    convert_concat_masked_graph (fg, nd);
	else if (fg->nodes[nd].nodetype == ND_CONSTRUCT_CONCAT_MASKED)
	    {
	    assert (fg->nodes[nd].inputs[0].back_edges != NULL);
	    snd0 = fg->nodes[nd].inputs[0].back_edges->node;
	    assert (fg->nodes[nd].inputs[1].back_edges != NULL);
	    snd1 = fg->nodes[nd].inputs[1].back_edges->node;

	    if ((fg->nodes[snd0].nodetype == ND_ARR_DEF) &&
				(fg->nodes[snd0].DefRank == 1) &&
	    			(fg->nodes[snd1].nodetype == ND_ARR_DEF))
	        convert_this_concat_masked (fg, nd, snd0, snd1, &list_copy);
	    }
	}

    free_intlist (&(fg->nodes[id].My_nodes));
    fg->nodes[id].My_nodes = list_copy;
    }

/*  'nd' is a CONSTRUCT_CONCAT_MASKED node that is fed by two ARR_DEF nodes, 'snd0'
 *  and 'snd1'; this will convert the CONSTRUCT_CONCAT_MASKED node to a CONSTRUCT_CONCAT_MASKED_MANY
 *  and eliminate the ARR_DEF nodes
 */
void convert_this_concat_masked (FuncGraph *fg, int nd, int snd0, int snd1, IntList **list_copy)
    {
    int i, j, n_inputs, snd, spt;

    /*
    printf ("converting a CONSTRUCT_CONCAT_MASKED node\n");
    */

    is_stable = FALSE;

    fg->nodes[nd].nodetype = ND_CONSTRUCT_CONCAT_MASKED_MANY;
    n_inputs = fg->nodes[snd0].num_inputs;

    /* make sure the two ARR_DEF nodes have same number of inputs */
    if (n_inputs != fg->nodes[snd1].num_inputs)
        handle_error (ErrConcatMaskedArrDefExtents, Deadly, fg->nodes[nd].loc.line,
		fg->nodes[nd].loc.file, fg->nodes[nd].loc.func, n_inputs, fg->nodes[snd1].num_inputs);

    remove_edge (fg, snd0, 0, nd, 0);
    remove_edge (fg, snd1, 0, nd, 1);

    SacFree (fg->nodes[nd].inputs);
    fg->nodes[nd].inputs = make_input_ports (n_inputs * 2);
    fg->nodes[nd].num_inputs = n_inputs * 2;

    for (i=0, j=0; i<n_inputs; i++)
        {
	if (fg->nodes[snd0].inputs[i].is_const)
	    fg->nodes[nd].inputs[j] = fg->nodes[snd0].inputs[i];
	else
	    {
	    assert (fg->nodes[snd0].inputs[i].back_edges != NULL);
	    snd = fg->nodes[snd0].inputs[i].back_edges->node;
	    spt = fg->nodes[snd0].inputs[i].back_edges->port;
	    remove_edge (fg, snd, spt, snd0, i);
	    add_edge (fg, snd, spt, nd, j);
	    copy_type_info (&(fg->nodes[nd].inputs[j].ty), &(fg->nodes[snd].outputs[spt].ty));
	    }

	j++;

	if (fg->nodes[snd1].inputs[i].is_const)
	    fg->nodes[nd].inputs[j] = fg->nodes[snd1].inputs[i];
	else
	    {
	    assert (fg->nodes[snd1].inputs[i].back_edges != NULL);
	    snd = fg->nodes[snd1].inputs[i].back_edges->node;
	    spt = fg->nodes[snd1].inputs[i].back_edges->port;
	    remove_edge (fg, snd, spt, snd1, i);
	    add_edge (fg, snd, spt, nd, j);
	    copy_type_info (&(fg->nodes[nd].inputs[j].ty), &(fg->nodes[snd].outputs[spt].ty));
	    }

	j++;
	}

    free_ddcf_node (fg, snd0, list_copy);
    free_ddcf_node (fg, snd1, list_copy);
    }
