#include "sassy.h"
#include "ddcf.h"
#include "error.h"

void size_propagate_node_down (FuncGraph*, int);
void transfer_through_inputs_down (FuncGraph*, int);
void transfer_through_outputs_down (FuncGraph*, int);
void size_propagate_elegen_down (FuncGraph*, int);
void size_propagate_slicegen_down (FuncGraph*, int);
void size_propagate_windowgen_down (FuncGraph*, int);
void size_propagate_forall_down (FuncGraph*, int);
int get_loop_dimensions (FuncGraph*, int, int*);
void process_loop_returns_down (FuncGraph*, int, int, int*);
void propagate_downward (FuncGraph*, int, int);
void size_transfer (TypeInfo*, TypeInfo*);
void size_write (int*, int, int);
int get_int_const (char*);

void size_propagate_down ()
    {
    FuncGraph *fg;
    IntList *it;
    TypeInfo *tt;
    int i;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
	if (fg->nodes_used > 0)
	    {
	    for (it=fg->nodes[0].My_nodes; it!=NULL; it=it->link)
	        size_propagate_node_down (fg, it->val);
	    transfer_through_outputs_down (fg, 0);

	    for (tt=fg->rets, i=0; tt!=NULL; tt=tt->link, i++)
                size_transfer (tt, &(fg->nodes[0].outputs[i].ty));
	    }
    }

