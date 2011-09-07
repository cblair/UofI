#include "sassy.h"
#include "error.h"
#include "ddcf.h"

/*
#define FAILPRINT(str) fprintf (stderr, str);
*/
#define FAILPRINT(str)

void create_and_install_buff_terminate (FuncGraph*, int, int, int, int, int);
void cast_to_bool (FuncGraph *fg, int nd);
int find_next_nodes_output (FuncGraph *fg, int nd);
void give_next_nodes_outputs (FuncGraph *fg, int id);
void convert_next (FuncGraph *fg, int nd, int loop_id);
int convert_loop_return_node_for_dfg (FuncGraph*, int, int, int, int, int, IntList**);
void convert_node_for_dfg (FuncGraph*, int, int, IntList**);
void bitwidth_narrow (FuncGraph*, int);
int elegen_extents_build (FuncGraph*, int, int, Location, int*, int, int*);
int slicegen_extents_build (FuncGraph*, int, int, Location, int*, int, int*);
int windowgen_extents_build (FuncGraph*, int, int, Location, int*, int, int*);
int scalargen_extents_build (FuncGraph*, int, int, Location, int*, int, int*);
void process_graphs_nodes_to_dfg (FuncGraph*, int);
int install_interface_code (FuncGraph*, int, int, int);
int num_inputs_needed (FuncGraph*, int, int);
int enclose_graph (FuncGraph*, int);
int find_extents_of_gen_input (FuncGraph*, int);
void create_and_install_trf_scalar_to_host (FuncGraph*, int, int, int, int);
void create_and_install_trf_array_to_host (FuncGraph*, int, int, int, int, int);
void lift_graph (FuncGraph*, int, IntList**);
void window_convert (FuncGraph*, int, IntList**);
void elegen_convert (FuncGraph*, int, IntList**);
void slicegen_convert (FuncGraph*, int, IntList**);
void convert_to_dfg (FuncGraph*, int, int, int);
int rc_capable (FuncGraph*, int);
void set_graphs_input_node_type_to_uint (FuncGraph*, int, int, int);
int is_unsigned (Type);
int is_complex_num_type (Type);
void convert_input_for_dfg (FuncGraph*, int, int);
void convert_loop_to_dfg (FuncGraph*, int, IntList**);
void convert_arith (FuncGraph*, int, IntList**);
void convert_mul (FuncGraph*, int, IntList**);
void convert_compares (FuncGraph*, int, IntList**);
void convert_macros (FuncGraph*, int);
void convert_switch (FuncGraph*, int, int, IntList**);
void convert_cast (FuncGraph*, int, IntList**);
void convert_cast_from_uint (FuncGraph*, int, IntList**);
void convert_cast_from_int (FuncGraph*, int, IntList**);
void convert_cast_from_bits (FuncGraph*, int, IntList**);
void convert_cast_from_bool (FuncGraph*, int, IntList**);
void convert_cast_from_fix (FuncGraph*, int, IntList**);
void convert_cast_from_ufix (FuncGraph*, int, IntList**);
void shift_arg (FuncGraph*, int, int, int, IntList**, char);
void change_width (FuncGraph*, int, int, int, IntList**);
void dfl_dump (FuncGraph*, int, FILE*);
void delete_this_node (FuncGraph*, int, IntList**);
void cnvrt_to_bits_node (FuncGraph*, int, int, DdcfType, IntList**);
void insert_bits_node (FuncGraph*, int, int, DdcfType, IntList**);
int sign_extend (int, int, int);
int is_signed (Type);
int bit_width (int);
int follow_to_input_source (FuncGraph*, int, int);
int last_in_list (int, int, int, IntList*);
void build_extents_info (FuncGraph*, int, int, int, Location, int*);
int build_array_input_interface (FuncGraph*, int, int);
void allocate_targets (FuncGraph*, int, int, int, int, int*, int, int, IntList**);
void coalesce_tile (FuncGraph*, int, int, int, int, int, IntList**);
DdcfType write_tile_type (FuncGraph*, int, int, int);
int create_malloc_tgt_node (FuncGraph*, int, int, int);
void convert_sqrt (FuncGraph*, int, IntList**);
void convert_arrayref (FuncGraph*, int, IntList**);
void convert_all_multiplies (FuncGraph*, int);
void convert_mult_to_mach_mult (FuncGraph*, int, IntList**);
void convert_lshift_to_mach_lshift (FuncGraph*, int, IntList**);
void try_to_elim_a_cast_above (FuncGraph*, int, int, IntList**, Location);
void cast_an_input_to_unsigned (FuncGraph*, int, int, IntList**, Location);
void cast_an_input_to_signed (FuncGraph*, int, int, IntList**, Location);
void insert_a_cast_below (FuncGraph*, int,  IntList**, Location);
int no_lost_bits (FuncGraph*, int);
int does_mult_fit_in_32_bits (FuncGraph*, int);
int has_removeable_cast (FuncGraph*, int, int, int);

void ddcf2dfg ()
    {
    FuncGraph *fg;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
            {
            process_graphs_nodes_to_dfg (fg, 0);
            }
    }

void process_graphs_nodes_to_dfg (FuncGraph *fg, int id)
    {
    IntList *it;
    int nd, new_node, res_index, loop_rank;

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;

	if (!is_simple_node (fg, nd))
	    process_graphs_nodes_to_dfg (fg, nd);

	if (rc_capable (fg, nd))
	    {
	    if (!quiet)
	        printf ("  converting loop to DFG, line %d, function '%s', file '%s'\n",
				fg->nodes[nd].loc.line, fg->nodes[nd].loc.func, fg->nodes[nd].loc.file);
	    loop_rank = get_loop_rank (fg, nd);
	    give_next_nodes_outputs (fg, nd);
	    new_node = enclose_graph (fg, nd);
	    replace_in_intlist (&(fg->nodes[id].My_nodes), nd, new_node);
	    res_index = install_interface_code (fg, new_node, nd, loop_rank);
	    convert_to_dfg (fg, nd, res_index, loop_rank);
	    }
	}
    }

/*  find each NEXT node, and see whether the output that feeds it also feeds an
 *  OUTPUT node; if so, it is a 'final' output; we re-source the OUTPUT node with
 *  an edge from the NEXT node's output; then we can treat it in a way similar to
 *  the treatment of other loop returns that produce scalars; while we're here, we
 *  also give each NEXT node two outputs, in preparation for later conversion to
 *  CIRCULATE;
 */
void give_next_nodes_outputs (FuncGraph *fg, int id)
    {
    IntList *it, *list_copy;
    Edge *ed;
    int nd, onode, snd, spt, write_node;

    copy_intlist (&list_copy, &(fg->nodes[id].My_nodes));

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;

	if (fg->nodes[nd].nodetype == ND_NEXT)
	    {
	    fg->nodes[nd].outputs = make_output_ports (2);
	    fg->nodes[nd].num_outputs = 2;

	    onode = find_next_nodes_output (fg, nd);

	    if (onode != -1)
	        {
		snd = fg->nodes[nd].inputs[0].back_edges->node;
		spt = fg->nodes[nd].inputs[0].back_edges->port;

		remove_edge (fg, snd, spt, onode, 0);

		write_node = new_ddcf_node (fg, ND_WRITE_SCALAR, 2, 1, fg->nodes[nd].loc);
		add_edge (fg, nd, 0, write_node, 0);
		add_edge (fg, write_node, 0, onode, 0);
		copy_type_info (&(fg->nodes[nd].outputs[0].ty), &(fg->nodes[snd].outputs[spt].ty));
		copy_type_info (&(fg->nodes[write_node].inputs[0].ty), &(fg->nodes[snd].outputs[spt].ty));
		copy_type_info (&(fg->nodes[write_node].outputs[0].ty), &(fg->nodes[snd].outputs[spt].ty));
		insert_val_after_in_intlist (write_node, nd, &list_copy);
		}
	    }
	}

    free_intlist (&(fg->nodes[id].My_nodes));
    fg->nodes[id].My_nodes = list_copy;
    }

/*  this routine installs the interface code between the RC_COMPUTE node and its
 *  internal loop node; the internal loop node needs more inputs to convey the
 *  interface information; the order of the loop inputs is:
 *	1 - all of the loop's input data; each scalar gets one input; each array gets
 *	    an input for its RC source memory address and an input for each of its extents
 *	2 - an input for the address of each return value in the RC target memory
 *	3 - an input for each loop extent
 *	4 - an input for the loop's total iteration count
 *  the loop gets one output, and the internal reduction nodes must be ANDed together
 *  to provide that output if there were multiple outputs;
 */
int install_interface_code (FuncGraph *fg, int id, int rc_code, int loop_rank)
    {
    IntList *it_copy;
    InputPort *new_inputs;
    int gin, i, j, k, loop_in_cnt;
    int n_inputs, n_outputs, *new_innums, newgin;
    int s_node, and_node, gout;
    int *loop_extents;

    SacMalloc (loop_extents, int*, loop_rank * sizeof (int))

    /* allocate the new inputs for the loop */
    n_inputs = num_inputs_needed (fg, rc_code, loop_rank);
    new_inputs = make_input_ports (n_inputs);
    SacMalloc (new_innums, int*, n_inputs * sizeof (int))

    /* redistribute the inputs, leaving space for the additional pieces of info */
    for (i=0, j=0; i<fg->nodes[rc_code].num_inputs; i++)
        {
	new_inputs[j] = fg->nodes[rc_code].inputs[i];
	retarget (fg, new_inputs[j].back_edges, rc_code, i, rc_code, j);
	gin = fg->nodes[rc_code].My_inputs[i];
	fg->nodes[gin].Io_num = j;
	new_innums[j] = gin;
	retarget_backedges (fg, fg->nodes[gin].outputs[0].targets, gin, i, gin, j);
	j++;

	/* create inputs for the extents */
	if (fg->nodes[rc_code].inputs[i].ty.kind == Array)
	    for (k=0; k<fg->nodes[rc_code].inputs[i].ty.dims; k++)
	        {
	        newgin = new_ddcf_node (fg, ND_G_INPUT, 0, 1, fg->nodes[rc_code].loc);
	        fg->nodes[newgin].Io_num = j;
	        fg->nodes[newgin].My_graph = rc_code;
	        new_innums[j] = newgin;
	        insert_val_after_in_intlist (newgin, gin, &(fg->nodes[rc_code].My_nodes));
		j++;
	        }
	}

    /* set up the inputs that are associated with the loop's reductions */
    for (k=j; k<n_inputs; k++)
        {
	newgin = new_ddcf_node (fg, ND_G_INPUT, 0, 1, fg->nodes[rc_code].loc);
	fg->nodes[newgin].Io_num = k;
	fg->nodes[newgin].My_graph = rc_code;
	new_innums[k] = newgin;
	add_to_list (newgin, &(fg->nodes[rc_code].My_nodes));
	}

    /* free the old input info and install the new */
    SacFree (fg->nodes[rc_code].inputs)
    fg->nodes[rc_code].inputs = new_inputs;
    SacFree (fg->nodes[rc_code].My_inputs)
    fg->nodes[rc_code].My_inputs = new_innums;
    fg->nodes[rc_code].num_inputs = n_inputs;

    /* build the array information for extents and size */
    loop_in_cnt = build_array_input_interface (fg, id, rc_code);

    /* build the information for the loop's iteration total */
    build_extents_info (fg, rc_code, id, n_inputs-1-loop_rank, fg->nodes[id].loc, loop_extents);

    /* if there are multiple outputs, create an AND node to combine the trigger outputs */
    n_outputs = fg->nodes[rc_code].num_outputs;
    if (n_outputs > 1)
	{
	copy_intlist (&it_copy, &(fg->nodes[rc_code].My_nodes));
	and_node = new_ddcf_node (fg, ND_REDUCE_AND_MACRO, n_outputs*2, 1, fg->nodes[id].loc);
	}
    else
        and_node = -1;

    /*  malloc result space for each loop reduction */
    allocate_targets (fg, rc_code, id, and_node, loop_in_cnt, loop_extents, loop_rank, n_outputs, &it_copy);

    if (and_node > 0)
        {
	for (i=1; i<fg->nodes[rc_code].num_outputs; i++)
	    {
	    gout = fg->nodes[rc_code].My_outputs[i];
	    free_ddcf_node (fg, gout, &it_copy);
	    }

	gout = fg->nodes[rc_code].My_outputs[0];
	add_edge (fg, and_node, 0, gout, 0);
	fg->nodes[rc_code].num_outputs = 1;
	set_type_to_bool (&(fg->nodes[and_node].outputs[0].ty));
	set_type_to_bool (&(fg->nodes[gout].inputs[0].ty));
	set_type_to_bool (&(fg->nodes[rc_code].outputs[0].ty));

	free_intlist (&(fg->nodes[rc_code].My_nodes));
	fg->nodes[rc_code].My_nodes = it_copy;
	}

    SacFree (loop_extents)

    return loop_in_cnt;
    }

int build_array_input_interface (FuncGraph *fg, int id, int rc_code)
    {
    int i, j, k, gin, tg_port, rank_nd, extents_nd, mallxfer_nd;

    for (i=0, j=0; i<fg->nodes[id].num_inputs; i++)
        if (fg->nodes[id].inputs[i].ty.kind == Array)
	    {
	    gin = fg->nodes[id].My_inputs[i];
	    tg_port = fg->nodes[gin].outputs[0].targets->port;
	    j++;

	    extents_nd = new_ddcf_node (fg, ND_EXTENTS, 1, fg->nodes[id].inputs[i].ty.dims, fg->nodes[id].loc);
	    add_edge (fg, gin, 0, extents_nd, 0);
	    copy_type_info (&(fg->nodes[extents_nd].inputs[0].ty), &(fg->nodes[gin].outputs[0].ty));
	    insert_val_after_in_intlist (extents_nd, gin, &(fg->nodes[id].My_nodes));
	    for (k=0; k<fg->nodes[id].inputs[i].ty.dims; k++)
		{
	        add_edge (fg, extents_nd, k, rc_code, j);
		set_type_to_uint (&(fg->nodes[extents_nd].outputs[k].ty), 32);
		set_type_to_uint (&(fg->nodes[rc_code].inputs[j].ty), 32);
	        set_graphs_input_node_type_to_uint (fg, rc_code, j++, 32);
		}

	    mallxfer_nd = new_ddcf_node (fg, ND_MALL_XFER, 1, 1, fg->nodes[id].loc);
	    add_edge (fg, gin, 0, mallxfer_nd, 0);
	    copy_type_info (&(fg->nodes[mallxfer_nd].inputs[0].ty), &(fg->nodes[gin].outputs[0].ty));
	    set_type_to_uint (&(fg->nodes[mallxfer_nd].outputs[0].ty), 32);
	    insert_val_after_in_intlist (mallxfer_nd, gin, &(fg->nodes[id].My_nodes));

	    remove_edge (fg, gin, 0, rc_code, tg_port);
	    add_edge (fg, mallxfer_nd, 0, rc_code, tg_port);
	    set_type_to_uint (&(fg->nodes[rc_code].inputs[tg_port].ty), 32);
	    set_graphs_input_node_type_to_uint (fg, rc_code, tg_port, 32);
	    }
	else
	    j++;

    return j;
    }

