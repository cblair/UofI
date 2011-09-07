#include "sassy.h"
#include "error.h"
#include "ddcf.h"

/*  This optimization works on DFGs, i.e. dataflow graphs without SA-C type on the edges.
 *  (Each edge simply has a bitwidth.) It works in three phases. The first is a downward
 *  pass that tags each node port with a maximum value; in the absence of any other info,
 *  the max value of a port is simply
 *	2^n - 1
 *  where 'n' is the bitwidth of the port.
 *  The second phase is an upward pass that narrows the widths of the ports:
 *	- a node's output port width is set to
 *		min (bitwidth (maxval), max (all target ports bitwidths))
 *	  (this is not done for nodes that read from memory (ie generators or ARRAYREFs)
 *	- insert a CHANGE-WIDTH node between an output target and its target node
 *	  if the port widths dont match
 *  The third phase goes through the DFG and removes any CHANGE-WIDTH nodes whose input
 *  and output widths are equal.
 *
 *
 *  For the 2-input, 1-output nodes, the following names are assumed:
 *	- Va is the maxval of inputs[0]
 *	- a is the bitwidth of Va
 *	- A is the value of inputs[0] is the input is a constant
 *	- Vb is the maxval of inputs[1]
 *	- b is the bitwidth of Vb
 *	- B is the value of inputs[1] is the input is a constant
 *	- Vc is the maxval of outputs[0]
 *	- c is the bitwidth of Vc
 *
 *  The rules for the downward pass are:
 *	BIT_OR
 *	    if inputs[1] is constant then
 *		Vc = (Va | B) & (2^c - 1)
 *	    else if inputs[0] is constant then
 *		Vc = (Vb | A) & (2^c - 1)
 *	    else
 *		w = bitwidth (min (Va, Vb))
 *		Vc = ((2^w - 1) | max (Va, Vb)) & (2^c - 1)
 *	BIT_AND
 *	    if inputs[1] is constant then
 *		Vc = (min (Va, B)) & (2^c - 1)
 *	    else if inputs[0] is constant them
 *		Vc = (min (Vb, A)) & (2^c - 1)
 *	    else
 *		Vc = (min (Va, Vb)) & (2^c - 1)
 *	BIT_EOR
 *	    if inputs[1] is constant then
 *		Vb = B
 *	    else if inputs[0] is constant then
 *		Va = A
 *	    w = bitwidth (min (Va, Vb))
 *	    Vc = ((2^w - 1) | max (Va, Vb)) & (2^c - 1)
 *	BIT_COMPL
 *	    Vc = (2^a - 1) & (2^c - 1)
 *	UADD
 *	    if inputs[1] is constant then
 *		Vb = B
 *	    else if inputs[0] is constant then
 *		Va = A
 *	    Vc = (Va + Vb) & (2^c - 1)
 *	USUB
 *	    Vc = 2^c - 1
 *	U<compare>
 *	    Vc = 1
 *	LSHIFT
 *	    if inputs[1] is constant then
 *		Vc = (Va << B) & (2^c - 1)
 *	    else
 *		Vc = 2^c - 1
 *	RSHIFT
 *	    if inputs[1] is constant then
 *		Vc = (Va >> B) & (2^c - 1)
 *	    else
 *		Vc = 2^c - 1
 *	CHANGE_WIDTH
 *	    w = bitwidth (Va)
 *	    if (c >= w) then
 *		Vc = Va
 *	    else
 *		Vc = 2^c - 1
 *	SELECTOR
 *	    Vc = max (all the input maxvals)
 *	all other node types
 *	    each output's maxval = 2^c - 1
 *		where c = bitwidth of the output
 *	    
 *
 *  The rules for the upward pass are:
 *	BIT_OR
 *	BIT_AND
 *	BIT_EOR
 *	    a = min (c, bitwidth (Va))
 *	    b = min (c, bitwidth (Vb))
 *	BIT_COMPL
 *	    a = min (a, c)
 *	UADD
 *	USUB
 *	    if (Va > Vb) then
 *		a = c
 *		b = min (bitwidth (Vb), c)
 *	    else
 *		b = c
 *		a = min (bitwidth (Va), c)
 *	U<compare>
 *	    a = bitwidth (Va)
 *	    b = bitwidth (Vb)
 *	LSHIFT
 *	    if inputs[1] is constant then
 *		a = c - B
 *	    else
 *		input bitwidths are left unchanged
 *	RSHIFT
 *	    if inputs[1] is constant then
 *		a = c + B
 *	    else
 *		input bitwidths are left unchanged
 *	CHANGE_WIDTH
 *	    a = min (c, bitwidth (Va))
 *	SELECTOR
 *	    for each input
 *		width of input = width of output
 *	REDUCE_ISUM_MACRO :
 *	REDUCE_USUM_MACRO :
 *	    for each value input
 *		width of input = width of output
 *	REDUCE_VAL_AT_FIRST_IMAX_MACRO :
 *	REDUCE_VAL_AT_FIRST_IMIN_MACRO :
 *	REDUCE_VAL_AT_LAST_IMAX_MACRO :
 *	REDUCE_VAL_AT_LAST_IMIN_MACRO :
 *	REDUCE_VAL_AT_FIRST_UMAX_MACRO :
 *	REDUCE_VAL_AT_FIRST_UMIN_MACRO :
 *	REDUCE_VAL_AT_LAST_UMAX_MACRO :
 *	REDUCE_VAL_AT_LAST_UMIN_MACRO :
 *	    for each captured value input
 *		width of input = width of its associated output
 *	all other node types
 *	    input widths are left unchanged
 */