void size_propagate_node_down (FuncGraph *fg, int id)
    {
    IntList *it;
    int i, j, k;

    /*
    printf ("node type %s\n", ddcf_node_str (fg->nodes[id].nodetype)); fflush (stdout);
    */

    switch (fg->nodes[id].nodetype)
        {
	case ND_CASE :
	    transfer_through_inputs_down (fg, id);

	    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
	        size_propagate_node_down (fg, it->val);

	    transfer_through_outputs_down (fg, id);

	    /*  here we *don't* propagate to the targets because there
	     *  can be multiple edges feeding those targets; we could do
	     *  a special check to see if all CASE outputs have same sizes...
	     */
	    break;
	case ND_FORALL :
	    size_propagate_forall_down (fg, id);
	    break;
	case ND_FORNXT :
	case ND_FUNC :
	case ND_CROSS_PROD :
	case ND_DOT_PROD :
	case ND_WHILE_PRED :
	case ND_SWITCH :
	case ND_WHILE :
	    transfer_through_inputs_down (fg, id);

	    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
	        size_propagate_node_down (fg, it->val);

	    transfer_through_outputs_down (fg, id);

	    for (i=0; i<fg->nodes[id].num_outputs; i++)
		if (fg->nodes[id].outputs[i].ty.kind == Array)
	            propagate_downward (fg, id, i);
	    break;
	case ND_ADD :
	case ND_SUB :
	case ND_MUL :
	case ND_DIV :
	case ND_MOD :
	case ND_COMPLEX :
	case ND_LT :
	case ND_GT :
	case ND_LE :
	case ND_GE :
	case ND_NEQ :
	case ND_EQ :
	case ND_AND :
	case ND_OR :
	case ND_BIT_AND :
	case ND_BIT_OR :
	case ND_BIT_EOR :
	case ND_LEFT_SHIFT :
	case ND_LEFT_SHIFT_MACH :
	case ND_RIGHT_SHIFT :
	case ND_SCALAR_GEN :
	case ND_REAL :
	case ND_IMAG :
	case ND_NOT :
	case ND_NEG :
	case ND_INTRINCALL :
	case ND_SWITCH_KEY :
	case ND_SELECTORS :
	case ND_PRINT :
	case ND_ASSERT :
	case ND_LOOP_INDICES :
	case ND_REDUCE_SUM :
	case ND_REDUCE_MIN :
	case ND_REDUCE_MAX :
	case ND_REDUCE_AND :
	case ND_REDUCE_OR :
	case ND_REDUCE_PRODUCT :
	case ND_REDUCE_MEAN :
	case ND_REDUCE_ST_DEV :
	case ND_REDUCE_MEDIAN :
	case ND_EXTENTS :
	    /* these don't output arrays, so nothing to do here */
	    break;
	case ND_ELE_GEN :
	  break;
	case ND_SLICE_GEN :
	    if (fg->nodes[id].outputs[0].ty.kind == Array)
	        size_propagate_slicegen_down (fg, id);
	    break;
	case ND_WINDOW_GEN :
	    if (fg->nodes[id].outputs[0].ty.kind == Array)
	        size_propagate_windowgen_down (fg, id);
	    break;
	case ND_REDUCE_VAL_AT_FIRST_MAX :
	case ND_REDUCE_VAL_AT_FIRST_MIN :
	case ND_REDUCE_VAL_AT_LAST_MAX :
	case ND_REDUCE_VAL_AT_LAST_MIN :
	case ND_REDUCE_VAL_AT_MAXS :
	case ND_REDUCE_VAL_AT_MINS :
	case ND_REDUCE_MODE :
	case ND_REDUCE_HIST :
	case ND_CONSTRUCT_ARRAY :
	case ND_CONSTRUCT_CONCAT :
	case ND_CONSTRUCT_CONCAT_MASKED :
	case ND_CONSTRUCT_TILE :
	case ND_ACCUM_SUM :
	case ND_ACCUM_MIN :
	case ND_ACCUM_MAX :
	case ND_ACCUM_AND :
	case ND_ACCUM_OR :
	case ND_ACCUM_PRODUCT :
	case ND_ACCUM_MEAN :
	case ND_ACCUM_ST_DEV :
	case ND_ACCUM_MEDIAN :
	case ND_ACCUM_HIST :
	    break;
	case ND_G_INPUT_NEXT :
	case ND_G_INPUT :
	    if (fg->nodes[id].outputs[0].ty.kind == Array)
	        propagate_downward (fg, id, 0);
	    break;
	case ND_G_OUTPUT :
	    break;
	case ND_ARR_DEF :
	    {
	    int rank;
	    rank = fg->nodes[id].DefRank;
	    for (i=0; i<rank; i++)
	        fg->nodes[id].outputs[0].ty.dim_sizes[i] = fg->nodes[id].DefDims[i];
	    propagate_downward (fg, id, 0);
	    }
	    break;
	case ND_CAST :
	case ND_FEED_NEXT :
	    size_transfer (&(fg->nodes[id].outputs[0].ty),
	    			&(fg->nodes[id].inputs[0].ty));
	    propagate_downward (fg, id, 0);
	    break;
	case ND_ARR_CONCAT :
	    /* we merge, and check for clashes, all but the rightmost dimension */
	    for (i=0; i<fg->nodes[id].outputs[0].ty.dims-1; i++)
	        if (fg->nodes[id].inputs[0].ty.dim_sizes[i] >= 0)
		    {
		    if (fg->nodes[id].inputs[1].ty.dim_sizes[i] >= 0)
		        {
			if (fg->nodes[id].inputs[0].ty.dim_sizes[i] !=
					fg->nodes[id].inputs[1].ty.dim_sizes[i])
	        	    handle_error (ErrSizePropConcatClash,
			    			Fatal, 0, NULL, NULL, i);
			else
			    size_write (fg->nodes[id].outputs[0].ty.dim_sizes,
			    		fg->nodes[id].inputs[0].ty.dim_sizes[i], i);
			}
		    else
			size_write (fg->nodes[id].outputs[0].ty.dim_sizes,
					fg->nodes[id].inputs[0].ty.dim_sizes[i], i);
		    }
		else if (fg->nodes[id].inputs[1].ty.dim_sizes[i] >= 0)
		    size_write (fg->nodes[id].outputs[0].ty.dim_sizes,
		    			fg->nodes[id].inputs[1].ty.dim_sizes[i], i);

	    /* the rightmost dimension is a constant if both sources have it constant */
	    if ((fg->nodes[id].inputs[0].ty.dim_sizes[i] >= 0) &&
	    		(fg->nodes[id].inputs[1].ty.dim_sizes[i] >= 0))
		size_write (fg->nodes[id].outputs[0].ty.dim_sizes,
		    fg->nodes[id].inputs[0].ty.dim_sizes[i] +
		    fg->nodes[id].inputs[1].ty.dim_sizes[i], i);

	    propagate_downward (fg, id, 0);
	    break;
	case ND_FCALL :
	    for (i=0; i<fg->nodes[id].num_outputs; i++)
	        if (fg->nodes[id].outputs[i].ty.kind == Array)
		    propagate_downward (fg, id, i);
	    break;
	case ND_ARR_CONPERIM :
	    if (fg->nodes[id].inputs[1].is_const)
		{
		int val = get_int_const (fg->nodes[id].inputs[1].constval);

	        for (i=0; i<fg->nodes[id].inputs[0].ty.dims; i++)
		    if (fg->nodes[id].inputs[0].ty.dim_sizes[i] >= 0)
			size_write (fg->nodes[id].outputs[0].ty.dim_sizes,
				2*val + fg->nodes[id].inputs[0].ty.dim_sizes[i], i);
		}

	    propagate_downward (fg, id, 0);
	    break;
	case ND_ARRAYREF :
	    {
	    int lo, hi, step, sz;

	    if (fg->nodes[id].outputs[0].ty.kind == Array)
	        {
	        for (i=0, j=1, k=0; i<fg->nodes[id].inputs[0].ty.dims; i++)
	            if (fg->nodes[id].Reftypes[i] == ':')
		        {
		        if (fg->nodes[id].inputs[j].is_const &&
		    	    	fg->nodes[id].inputs[j+1].is_const &&
		    	    	fg->nodes[id].inputs[j+2].is_const)
			    {
			    lo = get_int_const (fg->nodes[id].inputs[j].constval);
			    hi = get_int_const (fg->nodes[id].inputs[j+1].constval);
			    step = get_int_const (fg->nodes[id].inputs[j+2].constval);
			    sz = hi-lo<0?0:1+(hi-lo)/step;
			    size_write (fg->nodes[id].outputs[0].ty.dim_sizes, sz, k);
			    }
		        j += 3;
		        k++;
		        }
		    else
		        j++;

	        propagate_downward (fg, id, 0);
		}
            }
	    break;
	case ND_NEXT :
	    {
	    int in_node = fg->nodes[id].In_next_id;
	    int g_node = fg->nodes[in_node].My_graph;
	    int g_port = fg->nodes[in_node].Io_num;

	    if (fg->nodes[id].inputs[0].ty.kind == Array)
		for (i=0; i<fg->nodes[id].inputs[0].ty.dims; i++)
		    if ((fg->nodes[id].inputs[0].ty.dim_sizes[i] ==
				fg->nodes[g_node].inputs[g_port].ty.dim_sizes[i]) &&
				(fg->nodes[id].inputs[0].ty.dim_sizes[i] >= 0))
			size_write (fg->nodes[in_node].outputs[0].ty.dim_sizes,
					fg->nodes[id].inputs[0].ty.dim_sizes[i], i);
	    }
	    break;
	default :
            break;
	}
    }