void allocate_targets (FuncGraph *fg, int rc_code, int id,
		int and_node, int j, int *loop_extents, int loop_rank, int n_outputs, IntList **it_copy)
    {
    IntList *it;
    int nd, cnt, mm, sz, mall_node, gout, i;
    char str[128];
    char cstring[256];

    for (it=fg->nodes[rc_code].My_nodes, cnt=0; it!=NULL; it=it->link)
	{
	nd = it->val;

        switch (fg->nodes[nd].nodetype)
	    {
	    case ND_CONSTRUCT_CONCAT :
	    case ND_CONSTRUCT_CONCAT_MASKED :
		assert (0);
	        break;
	    case ND_CONSTRUCT_CONCAT_MASKED_MANY :
		{
		int mall_node0, mall_node1;

		/* this is the target buffer allocation */
                mall_node0 = create_malloc_tgt_node (fg, id, fg->nodes[nd].outputs[0].ty.totsize, 1);
		insert_val_before_in_intlist (mall_node0, rc_code, &(fg->nodes[id].My_nodes));
		sprintf (cstring, "%d", buf_size_in_words * 32 / fg->nodes[nd].inputs[0].ty.totsize);
		fg->nodes[mall_node0].inputs[0].constval = copy_to_const (cstring);
		fg->nodes[mall_node0].inputs[0].is_const = TRUE;
		set_type_to_uint (&(fg->nodes[mall_node0].inputs[0].ty), 32);
                add_edge (fg, mall_node0, 0, rc_code, j);
                set_type_to_uint (&(fg->nodes[mall_node0].outputs[0].ty), 32);
                set_type_to_uint (&(fg->nodes[rc_code].inputs[j].ty), 32);
                set_graphs_input_node_type_to_uint (fg, rc_code, j++, 32);

		/* this is the counter allocation */
                mall_node1 = create_malloc_tgt_node (fg, id, 32, 1);
		insert_val_before_in_intlist (mall_node1, rc_code, &(fg->nodes[id].My_nodes));
		strcpy (cstring, "1");
		fg->nodes[mall_node1].inputs[0].constval = copy_to_const (cstring);
		fg->nodes[mall_node1].inputs[0].is_const = TRUE;
		set_type_to_uint (&(fg->nodes[mall_node1].inputs[0].ty), 32);
                add_edge (fg, mall_node1, 0, rc_code, j);
                set_type_to_uint (&(fg->nodes[mall_node1].outputs[0].ty), 32);
                set_type_to_uint (&(fg->nodes[rc_code].inputs[j].ty), 32);
                set_graphs_input_node_type_to_uint (fg, rc_code, j++, 32);

		create_and_install_buff_terminate (fg, nd, mall_node0, mall_node1, rc_code, id);
		set_type_to_bool (&(fg->nodes[nd].outputs[0].ty));

		if (and_node > 0)
		    {
		    assert (fg->nodes[nd].outputs[0].targets != NULL);
		    gout = fg->nodes[nd].outputs[0].targets->node;
		    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
		    remove_edge (fg, nd, 0, gout, 0);
		    add_edge (fg, nd, 0, and_node, cnt*2);
		    fg->nodes[and_node].inputs[cnt*2+1].is_const = TRUE;
		    fg->nodes[and_node].inputs[cnt*2+1].constval = copy_to_const ("true");
		    set_type_to_bool (&(fg->nodes[and_node].inputs[cnt*2].ty));
		    set_type_to_bool (&(fg->nodes[and_node].inputs[cnt*2+1].ty));
		    }
		else
		    {
		    assert (fg->nodes[nd].outputs[0].targets != NULL);
		    gout = fg->nodes[nd].outputs[0].targets->node;
		    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
		    set_type_to_bool (&(fg->nodes[gout].inputs[0].ty));
		    }

		cnt++;

		if ((cnt==n_outputs) && (and_node>0))
		    {
		    insert_val_after_in_intlist (and_node, nd, it_copy);
		    gout = fg->nodes[rc_code].My_outputs[0];
		    delete_ele_from_intlist (gout, it_copy);
		    }
		}
	        break;
	    case ND_CONSTRUCT_TILE :
                {
                int snd, result_rank, component_rank, mult_node, arrdef_node, lidx, cidx;

		result_rank = fg->nodes[nd].outputs[0].ty.dims;
		component_rank = fg->nodes[nd].inputs[0].ty.dims;

                assert (fg->nodes[nd].inputs[0].back_edges != NULL);
                snd = fg->nodes[nd].inputs[0].back_edges->node;

                if (fg->nodes[snd].nodetype != ND_ARR_DEF)
		    assert (0);

                mall_node = create_malloc_tgt_node (fg, id, fg->nodes[nd].outputs[0].ty.totsize, result_rank);

		insert_val_before_in_intlist (mall_node, rc_code, &(fg->nodes[id].My_nodes));

		assert (fg->nodes[nd].inputs[0].back_edges != NULL);
		arrdef_node = fg->nodes[nd].inputs[0].back_edges->node;
		assert (fg->nodes[arrdef_node].nodetype == ND_ARR_DEF);

		lidx = loop_rank - result_rank;
		cidx = component_rank - result_rank;

                for (i=0; i<result_rank; i++, lidx++, cidx++)
                    {
		    mult_node = new_ddcf_node (fg, ND_MUL, 2, 1, fg->nodes[nd].loc);
		    add_edge (fg, mult_node, 0, mall_node, i);
                    set_type_to_uint (&(fg->nodes[mult_node].outputs[0].ty), 32);
                    set_type_to_uint (&(fg->nodes[mall_node].inputs[i].ty), 32);

		    /* left input of MUL node comes from tile dim (must be const) */
		    if (cidx >= 0)
			sprintf (cstring, "%d", fg->nodes[arrdef_node].DefDims[cidx]);
		    else
		        strcpy (cstring, "1");
		    fg->nodes[mult_node].inputs[0].constval = copy_to_const (cstring);
		    fg->nodes[mult_node].inputs[0].is_const = TRUE;
		    set_type_to_uint (&(fg->nodes[mult_node].inputs[0].ty), 32);

		    /* right input comes from loop extents (never a constant) */
		    if (lidx >= 0)
		        add_edge (fg, loop_extents[lidx], 0, mult_node, 1);
		    else
		        {
			strcpy (cstring, "1");
			fg->nodes[mult_node].inputs[1].constval = copy_to_const (cstring);
			fg->nodes[mult_node].inputs[1].is_const = TRUE;
			}
		    set_type_to_uint (&(fg->nodes[mult_node].inputs[1].ty), 32);

		    insert_val_before_in_intlist (mult_node, mall_node, &(fg->nodes[id].My_nodes));
                    }

                add_edge (fg, mall_node, 0, rc_code, j);
                set_type_to_uint (&(fg->nodes[mall_node].outputs[0].ty), 32);
                set_type_to_uint (&(fg->nodes[rc_code].inputs[j].ty), 32);
                set_graphs_input_node_type_to_uint (fg, rc_code, j++, 32);
		create_and_install_trf_array_to_host (fg, nd, mall_node, rc_code, id, result_rank);
                set_type_to_bool (&(fg->nodes[nd].outputs[0].ty));

                if (and_node > 0)
                    {
                    assert (fg->nodes[nd].outputs[0].targets != NULL);
                    gout = fg->nodes[nd].outputs[0].targets->node;
                    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
                    remove_edge (fg, nd, 0, gout, 0);
                    add_edge (fg, nd, 0, and_node, cnt*2);
                    fg->nodes[and_node].inputs[cnt*2+1].is_const = TRUE;
                    fg->nodes[and_node].inputs[cnt*2+1].constval = copy_to_const ("true");
                    set_type_to_bool (&(fg->nodes[and_node].inputs[cnt*2].ty));
                    set_type_to_bool (&(fg->nodes[and_node].inputs[cnt*2+1].ty));
                    }
                else
                    {
                    assert (fg->nodes[nd].outputs[0].targets != NULL);
                    gout = fg->nodes[nd].outputs[0].targets->node;
                    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
                    set_type_to_bool (&(fg->nodes[gout].inputs[0].ty));
                    }

                cnt++;

                if ((cnt==n_outputs) && (and_node>0))
                    {
                    insert_val_after_in_intlist (and_node, nd, it_copy);
                    gout = fg->nodes[rc_code].My_outputs[0];
                    delete_ele_from_intlist (gout, it_copy);
                    }
                }

	        break;
	    case ND_CONSTRUCT_ARRAY :
                mall_node = create_malloc_tgt_node (fg, id, fg->nodes[nd].inputs[0].ty.totsize, loop_rank);
		insert_val_before_in_intlist (mall_node, rc_code, &(fg->nodes[id].My_nodes));
                for (i=0; i<loop_rank; i++)
                    {
                    add_edge (fg, loop_extents[i], 0, mall_node, i);
                    set_type_to_uint (&(fg->nodes[mall_node].inputs[i].ty), 32);
                    }

                add_edge (fg, mall_node, 0, rc_code, j);
                set_type_to_uint (&(fg->nodes[mall_node].outputs[0].ty), 32);
                set_type_to_uint (&(fg->nodes[rc_code].inputs[j].ty), 32);
                set_graphs_input_node_type_to_uint (fg, rc_code, j++, 32);

		create_and_install_trf_array_to_host (fg, nd, mall_node, rc_code, id, loop_rank);
                set_type_to_bool (&(fg->nodes[nd].outputs[0].ty));

                if (and_node > 0)
                    {
                    assert (fg->nodes[nd].outputs[0].targets != NULL);
                    gout = fg->nodes[nd].outputs[0].targets->node;
                    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
                    remove_edge (fg, nd, 0, gout, 0);
                    add_edge (fg, nd, 0, and_node, cnt*2);
                    fg->nodes[and_node].inputs[cnt*2+1].is_const = TRUE;
                    fg->nodes[and_node].inputs[cnt*2+1].constval = copy_to_const ("true");
                    set_type_to_bool (&(fg->nodes[and_node].inputs[cnt*2].ty));
                    set_type_to_bool (&(fg->nodes[and_node].inputs[cnt*2+1].ty));
                    }
                else
                    {
                    assert (fg->nodes[nd].outputs[0].targets != NULL);
                    gout = fg->nodes[nd].outputs[0].targets->node;
                    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
                    set_type_to_bool (&(fg->nodes[gout].inputs[0].ty));
                    }

                cnt++;

                if ((cnt==n_outputs) && (and_node>0))
                    {
                    insert_val_after_in_intlist (and_node, nd, it_copy);
                    gout = fg->nodes[rc_code].My_outputs[0];
                    delete_ele_from_intlist (gout, it_copy);
                    }

		break;
	    case ND_REDUCE_HIST :
                mall_node = create_malloc_tgt_node (fg, id, fg->nodes[nd].outputs[0].ty.totsize, 1);

		/* the size input of the malloc node gets its value from the size input of the hist node */
		if (fg->nodes[nd].inputs[2].is_const)
		    {
		    fg->nodes[mall_node].inputs[0] = fg->nodes[nd].inputs[2];
		    insert_val_before_in_intlist (mall_node, rc_code, &(fg->nodes[id].My_nodes));
		    }
		else
		    {
		    int gin, io_num, grph, snd, spt;
		    assert (fg->nodes[nd].inputs[2].back_edges != NULL);
		    gin = fg->nodes[nd].inputs[2].back_edges->node;
		    assert (fg->nodes[gin].nodetype == ND_G_INPUT);
		    io_num = fg->nodes[gin].Io_num;
		    grph = fg->nodes[gin].My_graph;
		    assert (fg->nodes[grph].inputs[io_num].back_edges != NULL);
		    snd = fg->nodes[grph].inputs[io_num].back_edges->node;
		    spt = fg->nodes[grph].inputs[io_num].back_edges->port;
		    add_edge (fg, snd, spt, mall_node, 0);
		    copy_type_info (&(fg->nodes[mall_node].inputs[0].ty),
		    				&(fg->nodes[snd].outputs[spt].ty));
		    insert_val_after_in_intlist (mall_node, snd, &(fg->nodes[id].My_nodes));
		    }

                add_edge (fg, mall_node, 0, rc_code, j);
                set_type_to_uint (&(fg->nodes[mall_node].outputs[0].ty), 32);
                set_type_to_uint (&(fg->nodes[rc_code].inputs[j].ty), 32);
                set_graphs_input_node_type_to_uint (fg, rc_code, j++, 32);

		create_and_install_trf_array_to_host (fg, nd, mall_node, rc_code, id, 1);
                set_type_to_bool (&(fg->nodes[nd].outputs[0].ty));

                if (and_node > 0)
                    {
                    assert (fg->nodes[nd].outputs[0].targets != NULL);
                    gout = fg->nodes[nd].outputs[0].targets->node;
                    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
                    remove_edge (fg, nd, 0, gout, 0);
                    add_edge (fg, nd, 0, and_node, cnt*2);
                    fg->nodes[and_node].inputs[cnt*2+1].is_const = TRUE;
                    fg->nodes[and_node].inputs[cnt*2+1].constval = copy_to_const ("true");
                    set_type_to_bool (&(fg->nodes[and_node].inputs[cnt*2].ty));
                    set_type_to_bool (&(fg->nodes[and_node].inputs[cnt*2+1].ty));
                    }
                else
                    {
                    assert (fg->nodes[nd].outputs[0].targets != NULL);
                    gout = fg->nodes[nd].outputs[0].targets->node;
                    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
                    set_type_to_bool (&(fg->nodes[gout].inputs[0].ty));
                    }

                cnt++;

                if ((cnt==n_outputs) && (and_node>0))
                    {
                    insert_val_after_in_intlist (and_node, nd, it_copy);
                    gout = fg->nodes[rc_code].My_outputs[0];
                    delete_ele_from_intlist (gout, it_copy);
                    }

		break;
	    case ND_ACCUM_SUM :
	    case ND_ACCUM_MIN :
	    case ND_ACCUM_MAX :
	    case ND_ACCUM_AND :
	    case ND_ACCUM_OR :
                mall_node = create_malloc_tgt_node (fg, id, fg->nodes[nd].outputs[0].ty.totsize, 1);

		/* the size input of the malloc node gets its value from the size input of the accum node */
		if (fg->nodes[nd].inputs[2].is_const)
		    {
		    fg->nodes[mall_node].inputs[0] = fg->nodes[nd].inputs[2];
		    insert_val_before_in_intlist (mall_node, rc_code, &(fg->nodes[id].My_nodes));
		    }
		else
		    {
		    int gin, io_num, grph, snd, spt;
		    assert (fg->nodes[nd].inputs[2].back_edges != NULL);
		    gin = fg->nodes[nd].inputs[2].back_edges->node;
		    assert (fg->nodes[gin].nodetype == ND_G_INPUT);
		    io_num = fg->nodes[gin].Io_num;
		    grph = fg->nodes[gin].My_graph;
		    assert (fg->nodes[grph].inputs[io_num].back_edges != NULL);
		    snd = fg->nodes[grph].inputs[io_num].back_edges->node;
		    spt = fg->nodes[grph].inputs[io_num].back_edges->port;
		    add_edge (fg, snd, spt, mall_node, 0);
		    copy_type_info (&(fg->nodes[mall_node].inputs[0].ty),
		    				&(fg->nodes[snd].outputs[spt].ty));
		    insert_val_after_in_intlist (mall_node, snd, &(fg->nodes[id].My_nodes));
		    }

                add_edge (fg, mall_node, 0, rc_code, j);
                set_type_to_uint (&(fg->nodes[mall_node].outputs[0].ty), 32);
                set_type_to_uint (&(fg->nodes[rc_code].inputs[j].ty), 32);
                set_graphs_input_node_type_to_uint (fg, rc_code, j++, 32);

		create_and_install_trf_array_to_host (fg, nd, mall_node, rc_code, id, 1);
                set_type_to_bool (&(fg->nodes[nd].outputs[0].ty));

                if (and_node > 0)
                    {
                    assert (fg->nodes[nd].outputs[0].targets != NULL);
                    gout = fg->nodes[nd].outputs[0].targets->node;
                    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
                    remove_edge (fg, nd, 0, gout, 0);
                    add_edge (fg, nd, 0, and_node, cnt*2);
                    fg->nodes[and_node].inputs[cnt*2+1].is_const = TRUE;
                    fg->nodes[and_node].inputs[cnt*2+1].constval = copy_to_const ("true");
                    set_type_to_bool (&(fg->nodes[and_node].inputs[cnt*2].ty));
                    set_type_to_bool (&(fg->nodes[and_node].inputs[cnt*2+1].ty));
                    }
                else
                    {
                    assert (fg->nodes[nd].outputs[0].targets != NULL);
                    gout = fg->nodes[nd].outputs[0].targets->node;
                    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
                    set_type_to_bool (&(fg->nodes[gout].inputs[0].ty));
                    }

                cnt++;

                if ((cnt==n_outputs) && (and_node>0))
                    {
                    insert_val_after_in_intlist (and_node, nd, it_copy);
                    gout = fg->nodes[rc_code].My_outputs[0];
                    delete_ele_from_intlist (gout, it_copy);
                    }

		break;
	    case ND_ACCUM_HIST :
                mall_node = create_malloc_tgt_node (fg, id, fg->nodes[nd].outputs[0].ty.totsize, 2);

		/*  the size inputs of the malloc node get their values from
		 *  the accum and hist size inputs of the accum node
		 */
		if (fg->nodes[nd].inputs[2].is_const)
		    {
		    fg->nodes[mall_node].inputs[0] = fg->nodes[nd].inputs[2];
		    insert_val_before_in_intlist (mall_node, rc_code, &(fg->nodes[id].My_nodes));
		    }
		else
		    {
		    int gin, io_num, grph, snd, spt;
		    assert (fg->nodes[nd].inputs[2].back_edges != NULL);
		    gin = fg->nodes[nd].inputs[2].back_edges->node;
		    assert (fg->nodes[gin].nodetype == ND_G_INPUT);
		    io_num = fg->nodes[gin].Io_num;
		    grph = fg->nodes[gin].My_graph;
		    assert (fg->nodes[grph].inputs[io_num].back_edges != NULL);
		    snd = fg->nodes[grph].inputs[io_num].back_edges->node;
		    spt = fg->nodes[grph].inputs[io_num].back_edges->port;
		    add_edge (fg, snd, spt, mall_node, 0);
		    copy_type_info (&(fg->nodes[mall_node].inputs[0].ty),
		    				&(fg->nodes[snd].outputs[spt].ty));
		    insert_val_after_in_intlist (mall_node, snd, &(fg->nodes[id].My_nodes));
		    }
		if (fg->nodes[nd].inputs[3].is_const)
		    {
		    fg->nodes[mall_node].inputs[1] = fg->nodes[nd].inputs[3];
		    insert_val_before_in_intlist (mall_node, rc_code, &(fg->nodes[id].My_nodes));
		    }
		else
		    {
		    int gin, io_num, grph, snd, spt;
		    assert (fg->nodes[nd].inputs[3].back_edges != NULL);
		    gin = fg->nodes[nd].inputs[3].back_edges->node;
		    assert (fg->nodes[gin].nodetype == ND_G_INPUT);
		    io_num = fg->nodes[gin].Io_num;
		    grph = fg->nodes[gin].My_graph;
		    assert (fg->nodes[grph].inputs[io_num].back_edges != NULL);
		    snd = fg->nodes[grph].inputs[io_num].back_edges->node;
		    spt = fg->nodes[grph].inputs[io_num].back_edges->port;
		    add_edge (fg, snd, spt, mall_node, 1);
		    copy_type_info (&(fg->nodes[mall_node].inputs[1].ty),
		    				&(fg->nodes[snd].outputs[spt].ty));
		    insert_val_after_in_intlist (mall_node, snd, &(fg->nodes[id].My_nodes));
		    }

                add_edge (fg, mall_node, 0, rc_code, j);
                set_type_to_uint (&(fg->nodes[mall_node].outputs[0].ty), 32);
                set_type_to_uint (&(fg->nodes[rc_code].inputs[j].ty), 32);
                set_graphs_input_node_type_to_uint (fg, rc_code, j++, 32);

		create_and_install_trf_array_to_host (fg, nd, mall_node, rc_code, id, 2);
                set_type_to_bool (&(fg->nodes[nd].outputs[0].ty));

                if (and_node > 0)
                    {
                    assert (fg->nodes[nd].outputs[0].targets != NULL);
                    gout = fg->nodes[nd].outputs[0].targets->node;
                    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
                    remove_edge (fg, nd, 0, gout, 0);
                    add_edge (fg, nd, 0, and_node, cnt*2);
                    fg->nodes[and_node].inputs[cnt*2+1].is_const = TRUE;
                    fg->nodes[and_node].inputs[cnt*2+1].constval = copy_to_const ("true");
                    set_type_to_bool (&(fg->nodes[and_node].inputs[cnt*2].ty));
                    set_type_to_bool (&(fg->nodes[and_node].inputs[cnt*2+1].ty));
                    }
                else
                    {
                    assert (fg->nodes[nd].outputs[0].targets != NULL);
                    gout = fg->nodes[nd].outputs[0].targets->node;
                    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
                    set_type_to_bool (&(fg->nodes[gout].inputs[0].ty));
                    }

                cnt++;

                if ((cnt==n_outputs) && (and_node>0))
                    {
                    insert_val_after_in_intlist (and_node, nd, it_copy);
                    gout = fg->nodes[rc_code].My_outputs[0];
                    delete_ele_from_intlist (gout, it_copy);
                    }

		break;
	    case ND_REDUCE_SUM :
	    case ND_REDUCE_MIN :
	    case ND_REDUCE_MAX :
	    case ND_REDUCE_AND :
	    case ND_REDUCE_OR :
	    case ND_WRITE_SCALAR :
		mall_node = create_malloc_tgt_node (fg, id, fg->nodes[nd].inputs[0].ty.totsize, 1);
		insert_val_before_in_intlist (mall_node, rc_code, &(fg->nodes[id].My_nodes));
		fg->nodes[mall_node].inputs[0].is_const = TRUE;
		fg->nodes[mall_node].inputs[0].constval = copy_to_const ("1");
    		set_type_to_uint (&(fg->nodes[mall_node].inputs[0].ty), 32);
    		set_type_to_uint (&(fg->nodes[mall_node].outputs[0].ty), 32);
    		set_type_to_uint (&(fg->nodes[rc_code].inputs[j].ty), 32);
		add_edge (fg, mall_node, 0, rc_code, j);
	        set_graphs_input_node_type_to_uint (fg, rc_code, j++, 32);
		create_and_install_trf_scalar_to_host (fg, nd, mall_node, rc_code, id);
		set_type_to_bool (&(fg->nodes[nd].outputs[0].ty));

		if (and_node > 0)
		    {
		    assert (fg->nodes[nd].outputs[0].targets != NULL);
		    gout = fg->nodes[nd].outputs[0].targets->node;
		    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
		    remove_edge (fg, nd, 0, gout, 0);
		    add_edge (fg, nd, 0, and_node, cnt*2);
		    fg->nodes[and_node].inputs[cnt*2+1].is_const = TRUE;
		    fg->nodes[and_node].inputs[cnt*2+1].constval = copy_to_const ("true");
		    set_type_to_bool (&(fg->nodes[and_node].inputs[cnt*2].ty));
		    set_type_to_bool (&(fg->nodes[and_node].inputs[cnt*2+1].ty));
		    }
		else
		    {
		    assert (fg->nodes[nd].outputs[0].targets != NULL);
		    gout = fg->nodes[nd].outputs[0].targets->node;
		    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
		    set_type_to_bool (&(fg->nodes[gout].inputs[0].ty));
		    }

		cnt++;

		if ((cnt==n_outputs) && (and_node>0))
		    {
		    insert_val_after_in_intlist (and_node, nd, it_copy);
		    gout = fg->nodes[rc_code].My_outputs[0];
		    delete_ele_from_intlist (gout, it_copy);
		    }

		break;
	    default :
	        break;
	    }
	}
    }

void build_extents_info (FuncGraph *fg, int rc_code, int id, int idx, Location loc, int *loop_extents)
    {
    IntList *it;
    int gen, found, nd, s_node;
    int i, k;

    /* build the information for the loop's extents and iteration total */
    gen = find_generator_graph (fg, rc_code);
    found = FALSE;

    /* process the internal nodes */
    for (it=fg->nodes[gen].My_nodes; it!=NULL; it=it->link)
	{
	nd = it->val;

        switch (fg->nodes[nd].nodetype)
	    {
	    case ND_WINDOW_GEN :
	        found = TRUE;

		s_node = windowgen_extents_build (fg, nd, id, loc, loop_extents, rc_code, &idx);

		break;
	    case ND_ELE_GEN :
	      found = TRUE;
	      
	      s_node = elegen_extents_build 
		(fg, nd, id, loc, loop_extents, rc_code, &idx);
	      
	      break;
	    case ND_SLICE_GEN :
	      found = TRUE;
	      
	      s_node = slicegen_extents_build 
		(fg, nd, id, loc, loop_extents, rc_code, &idx);
	      
	      break;
	    case ND_SCALAR_GEN :
	        found = TRUE;

		s_node = scalargen_extents_build (fg, nd, id, loc, loop_extents, rc_code, &idx);

	        break;
	    default :
	        break;
	    }

	if (found && fg->nodes[gen].nodetype == ND_DOT_PROD)
	    break;
	}

    add_edge (fg, s_node, 0, rc_code, idx);
    set_type_to_uint (&(fg->nodes[rc_code].inputs[idx].ty), 32);
    set_graphs_input_node_type_to_uint (fg, rc_code, idx, 32);
    }

int scalargen_extents_build (FuncGraph *fg, int nd, int id, Location loc, int *loop_extents, int rc_code, int *pidx)
    {
    int idx, gin_lo, gin_hi, gin_step, g_last, w_loop_ext, i, k, mul_node, s_node;

    idx = *pidx;

    gin_lo = follow_to_input_source (fg, nd, 0);
    gin_hi = follow_to_input_source (fg, nd, 1);
    gin_step = follow_to_input_source (fg, nd, 2);
    g_last = last_in_list (gin_lo, gin_hi, gin_step, fg->nodes[id].My_nodes);

    w_loop_ext = new_ddcf_node (fg, ND_W_LOOP_EXTENT, 3, 1, loc);
    loop_extents[0] = w_loop_ext;
    if (g_last >= 0)
	insert_val_after_in_intlist (w_loop_ext, g_last, &(fg->nodes[id].My_nodes));
    else
	add_to_list (w_loop_ext, &(fg->nodes[id].My_nodes));

    if (fg->nodes[nd].inputs[0].is_const)
	fg->nodes[w_loop_ext].inputs[1] = fg->nodes[nd].inputs[0];
    else
	{
	assert (gin_lo >= 0);
	add_edge (fg, gin_lo, 0, w_loop_ext, 1);
	copy_type_info (&(fg->nodes[w_loop_ext].inputs[1].ty), &(fg->nodes[gin_lo].outputs[0].ty));
	}

    if (fg->nodes[nd].inputs[1].is_const)
	fg->nodes[w_loop_ext].inputs[0] = fg->nodes[nd].inputs[1];
    else
	{
	assert (gin_hi >= 0);
	add_edge (fg, gin_hi, 0, w_loop_ext, 0);
	copy_type_info (&(fg->nodes[w_loop_ext].inputs[0].ty), &(fg->nodes[gin_hi].outputs[0].ty));
	}

    if (fg->nodes[nd].inputs[2].is_const)
	fg->nodes[w_loop_ext].inputs[2] = fg->nodes[nd].inputs[2];
    else
	{
	assert (gin_step >= 0);
	add_edge (fg, gin_step, 0, w_loop_ext, 2);
	copy_type_info (&(fg->nodes[w_loop_ext].inputs[2].ty), &(fg->nodes[gin_step].outputs[0].ty));
	}

    set_type_to_uint (&(fg->nodes[w_loop_ext].outputs[0].ty), 32);
    add_edge (fg, w_loop_ext, 0, rc_code, idx);
    set_graphs_input_node_type_to_uint (fg, rc_code, idx, 32);
    copy_type_info (&(fg->nodes[rc_code].inputs[idx++].ty), &(fg->nodes[w_loop_ext].outputs[0].ty));
    s_node = w_loop_ext;

    for (i=1, k=3; i<fg->nodes[nd].num_outputs; i++, k+=3)
	{
	gin_lo = follow_to_input_source (fg, nd, k);
	gin_hi = follow_to_input_source (fg, nd, k+1);
	gin_step = follow_to_input_source (fg, nd, k+2);
	g_last = last_in_list (gin_lo, gin_hi, gin_step, fg->nodes[id].My_nodes);

	w_loop_ext = new_ddcf_node (fg, ND_W_LOOP_EXTENT, 3, 1, loc);
	loop_extents[i] = w_loop_ext;
	if (g_last >= 0)
	    insert_val_after_in_intlist (w_loop_ext, g_last, &(fg->nodes[id].My_nodes));
	else
	    add_to_list (w_loop_ext, &(fg->nodes[id].My_nodes));

	if (fg->nodes[nd].inputs[k].is_const)
	    fg->nodes[w_loop_ext].inputs[1] = fg->nodes[nd].inputs[k];
	else
	    {
	    assert (gin_lo >= 0);
	    add_edge (fg, gin_lo, 0, w_loop_ext, 1);
	    copy_type_info (&(fg->nodes[w_loop_ext].inputs[1].ty), &(fg->nodes[gin_lo].outputs[0].ty));
	    }

	if (fg->nodes[nd].inputs[k+1].is_const)
	    fg->nodes[w_loop_ext].inputs[0] = fg->nodes[nd].inputs[k+1];
	else
	    {
	    assert (gin_hi >= 0);
	    add_edge (fg, gin_hi, 0, w_loop_ext, 0);
	    copy_type_info (&(fg->nodes[w_loop_ext].inputs[0].ty), &(fg->nodes[gin_hi].outputs[0].ty));
	    }

	if (fg->nodes[nd].inputs[k+2].is_const)
	    fg->nodes[w_loop_ext].inputs[2] = fg->nodes[nd].inputs[k+2];
	else
	    {
	    assert (gin_step >= 0);
	    add_edge (fg, gin_step, 0, w_loop_ext, 2);
	    copy_type_info (&(fg->nodes[w_loop_ext].inputs[2].ty), &(fg->nodes[gin_step].outputs[0].ty));
	    }

	set_type_to_uint (&(fg->nodes[w_loop_ext].outputs[0].ty), 32);

	add_edge (fg, w_loop_ext, 0, rc_code, idx);
	set_graphs_input_node_type_to_uint (fg, rc_code, idx, 32);
	copy_type_info (&(fg->nodes[rc_code].inputs[idx++].ty), &(fg->nodes[w_loop_ext].outputs[0].ty));
	mul_node = new_ddcf_node (fg, ND_MUL, 2, 1, loc);
	insert_val_after_in_intlist (mul_node, s_node, &(fg->nodes[id].My_nodes));
	add_edge (fg, s_node, 0, mul_node, 0);
	set_type_to_uint (&(fg->nodes[mul_node].inputs[0].ty), 32);
	add_edge (fg, w_loop_ext, 0, mul_node, 1);
	set_type_to_uint (&(fg->nodes[mul_node].inputs[1].ty), 32);
	set_type_to_uint (&(fg->nodes[mul_node].outputs[0].ty), 32);
	s_node = mul_node;
	}

    *pidx = idx;
    return s_node;
    }

int windowgen_extents_build (FuncGraph *fg, int nd, int id, Location loc, int *loop_extents, int rc_code, int *pidx)
    {
    int idx, extents_nd, w_loop_ext, gin_step, g_last, s_node, i, k, mul_node;

    idx = *pidx;

    extents_nd = find_extents_of_gen_input (fg, nd);

    w_loop_ext = new_ddcf_node (fg, ND_W_LOOP_EXTENT, 3, 1, loc);
    loop_extents[0] = w_loop_ext;
    add_edge (fg, extents_nd, 0, w_loop_ext, 0);
    set_type_to_uint (&(fg->nodes[w_loop_ext].inputs[0].ty), 32);
    set_type_to_uint (&(fg->nodes[w_loop_ext].outputs[0].ty), 32);
    fg->nodes[w_loop_ext].inputs[1] = fg->nodes[nd].inputs[1];
    if (!fg->nodes[nd].inputs[2].is_const)
	{
	gin_step = follow_to_input_source (fg, nd, 2);
	g_last = last_in_list (-1, extents_nd, gin_step, fg->nodes[id].My_nodes);
	insert_val_after_in_intlist (w_loop_ext, g_last, &(fg->nodes[id].My_nodes));
	add_edge (fg, gin_step, 0, w_loop_ext, 2);
	copy_type_info (&(fg->nodes[w_loop_ext].inputs[2].ty), &(fg->nodes[gin_step].outputs[0].ty));
	}
    else
	{
	fg->nodes[w_loop_ext].inputs[2] = fg->nodes[nd].inputs[2];
	insert_val_after_in_intlist (w_loop_ext, extents_nd, &(fg->nodes[id].My_nodes));
	}

    add_edge (fg, w_loop_ext, 0, rc_code, idx);
    set_graphs_input_node_type_to_uint (fg, rc_code, idx, 32);
    copy_type_info (&(fg->nodes[rc_code].inputs[idx++].ty), &(fg->nodes[w_loop_ext].outputs[0].ty));
    s_node = w_loop_ext;

    for (i=1, k=3; i<fg->nodes[nd].inputs[0].ty.dims; i++, k+=2)
	{
	w_loop_ext = new_ddcf_node (fg, ND_W_LOOP_EXTENT, 3, 1, loc);
	loop_extents[i] = w_loop_ext;
	add_edge (fg, extents_nd, i, w_loop_ext, 0);
	set_type_to_uint (&(fg->nodes[w_loop_ext].inputs[0].ty), 32);
	set_type_to_uint (&(fg->nodes[w_loop_ext].outputs[0].ty), 32);
	fg->nodes[w_loop_ext].inputs[1] = fg->nodes[nd].inputs[k];
	if (!fg->nodes[nd].inputs[k+1].is_const)
	    {
	    gin_step = follow_to_input_source (fg, nd, k+1);
	    g_last = last_in_list (-1, extents_nd, gin_step, fg->nodes[id].My_nodes);
	    insert_val_after_in_intlist (w_loop_ext, g_last, &(fg->nodes[id].My_nodes));
	    add_edge (fg, gin_step, 0, w_loop_ext, 2);
	    copy_type_info (&(fg->nodes[w_loop_ext].inputs[2].ty), &(fg->nodes[gin_step].outputs[0].ty));
	    }
	else
	    {
	    fg->nodes[w_loop_ext].inputs[2] = fg->nodes[nd].inputs[k+1];
	    insert_val_after_in_intlist (w_loop_ext, extents_nd, &(fg->nodes[id].My_nodes));
	    }
	add_edge (fg, w_loop_ext, 0, rc_code, idx);
	set_graphs_input_node_type_to_uint (fg, rc_code, idx, 32);
	copy_type_info (&(fg->nodes[rc_code].inputs[idx++].ty), &(fg->nodes[w_loop_ext].outputs[0].ty));
	mul_node = new_ddcf_node (fg, ND_MUL, 2, 1, loc);
	insert_val_after_in_intlist (mul_node, s_node, &(fg->nodes[id].My_nodes));
	add_edge (fg, s_node, 0, mul_node, 0);
	set_type_to_uint (&(fg->nodes[mul_node].inputs[0].ty), 32);
	add_edge (fg, w_loop_ext, 0, mul_node, 1);
	set_type_to_uint (&(fg->nodes[mul_node].inputs[1].ty), 32);
	set_type_to_uint (&(fg->nodes[mul_node].outputs[0].ty), 32);
	s_node = mul_node;
	}

    *pidx = idx;
    return s_node;
    }