int ids_process_graphs_nodes (FuncGraph *fg, int id);
void dead_code_graph (FuncGraph *fg, int id);
int const_fold_graph (FuncGraph *fg, int id);
int reads_mem (FuncGraph*, int);
void count_bit_ops (FuncGraph*, int, int*, int*, int*, int*);
void unify_with_targets (FuncGraph*, int, IntList**);
void adjust_const_input_vals (FuncGraph*, int);
int remove_change_widths (FuncGraph*, int);
void bitwidth_narrow_upward (FuncGraph*, int);
void bitwidth_narrow_downward (FuncGraph*, int);
void send_values_down (FuncGraph*, Edge*, Uint64);
int width_of_val (Uint64);
Uint64 bound_the_value (Uint64, int);
Uint64 get_const_64 (char*);

void bitwidth_narrow (FuncGraph *fg, int loop)
    {
    int changed;
    int arith_before, logic_before, compare_before, select_before;
    int arith_after, logic_after, compare_after, select_after;

    count_bit_ops (fg, loop, &arith_before, &compare_before, &logic_before, &select_before);
    bitwidth_narrow_downward (fg, loop);
    bitwidth_narrow_upward (fg, loop);
    /*
    do  {
        changed = remove_change_widths (fg, loop);
	} while (changed);
    */

    do  {
        changed = const_fold_graph (fg, loop);
        changed = ids_process_graphs_nodes (fg, loop) || changed;
        dead_code_graph (fg, loop);
	} while (changed);

    count_bit_ops (fg, loop, &arith_after, &compare_after, &logic_after, &select_after);

    if (!quiet)
        {
	printf ("  bitwidth narrowing:\n");
	printf ("    logic ops\t%4d to %4d bits\n", logic_before, logic_after);
	printf ("    compare ops\t%4d to %4d bits\n", compare_before, compare_after);
	printf ("    arith ops\t%4d to %4d bits\n", arith_before, arith_after);
	printf ("    select ops\t%4d to %4d bits\n", select_before, select_after);
	}
    }