void transfer_through_inputs_down (FuncGraph *fg, int id)
    {
    int i, in_node;

    for (i=0; i<fg->nodes[id].num_inputs; i++)
	if (fg->nodes[id].inputs[i].ty.kind == Array)
	    {
	    in_node = fg->nodes[id].My_inputs[i];

	    if (fg->nodes[in_node].nodetype == ND_G_INPUT)
	        size_transfer (&(fg->nodes[in_node].outputs[0].ty),
				&(fg->nodes[id].inputs[i].ty));
	    }
    }

void transfer_through_outputs_down (FuncGraph *fg, int id)
    {
    int i, out_node;

    for (i=0; i<fg->nodes[id].num_outputs; i++)
        if (fg->nodes[id].outputs[i].ty.kind == Array)
	    {
	    out_node = fg->nodes[id].My_outputs[i];
	    size_transfer (&(fg->nodes[id].outputs[i].ty),
				&(fg->nodes[out_node].inputs[0].ty));
	    }
    }

void size_propagate_forall_down (FuncGraph *fg, int id)
    {
    IntList *it;
    int i, rank;

    transfer_through_inputs_down (fg, id);

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        size_propagate_node_down (fg, it->val);

    /* determine loop dimensions where possible */
    rank = get_loop_dimensions (fg, id, fg->nodes[id].dim_sizes);

    process_loop_returns_down (fg, id, rank, fg->nodes[id].dim_sizes);

    transfer_through_outputs_down (fg, id);

    for (i=0; i<fg->nodes[id].num_outputs; i++)
	if (fg->nodes[id].outputs[i].ty.kind == Array)
            propagate_downward (fg, id, i);
    }

