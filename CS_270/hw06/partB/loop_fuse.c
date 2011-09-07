#include "sassy.h"
#include "error.h"
#include "ddcf.h"

int count_generators (FuncGraph*, int);
void srch_loop_fuse_graph (FuncGraph*, int);
void fuse_two_loops (FuncGraph*, int, int, IntList**);
int is_fusable (FuncGraph*, int, int);
int find_lower_gen (FuncGraph*, int, int);
int build_gens_from_upper_loop (FuncGraph*, int, int, int, int, int);
int copy_body_and_returns (FuncGraph*, int, int, int);
void convert_elegens_to_window_gens (FuncGraph*, int, int);
void elegen_to_window (FuncGraph*, int, int, int);
void clear_dim_sizes_upper_loop (FuncGraph*, int, int);
void clear_dim_sizes (TypeInfo*);
void dead_code_graph (FuncGraph*, int);

void loop_fuse ()
    {
    FuncGraph *fg;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
	if (fg->nodes_used > 0)
            srch_loop_fuse_graph (fg, 0);
    }

void srch_loop_fuse_graph (FuncGraph *fg, int nd)
    {
    IntList *it, *itt, *list_copy;
    int did_a_fuse, id;

    for (it=fg->nodes[nd].My_nodes; it!=NULL; it=it->link)
        if (is_compound (fg->nodes[it->val].nodetype))
	    srch_loop_fuse_graph (fg, it->val);

    do  {
	did_a_fuse = FALSE;

        for (it=fg->nodes[nd].My_nodes; it!=NULL; it=it->link)
            {
	    id = it->val;

	    if (fg->nodes[id].nodetype == ND_FORALL)
	        for (itt=it->link; itt!=NULL; itt=itt->link)
	            if (fg->nodes[itt->val].nodetype == ND_FORALL)
		        if (is_fusable (fg, id, itt->val))
			    {
			    copy_intlist (&list_copy, &(fg->nodes[nd].My_nodes));
		            fuse_two_loops (fg, id, itt->val, &list_copy);
			    did_a_fuse = TRUE;
			    free_intlist (&(fg->nodes[nd].My_nodes));
			    fg->nodes[nd].My_nodes = list_copy;
			    dead_code_graph (fg, 0);
			    break;
			    }

	    if (did_a_fuse)
		{
		/*
		show_ddcf_func (fg, stdout, FALSE);
		printf ("-----------------------------------------------\n");
		if (ddcf_consistency ())
		    {
		    printf ("inconsistent after loop fuse\n");
		    exit (1);
		    }
		*/
	        break;
		}
	    }
	} while (did_a_fuse);
    }