int remove_change_widths (FuncGraph *fg, int loop)
    {
    IntList *it, *cpy;
    Edge *ed;
    int id, snd, spt, tnd, tpt;
    int changed = FALSE;

    copy_intlist (&cpy, &(fg->nodes[loop].My_nodes));

    for (it=fg->nodes[loop].My_nodes; it!=NULL; it=it->link)
        {
	id = it->val;

	if (fg->nodes[id].nodetype == ND_CHANGE_WIDTH)
	    if (fg->nodes[id].inputs[0].ty.totsize == fg->nodes[id].outputs[0].ty.totsize)
	        {
		changed = TRUE;

		if (fg->nodes[id].inputs[0].is_const)
		    {
		    for (ed=fg->nodes[id].outputs[0].targets; ed!=NULL; ed=ed->link)
		        {
			tnd = ed->node;
			tpt = ed->port;
			free_edge_list (&(fg->nodes[tnd].inputs[tpt].back_edges));
			fg->nodes[tnd].inputs[tpt].is_const = TRUE;
			fg->nodes[tnd].inputs[tpt].constval = fg->nodes[id].inputs[0].constval;
			}

		    free_edge_list (&(fg->nodes[id].outputs[0].targets));
		    free_ddcf_node (fg, id, &cpy);
		    }
		else
		    {
		    snd = fg->nodes[id].inputs[0].back_edges->node;
		    spt = fg->nodes[id].inputs[0].back_edges->port;
		    remove_edge (fg, snd, spt, id, 0);
		    retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, snd, spt);
		    append_target_list (&(fg->nodes[snd].outputs[spt].targets), fg->nodes[id].outputs[0].targets);
		    fg->nodes[id].outputs[0].targets = NULL;
		    free_edge_list (&(fg->nodes[id].inputs[0].back_edges));
		    free_ddcf_node (fg, id, &cpy);
		    }
		}
	}

    free_intlist (&(fg->nodes[loop].My_nodes));
    fg->nodes[loop].My_nodes = cpy;

    return changed;
    }