void process_loop_returns_down (FuncGraph *fg, int id, int loop_rank, int *loop_sizes)
    {
    IntList *it;
    int nd, i, j;

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
	{
	nd = it->val;

        switch (fg->nodes[nd].nodetype)
	    {
	    case ND_CONSTRUCT_ARRAY :
		for (i=0; i<loop_rank; i++)
		    if (loop_sizes[i] >= 0)
			size_write (fg->nodes[nd].outputs[0].ty.dim_sizes, loop_sizes[i], i);

		if (fg->nodes[nd].inputs[0].ty.kind == Array)
		    for (j=0; j<fg->nodes[nd].inputs[0].ty.dims; j++)
			{
		        if (fg->nodes[nd].inputs[0].ty.dim_sizes[j] >= 0)
			    size_write (fg->nodes[nd].outputs[0].ty.dim_sizes,
			    		    fg->nodes[nd].inputs[0].ty.dim_sizes[j], i);
			i++;
			}

	        propagate_downward (fg, nd, 0);
	        break;
	    case ND_CONSTRUCT_CONCAT :
		/* transfer all array dimensions except the last */
		for (i=0; i<fg->nodes[nd].inputs[0].ty.dims-1; i++)
		    if (fg->nodes[nd].inputs[0].ty.dim_sizes[i] >= 0)
			size_write (fg->nodes[nd].outputs[0].ty.dim_sizes,
					    fg->nodes[nd].inputs[0].ty.dim_sizes[i], i);

		/* do the last if we can */
		if ((loop_sizes[0] >= 0) && (fg->nodes[nd].inputs[0].ty.dim_sizes[i] >= 0))
		    size_write (fg->nodes[nd].outputs[0].ty.dim_sizes,
		    		loop_sizes[0] * fg->nodes[nd].inputs[0].ty.dim_sizes[i], i);

	        propagate_downward (fg, nd, 0);
	        break;
	    case ND_CONSTRUCT_TILE :
		{
		int component_rank = fg->nodes[nd].inputs[0].ty.dims;

		if (component_rank > loop_rank)
		    {
		    for (i=0; i<component_rank-loop_rank; i++)
			if (fg->nodes[nd].inputs[0].ty.dim_sizes[i] >= 0)
			    size_write (fg->nodes[nd].outputs[0].ty.dim_sizes,
			    		fg->nodes[nd].inputs[0].ty.dim_sizes[i], i);

		    for (j=0; j<loop_rank; j++, i++)
		        if ((fg->nodes[nd].inputs[0].ty.dim_sizes[i] >= 0) &&
							(loop_sizes[j] >= 0))
			    size_write (fg->nodes[nd].outputs[0].ty.dim_sizes,
			    		fg->nodes[nd].inputs[0].ty.dim_sizes[i] * loop_sizes[j], i);
		    }
		else
		    {
		    for (i=0; i<loop_rank-component_rank; i++)
			if (loop_sizes[i] >= 0)
			    size_write (fg->nodes[nd].outputs[0].ty.dim_sizes, loop_sizes[i], i);

		    for (j=0; j<fg->nodes[nd].inputs[0].ty.dims; j++, i++)
		        if ((fg->nodes[nd].inputs[0].ty.dim_sizes[j] >= 0) &&
							(loop_sizes[i] >= 0))
			    size_write (fg->nodes[nd].outputs[0].ty.dim_sizes,
			    	    fg->nodes[nd].inputs[0].ty.dim_sizes[j] * loop_sizes[i], i);
		    }

	        propagate_downward (fg, nd, 0);
	        break;
		}
	    case ND_REDUCE_VAL_AT_FIRST_MAX :
	    case ND_REDUCE_VAL_AT_FIRST_MIN :
	    case ND_REDUCE_VAL_AT_LAST_MAX :
	    case ND_REDUCE_VAL_AT_LAST_MIN :
	    case ND_REDUCE_VAL_AT_MAXS :
	    case ND_REDUCE_VAL_AT_MINS :
		size_write (fg->nodes[nd].outputs[0].ty.dim_sizes,
					fg->nodes[nd].num_inputs-2, 0);
	        propagate_downward (fg, nd, 0);
	        break;
	    case ND_REDUCE_HIST :
	    case ND_ACCUM_SUM :
	    case ND_ACCUM_PRODUCT :
	    case ND_ACCUM_MIN :
	    case ND_ACCUM_MAX :
	    case ND_ACCUM_AND :
	    case ND_ACCUM_OR :
	    case ND_ACCUM_MEAN :
	    case ND_ACCUM_ST_DEV :
	    case ND_ACCUM_MEDIAN :
		if (fg->nodes[nd].inputs[2].is_const)
		    {
		    int val = get_int_const (fg->nodes[nd].inputs[2].constval);
		    size_write (fg->nodes[nd].outputs[0].ty.dim_sizes, val, 0);
		    }
	        propagate_downward (fg, nd, 0);
	        break;
	    case ND_ACCUM_HIST :
		if (fg->nodes[nd].inputs[2].is_const)
		    {
		    int val = get_int_const (fg->nodes[nd].inputs[2].constval);
		    size_write (fg->nodes[nd].outputs[0].ty.dim_sizes, val, 0);
		    }
		if (fg->nodes[nd].inputs[3].is_const)
		    {
		    int val = get_int_const (fg->nodes[nd].inputs[3].constval);
		    size_write (fg->nodes[nd].outputs[0].ty.dim_sizes, val, 1);
		    }
	        propagate_downward (fg, nd, 0);
	        break;
	    default :
                break;
	    }
	}
    }

