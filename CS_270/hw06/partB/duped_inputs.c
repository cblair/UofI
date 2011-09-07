#include "sassy.h"
#include "error.h"
#include "ddcf.h"

int count_targets_to_this_node (FuncGraph*, int, int, int);
void reroute_through_other_port (FuncGraph*, Edge**, int, int, int, int, int);
void remove_duped_inputs (FuncGraph*, int);
void process_graphs_nodes_duped_inputs (FuncGraph*, int);
void dead_code_graph (FuncGraph*, int);

void duped_inputs ()
    {
    FuncGraph *fg;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
	    {
            process_graphs_nodes_duped_inputs (fg, 0);
	    dead_code_graph (fg, 0);
	    }
    }

void process_graphs_nodes_duped_inputs (FuncGraph *fg, int id)
    {
    IntList *it;

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        if (!is_simple_node (fg, it->val))
            process_graphs_nodes_duped_inputs (fg, it->val);

    if (fg->nodes[id].nodetype == ND_FORALL)
        remove_duped_inputs (fg, id);
    }

void remove_duped_inputs (FuncGraph *fg, int id)
    {
    Edge **ped;
    int i, snd, spt, cnt, ct, first_input_port;

    for (i=0; i<fg->nodes[id].num_inputs; i++)
        {
	assert (fg->nodes[id].inputs[i].is_const == FALSE);

	if (fg->nodes[id].inputs[i].back_edges != NULL)
	    {
	    snd = fg->nodes[id].inputs[i].back_edges->node;
	    spt = fg->nodes[id].inputs[i].back_edges->port;

	    cnt = count_targets_to_this_node (fg, snd, spt, id);

	    if (cnt > 1)
	        {
	        is_stable = FALSE;

	        for (ct=0, ped=&(fg->nodes[snd].outputs[spt].targets); (*ped)!=NULL; )
	            if ((*ped)->node == id)
		        {
		        ct++;

		        if (ct == 1)
			    {
		            first_input_port = (*ped)->port;
			    ped = &((*ped)->link);
			    }
		        else
			    reroute_through_other_port (fg, ped, first_input_port, (*ped)->port, id, snd, spt);
		        }
		    else
		        ped = &((*ped)->link);
		}
	    }
        }
    }

void reroute_through_other_port (FuncGraph *fg, Edge **ped,
			int first_input_port, int cur_idx, int id, int snd, int spt)
    {
    Edge *ed;
    int first_gin, cur_gin;

    /* remove the edge to the redundant input */
    ed = *ped;
    *ped = ed->link;
    ed->link = NULL;

    remove_backedges_to_this_target (fg, ed, snd, spt);

    SacFree (ed);

    /* reroute inside the loop */
    first_gin = fg->nodes[id].My_inputs[first_input_port];
    cur_gin = fg->nodes[id].My_inputs[cur_idx];

    retarget_backedges (fg, fg->nodes[cur_gin].outputs[0].targets, cur_gin, 0, first_gin, 0);
    append_target_list (&(fg->nodes[first_gin].outputs[0].targets), fg->nodes[cur_gin].outputs[0].targets);
    fg->nodes[cur_gin].outputs[0].targets = NULL;
    }

int count_targets_to_this_node (FuncGraph *fg, int snd, int spt, int id)
   {
   Edge *ed;
   int cnt;

   for (cnt=0, ed=fg->nodes[snd].outputs[spt].targets; ed!=NULL; ed=ed->link)
       if (ed->node == id)
           cnt++;

    return cnt;
    }