int elegen_extents_build (FuncGraph *fg, int nd, int id, Location loc, 
			  int *loop_extents, int rc_code, int *pidx)
{
  int extents_nd, w_loop_ext, gin_step, g_last, i, k, mul_node, s_node, idx;

  idx = *pidx;

  extents_nd = find_extents_of_gen_input (fg, nd);

  w_loop_ext = new_ddcf_node (fg, ND_W_LOOP_EXTENT, 3, 1, loc);
  loop_extents[0] = w_loop_ext;
  add_edge (fg, extents_nd, 0, w_loop_ext, 0);
  set_type_to_uint (&(fg->nodes[w_loop_ext].inputs[0].ty), 32);
  set_type_to_uint (&(fg->nodes[w_loop_ext].outputs[0].ty), 32);
  fg->nodes[w_loop_ext].inputs[1].is_const = TRUE;
  fg->nodes[w_loop_ext].inputs[1].constval = copy_to_const ("1");
  set_type_to_uint (&(fg->nodes[w_loop_ext].inputs[1].ty), 32);

  if (!fg->nodes[nd].inputs[1].is_const)
    {
      gin_step = follow_to_input_source (fg, nd, 1);
      g_last = last_in_list (-1, extents_nd, gin_step, fg->nodes[id].My_nodes);
      insert_val_after_in_intlist (w_loop_ext, g_last, &(fg->nodes[id].My_nodes));
      add_edge (fg, gin_step, 0, w_loop_ext, 2);
      copy_type_info (&(fg->nodes[w_loop_ext].inputs[2].ty), &(fg->nodes[gin_step].outputs[0].ty));
    }
  else
    {
      fg->nodes[w_loop_ext].inputs[2] = fg->nodes[nd].inputs[1];
      insert_val_after_in_intlist (w_loop_ext, extents_nd, &(fg->nodes[id].My_nodes));
    }
  add_edge (fg, w_loop_ext, 0, rc_code, idx);
  set_graphs_input_node_type_to_uint (fg, rc_code, idx, 32);
  copy_type_info (&(fg->nodes[rc_code].inputs[idx++].ty), &(fg->nodes[w_loop_ext].outputs[0].ty));
  s_node = w_loop_ext;

  for (i=1, k=2; i<fg->nodes[nd].inputs[0].ty.dims; i++, k++)
    {
      w_loop_ext = new_ddcf_node (fg, ND_W_LOOP_EXTENT, 3, 1, loc);
      loop_extents[i] = w_loop_ext;
      add_edge (fg, extents_nd, i, w_loop_ext, 0);
      set_type_to_uint (&(fg->nodes[w_loop_ext].inputs[0].ty), 32);
      set_type_to_uint (&(fg->nodes[w_loop_ext].outputs[0].ty), 32);
      fg->nodes[w_loop_ext].inputs[1].is_const = TRUE;
      fg->nodes[w_loop_ext].inputs[1].constval = copy_to_const ("1");
      set_type_to_uint (&(fg->nodes[w_loop_ext].inputs[1].ty), 32);
      if (!fg->nodes[nd].inputs[k].is_const)
	{
	  gin_step = follow_to_input_source (fg, nd, k);
	  g_last = last_in_list (-1, extents_nd, gin_step, fg->nodes[id].My_nodes);
	  insert_val_after_in_intlist (w_loop_ext, g_last, &(fg->nodes[id].My_nodes));
	  add_edge (fg, gin_step, 0, w_loop_ext, 2);
	  copy_type_info (&(fg->nodes[w_loop_ext].inputs[2].ty), &(fg->nodes[gin_step].outputs[0].ty));
	}
      else
	{
	  fg->nodes[w_loop_ext].inputs[2] = fg->nodes[nd].inputs[k];
	  insert_val_after_in_intlist (w_loop_ext, extents_nd, &(fg->nodes[id].My_nodes));
	}
      add_edge (fg, w_loop_ext, 0, rc_code, idx);
      set_graphs_input_node_type_to_uint (fg, rc_code, idx, 32);
      copy_type_info (&(fg->nodes[rc_code].inputs[idx++].ty), &(fg->nodes[w_loop_ext].outputs[0].ty));
      mul_node = new_ddcf_node (fg, ND_MUL, 2, 1, loc);
      insert_val_after_in_intlist (mul_node, s_node, &(fg->nodes[id].My_nodes));
      add_edge (fg, s_node, 0, mul_node, 0);
      set_type_to_uint (&(fg->nodes[mul_node].inputs[0].ty), 32);
      add_edge (fg, w_loop_ext, 0, mul_node, 1);
      set_type_to_uint (&(fg->nodes[mul_node].inputs[1].ty), 32);
      set_type_to_uint (&(fg->nodes[mul_node].outputs[0].ty), 32);
      s_node = mul_node;
    }

  *pidx = idx;
  return s_node;
}

int slicegen_extents_build (FuncGraph *fg, int nd, int id, Location loc, 
			    int *loop_extents, int rc_code, int *pidx)
{
  int extents_nd, w_loop_ext, gin_step, g_last, mul_node, s_node, idx;
  int slice_column;

  assert (fg->nodes[nd].inputs[0].ty.dims == 2);
  assert (fg->nodes[nd].outputs[0].ty.kind != Scalar);

  idx = *pidx;

  slice_column = (fg->nodes[nd].Reftypes[0] == ':');

  extents_nd = find_extents_of_gen_input (fg, nd);

  w_loop_ext = new_ddcf_node (fg, ND_W_LOOP_EXTENT, 3, 1, loc);
  loop_extents[0] = w_loop_ext;
  if (slice_column)
    add_edge (fg, extents_nd, 1, w_loop_ext, 0);
  else
    add_edge (fg, extents_nd, 0, w_loop_ext, 0);
  set_type_to_uint (&(fg->nodes[w_loop_ext].inputs[0].ty), 32);
  set_type_to_uint (&(fg->nodes[w_loop_ext].outputs[0].ty), 32);
  fg->nodes[w_loop_ext].inputs[1].is_const = TRUE;
  fg->nodes[w_loop_ext].inputs[1].constval = copy_to_const ("1");
  set_type_to_uint (&(fg->nodes[w_loop_ext].inputs[1].ty), 32);

  if (!fg->nodes[nd].inputs[1].is_const)
    {
      gin_step = follow_to_input_source (fg, nd, 1);
      g_last = last_in_list (-1, extents_nd, gin_step, fg->nodes[id].My_nodes);
      insert_val_after_in_intlist (w_loop_ext, g_last, &(fg->nodes[id].My_nodes));
      add_edge (fg, gin_step, 0, w_loop_ext, 2);
      copy_type_info (&(fg->nodes[w_loop_ext].inputs[2].ty), &(fg->nodes[gin_step].outputs[0].ty));
    }
  else
    {
      fg->nodes[w_loop_ext].inputs[2] = fg->nodes[nd].inputs[1];
      insert_val_after_in_intlist (w_loop_ext, extents_nd, &(fg->nodes[id].My_nodes));
    }
  add_edge (fg, w_loop_ext, 0, rc_code, idx);
  set_graphs_input_node_type_to_uint (fg, rc_code, idx, 32);
  copy_type_info (&(fg->nodes[rc_code].inputs[idx++].ty), &(fg->nodes[w_loop_ext].outputs[0].ty));
  s_node = w_loop_ext;

  *pidx = idx;
  return s_node;
}

/*  'red_node' is the reduction node being processed
 *  'mall_node' is the node that mallocs the target array in the RC
 *  'rc_code' is the graph of the loop going onto the RC
 *  'encl_node' is the RC_COMPUTE graph enclosing the loop's graph and containing interface code
 */
void create_and_install_trf_array_to_host (FuncGraph *fg, int red_node,
			int mall_node, int rc_code, int encl_node, int result_rank)
    {
    Edge *edg;
    int trf_to_host, nd, io_num, gout;
    int num_inputs, i, j, k, m;
    char str[128];

    num_inputs = 2 + result_rank;
    trf_to_host = new_ddcf_node (fg, ND_TRANSFER_TO_HOST_RET_ARRAY, num_inputs, 1, fg->nodes[encl_node].loc);
    assert (fg->nodes[red_node].outputs[0].targets != NULL);
    nd = fg->nodes[red_node].outputs[0].targets->node;
    io_num = fg->nodes[nd].Io_num;
    assert (fg->nodes[rc_code].outputs[io_num].targets != NULL);

    /* find the edge that targets a ND_G_OUTPUT node */
    for (edg=fg->nodes[rc_code].outputs[io_num].targets; edg!=NULL; edg=edg->link)
        if (fg->nodes[edg->node].nodetype == ND_G_OUTPUT)
	    {
	    gout = edg->node;
	    break;
	    }
    assert (edg != NULL);

    remove_edge (fg, rc_code, io_num, gout, 0);
    add_edge (fg, rc_code, 0, trf_to_host, 0);
    set_type_to_bool (&(fg->nodes[rc_code].outputs[0].ty));
    set_type_to_bool (&(fg->nodes[trf_to_host].inputs[0].ty));
    add_edge (fg, trf_to_host, 0, gout, 0);
    add_edge (fg, mall_node, 0, trf_to_host, 1);
    set_type_to_uint (&(fg->nodes[trf_to_host].inputs[1].ty), 32);

    /* get the sizes for the 'trf_to_host' node from 'mall_node's inputs */
    for (i=0; i<result_rank; i++)
        if (fg->nodes[mall_node].inputs[i].is_const)
	    fg->nodes[trf_to_host].inputs[i+2] = fg->nodes[mall_node].inputs[i];
        else
            {
	    int snd, spt;
	    assert (fg->nodes[mall_node].inputs[i].back_edges != NULL);
	    snd = fg->nodes[mall_node].inputs[i].back_edges->node;
	    spt = fg->nodes[mall_node].inputs[i].back_edges->port;
            add_edge (fg, snd, spt, trf_to_host, i+2);
	    copy_type_info (&(fg->nodes[trf_to_host].inputs[i+2].ty), &(fg->nodes[snd].outputs[spt].ty));
	    }

    copy_type_info (&(fg->nodes[trf_to_host].outputs[0].ty), &(fg->nodes[gout].inputs[0].ty));

    insert_val_after_in_intlist (trf_to_host, rc_code, &(fg->nodes[encl_node].My_nodes));
    }

void create_and_install_buff_terminate (FuncGraph *fg, int wr_buf,
		int mall_node0, int mall_node1, int rc_code, int encl_node)
    {
    Edge *edg;
    int buf_term, nd, io_num, gout;

    buf_term = new_ddcf_node (fg, ND_BUFFER_TERMINATE, 3, 1, fg->nodes[encl_node].loc);
    assert (fg->nodes[wr_buf].outputs[0].targets != NULL);
    nd = fg->nodes[wr_buf].outputs[0].targets->node;
    io_num = fg->nodes[nd].Io_num;
    assert (fg->nodes[rc_code].outputs[io_num].targets != NULL);

    /* find the edge that targets a ND_G_OUTPUT node */
    for (edg=fg->nodes[rc_code].outputs[io_num].targets; edg!=NULL; edg=edg->link)
        if (fg->nodes[edg->node].nodetype == ND_G_OUTPUT)
	    {
	    gout = edg->node;
	    break;
	    }
    assert (edg != NULL);

    remove_edge (fg, rc_code, io_num, gout, 0);
    add_edge (fg, rc_code, 0, buf_term, 0);
    set_type_to_bool (&(fg->nodes[rc_code].outputs[0].ty));
    set_type_to_bool (&(fg->nodes[buf_term].inputs[0].ty));
    add_edge (fg, buf_term, 0, gout, 0);
    add_edge (fg, mall_node0, 0, buf_term, 1);
    set_type_to_uint (&(fg->nodes[buf_term].inputs[1].ty), 32);
    add_edge (fg, mall_node1, 0, buf_term, 2);
    set_type_to_uint (&(fg->nodes[buf_term].inputs[2].ty), 32);

    copy_type_info (&(fg->nodes[buf_term].outputs[0].ty), &(fg->nodes[gout].inputs[0].ty));

    insert_val_after_in_intlist (buf_term, rc_code, &(fg->nodes[encl_node].My_nodes));
    }

void create_and_install_trf_scalar_to_host (FuncGraph *fg,
		int red_node, int mall_node, int rc_code, int encl_node)
    {
    Edge *edg;
    int trf_to_host, nd, io_num, gout;

    trf_to_host = new_ddcf_node (fg, ND_TRANSFER_TO_HOST_SCALAR, 2, 1, fg->nodes[encl_node].loc);
    assert (fg->nodes[red_node].outputs[0].targets != NULL);
    nd = fg->nodes[red_node].outputs[0].targets->node;
    io_num = fg->nodes[nd].Io_num;
    assert (fg->nodes[rc_code].outputs[io_num].targets != NULL);

    /* find the edge that targets a ND_G_OUTPUT node */
    for (edg=fg->nodes[rc_code].outputs[io_num].targets; edg!=NULL; edg=edg->link)
        if (fg->nodes[edg->node].nodetype == ND_G_OUTPUT)
	    {
	    gout = edg->node;
	    break;
	    }
    assert (edg != NULL);

    remove_edge (fg, rc_code, io_num, gout, 0);
    add_edge (fg, rc_code, 0, trf_to_host, 0);
    set_type_to_bool (&(fg->nodes[rc_code].outputs[0].ty));
    set_type_to_bool (&(fg->nodes[trf_to_host].inputs[0].ty));
    add_edge (fg, trf_to_host, 0, gout, 0);
    add_edge (fg, mall_node, 0, trf_to_host, 1);
    set_type_to_uint (&(fg->nodes[trf_to_host].inputs[1].ty), 32);

    copy_type_info (&(fg->nodes[trf_to_host].outputs[0].ty), &(fg->nodes[gout].inputs[0].ty));

    insert_val_after_in_intlist (trf_to_host, rc_code, &(fg->nodes[encl_node].My_nodes));
    }

/*  we have to climb out of the generator graph and out of the loop graph
 *  and through the ND_MALL_XFER node to find the input that feeds this
 *  generator and the ND_EXTENTS node; 'id' is the simple generator node;
 */
int find_extents_of_gen_input (FuncGraph *fg, int id)
    {
    Edge *tg;
    int nd, gen, loop, io_num;

    assert (fg->nodes[id].inputs[0].back_edges != NULL);
    nd = fg->nodes[id].inputs[0].back_edges->node;
    assert (fg->nodes[nd].nodetype == ND_G_INPUT);
    io_num = fg->nodes[nd].Io_num;
    gen = fg->nodes[nd].My_graph;
    assert (fg->nodes[gen].inputs[io_num].back_edges != NULL);
    nd = fg->nodes[gen].inputs[io_num].back_edges->node;
    assert (fg->nodes[nd].nodetype == ND_G_INPUT);
    io_num = fg->nodes[nd].Io_num;
    loop = fg->nodes[nd].My_graph;
    assert (fg->nodes[loop].inputs[io_num].back_edges != NULL);
    nd = fg->nodes[loop].inputs[io_num].back_edges->node;
    assert (fg->nodes[nd].nodetype == ND_MALL_XFER);
    assert (fg->nodes[nd].inputs[0].back_edges != NULL);
    nd = fg->nodes[nd].inputs[0].back_edges->node;
    assert (fg->nodes[nd].nodetype == ND_G_INPUT);

    for (tg=fg->nodes[nd].outputs[0].targets; tg!=NULL; tg=tg->link)
        if (fg->nodes[tg->node].nodetype == ND_EXTENTS)
	    break;

    assert (tg != NULL);
    return tg->node;
    }

/*  each scalar input stays as is (counts for one input)
 *  each array input gets an input for the address and one for each dimension
 *  there is one input for each loop dimension extent
 *  there is one input for the total number of loop iterations
 *  there is one input for each output, giving its address
 */
int num_inputs_needed (FuncGraph *fg, int id, int rank)
    {
    IntList *it;
    int i, cnt, nd;

    /* handle the loop's inputs */
    for (cnt=0, i=0; i<fg->nodes[id].num_inputs; i++)
        if (fg->nodes[id].inputs[i].ty.kind == Array)
	    cnt += 1 + fg->nodes[id].inputs[i].ty.dims;
	else
	    cnt++;

    cnt += rank + 1;

    /* handle the loop's outputs */
    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
	{
	nd = it->val;

        switch (fg->nodes[nd].nodetype)
	    {
	    case ND_ACCUM_SUM :
	    case ND_ACCUM_MIN :
	    case ND_ACCUM_MAX :
	    case ND_ACCUM_AND :
	    case ND_ACCUM_OR :
	    case ND_ACCUM_HIST :
	    case ND_REDUCE_HIST :
	    case ND_REDUCE_SUM :
	    case ND_REDUCE_AND :
	    case ND_REDUCE_OR :
	    case ND_REDUCE_MIN :
	    case ND_REDUCE_MAX :
	    case ND_CONSTRUCT_ARRAY :
	    case ND_CONSTRUCT_CONCAT :
	    case ND_CONSTRUCT_TILE :
	        cnt++;
		break;
	    case ND_CONSTRUCT_CONCAT_MASKED_MANY :
	        cnt += 2;
		break;
	    case ND_NEXT :
	        if (fg->nodes[nd].outputs[0].targets != NULL)
		    cnt++;
	        break;
	    default :
	        break;
	    }
	}

    return cnt;
    }

int find_next_nodes_output (FuncGraph *fg, int nd)
    {
    Edge *ed;
    int onode, snd, spt;

    onode = -1;

    assert (fg->nodes[nd].inputs[0].back_edges != NULL);
    snd = fg->nodes[nd].inputs[0].back_edges->node;
    spt = fg->nodes[nd].inputs[0].back_edges->port;

    for (ed=fg->nodes[snd].outputs[spt].targets; ed!=NULL; ed=ed->link)
        if (fg->nodes[ed->node].nodetype == ND_G_OUTPUT)
            {
            onode = ed->node;
            break;
            }

    return onode;
    }

int enclose_graph (FuncGraph *fg, int id)
    {
    int new_node, num_inputs, num_outputs;
    int i, gin, gout;

    num_inputs = fg->nodes[id].num_inputs;
    num_outputs = fg->nodes[id].num_outputs;
    new_node = new_ddcf_node (fg, ND_RC_COMPUTE, num_inputs, num_outputs, fg->nodes[id].loc);
    fg->nodes[new_node].My_inputs = make_io_array (num_inputs);
    fg->nodes[new_node].My_outputs = make_io_array (num_outputs);
    add_to_list (id, &(fg->nodes[new_node].My_nodes));

    for (i=0; i<num_inputs; i++)
        {
	/* retarget incoming edges to the new graph instead of the loop */
	retarget (fg, fg->nodes[id].inputs[i].back_edges, id, i, new_node, i);
	fg->nodes[new_node].inputs[i].back_edges = fg->nodes[id].inputs[i].back_edges;
	fg->nodes[id].inputs[i].back_edges = NULL;

	/* create an INPUT node for the new graph */
	gin = new_ddcf_node (fg, ND_G_INPUT, 0, 1, fg->nodes[id].loc);
	fg->nodes[gin].Io_num = i;
	fg->nodes[gin].My_graph = new_node;
	add_edge (fg, gin, 0, id, i);

	fg->nodes[new_node].My_inputs[i] = gin;

	copy_type_info (&(fg->nodes[new_node].inputs[i].ty), &(fg->nodes[id].inputs[i].ty));
	copy_type_info (&(fg->nodes[gin].outputs[0].ty), &(fg->nodes[id].inputs[i].ty));

	add_to_list (gin, &(fg->nodes[new_node].My_nodes));
	}

    for (i=0; i<num_outputs; i++)
        {
	retarget_backedges (fg, fg->nodes[id].outputs[i].targets, id, i, new_node, i);
	fg->nodes[new_node].outputs[i].targets = fg->nodes[id].outputs[i].targets;
	fg->nodes[id].outputs[i].targets = NULL;

	gout = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, fg->nodes[id].loc);
	fg->nodes[gout].Io_num = i;
	fg->nodes[gout].My_graph = new_node;
	add_edge (fg, id, i, gout, 0);

	fg->nodes[new_node].My_outputs[i] = gout;

	copy_type_info (&(fg->nodes[new_node].outputs[i].ty), &(fg->nodes[id].outputs[i].ty));
	copy_type_info (&(fg->nodes[gout].inputs[0].ty), &(fg->nodes[id].outputs[i].ty));
	}

    return new_node;
    }