void bitwidth_narrow_upward (FuncGraph *fg, int loop)
    {
    IntList *it, *cpy;
    int id, width, w0, w1, wout, i, j;
    char str[256];
    Uint64 v0, v1;

    copy_intlist (&cpy, &(fg->nodes[loop].My_nodes));
    reverse_intlist (&cpy);

    for (it=cpy; it!=NULL; it=it->link)
        {
	id = it->val;

	unify_with_targets (fg, id, &(fg->nodes[loop].My_nodes));

	switch (fg->nodes[id].nodetype)
	    {
	    case ND_CHANGE_WIDTH :
		v0 = fg->nodes[id].inputs[0].maxval;
	        width = width_of_val (v0);

		if (width > fg->nodes[id].outputs[0].ty.totsize)
		    fg->nodes[id].inputs[0].ty.totsize = fg->nodes[id].outputs[0].ty.totsize;
		else
		    fg->nodes[id].inputs[0].ty.totsize = width;

		/* make inputs[1] specify the new output width */
		sprintf (str, "0x%x", fg->nodes[id].outputs[0].ty.totsize);
		fg->nodes[id].inputs[1].constval = copy_to_const (str);
		fg->nodes[id].inputs[1].maxval = fg->nodes[id].outputs[0].ty.totsize;
		width = width_of_val (fg->nodes[id].outputs[0].ty.totsize);
		fg->nodes[id].inputs[1].ty.totsize = width;

		break;
	    case ND_BIT_OR :
	    case ND_BIT_AND :
	    case ND_BIT_EOR :
		width = fg->nodes[id].outputs[0].ty.totsize;

		v0 = fg->nodes[id].inputs[0].maxval;
	        w0 = width_of_val (v0);
		w0 = (w0<width) ? w0 : width;
		fg->nodes[id].inputs[0].ty.totsize = w0;

		v1 = fg->nodes[id].inputs[1].maxval;
	        w1 = width_of_val (v1);
		w1 = (w1<width) ? w1 : width;
		fg->nodes[id].inputs[1].ty.totsize = w1;

		break;
	    case ND_BIT_COMPL :
		width = fg->nodes[id].outputs[0].ty.totsize;
	        w0 = fg->nodes[id].inputs[0].ty.totsize;
		fg->nodes[id].inputs[0].ty.totsize = (w0>width) ? width : w0;
		break;
	    case ND_RC_SELECTOR :
		width = fg->nodes[id].outputs[0].ty.totsize;

		for (i=2; i<fg->nodes[id].num_inputs; i+=2)
		    fg->nodes[id].inputs[i].ty.totsize = width;
		fg->nodes[id].inputs[fg->nodes[id].num_inputs-1].ty.totsize = width;
	        break;
	    case ND_UEQ :
	    case ND_UNEQ :
	    case ND_UGT :
	    case ND_UGE :
	    case ND_ULT :
	    case ND_ULE :
		v0 = fg->nodes[id].inputs[0].maxval;
		w0 = width_of_val (v0);
		fg->nodes[id].inputs[0].ty.totsize = w0;
		v1 = fg->nodes[id].inputs[1].maxval;
		w1 = width_of_val (v1);
		fg->nodes[id].inputs[1].ty.totsize = w1;
	        break;
	    case ND_LEFT_SHIFT :
		fg->nodes[id].inputs[0].ty.totsize = fg->nodes[id].outputs[0].ty.totsize;
		break;
	    case ND_RIGHT_SHIFT :
	        if (fg->nodes[id].inputs[1].is_const)
		    {
		    width = fg->nodes[id].outputs[0].ty.totsize + fg->nodes[id].inputs[1].maxval;
		    width = (width>32) ? 32 : width;
		    fg->nodes[id].inputs[0].ty.totsize = width;
		    }
		break;
	    case ND_UADD :
	    case ND_USUB :
		v0 = fg->nodes[id].inputs[0].maxval;
		v1 = fg->nodes[id].inputs[1].maxval;
		if (v0 > v1)
		    {
		    fg->nodes[id].inputs[0].ty.totsize = fg->nodes[id].outputs[0].ty.totsize;

		    width = width_of_val (v1);
		    if (width > fg->nodes[id].outputs[0].ty.totsize)
		        fg->nodes[id].inputs[1].ty.totsize = fg->nodes[id].outputs[0].ty.totsize;
		    else
		        fg->nodes[id].inputs[1].ty.totsize = width;
		    }
		else
		    {
		    fg->nodes[id].inputs[1].ty.totsize = fg->nodes[id].outputs[0].ty.totsize;

		    width = width_of_val (v0);
		    if (width > fg->nodes[id].outputs[0].ty.totsize)
		        fg->nodes[id].inputs[0].ty.totsize = fg->nodes[id].outputs[0].ty.totsize;
		    else
		        fg->nodes[id].inputs[0].ty.totsize = width;
		    }
	        break;
	    case ND_REDUCE_ISUM_MACRO :
	    case ND_REDUCE_USUM_MACRO :
		if (fg->nodes[id].inputs[0].ty.totsize > fg->nodes[id].outputs[0].ty.totsize)
		    for (i=0; i<fg->nodes[id].num_inputs; i+=2)
		        fg->nodes[id].inputs[i].ty.totsize = fg->nodes[id].outputs[0].ty.totsize;
	        break;
	    case ND_REDUCE_VAL_AT_FIRST_IMAX_MACRO :
	    case ND_REDUCE_VAL_AT_FIRST_IMIN_MACRO :
	    case ND_REDUCE_VAL_AT_LAST_IMAX_MACRO :
	    case ND_REDUCE_VAL_AT_LAST_IMIN_MACRO :
	    case ND_REDUCE_VAL_AT_FIRST_UMAX_MACRO :
	    case ND_REDUCE_VAL_AT_FIRST_UMIN_MACRO :
	    case ND_REDUCE_VAL_AT_LAST_UMAX_MACRO :
	    case ND_REDUCE_VAL_AT_LAST_UMIN_MACRO :
		{
		int vecsize = fg->nodes[id].num_outputs;
		int out_width;

		for (i=0; i<fg->nodes[id].num_inputs; i+=(2+vecsize))
		    for (j=0; j<vecsize; j++)
		        {
			out_width = fg->nodes[id].outputs[j].ty.totsize;
			if (fg->nodes[id].inputs[i+j+1].ty.totsize > out_width)
			    fg->nodes[id].inputs[i+j+1].ty.totsize = out_width;
			}
		}
	        break;
	    default :
	        break;
	    }

	adjust_const_input_vals (fg, id);
	}

    free_intlist (&cpy);
    }

