#include "sassy.h"
#include "error.h"
#include "ddcf.h"
#include <limits.h>
#include <float.h>

int cf_node (FuncGraph*, int, IntList**);
void cf_cast (TypeInfo*, TypeInfo*, char*, char*);
void cf_cast_from_uint (TypeInfo*, int, char*, char*);
void cf_cast_from_int (TypeInfo*, int, char*, char*);
void cf_cast_from_ufix (TypeInfo*, int, int, char*, char*);
void cf_cast_from_fix (TypeInfo*, int, int, char*, char*);
void cf_cast_from_bits (TypeInfo*, int, char*, char*);
void cf_cast_from_bool (TypeInfo*, char*, char*);
void cf_cast_from_float (TypeInfo*, char*, char*);
void cf_cast_from_double (TypeInfo*, char*, char*);
int int_from_string (char*);
void free_edge_list (Edge**);
int transfer_const_to_targets (FuncGraph*, int, int, char*);
int const_fold_graph (FuncGraph*, int);
void delete_cast (FuncGraph*, int, IntList**);
int fold_add (FuncGraph*, int, IntList**);
int fold_sub (FuncGraph*, int, IntList**);
int fold_mul (FuncGraph*, int, IntList**);
int fold_div (FuncGraph*, int, IntList**);
int fold_mod (FuncGraph*, int, IntList**);
int fold_cmp (FuncGraph*, int, IntList**, DdcfType);
int fold_bool (FuncGraph*, int, IntList**, DdcfType);
int fold_bits (FuncGraph*, int, IntList**, DdcfType);
int fold_shift (FuncGraph*, int, IntList**, DdcfType);
int fold_neg (FuncGraph*, int, IntList**);
int fold_sum_macro (FuncGraph*, int, IntList**);
int fold_product_macro (FuncGraph*, int, IntList**);
int fold_and_macro (FuncGraph*, int, IntList**);
int fold_or_macro (FuncGraph*, int, IntList**);
int fold_min_macro (FuncGraph*, int, IntList**);
int fold_max_macro (FuncGraph*, int, IntList**);
int partial_fold_macro (FuncGraph*, int, IntList**);
void sum_constants (FuncGraph*, int, char*);
void and_constants (FuncGraph*, int, char*);
void or_constants (FuncGraph*, int, char*);
void max_constants (FuncGraph*, int, char*);
void min_constants (FuncGraph*, int, char*);
int val_from_hex_string (char*);
int cf_extents (FuncGraph*, int, IntList**);
int zero_const_w_type (Type, char*);
int intrin_func_call (FuncGraph*, int, IntList**);
int cf_intrin_1_1 (FuncGraph*, int, IntList**);
int cf_intrin_2_1 (FuncGraph*, int, IntList**);

int sign_extend (int s_val, int s_oldsz, int s_newsz)
    {
    int res;

    /* if sign bit not set, we just return the value */
    if (sc_pow2[s_oldsz-1] & s_val)
        res = s_val | (set_ones[s_oldsz]&mask_ones[s_newsz]);
    else
        res = s_val;

    return res;
    }

void constant_fold ()
    {
    FuncGraph *fg;
    int changed;

    do  {
	changed = FALSE;
        for (fg=fgraphs; fg!=NULL; fg=fg->link)
            if (fg->nodes_used > 0)
	        changed = const_fold_graph (fg, 0) || changed;
	if (changed)
	    is_stable = FALSE;
	} while (changed);
    }

int const_fold_graph (FuncGraph *fg, int id)
    {
    IntList *it, *list_copy;
    int changed = FALSE;

    copy_intlist (&list_copy, &(fg->nodes[id].My_nodes));

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        changed = cf_node (fg, it->val, &(list_copy)) || changed;

    free_intlist (&(fg->nodes[id].My_nodes));
    fg->nodes[id].My_nodes = list_copy;

    return changed;
    }

int cf_node (FuncGraph *fg, int id, IntList **itlist)
    {
    char str[256];
    int keep, in_node, i;
    int changed = FALSE;
    TypeInfo *tt;

    /*
    printf ("entering 'cf_node', id %d\n", id); fflush (stdout);
    */

    /* this is for the DFGs; any <bits0> output should be replaced by constant
     * zeroes on each of its targets
     */
    for (i=0; i<fg->nodes[id].num_outputs; i++)
	{
	tt = &(fg->nodes[id].outputs[i].ty);
        if ((tt->kind==Scalar) && (tt->type==Bits) && (tt->totsize==0))
	    {
	    transfer_const_to_targets (fg, id, i, "0x0");
	    free_edge_list (&(fg->nodes[id].outputs[i].targets));
	    }
	}

    switch (fg->nodes[id].nodetype)
        {
	case ND_CHANGE_WIDTH:
	    if (fg->nodes[id].inputs[0].is_const)
	        {
		unsigned int val;
		sscanf (fg->nodes[id].inputs[0].constval, "%x", &val);
		val &= mask_ones[fg->nodes[id].outputs[0].ty.totsize];
		sprintf (str, "0x%x", val);
		transfer_const_to_targets (fg, id, 0, str);
		free_edge_list (&(fg->nodes[id].outputs[0].targets));
		}
	    break;
// 	case ND_RC_SELECTOR :
// 	    if (fg->nodes[id].inputs[0].is_const)
// 	        {
// 	        unsigned int kval, val;
// 		sscanf (fg->nodes[id].inputs[0].constval, "%x", &kval);
// 		for (i=1; i<fg->nodes[id].num_inputs-1; i+=2)
// 		    {
// 		    assert (fg->nodes[id].inputs[i].is_const);
// 		    sscanf (fg->nodes[id].inputs[i].constval, "%x", &val);
// 		    if (val == kval)
// 		        break;
// 		    }
// 
// 		if (i >= fg->nodes[id].num_inputs-1)
// 		    i = fg->nodes[id].num_inputs-1;
// 		else
// 		    i++;
// 
// 		if (fg->nodes[id].inputs[i].is_const)
// 		    {
// 		    val &= mask_ones[fg->nodes[id].outputs[0].ty.totsize];
// 		    sprintf (str, "0x%x", val);
// 		    transfer_const_to_targets (fg, id, 0, str);
// 		    free_edge_list (&(fg->nodes[id].outputs[0].targets));
// 		    }
// 		else
// 		    {
// 		    int snd, spt;
// 		    snd = fg->nodes[id].inputs[i].back_edges->node;
// 		    spt = fg->nodes[id].inputs[i].back_edges->port;
// 		    remove_edge (fg, snd, spt, id, i);
// 		    retarget_backedges (fg, fg->nodes[id].outputs[0].targets, id, 0, snd, spt);
// 		    append_target_list (&(fg->nodes[snd].outputs[spt].targets), fg->nodes[id].outputs[0].targets);
// 		    fg->nodes[id].outputs[0].targets = NULL;
// 		    }
// 	        }
// 	    break;
	case ND_CAST :
	    if (fg->nodes[id].inputs[0].is_const)
		{
	        cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[0].ty),
						str, fg->nodes[id].inputs[0].constval);
		changed = propagate_and_delete (fg, id, str, itlist) || changed;
		}
	    else if (type_equal (&(fg->nodes[id].inputs[0].ty), &(fg->nodes[id].outputs[0].ty)))
	        {
		delete_cast (fg, id, itlist);
		changed = TRUE;
		}
	    break;
	case ND_EXTENTS :
	    changed = cf_extents (fg, id, itlist) || changed;
	    break;
	case ND_REDUCE_SUM_MACRO :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_sum_macro (fg, id, itlist) || changed;
	    else
	        changed = partial_fold_macro (fg, id, itlist) || changed;
	    break;
	case ND_REDUCE_PRODUCT_MACRO :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_product_macro (fg, id, itlist) || changed;
	    /*
	    else
	        changed = partial_fold_macro (fg, id, itlist) || changed;
	    */
	    break;
	case ND_REDUCE_AND_MACRO :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_and_macro (fg, id, itlist) || changed;
	    else
	        changed = partial_fold_macro (fg, id, itlist) || changed;
	    break;
	case ND_REDUCE_OR_MACRO :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_or_macro (fg, id, itlist) || changed;
	    else
	        changed = partial_fold_macro (fg, id, itlist) || changed;
	    break;
	case ND_REDUCE_MIN_MACRO :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_min_macro (fg, id, itlist) || changed;
	    else
	        changed = partial_fold_macro (fg, id, itlist) || changed;
	    break;
	case ND_REDUCE_MAX_MACRO :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_max_macro (fg, id, itlist) || changed;
	    else
	        changed = partial_fold_macro (fg, id, itlist) || changed;
	    break;
	case ND_ADD :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_add (fg, id, itlist) || changed;
	    break;
	case ND_SUB :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_sub (fg, id, itlist) || changed;
	    break;
	case ND_MUL :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_mul (fg, id, itlist) || changed;
	    break;
	case ND_DIV :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_div (fg, id, itlist) || changed;
	    break;
	case ND_MOD :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_mod (fg, id, itlist) || changed;
	    break;
	case ND_LT :
	case ND_LE :
	case ND_GT :
	case ND_GE :
	case ND_EQ :
	case ND_NEQ :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_cmp (fg, id, itlist, fg->nodes[id].nodetype) || changed;
	    break;
	case ND_AND :
	case ND_OR :
	case ND_NOT :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_bool (fg, id, itlist, fg->nodes[id].nodetype) || changed;
	    break;
	case ND_BIT_AND :
	case ND_BIT_OR :
	case ND_BIT_EOR :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_bits (fg, id, itlist, fg->nodes[id].nodetype) || changed;
	    break;
	case ND_LEFT_SHIFT_MACH :
	case ND_LEFT_SHIFT :
	case ND_RIGHT_SHIFT :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_shift (fg, id, itlist, fg->nodes[id].nodetype) || changed;
	    break;
	case ND_NEG :
	    if (all_inputs_are_constant (fg, id))
	        changed = fold_neg (fg, id, itlist) || changed;
	    break;
	case ND_SWITCH :
	case ND_CASE :
	case ND_WHILE :
	case ND_FORALL :
	case ND_FORNXT :
	case ND_FUNC :
	case ND_CROSS_PROD :
	case ND_DOT_PROD :
	case ND_WHILE_PRED :
	    /* push any constants from the graph's inputs into the interior of the graph */
	    for (i=0; i<fg->nodes[id].num_inputs; )
	        if (fg->nodes[id].inputs[i].is_const)
		    {
		    strcpy (str, fg->nodes[id].inputs[i].constval);
		    in_node = fg->nodes[id].My_inputs[i];
		    keep = transfer_const_to_targets (fg, in_node, 0, str);

		    if (! keep)
		        {
		        SacFree (fg->nodes[in_node].inputs)
		        free_edge_list (&(fg->nodes[in_node].outputs[0].targets));
		        SacFree (fg->nodes[in_node].outputs)
		        fg->nodes[in_node].nodetype = ND_VOIDED;

		        /* remove the node from the graph's list of nodes */
		        delete_ele_from_intlist (in_node, &(fg->nodes[id].My_nodes));
			changed = TRUE;
		        }

		    eliminate_graph_input (fg, id, i);
		    }
		else
		    i++;

	    changed = const_fold_graph (fg, id) || changed;

	    break;
	case ND_INTRINCALL:
	    if (all_inputs_are_constant (fg, id))
	        changed = intrin_func_call (fg, id, itlist) || changed;
	    break;
	default :
            break;
	}

    /*
    printf ("leaving 'cf_node'\n"); fflush (stdout);
    */

    return changed;
    }