void fuse_two_loops (FuncGraph *fg, int upper_loop, int lower_loop, IntList **list_copy)
    {
    Edge *tg;
    int num_inputs, num_outputs, new_loop, upper_gen, lower_gen, lower_win, gin, gout, i, j, k, m;
    int nd, pt, num_inputs_dot, num_outputs_dot, new_dot, tmp_dot, ngin;
    int ogin, rank, gout_index, new_lower, n_upper_gens, n_lower_gens, special_input;

    is_stable = FALSE;

    if (!quiet)
        printf ("  fusing loops from lines %d and %d in function '%s', file '%s'\n", 
			fg->nodes[upper_loop].loc.line, fg->nodes[lower_loop].loc.line,
			fg->nodes[upper_loop].loc.func, fg->nodes[upper_loop].loc.file);

    /* remove any size info from the output of the upper loop */
    gout = fg->nodes[upper_loop].My_outputs[0];
    clear_dim_sizes (&(fg->nodes[gout].inputs[0].ty));
    clear_dim_sizes (&(fg->nodes[upper_loop].outputs[0].ty));
    nd = fg->nodes[gout].inputs[0].back_edges->node;
    pt = fg->nodes[gout].inputs[0].back_edges->port;
    clear_dim_sizes (&(fg->nodes[nd].outputs[pt].ty));
    for (i=0; i<MAXRANK; i++)
        fg->nodes[upper_loop].dim_sizes[i] = -1;

    convert_elegens_to_window_gens (fg, upper_loop, lower_loop);

    rank = fg->nodes[upper_loop].outputs[0].ty.dims;

    num_inputs = fg->nodes[upper_loop].num_inputs + fg->nodes[lower_loop].num_inputs;
    num_outputs = fg->nodes[lower_loop].num_outputs;

    upper_gen = find_generator_graph (fg, upper_loop);
    lower_gen = find_generator_graph (fg, lower_loop);

    /*
    n_upper_gens = fg->nodes[upper_gen].num_outputs;
    n_lower_gens = fg->nodes[lower_gen].num_outputs;
    */
    n_upper_gens = count_generators (fg, upper_gen);
    n_lower_gens = count_generators (fg, lower_gen);

    special_input = fg->nodes[upper_loop].outputs[0].targets->port;
    lower_win = find_lower_gen (fg, upper_loop, lower_loop);

    /* create the new loop */
    new_loop = new_ddcf_graph (fg, ND_FORALL, num_inputs, num_outputs, fg->nodes[upper_loop].loc);

    /* transfer pragmas from upper loop to new loop */
    fg->nodes[new_loop].pragmas = fg->nodes[upper_loop].pragmas;
    fg->nodes[upper_loop].pragmas = NULL;

    /* retarget the upper loop's input edges to the new loop */
    for (i=0, j=0; i<fg->nodes[upper_loop].num_inputs; i++, j++)
	{
	fg->nodes[new_loop].inputs[j] = fg->nodes[upper_loop].inputs[i];
	fg->nodes[upper_loop].inputs[i].back_edges = NULL;
	retarget (fg, fg->nodes[new_loop].inputs[j].back_edges, upper_loop, i, new_loop, j);
	gin = fg->nodes[new_loop].My_inputs[j];
	copy_type_info (&(fg->nodes[gin].outputs[0].ty), &(fg->nodes[new_loop].inputs[j].ty));
	add_edge (fg, gin, 0, upper_loop, i);
	}

    /* retarget the lower loop's input edges to the new loop, except the ones coming from the upper loop */
    for (i=0; i<fg->nodes[lower_loop].num_inputs; i++, j++)
        {
	assert (fg->nodes[lower_loop].inputs[i].back_edges != NULL);
	nd = fg->nodes[lower_loop].inputs[i].back_edges->node;
	if (nd != upper_loop)
	    {
	    fg->nodes[new_loop].inputs[j] = fg->nodes[lower_loop].inputs[i];
	    fg->nodes[lower_loop].inputs[i].back_edges = NULL;
	    gin = fg->nodes[new_loop].My_inputs[j];
	    copy_type_info (&(fg->nodes[gin].outputs[0].ty), &(fg->nodes[new_loop].inputs[j].ty));
	    retarget (fg, fg->nodes[new_loop].inputs[j].back_edges, lower_loop, i, new_loop, j);
	    }
	}

    /* re-source lower loop's outputs to come from new_loop */
    for (i=0; i<fg->nodes[lower_loop].num_outputs; i++)
        {
	fg->nodes[new_loop].outputs[i] = fg->nodes[lower_loop].outputs[i];
	retarget_backedges (fg, fg->nodes[new_loop].outputs[i].targets, lower_loop, i, new_loop, i);
	}

    /* build the new dot product graph */
    num_inputs_dot = fg->nodes[upper_gen].num_inputs + fg->nodes[lower_gen].num_inputs;
    num_outputs_dot = fg->nodes[upper_gen].num_outputs + fg->nodes[lower_gen].num_outputs;
    new_dot = new_ddcf_graph (fg, ND_DOT_PROD, num_inputs_dot, num_outputs_dot, fg->nodes[upper_loop].loc);

    j = build_gens_from_upper_loop (fg, upper_loop, lower_win, new_loop, new_dot, rank);

    /* create a copy of the lower generator graph and remove the simple generator
     * that is fed by the upper loop; not necessary if the only generator in it
     * was the simple gen fed by upper loop (which we detect by there being only
     * one input to the lower gen);
     */
    if (fg->nodes[lower_gen].num_inputs > 1)
        {
        tmp_dot = copy_graph (fg, fg, lower_gen);
	/* find the input that is feeding the special window generator */
        for (i=0; i<fg->nodes[lower_gen].num_inputs; i++)
            {
	    gin = fg->nodes[lower_gen].My_inputs[i];
	    nd = fg->nodes[gin].outputs[0].targets->node;
	    if (nd == lower_win)
	        break;
	    }
        assert (i != fg->nodes[lower_gen].num_inputs);
        gin = fg->nodes[tmp_dot].My_inputs[i];
        nd = fg->nodes[gin].outputs[0].targets->node;
        pt = fg->nodes[gin].outputs[0].targets->port;
        remove_edge (fg, gin, 0, nd, pt);
        gout = fg->nodes[nd].outputs[0].targets->node;
        gout_index = fg->nodes[gout].Io_num;
        assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
        remove_edge (fg, nd, 0, gout, 0);

        free_ddcf_node (fg, nd, &(fg->nodes[tmp_dot].My_nodes));

        for (m=0, k=j; m<fg->nodes[tmp_dot].num_inputs; m++, k++)
            {
	    gin = fg->nodes[new_dot].My_inputs[k];
	    add_edge (fg, gin, 0, tmp_dot, m);
	    }
        for (m=0, k=j; m<fg->nodes[tmp_dot].num_outputs; m++, k++)
            {
	    gout = fg->nodes[new_dot].My_outputs[k];
	    add_edge (fg, tmp_dot, m, gout, 0);
	    }

	insert_node_in_intlist (fg, tmp_dot, &(fg->nodes[new_dot].My_nodes));

	lift_graph (fg, tmp_dot, &(fg->nodes[new_dot].My_nodes));
	}


    /* connect the remaining inputs of the new dot graph */
    for (i=0; i<fg->nodes[lower_loop].num_inputs; i++)
        {
	gin = fg->nodes[lower_loop].My_inputs[i];

	for (tg=fg->nodes[gin].outputs[0].targets; tg!=NULL; tg=tg->link)
	    if (tg->node == lower_gen)
	        {
		pt = tg->port;
		ogin = fg->nodes[new_loop].My_inputs[fg->nodes[upper_loop].num_inputs + i];
		add_edge (fg, ogin, 0, new_dot, n_upper_gens + pt);
		copy_type_info (&(fg->nodes[new_dot].inputs[n_upper_gens + pt].ty), &(fg->nodes[ogin].outputs[0].ty));
		ngin = fg->nodes[new_dot].My_inputs[n_upper_gens + pt];
		copy_type_info (&(fg->nodes[ngin].outputs[0].ty), &(fg->nodes[new_dot].inputs[n_upper_gens + pt].ty));
		}
	}

    remove_edge (fg, upper_loop, 0, lower_loop, special_input);

    /* make a copy of the lower loop, remove its generator */
    new_lower = copy_body_and_returns (fg, lower_loop, special_input, lower_win);

    /* connect the inputs of the new lower loop copy */
    for (i=0, j=fg->nodes[upper_loop].num_inputs; i<fg->nodes[lower_loop].num_inputs; i++, j++)
        if (i == special_input)
	    add_edge (fg, upper_loop, 0, new_lower, special_input);
	else
	    {
	    gin = fg->nodes[new_loop].My_inputs[j];
	    add_edge (fg, gin, 0, new_lower, i);
	    }

    for (i=fg->nodes[lower_loop].num_inputs, j=n_upper_gens; i<fg->nodes[new_lower].num_inputs; i++, j++)
	{
        add_edge (fg, new_dot, j, new_lower, i);
	copy_type_info (&(fg->nodes[new_dot].outputs[j].ty), &(fg->nodes[new_lower].inputs[i].ty));
	gout = fg->nodes[new_dot].My_outputs[j];
	copy_type_info (&(fg->nodes[gout].inputs[0].ty), &(fg->nodes[new_lower].inputs[i].ty));
	}

    /* connect the outputs of the new lower loop */
    for (i=0; i<fg->nodes[new_lower].num_outputs; i++)
        {
	gout = fg->nodes[new_loop].My_outputs[i];
	add_edge (fg, new_lower, i, gout, 0);
	copy_type_info (&(fg->nodes[gout].inputs[0].ty), &(fg->nodes[new_lower].outputs[i].ty));
	}

    insert_node_in_intlist (fg, new_dot, &(fg->nodes[new_loop].My_nodes));
    insert_node_in_intlist (fg, new_lower, &(fg->nodes[new_loop].My_nodes));
    insert_node_in_intlist (fg, new_loop, list_copy);
    delete_ele_from_intlist (upper_loop, list_copy);
    delete_ele_from_intlist (lower_loop, list_copy);
    insert_node_in_intlist (fg, upper_loop, &(fg->nodes[new_loop].My_nodes));

    lift_graph (fg, new_lower, &(fg->nodes[new_loop].My_nodes));

    destroy_graph (fg, lower_loop);
    delete_ele_from_intlist (lower_loop, &(fg->nodes[new_loop].My_nodes));
    }