int convert_loop_return_node_for_dfg (FuncGraph *fg, int nd, int id, int sz_index,
			int res_index, int loop_rank, IntList **list_copy)
    {
    InputPort *new_inputs;
    int gen, gin, snd, spt, i;
    int cast_node;

    switch (fg->nodes[nd].nodetype)
	    {
	    case ND_CONSTRUCT_CONCAT :
	    case ND_CONSTRUCT_CONCAT_MASKED :
		assert (0);
		break;
	    case ND_CONSTRUCT_CONCAT_MASKED_MANY :
		{
		int n_ins, iter_cnt;
		fg->nodes[nd].nodetype = ND_WRITE_VALS_TO_BUFFER;
		n_ins = fg->nodes[nd].num_inputs;
		new_inputs = make_input_ports (n_ins + 4);
		for (i=0; i<n_ins; i++)
		    new_inputs[i] = fg->nodes[nd].inputs[i];
		SacFree (fg->nodes[nd].inputs)
		fg->nodes[nd].inputs = new_inputs;
		fg->nodes[nd].num_inputs += 4;

		make_uint_const_input (fg, nd, n_ins,
			buf_size_in_words * 32 / fg->nodes[nd].inputs[0].ty.totsize);

		/* hook up the buffer address input */
		gin = fg->nodes[id].My_inputs[res_index];
		add_edge (fg, gin, 0, nd, n_ins+1);
		set_type_to_uint (&(fg->nodes[nd].inputs[n_ins+1].ty), 32);

		/* hook up the counter address input */
		gin = fg->nodes[id].My_inputs[res_index+1];
		add_edge (fg, gin, 0, nd, n_ins+2);
		set_type_to_uint (&(fg->nodes[nd].inputs[n_ins+2].ty), 32);

		/* hook up the iteration count */
		iter_cnt = fg->nodes[id].num_inputs - 1;
		gin = fg->nodes[id].My_inputs[iter_cnt];
		add_edge (fg, gin, 0, nd, n_ins+3);
		set_type_to_uint (&(fg->nodes[nd].inputs[n_ins+3].ty), 32);

		res_index += 2;
		}
	        break;
	    case ND_CONSTRUCT_TILE :
		/* combine ARR_DEF and TILE nodes into one node */
		assert (fg->nodes[nd].inputs[0].back_edges != NULL);
		snd = fg->nodes[nd].inputs[0].back_edges->node;
		assert (fg->nodes[snd].nodetype == ND_ARR_DEF);

		coalesce_tile (fg, nd, snd, id, res_index, loop_rank, list_copy);

		res_index++;
	        break;
	    case ND_CONSTRUCT_ARRAY :
		switch (loop_rank)
		    {
		    case 1 :
			{
			int lp_extent_idx;
			fg->nodes[nd].nodetype = ND_WRITE_TILE_1D_1D;
			new_inputs = make_input_ports (4);
			new_inputs[0] = fg->nodes[nd].inputs[0];
			SacFree (fg->nodes[nd].inputs)
			fg->nodes[nd].inputs = new_inputs;
			fg->nodes[nd].num_inputs = 4;
			gin = fg->nodes[id].My_inputs[res_index];
			add_edge (fg, gin, 0, nd, 1);
    			lp_extent_idx = fg->nodes[id].num_inputs - 2;
			gin = fg->nodes[id].My_inputs[lp_extent_idx];
			add_edge (fg, gin, 0, nd, 2);
			make_uint_const_input (fg, nd, 3, 1);
			set_type_to_uint (&(fg->nodes[nd].inputs[1].ty), 32);
			set_type_to_uint (&(fg->nodes[nd].inputs[2].ty), 32);
			set_type_to_uint (&(fg->nodes[nd].inputs[3].ty), 32);
			}
		        break;
		    case 2 :
			{
			int lp_extent_idx;
			fg->nodes[nd].nodetype = ND_WRITE_TILE_2D_2D;
			new_inputs = make_input_ports (6);
			new_inputs[0] = fg->nodes[nd].inputs[0];
			SacFree (fg->nodes[nd].inputs)
			fg->nodes[nd].inputs = new_inputs;
			fg->nodes[nd].num_inputs = 6;
			gin = fg->nodes[id].My_inputs[res_index];
			add_edge (fg, gin, 0, nd, 1);
    			lp_extent_idx = fg->nodes[id].num_inputs - 3;
			gin = fg->nodes[id].My_inputs[lp_extent_idx];
			add_edge (fg, gin, 0, nd, 2);
    			lp_extent_idx = fg->nodes[id].num_inputs - 2;
			gin = fg->nodes[id].My_inputs[lp_extent_idx];
			add_edge (fg, gin, 0, nd, 3);
			make_uint_const_input (fg, nd, 4, 1);
			make_uint_const_input (fg, nd, 5, 1);
			set_type_to_uint (&(fg->nodes[nd].inputs[1].ty), 32);
			set_type_to_uint (&(fg->nodes[nd].inputs[2].ty), 32);
			set_type_to_uint (&(fg->nodes[nd].inputs[3].ty), 32);
			set_type_to_uint (&(fg->nodes[nd].inputs[4].ty), 32);
			set_type_to_uint (&(fg->nodes[nd].inputs[5].ty), 32);
			}
		        break;
		    case 3 :
			{
			int lp_extent_idx;
			fg->nodes[nd].nodetype = ND_WRITE_TILE_3D_3D;
			new_inputs = make_input_ports (8);
			new_inputs[0] = fg->nodes[nd].inputs[0];
			SacFree (fg->nodes[nd].inputs)
			fg->nodes[nd].inputs = new_inputs;
			fg->nodes[nd].num_inputs = 8;
			gin = fg->nodes[id].My_inputs[res_index];
			add_edge (fg, gin, 0, nd, 1);
    			lp_extent_idx = fg->nodes[id].num_inputs - 4;
			gin = fg->nodes[id].My_inputs[lp_extent_idx];
			add_edge (fg, gin, 0, nd, 2);
    			lp_extent_idx = fg->nodes[id].num_inputs - 3;
			gin = fg->nodes[id].My_inputs[lp_extent_idx];
			add_edge (fg, gin, 0, nd, 3);
    			lp_extent_idx = fg->nodes[id].num_inputs - 2;
			gin = fg->nodes[id].My_inputs[lp_extent_idx];
			add_edge (fg, gin, 0, nd, 4);
			make_uint_const_input (fg, nd, 5, 1);
			make_uint_const_input (fg, nd, 6, 1);
			make_uint_const_input (fg, nd, 7, 1);
			set_type_to_uint (&(fg->nodes[nd].inputs[1].ty), 32);
			set_type_to_uint (&(fg->nodes[nd].inputs[2].ty), 32);
			set_type_to_uint (&(fg->nodes[nd].inputs[3].ty), 32);
			set_type_to_uint (&(fg->nodes[nd].inputs[4].ty), 32);
			set_type_to_uint (&(fg->nodes[nd].inputs[5].ty), 32);
			set_type_to_uint (&(fg->nodes[nd].inputs[6].ty), 32);
			set_type_to_uint (&(fg->nodes[nd].inputs[7].ty), 32);
			}
		        break;
		    default :
		        assert (0);
		    }

		res_index++;
		break;
	    case ND_REDUCE_HIST :
		fg->nodes[nd].nodetype = ND_HIST_VALUES;
		new_inputs = make_input_ports (5);
		new_inputs[0] = fg->nodes[nd].inputs[0];
		new_inputs[1] = fg->nodes[nd].inputs[1];
		new_inputs[2] = fg->nodes[nd].inputs[2];
		SacFree (fg->nodes[nd].inputs)
		fg->nodes[nd].inputs = new_inputs;
		fg->nodes[nd].num_inputs = 5;
		gin = fg->nodes[id].My_inputs[res_index];
		add_edge (fg, gin, 0, nd, 3);
		gin = fg->nodes[id].My_inputs[sz_index];
		add_edge (fg, gin, 0, nd, 4);
		set_type_to_uint (&(fg->nodes[nd].inputs[3].ty), 32);
		set_type_to_uint (&(fg->nodes[nd].inputs[4].ty), 32);
		res_index++;
	        break;
	    case ND_ACCUM_HIST :
		fg->nodes[nd].nodetype = ND_ACCUM_HIST_VALUES;
		new_inputs = make_input_ports (7);
		new_inputs[0] = fg->nodes[nd].inputs[0];
		new_inputs[1] = fg->nodes[nd].inputs[1];
		new_inputs[2] = fg->nodes[nd].inputs[2];
		new_inputs[3] = fg->nodes[nd].inputs[3];
		new_inputs[4] = fg->nodes[nd].inputs[4];
		SacFree (fg->nodes[nd].inputs)
		fg->nodes[nd].inputs = new_inputs;
		fg->nodes[nd].num_inputs = 7;
		gin = fg->nodes[id].My_inputs[res_index];
		add_edge (fg, gin, 0, nd, 5);
		gin = fg->nodes[id].My_inputs[sz_index];
		add_edge (fg, gin, 0, nd, 6);
		set_type_to_uint (&(fg->nodes[nd].inputs[5].ty), 32);
		set_type_to_uint (&(fg->nodes[nd].inputs[6].ty), 32);
		res_index++;
	        break;
	    case ND_ACCUM_SUM :
		if (is_unsigned (fg->nodes[nd].inputs[0].ty.type))
		    fg->nodes[nd].nodetype = ND_ACCUM_USUM_VALUES;
		else
		    fg->nodes[nd].nodetype = ND_ACCUM_ISUM_VALUES;
		new_inputs = make_input_ports (6);
		new_inputs[0] = fg->nodes[nd].inputs[0];
		new_inputs[1] = fg->nodes[nd].inputs[1];
		new_inputs[2] = fg->nodes[nd].inputs[2];
		new_inputs[3] = fg->nodes[nd].inputs[3];
		SacFree (fg->nodes[nd].inputs)
		fg->nodes[nd].inputs = new_inputs;
		fg->nodes[nd].num_inputs = 6;
		gin = fg->nodes[id].My_inputs[res_index];
		add_edge (fg, gin, 0, nd, 4);
		gin = fg->nodes[id].My_inputs[sz_index];
		add_edge (fg, gin, 0, nd, 5);
		set_type_to_uint (&(fg->nodes[nd].inputs[4].ty), 32);
		set_type_to_uint (&(fg->nodes[nd].inputs[5].ty), 32);
		res_index++;
	        break;
	    case ND_ACCUM_MIN :
		if (is_unsigned (fg->nodes[nd].inputs[0].ty.type))
		    fg->nodes[nd].nodetype = ND_ACCUM_UMIN_VALUES;
		else
		    fg->nodes[nd].nodetype = ND_ACCUM_IMIN_VALUES;
		new_inputs = make_input_ports (6);
		new_inputs[0] = fg->nodes[nd].inputs[0];
		new_inputs[1] = fg->nodes[nd].inputs[1];
		new_inputs[2] = fg->nodes[nd].inputs[2];
		new_inputs[3] = fg->nodes[nd].inputs[3];
		SacFree (fg->nodes[nd].inputs)
		fg->nodes[nd].inputs = new_inputs;
		fg->nodes[nd].num_inputs = 6;
		gin = fg->nodes[id].My_inputs[res_index];
		add_edge (fg, gin, 0, nd, 4);
		gin = fg->nodes[id].My_inputs[sz_index];
		add_edge (fg, gin, 0, nd, 5);
		set_type_to_uint (&(fg->nodes[nd].inputs[4].ty), 32);
		set_type_to_uint (&(fg->nodes[nd].inputs[5].ty), 32);
		res_index++;
	        break;
	    case ND_ACCUM_AND :
		fg->nodes[nd].nodetype = ND_ACCUM_AND_VALUES;
		new_inputs = make_input_ports (6);
		new_inputs[0] = fg->nodes[nd].inputs[0];
		new_inputs[1] = fg->nodes[nd].inputs[1];
		new_inputs[2] = fg->nodes[nd].inputs[2];
		new_inputs[3] = fg->nodes[nd].inputs[3];
		SacFree (fg->nodes[nd].inputs)
		fg->nodes[nd].inputs = new_inputs;
		fg->nodes[nd].num_inputs = 6;
		gin = fg->nodes[id].My_inputs[res_index];
		add_edge (fg, gin, 0, nd, 4);
		gin = fg->nodes[id].My_inputs[sz_index];
		add_edge (fg, gin, 0, nd, 5);
		set_type_to_uint (&(fg->nodes[nd].inputs[4].ty), 32);
		set_type_to_uint (&(fg->nodes[nd].inputs[5].ty), 32);
		res_index++;
	        break;
	    case ND_ACCUM_OR :
		fg->nodes[nd].nodetype = ND_ACCUM_OR_VALUES;
		new_inputs = make_input_ports (6);
		new_inputs[0] = fg->nodes[nd].inputs[0];
		new_inputs[1] = fg->nodes[nd].inputs[1];
		new_inputs[2] = fg->nodes[nd].inputs[2];
		new_inputs[3] = fg->nodes[nd].inputs[3];
		SacFree (fg->nodes[nd].inputs)
		fg->nodes[nd].inputs = new_inputs;
		fg->nodes[nd].num_inputs = 6;
		gin = fg->nodes[id].My_inputs[res_index];
		add_edge (fg, gin, 0, nd, 4);
		gin = fg->nodes[id].My_inputs[sz_index];
		add_edge (fg, gin, 0, nd, 5);
		set_type_to_uint (&(fg->nodes[nd].inputs[4].ty), 32);
		set_type_to_uint (&(fg->nodes[nd].inputs[5].ty), 32);
		res_index++;
	        break;
	    case ND_ACCUM_MAX :
		if (is_unsigned (fg->nodes[nd].inputs[0].ty.type))
		    fg->nodes[nd].nodetype = ND_ACCUM_UMAX_VALUES;
		else
		    fg->nodes[nd].nodetype = ND_ACCUM_IMAX_VALUES;
		new_inputs = make_input_ports (6);
		new_inputs[0] = fg->nodes[nd].inputs[0];
		new_inputs[1] = fg->nodes[nd].inputs[1];
		new_inputs[2] = fg->nodes[nd].inputs[2];
		new_inputs[3] = fg->nodes[nd].inputs[3];
		SacFree (fg->nodes[nd].inputs)
		fg->nodes[nd].inputs = new_inputs;
		fg->nodes[nd].num_inputs = 6;
		gin = fg->nodes[id].My_inputs[res_index];
		add_edge (fg, gin, 0, nd, 4);
		gin = fg->nodes[id].My_inputs[sz_index];
		add_edge (fg, gin, 0, nd, 5);
		set_type_to_uint (&(fg->nodes[nd].inputs[4].ty), 32);
		set_type_to_uint (&(fg->nodes[nd].inputs[5].ty), 32);
		res_index++;
	        break;
	    case ND_REDUCE_SUM :
		if (is_unsigned (fg->nodes[nd].inputs[0].ty.type))
		    fg->nodes[nd].nodetype = ND_USUM_VALUES;
		else
		    fg->nodes[nd].nodetype = ND_ISUM_VALUES;
		new_inputs = make_input_ports (4);
		new_inputs[0] = fg->nodes[nd].inputs[0];
		new_inputs[1] = fg->nodes[nd].inputs[1];
		SacFree (fg->nodes[nd].inputs)
		fg->nodes[nd].inputs = new_inputs;
		fg->nodes[nd].num_inputs = 4;
		gin = fg->nodes[id].My_inputs[res_index];
		add_edge (fg, gin, 0, nd, 2);
		gin = fg->nodes[id].My_inputs[sz_index];
		add_edge (fg, gin, 0, nd, 3);
		set_type_to_uint (&(fg->nodes[nd].inputs[2].ty), 32);
		set_type_to_uint (&(fg->nodes[nd].inputs[3].ty), 32);
		res_index++;
	        break;
	    case ND_REDUCE_MIN :
		if (is_unsigned (fg->nodes[nd].inputs[0].ty.type))
		    fg->nodes[nd].nodetype = ND_UMIN_VALUES;
		else
		    fg->nodes[nd].nodetype = ND_IMIN_VALUES;
		new_inputs = make_input_ports (4);
		new_inputs[0] = fg->nodes[nd].inputs[0];
		new_inputs[1] = fg->nodes[nd].inputs[1];
		SacFree (fg->nodes[nd].inputs)
		fg->nodes[nd].inputs = new_inputs;
		fg->nodes[nd].num_inputs = 4;
		gin = fg->nodes[id].My_inputs[res_index];
		add_edge (fg, gin, 0, nd, 2);
		gin = fg->nodes[id].My_inputs[sz_index];
		add_edge (fg, gin, 0, nd, 3);
		set_type_to_uint (&(fg->nodes[nd].inputs[2].ty), 32);
		set_type_to_uint (&(fg->nodes[nd].inputs[3].ty), 32);
		res_index++;
	        break;
	    case ND_REDUCE_MAX :
		if (is_unsigned (fg->nodes[nd].inputs[0].ty.type))
		    fg->nodes[nd].nodetype = ND_UMAX_VALUES;
		else
		    fg->nodes[nd].nodetype = ND_IMAX_VALUES;
		new_inputs = make_input_ports (4);
		new_inputs[0] = fg->nodes[nd].inputs[0];
		new_inputs[1] = fg->nodes[nd].inputs[1];
		SacFree (fg->nodes[nd].inputs)
		fg->nodes[nd].inputs = new_inputs;
		fg->nodes[nd].num_inputs = 4;
		gin = fg->nodes[id].My_inputs[res_index];
		add_edge (fg, gin, 0, nd, 2);
		gin = fg->nodes[id].My_inputs[sz_index];
		add_edge (fg, gin, 0, nd, 3);
		set_type_to_uint (&(fg->nodes[nd].inputs[2].ty), 32);
		set_type_to_uint (&(fg->nodes[nd].inputs[3].ty), 32);
		res_index++;
	        break;
	    case ND_REDUCE_AND :
		fg->nodes[nd].nodetype = ND_AND_VALUES;
		new_inputs = make_input_ports (4);
		new_inputs[0] = fg->nodes[nd].inputs[0];
		new_inputs[1] = fg->nodes[nd].inputs[1];
		SacFree (fg->nodes[nd].inputs)
		fg->nodes[nd].inputs = new_inputs;
		fg->nodes[nd].num_inputs = 4;
		gin = fg->nodes[id].My_inputs[res_index];
		add_edge (fg, gin, 0, nd, 2);
		gin = fg->nodes[id].My_inputs[sz_index];
		add_edge (fg, gin, 0, nd, 3);
		set_type_to_uint (&(fg->nodes[nd].inputs[2].ty), 32);
		set_type_to_uint (&(fg->nodes[nd].inputs[3].ty), 32);
		res_index++;
	        break;
	    case ND_REDUCE_OR :
		fg->nodes[nd].nodetype = ND_OR_VALUES;
		new_inputs = make_input_ports (4);
		new_inputs[0] = fg->nodes[nd].inputs[0];
		new_inputs[1] = fg->nodes[nd].inputs[1];
		SacFree (fg->nodes[nd].inputs)
		fg->nodes[nd].inputs = new_inputs;
		fg->nodes[nd].num_inputs = 4;
		gin = fg->nodes[id].My_inputs[res_index];
		add_edge (fg, gin, 0, nd, 2);
		gin = fg->nodes[id].My_inputs[sz_index];
		add_edge (fg, gin, 0, nd, 3);
		set_type_to_uint (&(fg->nodes[nd].inputs[2].ty), 32);
		set_type_to_uint (&(fg->nodes[nd].inputs[3].ty), 32);
		res_index++;
	        break;
	    case ND_WRITE_SCALAR :
		gin = fg->nodes[id].My_inputs[res_index];
		add_edge (fg, gin, 0, nd, 1);
		set_type_to_uint (&(fg->nodes[nd].inputs[1].ty), 32);
		res_index++;
	        break;
	    default :
	        break;
	    }

    return res_index;
    }

void convert_node_for_dfg (FuncGraph *fg, int nd, int loop_id, IntList **list_copy)
    {
    InputPort *new_inputs;
    OutputPort *new_outputs;
    int gen, gin, snd, spt, i;
    int cast_node;

    switch (fg->nodes[nd].nodetype)
	    {
	    case ND_ARR_DEF :
		/* insert a CAST on an input if its type is not same as output type */
		for (i=0; i<fg->nodes[nd].num_inputs; i++)
		    if (!type_equal (&(fg->nodes[nd].inputs[i].ty), &(fg->nodes[nd].outputs[0].ty)))
		        {
			cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[nd].loc);
			if (fg->nodes[nd].inputs[i].is_const)
			    {
			    fg->nodes[cast_node].inputs[0] = fg->nodes[nd].inputs[i];
			    fg->nodes[nd].inputs[i].is_const = FALSE;
			    add_edge (fg, cast_node, 0, nd, i);
			    copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), &(fg->nodes[nd].outputs[0].ty));
			    fg->nodes[cast_node].outputs[0].ty.kind = Scalar;
			    copy_type_info (&(fg->nodes[nd].inputs[i].ty), &(fg->nodes[cast_node].outputs[0].ty));
			    insert_val_before_in_intlist (cast_node, nd, list_copy);
			    }
			else
			    {
			    snd = fg->nodes[nd].inputs[i].back_edges->node;
			    spt = fg->nodes[nd].inputs[i].back_edges->port;
			    remove_edge (fg, snd, spt, nd, i);
			    add_edge (fg, snd, spt, cast_node, 0);
			    add_edge (fg, cast_node, 0, nd, i);
			    copy_type_info (&(fg->nodes[cast_node].inputs[0].ty), &(fg->nodes[snd].outputs[spt].ty));
			    copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), &(fg->nodes[nd].outputs[0].ty));
			    fg->nodes[cast_node].outputs[0].ty.kind = Scalar;
			    copy_type_info (&(fg->nodes[nd].inputs[i].ty), &(fg->nodes[cast_node].outputs[0].ty));
			    insert_val_after_in_intlist (cast_node, snd, list_copy);
			    }
			}
	        break;
	    case ND_WINDOW_GEN :
		window_convert (fg, nd, list_copy);
		break;
	    case ND_ELE_GEN :
	      elegen_convert (fg, nd, list_copy);
	      break;
	    case ND_SLICE_GEN :
	      slicegen_convert (fg, nd, list_copy);
	      break;
	    case ND_ARRAYREF :
	        convert_arrayref (fg, nd, list_copy);
	        break;
	    case ND_MUL :
		convert_mult_to_mach_mult (fg, nd, list_copy);
		break;
	    case ND_LEFT_SHIFT :
		convert_lshift_to_mach_lshift (fg, nd, list_copy);
		break;
	    case ND_INTRINCALL :
	        /* for now, it must be a sqrt */
		convert_sqrt (fg, nd, list_copy);
		break;
	    case ND_NEXT :
	        convert_next (fg, nd, loop_id);
	        break;
	    default :
	        break;
	    }
    }

void convert_next (FuncGraph *fg, int nd, int loop_id)
    {
    InputPort *new_inputs;
    int inode, sz_index;

    fg->nodes[nd].nodetype = ND_CIRCULATE;

    /* create new input ports */
    new_inputs = make_input_ports (3);
    new_inputs[0] = fg->nodes[nd].inputs[0];
    free (fg->nodes[nd].inputs);
    fg->nodes[nd].inputs = new_inputs;
    fg->nodes[nd].num_inputs = 3;

    inode = fg->nodes[nd].In_next_id;

    fg->nodes[nd].outputs[1] = fg->nodes[inode].outputs[0];
    fg->nodes[inode].outputs[0].targets = NULL;
    retarget_backedges (fg, fg->nodes[nd].outputs[1].targets, inode, 0, nd, 1);

    add_edge (fg, inode, 0, nd, 1);
    copy_type_info (&(fg->nodes[nd].inputs[1].ty), &(fg->nodes[inode].outputs[0].ty));

    sz_index = fg->nodes[loop_id].num_inputs - 1;
    inode = fg->nodes[loop_id].My_inputs[sz_index];
    add_edge (fg, inode, 0, nd, 2);
    copy_type_info (&(fg->nodes[nd].inputs[2].ty), &(fg->nodes[inode].outputs[0].ty));
    }

void convert_to_dfg (FuncGraph *fg, int loop_id, int res_index, int loop_rank)
    {
    IntList *list_copy, *it;
    InputPort *new_inputs;
    int gen, nd, gin, sz_index, snd, spt, i;
    int cast_node;

    sz_index = fg->nodes[loop_id].num_inputs - 1;

    fg->nodes[loop_id].nodetype = ND_RC_FORALL;

    /* lift the generator graph */
    gen = find_generator_graph (fg, loop_id);
    copy_intlist (&list_copy, &(fg->nodes[loop_id].My_nodes));
    lift_graph (fg, gen, &list_copy);
    free_intlist (&(fg->nodes[loop_id].My_nodes));
    fg->nodes[loop_id].My_nodes = list_copy;

    /* lift any switches */
    copy_intlist (&list_copy, &(fg->nodes[loop_id].My_nodes));
    for (it=fg->nodes[loop_id].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;
	if (fg->nodes[nd].nodetype == ND_SWITCH)
	    convert_switch (fg, nd, loop_id, &list_copy);
	}
    free_intlist (&(fg->nodes[loop_id].My_nodes));
    fg->nodes[loop_id].My_nodes = list_copy;

    copy_intlist (&list_copy, &(fg->nodes[loop_id].My_nodes));
    /* convert the interior nodes */
    for (it=fg->nodes[loop_id].My_nodes; it!=NULL; it=it->link)
	{
	nd = it->val;
	switch (fg->nodes[nd].nodetype)
	    {
	    case ND_CONSTRUCT_CONCAT :
	    case ND_CONSTRUCT_CONCAT_MASKED_MANY :
	    case ND_CONSTRUCT_TILE :
	    case ND_CONSTRUCT_ARRAY :
	    case ND_REDUCE_HIST :
	    case ND_ACCUM_HIST :
	    case ND_ACCUM_SUM :
	    case ND_ACCUM_MIN :
	    case ND_ACCUM_AND :
	    case ND_ACCUM_OR :
	    case ND_ACCUM_MAX :
	    case ND_REDUCE_SUM :
	    case ND_REDUCE_MIN :
	    case ND_REDUCE_MAX :
	    case ND_REDUCE_AND :
	    case ND_REDUCE_OR :
	    case ND_WRITE_SCALAR :
		res_index = convert_loop_return_node_for_dfg (fg, nd, loop_id,
				sz_index, res_index, loop_rank, &list_copy);
		break;
	    case ND_ARR_DEF :
	    case ND_WINDOW_GEN :
	    case ND_ELE_GEN :
	    case ND_SLICE_GEN :
	    case ND_ARRAYREF :
	    case ND_MUL :
	    case ND_LEFT_SHIFT :
	    case ND_INTRINCALL :
	    case ND_NEXT :
		convert_node_for_dfg (fg, nd, loop_id, &list_copy);
	        break;
	    }
	}

    free_intlist (&(fg->nodes[loop_id].My_nodes));
    fg->nodes[loop_id].My_nodes = list_copy;

    convert_loop_to_dfg (fg, loop_id, &(fg->nodes[loop_id].My_nodes));
    }

void convert_arrayref (FuncGraph *fg, int nd, IntList **list_copy)
    {
    int rank, i, gin, io_num, graph;
    InputPort *new_inputs;

    rank = fg->nodes[nd].inputs[0].ty.dims;
    new_inputs = make_input_ports (1 + 2*rank);
    new_inputs[0] = fg->nodes[nd].inputs[0];
    for (i=0; i<rank; i++)
        new_inputs[i+1] = fg->nodes[nd].inputs[i+1];
    SacFree (fg->nodes[nd].inputs)
    fg->nodes[nd].inputs = new_inputs;
    fg->nodes[nd].num_inputs = 1 + 2*rank;

    assert (fg->nodes[nd].inputs[0].back_edges != NULL);
    gin = fg->nodes[nd].inputs[0].back_edges->node;
    assert (fg->nodes[gin].nodetype == ND_G_INPUT);
    graph = fg->nodes[gin].My_graph;
    io_num = 1 + fg->nodes[gin].Io_num;

    for (i=0; i<rank; i++, io_num++)
	{
	gin = fg->nodes[graph].My_inputs[io_num];
	assert (fg->nodes[gin].nodetype == ND_G_INPUT);
	add_edge (fg, gin, 0, nd, 1+rank+i);
	copy_type_info (&(fg->nodes[nd].inputs[1+rank+i].ty), &(fg->nodes[gin].outputs[0].ty));
	}

    switch (rank)
        {
	case 1 :
	    fg->nodes[nd].nodetype = ND_ARRAYREF_1D;
	    break;
	case 2 :
	    fg->nodes[nd].nodetype = ND_ARRAYREF_2D;
	    break;
	case 3 :
	    fg->nodes[nd].nodetype = ND_ARRAYREF_3D;
	    break;
	case 4 :
	    fg->nodes[nd].nodetype = ND_ARRAYREF_4D;
	    break;
	case 5 :
	    fg->nodes[nd].nodetype = ND_ARRAYREF_5D;
	    break;
	case 6 :
	    fg->nodes[nd].nodetype = ND_ARRAYREF_6D;
	    break;
	case 7 :
	    fg->nodes[nd].nodetype = ND_ARRAYREF_7D;
	    break;
	case 8 :
	    fg->nodes[nd].nodetype = ND_ARRAYREF_8D;
	    break;
	default :
	    assert (0);
	}
    }

