#include "sassy.h"
#include "ddcf.h"
#include "error.h"

void window_to_elegen_graph (FuncGraph*, int);
void try_to_restore_elegens (FuncGraph*, int, int);

void window_to_elegen ()
    {
    FuncGraph *fg;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
	if (fg->nodes_used > 0)
            window_to_elegen_graph (fg, 0);
    }

void window_to_elegen_graph (FuncGraph *fg, int nd)
    {
    IntList *it;
    int id, gen;

    for (it=fg->nodes[nd].My_nodes; it!=NULL; it=it->link)
        {
	id = it->val;
	if (is_compound (fg->nodes[id].nodetype))
	    window_to_elegen_graph (fg, id);
	}

    if (fg->nodes[nd].nodetype == ND_FORALL)
        {
	gen = find_generator_graph (fg, nd);
	try_to_restore_elegens (fg, nd, gen);
	}
    }

void try_to_restore_elegens (FuncGraph *fg, int loop, int gen_graph)
    {
    IntList *it;
    int i, j, no_convert, gout, io_num, aref, nd, rank;

    for (it=fg->nodes[gen_graph].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;
	if (fg->nodes[nd].nodetype == ND_WINDOW_GEN)
	    {
	    no_convert = FALSE;
	    for (i=1; i<fg->nodes[nd].num_inputs; i+=2)
	        if (!fg->nodes[nd].inputs[i].is_const || (strcmp (fg->nodes[nd].inputs[i].constval, "1") != 0))
		    {
		    no_convert = TRUE;
		    break;
		    }
	    if (no_convert)
	        continue;

	    gout = fg->nodes[nd].outputs[0].targets->node;
	    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
	    io_num = fg->nodes[gout].Io_num;

	    if (fg->nodes[gen_graph].outputs[io_num].targets != NULL)
	        {
	        if (fg->nodes[gen_graph].outputs[io_num].targets->link != NULL)
	            break;

	        aref = fg->nodes[gen_graph].outputs[io_num].targets->node;
	        if (fg->nodes[aref].nodetype != ND_ARRAYREF)
	            break;

	        no_convert = FALSE;
	        for (i=1; i<fg->nodes[aref].num_inputs; i++)
	            if (!fg->nodes[aref].inputs[i].is_const || (strcmp (fg->nodes[aref].inputs[i].constval, "0") != 0))
		        {
		        no_convert = TRUE;
		        break;
		        }

	        if (no_convert)
	            continue;
	        }
	    else
	        aref = -1;

	    /* if we've gotten this far, we can convert this window gen to an elegen */
	    is_stable = FALSE;

	    rank = fg->nodes[nd].inputs[0].ty.dims;
	    fg->nodes[nd].nodetype = ND_ELE_GEN;
	    for (i=2, j=1; i<fg->nodes[nd].num_inputs; i+=2, j++)
	        fg->nodes[nd].inputs[j] = fg->nodes[nd].inputs[i];
	    fg->nodes[nd].num_inputs = j;

	    SacMalloc (fg->nodes[nd].Reftypes, char*, 1 + rank)
	    for (i=0; i<rank; i++)
	        fg->nodes[nd].Reftypes[i] = '~';
	    fg->nodes[nd].Reftypes[i] = '\0';

	    fg->nodes[nd].outputs[0].ty.kind = Scalar;
	    copy_type_info (&(fg->nodes[gout].inputs[0].ty), &(fg->nodes[nd].outputs[0].ty));
	    copy_type_info (&(fg->nodes[gen_graph].outputs[io_num].ty), &(fg->nodes[nd].outputs[0].ty));

	    if (aref >= 0)
	        {
	        fg->nodes[gen_graph].outputs[io_num] = fg->nodes[aref].outputs[0];
	        fg->nodes[aref].outputs[0].targets = NULL;
	        retarget_backedges (fg, fg->nodes[gen_graph].outputs[io_num].targets, aref, 0, gen_graph, io_num);
	        free_ddcf_node (fg, aref, &(fg->nodes[loop].My_nodes));
	        }
	    }
	}
    }