int intrin_func_call (FuncGraph *fg, int id, IntList **itlist)
    {
    int changed;

    switch (fg->nodes[id].Intrin_func)
        {
	case IntrinFrexp :
	case IntrinModf :
	    /* haven't implemented these yet... */
	    return FALSE;
	    break;
	case IntrinHypot :
	case IntrinAtan2 :
	case IntrinFmod :
	case IntrinCopysign :
	    changed = cf_intrin_2_1 (fg, id, itlist);
	    break;
	case IntrinSin :
	case IntrinCos :
	case IntrinTan :
	case IntrinAsin :
	case IntrinAcos :
	case IntrinAtan :
	case IntrinSinh :
	case IntrinCosh :
	case IntrinTanh :
	case IntrinAsinh :
	case IntrinAcosh :
	case IntrinAtanh :
	case IntrinCbrt :
	case IntrinExp :
	case IntrinLog :
	case IntrinLog10 :
	case IntrinExpm1 :
	case IntrinLog1p :
	case IntrinCeil :
	case IntrinFabs :
	case IntrinFloor :
	case IntrinRint :
	case IntrinSqrt :
	    changed = cf_intrin_1_1 (fg, id, itlist);
	    break;
	default :
	    assert (0);
	}

    return changed;
    }

int cf_intrin_2_1 (FuncGraph *fg, int id, IntList **itlist)
    {
    char tg[128], tg0[128], tg1[128];
    TypeInfo ty;
    double dval, dval0, dval1;
    int changed;

    ty.type = Double;
    ty.kind = Scalar;
    ty.totsize = 64;
    ty.fracsize = 0;

    cf_cast (&ty, &(fg->nodes[id].inputs[0].ty), tg0, fg->nodes[id].inputs[0].constval);
    cf_cast (&ty, &(fg->nodes[id].inputs[1].ty), tg1, fg->nodes[id].inputs[1].constval);
    sscanf (tg0, "%lf", &dval0);
    sscanf (tg1, "%lf", &dval1);

    switch (fg->nodes[id].Intrin_func)
        {
	case IntrinHypot :
	    dval = hypot (dval0, dval1);
	    break;
	case IntrinAtan2 :
	    dval = atan2 (dval0, dval1);
	    break;
	case IntrinFmod :
	    dval = fmod (dval0, dval1);
	    break;
	case IntrinCopysign :
	    dval = copysign (dval0, dval1);
	    break;
	default :
	    assert (0);
	}

    sprintf (tg, "%f", dval);
    cf_cast (&(fg->nodes[id].outputs[0].ty), &ty, tg1, tg);
    changed = propagate_and_delete (fg, id, tg1, itlist);

    return changed;
    }

int cf_intrin_1_1 (FuncGraph *fg, int id, IntList **itlist)
    {
    char tg[128], tg1[128];
    TypeInfo ty;
    double dval;
    int changed;

    ty.type = Double;
    ty.kind = Scalar;
    ty.totsize = 64;
    ty.fracsize = 0;

    cf_cast (&ty, &(fg->nodes[id].inputs[0].ty), tg, fg->nodes[id].inputs[0].constval);
    sscanf (tg, "%lf", &dval);

    switch (fg->nodes[id].Intrin_func)
        {
	case IntrinSin :
	    dval = sin (dval);
	    break;
	case IntrinCos :
	    dval = cos (dval);
	    break;
	case IntrinTan :
	    dval = tan (dval);
	    break;
	case IntrinAsin :
	    dval = asin (dval);
	    break;
	case IntrinAcos :
	    dval = acos (dval);
	    break;
	case IntrinAtan :
	    dval = atan (dval);
	    break;
	case IntrinSinh :
	    dval = sinh (dval);
	    break;
	case IntrinCosh :
	    dval = cosh (dval);
	    break;
	case IntrinTanh :
	    dval = tanh (dval);
	    break;
	case IntrinAsinh :
	    dval = asinh (dval);
	    break;
	case IntrinAcosh :
	    dval = acosh (dval);
	    break;
	case IntrinAtanh :
	    dval = atanh (dval);
	    break;
	case IntrinCbrt :
	    dval = cbrt (dval);
	    break;
	case IntrinExp :
	    dval = exp (dval);
	    break;
	case IntrinLog :
	    dval = log (dval);
	    break;
	case IntrinLog10 :
	    dval = log10 (dval);
	    break;
	case IntrinExpm1 :
	    dval = expm1 (dval);
	    break;
	case IntrinLog1p :
	    dval = log1p (dval);
	    break;
	case IntrinCeil :
	    dval = ceil (dval);
	    break;
	case IntrinFabs :
	    dval = fabs (dval);
	    break;
	case IntrinFloor :
	    dval = floor (dval);
	    break;
	case IntrinRint :
	    dval = rint (dval);
	    break;
	case IntrinSqrt :
	    dval = sqrt (dval);
	    break;
	default :
	    assert (0);
	}

    sprintf (tg, "%f", dval);
    cf_cast (&(fg->nodes[id].outputs[0].ty), &ty, tg1, tg);
    changed = propagate_and_delete (fg, id, tg1, itlist);

    return changed;
    }

int cf_extents (FuncGraph *fg, int id, IntList **itlist)
    {
    int rank, i, keep, kk;
    char str[128];

    rank = fg->nodes[id].inputs[0].ty.dims;
    keep = FALSE;

    for (i=0; i<rank; i++)
        if (fg->nodes[id].inputs[0].ty.dim_sizes[i] >= 0)
	    {
	    sprintf (str, "%d", fg->nodes[id].inputs[0].ty.dim_sizes[i]);
	    kk = transfer_const_to_targets (fg, id, i, str);
	    if (kk)
	        keep = TRUE;
	    else
		free_edge_list (&(fg->nodes[id].outputs[i].targets));
	    }
	else
	    keep = TRUE;

    /* we won't free the node here, even if it has no output edges,
     * because it has an input edge that we would have to eliminate
     * as well; it's easier to let dead-code elimination clean it
     * up; return FALSE, indicating that the node was not freed yet;
     */
    return FALSE;
    }