void bitwidth_narrow_downward (FuncGraph *fg, int loop)
    {
    IntList *it;
    int id, width, i;
    Uint64 mx, v0, v1, cval, mn_val, mx_val;

    for (it=fg->nodes[loop].My_nodes; it!=NULL; it=it->link)
        {
	id = it->val;

	switch (fg->nodes[id].nodetype)
	    {
	    case ND_G_INPUT :
	        mx = mask_ones[fg->nodes[id].outputs[0].ty.totsize];
		fg->nodes[id].outputs[0].maxval = mx;
		send_values_down (fg, fg->nodes[id].outputs[0].targets, mx);
		break;
	    case ND_CHANGE_WIDTH :
		if (fg->nodes[id].inputs[0].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[0].constval);
		    fg->nodes[id].inputs[0].maxval = cval;
		    }
		cval = get_const_64 (fg->nodes[id].inputs[1].constval);
		fg->nodes[id].inputs[1].maxval = cval;
		v0 = fg->nodes[id].inputs[0].maxval;
		v1 = fg->nodes[id].inputs[1].maxval;
		width = width_of_val (v0);
		if (v1 >= width)
		    mx = bound_the_value (v0, fg->nodes[id].outputs[0].ty.totsize);
		else
		    mx = mask_ones[fg->nodes[id].outputs[0].ty.totsize];
		fg->nodes[id].outputs[0].maxval = mx;
		send_values_down (fg, fg->nodes[id].outputs[0].targets, mx);
	        break;
	    case ND_BIT_OR :
	        if (fg->nodes[id].inputs[0].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[0].constval);
		    fg->nodes[id].inputs[0].maxval = cval;
		    v1 = fg->nodes[id].inputs[1].maxval;
		    mx = bound_the_value (cval | v1, fg->nodes[id].outputs[0].ty.totsize);
		    }
		else if (fg->nodes[id].inputs[1].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[1].constval);
		    fg->nodes[id].inputs[1].maxval = cval;
		    v0 = fg->nodes[id].inputs[0].maxval;
		    mx = bound_the_value (cval | v0, fg->nodes[id].outputs[0].ty.totsize);
		    }
		else
		    {
		    v0 = fg->nodes[id].inputs[0].maxval;
		    v1 = fg->nodes[id].inputs[1].maxval;
		    mn_val = (v0>v1) ? v1 : v0;
		    width = width_of_val (mn_val);
		    mx_val = (v0>v1) ? v0 : v1;
		    mx = bound_the_value (mx_val | mask_ones[width], fg->nodes[id].outputs[0].ty.totsize);
		    }
		fg->nodes[id].outputs[0].maxval = mx;
		send_values_down (fg, fg->nodes[id].outputs[0].targets, mx);
		break;
	    case ND_BIT_AND :
	        if (fg->nodes[id].inputs[0].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[0].constval);
		    fg->nodes[id].inputs[0].maxval = cval;
		    v1 = fg->nodes[id].inputs[1].maxval;
		    mx = (v1 > cval) ? cval : v1;
		    mx = bound_the_value (mx, fg->nodes[id].outputs[0].ty.totsize);
		    }
		else if (fg->nodes[id].inputs[1].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[1].constval);
		    fg->nodes[id].inputs[1].maxval = cval;
		    v0 = fg->nodes[id].inputs[0].maxval;
		    mx = (v0 > cval) ? cval : v0;
		    mx = bound_the_value (mx, fg->nodes[id].outputs[0].ty.totsize);
		    }
		else
		    {
		    v0 = fg->nodes[id].inputs[0].maxval;
		    v1 = fg->nodes[id].inputs[1].maxval;
		    mx = (v0 > v1) ? v1 : v0;
		    mx = bound_the_value (mx, fg->nodes[id].outputs[0].ty.totsize);
		    }
		fg->nodes[id].outputs[0].maxval = mx;
		send_values_down (fg, fg->nodes[id].outputs[0].targets, mx);
		break;
	    case ND_BIT_EOR :
	        if (fg->nodes[id].inputs[0].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[0].constval);
		    fg->nodes[id].inputs[0].maxval = cval;
		    }
		if (fg->nodes[id].inputs[1].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[1].constval);
		    fg->nodes[id].inputs[1].maxval = cval;
		    }
		v0 = fg->nodes[id].inputs[0].maxval;
		v1 = fg->nodes[id].inputs[1].maxval;
		width = width_of_val ((v0>v1)?v1:v0);
		mx = ((v0>v1)?v0:v1) | mask_ones[width];
		mx = bound_the_value (mx, fg->nodes[id].outputs[0].ty.totsize);
		fg->nodes[id].outputs[0].maxval = mx;
		send_values_down (fg, fg->nodes[id].outputs[0].targets, mx);
	        break;
	    case ND_BIT_COMPL :
	        if (fg->nodes[id].inputs[0].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[0].constval);
		    fg->nodes[id].inputs[0].maxval = cval;
		    }
		mx = mask_ones[fg->nodes[id].inputs[0].ty.totsize];
		mx = bound_the_value (mx, fg->nodes[id].outputs[0].ty.totsize);
		fg->nodes[id].outputs[0].maxval = mx;
		send_values_down (fg, fg->nodes[id].outputs[0].targets, mx);
	        break;
	    case ND_UADD :
	        if (fg->nodes[id].inputs[0].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[0].constval);
		    fg->nodes[id].inputs[0].maxval = cval;
		    }
		if (fg->nodes[id].inputs[1].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[1].constval);
		    fg->nodes[id].inputs[1].maxval = cval;
		    }
		v0 = fg->nodes[id].inputs[0].maxval;
		v1 = fg->nodes[id].inputs[1].maxval;
		mx = bound_the_value (v0+v1, fg->nodes[id].outputs[0].ty.totsize);
		fg->nodes[id].outputs[0].maxval = mx;
		send_values_down (fg, fg->nodes[id].outputs[0].targets, mx);
		break;
	    case ND_UEQ :
	    case ND_UNEQ :
	    case ND_UGT :
	    case ND_UGE :
	    case ND_ULT :
	    case ND_ULE :
	        if (fg->nodes[id].inputs[0].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[0].constval);
		    fg->nodes[id].inputs[0].maxval = cval;
		    }
		if (fg->nodes[id].inputs[1].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[1].constval);
		    fg->nodes[id].inputs[1].maxval = cval;
		    }
	        fg->nodes[id].outputs[0].maxval = 1;
		send_values_down (fg, fg->nodes[id].outputs[0].targets, mx);
		break;
	    case ND_LEFT_SHIFT :
	        if (fg->nodes[id].inputs[0].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[0].constval);
		    fg->nodes[id].inputs[0].maxval = cval;
		    }
		if (fg->nodes[id].inputs[1].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[1].constval);
		    fg->nodes[id].inputs[1].maxval = cval;
		    v0 = fg->nodes[id].inputs[0].maxval;
		    mx = bound_the_value (v0 << cval, fg->nodes[id].outputs[0].ty.totsize);
		    }
		else
		    mx = mask_ones[fg->nodes[id].outputs[0].ty.totsize];
		fg->nodes[id].outputs[0].maxval = mx;
		send_values_down (fg, fg->nodes[id].outputs[0].targets, mx);
		break;
	    case ND_RIGHT_SHIFT :
	        if (fg->nodes[id].inputs[0].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[0].constval);
		    fg->nodes[id].inputs[0].maxval = cval;
		    }
		if (fg->nodes[id].inputs[1].is_const)
		    {
		    cval = get_const_64 (fg->nodes[id].inputs[1].constval);
		    fg->nodes[id].inputs[1].maxval = cval;
		    v0 = fg->nodes[id].inputs[0].maxval;
		    mx = bound_the_value (v0 >> cval, fg->nodes[id].outputs[0].ty.totsize);
		    }
		else
		    mx = mask_ones[fg->nodes[id].outputs[0].ty.totsize];
		fg->nodes[id].outputs[0].maxval = mx;
		send_values_down (fg, fg->nodes[id].outputs[0].targets, mx);
		break;
	    case ND_RC_SELECTOR :
	        for (i=0; i<fg->nodes[id].num_inputs; i++)
		    if (fg->nodes[id].inputs[i].is_const)
		        {
		        cval = get_const_64 (fg->nodes[id].inputs[i].constval);
			fg->nodes[id].inputs[i].maxval = cval;
			}

		/* get default value */
		mx = fg->nodes[id].inputs[fg->nodes[id].num_inputs-1].maxval;

		/* check for biggest among the rest of the value inputs */
		for (i=2; i<fg->nodes[id].num_inputs; i+=2)
		    {
		    v0 = fg->nodes[id].inputs[i].maxval;
		    mx = (v0>mx) ? v0 : mx;
		    }

		mx = bound_the_value (mx, fg->nodes[id].outputs[0].ty.totsize);
		fg->nodes[id].outputs[0].maxval = mx;
		send_values_down (fg, fg->nodes[id].outputs[0].targets, mx);
	        break;
	    default :
	        for (i=0; i<fg->nodes[id].num_inputs; i++)
		    if (fg->nodes[id].inputs[i].is_const)
		        {
		        cval = get_const_64 (fg->nodes[id].inputs[i].constval);
			fg->nodes[id].inputs[i].maxval = cval;
			}
	        for (i=0; i<fg->nodes[id].num_outputs; i++)
		    {
		    mx = mask_ones[fg->nodes[id].outputs[i].ty.totsize];
		    fg->nodes[id].outputs[i].maxval = mx;
		    send_values_down (fg, fg->nodes[id].outputs[i].targets, mx);
		    }
		break;
	    }
	}
    }

