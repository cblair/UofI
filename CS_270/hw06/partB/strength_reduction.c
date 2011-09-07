#include "sassy.h"
#include "ddcf.h"
#include "error.h"

void strength_reduction_process_graphs_nodes (FuncGraph*, int);
void strength_reduction_simple_node (FuncGraph*, int, IntList**);
void strength_reduction_mul (FuncGraph*, int, IntList**);
void strength_reduction_div (FuncGraph*, int, IntList**);
void convert_to_shift (FuncGraph*, int, IntList**, DdcfType);
int is_pow2_const (unsigned int);
int is_signed (Type ty);

void strength_reduction ()
    {
    FuncGraph *fg;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
            strength_reduction_process_graphs_nodes (fg, 0);
    }

void strength_reduction_process_graphs_nodes (FuncGraph *fg, int id)
    {
    IntList *it, *list_copy;
    int nd;

    copy_intlist (&list_copy, &(fg->nodes[id].My_nodes));

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;

	if (!is_simple_node (fg, nd))
	    strength_reduction_process_graphs_nodes (fg, nd);
	else
            strength_reduction_simple_node (fg, nd, &list_copy);
	}

    free_intlist (&(fg->nodes[id].My_nodes));
    fg->nodes[id].My_nodes = list_copy;
    }

void strength_reduction_simple_node (FuncGraph *fg, int id, IntList **list_copy)
    {
    switch (fg->nodes[id].nodetype)
        {
	case ND_MUL :
	    strength_reduction_mul (fg, id, list_copy);
	    break;
	case ND_DIV :
	    strength_reduction_div (fg, id, list_copy);
	    break;
	default :
            break;
	}
    }

void strength_reduction_mul (FuncGraph *fg, int nd, IntList **list_copy)
    {
    int snd, spt;
    InputPort tmp_pt;

    if (fg->nodes[nd].inputs[1].is_const)
	{
        if ((fg->nodes[nd].inputs[0].ty.type!=Uint) && (fg->nodes[nd].inputs[0].ty.type!=Ufix) &&
    			(fg->nodes[nd].inputs[0].ty.type!=Int) && (fg->nodes[nd].inputs[0].ty.type!=Fix))
            return;

        if ((fg->nodes[nd].inputs[1].ty.type!=Uint) && (fg->nodes[nd].inputs[1].ty.type!=Int))
            return;

        convert_to_shift (fg, nd, list_copy, ND_LEFT_SHIFT_MACH);
	}
    else if (fg->nodes[nd].inputs[0].is_const)
	{
        if ((fg->nodes[nd].inputs[1].ty.type!=Uint) && (fg->nodes[nd].inputs[1].ty.type!=Ufix) &&
    			(fg->nodes[nd].inputs[1].ty.type!=Int) && (fg->nodes[nd].inputs[1].ty.type!=Fix))
            return;

        if ((fg->nodes[nd].inputs[0].ty.type!=Uint) && (fg->nodes[nd].inputs[0].ty.type!=Int))
            return;

	/* swap the two inputs to set them for the convert routine */
	tmp_pt = fg->nodes[nd].inputs[0];
	fg->nodes[nd].inputs[0] = fg->nodes[nd].inputs[1];
	fg->nodes[nd].inputs[1] = tmp_pt;

	if (!fg->nodes[nd].inputs[0].is_const)
	    {
	    snd = fg->nodes[nd].inputs[0].back_edges->node;
	    spt = fg->nodes[nd].inputs[0].back_edges->port;
	    retarget (fg, fg->nodes[nd].inputs[0].back_edges, nd, 1, nd, 0);
	    }

        convert_to_shift (fg, nd, list_copy, ND_LEFT_SHIFT_MACH);
	}
    }

void strength_reduction_div (FuncGraph *fg, int nd, IntList **list_copy)
    {

    if (!fg->nodes[nd].inputs[1].is_const)
        return;

    if ((fg->nodes[nd].inputs[0].ty.type!=Uint) && (fg->nodes[nd].inputs[0].ty.type!=Ufix))
        return;

    if ((fg->nodes[nd].inputs[1].ty.type!=Uint) && (fg->nodes[nd].inputs[1].ty.type!=Int))
        return;

    convert_to_shift (fg, nd, list_copy, ND_RIGHT_SHIFT);
    }