int copy_body_and_returns (FuncGraph *fg, int loop, int special_input, int lower_win)
    {
    InputPort *new_inputs;
    int ret, gen, i, j, *new_in_ids, num_inputs, gin, gout;
    int special_position, io_num;

    ret = copy_graph (fg, fg, loop);
    gen = find_generator_graph (fg, ret);

    /* remove all edges from INPUT nodes that feed the generator graph */
    for (i=0; i<fg->nodes[ret].num_inputs; i++)
        {
	gin = fg->nodes[ret].My_inputs[i];
	remove_any_edges_to_a_target_node (fg, gin, 0, gen);
	}

    num_inputs = fg->nodes[loop].num_inputs + fg->nodes[gen].num_outputs;
    new_inputs = make_input_ports (num_inputs);
    new_in_ids = make_io_array (num_inputs);

    /* copy the existing set of inputs */
    for (i=0; i<fg->nodes[ret].num_inputs; i++)
        {
	new_inputs[i] = fg->nodes[ret].inputs[i];
	new_in_ids[i] = fg->nodes[ret].My_inputs[i];
	}

    /* create the additional inputs */
    for (i=fg->nodes[ret].num_inputs; i<num_inputs; i++)
        {
	new_in_ids[i] = new_ddcf_node (fg, ND_G_INPUT, 0, 1, fg->nodes[loop].loc);
	fg->nodes[new_in_ids[i]].My_graph = ret;
	fg->nodes[new_in_ids[i]].Io_num = i;
	add_to_list (new_in_ids[i], &(fg->nodes[ret].My_nodes));
	}
    
    SacFree (fg->nodes[ret].inputs)
    fg->nodes[ret].inputs = new_inputs;
    SacFree (fg->nodes[ret].My_inputs)
    fg->nodes[ret].My_inputs = new_in_ids;
    fg->nodes[ret].num_inputs = num_inputs;

    gout = fg->nodes[lower_win].outputs[0].targets->node;
    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
    special_position = fg->nodes[gout].Io_num;

    for (i=0, j=fg->nodes[loop].num_inputs; i<fg->nodes[gen].num_outputs; i++, j++)
        if (i == special_position)
	    {
	    gin = fg->nodes[ret].My_inputs[special_input];
	    fg->nodes[gin].outputs[0] = fg->nodes[gen].outputs[i];
	    fg->nodes[gen].outputs[i].targets = NULL;
	    retarget_backedges (fg, fg->nodes[gin].outputs[0].targets, gen, i, gin, 0);
	    io_num = fg->nodes[gin].Io_num;
	    copy_type_info (&(fg->nodes[ret].inputs[io_num].ty), &(fg->nodes[gin].outputs[0].ty));
	    }
	else
	    {
	    gin = fg->nodes[ret].My_inputs[j];
	    fg->nodes[gin].outputs[0] = fg->nodes[gen].outputs[i];
	    fg->nodes[gen].outputs[i].targets = NULL;
	    retarget_backedges (fg, fg->nodes[gin].outputs[0].targets, gen, i, gin, 0);
	    io_num = fg->nodes[gin].Io_num;
	    copy_type_info (&(fg->nodes[ret].inputs[io_num].ty), &(fg->nodes[gin].outputs[0].ty));
	    }

    destroy_graph (fg, gen);
    delete_ele_from_intlist (gen, &(fg->nodes[ret].My_nodes));

    return ret;
    }