/*  this is called on the upward pass, just before processing a node;
 *  it first adjusts each output width to its narrowest by looking at
 *  the maxval of that output; then, if the new width doesn't match the
 *  widths of some of the output's target inputs, CHANGE-WIDTH nodes
 *  are inserted to make the change;
 */
void unify_with_targets (FuncGraph *fg, int id, IntList **its)
    {
    Edge *ed, *edd;
    int i, mwidth, width, w, tnd, tpt, ww;
    int ch_node;
    int is_mem_reading_node;
    char str[256];

    is_mem_reading_node = reads_mem (fg, id);

    for (i=0; i<fg->nodes[id].num_outputs; i++)
        {
	if (is_mem_reading_node)
	    width = fg->nodes[id].outputs[i].ty.totsize;
	else
	    {
	    /* first find the max width of the targets */
	    mwidth = 0;
	    for (ed=fg->nodes[id].outputs[i].targets; ed!=NULL; ed=ed->link)
	        {
	        tnd = ed->node;
	        tpt = ed->port;
	        w = fg->nodes[tnd].inputs[tpt].ty.totsize;
	        mwidth = (w>mwidth) ? w : mwidth;
	        }

	    width = width_of_val (fg->nodes[id].outputs[i].maxval);

	    if (mwidth < width)
	        width = mwidth;

	    fg->nodes[id].outputs[i].ty.totsize = width;
	    fg->nodes[id].outputs[i].maxval = mask_ones[width];
	    }

	/* insert CHANGE-WIDTH nodes between this node and its targets where needed */
        for (ed=fg->nodes[id].outputs[i].targets; ed!=NULL; ed=ed->link)
            {
            tnd = ed->node;
            tpt = ed->port;
            w = fg->nodes[tnd].inputs[tpt].ty.totsize;

            if (width != w)
                {
                ch_node = new_ddcf_node (fg, ND_CHANGE_WIDTH, 2, 1, fg->nodes[id].loc);

                for (edd=fg->nodes[tnd].inputs[tpt].back_edges; edd!=NULL; edd=edd->link)
                    if ((edd->node==id) && (edd->port==i))
                        {
                        edd->node = ch_node;
                        edd->port = 0;
                        break;
                        }

                ed->node = ch_node;
                ed->port = 0;
                copy_type_info (&(fg->nodes[ch_node].inputs[0].ty), &(fg->nodes[id].outputs[i].ty));
                fg->nodes[ch_node].inputs[0].back_edges =  malloc_backedge (id, i, NULL);
                fg->nodes[ch_node].inputs[0].maxval = fg->nodes[id].outputs[i].maxval;

                /* set the bit-width input */
                sprintf (str, "0x%x", w);
                fg->nodes[ch_node].inputs[1].is_const = TRUE;
                fg->nodes[ch_node].inputs[1].constval = copy_to_const (str);
                ww = width_of_val (w);
                set_type_to_uint (&(fg->nodes[ch_node].inputs[1].ty), ww);
                fg->nodes[ch_node].inputs[1].maxval = w;

                fg->nodes[ch_node].outputs[0].targets = malloc_target (tnd, tpt, NULL);
                copy_type_info (&(fg->nodes[ch_node].outputs[0].ty), &(fg->nodes[tnd].inputs[tpt].ty));
                fg->nodes[ch_node].outputs[0].maxval = fg->nodes[tnd].inputs[tpt].maxval;

                if (w != fg->nodes[ch_node].outputs[0].ty.totsize)
                    printf ("  made a change-width node: 0x%x on input 1, and 0x%x output width\n",
                                        w, fg->nodes[ch_node].outputs[0].ty.totsize);

                insert_val_before_in_intlist (ch_node, tnd, its);
                }
            }

	}
    }