void size_propagate_slicegen_down (FuncGraph *fg, int id)
    {
    int i, j;

    for (i=0, j=0; i<fg->nodes[id].inputs[0].ty.dims; i++)
        if (fg->nodes[id].Reftypes[i] == ':')
	    {
	    if (fg->nodes[id].inputs[0].ty.dim_sizes[i] >= 0)
		size_write (fg->nodes[id].outputs[0].ty.dim_sizes,
				fg->nodes[id].inputs[0].ty.dim_sizes[i], j);
	    j++;
	    }

    propagate_downward (fg, id, 0);
    }

void size_propagate_windowgen_down (FuncGraph *fg, int id)
    {
    int i, j;

    for (i=0, j=1; i<fg->nodes[id].inputs[0].ty.dims; i++, j+=2)
	if (fg->nodes[id].inputs[j].is_const)
	    size_write (fg->nodes[id].outputs[0].ty.dim_sizes,
	    		get_int_const (fg->nodes[id].inputs[j].constval), i);

    propagate_downward (fg, id, 0);
    }

void propagate_downward (FuncGraph *fg, int id, int outport)
    {
    Edge *tg;
    int node, port;

    for (tg=fg->nodes[id].outputs[outport].targets; tg!=NULL; tg=tg->link)
	{
	node = tg->node;
	port = tg->port;
	size_transfer (&(fg->nodes[node].inputs[port].ty),
    				&(fg->nodes[id].outputs[outport].ty));
	}
    }
