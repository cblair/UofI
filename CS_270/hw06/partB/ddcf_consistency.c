#include "sassy.h"
#include "error.h"
#include "ddcf.h"

void node_check (FuncGraph*, int, char*);
void check_back_edges (FuncGraph*, int, int, char*);
void check_targets (FuncGraph*, int, int, char*);

int consistency_error;

void ddcf_consistency ()
    {
    FuncGraph *fg;
    IntList *it;

    consistency_error = FALSE;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
	    for (it=fg->nodes[0].My_nodes; it!=NULL; it=it->link)
	        node_check (fg, it->val, fg->name);

    if (consistency_error)
        exit (1);
    }

void node_check (FuncGraph *fg, int nd, char *name)
    {
    int i, io_node;
    IntList *it;

    /*
    printf ("checking node %d\n", nd);
    */

    for (i=0; i<fg->nodes[nd].num_inputs; i++)
        if ((fg->nodes[nd].inputs[i].is_const) && (fg->nodes[nd].inputs[i].back_edges != NULL))
	    {
	    handle_error (ErrConsistConstAndEdge, Fatal, fg->nodes[nd].loc.line,
	    		fg->nodes[nd].loc.file, fg->nodes[nd].loc.func, nd,
			ddcf_node_str (fg->nodes[nd].nodetype), i);
	    consistency_error = TRUE;
	    }
	else if (!fg->nodes[nd].inputs[i].is_const)
	    {
	    if (fg->nodes[nd].inputs[i].back_edges == NULL)
		{
		if (nd != 0)
		    {
	    	    handle_error (ErrConsistNoConstOrEdge, Fatal, fg->nodes[nd].loc.line,
	    		fg->nodes[nd].loc.file, fg->nodes[nd].loc.func, nd,
			ddcf_node_str (fg->nodes[nd].nodetype), i);
	            consistency_error = TRUE;
		    }
	        }
	    else
	        {
		if ((fg->nodes[nd].inputs[i].back_edges->link != NULL) && (fg->nodes[nd].nodetype != ND_G_OUTPUT))
		    {
	    	    handle_error (ErrConsistMultBackEdges, Fatal, fg->nodes[nd].loc.line,
	    		fg->nodes[nd].loc.file, fg->nodes[nd].loc.func, nd,
			ddcf_node_str (fg->nodes[nd].nodetype), i);
	            consistency_error = TRUE;
	            }
		check_back_edges (fg, nd, i, name);
		}
	    }

    for (i=0; i<fg->nodes[nd].num_outputs; i++)
        check_targets (fg, nd, i, name);

    if (is_compound (fg->nodes[nd].nodetype))
        {
	for (i=0; i<fg->nodes[nd].num_inputs; i++)
	    {
	    io_node = fg->nodes[nd].My_inputs[i];
	    if ((fg->nodes[io_node].nodetype!=ND_G_INPUT) && (fg->nodes[io_node].nodetype!=ND_G_INPUT_NEXT))
	        {
	    	handle_error (ErrConsistGraphInputWrongType, Fatal, fg->nodes[io_node].loc.line,
	    		fg->nodes[io_node].loc.file, fg->nodes[io_node].loc.func, nd,
			ddcf_node_str (fg->nodes[io_node].nodetype), i);
		consistency_error = TRUE;
		}
	    else if ((fg->nodes[io_node].Io_num != i) || (fg->nodes[io_node].My_graph != nd))
	        {
	    	handle_error (ErrConsistBadOwner, Fatal, fg->nodes[io_node].loc.line,
	    		fg->nodes[io_node].loc.file, fg->nodes[io_node].loc.func,
			io_node, ddcf_node_str (fg->nodes[io_node].nodetype));
		consistency_error = TRUE;
		}
	    }

	for (i=0; i<fg->nodes[nd].num_outputs; i++)
	    {
	    io_node = fg->nodes[nd].My_outputs[i];
	    if (fg->nodes[io_node].nodetype!=ND_G_OUTPUT)
	        {
	    	handle_error (ErrConsisiGraphOutputWrongType, Fatal, fg->nodes[io_node].loc.line,
	    		fg->nodes[io_node].loc.file, fg->nodes[io_node].loc.func, nd,
			ddcf_node_str (fg->nodes[io_node].nodetype), i);
		consistency_error = TRUE;
		}
	    else if ((fg->nodes[io_node].Io_num != i) || (fg->nodes[io_node].My_graph != nd))
	        {
	    	handle_error (ErrConsistBadOwner, Fatal, fg->nodes[io_node].loc.line,
	    		fg->nodes[io_node].loc.file, fg->nodes[io_node].loc.func,
			io_node, ddcf_node_str (fg->nodes[io_node].nodetype));
		consistency_error = TRUE;
		}
	    }

	for (it=fg->nodes[nd].My_nodes; it!=NULL; it=it->link)
	    node_check (fg, it->val, name);
	}
    }