int reads_mem (FuncGraph *fg, int id)
    {
    switch (fg->nodes[id].nodetype)
        {
	case ND_RC_WINDOW_GEN_1D :
	case ND_RC_WINDOW_GEN_2D :
	case ND_RC_SLICE_GEN_2D_COL :
	case ND_RC_SLICE_GEN_2D_ROW :
	case ND_RC_ELE_GEN_1D :
	case ND_RC_ELE_GEN_2D :
	case ND_RC_ELE_GEN_3D :
	case ND_ARRAYREF_1D :
	case ND_ARRAYREF_2D :
	case ND_ARRAYREF_3D :
	case ND_ARRAYREF_4D :
	case ND_ARRAYREF_5D :
	case ND_ARRAYREF_6D :
	case ND_ARRAYREF_7D :
	case ND_ARRAYREF_8D :
	case ND_G_INPUT :
	    return TRUE;
	default :
	    return FALSE;
	}
    }

/*  after bitwidths on edges have been reduced, some input constants may be
 *  wider than their input edge widths; this routine masks the constant values
 *  down to fit the new bitwidths
 */
void adjust_const_input_vals (FuncGraph *fg, int id)
    {
    int i, w;
    unsigned int val;
    char str[256];

    for (i=0; i<fg->nodes[id].num_inputs; i++)
        if (fg->nodes[id].inputs[i].is_const)
	    {
	    sscanf (fg->nodes[id].inputs[i].constval, "%x", &val);
	    w = width_of_val ((Uint64)val);
	    if (w > fg->nodes[id].inputs[i].ty.totsize)
	        {
		val = val & mask_ones[fg->nodes[id].inputs[i].ty.totsize];
		sprintf (str, "0x%x", val);
		fg->nodes[id].inputs[i].constval = copy_to_const (str);
		fg->nodes[id].inputs[i].maxval = val;
		}
	    }
    }