int build_gens_from_upper_loop (FuncGraph *fg, int upper_loop, int lower_win, int new_loop, int new_dot, int rank)
    {
    int i, j, k, m, gin, ugin, udgin, nd, pt, upper_win, new_win, ngin, ngout;
    int c_new, s_new, c_upper, s_upper, c_lower, s_lower;
    char str[128];

    for (i=0, j=0; i<fg->nodes[upper_loop].num_inputs; i++)
        {
	gin = fg->nodes[new_loop].My_inputs[i];
	ugin = fg->nodes[upper_loop].My_inputs[i];
	nd = fg->nodes[ugin].outputs[0].targets->node;
	pt = fg->nodes[ugin].outputs[0].targets->port;
	if (fg->nodes[nd].nodetype == ND_DOT_PROD)
	    {
	    remove_edge (fg, gin, 0, upper_loop, i);
	    add_edge (fg, gin, 0, new_dot, j);
	    copy_type_info (&(fg->nodes[new_dot].inputs[j].ty), &(fg->nodes[gin].outputs[0].ty));
	    add_edge (fg, new_dot, j, upper_loop, i);
	    clear_dim_sizes_upper_loop (fg, upper_loop, i);
	    copy_type_info (&(fg->nodes[new_dot].outputs[j].ty), &(fg->nodes[upper_loop].inputs[i].ty));

	    udgin = fg->nodes[nd].My_inputs[pt];
	    upper_win = fg->nodes[udgin].outputs[0].targets->node;

	    /* create a new window generator */
	    new_win = new_ddcf_node (fg, ND_WINDOW_GEN, 2*rank+1, rank+1, fg->nodes[upper_loop].loc);
	    ngin = fg->nodes[new_dot].My_inputs[j];
	    copy_type_info (&(fg->nodes[ngin].outputs[0].ty), &(fg->nodes[new_dot].inputs[j].ty));
	    add_edge (fg, ngin, 0, new_win, 0);
	    copy_type_info (&(fg->nodes[new_win].inputs[0].ty), &(fg->nodes[new_dot].inputs[j].ty));
	    ngout = fg->nodes[new_dot].My_outputs[j];
	    add_edge (fg, new_win, 0, ngout, 0);
	    copy_type_info (&(fg->nodes[new_win].outputs[0].ty), &(fg->nodes[new_dot].outputs[j].ty));
	    copy_type_info (&(fg->nodes[ngout].inputs[0].ty), &(fg->nodes[new_dot].outputs[j].ty));
	    insert_val_after_in_intlist (new_win, ngin, &(fg->nodes[new_dot].My_nodes));

	    for (k=1; k<fg->nodes[new_win].num_outputs; k++)
	        set_type_to_uint (&(fg->nodes[new_win].outputs[k].ty), 32);

	    for (k=0, m=1; k<rank; k++, m+=2)
	        {
		sscanf (fg->nodes[upper_win].inputs[m].constval, "%d", &c_upper);
		sscanf (fg->nodes[upper_win].inputs[m+1].constval, "%d", &s_upper);
		sscanf (fg->nodes[lower_win].inputs[m].constval, "%d", &c_lower);
		sscanf (fg->nodes[lower_win].inputs[m+1].constval, "%d", &s_lower);
		c_new = (c_lower-1)*s_upper+c_upper;
		s_new = s_upper * s_lower;
		fg->nodes[new_win].inputs[m].is_const = TRUE;
		sprintf (str, "%d", c_new);
		fg->nodes[new_win].inputs[m].constval = copy_to_const (str);
		fg->nodes[new_win].inputs[m+1].is_const = TRUE;
		sprintf (str, "%d", s_new);
		fg->nodes[new_win].inputs[m+1].constval = copy_to_const (str);
		set_type_to_uint (&(fg->nodes[new_win].inputs[m].ty), 32);
		set_type_to_uint (&(fg->nodes[new_win].inputs[m+1].ty), 32);
		}

	    j++;
	    }
	}

    return j;
    }