void convert_to_shift (FuncGraph *fg, int nd, IntList **list_copy, DdcfType nty)
    {
    int ival, cast_node, neg_node, snd, spt, tnd, tpt;
    int mul_negative = FALSE;
    int width, sgn, num_w0, num_w1;
    unsigned int uval, cnt;
    char str[128];

    /* determine the width of the node's result */
    sgn = 0;
    if (is_signed (fg->nodes[nd].inputs[0].ty.type))
	{
	sgn = 1;
	num_w0 = fg->nodes[nd].inputs[0].ty.totsize - 1;
	}
    else
	num_w0 = fg->nodes[nd].inputs[0].ty.totsize;

    if (is_signed (fg->nodes[nd].inputs[1].ty.type))
	{
	sgn = 1;
	num_w1 = fg->nodes[nd].inputs[1].ty.totsize - 1;
	}
    else
	num_w1 = fg->nodes[nd].inputs[1].ty.totsize;

    width = sgn + ((num_w0>num_w1) ? num_w0 : num_w1);

    if (fg->nodes[nd].inputs[1].ty.type==Int)
        {
	sscanf (fg->nodes[nd].inputs[1].constval, "%d", &ival);

	if (ival < 0)
	    {
	    mul_negative = TRUE;
	    ival = -ival;
	    }

	uval = ival;
	}
    else
        sscanf (fg->nodes[nd].inputs[1].constval, "%u", &uval);

    if (is_pow2_const (uval))
        {
	is_stable = FALSE;

	/* if the multiplier was negative, insert a NEG node */
	if (mul_negative)
	    {
	    neg_node = new_ddcf_node (fg, ND_NEG, 1, 1, fg->nodes[nd].loc);
	    fg->nodes[neg_node].inputs[0] = fg->nodes[nd].inputs[0];
	    if (!fg->nodes[neg_node].inputs[0].is_const)
	        {
	        snd = fg->nodes[neg_node].inputs[0].back_edges->node;
	        spt = fg->nodes[neg_node].inputs[0].back_edges->port;
	        retarget (fg, fg->nodes[neg_node].inputs[0].back_edges, nd, 0, neg_node, 0);
	        fg->nodes[nd].inputs[0].back_edges = NULL;
	        }
	    add_edge (fg, neg_node, 0, nd, 0);
	    copy_type_info (&(fg->nodes[neg_node].outputs[0].ty), &(fg->nodes[nd].inputs[0].ty));
	    if (fg->nodes[neg_node].outputs[0].ty.type == Uint)
	        {
		fg->nodes[neg_node].outputs[0].ty.type = Int;
		fg->nodes[neg_node].outputs[0].ty.totsize++;
		fg->nodes[nd].inputs[0].ty.type = Int;
		fg->nodes[nd].inputs[0].ty.totsize++;
		}
	    insert_val_before_in_intlist (neg_node, nd, list_copy);
	    }

	fg->nodes[nd].nodetype = nty;

	/* determine shift dist */
	for (cnt=0; uval>1; uval=uval>>1, cnt++);
	sprintf (str, "%u", cnt);
	fg->nodes[nd].inputs[1].constval = copy_to_const (str);
	set_type_to_uint (&(fg->nodes[nd].inputs[1].ty), 32);

	/* cast the incoming value to a 'bits' type; if it's an Int or Fix, this
	 * should be done in two steps: first cast it to an Int or Fix with a
	 * 'totsize' equal to 'width'; second, cast it to Bits
	 */
	if (is_signed (fg->nodes[nd].inputs[0].ty.type))
	    {
	    cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[nd].loc);
	    fg->nodes[cast_node].inputs[0] = fg->nodes[nd].inputs[0];
	    if (!fg->nodes[cast_node].inputs[0].is_const)
	        {
	        snd = fg->nodes[cast_node].inputs[0].back_edges->node;
	        spt = fg->nodes[cast_node].inputs[0].back_edges->port;
	        retarget (fg, fg->nodes[cast_node].inputs[0].back_edges, nd, 0, cast_node, 0);
	        fg->nodes[nd].inputs[0].back_edges = NULL;
	        }
	    add_edge (fg, cast_node, 0, nd, 0);
	    fg->nodes[nd].inputs[0].ty.totsize = width;
	    copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), &(fg->nodes[nd].inputs[0].ty));
	    insert_val_before_in_intlist (cast_node, nd, list_copy);
	    }

	cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[nd].loc);
	fg->nodes[cast_node].inputs[0] = fg->nodes[nd].inputs[0];
	if (!fg->nodes[cast_node].inputs[0].is_const)
	    {
	    snd = fg->nodes[cast_node].inputs[0].back_edges->node;
	    spt = fg->nodes[cast_node].inputs[0].back_edges->port;
	    retarget (fg, fg->nodes[cast_node].inputs[0].back_edges, nd, 0, cast_node, 0);
	    fg->nodes[nd].inputs[0].back_edges = NULL;
	    }
	add_edge (fg, cast_node, 0, nd, 0);
	fg->nodes[nd].inputs[0].ty.type = Bits;
	fg->nodes[nd].inputs[0].ty.fracsize = 0;
	fg->nodes[nd].inputs[0].ty.totsize = width;

	copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), &(fg->nodes[nd].inputs[0].ty));
	insert_val_before_in_intlist (cast_node, nd, list_copy);

	/* cast the outgoing value back to the original type */
	cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[nd].loc);
	fg->nodes[cast_node].outputs[0] = fg->nodes[nd].outputs[0];
	tnd = fg->nodes[cast_node].outputs[0].targets->node;
	tpt = fg->nodes[cast_node].outputs[0].targets->port;
	retarget_backedges (fg, fg->nodes[cast_node].outputs[0].targets, nd, 0, cast_node, 0);
	fg->nodes[nd].outputs[0].targets = NULL;
	add_edge (fg, nd, 0, cast_node, 0);
	fg->nodes[nd].outputs[0].ty.type = Bits;
	fg->nodes[nd].outputs[0].ty.fracsize = 0;
	copy_type_info (&(fg->nodes[cast_node].inputs[0].ty), &(fg->nodes[nd].outputs[0].ty));
	insert_val_after_in_intlist (cast_node, nd, list_copy);
	}
    }

int is_pow2_const (unsigned int v)
    {

    while (v > 0)
        if (v == 1)
	    {
	    return TRUE;
	    }
	else if (v & 1)
	    {
	    return FALSE;
	    }
	else
	    v = v>>1;

    return FALSE;
    }