void count_bit_ops (FuncGraph *fg, int loop, int *p_arith, int *p_compare, int *p_logic, int *p_sel)
    {
    IntList *it;
    int id, w0, w1, width;
    int logic_ops=0, arith_ops=0, compare_ops=0, sel_ops=0;

    for (it=fg->nodes[loop].My_nodes; it!=NULL; it=it->link)
        {
	id = it->val;

	switch (fg->nodes[id].nodetype)
	    {
	    case ND_UADD :
	    case ND_USUB :
	    case ND_IADD :
	    case ND_ISUB :
	    case ND_NEG :
	        arith_ops += fg->nodes[id].outputs[0].ty.totsize;
		break;
	    case ND_UEQ :
	    case ND_IEQ :
	    case ND_UNEQ :
	    case ND_INEQ :
	    case ND_UGT :
	    case ND_IGT :
	    case ND_UGE :
	    case ND_IGE :
	    case ND_ULT :
	    case ND_ILT :
	    case ND_ULE :
	    case ND_ILE :
		w0 = fg->nodes[id].inputs[0].ty.totsize;
		w1 = fg->nodes[id].inputs[1].ty.totsize;
		width = (w0>w1) ? w0 : w1;
	        compare_ops += width;
		break;
	    case ND_BIT_AND :
	    case ND_BIT_OR :
	    case ND_BIT_EOR :
	    case ND_BIT_COMPL :
		w0 = fg->nodes[id].inputs[0].ty.totsize;
		w1 = fg->nodes[id].inputs[1].ty.totsize;
		width = (w0<w1) ? w0 : w1;
	        logic_ops += width;
		break;
	    case ND_RC_SELECTOR :
	        sel_ops += fg->nodes[id].outputs[0].ty.totsize;
	        break;
	    default :
	        break;
	    }
	}

    *p_arith = arith_ops;
    *p_compare = compare_ops;
    *p_logic = logic_ops;
    *p_sel = sel_ops;
    }

void send_values_down (FuncGraph *fg, Edge *targets, Uint64 mx)
    {
    Edge *ed;
    int tnd, tpt;

    for (ed=targets; ed!=NULL; ed=ed->link)
	{
	tnd = ed->node;
	tpt = ed->port;
	fg->nodes[tnd].inputs[tpt].maxval = mx;
	}
    }

int width_of_val (Uint64 val)
    {
    int i;
    Uint64 vv = val;

    /*
    if (vv == 0)
        i = 1;
    else
    */
        for (i=0; vv!=0; i++, vv=vv>>1);

    return i;
    }

Uint64 bound_the_value (Uint64 val, int width)
    {
    Uint64 lim = mask_ones[width];
    if (val > lim)
        return lim;
    else
        return val;
    }

Uint64 get_const_64 (char *str)
    {
    unsigned int val;

    sscanf (str, "%x", &val);
    return (Uint64)val;
    }