int is_fusable (FuncGraph *fg, int upper_loop, int lower_loop)
    {
    IntList *it;
    int gin, gout, nd, pt, upper_gen, lower_gen, i;

    if (find_pragma (fg, upper_loop, PRAG_NO_FUSE) != NULL)
	{
        return FALSE;
	}

    /* upper loop must have exactly one return value */
    if (fg->nodes[upper_loop].num_outputs != 1)
	{
        return FALSE;
	}

    /* return node of upper loop must be ND_CONSTRUCT_ARRAY */
    gout = fg->nodes[upper_loop].My_outputs[0];
    assert (fg->nodes[gout].inputs[0].back_edges != NULL);
    nd = fg->nodes[gout].inputs[0].back_edges->node;
    if (fg->nodes[nd].nodetype != ND_CONSTRUCT_ARRAY)
	{
        return FALSE;
	}

    /* value flowing into the upper loop return node must be scalar */
    if (fg->nodes[nd].inputs[0].ty.kind != Scalar)
	{
        return FALSE;
	}

    /* upper loop's generator must be DOT */
    upper_gen = find_generator_graph (fg, upper_loop);
    if (fg->nodes[upper_gen].nodetype != ND_DOT_PROD)
	{
        return FALSE;
	}

    /* all simple generators within the upper loop's gen graph must be
     * window gens or ele gens with scalar outputs, and every generator's
     * outputs[0] must target a ND_G_OUTPUT node (i.e., the value must be used)
     */
    for (it=fg->nodes[upper_gen].My_nodes; it!=NULL; it=it->link)
        switch (fg->nodes[it->val].nodetype)
	    {
	    case ND_WINDOW_GEN :
		if (fg->nodes[it->val].outputs[0].targets == NULL)
		    return FALSE;
		break;
	    case ND_ELE_GEN :
	      if (fg->nodes[it->val].outputs[0].targets == NULL)
		return FALSE;
	      break;
	    case ND_SLICE_GEN:
	      if (fg->nodes[it->val].outputs[0].targets == NULL)
		return FALSE;
	      if (fg->nodes[it->val].outputs[0].ty.kind != Scalar)
		return FALSE;
	      break;
	    case ND_SCALAR_GEN :
	    case ND_LOOP_INDICES :
	        return FALSE;
	    default :
                break;
	    }

    /* make sure the gens have const inputs, and no 'at' values */
    for (it=fg->nodes[upper_gen].My_nodes; it!=NULL; it=it->link)
	switch (fg->nodes[it->val].nodetype)
	    {
	    case ND_WINDOW_GEN :
	    case ND_ELE_GEN :
	    case ND_SLICE_GEN:
	        for (i=1; i<fg->nodes[it->val].num_outputs; i++)
	            if (fg->nodes[it->val].outputs[i].targets != NULL)
		        {
	                return FALSE;
		        }

	        for (i=1; i<fg->nodes[it->val].num_inputs; i++)
	            if (! fg->nodes[it->val].inputs[i].is_const)
		        {
		        return FALSE;
		        }
		break;
	    default :
                break;
	    }

    /* the upper loop's output must target the lower loop and nothing else */
    assert (fg->nodes[upper_loop].outputs[0].targets != NULL);
    nd = fg->nodes[upper_loop].outputs[0].targets->node;
    pt = fg->nodes[upper_loop].outputs[0].targets->port;
    if (nd != lower_loop)
	{
        return FALSE;
	}
    if (fg->nodes[upper_loop].outputs[0].targets->link != NULL)
	{
        return FALSE;
	}

    /* the lower loop's generator must be a DOT */
    lower_gen = find_generator_graph (fg, lower_loop);
    if (fg->nodes[lower_gen].nodetype != ND_DOT_PROD)
	{
        return FALSE;
	}

    /* every lower loop generator's outputs[0] must target a ND_G_OUTPUT
     * node (i.e., the value must be used)
     */
    for (it=fg->nodes[lower_gen].My_nodes; it!=NULL; it=it->link)
        switch (fg->nodes[it->val].nodetype)
	    {
	    case ND_WINDOW_GEN :
	    case ND_ELE_GEN :
	    case ND_SLICE_GEN:
		if (fg->nodes[it->val].outputs[0].targets == NULL)
		    return FALSE;
		break;
	    default :
                break;
	    }

    /* the upper loop's output must target a window generator or
     * ele gen with a scalar output in the lower loop, and nothing else
     */
    gin = fg->nodes[lower_loop].My_inputs[pt];
    assert (fg->nodes[gin].outputs[0].targets != NULL);
    nd = fg->nodes[gin].outputs[0].targets->node;
    pt = fg->nodes[gin].outputs[0].targets->port;
    if (nd != lower_gen)
	{
        return FALSE;
	}
    if (fg->nodes[gin].outputs[0].targets->link != NULL)
	{
        return FALSE;
	}
    gin = fg->nodes[lower_gen].My_inputs[pt];
    assert (fg->nodes[gin].outputs[0].targets != NULL);
    nd = fg->nodes[gin].outputs[0].targets->node;
    switch (fg->nodes[nd].nodetype)
        {
	case ND_WINDOW_GEN :
	case ND_ELE_GEN :
	    break;
	case ND_SLICE_GEN :
	    if (fg->nodes[nd].outputs[0].ty.kind != Scalar)
	        return FALSE;
	    break;
	default :
            return FALSE;
	}
    if (fg->nodes[gin].outputs[0].targets->link != NULL)
	{
        return FALSE;
	}

    /* the generator must have constant values for size and step, and must have no 'at' spec */
    for (i=1; i<fg->nodes[nd].num_inputs; i++)
        if (! fg->nodes[nd].inputs[i].is_const)
	    {
	    return FALSE;
	    }
    for (i=1; i<fg->nodes[nd].num_outputs; i++)
        if (fg->nodes[nd].outputs[i].targets != NULL)
	    {
            return FALSE;
	    }

    return TRUE;
    }