void delete_cast (FuncGraph *fg, int cast_node, IntList **itlist)
    {
    int node, port;

    assert (fg->nodes[cast_node].inputs[0].back_edges->link == NULL);

    node = fg->nodes[cast_node].inputs[0].back_edges->node;
    port = fg->nodes[cast_node].inputs[0].back_edges->port;
    retarget_backedges (fg, fg->nodes[cast_node].outputs[0].targets, cast_node, 0, node, port);
    remove_targets_to_this_port (fg, fg->nodes[cast_node].inputs[0].back_edges, cast_node, 0);
    free_edge_list (&(fg->nodes[cast_node].inputs[0].back_edges));
    append_target_list (&(fg->nodes[node].outputs[port].targets), fg->nodes[cast_node].outputs[0].targets);
    fg->nodes[cast_node].outputs[0].targets = NULL;
    free_ddcf_node (fg, cast_node, itlist);
    }

int partial_fold_macro (FuncGraph *fg, int id, IntList **itlist)
    {
    InputPort *new_ports;
    int c1, c2, c3, i, j, n_in_pairs;
    char str[256];
    int changed = FALSE;
    int deleted = FALSE;

    /* count the different kinds of values:
     *
     *  c1 = values with unknown mask
     *  c2 = unknown values with "true" mask
     *  c3 = known values with "true" mask
     */
    for (c1=c2=c3=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
        if (!fg->nodes[id].inputs[i+1].is_const)
	    c1++;
	else if ((strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0) ||
				(strcmp (fg->nodes[id].inputs[i+1].constval, "0x1") == 0))
	    {
	    if (fg->nodes[id].inputs[i].is_const)
	        c3++;
	    else
	        c2++;
	    }

    n_in_pairs = c1 + c2;
    if (c3 > 0)
        n_in_pairs++;
    
    if (2 * n_in_pairs == fg->nodes[id].num_inputs)
        return changed;

    changed = TRUE;

    new_ports = make_input_ports (2 * n_in_pairs);
    for (i=0, j=0; i<fg->nodes[id].num_inputs; i+=2)
        if (!fg->nodes[id].inputs[i+1].is_const)
	    {
	    new_ports[j] = fg->nodes[id].inputs[i];
	    if (!new_ports[j].is_const)
	        retarget (fg, new_ports[j].back_edges, id, i, id, j);
	    new_ports[j+1] = fg->nodes[id].inputs[i+1];
	    retarget (fg, new_ports[j+1].back_edges, id, i+1, id, j+1);
	    j+=2;
	    }
	else if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
	    {
	    if (!fg->nodes[id].inputs[i].is_const)
	        {
		new_ports[j] = fg->nodes[id].inputs[i];
		retarget (fg, new_ports[j].back_edges, id, i, id, j);
		new_ports[j+1] = fg->nodes[id].inputs[i+1];
		j+=2;
		}
	    }
	else if (!fg->nodes[id].inputs[i].is_const)
	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[i].back_edges, id, i);

    switch (fg->nodes[id].nodetype)
        {
	case ND_REDUCE_SUM_MACRO :
    	    sum_constants (fg, id, str);
    	    if (c3 > 0)
        	{
		if (zero_const_w_type (fg->nodes[id].outputs[0].ty.type, str))
		    n_in_pairs--;
		else
		    {
        	    new_ports[j].is_const = TRUE;
		    new_ports[j].constval = copy_to_const (str);
        	    copy_type_info (&(new_ports[j].ty), &(fg->nodes[id].outputs[0].ty));
        	    new_ports[j+1].is_const = TRUE;
		    new_ports[j+1].constval = copy_to_const ("true");
        	    new_ports[j+1].ty.kind = Scalar;
        	    new_ports[j+1].ty.type = Bool;
		    }
		}
	    break;
	/*
	case ND_REDUCE_PRODUCT_MACRO :
    	    product_constants (fg, id, str);
    	    if (c3 > 0)
        	{
        	new_ports[j].is_const = TRUE;
		new_ports[j].constval = copy_to_const (str);
        	copy_type_info (&(new_ports[j].ty), &(fg->nodes[id].outputs[0].ty));
        	new_ports[j+1].is_const = TRUE;
		new_ports[j+1].constval = copy_to_const ("true");
        	new_ports[j+1].ty.kind = Scalar;
        	new_ports[j+1].ty.type = Bool;
		}
	    break;
	*/
	case ND_REDUCE_AND_MACRO :
    	    and_constants (fg, id, str);
    	    if (c3 > 0)
        	{
		if (strcmp (str, "true") == 0)
		    {
		    n_in_pairs--;
		    }
		else if (strcmp (str, "false") == 0)
		    {
		    /* remove other incoming edges before deleting this node */
        	    SacFree (fg->nodes[id].inputs)
        	    fg->nodes[id].inputs = new_ports;
        	    fg->nodes[id].num_inputs = 2 * n_in_pairs;

		    for (i=0; i<fg->nodes[id].num_inputs; i++)
		        if (!fg->nodes[id].inputs[i].is_const)
			    {
			    remove_targets_to_this_port (fg, fg->nodes[id].inputs[i].back_edges, id, i);
			    free_edge_list (&(fg->nodes[id].inputs[i].back_edges));
			    }

		    deleted = propagate_and_delete (fg, id, "false", itlist);
		    }
		else
		    assert (0);
		}
	    break;
	case ND_REDUCE_OR_MACRO :
    	    or_constants (fg, id, str);
    	    if (c3 > 0)
        	{
		if (strcmp (str, "false") == 0)
		    {
		    n_in_pairs--;
		    }
		else if (strcmp (str, "true") == 0)
		    {
		    /* remove other incoming edges before deleting this node */
        	    SacFree (fg->nodes[id].inputs)
        	    fg->nodes[id].inputs = new_ports;
        	    fg->nodes[id].num_inputs = 2 * n_in_pairs;

		    for (i=0; i<fg->nodes[id].num_inputs; i++)
		        if (!fg->nodes[id].inputs[i].is_const)
			    {
			    remove_targets_to_this_port (fg, fg->nodes[id].inputs[i].back_edges, id, i);
			    free_edge_list (&(fg->nodes[id].inputs[i].back_edges));
			    }

		    deleted = propagate_and_delete (fg, id, "true", itlist);
		    }
		else
		    assert (0);
		}
	    break;
	case ND_REDUCE_MIN_MACRO :
    	    min_constants (fg, id, str);
    	    if (c3 > 0)
        	{
        	new_ports[j].is_const = TRUE;
		new_ports[j].constval = copy_to_const (str);
        	copy_type_info (&(new_ports[j].ty), &(fg->nodes[id].outputs[0].ty));
        	new_ports[j+1].is_const = TRUE;
		new_ports[j+1].constval = copy_to_const ("true");
        	new_ports[j+1].ty.kind = Scalar;
        	new_ports[j+1].ty.type = Bool;
		}
	    break;
	case ND_REDUCE_MAX_MACRO :
    	    max_constants (fg, id, str);
    	    if (c3 > 0)
        	{
        	new_ports[j].is_const = TRUE;
		new_ports[j].constval = copy_to_const (str);
        	copy_type_info (&(new_ports[j].ty), &(fg->nodes[id].outputs[0].ty));
        	new_ports[j+1].is_const = TRUE;
		new_ports[j+1].constval = copy_to_const ("true");
        	new_ports[j+1].ty.kind = Scalar;
        	new_ports[j+1].ty.type = Bool;
		}
	    break;
	default :
	    assert (0);
	}

    if (!deleted)
        {
        SacFree (fg->nodes[id].inputs)
        fg->nodes[id].inputs = new_ports;
        fg->nodes[id].num_inputs = 2 * n_in_pairs;
	}
    else
        SacFree (new_ports)

    return changed;
    }

