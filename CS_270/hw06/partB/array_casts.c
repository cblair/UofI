#include <assert.h>
#include "sassy.h"
#include "ddcf.h"
#include "error.h"

void push_array_cast_graph (FuncGraph*, int);
int push_cast (FuncGraph*, int, int, int);
void push_cast_past_arrayrefs (FuncGraph*, int, IntList**);

void push_array_cast ()
    {
    FuncGraph *fg;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
	    push_array_cast_graph (fg, 0);
    }

void push_array_cast_graph (FuncGraph *fg, int id)
    {
    IntList *it, **pit, *list_copy;
    Edge *tg;
    int deleted, nd, gen_graph, cnd, gnd, tnd, can_be_pushed;

    copy_intlist (&list_copy, &(fg->nodes[id].My_nodes));

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;

	if (!is_simple_node (fg, nd))
	    push_array_cast_graph (fg, nd);

	if ((fg->nodes[nd].nodetype==ND_FORALL) || (fg->nodes[nd].nodetype==ND_FORNXT))
	    {
	    gen_graph = find_generator_graph (fg, nd);

	    for (pit=&(fg->nodes[gen_graph].My_nodes); (*pit)!=NULL; )
	        {
		deleted = FALSE;

		cnd = (*pit)->val;

		if ((fg->nodes[cnd].nodetype==ND_CAST) && (fg->nodes[cnd].outputs[0].targets!=NULL) &&
						(fg->nodes[cnd].outputs[0].targets->link==NULL))
		    {
		    gnd = fg->nodes[cnd].outputs[0].targets->node;

		    if ((fg->nodes[gnd].nodetype==ND_ELE_GEN) || 
			(fg->nodes[gnd].nodetype==ND_WINDOW_GEN) ||
		      (fg->nodes[gnd].nodetype==ND_SLICE_GEN))
		        deleted = push_cast (fg, nd, gen_graph, cnd);
		    }

		if (!deleted)
		    pit = &((*pit)->link);
		}
	    }
	else if (fg->nodes[nd].nodetype==ND_CAST)
	    {
	    if (fg->nodes[nd].outputs[0].ty.kind == Array)
	        {
		can_be_pushed = TRUE;

		for (tg=fg->nodes[nd].outputs[0].targets; tg!=NULL; tg=tg->link)
		    {
		    tnd = tg->node;
		    if (fg->nodes[tnd].nodetype != ND_ARRAYREF)
		        {
			can_be_pushed = FALSE;
			break;
			}
		    }

		if (can_be_pushed)
		    push_cast_past_arrayrefs (fg, nd, &list_copy);
		}
	    }
	}

    free_intlist (&(fg->nodes[id].My_nodes));
    fg->nodes[id].My_nodes = list_copy;
    }

void push_cast_past_arrayrefs (FuncGraph *fg, int id, IntList **itlist)
    {
    int snd, spt, tnd, tpt, wnd, wpt, cast_node;
    TypeInfo ty;
    Edge *tg, *out_targets;

    is_stable = FALSE;

    assert (fg->nodes[id].inputs[0].back_edges != NULL);
    snd = fg->nodes[id].inputs[0].back_edges->node;
    spt = fg->nodes[id].inputs[0].back_edges->port;

    copy_type_info (&ty, &(fg->nodes[id].outputs[0].ty));

    for (tg=fg->nodes[id].outputs[0].targets; tg!=NULL; tg=tg->link)
	{
	tnd = tg->node;
	tpt = tg->port;
        copy_type_info (&(fg->nodes[tnd].inputs[tpt].ty), &ty);
	}

    retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, snd, spt);
    out_targets = fg->nodes[id].outputs[0].targets;
    fg->nodes[id].outputs[0].targets = NULL;
    remove_targets_to_this_port (fg, fg->nodes[id].inputs[0].back_edges, id, 0);
    free_edge_list (&(fg->nodes[id].inputs[0].back_edges));
    free_ddcf_node (fg, id, itlist);
    
    /* now install casts after the ND_ARRAYREFs */
    for (tg=out_targets; tg!=NULL; tg=tg->link)
        {
	wnd = tg->node;
	wpt = tg->port;

	copy_type_info (&(fg->nodes[wnd].inputs[0].ty), &(fg->nodes[snd].outputs[spt].ty));

	cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[wnd].loc);
	fg->nodes[cast_node].outputs[0].targets = fg->nodes[wnd].outputs[wpt].targets;
	fg->nodes[wnd].outputs[wpt].targets = NULL;
	retarget_backedges (fg, fg->nodes[cast_node].outputs[0].targets, wnd, wpt, cast_node, 0);
	add_edge (fg, wnd, wpt, cast_node, 0);

	copy_type_info (&ty, &(fg->nodes[wnd].outputs[0].ty));

	fg->nodes[wnd].outputs[0].ty.type = fg->nodes[wnd].inputs[0].ty.type;
	fg->nodes[wnd].outputs[0].ty.totsize = fg->nodes[wnd].inputs[0].ty.totsize;
	fg->nodes[wnd].outputs[0].ty.fracsize = fg->nodes[wnd].inputs[0].ty.fracsize;

	copy_type_info (&(fg->nodes[cast_node].inputs[0].ty), &(fg->nodes[wnd].outputs[0].ty));
	copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), &ty);

	insert_val_after_in_intlist (cast_node, wnd, itlist);
	}

    append_target_list (&(fg->nodes[snd].outputs[spt].targets), out_targets);
    }