void coalesce_tile (FuncGraph *fg, int tile_nd, int arrdef_nd,
			int loop_nd, int res_index, int loop_rank, IntList **list_copy)
    {
    InputPort *new_inputs;
    int component_rank, n_inputs, n_values, i, j, k, gin;
    char str[128];

    component_rank = fg->nodes[arrdef_nd].DefRank;
    n_values = fg->nodes[arrdef_nd].num_inputs;
    fg->nodes[arrdef_nd].nodetype = write_tile_type (fg, arrdef_nd, loop_rank, component_rank);

    /* we need additional inputs: one for the address, and one for each loop extent and component extent */
    n_inputs = n_values + 1 + loop_rank + component_rank;
    new_inputs = make_input_ports (n_inputs);
    for (i=0; i<n_values; i++)
        new_inputs[i] = fg->nodes[arrdef_nd].inputs[i];
    SacFree (fg->nodes[arrdef_nd].inputs)
    fg->nodes[arrdef_nd].inputs = new_inputs;
    fg->nodes[arrdef_nd].num_inputs = n_inputs;

    k = n_values;

    /* connect the address input */
    gin = fg->nodes[loop_nd].My_inputs[res_index];
    add_edge (fg, gin, 0, arrdef_nd, k);
    set_type_to_uint (&(fg->nodes[arrdef_nd].inputs[k++].ty), 32);

    /* connect the loop extent inputs */
    for (i=0, j=fg->nodes[loop_nd].num_inputs-1-loop_rank; i<loop_rank; i++, j++)
        {
        gin = fg->nodes[loop_nd].My_inputs[j];
        add_edge (fg, gin, 0, arrdef_nd, k);
        set_type_to_uint (&(fg->nodes[arrdef_nd].inputs[k++].ty), 32);
	}

    /* connect the component extent inputs */
    for (i=0; i<component_rank; i++)
        {
	fg->nodes[arrdef_nd].inputs[k].is_const = TRUE;
	sprintf (str, "%d", fg->nodes[arrdef_nd].DefDims[i]);
	fg->nodes[arrdef_nd].inputs[k].constval = copy_to_const (str);
        set_type_to_uint (&(fg->nodes[arrdef_nd].inputs[k++].ty), 32);
	}

    SacFree (fg->nodes[arrdef_nd].DefDims)

    /* delete the tile node */
    assert (fg->nodes[arrdef_nd].outputs[0].targets != NULL);
    assert (fg->nodes[arrdef_nd].outputs[0].targets->link == NULL);
    remove_edge (fg, arrdef_nd, 0, tile_nd, 0);
    fg->nodes[arrdef_nd].outputs[0] = fg->nodes[tile_nd].outputs[0];
    fg->nodes[tile_nd].outputs[0].targets = NULL;
    retarget_backedges (fg, fg->nodes[arrdef_nd].outputs[0].targets, tile_nd, 0, arrdef_nd, 0);
    free_ddcf_node (fg, tile_nd, list_copy);
    }

DdcfType write_tile_type (FuncGraph *fg, int nd, int loop_rank, int component_rank)
    {
    switch (loop_rank) {
	    case 1 :
	        switch (component_rank) {
		    case 1 :
		        return ND_WRITE_TILE_1D_1D;
		    case 2 :
		        return ND_WRITE_TILE_1D_2D;
		    case 3 :
		        return ND_WRITE_TILE_1D_3D;
		    default :
		        assert (0);
		    }
		break;
	    case 2 :
	        switch (component_rank) {
		    case 1 :
		        return ND_WRITE_TILE_2D_1D;
		    case 2 :
		        return ND_WRITE_TILE_2D_2D;
		    case 3 :
		        return ND_WRITE_TILE_2D_3D;
		    default :
		        assert (0);
		    }
		break;
	    case 3 :
	        switch (component_rank) {
		    case 1 :
		        return ND_WRITE_TILE_3D_1D;
		    case 2 :
		        return ND_WRITE_TILE_3D_2D;
		    case 3 :
		        return ND_WRITE_TILE_3D_3D;
		    default :
		        assert (0);
		    }
		break;
	    default :
	        assert (0);
	    }
    }

void convert_loop_to_dfg (FuncGraph *fg, int id, IntList **itlist)
    {
    IntList *list_copy, *it;
    int nd, gout, i;

    copy_intlist (&list_copy, &(fg->nodes[id].My_nodes));

    /* convert the interior nodes */
    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
	{
	nd = it->val;

	switch (fg->nodes[nd].nodetype)
	    {
	    case ND_VHDL_CALL :
		for (i=0; i<fg->nodes[nd].num_inputs; i++)
		    convert_input_for_dfg (fg, nd, i);
		for (i=0; i<fg->nodes[nd].num_outputs; i++)
		    fg->nodes[nd].outputs[i].ty.type = Bits;
	        break;
	    case ND_CAST :
		convert_cast (fg, nd, &list_copy);
	        break;
	    case ND_G_INPUT_NEXT :
	        fg->nodes[nd].nodetype = ND_G_INPUT;
		fg->nodes[nd].outputs[0].ty.type = Bits;
		break;
	    case ND_G_INPUT :
	        if (is_complex_num_type (fg->nodes[nd].outputs[0].ty.type))
		    assert (0);

	        fg->nodes[nd].outputs[0].ty.type = Bits;
		break;
	    case ND_ARRAYREF_1D :
	    case ND_ARRAYREF_2D :
	    case ND_ARRAYREF_3D :
	    case ND_ARRAYREF_4D :
	    case ND_ARRAYREF_5D :
	    case ND_ARRAYREF_6D :
	    case ND_ARRAYREF_7D :
	    case ND_ARRAYREF_8D :
		fg->nodes[nd].inputs[0].ty.totsize = 32;
		for (i=0; i<fg->nodes[nd].num_inputs; i++)
		    convert_input_for_dfg (fg, nd, i);

		fg->nodes[nd].outputs[0].ty.type = Bits;
	        break;
	    case ND_REDUCE_VAL_AT_FIRST_MAX_MACRO :
	    case ND_REDUCE_VAL_AT_FIRST_MIN_MACRO :
	    case ND_REDUCE_VAL_AT_LAST_MAX_MACRO :
	    case ND_REDUCE_VAL_AT_LAST_MIN_MACRO :
	    case ND_REDUCE_MEDIAN_MACRO :
	    case ND_REDUCE_SUM_MACRO :
	    case ND_REDUCE_MAX_MACRO :
	    case ND_REDUCE_MIN_MACRO :
	    case ND_REDUCE_AND_MACRO :
	    case ND_REDUCE_OR_MACRO :
	    case ND_REDUCE_HIST_MACRO :
	        convert_macros (fg, nd);
		break;
	    case ND_HIST_VALUES :
		convert_input_for_dfg (fg, nd, 0); 
		convert_input_for_dfg (fg, nd, 1); 
		convert_input_for_dfg (fg, nd, 2); 
		convert_input_for_dfg (fg, nd, 3); 
		convert_input_for_dfg (fg, nd, 4); 
		fg->nodes[nd].outputs[0].ty.type = Bits;
	        break;
	    case ND_ACCUM_USUM_VALUES :
	    case ND_ACCUM_ISUM_VALUES :
	    case ND_ACCUM_UMIN_VALUES :
	    case ND_ACCUM_IMIN_VALUES :
	    case ND_ACCUM_UMAX_VALUES :
	    case ND_ACCUM_IMAX_VALUES :
	    case ND_ACCUM_AND_VALUES :
	    case ND_ACCUM_OR_VALUES :
		convert_input_for_dfg (fg, nd, 0); 
		convert_input_for_dfg (fg, nd, 1); 
		convert_input_for_dfg (fg, nd, 2); 
		convert_input_for_dfg (fg, nd, 3); 
		convert_input_for_dfg (fg, nd, 4); 
		convert_input_for_dfg (fg, nd, 5); 
		fg->nodes[nd].outputs[0].ty.type = Bits;
	        break;
	    case ND_ACCUM_HIST_VALUES :
		convert_input_for_dfg (fg, nd, 0); 
		convert_input_for_dfg (fg, nd, 1); 
		convert_input_for_dfg (fg, nd, 2); 
		convert_input_for_dfg (fg, nd, 3); 
		convert_input_for_dfg (fg, nd, 4); 
		convert_input_for_dfg (fg, nd, 5); 
		convert_input_for_dfg (fg, nd, 6); 
		fg->nodes[nd].outputs[0].ty.type = Bits;
	        break;
	    case ND_USUM_VALUES :
	    case ND_ISUM_VALUES :
	    case ND_UMIN_VALUES :
	    case ND_IMIN_VALUES :
	    case ND_UMAX_VALUES :
	    case ND_IMAX_VALUES :
	    case ND_AND_VALUES :
	    case ND_OR_VALUES :
		convert_input_for_dfg (fg, nd, 0); 
		convert_input_for_dfg (fg, nd, 1); 
		convert_input_for_dfg (fg, nd, 2); 
		convert_input_for_dfg (fg, nd, 3); 
		fg->nodes[nd].outputs[0].ty.type = Bits;
	        break;
	    case ND_ADD :
	    case ND_SUB :
		convert_arith (fg, nd, &list_copy);
		break;
	    case ND_MUL_MACH :
	        convert_mul (fg, nd, &list_copy);
		break;
	    case ND_SQRT_MACH :
	        convert_input_for_dfg (fg, nd, 0);
		fg->nodes[nd].outputs[0].ty.type = Bits;
		break;
	    case ND_LE :
	    case ND_LT :
	    case ND_GE :
	    case ND_GT :
	    case ND_EQ :
	    case ND_NEQ:
	        convert_compares (fg, nd, &list_copy);
		break;
	    case ND_NEG :
	        if ((is_unsigned (fg->nodes[nd].inputs[0].ty.type)) && (fg->nodes[nd].inputs[0].ty.totsize < 32))
	            change_width (fg, nd, 0, 1+fg->nodes[nd].inputs[0].ty.totsize, &list_copy);
		convert_input_for_dfg (fg, nd, 0);
		fg->nodes[nd].outputs[0].ty.type = Bits;
	        break;
	    case ND_NOT :
		fg->nodes[nd].nodetype = ND_BIT_COMPL;
		convert_input_for_dfg (fg, nd, 0);
		fg->nodes[nd].outputs[0].ty.type = Bits;
	        break;
	    case ND_AND :
	    case ND_BIT_AND :
		fg->nodes[nd].nodetype = ND_BIT_AND;
		convert_input_for_dfg (fg, nd, 0);
		convert_input_for_dfg (fg, nd, 1);
		fg->nodes[nd].outputs[0].ty.type = Bits;
	        break;
	    case ND_OR :
	    case ND_BIT_OR :
		fg->nodes[nd].nodetype = ND_BIT_OR;
		convert_input_for_dfg (fg, nd, 0);
		convert_input_for_dfg (fg, nd, 1);
		fg->nodes[nd].outputs[0].ty.type = Bits;
	        break;
	    case ND_BIT_EOR :
		fg->nodes[nd].nodetype = ND_BIT_EOR;
		convert_input_for_dfg (fg, nd, 0);
		convert_input_for_dfg (fg, nd, 1);
		fg->nodes[nd].outputs[0].ty.type = Bits;
	        break;
	    case ND_LEFT_SHIFT_MACH :
	    case ND_LEFT_SHIFT :
	    case ND_RIGHT_SHIFT :
		convert_input_for_dfg (fg, nd, 0);
		convert_input_for_dfg (fg, nd, 1);
		fg->nodes[nd].outputs[0].ty.type = Bits;
	        break;
	    case ND_RC_WINDOW_GEN_1D :
	    case ND_RC_WINDOW_GEN_2D :
	    case ND_RC_SLICE_GEN_2D_COL :
	    case ND_RC_SLICE_GEN_2D_ROW :
	        if (is_complex_num_type (fg->nodes[nd].inputs[0].ty.type))
		    assert (0);

		for (i=0; i<fg->nodes[nd].num_inputs; i++)
		    convert_input_for_dfg (fg, nd, i);

		for (i=0; i<fg->nodes[nd].num_outputs; i++)
		    fg->nodes[nd].outputs[i].ty.type = Bits;

		break;
	    case ND_RC_ELE_GEN_1D :
	    case ND_RC_ELE_GEN_2D :
	    case ND_RC_ELE_GEN_3D :
	        if (is_complex_num_type (fg->nodes[nd].inputs[0].ty.type))
		    assert (0);

		for (i=0; i<fg->nodes[nd].num_inputs; i++)
		    convert_input_for_dfg (fg, nd, i);

		fg->nodes[nd].outputs[0].ty.type = Bits;

		break;
	    case ND_SCALAR_GEN :
	    case ND_CIRCULATE :
		for (i=0; i<fg->nodes[nd].num_inputs; i++)
		    convert_input_for_dfg (fg, nd, i);

		for (i=0; i<fg->nodes[nd].num_outputs; i++)
		    fg->nodes[nd].outputs[i].ty.type = Bits;

	        break;
	    case ND_WRITE_TILE_1D_1D :
	    case ND_WRITE_TILE_1D_2D :
	    case ND_WRITE_TILE_1D_3D :
	    case ND_WRITE_TILE_2D_1D :
	    case ND_WRITE_TILE_2D_2D :
	    case ND_WRITE_TILE_2D_3D :
	    case ND_WRITE_TILE_3D_1D :
	    case ND_WRITE_TILE_3D_2D :
	    case ND_WRITE_TILE_3D_3D :
	    case ND_WRITE_SCALAR :
	    case ND_WRITE_VALS_TO_BUFFER :
	        if (is_complex_num_type (fg->nodes[nd].inputs[0].ty.type))
		    assert (0);

		for (i=0; i<fg->nodes[nd].num_inputs; i++)
		    convert_input_for_dfg (fg, nd, i);

		fg->nodes[nd].outputs[0].ty.type = Bits;

	        break;
	    case ND_RC_SELECTOR :
	        if (is_complex_num_type (fg->nodes[nd].inputs[1].ty.type))
		    assert (0);

		/* set all the selector values to the same width */
		for (i=1; i<fg->nodes[nd].num_inputs-1; i+=2)
		    fg->nodes[nd].inputs[i].ty.totsize = fg->nodes[nd].inputs[0].ty.totsize;

		for (i=0; i<fg->nodes[nd].num_inputs; i++)
		    convert_input_for_dfg (fg, nd, i);

		fg->nodes[nd].outputs[0].ty.type = Bits;

		break;
	    case ND_ACCUM_SUM :
	    case ND_ACCUM_MIN :
	    case ND_ACCUM_MAX :
	    case ND_ACCUM_AND :
	    case ND_ACCUM_OR :
	    case ND_ACCUM_HIST :
	    case ND_REDUCE_HIST :
	    case ND_REDUCE_SUM :
	    case ND_REDUCE_MIN :
	    case ND_REDUCE_MAX :
	    case ND_REDUCE_AND :
	    case ND_REDUCE_OR :
	        break;
	    default :
	        assert (0);
	    }
	}

    for (i=0; i<fg->nodes[id].num_outputs; i++)
        {
	gout = fg->nodes[id].My_outputs[i];
	convert_input_for_dfg (fg, gout, 0);
	}

    free_intlist (&(fg->nodes[id].My_nodes));
    fg->nodes[id].My_nodes = list_copy;

    if (bit_width_opt)
        bitwidth_narrow (fg, id);
    }

/*  'id' is a ND_SWITCH graph */
void convert_switch (FuncGraph *fg, int id, int loop_id, IntList **parent_list)
    {
    IntList *it, *itt, *list_copy, *loc_copy;
    Edge *ee;
    int i, j, k, nd, ndd, *new_selectors, num_outputs, sw_key, sel_nd, key_src_nd, key_src_pt;
    int key_cnt, gout, default_found;

    copy_intlist (&list_copy, &(fg->nodes[id].My_nodes));

    /* recursively convert any lower level switches */
    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;

	if (fg->nodes[nd].nodetype == ND_CASE)
	    {
	    copy_intlist (&loc_copy, &(fg->nodes[nd].My_nodes));

	    for (itt=fg->nodes[nd].My_nodes; itt!=NULL; itt=itt->link)
		{
		ndd = itt->val;

		if (fg->nodes[ndd].nodetype == ND_SWITCH)
		    convert_switch (fg, ndd, loop_id, &loc_copy);
		}
	    
	    free_intlist (&(fg->nodes[nd].My_nodes));
	    fg->nodes[nd].My_nodes = loc_copy;
	    }
	else if (fg->nodes[nd].nodetype == ND_SWITCH)
	    convert_switch (fg, nd, loop_id, &list_copy);
	}

    /* count the number of compare keys */
    for (it=fg->nodes[id].My_nodes, key_cnt=0; it!=NULL; it=it->link)
        {
	nd = it->val;

	if (fg->nodes[nd].nodetype == ND_CASE)
	    {
	    sel_nd = find_selector_node (fg, nd);
	    if (sel_nd >= 0)
	        key_cnt += fg->nodes[sel_nd].num_inputs;
	    }
	}

    sw_key = find_switch_key (fg, id);
    assert (fg->nodes[sw_key].inputs[0].is_const == FALSE);
    key_src_nd = fg->nodes[sw_key].inputs[0].back_edges->node;
    key_src_pt = fg->nodes[sw_key].inputs[0].back_edges->port;

    /* eliminate ND_SWITCH_KEY node */
    remove_edge (fg, key_src_nd, key_src_pt, sw_key, 0);
    free_ddcf_node (fg, sw_key, &list_copy);

    num_outputs = fg->nodes[id].num_outputs;
    SacMalloc (new_selectors, int*, num_outputs * sizeof (int))

    for (i=0; i<num_outputs; i++)
        {
	new_selectors[i] = new_ddcf_node (fg, ND_RC_SELECTOR, 2*key_cnt+2, 1, fg->nodes[id].loc);
	gout = fg->nodes[id].My_outputs[i];
	add_edge (fg, new_selectors[i], 0, gout, 0);
	copy_type_info (&(fg->nodes[new_selectors[i]].outputs[0].ty), &(fg->nodes[gout].inputs[0].ty));
	append_intlist (&list_copy, new_selectors[i]);
	add_edge (fg, key_src_nd, key_src_pt, new_selectors[i], 0);
	copy_type_info (&(fg->nodes[new_selectors[i]].inputs[0].ty), &(fg->nodes[key_src_nd].outputs[key_src_pt].ty));
	}

    default_found = FALSE;

    /* rewire the ND_CASE outputs to the new selectors */
    for (it=fg->nodes[id].My_nodes, k=1; it!=NULL; it=it->link)
        {
	nd = it->val;

	if (fg->nodes[nd].nodetype == ND_CASE)
	    {
	    sel_nd = find_selector_node (fg, nd);
	    if (sel_nd >= 0)
	        {
		for (j=0; j<fg->nodes[sel_nd].num_inputs; j++)
		    {
		    for (i=0; i<num_outputs; i++)
			{
		        fg->nodes[new_selectors[i]].inputs[k] = fg->nodes[sel_nd].inputs[j];
			add_edge (fg, nd, i, new_selectors[i], k+1);
			copy_type_info (&(fg->nodes[new_selectors[i]].inputs[k+1].ty), &(fg->nodes[nd].outputs[i].ty));
			}
		    k += 2;
		    }
		}
	    else
		{
	        for (i=0; i<num_outputs; i++)
		    {
		    add_edge (fg, nd, i, new_selectors[i], k);
		    copy_type_info (&(fg->nodes[new_selectors[i]].inputs[k].ty), &(fg->nodes[nd].outputs[i].ty));
		    }

		k++;
		default_found = TRUE;
		}
	    }
	}

    if (!default_found)
	for (i=0; i<num_outputs; i++)
	    {
	    fg->nodes[new_selectors[i]].inputs[k].is_const = TRUE;
	    fg->nodes[new_selectors[i]].inputs[k].constval = copy_to_const ("0x0");
	    copy_type_info (&(fg->nodes[new_selectors[i]].inputs[k].ty), &(fg->nodes[new_selectors[i]].inputs[k-1].ty));
	    }

    /* lift the ND_CASE nodes */
    for (it=fg->nodes[id].My_nodes, k=1; it!=NULL; it=it->link)
        {
	nd = it->val;

	if (fg->nodes[nd].nodetype == ND_CASE)
	    {
	    sel_nd = find_selector_node (fg, nd);
	    if (sel_nd >= 0)
	        free_ddcf_node (fg, sel_nd, &(fg->nodes[nd].My_nodes));

	    for (i=0; i<fg->nodes[nd].num_outputs; i++)
	        for (ee=fg->nodes[nd].outputs[i].targets; ee!=NULL; ee=ee->link)
		    if (fg->nodes[ee->node].nodetype == ND_G_OUTPUT)
			{
		        remove_edge (fg, nd, i, ee->node, ee->port);
			break;
			}

            lift_graph (fg, nd, &list_copy);
	    }
	}

    free_intlist (&(fg->nodes[id].My_nodes));
    fg->nodes[id].My_nodes = list_copy;

    SacFree (new_selectors)

    lift_graph (fg, id, parent_list);
    }

void convert_cast (FuncGraph *fg, int nd, IntList **list_copy)
    {
    assert (fg->nodes[nd].inputs[0].ty.kind == Scalar);

    switch (fg->nodes[nd].inputs[0].ty.type)
        {
	case Uint :
	    convert_cast_from_uint (fg, nd, list_copy);
	    break;
	case Int :
	    convert_cast_from_int (fg, nd, list_copy);
	    break;
	case Ufix :
	    convert_cast_from_ufix (fg, nd, list_copy);
	    break;
	case Fix :
	    convert_cast_from_fix (fg, nd, list_copy);
	    break;
	case Bits :
	    convert_cast_from_bits (fg, nd, list_copy);
	    break;
	case Bool :
	    convert_cast_from_bool (fg, nd, list_copy);
	    break;
	default :
	    handle_error (ErrDdcf2dfgNoCastYet, Deadly, 0, NULL, NULL, type_str (fg->nodes[nd].inputs[0].ty.type));
	}
    }

void convert_cast_from_uint (FuncGraph *fg, int nd, IntList **list_copy)
    {
    int old_width, new_width, shift_width;

    old_width = fg->nodes[nd].inputs[0].ty.totsize;
    new_width = fg->nodes[nd].outputs[0].ty.totsize;

    switch (fg->nodes[nd].outputs[0].ty.type)
        {
	case Uint :
	case Int :
	case Bits :
	    if (old_width == new_width)
		{
	        delete_this_node (fg, nd, list_copy);
		}
	    else
		{
		cnvrt_to_bits_node (fg, nd, new_width, ND_CHANGE_WIDTH, list_copy);
		}
	    break;
	case Ufix :
	case Fix :
	    shift_width = fg->nodes[nd].outputs[0].ty.fracsize;
	    cnvrt_to_bits_node (fg, nd, shift_width, ND_LEFT_SHIFT_MACH, list_copy);
	    break;
	case Bool :
	    if (old_width == new_width)
	        delete_this_node (fg, nd, list_copy);
	    else
		cast_to_bool (fg, nd);
	    break;
	default :
	    assert (0);
	}
    }

void convert_cast_from_int (FuncGraph *fg, int nd, IntList **list_copy)
    {
    int old_width, new_width, shift_width;

    old_width = fg->nodes[nd].inputs[0].ty.totsize;
    new_width = fg->nodes[nd].outputs[0].ty.totsize;

    switch (fg->nodes[nd].outputs[0].ty.type)
        {
	case Uint :
	case Bits :
	    if (old_width == new_width)
	        delete_this_node (fg, nd, list_copy);
	    else
		cnvrt_to_bits_node (fg, nd, new_width, ND_CHANGE_WIDTH, list_copy);
	    break;
	case Int :
	    if (old_width == new_width)
	        delete_this_node (fg, nd, list_copy);
	    else
		cnvrt_to_bits_node (fg, nd, new_width, ND_CHANGE_WIDTH_SE, list_copy);
	    break;
	case Ufix :
	    shift_width = fg->nodes[nd].outputs[0].ty.fracsize;
	    cnvrt_to_bits_node (fg, nd, shift_width, ND_LEFT_SHIFT_MACH, list_copy);
	    break;
	case Fix :
	    shift_width = fg->nodes[nd].outputs[0].ty.fracsize;
	    cnvrt_to_bits_node (fg, nd, new_width, ND_CHANGE_WIDTH_SE, list_copy);
	    insert_bits_node (fg, nd, shift_width, ND_LEFT_SHIFT_MACH, list_copy);
	    break;
	case Bool :
	    cast_to_bool (fg, nd);
	    break;
	default :
	    assert (0);
	}
    }

void convert_cast_from_bits (FuncGraph *fg, int nd, IntList **list_copy)
    {
    int old_width, new_width;

    old_width = fg->nodes[nd].inputs[0].ty.totsize;
    new_width = fg->nodes[nd].outputs[0].ty.totsize;

    switch (fg->nodes[nd].outputs[0].ty.type)
        {
	case Uint :
	case Ufix :
	case Bits :
	    if (old_width == new_width)
	        delete_this_node (fg, nd, list_copy);
	    else
		cnvrt_to_bits_node (fg, nd, new_width, ND_CHANGE_WIDTH, list_copy);
	    break;
	case Int :
	case Fix :
	    if (old_width == new_width)
	        delete_this_node (fg, nd, list_copy);
	    else
		cnvrt_to_bits_node (fg, nd, new_width, ND_CHANGE_WIDTH_SE, list_copy);
	    break;
	case Bool :
	    if (old_width == new_width)
	        delete_this_node (fg, nd, list_copy);
	    else
	        cast_to_bool (fg, nd);
	    break;
	default :
	    assert (0);
	}
    }

void convert_cast_from_ufix (FuncGraph *fg, int nd, IntList **list_copy)
    {
    int old_width, new_width, shift_width, old_frac, new_frac;

    old_width = fg->nodes[nd].inputs[0].ty.totsize;
    new_width = fg->nodes[nd].outputs[0].ty.totsize;

    switch (fg->nodes[nd].outputs[0].ty.type)
        {
	case Uint :
	case Int :
	    shift_width = fg->nodes[nd].inputs[0].ty.fracsize;
	    cnvrt_to_bits_node (fg, nd, shift_width, ND_RIGHT_SHIFT, list_copy);
	    break;
	case Ufix :
	case Fix :
	    old_frac = fg->nodes[nd].inputs[0].ty.fracsize;
	    new_frac = fg->nodes[nd].outputs[0].ty.fracsize;

	    if (old_frac == new_frac)
		{
		if (old_width == new_width)
	            delete_this_node (fg, nd, list_copy);
		else
		    cnvrt_to_bits_node (fg, nd, new_width, ND_CHANGE_WIDTH, list_copy);
		}
	    else if (old_frac < new_frac)
	        {
		shift_width = new_frac - old_frac;
		cnvrt_to_bits_node (fg, nd, shift_width, ND_LEFT_SHIFT_MACH, list_copy);
		}
	    else
	        {
		shift_width = old_frac - new_frac;
		cnvrt_to_bits_node (fg, nd, shift_width, ND_RIGHT_SHIFT, list_copy);
		}
	    break;
	case Bits :
	    if (old_width == new_width)
	        delete_this_node (fg, nd, list_copy);
	    else
		cnvrt_to_bits_node (fg, nd, new_width, ND_CHANGE_WIDTH, list_copy);
	    break;
	case Bool :
	    if (old_width == new_width)
	        delete_this_node (fg, nd, list_copy);
	    else
	        cast_to_bool (fg, nd);
	    break;
	default :
	    assert (0);
	}
    }