void sum_constants (FuncGraph *fg, int id, char *str)
    {
    int i;

    switch (fg->nodes[id].outputs[0].ty.type)
        {
	case Uint :
	    {
	    unsigned int v0, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			sscanf (fg->nodes[id].inputs[i].constval, "%u", &v0);
			accum += v0;
			}

	    sprintf (str, "%u", accum);
	    }
	    break;
	case Int :
	    {
	    int v0, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			sscanf (fg->nodes[id].inputs[i].constval, "%d", &v0);
			accum += v0;
			}

	    sprintf (str, "%d", accum);
	    }
	    break;
	case Ufix :
	    {
	    unsigned int v0, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			v0 = val_from_hex_string (fg->nodes[id].inputs[i].constval);
			accum += v0;
			}

	    sprintf (str, "0x%x", accum);
	    }
	    break;
	case Fix :
	    {
	    int v0, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			v0 = val_from_hex_string (fg->nodes[id].inputs[i].constval);
			accum += v0;
			}

	    accum = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (str, "0x%x", accum);
	    }
	    break;
	case Float :
	    {
	    float v0, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			sscanf (fg->nodes[id].inputs[i].constval, "%f", &v0);
			accum += v0;
			}

	    sprintf (str, "%f", accum);
	    }
	    break;
	case Double :
	    {
	    double v0, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			sscanf (fg->nodes[id].inputs[i].constval, "%lf", &v0);
			accum += v0;
			}

	    sprintf (str, "%f", accum);
	    }
	    break;
	default :
            break;
	}
    }

void and_constants (FuncGraph *fg, int id, char *str)
    {
    int i, accum;

    for (accum=TRUE, i=0; i<fg->nodes[id].num_inputs; i+=2)
        if ((fg->nodes[id].inputs[i+1].is_const) &&
			(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
	    if (fg->nodes[id].inputs[i].is_const)
	        {
		if (strcmp (fg->nodes[id].inputs[i].constval, "false") == 0)
		    accum = FALSE;
		}

    if (accum)
        strcpy (str, "true");
    else
        strcpy (str, "false");
    }

void or_constants (FuncGraph *fg, int id, char *str)
    {
    int i, accum;

    for (accum=FALSE, i=0; i<fg->nodes[id].num_inputs; i+=2)
        if ((fg->nodes[id].inputs[i+1].is_const) &&
			(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
	    if (fg->nodes[id].inputs[i].is_const)
	        {
		if (strcmp (fg->nodes[id].inputs[i].constval, "true") == 0)
		    accum = TRUE;
		}

    if (accum)
        strcpy (str, "true");
    else
        strcpy (str, "false");
    }

void min_constants (FuncGraph *fg, int id, char *str)
    {
    int i;

    switch (fg->nodes[id].outputs[0].ty.type)
        {
	case Uint :
	    {
	    unsigned int v0, accum;

	    for (accum=UINT_MAX, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			sscanf (fg->nodes[id].inputs[i].constval, "%u", &v0);
			if (v0 < accum)
			    accum = v0;
			}

	    sprintf (str, "%u", accum);
	    }
	    break;
	case Int :
	    {
	    int v0, accum;

	    for (accum=INT_MAX, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			sscanf (fg->nodes[id].inputs[i].constval, "%d", &v0);
			if (v0 < accum)
			    accum = v0;
			}

	    sprintf (str, "%d", accum);
	    }
	    break;
	case Ufix :
	    {
	    unsigned int v0, accum;

	    for (accum=UINT_MAX, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			v0 = val_from_hex_string (fg->nodes[id].inputs[i].constval);
			if (v0 < accum)
			    accum = v0;
			}

	    sprintf (str, "0x%x", accum);
	    }
	    break;
	case Fix :
	    {
	    int v0, accum;

	    for (accum=INT_MAX, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			v0 = val_from_hex_string (fg->nodes[id].inputs[i].constval);
			if (v0 < accum)
			    accum = v0;
			}

	    accum = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (str, "0x%x", accum);
	    }
	    break;
	case Float :
	    {
	    float v0, accum;

	    for (accum=FLT_MAX, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			sscanf (fg->nodes[id].inputs[i].constval, "%f", &v0);
			if (v0 < accum)
			    accum = v0;
			}

	    sprintf (str, "%f", accum);
	    }
	    break;
	case Double :
	    {
	    double v0, accum;

	    for (accum=DBL_MAX, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			sscanf (fg->nodes[id].inputs[i].constval, "%lf", &v0);
			if (v0 < accum)
			    accum = v0;
			}

	    sprintf (str, "%f", accum);
	    }
	    break;
	default :
            break;
	}
    }

void max_constants (FuncGraph *fg, int id, char *str)
    {
    int i;

    switch (fg->nodes[id].outputs[0].ty.type)
        {
	case Uint :
	    {
	    unsigned int v0, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			sscanf (fg->nodes[id].inputs[i].constval, "%u", &v0);
			if (v0 > accum)
			    accum = v0;
			}

	    sprintf (str, "%u", accum);
	    }
	    break;
	case Int :
	    {
	    int v0, accum;

	    for (accum=INT_MIN, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			sscanf (fg->nodes[id].inputs[i].constval, "%d", &v0);
			if (v0 > accum)
			    accum = v0;
			}

	    sprintf (str, "%d", accum);
	    }
	    break;
	case Ufix :
	    {
	    unsigned int v0, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			v0 = val_from_hex_string (fg->nodes[id].inputs[i].constval);
			if (v0 > accum)
			    accum = v0;
			}

	    sprintf (str, "0x%x", accum);
	    }
	    break;
	case Fix :
	    {
	    int v0, accum;

	    for (accum=INT_MIN, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			v0 = val_from_hex_string (fg->nodes[id].inputs[i].constval);
			if (v0 > accum)
			    accum = v0;
			}

	    accum = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (str, "0x%x", accum);
	    }
	    break;
	case Float :
	    {
	    float v0, accum;

	    for (accum=FLT_MIN, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			sscanf (fg->nodes[id].inputs[i].constval, "%f", &v0);
			if (v0 > accum)
			    accum = v0;
			}

	    sprintf (str, "%f", accum);
	    }
	    break;
	case Double :
	    {
	    double v0, accum;

	    for (accum=DBL_MIN, i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if ((fg->nodes[id].inputs[i+1].is_const) &&
				(strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0))
		    if (fg->nodes[id].inputs[i].is_const)
		        {
			sscanf (fg->nodes[id].inputs[i].constval, "%lf", &v0);
			if (v0 > accum)
			    accum = v0;
			}

	    sprintf (str, "%f", accum);
	    }
	    break;
	default :
            break;
	}
    }

int fold_sum_macro (FuncGraph *fg, int id, IntList **itlist)
    {
    char op0[128], res[128];
    int changed, i;

    switch (fg->nodes[id].outputs[0].ty.type)
        {
	case Uint :
	    {
	    unsigned int v0, val, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%u", &v0);
		    accum += v0;
		    }

	    val = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "%u", val);
	    }
	    break;
	case Int :
	    {
	    int v0, val, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%d", &v0);
		    accum += v0;
		    }

	    val = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    val = sign_extend (val, fg->nodes[id].outputs[0].ty.totsize, 32);
	    sprintf (res, "%d", val);
	    }
	    break;
	case Ufix :
	    {
	    unsigned int v0, val, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    v0 = val_from_hex_string (op0);
		    accum += v0;
		    }

	    val = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    }
	    break;
	case Fix :
	    {
	    int v0, val, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    v0 = val_from_hex_string (op0);
		    accum += v0;
		    }

	    val = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    }
	    break;
	case Float :
	    {
	    float v0, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%f", &v0);
		    accum += v0;
		    }

	    sprintf (res, "%f", accum);
	    }
	    break;
	case Double :
	    {
	    double v0, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%lf", &v0);
		    accum += v0;
		    }

	    sprintf (res, "%f", accum);
	    }
	    break;
	default :
	    assert (0);
	}

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

int fold_product_macro (FuncGraph *fg, int id, IntList **itlist)
    {
    char op0[128], res[128];
    int changed, i;

    switch (fg->nodes[id].outputs[0].ty.type)
        {
	case Uint :
	    {
	    unsigned int v0, val, accum;

	    for (accum=1, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%u", &v0);
		    accum *= v0;
		    }

	    val = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "%u", val);
	    }
	    break;
	case Int :
	    {
	    int v0, val, accum;

	    for (accum=1, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%d", &v0);
		    accum *= v0;
		    }

	    val = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    val = sign_extend (val, fg->nodes[id].outputs[0].ty.totsize, 32);
	    sprintf (res, "%d", val);
	    }
	    break;
	case Ufix :
	case Fix :
	    {
	    int v0, accum;
	    int totsize, fracsize, rsize;

	    totsize = fg->nodes[id].outputs[0].ty.totsize;
	    fracsize = fg->nodes[id].outputs[0].ty.fracsize;
	    rsize = 32 - fracsize;
	    accum = 1<<fracsize;

	    for (i=0; i<fg->nodes[id].num_inputs; i+=2)
	        if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    assert (type_equal (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty)));

	    	    switch (fg->nodes[id].inputs[i].ty.type)
	        	{
			case Ufix :
		    	    v0 = val_from_hex_string (fg->nodes[id].inputs[i].constval);
		    	    break;
			case Fix :
		    	    v0 = val_from_hex_string (fg->nodes[id].inputs[i].constval);
		    	    v0 = sign_extend (v0, totsize, 32);
		    	    break;
			default :
                            break;
			}

		    if (totsize > rsize)
		        {
		        Int64 tmp = (Int64)v0*accum;
	                accum = tmp>>fracsize;
		        if (fg->nodes[id].outputs[0].ty.type == Fix)
		            accum = sign_extend (accum, rsize, totsize);
		        }
		    else
	                accum = (((Int64)v0*accum)>>fracsize)&mask_ones[totsize];
		    }

	    accum &= mask_ones[totsize];
	    sprintf (res, "0x%x", accum);
	    }
	    break;
	case Float :
	    {
	    float v0, accum;

	    for (accum=1, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%f", &v0);
		    accum *= v0;
		    }

	    sprintf (res, "%f", accum);
	    }
	    break;
	case Double :
	    {
	    double v0, accum;

	    for (accum=1, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%lf", &v0);
		    accum *= v0;
		    }

	    sprintf (res, "%f", accum);
	    }
	    break;
	default :
	    assert (0);
	}

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