void convert_elegens_to_window_gens (FuncGraph *fg, int upper_loop, int lower_loop)
    {
    IntList *it;
    int upper_gen, lower_gen, lower_simple_gen;

    upper_gen = find_generator_graph (fg, upper_loop);
    lower_gen = find_generator_graph (fg, lower_loop);

    /* check the upper generators */
    for (it=fg->nodes[upper_gen].My_nodes; it!=NULL; it=it->link)
        if (fg->nodes[it->val].nodetype == ND_ELE_GEN)
	    elegen_to_window (fg, it->val, upper_gen, upper_loop);

    lower_simple_gen = find_lower_gen (fg, upper_loop, lower_loop);
    if (fg->nodes[lower_simple_gen].nodetype == ND_ELE_GEN)
        elegen_to_window (fg, lower_simple_gen, lower_gen, lower_loop);
    }

/* follow the single output of the upper loop into the lower loop, into
 * its generator graph, and to a window or ele generator node
 */
int find_lower_gen (FuncGraph *fg, int upper_loop, int lower_loop)
    {
    int nd, pt, gin;

    assert (fg->nodes[upper_loop].outputs[0].targets->node == lower_loop);
    pt = fg->nodes[upper_loop].outputs[0].targets->port;
    gin = fg->nodes[lower_loop].My_inputs[pt];
    nd = fg->nodes[gin].outputs[0].targets->node;
    pt = fg->nodes[gin].outputs[0].targets->port;
    assert (fg->nodes[nd].nodetype == ND_DOT_PROD);
    gin = fg->nodes[nd].My_inputs[pt];
    nd = fg->nodes[gin].outputs[0].targets->node;
    assert ((fg->nodes[nd].nodetype == ND_WINDOW_GEN) || (fg->nodes[nd].nodetype == ND_ELE_GEN));
    return nd;
    }