void convert_cast_from_fix (FuncGraph *fg, int nd, IntList **list_copy)
    {
    int old_width, new_width, shift_width, old_frac, new_frac;

    old_width = fg->nodes[nd].inputs[0].ty.totsize;
    new_width = fg->nodes[nd].outputs[0].ty.totsize;

    switch (fg->nodes[nd].outputs[0].ty.type)
        {
	case Uint :
	    shift_width = fg->nodes[nd].inputs[0].ty.fracsize;
	    cnvrt_to_bits_node (fg, nd, shift_width, ND_RIGHT_SHIFT, list_copy);
	    break;
	case Int :
	    shift_width = fg->nodes[nd].inputs[0].ty.fracsize;
	    if (fg->nodes[nd].inputs[0].ty.totsize - fg->nodes[nd].inputs[0].ty.fracsize != new_width)
		{
	        cnvrt_to_bits_node (fg, nd, new_width, ND_CHANGE_WIDTH_SE, list_copy);
	        insert_bits_node (fg, nd, shift_width, ND_RIGHT_SHIFT, list_copy);
		}
	    else
	        cnvrt_to_bits_node (fg, nd, shift_width, ND_RIGHT_SHIFT, list_copy);
	    break;
	case Ufix :
	    old_frac = fg->nodes[nd].inputs[0].ty.fracsize;
	    new_frac = fg->nodes[nd].outputs[0].ty.fracsize;

	    if (old_frac == new_frac)
		{
		if (old_width == new_width)
	            delete_this_node (fg, nd, list_copy);
		else
		    cnvrt_to_bits_node (fg, nd, new_width, ND_CHANGE_WIDTH, list_copy);
		}
	    else if (old_frac < new_frac)
	        {
		shift_width = new_frac - old_frac;
		cnvrt_to_bits_node (fg, nd, shift_width, ND_LEFT_SHIFT_MACH, list_copy);
		}
	    else
	        {
		shift_width = old_frac - new_frac;
		cnvrt_to_bits_node (fg, nd, shift_width, ND_RIGHT_SHIFT, list_copy);
		}
	    break;
	case Fix :
	    old_frac = fg->nodes[nd].inputs[0].ty.fracsize;
	    new_frac = fg->nodes[nd].outputs[0].ty.fracsize;

	    if (old_frac == new_frac)
		{
		if (old_width == new_width)
	            delete_this_node (fg, nd, list_copy);
		else
		    cnvrt_to_bits_node (fg, nd, new_width, ND_CHANGE_WIDTH_SE, list_copy);
		}
	    else
	        {
		if (old_frac < new_frac)
	            {
		    shift_width = new_frac - old_frac;
		    if (old_width + shift_width != new_width)
			{
		        cnvrt_to_bits_node (fg, nd, new_width, ND_CHANGE_WIDTH_SE, list_copy);
			insert_bits_node (fg, nd, shift_width, ND_LEFT_SHIFT_MACH, list_copy);
			}
		    else
		        cnvrt_to_bits_node (fg, nd, shift_width, ND_LEFT_SHIFT_MACH, list_copy);
		    }
	        else
	            {
		    shift_width = old_frac - new_frac;
		    if (old_width - shift_width != new_width)
			{
		        cnvrt_to_bits_node (fg, nd, new_width, ND_CHANGE_WIDTH_SE, list_copy);
			insert_bits_node (fg, nd, shift_width, ND_RIGHT_SHIFT, list_copy);
			}
		    else
		        cnvrt_to_bits_node (fg, nd, shift_width, ND_RIGHT_SHIFT, list_copy);
		    }
	        }
	    break;
	case Bits :
	    if (old_width == new_width)
	        delete_this_node (fg, nd, list_copy);
	    else
		cnvrt_to_bits_node (fg, nd, new_width, ND_CHANGE_WIDTH, list_copy);
	    break;
	case Bool :
	    cast_to_bool (fg, nd);
	    break;
	default :
	    assert (0);
	}
    }

void convert_cast_from_bool (FuncGraph *fg, int nd, IntList **list_copy)
    {
    int new_width, shift_width;

    new_width = fg->nodes[nd].outputs[0].ty.totsize;

    switch (fg->nodes[nd].outputs[0].ty.type)
        {
	case Uint :
	case Int :
	case Bits :
	    if (new_width == 1)
	        delete_this_node (fg, nd, list_copy);
	    else
	        cnvrt_to_bits_node (fg, nd, new_width, ND_CHANGE_WIDTH, list_copy);
	    break;
	case Ufix :
	case Fix :
	    shift_width = fg->nodes[nd].outputs[0].ty.fracsize;
	    cnvrt_to_bits_node (fg, nd, shift_width, ND_LEFT_SHIFT_MACH, list_copy);
	    break;
	case Bool :
	    delete_this_node (fg, nd, list_copy);
	    break;
	default :
	    assert (0);
	}
    }

void cast_to_bool (FuncGraph *fg, int nd)
    {
    InputPort *new_inputs;

    new_inputs = make_input_ports (4);
    new_inputs[0] = fg->nodes[nd].inputs[0];
    SacFree (fg->nodes[nd].inputs);
    fg->nodes[nd].inputs = new_inputs;
    fg->nodes[nd].num_inputs = 4;
    fg->nodes[nd].inputs[0].ty.type = Bits;

    fg->nodes[nd].nodetype = ND_RC_SELECTOR;
    fg->nodes[nd].outputs[0].ty.type = Bits;

    fg->nodes[nd].inputs[1].is_const = TRUE;
    fg->nodes[nd].inputs[1].constval = "0x0";
    copy_type_info (&(fg->nodes[nd].inputs[1].ty), &(fg->nodes[nd].inputs[0].ty));

    fg->nodes[nd].inputs[2].is_const = TRUE;
    fg->nodes[nd].inputs[2].constval = "0x0";
    copy_type_info (&(fg->nodes[nd].inputs[2].ty), &(fg->nodes[nd].outputs[0].ty));

    fg->nodes[nd].inputs[3].is_const = TRUE;
    fg->nodes[nd].inputs[3].constval = "0x1";
    copy_type_info (&(fg->nodes[nd].inputs[3].ty), &(fg->nodes[nd].outputs[0].ty));
    }

void cnvrt_to_bits_node (FuncGraph *fg, int nd, int param, DdcfType nty, IntList **itlist)
    {
    InputPort *new_inputs;
    int bwidth;
    char tmp[128];

    bwidth = bit_width (param);
    new_inputs = make_input_ports (2);
    new_inputs[0] = fg->nodes[nd].inputs[0];
    SacFree (fg->nodes[nd].inputs)
    fg->nodes[nd].inputs = new_inputs;
    fg->nodes[nd].num_inputs = 2;
    fg->nodes[nd].nodetype = nty;
    set_type_to_uint (&(fg->nodes[nd].inputs[1].ty), bwidth);
    fg->nodes[nd].inputs[1].is_const = TRUE;
    sprintf (tmp, "0x%x", param);
    fg->nodes[nd].inputs[1].constval = copy_to_const (tmp);
    convert_input_for_dfg (fg, nd, 0);
    convert_input_for_dfg (fg, nd, 1);
    fg->nodes[nd].outputs[0].ty.type = Bits;
    }

void insert_bits_node (FuncGraph *fg, int nd, int param, DdcfType nty, IntList **itlist)
    {
    int bwidth, new_node, snd, spt;
    char tmp[128];

    bwidth = bit_width (param);
    new_node = new_ddcf_node (fg, nty, 2, 1, fg->nodes[nd].loc);

    assert (fg->nodes[nd].inputs[0].back_edges != NULL);
    snd = fg->nodes[nd].inputs[0].back_edges->node;
    spt = fg->nodes[nd].inputs[0].back_edges->port;

    remove_edge (fg, snd, spt, nd, 0);
    add_edge (fg, snd, spt, new_node, 0);
    copy_type_info (&(fg->nodes[new_node].inputs[0].ty), &(fg->nodes[snd].outputs[spt].ty));
    add_edge (fg, new_node, 0, nd, 0);
    if (nty == ND_RIGHT_SHIFT)
        fg->nodes[nd].inputs[0].ty.totsize -= param;
    else
        fg->nodes[nd].inputs[0].ty.totsize += param;
    copy_type_info (&(fg->nodes[new_node].outputs[0].ty), &(fg->nodes[nd].inputs[0].ty));

    set_type_to_uint (&(fg->nodes[new_node].inputs[1].ty), bwidth);
    fg->nodes[new_node].inputs[1].is_const = TRUE;
    sprintf (tmp, "0x%x", param);
    fg->nodes[new_node].inputs[1].constval = copy_to_const (tmp);

    insert_val_before_in_intlist (new_node, nd, itlist);
    }

void delete_this_node (FuncGraph *fg, int nd, IntList **list_copy)
    {
    int snd, spt;

    snd = fg->nodes[nd].inputs[0].back_edges->node;
    spt = fg->nodes[nd].inputs[0].back_edges->port;
    remove_edge (fg, snd, spt, nd, 0);
    retarget_backedges (fg, fg->nodes[nd].outputs[0].targets, nd, 0, snd, spt);
    append_target_list (&(fg->nodes[snd].outputs[spt].targets), fg->nodes[nd].outputs[0].targets);
    fg->nodes[nd].outputs[0].targets = NULL;
    free_ddcf_node (fg, nd, list_copy);
    }

void change_width (FuncGraph *fg, int nd, int pt, int new_width, IntList **list_copy)
    {
    int new_node, snd, spt, bwidth;
    char tmp[128];

    if (fg->nodes[nd].inputs[pt].is_const)
        fg->nodes[nd].inputs[pt].ty.totsize = new_width;
    else
        {
        bwidth = bit_width (new_width);
        new_node = new_ddcf_node (fg, ND_CHANGE_WIDTH, 2, 1, fg->nodes[nd].loc);
        fg->nodes[new_node].inputs[0].back_edges = fg->nodes[nd].inputs[pt].back_edges;
        fg->nodes[nd].inputs[pt].back_edges = NULL;
        retarget (fg, fg->nodes[new_node].inputs[0].back_edges, nd, pt, new_node, 0);
        snd = fg->nodes[new_node].inputs[0].back_edges->node;
        spt = fg->nodes[new_node].inputs[0].back_edges->port;
        copy_type_info (&(fg->nodes[new_node].inputs[0].ty), &(fg->nodes[snd].outputs[spt].ty));
        convert_input_for_dfg (fg, new_node, 0);
    
        set_type_to_uint (&(fg->nodes[new_node].inputs[1].ty), bwidth);
        fg->nodes[new_node].inputs[1].is_const = TRUE;
        sprintf (tmp, "0x%x", new_width);
	fg->nodes[new_node].inputs[1].constval = copy_to_const (tmp);
        convert_input_for_dfg (fg, new_node, 1);

        add_edge (fg, new_node, 0, nd, pt);
        fg->nodes[nd].inputs[pt].ty.totsize = new_width;
        copy_type_info (&(fg->nodes[new_node].outputs[0].ty), &(fg->nodes[nd].inputs[pt].ty));
        fg->nodes[new_node].outputs[0].ty.type = Bits;

        insert_val_before_in_intlist (new_node, nd, list_copy);
	}
    }

void convert_macros (FuncGraph *fg, int nd)
    {
    int i, unsigned_op;

    if (is_complex_num_type (fg->nodes[nd].outputs[0].ty.type))
	assert (0);

    switch (fg->nodes[nd].nodetype)
        {
	case ND_REDUCE_VAL_AT_FIRST_MAX_MACRO :
    	    unsigned_op = is_unsigned (fg->nodes[nd].inputs[0].ty.type);
	    if (unsigned_op)
	        fg->nodes[nd].nodetype = ND_REDUCE_VAL_AT_FIRST_UMAX_MACRO;
	    else
	        fg->nodes[nd].nodetype = ND_REDUCE_VAL_AT_FIRST_IMAX_MACRO;
	    break;
	case ND_REDUCE_VAL_AT_FIRST_MIN_MACRO :
    	    unsigned_op = is_unsigned (fg->nodes[nd].inputs[0].ty.type);
	    if (unsigned_op)
	        fg->nodes[nd].nodetype = ND_REDUCE_VAL_AT_FIRST_UMIN_MACRO;
	    else
	        fg->nodes[nd].nodetype = ND_REDUCE_VAL_AT_FIRST_IMIN_MACRO;
	    break;
	case ND_REDUCE_VAL_AT_LAST_MAX_MACRO :
    	    unsigned_op = is_unsigned (fg->nodes[nd].inputs[0].ty.type);
	    if (unsigned_op)
	        fg->nodes[nd].nodetype = ND_REDUCE_VAL_AT_LAST_UMAX_MACRO;
	    else
	        fg->nodes[nd].nodetype = ND_REDUCE_VAL_AT_LAST_IMAX_MACRO;
	    break;
	case ND_REDUCE_VAL_AT_LAST_MIN_MACRO :
    	    unsigned_op = is_unsigned (fg->nodes[nd].inputs[0].ty.type);
	    if (unsigned_op)
	        fg->nodes[nd].nodetype = ND_REDUCE_VAL_AT_LAST_UMIN_MACRO;
	    else
	        fg->nodes[nd].nodetype = ND_REDUCE_VAL_AT_LAST_IMIN_MACRO;
	    break;
	case ND_REDUCE_SUM_MACRO :
    	    unsigned_op = is_unsigned (fg->nodes[nd].outputs[0].ty.type);
	    if (unsigned_op)
	        fg->nodes[nd].nodetype = ND_REDUCE_USUM_MACRO;
	    else
	        fg->nodes[nd].nodetype = ND_REDUCE_ISUM_MACRO;
	    break;
	case ND_REDUCE_MAX_MACRO :
    	    unsigned_op = is_unsigned (fg->nodes[nd].outputs[0].ty.type);
	    if (unsigned_op)
	        fg->nodes[nd].nodetype = ND_REDUCE_UMAX_MACRO;
	    else
	        fg->nodes[nd].nodetype = ND_REDUCE_IMAX_MACRO;
	    break;
	case ND_REDUCE_MIN_MACRO :
    	    unsigned_op = is_unsigned (fg->nodes[nd].outputs[0].ty.type);
	    if (unsigned_op)
	        fg->nodes[nd].nodetype = ND_REDUCE_UMIN_MACRO;
	    else
	        fg->nodes[nd].nodetype = ND_REDUCE_IMIN_MACRO;
	    break;
	case ND_REDUCE_MEDIAN_MACRO :
    	    unsigned_op = is_unsigned (fg->nodes[nd].outputs[0].ty.type);
	    if (unsigned_op)
	        fg->nodes[nd].nodetype = ND_REDUCE_UMEDIAN_MACRO;
	    else
	        fg->nodes[nd].nodetype = ND_REDUCE_IMEDIAN_MACRO;
	    break;
	case ND_REDUCE_HIST_MACRO :
	case ND_REDUCE_AND_MACRO :
	case ND_REDUCE_OR_MACRO :
	    break;
	default :
	    assert (0);
	}

    fg->nodes[nd].outputs[0].ty.type = Bits;

    for (i=0; i<fg->nodes[nd].num_inputs; i++)
        convert_input_for_dfg (fg, nd, i);
    }

void convert_mul (FuncGraph *fg, int nd, IntList **list_copy)
    {
    if (is_unsigned (fg->nodes[nd].outputs[0].ty.type))
	fg->nodes[nd].nodetype = ND_UMUL;
    else
	fg->nodes[nd].nodetype = ND_IMUL;

    convert_input_for_dfg (fg, nd, 0);
    convert_input_for_dfg (fg, nd, 1);
    fg->nodes[nd].outputs[0].ty.type = Bits;
    }

void convert_arith (FuncGraph *fg, int nd, IntList **list_copy)
    {
    int fsz0, fsz1, fsz;

    if (is_complex_num_type (fg->nodes[nd].outputs[0].ty.type))
	assert (0);

    fsz0 = fg->nodes[nd].inputs[0].ty.fracsize;
    fsz1 = fg->nodes[nd].inputs[1].ty.fracsize;
    fsz = fg->nodes[nd].outputs[0].ty.fracsize;

    if (is_unsigned (fg->nodes[nd].outputs[0].ty.type))
	switch (fg->nodes[nd].nodetype)
	    {
	    case ND_ADD :
		fg->nodes[nd].nodetype = ND_UADD;
		break;
	    case ND_SUB :
		fg->nodes[nd].nodetype = ND_USUB;
		break;
	    default :
	        assert (0);
	    }
    else
	{
	switch (fg->nodes[nd].nodetype)
	    {
	    case ND_ADD :
		fg->nodes[nd].nodetype = ND_IADD;
		break;
	    case ND_SUB :
		fg->nodes[nd].nodetype = ND_ISUB;
		break;
	    default :
	        assert (0);
	    }

	/* add a sign bit to the unsigned input */
	if (is_unsigned (fg->nodes[nd].inputs[0].ty.type))
	    change_width (fg, nd, 0, 1+fg->nodes[nd].inputs[0].ty.totsize, list_copy);
	else if (is_unsigned (fg->nodes[nd].inputs[1].ty.type))
	    change_width (fg, nd, 1, 1+fg->nodes[nd].inputs[1].ty.totsize, list_copy);
	}

    /* if necessary, shift inputs to align the operands */
    if ((fsz0 > 0) || (fsz1 > 0))
        {
        if (fsz > fsz0)
	    shift_arg (fg, fsz-fsz0, nd, 0, list_copy, 'L');
	else if (fsz < fsz0)
	    shift_arg (fg, fsz0-fsz, nd, 0, list_copy, 'R');

        if (fsz > fsz1)
	    shift_arg (fg, fsz-fsz1, nd, 1, list_copy, 'L');
	else if (fsz < fsz1)
	    shift_arg (fg, fsz1-fsz, nd, 1, list_copy, 'R');
        }

    fg->nodes[nd].outputs[0].ty.type = Bits;

    convert_input_for_dfg (fg, nd, 0);
    convert_input_for_dfg (fg, nd, 1);
    }

void convert_compares (FuncGraph *fg, int nd, IntList **list_copy)
    {
    int fsz0, fsz1, fsz, tsz0, tsz1;

    if (is_complex_num_type (fg->nodes[nd].outputs[0].ty.type))
	assert (0);

    tsz0 = fg->nodes[nd].inputs[0].ty.totsize;
    fsz0 = fg->nodes[nd].inputs[0].ty.fracsize;
    tsz1 = fg->nodes[nd].inputs[1].ty.totsize;
    fsz1 = fg->nodes[nd].inputs[1].ty.fracsize;
    fsz = fsz0>fsz1?fsz0:fsz1;
    if (tsz0 + (fsz - fsz0) > 32)
        fsz = tsz0 + (fsz - fsz0) - 32;
    if (tsz1 + (fsz - fsz1) > 32)
        fsz = tsz1 + (fsz - fsz1) - 32;

    if (is_unsigned (fg->nodes[nd].inputs[0].ty.type) && is_unsigned (fg->nodes[nd].inputs[1].ty.type))
	switch (fg->nodes[nd].nodetype)
	    {
	    case ND_LT :
		fg->nodes[nd].nodetype = ND_ULT;
		break;
	    case ND_LE :
		fg->nodes[nd].nodetype = ND_ULE;
		break;
	    case ND_GT :
		fg->nodes[nd].nodetype = ND_UGT;
		break;
	    case ND_GE :
		fg->nodes[nd].nodetype = ND_UGE;
		break;
	    case ND_EQ :
		fg->nodes[nd].nodetype = ND_UEQ;
		break;
	    case ND_NEQ :
		fg->nodes[nd].nodetype = ND_UNEQ;
		break;
	    default :
                break;
	    }
    else
	{
	switch (fg->nodes[nd].nodetype)
	    {
	    case ND_LT :
		fg->nodes[nd].nodetype = ND_ILT;
		break;
	    case ND_LE :
		fg->nodes[nd].nodetype = ND_ILE;
		break;
	    case ND_GT :
		fg->nodes[nd].nodetype = ND_IGT;
		break;
	    case ND_GE :
		fg->nodes[nd].nodetype = ND_IGE;
		break;
	    case ND_EQ :
		fg->nodes[nd].nodetype = ND_IEQ;
		break;
	    case ND_NEQ :
		fg->nodes[nd].nodetype = ND_INEQ;
		break;
	    default :
                break;
	    }

	/* add a sign bit to the unsigned input */
	if (is_unsigned (fg->nodes[nd].inputs[0].ty.type))
	    change_width (fg, nd, 0, 1+fg->nodes[nd].inputs[0].ty.totsize, list_copy);
	else
	    change_width (fg, nd, 1, 1+fg->nodes[nd].inputs[1].ty.totsize, list_copy);
	}

    /* if necessary, shift inputs to align the operands */
    if ((fsz0 > 0) || (fsz1 > 0))
        {
        if (fsz > fsz0)
	    shift_arg (fg, fsz-fsz0, nd, 0, list_copy, 'L');
	else if (fsz < fsz0)
	    shift_arg (fg, fsz0-fsz, nd, 0, list_copy, 'R');

        if (fsz > fsz1)
	    shift_arg (fg, fsz-fsz1, nd, 1, list_copy, 'L');
	else if (fsz < fsz1)
	    shift_arg (fg, fsz1-fsz, nd, 1, list_copy, 'R');
        }

    fg->nodes[nd].outputs[0].ty.type = Bits;

    convert_input_for_dfg (fg, nd, 0);
    convert_input_for_dfg (fg, nd, 1);
    }


void shift_arg (FuncGraph *fg, int dist, int nd, int pt, IntList **list_copy, char dir)
    {
    int new_node, width, snd, spt;
    char tmp[128];

    if (dir == 'L')
        new_node = new_ddcf_node (fg, ND_LEFT_SHIFT, 2, 1, fg->nodes[nd].loc);
    else if (dir == 'R')
        new_node = new_ddcf_node (fg, ND_RIGHT_SHIFT, 2, 1, fg->nodes[nd].loc);
    else
        assert (0);

    if (fg->nodes[nd].inputs[pt].is_const)
	{
        fg->nodes[new_node].inputs[0] = fg->nodes[nd].inputs[pt];
	fg->nodes[nd].inputs[pt].constval = NULL;
	fg->nodes[nd].inputs[pt].is_const = FALSE;
	}
    else
        {
        fg->nodes[new_node].inputs[0].back_edges = fg->nodes[nd].inputs[pt].back_edges;
        fg->nodes[nd].inputs[pt].back_edges = NULL;
        retarget (fg, fg->nodes[new_node].inputs[0].back_edges, nd, pt, new_node, 0);
        snd = fg->nodes[new_node].inputs[0].back_edges->node;
        spt = fg->nodes[new_node].inputs[0].back_edges->port;
        copy_type_info (&(fg->nodes[new_node].inputs[0].ty), &(fg->nodes[snd].outputs[spt].ty));
	}

    convert_input_for_dfg (fg, new_node, 0);
    
    width = bit_width (dist);
    set_type_to_uint (&(fg->nodes[new_node].inputs[1].ty), width);
    fg->nodes[new_node].inputs[1].is_const = TRUE;
    sprintf (tmp, "0x%x", dist);
    fg->nodes[new_node].inputs[1].constval = copy_to_const (tmp);
    convert_input_for_dfg (fg, new_node, 1);

    add_edge (fg, new_node, 0, nd, pt);

    if (dir == 'L')
        fg->nodes[nd].inputs[pt].ty.totsize += dist;
    else
        fg->nodes[nd].inputs[pt].ty.totsize -= dist;

    copy_type_info (&(fg->nodes[new_node].outputs[0].ty), &(fg->nodes[nd].inputs[pt].ty));
    fg->nodes[new_node].outputs[0].ty.type = Bits;

    insert_val_before_in_intlist (new_node, nd, list_copy);
    }