void check_back_edges (FuncGraph *fg, int nd, int pt, char *name)
    {
    Edge *bk, *tg;
    int snd, spt;

    for (bk=fg->nodes[nd].inputs[pt].back_edges; bk!=NULL; bk=bk->link)
        {
	snd = bk->node;
	spt = bk->port;

	if (fg->nodes[snd].nodetype == ND_VOIDED)
	    {
	    handle_error (ErrConsistEdgeToVoid, Fatal, fg->nodes[nd].loc.line,
	    		fg->nodes[nd].loc.file, fg->nodes[nd].loc.func,
			ddcf_node_str (fg->nodes[nd].nodetype), nd, pt, snd);
	    consistency_error = TRUE;
	    }
	else
	    {
	    if (spt >= fg->nodes[snd].num_outputs)
	        {
	        handle_error (ErrConsistEdgeToNonexistentPort, Fatal, fg->nodes[nd].loc.line,
	    		fg->nodes[nd].loc.file, fg->nodes[nd].loc.func,
			nd, pt, snd, spt);
	        consistency_error = TRUE;
	        }
	    else
	        {
	        for (tg=fg->nodes[snd].outputs[spt].targets; tg!=NULL; tg=tg->link)
	            if ((tg->node==nd) && (tg->port==pt))
	                break;

	        if (tg == NULL)
		    {
	            handle_error (ErrConsistNoTargetForBackedge, Fatal, fg->nodes[nd].loc.line,
	    		fg->nodes[nd].loc.file, fg->nodes[nd].loc.func,
			nd, pt, snd, spt);
	            consistency_error = TRUE;
	            }
		}
	    }
	}
    }

void check_targets (FuncGraph *fg, int nd, int pt, char *name)
    {
    Edge *tg, *bk;
    int snd, spt;

    for (tg=fg->nodes[nd].outputs[pt].targets; tg!=NULL; tg=tg->link)
        {
	snd = tg->node;
	spt = tg->port;

	if (fg->nodes[snd].nodetype == ND_VOIDED)
	    {
	    handle_error (ErrConsistTargetToVoid, Fatal, fg->nodes[nd].loc.line,
	    		fg->nodes[nd].loc.file, fg->nodes[nd].loc.func,
			nd, pt, snd);
	    consistency_error = TRUE;
	    }
	else
	    {
	    if (spt >= fg->nodes[snd].num_inputs)
	        {
	        handle_error (ErrConsistTargetToNonexistentInput, Fatal, fg->nodes[nd].loc.line,
	    		fg->nodes[nd].loc.file, fg->nodes[nd].loc.func,
			nd, pt, snd, spt);
	        consistency_error = TRUE;
	        }
	    else
	        {
	        for (bk=fg->nodes[snd].inputs[spt].back_edges; bk!=NULL; bk=bk->link)
	            if ((bk->node==nd) && (bk->port==pt))
	                break;

	        if (bk == NULL)
		    {
	            handle_error (ErrConsistNoEdgeMatchTarget, Fatal, fg->nodes[nd].loc.line,
	    		fg->nodes[nd].loc.file, fg->nodes[nd].loc.func,
			nd, pt, snd, spt);
	            consistency_error = TRUE;
	            }
	        }
	    }
	}
    }