int fold_and_macro (FuncGraph *fg, int id, IntList **itlist)
    {
    char res[128];
    int changed, i;
    unsigned int accum;

    for (accum=TRUE, i=0; i<fg->nodes[id].num_inputs; i+=2)
	if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
	    if (strcmp (fg->nodes[id].inputs[i].constval, "false") == 0)
	        {
	        accum = FALSE;
	        break;
	        }

    if (accum)
        strcpy (res, "true");
    else
        strcpy (res, "false");

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

int fold_or_macro (FuncGraph *fg, int id, IntList **itlist)
    {
    char res[128];
    int changed, i;
    unsigned int accum;

    for (accum=FALSE, i=0; i<fg->nodes[id].num_inputs; i+=2)
	if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
	    if (strcmp (fg->nodes[id].inputs[i].constval, "true") == 0)
	        {
	        accum = TRUE;
	        break;
	        }

    if (accum)
        strcpy (res, "true");
    else
        strcpy (res, "false");

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

int fold_min_macro (FuncGraph *fg, int id, IntList **itlist)
    {
    char op0[128], res[128];
    int changed, i;

    switch (fg->nodes[id].outputs[0].ty.type)
        {
	case Uint :
	    {
	    unsigned int v0, val, accum;

	    for (accum=UINT_MAX, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%u", &v0);
		    if (v0 < accum)
		        accum = v0;
		    }

	    val = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "%u", val);
	    }
	    break;
	case Int :
	    {
	    int v0, val, accum;

	    for (accum=INT_MAX, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%d", &v0);
		    if (v0 < accum)
		        accum = v0;
		    }

	    val = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    val = sign_extend (val, fg->nodes[id].outputs[0].ty.totsize, 32);
	    sprintf (res, "%d", val);
	    }
	    break;
	case Ufix :
	    {
	    unsigned int v0, val, accum;

	    for (accum=UINT_MAX, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    v0 = val_from_hex_string (op0);
		    if (v0 < accum)
		        accum = v0;
		    }

	    val = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    }
	    break;
	case Fix :
	    {
	    int v0, val, accum;

	    for (accum=INT_MAX, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    v0 = val_from_hex_string (op0);
		    if (v0 < accum)
		        accum = v0;
		    }

	    val = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    }
	    break;
	case Float :
	    {
	    float v0, accum;

	    for (accum=FLT_MAX, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%f", &v0);
		    if (v0 < accum)
		        accum = v0;
		    }

	    sprintf (res, "%f", accum);
	    }
	    break;
	case Double :
	    {
	    double v0, accum;

	    for (accum=DBL_MAX, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%lf", &v0);
		    if (v0 < accum)
		        accum = v0;
		    }

	    sprintf (res, "%f", accum);
	    }
	    break;
	default :
	    assert (0);
	}

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

int fold_max_macro (FuncGraph *fg, int id, IntList **itlist)
    {
    char op0[128], res[128];
    int changed, i;

    switch (fg->nodes[id].outputs[0].ty.type)
        {
	case Uint :
	    {
	    unsigned int v0, val, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%u", &v0);
		    if (v0 > accum)
		        accum = v0;
		    }

	    val = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "%u", val);
	    }
	    break;
	case Int :
	    {
	    int v0, val, accum;

	    for (accum=INT_MIN, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%d", &v0);
		    if (v0 > accum)
		        accum = v0;
		    }

	    val = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    val = sign_extend (val, fg->nodes[id].outputs[0].ty.totsize, 32);
	    sprintf (res, "%d", val);
	    }
	    break;
	case Ufix :
	    {
	    unsigned int v0, val, accum;

	    for (accum=0, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    v0 = val_from_hex_string (op0);
		    if (v0 > accum)
		        accum = v0;
		    }

	    val = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    }
	    break;
	case Fix :
	    {
	    int v0, val, accum;

	    for (accum=INT_MIN, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    v0 = val_from_hex_string (op0);
		    if (v0 > accum)
		        accum = v0;
		    }

	    val = accum & mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    }
	    break;
	case Float :
	    {
	    float v0, accum;

	    for (accum=FLT_MIN, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%f", &v0);
		    if (v0 > accum)
		        accum = v0;
		    }

	    sprintf (res, "%f", accum);
	    }
	    break;
	case Double :
	    {
	    double v0, accum;

	    for (accum=DBL_MIN, i=0; i<fg->nodes[id].num_inputs; i+=2)
		if (strcmp (fg->nodes[id].inputs[i+1].constval, "true") == 0)
		    {
		    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[i].ty),
		    		op0, fg->nodes[id].inputs[i].constval);
		    sscanf (op0, "%lf", &v0);
		    if (v0 > accum)
		        accum = v0;
		    }

	    sprintf (res, "%f", accum);
	    }
	    break;
	default :
	    assert (0);
	}

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