void window_convert (FuncGraph *fg, int nd, IntList **list_copy)
    {
    TypeInfo ty;
    Edge *tg;
    int i, n_outputs, new_window, r_node, idx, gin, io_num, parent;

    assert (fg->nodes[nd].inputs[0].back_edges != NULL);
    gin = fg->nodes[nd].inputs[0].back_edges->node;
    parent = fg->nodes[gin].My_graph;
    io_num = fg->nodes[gin].Io_num + 1;

    ty = fg->nodes[nd].outputs[0].ty;
    ty.kind = Scalar;
    ty.dims = 0;

    switch (fg->nodes[nd].outputs[0].ty.dims)
        {
	case 1 :
            {
            int d0;

            d0 = fg->nodes[nd].outputs[0].ty.dim_sizes[0];
            n_outputs = d0 + 1;
            new_window = new_ddcf_node (fg, ND_RC_WINDOW_GEN_1D, 4, n_outputs, fg->nodes[nd].loc);

	    fg->nodes[new_window].outputs[d0] = fg->nodes[nd].outputs[1];
	    retarget_backedges (fg, fg->nodes[new_window].outputs[d0].targets, nd, 1, new_window, d0);

	    for (i=0; i<d0; i++)
	        copy_type_info (&(fg->nodes[new_window].outputs[i].ty), &ty);
	    
            retarget (fg, fg->nodes[nd].inputs[0].back_edges, nd, 0, new_window, 0);
            fg->nodes[new_window].inputs[0] = fg->nodes[nd].inputs[0];
            assert (fg->nodes[nd].inputs[1].is_const);
            fg->nodes[new_window].inputs[1] = fg->nodes[nd].inputs[1];
            fg->nodes[new_window].inputs[2] = fg->nodes[nd].inputs[2];
	    if (!fg->nodes[nd].inputs[2].is_const)
	        retarget (fg, fg->nodes[nd].inputs[2].back_edges, nd, 2, new_window, 2);
	    gin = fg->nodes[parent].My_inputs[io_num++];
	    add_edge (fg, gin, 0, new_window, 3);
    	    set_type_to_uint (&(fg->nodes[new_window].inputs[3].ty), 32);

	    for (tg=fg->nodes[nd].outputs[0].targets; tg!=NULL; tg=tg->link)
	        {
	        r_node = tg->node;
	        assert (fg->nodes[r_node].nodetype == ND_ARRAYREF);
	        assert (fg->nodes[r_node].outputs[0].ty.kind == Scalar);
	        assert (fg->nodes[r_node].inputs[1].is_const);
	        sscanf (fg->nodes[r_node].inputs[1].constval, "%d", &idx);

		if (idx >= d0)
		    handle_error (ErrDdcf2dfgWindowIndex, Deadly, fg->nodes[r_node].loc.line,
		    		fg->nodes[r_node].loc.file, fg->nodes[r_node].loc.func, idx);

	        retarget_backedges (fg, fg->nodes[r_node].outputs[0].targets, r_node, 0, new_window, idx);
	        append_target_list (&(fg->nodes[new_window].outputs[idx].targets),
						fg->nodes[r_node].outputs[0].targets);
	        fg->nodes[r_node].outputs[0].targets = NULL;
	        }

	    for (tg=fg->nodes[nd].outputs[0].targets; tg!=NULL; tg=tg->link)
	        {
	        r_node = tg->node;
	        free_edge_list (&(fg->nodes[r_node].inputs[0].back_edges));
	        free_ddcf_node (fg, r_node, list_copy);
	        }

	    free_edge_list (&(fg->nodes[nd].outputs[0].targets));
	    free_ddcf_node (fg, nd, NULL);
	    replace_in_intlist (list_copy, nd, new_window);
	    }
	    break;
	case 2 :
	    {
	    int d0, d1, idx0, idx1;

            d0 = fg->nodes[nd].outputs[0].ty.dim_sizes[0];
            d1 = fg->nodes[nd].outputs[0].ty.dim_sizes[1];
            n_outputs = d0 * d1 + 2;
            new_window = new_ddcf_node (fg, ND_RC_WINDOW_GEN_2D, 7, n_outputs, fg->nodes[nd].loc);

	    fg->nodes[new_window].outputs[d0*d1] = fg->nodes[nd].outputs[1];
	    retarget_backedges (fg, fg->nodes[new_window].outputs[d0*d1].targets, nd, 1, new_window, d0*d1);
	    fg->nodes[new_window].outputs[d0*d1+1] = fg->nodes[nd].outputs[2];
	    retarget_backedges (fg, fg->nodes[new_window].outputs[d0*d1+1].targets, nd, 2, new_window, d0*d1+1);

	    for (i=0; i<d0*d1; i++)
	        copy_type_info (&(fg->nodes[new_window].outputs[i].ty), &ty);

            retarget (fg, fg->nodes[nd].inputs[0].back_edges, nd, 0, new_window, 0);
            fg->nodes[new_window].inputs[0] = fg->nodes[nd].inputs[0];
            assert (fg->nodes[nd].inputs[1].is_const);
            fg->nodes[new_window].inputs[1] = fg->nodes[nd].inputs[1];
            fg->nodes[new_window].inputs[2] = fg->nodes[nd].inputs[2];
	    if (!fg->nodes[nd].inputs[2].is_const)
	        retarget (fg, fg->nodes[nd].inputs[2].back_edges, nd, 2, new_window, 2);
	    gin = fg->nodes[parent].My_inputs[io_num++];
	    add_edge (fg, gin, 0, new_window, 3);
    	    set_type_to_uint (&(fg->nodes[new_window].inputs[3].ty), 32);
            assert (fg->nodes[nd].inputs[3].is_const);
            fg->nodes[new_window].inputs[4] = fg->nodes[nd].inputs[3];
            fg->nodes[new_window].inputs[5] = fg->nodes[nd].inputs[4];
	    if (!fg->nodes[nd].inputs[4].is_const)
	        retarget (fg, fg->nodes[nd].inputs[4].back_edges, nd, 4, new_window, 5);
	    gin = fg->nodes[parent].My_inputs[io_num++];
	    add_edge (fg, gin, 0, new_window, 6);
    	    set_type_to_uint (&(fg->nodes[new_window].inputs[6].ty), 32);

	    for (tg=fg->nodes[nd].outputs[0].targets; tg!=NULL; tg=tg->link)
	        {
	        r_node = tg->node;
	        assert (fg->nodes[r_node].nodetype == ND_ARRAYREF);
	        assert (fg->nodes[r_node].outputs[0].ty.kind == Scalar);
	        assert (fg->nodes[r_node].inputs[1].is_const);
	        assert (fg->nodes[r_node].inputs[2].is_const);
	        sscanf (fg->nodes[r_node].inputs[1].constval, "%d", &idx0);
	        sscanf (fg->nodes[r_node].inputs[2].constval, "%d", &idx1);

		if (idx0 >= d0)
		    handle_error (ErrDdcf2dfgWindowIndex, Deadly, fg->nodes[r_node].loc.line,
		    		fg->nodes[r_node].loc.file, fg->nodes[r_node].loc.func, idx0);
		if (idx1 >= d1)
		    handle_error (ErrDdcf2dfgWindowIndex, Deadly, fg->nodes[r_node].loc.line,
		    		fg->nodes[r_node].loc.file, fg->nodes[r_node].loc.func, idx1);

		idx = d1*idx0 + idx1;
	        retarget_backedges (fg, fg->nodes[r_node].outputs[0].targets, r_node, 0, new_window, idx);
	        append_target_list (&(fg->nodes[new_window].outputs[idx].targets),
						fg->nodes[r_node].outputs[0].targets);
	        fg->nodes[r_node].outputs[0].targets = NULL;
	        }

	    for (tg=fg->nodes[nd].outputs[0].targets; tg!=NULL; tg=tg->link)
	        {
	        r_node = tg->node;
	        free_edge_list (&(fg->nodes[r_node].inputs[0].back_edges));
	        free_ddcf_node (fg, r_node, list_copy);
	        }

	    free_edge_list (&(fg->nodes[nd].outputs[0].targets));
	    free_ddcf_node (fg, nd, NULL);
	    replace_in_intlist (list_copy, nd, new_window);
	    }
	    break;
	default :
	    assert (0);
	}

    set_type_to_uint (&(fg->nodes[new_window].inputs[0].ty), 32);
    }

void slicegen_convert (FuncGraph *fg, int nd, IntList **list_copy)
{
  TypeInfo ty;
  Edge *tg;
  int i, d0, val, n_outputs, new_gen, r_node, idx, gin, io_num, parent;
  int slice_column;
  char str[256];

  slice_column = (fg->nodes[nd].Reftypes[0] == ':');

  /* here we assume that the source array extent inputs are immediately after the array itself */
  assert (fg->nodes[nd].inputs[0].back_edges != NULL);
  gin = fg->nodes[nd].inputs[0].back_edges->node;
  parent = fg->nodes[gin].My_graph;
  io_num = fg->nodes[gin].Io_num + 1;

  ty = fg->nodes[nd].outputs[0].ty;
  ty.kind = Scalar;
  ty.dims = 0;

  if (slice_column)
    {
      d0 = fg->nodes[nd].inputs[0].ty.dim_sizes[0];
      n_outputs = d0 + 1;
      new_gen = new_ddcf_node (fg, ND_RC_SLICE_GEN_2D_COL, 4, n_outputs, fg->nodes[nd].loc);
    }
  else
    {
      d0 = fg->nodes[nd].inputs[0].ty.dim_sizes[1];
      n_outputs = d0 + 1;
      new_gen = new_ddcf_node (fg, ND_RC_SLICE_GEN_2D_ROW, 4, n_outputs, fg->nodes[nd].loc);
    }

  fg->nodes[new_gen].outputs[d0] = fg->nodes[nd].outputs[1];
  retarget_backedges (fg, fg->nodes[new_gen].outputs[d0].targets, nd, 1, new_gen, d0);

  for (i=0; i<d0; i++)
    copy_type_info (&(fg->nodes[new_gen].outputs[i].ty), &ty);
	    
  /* the array input and step input get transferred directly from 'nd' */
  fg->nodes[new_gen].inputs[0] = fg->nodes[nd].inputs[0];
  retarget (fg, fg->nodes[nd].inputs[0].back_edges, nd, 0, new_gen, 0);
  fg->nodes[new_gen].inputs[1] = fg->nodes[nd].inputs[1];
  if (!fg->nodes[nd].inputs[1].is_const)
    retarget (fg, fg->nodes[nd].inputs[1].back_edges, nd, 1, new_gen, 1);

  /* the two extents inputs must be installed; one is a constant from the size_prop pass */
  if (slice_column)
    {
      val = fg->nodes[nd].inputs[0].ty.dim_sizes[0];
      sprintf (str, "%d", val);
      fg->nodes[new_gen].inputs[2].constval = copy_to_const (str);
      fg->nodes[new_gen].inputs[2].is_const = TRUE;
      set_type_to_uint (&(fg->nodes[new_gen].inputs[2].ty), 32);

      gin = fg->nodes[parent].My_inputs[io_num+1];
      add_edge (fg, gin, 0, new_gen, 3);
      set_type_to_uint (&(fg->nodes[new_gen].inputs[3].ty), 32);
    }
  else
    {
      val = fg->nodes[nd].inputs[0].ty.dim_sizes[1];
      sprintf (str, "%d", val);
      fg->nodes[new_gen].inputs[3].constval = copy_to_const (str);
      fg->nodes[new_gen].inputs[3].is_const = TRUE;
      set_type_to_uint (&(fg->nodes[new_gen].inputs[3].ty), 32);

      gin = fg->nodes[parent].My_inputs[io_num];
      add_edge (fg, gin, 0, new_gen, 2);
      set_type_to_uint (&(fg->nodes[new_gen].inputs[2].ty), 32);
    }

  for (tg=fg->nodes[nd].outputs[0].targets; tg!=NULL; tg=tg->link)
    {
      r_node = tg->node;
      assert (fg->nodes[r_node].nodetype == ND_ARRAYREF);
      assert (fg->nodes[r_node].outputs[0].ty.kind == Scalar);
      assert (fg->nodes[r_node].inputs[1].is_const);
      sscanf (fg->nodes[r_node].inputs[1].constval, "%d", &idx);

      if (idx >= d0)
	handle_error (ErrDdcf2dfgVectorSliceIndex, Deadly, fg->nodes[r_node].loc.line,
		      fg->nodes[r_node].loc.file, fg->nodes[r_node].loc.func, idx);

      retarget_backedges (fg, fg->nodes[r_node].outputs[0].targets, r_node, 0, new_gen, idx);
      append_target_list (&(fg->nodes[new_gen].outputs[idx].targets),
			  fg->nodes[r_node].outputs[0].targets);
      fg->nodes[r_node].outputs[0].targets = NULL;
    }

  for (tg=fg->nodes[nd].outputs[0].targets; tg!=NULL; tg=tg->link)
    {
      r_node = tg->node;
      free_edge_list (&(fg->nodes[r_node].inputs[0].back_edges));
      free_ddcf_node (fg, r_node, list_copy);
    }

  free_edge_list (&(fg->nodes[nd].outputs[0].targets));
  free_ddcf_node (fg, nd, NULL);
  replace_in_intlist (list_copy, nd, new_gen);

  set_type_to_uint (&(fg->nodes[new_gen].inputs[0].ty), 32);
}

void elegen_convert (FuncGraph *fg, int nd, IntList **list_copy)
{
  int new_elegen, gin, io_num, parent;

  assert (fg->nodes[nd].inputs[0].back_edges != NULL);
  gin = fg->nodes[nd].inputs[0].back_edges->node;
  parent = fg->nodes[gin].My_graph;

  /* make 'io_num' point the first of the extents inputs */
  io_num = fg->nodes[gin].Io_num + 1;

  switch (fg->nodes[nd].inputs[0].ty.dims)
    {
    case 1 :
      new_elegen = new_ddcf_node (fg, ND_RC_ELE_GEN_1D, 3, 2, fg->nodes[nd].loc);

      fg->nodes[new_elegen].outputs[1] = fg->nodes[nd].outputs[1];
      retarget_backedges (fg, fg->nodes[new_elegen].outputs[1].targets, nd, 1, new_elegen, 1);

      retarget (fg, fg->nodes[nd].inputs[0].back_edges, nd, 0, new_elegen, 0);
      fg->nodes[new_elegen].inputs[0] = fg->nodes[nd].inputs[0];
      fg->nodes[new_elegen].inputs[1] = fg->nodes[nd].inputs[1];
      if (!fg->nodes[nd].inputs[1].is_const)
	retarget (fg, fg->nodes[nd].inputs[1].back_edges, nd, 1, new_elegen, 1);
      gin = fg->nodes[parent].My_inputs[io_num++];
      add_edge (fg, gin, 0, new_elegen, 2);
      set_type_to_uint (&(fg->nodes[new_elegen].inputs[2].ty), 32);

      fg->nodes[new_elegen].outputs[0] = fg->nodes[nd].outputs[0];
      retarget_backedges (fg, fg->nodes[new_elegen].outputs[0].targets, nd, 0, new_elegen, 0);

      free_ddcf_node (fg, nd, NULL);
      replace_in_intlist (list_copy, nd, new_elegen);

      break;
    case 2 :
      new_elegen = new_ddcf_node (fg, ND_RC_ELE_GEN_2D, 5, 3, fg->nodes[nd].loc);

      fg->nodes[new_elegen].outputs[1] = fg->nodes[nd].outputs[1];
      retarget_backedges (fg, fg->nodes[new_elegen].outputs[1].targets, nd, 1, new_elegen, 1);
      fg->nodes[new_elegen].outputs[2] = fg->nodes[nd].outputs[2];
      retarget_backedges (fg, fg->nodes[new_elegen].outputs[2].targets, nd, 2, new_elegen, 2);

      retarget (fg, fg->nodes[nd].inputs[0].back_edges, nd, 0, new_elegen, 0);
      fg->nodes[new_elegen].inputs[0] = fg->nodes[nd].inputs[0];

      fg->nodes[new_elegen].inputs[1] = fg->nodes[nd].inputs[1];
      if (!fg->nodes[nd].inputs[1].is_const)
	retarget (fg, fg->nodes[nd].inputs[1].back_edges, nd, 1, new_elegen, 1);
      gin = fg->nodes[parent].My_inputs[io_num++];
      add_edge (fg, gin, 0, new_elegen, 2);
      set_type_to_uint (&(fg->nodes[new_elegen].inputs[2].ty), 32);

      fg->nodes[new_elegen].inputs[3] = fg->nodes[nd].inputs[2];
      if (!fg->nodes[nd].inputs[2].is_const)
	retarget (fg, fg->nodes[nd].inputs[2].back_edges, nd, 2, new_elegen, 3);
      gin = fg->nodes[parent].My_inputs[io_num++];
      add_edge (fg, gin, 0, new_elegen, 4);
      set_type_to_uint (&(fg->nodes[new_elegen].inputs[4].ty), 32);

      fg->nodes[new_elegen].outputs[0] = fg->nodes[nd].outputs[0];
      retarget_backedges (fg, fg->nodes[new_elegen].outputs[0].targets, nd, 0, new_elegen, 0);

      free_ddcf_node (fg, nd, NULL);
      replace_in_intlist (list_copy, nd, new_elegen);

      break;
    case 3 :
      new_elegen = new_ddcf_node (fg, ND_RC_ELE_GEN_3D, 7, 4, fg->nodes[nd].loc);

      fg->nodes[new_elegen].outputs[1] = fg->nodes[nd].outputs[1];
      retarget_backedges (fg, fg->nodes[new_elegen].outputs[1].targets, nd, 1, new_elegen, 1);
      fg->nodes[new_elegen].outputs[2] = fg->nodes[nd].outputs[2];
      retarget_backedges (fg, fg->nodes[new_elegen].outputs[2].targets, nd, 2, new_elegen, 2);
      fg->nodes[new_elegen].outputs[3] = fg->nodes[nd].outputs[3];
      retarget_backedges (fg, fg->nodes[new_elegen].outputs[3].targets, nd, 3, new_elegen, 3);

      retarget (fg, fg->nodes[nd].inputs[0].back_edges, nd, 0, new_elegen, 0);
      fg->nodes[new_elegen].inputs[0] = fg->nodes[nd].inputs[0];

      fg->nodes[new_elegen].inputs[1] = fg->nodes[nd].inputs[1];
      if (!fg->nodes[nd].inputs[1].is_const)
	retarget (fg, fg->nodes[nd].inputs[1].back_edges, nd, 1, new_elegen, 1);
      gin = fg->nodes[parent].My_inputs[io_num++];
      add_edge (fg, gin, 0, new_elegen, 2);
      set_type_to_uint (&(fg->nodes[new_elegen].inputs[2].ty), 32);

      fg->nodes[new_elegen].inputs[3] = fg->nodes[nd].inputs[2];
      if (!fg->nodes[nd].inputs[2].is_const)
	retarget (fg, fg->nodes[nd].inputs[2].back_edges, nd, 2, new_elegen, 3);
      gin = fg->nodes[parent].My_inputs[io_num++];
      add_edge (fg, gin, 0, new_elegen, 4);
      set_type_to_uint (&(fg->nodes[new_elegen].inputs[4].ty), 32);

      fg->nodes[new_elegen].inputs[5] = fg->nodes[nd].inputs[3];
      if (!fg->nodes[nd].inputs[3].is_const)
	retarget (fg, fg->nodes[nd].inputs[3].back_edges, nd, 3, new_elegen, 5);
      gin = fg->nodes[parent].My_inputs[io_num++];
      add_edge (fg, gin, 0, new_elegen, 6);
      set_type_to_uint (&(fg->nodes[new_elegen].inputs[6].ty), 32);

      fg->nodes[new_elegen].outputs[0] = fg->nodes[nd].outputs[0];
      retarget_backedges (fg, fg->nodes[new_elegen].outputs[0].targets, nd, 0, new_elegen, 0);

      free_ddcf_node (fg, nd, NULL);
      replace_in_intlist (list_copy, nd, new_elegen);

      break;
    default :
      assert (0);
    }

  set_type_to_uint (&(fg->nodes[new_elegen].inputs[0].ty), 32);
}

int rc_capable (FuncGraph *fg, int id)
    {
    IntList *it;
    Edge *tg;
    int nd, gen, i, j, gin;

    /* reject if it's not a FORALL loop */
    if ((fg->nodes[id].nodetype != ND_FORALL) && (fg->nodes[id].nodetype != ND_FORNXT))
	{
	FAILPRINT ("fail 1\n");
        return FALSE;
	}

    if (has_pragma (fg, id, PRAG_NO_DFG))
	{
	FAILPRINT ("fail 2\n");
        return FALSE;
	}

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;

	/* reject if float, double or complex types */
	if (fg->nodes[nd].nodetype == ND_INTRINCALL)
	    switch (fg->nodes[nd].Intrin_func)
	        {
		case IntrinSqrt :
		    {
		    int cnode;
		    assert (fg->nodes[nd].outputs[0].targets != NULL);
		    if (fg->nodes[nd].outputs[0].targets->link != NULL)
		        {
			FAILPRINT ("fail 42\n");
			return FALSE;
			}
		    cnode = fg->nodes[nd].outputs[0].targets->node;
		    if (fg->nodes[cnode].nodetype != ND_CAST)
		        {
			FAILPRINT ("fail 43\n");
			return FALSE;
			}
		    } break;
		default :
		    break;
		}
	else
	    for (i=0; i<fg->nodes[nd].num_outputs; i++)
	        switch (fg->nodes[nd].outputs[i].ty.type)
	            {
		    case Uint :
		    case Int :
		    case Ufix :
		    case Fix :
		    case Bits :
		    case Bool :
		        break;
		    default :
		        {
		        FAILPRINT ("fail 3\n");
		        return FALSE;
		        }
		    }

	/* a variety of nodes will cause rejection */
	switch (fg->nodes[nd].nodetype)
	    {
	    case ND_ARRAYREF :
		{
		int snd;

		/* an ARRAYREF may have dynamic indices if it's not fed by a generator,
		 * but if fed by a generator, its indices must be constants; it also
		 * must not take a slice;
		 */
		if (fg->nodes[nd].outputs[0].ty.kind != Scalar)
		    {
        	    FAILPRINT ("fail 4\n");
		    return FALSE;
		    }

		assert (fg->nodes[nd].inputs[0].back_edges != NULL);
		snd = fg->nodes[nd].inputs[0].back_edges->node;
		switch (fg->nodes[snd].nodetype)
		    {
		    case ND_CROSS_PROD :
		    case ND_DOT_PROD :
	        	for (i=1; i<fg->nodes[nd].num_inputs; i++)
		    	    if (!fg->nodes[nd].inputs[i].is_const)
				{
        	    		FAILPRINT ("fail 5\n");
		        	return FALSE;
				}
			break;
		    case ND_G_INPUT :
		        break;
		    default :
			{
			FAILPRINT ("fail 6\n");
		        return FALSE;
			}
		    }

		}
		break;
	    case ND_ARR_DEF :
		/* this node can only feed a TILE node */
	        for (tg=fg->nodes[nd].outputs[0].targets; tg!=NULL; tg=tg->link)
		    if (fg->nodes[tg->node].nodetype != ND_CONSTRUCT_TILE)
		        {
			FAILPRINT ("fail 7\n");
			return FALSE;
			}
		break;
	    case ND_MUL :
		if (!does_mult_fit_in_32_bits (fg, nd))
		    {
		    FAILPRINT ("fail 8\n");
		    return FALSE;
		    }
	        break;
	    case ND_INTRINCALL :
		switch (fg->nodes[nd].Intrin_func)
		    {
		    case IntrinSqrt :
			/*
			if ((fg->nodes[nd].inputs[0].ty.type!=Uint) && (fg->nodes[nd].inputs[0].ty.type!=Int))
			    {
			    FAILPRINT ("fail 9\n");
			    return FALSE;
			    }
			*/
		        break;
		    default :
			{
			FAILPRINT ("fail 10\n");
		        return FALSE;
			}
		    }
	        break;
	    case ND_CONSTRUCT_TILE:
		{
		int snd;
		snd = fg->nodes[nd].inputs[0].back_edges->node;
		if (fg->nodes[snd].nodetype != ND_ARR_DEF)
		    {
		    FAILPRINT ("fail 20\n");
		    return FALSE;
		    }
		}
	        break;
	    case ND_G_INPUT_NEXT :
		if (fg->nodes[nd].outputs[0].ty.kind != Scalar)
		    {
		    FAILPRINT ("fail 55\n");
		    return FALSE;
		    }
	        break;
	    case ND_RC_COMPUTE :
	    case ND_RC_FORALL :
	    case ND_ARR_CONPERIM :
	    case ND_ARR_CONCAT :
	    case ND_EXTENTS :
	    case ND_FUNC :
	    case ND_FCALL :
	    case ND_FORNXT :
	    case ND_FORALL :
	    case ND_WHILE :
	    case ND_WHILE_PRED :
	    case ND_DIV :
	    case ND_MOD :
	    case ND_CONSTRUCT_CONCAT :
	    case ND_CONSTRUCT_CONCAT_MASKED :
	    case ND_REDUCE_VAL_AT_FIRST_MAX :
	    case ND_REDUCE_VAL_AT_FIRST_MIN :
	    case ND_REDUCE_VAL_AT_LAST_MAX :
	    case ND_REDUCE_VAL_AT_LAST_MIN :
	    case ND_REDUCE_VAL_AT_MAXS :
	    case ND_REDUCE_VAL_AT_MINS :
	    case ND_REDUCE_PRODUCT :
	    case ND_REDUCE_MEAN :
	    case ND_REDUCE_ST_DEV :
	    case ND_REDUCE_MODE :
	    case ND_REDUCE_MEDIAN :
	    case ND_REDUCE_HIST_MACRO :
	    case ND_ACCUM_PRODUCT :
	    case ND_ACCUM_MEAN :
	    case ND_ACCUM_ST_DEV :
	    case ND_ACCUM_MEDIAN :
		{
		FAILPRINT ("fail 11\n");
	        return FALSE;
		}
	    default :
	        break;
	    }
	}

    gen = find_generator_graph (fg, id);

    /* reject any cross products */
    if (fg->nodes[gen].nodetype == ND_CROSS_PROD)
	{
	FAILPRINT ("fail 12\n");
	return FALSE;
	}

    for (it=fg->nodes[gen].My_nodes; it!=NULL; it=it->link)
	{
	nd = it->val;

	switch (fg->nodes[nd].nodetype)
	    {
	    case ND_WINDOW_GEN :
		/* reject if any window sizes are not known */
		for (i=0, j=1; i<fg->nodes[nd].inputs[0].ty.dims; i++, j+=2)
		    if (!fg->nodes[nd].inputs[j].is_const)
			{
			FAILPRINT ("fail 13\n");
			return FALSE;
			}

		if (fg->nodes[nd].inputs[0].ty.dims > 2)
		    {
		    FAILPRINT ("fail 14\n");
		    return FALSE;
		    }

		break;
	    case ND_ELE_GEN :
	      if (fg->nodes[nd].inputs[0].ty.dims > 3)
		{
		  FAILPRINT ("fail 17\n");
		  return FALSE;
		}
	      
	      break;
	    case ND_SLICE_GEN :
	      if (fg->nodes[nd].inputs[0].ty.dims != 2)
		{
		  FAILPRINT ("fail 15\n");
		  return FALSE;
		}

	      /* see if the ':' dimension's size is known */
	      for (i=0; i<fg->nodes[nd].inputs[0].ty.dims; i++)
		{
		  if ((fg->nodes[nd].Reftypes[i]==':') && 
		      (fg->nodes[nd].inputs[0].ty.dim_sizes[i]<0))
		    {
		      FAILPRINT ("fail 16\n");
		      return FALSE;
		    }
		}
	      
	      break;
	    case ND_SCALAR_GEN :
		if (fg->nodes[nd].num_outputs > 2)
		    {
		    FAILPRINT ("fail 18\n");
		    return FALSE;
		    }

	        break;
	    case ND_LOOP_INDICES :
		FAILPRINT ("fail 19\n");
		return FALSE;
	    default :
	        break;
	    }
	}

    return TRUE;
    }