void clear_dim_sizes_upper_loop (FuncGraph *fg, int upper_loop, int i)
    {
    Edge *tg;
    int gin, dot, nd, pt;

    clear_dim_sizes (&(fg->nodes[upper_loop].inputs[i].ty));
    gin = fg->nodes[upper_loop].My_inputs[i];
    assert (fg->nodes[gin].nodetype == ND_G_INPUT);
    clear_dim_sizes (&(fg->nodes[gin].outputs[0].ty));
    for (tg=fg->nodes[gin].outputs[0].targets; tg!=NULL; tg=tg->link)
        {
	dot = tg->node;
	pt = tg->port;
	assert (fg->nodes[dot].nodetype == ND_DOT_PROD);
	clear_dim_sizes (&(fg->nodes[dot].inputs[pt].ty));
	gin = fg->nodes[dot].My_inputs[pt];
	assert (fg->nodes[gin].nodetype == ND_G_INPUT);
	clear_dim_sizes (&(fg->nodes[gin].outputs[0].ty));
	assert (fg->nodes[gin].outputs[0].targets != NULL);
	assert (fg->nodes[gin].outputs[0].targets->link == NULL);
	nd = fg->nodes[gin].outputs[0].targets->node;
	pt = fg->nodes[gin].outputs[0].targets->port;
	clear_dim_sizes (&(fg->nodes[nd].inputs[pt].ty));
	}
    }

int count_generators (FuncGraph *fg, int gen)
    {
    IntList *it;
    int id, cnt;

    for (cnt=0, it=fg->nodes[gen].My_nodes; it!=NULL; it=it->link)
        {
	id = it->val;

	switch (fg->nodes[id].nodetype)
	    {
	    case ND_WINDOW_GEN :
	    case ND_ELE_GEN :
	    case ND_SLICE_GEN :
	    case ND_SCALAR_GEN :
	        cnt++;
		break;
	    default :
	        break;
	    }
	}
    
    return cnt;
    }