int fold_add (FuncGraph *fg, int id, IntList **itlist)
    {
    char op0[128], op1[128], res[128];
    int changed;

    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[0].ty),
    				op0, fg->nodes[id].inputs[0].constval);
    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[1].ty),
    				op1, fg->nodes[id].inputs[1].constval);

    switch (fg->nodes[id].outputs[0].ty.type)
        {
	case Uint :
	    {
	    unsigned int v0, v1, val;
	    sscanf (op0, "%u", &v0);
	    sscanf (op1, "%u", &v1);
	    val = (v0+v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "%u", val);
	    }
	    break;
	case Int :
	    {
	    int v0, v1, val;
	    sscanf (op0, "%d", &v0);
	    sscanf (op1, "%d", &v1);
	    val = v0+v1;
	    val = val&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    val = sign_extend (val, fg->nodes[id].outputs[0].ty.totsize, 32);
	    sprintf (res, "%d", val);
	    }
	    break;
	case Ufix :
	    {
	    unsigned int v0, v1, val;
	    v0 = val_from_hex_string (op0);
	    v1 = val_from_hex_string (op1);
	    val = (v0+v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    }
	    break;
	case Fix :
	    {
	    int v0, v1, val;
	    v0 = val_from_hex_string (op0);
	    v1 = val_from_hex_string (op1);
	    val = (v0+v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    }
	    break;
	case Float :
	    {
	    float v0, v1, val;
	    sscanf (op0, "%f", &v0);
	    sscanf (op1, "%f", &v1);
	    val = v0+v1;
	    sprintf (res, "%f", val);
	    }
	    break;
	case Double :
	    {
	    double v0, v1, val;
	    sscanf (op0, "%lf", &v0);
	    sscanf (op1, "%lf", &v1);
	    val = v0+v1;
	    sprintf (res, "%f", val);
	    }
	    break;
	default :
	    assert (0);
	}

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

int fold_sub (FuncGraph *fg, int id, IntList **itlist)
    {
    char op0[128], op1[128], res[128];
    int changed;

    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[0].ty),
    				op0, fg->nodes[id].inputs[0].constval);
    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[1].ty),
    				op1, fg->nodes[id].inputs[1].constval);

    switch (fg->nodes[id].outputs[0].ty.type)
        {
	case Uint :
	    {
	    unsigned int v0, v1, val;
	    sscanf (op0, "%u", &v0);
	    sscanf (op1, "%u", &v1);
	    val = (v0-v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "%u", val);
	    }
	    break;
	case Int :
	    {
	    int v0, v1, val;
	    sscanf (op0, "%d", &v0);
	    sscanf (op1, "%d", &v1);
	    val = (v0-v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    val = sign_extend (val, fg->nodes[id].outputs[0].ty.totsize, 32);
	    sprintf (res, "%d", val);
	    }
	    break;
	case Ufix :
	    {
	    unsigned int v0, v1, val;
	    v0 = val_from_hex_string (op0);
	    v1 = val_from_hex_string (op1);
	    val = (v0-v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    }
	    break;
	case Fix :
	    {
	    int v0, v1, val;
	    v0 = val_from_hex_string (op0);
	    v1 = val_from_hex_string (op1);
	    val = (v0-v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    }
	    break;
	case Float :
	    {
	    float v0, v1, val;
	    sscanf (op0, "%f", &v0);
	    sscanf (op1, "%f", &v1);
	    val = v0-v1;
	    sprintf (res, "%f", val);
	    }
	    break;
	case Double :
	    {
	    double v0, v1, val;
	    sscanf (op0, "%lf", &v0);
	    sscanf (op1, "%lf", &v1);
	    val = v0-v1;
	    sprintf (res, "%f", val);
	    }
	    break;
	default :
	    assert (0);
	}

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

int fold_mul (FuncGraph *fg, int id, IntList **itlist)
    {
    char op0[128], op1[128], res[128];
    int changed;

    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[0].ty),
    				op0, fg->nodes[id].inputs[0].constval);
    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[1].ty),
    				op1, fg->nodes[id].inputs[1].constval);

    switch (fg->nodes[id].outputs[0].ty.type)
        {
	case Uint :
	    {
	    unsigned int v0, v1, val;
	    sscanf (op0, "%u", &v0);
	    sscanf (op1, "%u", &v1);
	    val = (v0*v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "%u", val);
	    }
	    break;
	case Int :
	    {
	    int v0, v1, val;
	    sscanf (op0, "%d", &v0);
	    sscanf (op1, "%d", &v1);
	    val = (v0*v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    val = sign_extend (val, fg->nodes[id].outputs[0].ty.totsize, 32);
	    sprintf (res, "%d", val);
	    }
	    break;
	case Ufix :
	case Fix :
	    /*  for fix and ufix multiplication, we ignore the casts into
	     *  'op0' and 'op1', so things are more complicated; we have to
	     *  allow for either operand to be Uint, Int, Ufix or Fix
	     */
	    {
	    int v0, v1, val;
	    int totsize, dist, rsize;

	    switch (fg->nodes[id].inputs[0].ty.type)
	        {
		case Uint :
		case Int :
		    sscanf (fg->nodes[id].inputs[0].constval, "%u", &v0);
		    break;
		case Ufix :
		    v0 = val_from_hex_string (fg->nodes[id].inputs[0].constval);
		    break;
		case Fix :
		    v0 = val_from_hex_string (fg->nodes[id].inputs[0].constval);
		    v0 = sign_extend (v0, fg->nodes[id].inputs[0].ty.totsize, 32);
		    break;
		default :
                    break;
		}

	    switch (fg->nodes[id].inputs[1].ty.type)
	        {
		case Uint :
		case Int :
		    sscanf (fg->nodes[id].inputs[1].constval, "%u", &v1);
		    break;
		case Ufix :
		    v1 = val_from_hex_string (fg->nodes[id].inputs[1].constval);
		    break;
		case Fix :
		    v1 = val_from_hex_string (fg->nodes[id].inputs[1].constval);
		    v1 = sign_extend (v1, fg->nodes[id].inputs[1].ty.totsize, 32);
		    break;
		default :
                    break;
		}

	    totsize = fg->nodes[id].outputs[0].ty.totsize;
	    dist = fg->nodes[id].inputs[0].ty.fracsize +
	    		fg->nodes[id].inputs[1].ty.fracsize -
			fg->nodes[id].outputs[0].ty.fracsize;
	    rsize = 32 - dist;

	    if (dist >= 0)
		if (totsize > rsize)
		    {
		    Int64 tmp = (Int64)v0*v1;
	            val = tmp>>dist;
		    if (fg->nodes[id].outputs[0].ty.type == Fix)
		        val = sign_extend (val, rsize, totsize);
		    }
		else
	            val = (((Int64)v0*v1)>>dist)&mask_ones[totsize];
	    else
	        val = (((Uint64)v0*v1)<<(-dist))&mask_ones[totsize];

	    val &= mask_ones[totsize];
	    sprintf (res, "0x%x", val);
	    }
	    break;
	case Float :
	    {
	    float v0, v1, val;
	    sscanf (op0, "%f", &v0);
	    sscanf (op1, "%f", &v1);
	    val = v0*v1;
	    sprintf (res, "%20.10e", val);
	    }
	    break;
	case Double :
	    {
	    double v0, v1, val;
	    sscanf (op0, "%lf", &v0);
	    sscanf (op1, "%lf", &v1);
	    val = v0*v1;
	    sprintf (res, "%30.20g", val);
	    }
	    break;
	default :
	    assert (0);
	}

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

int fold_div (FuncGraph *fg, int id, IntList **itlist)
    {
    char op0[128], op1[128], res[128];
    int changed;

    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[0].ty),
    				op0, fg->nodes[id].inputs[0].constval);
    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[1].ty),
    				op1, fg->nodes[id].inputs[1].constval);

    switch (fg->nodes[id].outputs[0].ty.type)
        {
	case Uint :
	    {
	    unsigned int v0, v1, val;
	    sscanf (op0, "%u", &v0);
	    sscanf (op1, "%u", &v1);
	    val = (v0/v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "%u", val);
	    }
	    break;
	case Int :
	    {
	    int v0, v1, val;
	    sscanf (op0, "%d", &v0);
	    sscanf (op1, "%d", &v1);
	    val = (v0/v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    val = sign_extend (val, fg->nodes[id].outputs[0].ty.totsize, 32);
	    sprintf (res, "%d", val);
	    }
	    break;
	case Ufix :
	case Fix :
	    /*  for fix and ufix division, we ignore the casts into
	     *  'op0' and 'op1', so things are more complicated; we have to
	     *  allow for either operand to be Uint, Int, Ufix or Fix
	     */
	    {
	    int v0, v1, val;
	    int totsize, dist;

	    switch (fg->nodes[id].inputs[0].ty.type)
	        {
		case Uint :
		case Int :
		    sscanf (fg->nodes[id].inputs[0].constval, "%u", &v0);
		    break;
		case Ufix :
		    v0 = val_from_hex_string (fg->nodes[id].inputs[0].constval);
		    break;
		case Fix :
		    v0 = val_from_hex_string (fg->nodes[id].inputs[0].constval);
		    v0 = sign_extend (v0, fg->nodes[id].inputs[0].ty.totsize, 32);
		    break;
		default :
                    break;
		}

	    switch (fg->nodes[id].inputs[1].ty.type)
	        {
		case Uint :
		case Int :
		    sscanf (fg->nodes[id].inputs[1].constval, "%u", &v1);
		    break;
		case Ufix :
		    v1 = val_from_hex_string (fg->nodes[id].inputs[1].constval);
		    break;
		case Fix :
		    v1 = val_from_hex_string (fg->nodes[id].inputs[1].constval);
		    v1 = sign_extend (v1, fg->nodes[id].inputs[1].ty.totsize, 32);
		    break;
		default :
                    break;
		}

	    totsize = fg->nodes[id].outputs[0].ty.totsize;

	    /* calc left shift dist for numerator */
	    dist = fg->nodes[id].inputs[1].ty.fracsize +
			fg->nodes[id].outputs[0].ty.fracsize -
			fg->nodes[id].inputs[0].ty.fracsize;

	    if (dist >= 0)
	        {
		Int64 tmp = (Int64)v0<<dist;
		val = tmp/v1;
		}
	    else
	        {
		v0 = v0>>(-dist);
		val = v0/v1;
		}

	    val &= mask_ones[totsize];
	    sprintf (res, "0x%x", val);
	    }
	    break;
	case Float :
	    {
	    float v0, v1, val;
	    sscanf (op0, "%f", &v0);
	    sscanf (op1, "%f", &v1);
	    val = v0/v1;
	    sprintf (res, "%20.10e", val);
	    }
	    break;
	case Double :
	    {
	    double v0, v1, val;
	    sscanf (op0, "%lf", &v0);
	    sscanf (op1, "%lf", &v1);
	    val = v0/v1;
	    sprintf (res, "%30.20g", val);
	    }
	    break;
	default :
	    assert (0);
	}

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

int fold_mod (FuncGraph *fg, int id, IntList **itlist)
    {
    char op0[128], op1[128], res[128];
    int changed, v0, v1;

    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[0].ty),
    				op0, fg->nodes[id].inputs[0].constval);
    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[1].ty),
    				op1, fg->nodes[id].inputs[1].constval);

    sscanf (op0, "%d", &v0);
    sscanf (op1, "%d", &v1);
    v0 = (v0%v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
    sprintf (res, "%d", v0);

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

int fold_cmp (FuncGraph *fg, int id, IntList **itlist, DdcfType ndty)
    {
    char res[128], *op0, *op1;
    double dv0, dv1;
    int changed;

    op0 = fg->nodes[id].inputs[0].constval;
    op1 = fg->nodes[id].inputs[1].constval;

    switch (fg->nodes[id].inputs[0].ty.type)
        {
	case Uint :
	    {
	    unsigned int v0;
	    sscanf (op0, "%u", &v0);
	    dv0 = v0;
	    }
	    break;
	case Int :
	    {
	    int v0;
	    sscanf (op0, "%d", &v0);
	    dv0 = v0;
	    }
	    break;
	case Ufix :
	    {
	    unsigned int v0;
	    v0 = val_from_hex_string (op0);
	    dv0 = v0/sc_pow2[fg->nodes[id].inputs[0].ty.fracsize];
	    }
	    break;
	case Fix :
	    {
	    int v0;
	    v0 = val_from_hex_string (op0);
	    v0 = sign_extend (v0, fg->nodes[id].inputs[0].ty.totsize, 32);
	    dv0 = v0/sc_pow2[fg->nodes[id].inputs[0].ty.fracsize];
	    }
	    break;
	case Float :
	    {
	    float v0;
	    sscanf (op0, "%f", &v0);
	    dv0 = v0;
	    }
	    break;
	case Double :
	    sscanf (op0, "%lf", &dv0);
	    break;
	case Bits :
	    {
	    unsigned int v0;
	    sscanf (op0, "%x", &v0);
	    dv0 = v0;
	    }
	    break;
	case Bool :
	    if (strcmp (op0, "true") == 0)
	        dv0 = 1.0;
	    else if (strcmp (op0, "false") == 0)
	        dv0 = 0.0;
	    else
	        assert (0);
	    break;
	default :
	    assert (0);
	}

    switch (fg->nodes[id].inputs[1].ty.type)
        {
	case Uint :
	    {
	    unsigned int v1;
	    sscanf (op1, "%u", &v1);
	    dv1 = v1;
	    }
	    break;
	case Int :
	    {
	    int v1;
	    sscanf (op1, "%d", &v1);
	    dv1 = v1;
	    }
	    break;
	case Ufix :
	    {
	    unsigned int v1;
	    v1 = val_from_hex_string (op1);
	    dv1 = v1/sc_pow2[fg->nodes[id].inputs[1].ty.fracsize];
	    }
	    break;
	case Fix :
	    {
	    int v1;
	    v1 = val_from_hex_string (op1);
	    v1 = sign_extend (v1, fg->nodes[id].inputs[1].ty.totsize, 32);
	    dv1 = v1/sc_pow2[fg->nodes[id].inputs[1].ty.fracsize];
	    }
	    break;
	case Float :
	    {
	    float v1;
	    sscanf (op1, "%f", &v1);
	    dv1 = v1;
	    }
	    break;
	case Double :
	    sscanf (op1, "%lf", &dv1);
	    break;
	case Bits :
	    {
	    unsigned int v1;
	    sscanf (op1, "%x", &v1);
	    dv1 = v1;
	    }
	    break;
	case Bool :
	    if (strcmp (op1, "true") == 0)
	        dv1 = 1.0;
	    else if (strcmp (op1, "false") == 0)
	        dv1 = 0.0;
	    else
	        assert (0);
	    break;
	default :
	    assert (0);
	}

    switch (ndty)
        {
	case ND_LT :
	    sprintf (res, "%s", dv0<dv1?"true":"false");
	    break;
	case ND_GT :
	    sprintf (res, "%s", dv0>dv1?"true":"false");
	    break;
	case ND_LE :
	    sprintf (res, "%s", dv0<=dv1?"true":"false");
	    break;
	case ND_GE :
	    sprintf (res, "%s", dv0>=dv1?"true":"false");
	    break;
	case ND_EQ :
	    sprintf (res, "%s", dv0==dv1?"true":"false");
	    break;
	case ND_NEQ :
	    sprintf (res, "%s", dv0!=dv1?"true":"false");
	    break;
	default :
            break;
	}

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

int fold_bool (FuncGraph *fg, int id, IntList **itlist, DdcfType ndty)
    {
    int v0, v1, changed;
    char res[128];

    v0 = strcmp (fg->nodes[id].inputs[0].constval, "false");

    switch (ndty)
        {
	case ND_AND :
    	    v1 = strcmp (fg->nodes[id].inputs[1].constval, "false");
	    sprintf (res, "%s", v0&&v1?"true":"false");
	    break;
	case ND_OR :
    	    v1 = strcmp (fg->nodes[id].inputs[1].constval, "false");
	    sprintf (res, "%s", v0||v1?"true":"false");
	    break;
	case ND_NOT :
	    sprintf (res, "%s", !v0?"true":"false");
	    break;
	default :
            break;
	}

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

int fold_bits (FuncGraph *fg, int id, IntList **itlist, DdcfType ndty)
    {
    int v0, v1, val, changed;
    char res[128];

    v0 = val_from_hex_string (fg->nodes[id].inputs[0].constval);
    v1 = val_from_hex_string (fg->nodes[id].inputs[1].constval);

    switch (ndty)
        {
	case ND_BIT_AND :
	    val = (v0&v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    break;
	case ND_BIT_OR :
	    val = (v0|v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    break;
	case ND_BIT_EOR :
	    val = (v0^v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    break;
	default :
            break;
	}

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

int fold_shift (FuncGraph *fg, int id, IntList **itlist, DdcfType ndty)
    {
    int v0, v1, val, changed;
    char res[128];

    v0 = val_from_hex_string (fg->nodes[id].inputs[0].constval);
    sscanf (fg->nodes[id].inputs[1].constval, "%d", &v1);

    switch (ndty)
        {
	case ND_LEFT_SHIFT_MACH :
	case ND_LEFT_SHIFT :
	    val = (v0<<v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    break;
	case ND_RIGHT_SHIFT :
	    val = (v0>>v1)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    break;
	default :
            break;
	}

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

int fold_neg (FuncGraph *fg, int id, IntList **itlist)
    {
    char op0[128], res[128];
    int changed;

    cf_cast (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[0].ty),
    				op0, fg->nodes[id].inputs[0].constval);

    switch (fg->nodes[id].outputs[0].ty.type)
        {
	case Uint :
	case Int :
	    {
	    int v0, val;
	    sscanf (op0, "%d", &v0);
	    val = -v0;
	    if (fg->nodes[id].outputs[0].ty.type == Int)
	        {
	        val = val&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	        val = sign_extend (val, fg->nodes[id].outputs[0].ty.totsize, 32);
		}
	    sprintf (res, "%d", val);
	    }
	    break;
	case Ufix :
	case Fix :
	    {
	    int v0, val;
	    v0 = val_from_hex_string (op0);
	    val = (-v0)&mask_ones[fg->nodes[id].outputs[0].ty.totsize];
	    sprintf (res, "0x%x", val);
	    }
	    break;
	case Float :
	    {
	    float v0, val;
	    sscanf (op0, "%f", &v0);
	    val = -v0;
	    sprintf (res, "%f", val);
	    }
	    break;
	case Double :
	    {
	    double v0, val;
	    sscanf (op0, "%lf", &v0);
	    val = -v0;
	    sprintf (res, "%f", val);
	    }
	    break;
	default :
	    assert (0);
	}

    changed = propagate_and_delete (fg, id, res, itlist);

    return changed;
    }

void cf_cast (TypeInfo *tgt, TypeInfo *srct, char *tg, char *src)
    {
    switch (srct->type)
        {
	case Uint :
	    cf_cast_from_uint (tgt, srct->totsize, tg, src);
	    break;
	case Int :
	    cf_cast_from_int (tgt, srct->totsize, tg, src);
	    break;
	case Ufix :
	    cf_cast_from_ufix (tgt, srct->totsize, srct->fracsize, tg, src);
	    break;
	case Fix :
	    cf_cast_from_fix (tgt, srct->totsize, srct->fracsize, tg, src);
	    break;
	case Bits :
	    cf_cast_from_bits (tgt, srct->totsize, tg, src);
	    break;
	case Bool :
	    cf_cast_from_bool (tgt, tg, src);
	    break;
	case Float :
	    cf_cast_from_float (tgt, tg, src);
	    break;
	case Double :
	    cf_cast_from_double (tgt, tg, src);
	    break;
	default :
            break;
	}
    }

void cf_cast_from_uint (TypeInfo *tgt, int totsize, char *tg, char *src)
    {
    unsigned int val;

    sscanf (src, "%u", &val);
    val &= mask_ones[totsize];

    switch (tgt->type)
        {
	case Uint :
	case Int:
	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "%u", val);
	    break;
	case Ufix :
	case Fix :
	    val = val<<tgt->fracsize;
	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", val);
	    break;
	case Bits :
	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", val);
	    break;
	case Bool :
	    if (val == 0)
	        sprintf (tg, "false");
	    else
	        sprintf (tg, "true");
	    break;
	case Float :
	case Double :
	    sprintf (tg, "%u.0", val);
	    break;
	default :
            break;
	}
    }

void cf_cast_from_int (TypeInfo *tgt, int totsize, char *tg, char *src)
    {
    int val;

    sscanf (src, "%d", &val);
    val &= mask_ones[totsize];
    val = sign_extend (val, totsize, 32);

    switch (tgt->type)
        {
	case Uint :
	case Int:
	    if (val >= 0)
	        val &= mask_ones[tgt->totsize];
	    else
	        {
	        val &= mask_ones[tgt->totsize];
		val = sign_extend (val, tgt->totsize, 32);
		}
	    sprintf (tg, "%d", val);
	    break;
	case Ufix :
	case Fix :
	    val = val<<tgt->fracsize;
	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", val);
	    break;
	case Bits :
	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", val);
	    break;
	case Bool :
	    if (val == 0)
	        sprintf (tg, "false");
	    else
	        sprintf (tg, "true");
	    break;
	case Float :
	case Double :
	    sprintf (tg, "%d.0", val);
	    break;
	default :
            break;
	}
    }

void cf_cast_from_ufix (TypeInfo *tgt, int totsize, int fracsize, char *tg, char *src)
    {
    int val, dist;

    val = val_from_hex_string (src);
    val &= mask_ones[totsize];

    switch (tgt->type)
        {
	case Uint :
	case Int:
	    val = val >> fracsize;
	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "%d", val);
	    break;
	case Ufix :
	case Fix :
	    dist = fracsize - tgt->fracsize;
	    if (dist > 0)
		val = val>>dist;
	    else if (dist < 0)
	        val = val<<(-dist);

	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", val);
	    break;
	case Bits :
	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", val);
	    break;
	case Bool :
	    if (val == 0)
	        sprintf (tg, "false");
	    else
	        sprintf (tg, "true");
	    break;
	case Float :
	    {
	    float fv = (float)val/sc_pow2[fracsize];
	    sprintf (tg, "%20.10e", fv);
	    }
	    break;
	case Double :
	    {
	    double fv = (double)val/sc_pow2[fracsize];
	    sprintf (tg, "%30.20g", fv);
	    }
	    break;
	default :
            break;
	}
    }

void cf_cast_from_fix (TypeInfo *tgt, int totsize, int fracsize, char *tg, char *src)
    {
    int val, dist;

    val = val_from_hex_string (src);
    val &= mask_ones[totsize];
    val = sign_extend (val, totsize, 32);

    switch (tgt->type)
        {
	case Uint :
	    val = val >> fracsize;
	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "%d", val);
	    break;
	case Int:
	    /* when we shift right, we bring in zeroes; these are harmless
	     * if they get masked off anyway, but otherwise we have to
	     * sign extend in case the number is negative
	     */
	    val = val >> fracsize;
	    val &= mask_ones[tgt->totsize];
	    if (tgt->totsize > totsize - fracsize)
		val = sign_extend (val, totsize-fracsize, tgt->totsize);
	    sprintf (tg, "%d", val);
	    break;
	case Ufix :
	    dist = fracsize - tgt->fracsize;
	    if (dist > 0)
		val = val>>dist;
	    else if (dist < 0)
	        val = val<<(-dist);

	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", val);
	    break;
	case Fix :
	    dist = fracsize - tgt->fracsize;
	    if (dist > 0)
		{
		val = val>>dist;
		val = sign_extend (val, totsize-dist, tgt->totsize);
		}
	    else if (dist < 0)
		{
	        val = val<<(-dist);
		val = sign_extend (val, totsize-dist, tgt->totsize);
		}

	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", val);
	    break;
	case Bits :
	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", val);
	    break;
	case Bool :
	    if (val == 0)
	        sprintf (tg, "false");
	    else
	        sprintf (tg, "true");
	    break;
	case Float :
	    {
	    float fv = (float)val/sc_pow2[fracsize];
	    sprintf (tg, "%20.10e", fv);
	    }
	    break;
	case Double :
	    {
	    double fv = (double)val/sc_pow2[fracsize];
	    sprintf (tg, "%30.20g", fv);
	    }
	    break;
	default :
            break;
	}
    }

void cf_cast_from_bits (TypeInfo *tgt, int totsize, char *tg, char *src)
    {
    int val;

    val = val_from_hex_string (src);
    val &= mask_ones[totsize];

    switch (tgt->type)
        {
	case Uint :
	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "%d", val);
	    break;
	case Int:
	    val = sign_extend (val, totsize, 32);
	    val &= mask_ones[tgt->totsize];
	    val = sign_extend (val, tgt->totsize, 32);
	    sprintf (tg, "%d", val);
	    break;
	case Ufix :
	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", val);
	    break;
	case Fix :
	    val = sign_extend (val, totsize, 32);
	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", val);
	    break;
	case Bits :
	    val &= mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", val);
	    break;
	case Bool :
	    if (val == 0)
	        sprintf (tg, "false");
	    else
	        sprintf (tg, "true");
	    break;
	case Float :
	case Double :
	    assert (0);
	default :
            break;
	}
    }

void cf_cast_from_bool (TypeInfo *tgt, char *tg, char *src)
    {
    int val;

    if (strcmp (src, "true") == 0)
        val = 1;
    else
        val = 0;

    switch (tgt->type)
        {
	case Uint :
	case Int:
	    if (val)
	        strcpy (tg, "1");
	    else
	        strcpy (tg, "0");
	    break;
	case Ufix :
	case Fix :
	case Bits :
	    if (val)
	        strcpy (tg, "0x1");
	    else
	        strcpy (tg, "0x0");
	    break;
	case Bool :
	    strcpy (tg, src);
	    break;
	case Float :
	case Double :
	    if (val)
	        strcpy (tg, "1.0");
	    else
	        strcpy (tg, "0.0");
	    break;
	default :
            break;
	}
    }

void cf_cast_from_float (TypeInfo *tgt, char *tg, char *src)
    {
    float val;
    int ival;
    unsigned int uval;

    sscanf (src, "%f", &val);

    switch (tgt->type)
        {
	case Uint :
	    uval = (unsigned int)val;
	    uval &= mask_ones[tgt->totsize];
	    sprintf (tg, "%u", uval);
	    break;
	case Int:
	    ival = (int)val;
	    ival &= mask_ones[tgt->totsize];
	    val = sign_extend (ival, tgt->totsize, 32);
	    sprintf (tg, "%d", ival);
	    break;
	case Ufix :
	    val *= sc_pow2[tgt->fracsize];
	    uval = (unsigned int)val & mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", uval);
	    break;
	case Fix :
	    val *= sc_pow2[tgt->fracsize];
	    ival = (int)val & mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", ival);
	    break;
	case Bits :
	    assert (0);
	    break;
	case Bool :
	    if (val == 1.0)
	        strcpy (tg, "true");
	    else
	        strcpy (tg, "false");
	    break;
	case Float :
	case Double :
	    strcpy (tg, src);
	    break;
	default :
            break;
	}
    }

void cf_cast_from_double (TypeInfo *tgt, char *tg, char *src)
    {
    double val;
    int ival;
    unsigned int uval;

    sscanf (src, "%lf", &val);

    switch (tgt->type)
        {
	case Uint :
	    uval = (unsigned int)val;
	    uval &= mask_ones[tgt->totsize];
	    sprintf (tg, "%u", uval);
	    break;
	case Int:
	    ival = (int)val;
	    ival &= mask_ones[tgt->totsize];
	    val = sign_extend (ival, tgt->totsize, 32);
	    sprintf (tg, "%d", ival);
	    break;
	case Ufix :
	    val *= sc_pow2[tgt->fracsize];
	    uval = (unsigned int)val & mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", uval);
	    break;
	case Fix :
	    val *= sc_pow2[tgt->fracsize];
	    ival = (int)val & mask_ones[tgt->totsize];
	    sprintf (tg, "0x%x", ival);
	    break;
	case Bits :
	    assert (0);
	    break;
	    break;
	case Bool :
	    if (val == 1.0)
	        strcpy (tg, "true");
	    else
	        strcpy (tg, "false");
	    break;
	case Float :
	    {
	    float fv;
	    sscanf (src, "%f", &fv);
	    sprintf (tg, "%20.10e", fv);
	    }
	    break;
	case Double :
	    strcpy (tg, src);
	    break;
	default :
            break;
	}
    }