void set_graphs_input_node_type_to_uint (FuncGraph *fg, int id, int port, int width)
    {
    int gin;

    gin = fg->nodes[id].My_inputs[port];
    set_type_to_uint (&(fg->nodes[gin].outputs[0].ty), width);
    }

int is_unsigned (Type ty)
    {
    switch (ty)
        {
	case Int :
	case Fix :
	case Float :
	case Double :
	case CxInt :
	case CxFix :
	case CxFloat :
	case CxDouble :
	    return FALSE;
	default :
	    return TRUE;
	}
    }

void convert_input_for_dfg (FuncGraph *fg, int nd, int pt)
    {
    char *str, tmp[128];
    unsigned int uval;
    int ival;

    if (fg->nodes[nd].inputs[pt].is_const)
        {
	str = fg->nodes[nd].inputs[pt].constval;

	if (strcmp (str, "true") == 0)
	    fg->nodes[nd].inputs[pt].constval = copy_to_const ("0x1");
	else if (strcmp (str, "false") == 0)
	    fg->nodes[nd].inputs[pt].constval = copy_to_const ("0x0");
	else if ((strlen (str) < 2) || (str[0] != '0') || (str[1] != 'x'))
	    {
	    if (is_unsigned (fg->nodes[nd].inputs[pt].ty.type))
		{
	        sscanf (str, "%u", &uval);
		sprintf (tmp, "0x%x", uval);
		fg->nodes[nd].inputs[pt].constval = copy_to_const (tmp);
		}
	    else
	        {
		sscanf (str, "%d", &ival);
		uval = (unsigned int)ival & mask_ones[fg->nodes[nd].inputs[pt].ty.totsize];
		sprintf (tmp, "0x%x", uval);
		fg->nodes[nd].inputs[pt].constval = copy_to_const (tmp);
		}
	    }
	}
	
    fg->nodes[nd].inputs[pt].ty.type = Bits;
    }

/*  a special routine to follow a source up through two graph boundaries
 *  to its ND_G_INPUT source; used for example to take a generator input
 *  and trace it up out of the generator graph, up out of the loop graph,
 *  and to the ND_G_INPUT node of the ND_RC_COMPUTE graph;
 */
int follow_to_input_source (FuncGraph *fg, int nd, int pt)
    {
    int snd, parent, io_num;

    if (fg->nodes[nd].inputs[pt].is_const)
        return -1;

    assert (fg->nodes[nd].inputs[pt].back_edges != NULL);
    snd = fg->nodes[nd].inputs[pt].back_edges->node;
    assert (fg->nodes[snd].nodetype == ND_G_INPUT);
    io_num = fg->nodes[snd].Io_num;
    parent = fg->nodes[snd].My_graph;

    snd = fg->nodes[parent].inputs[io_num].back_edges->node;
    assert (fg->nodes[parent].inputs[io_num].back_edges != NULL);
    assert (fg->nodes[snd].nodetype == ND_G_INPUT);
    io_num = fg->nodes[snd].Io_num;
    parent = fg->nodes[snd].My_graph;

    snd = fg->nodes[parent].inputs[io_num].back_edges->node;
    assert (fg->nodes[parent].inputs[io_num].back_edges != NULL);
    assert (fg->nodes[snd].nodetype == ND_G_INPUT);
    return snd;
    }

int last_in_list (int gin_lo, int gin_hi, int gin_step, IntList *its)
    {
    IntList *it;
    int last_one = -1;

    for (it=its; it!=NULL; it=it->link)
        if (it->val == gin_lo)
	    last_one = gin_lo;
	else if (it->val == gin_hi)
	    last_one = gin_hi;
	else if (it->val == gin_step)
	    last_one = gin_step;
    
    return last_one;
    }

int create_malloc_tgt_node (FuncGraph *fg, int id, int sz, int loop_rank)
    {
    int mall_node;

    if (sz == 1)
        switch (loop_rank)
            {
            case 1 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_1_BIT_1D, 1, 1, fg->nodes[id].loc);
                break;
            case 2 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_1_BIT_2D, 2, 1, fg->nodes[id].loc);
                break;
            case 3 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_1_BIT_3D, 3, 1, fg->nodes[id].loc);
                break;
            default :
                assert (0);
            }
    else if (sz == 2)
        switch (loop_rank)
            {
            case 1 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_2_BIT_1D, 1, 1, fg->nodes[id].loc);
                break;
            case 2 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_2_BIT_2D, 2, 1, fg->nodes[id].loc);
                break;
            case 3 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_2_BIT_3D, 3, 1, fg->nodes[id].loc);
                break;
            default :
                assert (0);
            }
    else if (sz <= 4)
        switch (loop_rank)
            {
            case 1 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_4_BIT_1D, 1, 1, fg->nodes[id].loc);
                break;
            case 2 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_4_BIT_2D, 2, 1, fg->nodes[id].loc);
                break;
            case 3 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_4_BIT_3D, 3, 1, fg->nodes[id].loc);
                break;
            default :
                assert (0);
            }
    else if (sz <= 8)
        switch (loop_rank)
            {
            case 1 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_8_BIT_1D, 1, 1, fg->nodes[id].loc);
                break;
            case 2 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_8_BIT_2D, 2, 1, fg->nodes[id].loc);
                break;
            case 3 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_8_BIT_3D, 3, 1, fg->nodes[id].loc);
                break;
            default :
                assert (0);
            }
    else if (sz <= 16)
        switch (loop_rank)
            {
            case 1 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_16_BIT_1D, 1, 1, fg->nodes[id].loc);
                break;
            case 2 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_16_BIT_2D, 2, 1, fg->nodes[id].loc);
                break;
            case 3 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_16_BIT_3D, 3, 1, fg->nodes[id].loc);
                break;
            default :
                assert (0);
            }
    else
        switch (loop_rank)
            {
            case 1 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_32_BIT_1D, 1, 1, fg->nodes[id].loc);
                break;
            case 2 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_32_BIT_2D, 2, 1, fg->nodes[id].loc);
                break;
            case 3 :
                mall_node = new_ddcf_node (fg, ND_MALLOC_TGT_32_BIT_3D, 3, 1, fg->nodes[id].loc);
                break;
            default :
                assert (0);
            }

    return mall_node;
    }

void convert_lshift_to_mach_lshift (FuncGraph *fg, int id, IntList **its)
    {
    int snd, dist, ssnd, sspt;

    if (!fg->nodes[id].inputs[1].is_const)
        return;

    assert (!fg->nodes[id].inputs[0].is_const);

    snd = fg->nodes[id].inputs[0].back_edges->node;

    if (fg->nodes[snd].nodetype != ND_CAST)
        return;

    assert (!fg->nodes[snd].inputs[0].is_const);

    sscanf (fg->nodes[id].inputs[1].constval, "%x", &dist);

    if (fg->nodes[snd].outputs[0].ty.totsize - fg->nodes[snd].inputs[0].ty.totsize >= dist)
        {
	ssnd = fg->nodes[snd].inputs[0].back_edges->node;
	sspt = fg->nodes[snd].inputs[0].back_edges->port;
	remove_edge (fg, snd, 0, id, 0);
	add_edge (fg, ssnd, sspt, id, 0);
	copy_type_info (&(fg->nodes[id].inputs[0].ty), &(fg->nodes[ssnd].outputs[sspt].ty));
	if (fg->nodes[snd].outputs[0].targets == NULL)
	    {
	    remove_edge (fg, ssnd, sspt, snd, 0);
	    free_ddcf_node (fg, snd, its);
	    }
	}
    }
    
void convert_sqrt (FuncGraph *fg, int id, IntList **its)
    {
    int hook_nd, hook_pt, tsz, fsz, otsz, ofsz, cur_sz;
    int shift_node, cast_node, shift_dist, bwidth, cnode;
    Edge *tgts;
    Type ty;
    char str[256];

    assert (!fg->nodes[id].inputs[0].is_const);

    /*   we assume that this Sqrt node is followed by a CAST that transforms
     *   its type 'double' output
     */
    assert (fg->nodes[id].outputs[0].targets != NULL);
    cnode = fg->nodes[id].outputs[0].targets->node;
    assert (fg->nodes[cnode].nodetype == ND_CAST);

    tsz = cur_sz = fg->nodes[id].inputs[0].ty.totsize;
    fsz = fg->nodes[id].inputs[0].ty.fracsize;
    otsz = fg->nodes[cnode].outputs[0].ty.totsize;
    ofsz = fg->nodes[cnode].outputs[0].ty.fracsize;
    ty = fg->nodes[id].inputs[0].ty.type;
    hook_nd = fg->nodes[id].inputs[0].back_edges->node;
    hook_pt = fg->nodes[id].inputs[0].back_edges->port;

    /* first cast it to Bits type */
    cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[id].loc);
    remove_edge (fg, hook_nd, hook_pt, id, 0);
    add_edge (fg, hook_nd, hook_pt, cast_node, 0);
    add_edge (fg, cast_node, 0, id, 0);
    copy_type_info (&(fg->nodes[cast_node].inputs[0].ty), &(fg->nodes[hook_nd].outputs[hook_pt].ty));
    fg->nodes[id].inputs[0].ty.type = Bits;
    fg->nodes[id].inputs[0].ty.fracsize = 0;
    copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), &(fg->nodes[id].inputs[0].ty));
    insert_val_after_in_intlist (cast_node, hook_nd, its);

    hook_nd = cast_node;
    hook_pt = 0;

    /* left-shift to set up for the desired output fracsize */
    shift_dist = 2 * ofsz - fsz;
    if (shift_dist < 0)
        {
        shift_node = new_ddcf_node (fg, ND_RIGHT_SHIFT, 2, 1, fg->nodes[id].loc);
        remove_edge (fg, hook_nd, hook_pt, id, 0);
        add_edge (fg, hook_nd, hook_pt, shift_node, 0);
        add_edge (fg, shift_node, 0, id, 0);
        copy_type_info (&(fg->nodes[shift_node].inputs[0].ty), &(fg->nodes[hook_nd].outputs[hook_pt].ty));
        fg->nodes[id].inputs[0].ty.totsize += shift_dist;
        copy_type_info (&(fg->nodes[shift_node].outputs[0].ty), &(fg->nodes[id].inputs[0].ty));
        insert_val_after_in_intlist (shift_node, hook_nd, its);
        fg->nodes[shift_node].inputs[1].is_const = TRUE;
        bwidth = bit_width (-shift_dist);
        set_type_to_uint (&(fg->nodes[shift_node].inputs[1].ty), bwidth);
        sprintf (str, "0x%x", -shift_dist);
        fg->nodes[shift_node].inputs[1].constval = copy_to_const (str);

        hook_nd = shift_node;
        hook_pt = 0;
        cur_sz += shift_dist;
	}
    else if (shift_dist > 0)
        {
        shift_node = new_ddcf_node (fg, ND_LEFT_SHIFT_MACH, 2, 1, fg->nodes[id].loc);
        remove_edge (fg, hook_nd, hook_pt, id, 0);
        add_edge (fg, hook_nd, hook_pt, shift_node, 0);
        add_edge (fg, shift_node, 0, id, 0);
        copy_type_info (&(fg->nodes[shift_node].inputs[0].ty), &(fg->nodes[hook_nd].outputs[hook_pt].ty));
        fg->nodes[id].inputs[0].ty.totsize += shift_dist;
        copy_type_info (&(fg->nodes[shift_node].outputs[0].ty), &(fg->nodes[id].inputs[0].ty));
        insert_val_after_in_intlist (shift_node, hook_nd, its);
        fg->nodes[shift_node].inputs[1].is_const = TRUE;
        bwidth = bit_width (shift_dist);
        set_type_to_uint (&(fg->nodes[shift_node].inputs[1].ty), bwidth);
        sprintf (str, "0x%x", shift_dist);
        fg->nodes[shift_node].inputs[1].constval = copy_to_const (str);

        hook_nd = shift_node;
        hook_pt = 0;
        cur_sz += shift_dist;
	}

    /* if the total width is now odd, add a bit on the left */
    if (cur_sz % 2 == 1)
        {
	cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[id].loc);
	remove_edge (fg, hook_nd, hook_pt, id, 0);
	add_edge (fg, hook_nd, hook_pt, cast_node, 0);
	add_edge (fg, cast_node, 0, id, 0);
	copy_type_info (&(fg->nodes[cast_node].inputs[0].ty), &(fg->nodes[hook_nd].outputs[hook_pt].ty));
	fg->nodes[id].inputs[0].ty.totsize++;
	copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), &(fg->nodes[id].inputs[0].ty));
	insert_val_after_in_intlist (cast_node, hook_nd, its);

	hook_nd = cast_node;
	hook_pt = 0;
	cur_sz++;
	}

    /* convert the sqrt node to the -MACH version */
    fg->nodes[id].nodetype = ND_SQRT_MACH;
    copy_type_info (&(fg->nodes[id].outputs[0].ty), &(fg->nodes[id].inputs[0].ty));
    fg->nodes[id].outputs[0].ty.totsize = cur_sz/2;
    copy_type_info (&(fg->nodes[cnode].inputs[0].ty), &(fg->nodes[id].outputs[0].ty));
    }

void convert_mult_to_mach_mult (FuncGraph *fg, int id, IntList **its)
    {
    int snd, spt;

    if (is_unsigned (fg->nodes[id].inputs[0].ty.type) && !is_unsigned (fg->nodes[id].inputs[1].ty.type))
	{
        cast_an_input_to_signed (fg, id, 0, its, fg->nodes[id].loc);
	}
    else if (is_unsigned (fg->nodes[id].inputs[1].ty.type) && !is_unsigned (fg->nodes[id].inputs[0].ty.type))
	{
        cast_an_input_to_signed (fg, id, 1, its, fg->nodes[id].loc);
	}

    /* try to eliminate a CAST on left input */
    try_to_elim_a_cast_above (fg, id, 0, its, fg->nodes[id].loc);

    /* if node did not get converted, try to eliminate a CAST on right input */
    if (fg->nodes[id].nodetype != ND_MUL_MACH)
        try_to_elim_a_cast_above (fg, id, 1, its, fg->nodes[id].loc);

    /* if node did not get converted, convert it and insert the necessary CAST below */
    if (fg->nodes[id].nodetype != ND_MUL_MACH)
	{
        insert_a_cast_below (fg, id, its, fg->nodes[id].loc);
	fg->nodes[id].nodetype = ND_MUL_MACH;
	}
    }
    
void try_to_elim_a_cast_above (FuncGraph *fg, int id, int pt, IntList **its, Location loc)
    {
    int snd, spt, ssnd, sspt;

    if (fg->nodes[id].inputs[pt].is_const)
        return;

    snd = fg->nodes[id].inputs[pt].back_edges->node;
    spt = fg->nodes[id].inputs[pt].back_edges->port;
    if (fg->nodes[snd].nodetype != ND_CAST)
        return;

    if (is_unsigned (fg->nodes[snd].inputs[0].ty.type) && !is_unsigned (fg->nodes[snd].outputs[0].ty.type))
        {
	if ((fg->nodes[snd].inputs[0].ty.fracsize != fg->nodes[snd].outputs[0].ty.fracsize) ||
		(fg->nodes[snd].inputs[0].ty.totsize+1 != fg->nodes[snd].outputs[0].ty.totsize))
	    {
	    cast_an_input_to_signed (fg, snd, 0, its, loc);
	    }
	}
    else if (!is_unsigned (fg->nodes[snd].inputs[0].ty.type) && is_unsigned (fg->nodes[snd].outputs[0].ty.type))
        {
	if ((fg->nodes[snd].inputs[0].ty.fracsize != fg->nodes[snd].outputs[0].ty.fracsize) ||
		(fg->nodes[snd].inputs[0].ty.totsize-1 != fg->nodes[snd].outputs[0].ty.totsize))
	    {
	    cast_an_input_to_unsigned (fg, snd, 0, its, loc);
	    }
	}

    if (no_lost_bits (fg, snd))
        {
	remove_edge (fg, snd, 0, id, pt);
	if (fg->nodes[snd].inputs[0].is_const)
	    fg->nodes[id].inputs[pt] = fg->nodes[snd].inputs[0];
	else
	    {
	    ssnd = fg->nodes[snd].inputs[0].back_edges->node;
	    sspt = fg->nodes[snd].inputs[0].back_edges->port;
	    add_edge (fg, ssnd, sspt, id, pt);
	    copy_type_info (&(fg->nodes[id].inputs[pt].ty), &(fg->nodes[ssnd].outputs[sspt].ty));

	    if (fg->nodes[snd].outputs[0].targets == NULL)
	        {
		remove_edge (fg, ssnd, sspt, snd, 0);
		free_ddcf_node (fg, snd, its);
		}
	    }
	insert_a_cast_below (fg, id, its, loc);
	fg->nodes[id].nodetype = ND_MUL_MACH;
	}
    }

void cast_an_input_to_unsigned (FuncGraph *fg, int id, int pt, IntList **its, Location loc)
    {
    int cast_node, snd, spt;

    if (fg->nodes[id].inputs[pt].is_const)
        {
	fg->nodes[id].inputs[pt].ty.totsize--;
        if (fg->nodes[id].inputs[pt].ty.type == Int)
	    fg->nodes[id].inputs[pt].ty.type = Uint;
        else if (fg->nodes[id].inputs[pt].ty.type == Fix)
	    fg->nodes[id].inputs[pt].ty.type = Ufix;
        else
	    assert (0);
	}
    else
        {
        cast_node =  new_ddcf_node (fg, ND_CAST, 1, 1, loc);
        snd = fg->nodes[id].inputs[pt].back_edges->node;
        spt = fg->nodes[id].inputs[pt].back_edges->port;
        remove_edge (fg, snd, spt, id, pt);
        add_edge (fg, snd, spt, cast_node, 0);
        add_edge (fg, cast_node, 0, id, pt);
        copy_type_info (&(fg->nodes[cast_node].inputs[pt].ty), &(fg->nodes[snd].outputs[spt].ty));
	fg->nodes[id].inputs[pt].ty.totsize--;
        if (fg->nodes[id].inputs[pt].ty.type == Int)
	    fg->nodes[id].inputs[pt].ty.type = Uint;
        else if (fg->nodes[id].inputs[pt].ty.type == Fix)
	    fg->nodes[id].inputs[pt].ty.type = Ufix;
        else
	    assert (0);
        copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), &(fg->nodes[id].inputs[pt].ty));
        insert_val_after_in_intlist (cast_node, snd, its);
	}
    }

void cast_an_input_to_signed (FuncGraph *fg, int id, int pt, IntList **its, Location loc)
    {
    int cast_node, snd, spt;

    if (fg->nodes[id].inputs[pt].is_const)
        {
        if (fg->nodes[id].inputs[pt].ty.totsize < 32)
	    fg->nodes[id].inputs[pt].ty.totsize++;
        if (fg->nodes[id].inputs[pt].ty.type == Uint)
	    fg->nodes[id].inputs[pt].ty.type = Int;
        else if (fg->nodes[id].inputs[pt].ty.type == Ufix)
	    fg->nodes[id].inputs[pt].ty.type = Fix;
        else
	    assert (0);
	}
    else
        {
        cast_node =  new_ddcf_node (fg, ND_CAST, 1, 1, loc);
        snd = fg->nodes[id].inputs[pt].back_edges->node;
        spt = fg->nodes[id].inputs[pt].back_edges->port;
        remove_edge (fg, snd, spt, id, pt);
        add_edge (fg, snd, spt, cast_node, 0);
        add_edge (fg, cast_node, 0, id, pt);
        copy_type_info (&(fg->nodes[cast_node].inputs[0].ty), &(fg->nodes[snd].outputs[spt].ty));
        if (fg->nodes[id].inputs[pt].ty.totsize < 32)
	    fg->nodes[id].inputs[pt].ty.totsize++;
        if (fg->nodes[id].inputs[pt].ty.type == Uint)
	    fg->nodes[id].inputs[pt].ty.type = Int;
        else if (fg->nodes[id].inputs[pt].ty.type == Ufix)
	    fg->nodes[id].inputs[pt].ty.type = Fix;
        else
	    assert (0);
        copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), &(fg->nodes[id].inputs[pt].ty));
        insert_val_after_in_intlist (cast_node, snd, its);
	}
    }

void insert_a_cast_below (FuncGraph *fg, int id, IntList **its, Location loc)
    {
    int tnd, tpt, cast_node;

    if (fg->nodes[id].outputs[0].targets == NULL)
        return;

    cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, loc);
    fg->nodes[cast_node].outputs[0].targets = fg->nodes[id].outputs[0].targets;
    fg->nodes[id].outputs[0].targets = NULL;
    retarget_backedges (fg, fg->nodes[cast_node].outputs[0].targets, id, 0, cast_node, 0);
    copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), &(fg->nodes[id].outputs[0].ty));
    add_edge (fg, id, 0, cast_node, 0);
    fg->nodes[id].outputs[0].ty.totsize =
    	fg->nodes[id].inputs[0].ty.totsize + fg->nodes[id].inputs[1].ty.totsize;
    fg->nodes[id].outputs[0].ty.fracsize =
    	fg->nodes[id].inputs[0].ty.fracsize + fg->nodes[id].inputs[1].ty.fracsize;
    copy_type_info (&(fg->nodes[cast_node].inputs[0].ty), &(fg->nodes[id].outputs[0].ty));
    insert_val_after_in_intlist (cast_node, id, its);
    }

int no_lost_bits (FuncGraph *fg, int id)
    {
    int tsz_in, tsz_out, fsz_in, fsz_out;

    assert (fg->nodes[id].nodetype == ND_CAST);

    if (is_unsigned (fg->nodes[id].inputs[0].ty.type) && !is_unsigned (fg->nodes[id].outputs[0].ty.type))
        return FALSE;

    tsz_in = fg->nodes[id].inputs[0].ty.totsize;
    tsz_out = fg->nodes[id].outputs[0].ty.totsize;
    fsz_in = fg->nodes[id].inputs[0].ty.fracsize;
    fsz_out = fg->nodes[id].outputs[0].ty.fracsize;

    if (fsz_in > fsz_out)
        return FALSE;

    if ((tsz_out-tsz_in) < (fsz_out-fsz_in))
        return FALSE;

    return TRUE;
    }

int does_mult_fit_in_32_bits (FuncGraph *fg, int id)
    {
    int mixed, snd, in_width0, in_width1;

    assert (fg->nodes[id].nodetype == ND_MUL);

    if (is_unsigned (fg->nodes[id].outputs[0].ty.type))
        mixed = FALSE;
    else if (is_unsigned (fg->nodes[id].inputs[0].ty.type) || is_unsigned (fg->nodes[id].inputs[1].ty.type))
        mixed = TRUE;
    else
        mixed = FALSE;

    if (has_removeable_cast (fg, id, 0, mixed))
        {
	snd = fg->nodes[id].inputs[0].back_edges->node;
	assert (fg->nodes[snd].nodetype == ND_CAST);
	in_width0 = fg->nodes[snd].inputs[0].ty.totsize;
	in_width1 = fg->nodes[id].inputs[1].ty.totsize;
	}
    else if (has_removeable_cast (fg, id, 1, mixed))
        {
	snd = fg->nodes[id].inputs[1].back_edges->node;
	assert (fg->nodes[snd].nodetype == ND_CAST);
	in_width0 = fg->nodes[id].inputs[0].ty.totsize;
	in_width1 = fg->nodes[snd].inputs[0].ty.totsize;
	}
    else
        {
	in_width0 = fg->nodes[id].inputs[0].ty.totsize;
	in_width1 = fg->nodes[id].inputs[1].ty.totsize;
	}

    return ((in_width0+in_width1) <= 32);
    }

int has_removeable_cast (FuncGraph *fg, int id, int pt, int mixed)
    {
    int snd;

    if (fg->nodes[id].inputs[pt].is_const)
        return FALSE;

    snd = fg->nodes[id].inputs[pt].back_edges->node;
    if (fg->nodes[snd].nodetype != ND_CAST)
        return FALSE;

    if (mixed && is_unsigned (fg->nodes[id].inputs[pt].ty.type))
        return FALSE;

    return no_lost_bits (fg, snd);
    }