int push_cast (FuncGraph* fg, int loop_nd, int gen_nd, int cast_node) {
    TypeInfo in_type, out_type;
    int gnd, gpt, snd, spt, g_out, outport, new_id;

    is_stable = FALSE;

    in_type = fg->nodes[cast_node].inputs[0].ty;
    out_type = fg->nodes[cast_node].outputs[0].ty;

    gnd = fg->nodes[cast_node].outputs[0].targets->node;
    gpt = fg->nodes[cast_node].outputs[0].targets->port;
    snd = fg->nodes[cast_node].inputs[0].back_edges->node;
    spt = fg->nodes[cast_node].inputs[0].back_edges->port;

    /* delete the cast node */
    retarget (fg, fg->nodes[cast_node].inputs[0].back_edges, cast_node, 0, gnd, gpt);
    retarget_backedges (fg, fg->nodes[cast_node].outputs[0].targets, cast_node, 0, snd, spt);
    free_edge_list (&(fg->nodes[cast_node].inputs[0].back_edges));
    free_edge_list (&(fg->nodes[cast_node].outputs[0].targets));
    free_ddcf_node (fg, cast_node, &(fg->nodes[gen_nd].My_nodes));

    /* push type down */
    fg->nodes[gnd].inputs[gpt].ty = in_type;
    fg->nodes[gnd].outputs[0].ty.type = in_type.type;
    fg->nodes[gnd].outputs[0].ty.totsize = in_type.totsize;
    fg->nodes[gnd].outputs[0].ty.fracsize = in_type.fracsize;

    g_out = fg->nodes[gnd].outputs[0].targets->node;

    assert (fg->nodes[g_out].nodetype == ND_G_OUTPUT);

    fg->nodes[g_out].inputs[0].ty.type = in_type.type;
    fg->nodes[g_out].inputs[0].ty.totsize = in_type.totsize;
    fg->nodes[g_out].inputs[0].ty.fracsize = in_type.fracsize;

    outport = fg->nodes[g_out].Io_num;
    fg->nodes[gen_nd].outputs[outport].ty.type = in_type.type;
    fg->nodes[gen_nd].outputs[outport].ty.totsize = in_type.totsize;
    fg->nodes[gen_nd].outputs[outport].ty.fracsize = in_type.fracsize;

    new_id = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[gen_nd].loc);
    copy_type_info (&(fg->nodes[new_id].inputs[0].ty), &(fg->nodes[gen_nd].outputs[outport].ty));
    copy_type_info (&(fg->nodes[new_id].outputs[0].ty), &(fg->nodes[gen_nd].outputs[outport].ty));
    fg->nodes[new_id].outputs[0].ty.type = out_type.type;
    fg->nodes[new_id].outputs[0].ty.totsize = out_type.totsize;
    fg->nodes[new_id].outputs[0].ty.fracsize = out_type.fracsize;

    fg->nodes[new_id].outputs[0].targets = fg->nodes[gen_nd].outputs[outport].targets;
    retarget_backedges (fg, fg->nodes[new_id].outputs[0].targets, gen_nd, outport, new_id, 0);

    fg->nodes[gen_nd].outputs[outport].targets = NULL;
    add_edge (fg, gen_nd, outport, new_id, 0);

    insert_val_after_in_intlist (new_id, gen_nd, &(fg->nodes[loop_nd].My_nodes));

    return TRUE;
    }
