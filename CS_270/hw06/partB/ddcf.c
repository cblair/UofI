#include "sassy.h"
#include "ddcf.h"
#include "error.h"

int incr_ele_gen_rank (FuncGraph*, int, int);
int incr_window_gen_rank (FuncGraph*, int, int);
int incr_scalar_gen_rank (FuncGraph*, int, int);

void add_edge (FuncGraph *fg, int src_nd, int src_pt, int tgt_nd, int tgt_pt)
    {
    Edge *tg;
    Edge *bk;

    SacMalloc (tg, Edge*, sizeof (Edge))
    tg->node = tgt_nd;
    tg->port = tgt_pt;
    tg->link = fg->nodes[src_nd].outputs[src_pt].targets;
    fg->nodes[src_nd].outputs[src_pt].targets = tg;

    SacMalloc (bk, Edge*, sizeof (Edge))
    bk->node = src_nd;
    bk->port = src_pt;
    bk->link = fg->nodes[tgt_nd].inputs[tgt_pt].back_edges;
    fg->nodes[tgt_nd].inputs[tgt_pt].back_edges = bk;
    }

void remove_edge (FuncGraph *fg, int src_nd, int src_pt, int tgt_nd, int tgt_pt)
    {
    Edge **ed, *e;
    int deleted;

    deleted = FALSE;
    for (ed=&(fg->nodes[src_nd].outputs[src_pt].targets); (*ed)!=NULL; ed=&((*ed)->link))
        if (((*ed)->node==tgt_nd) && ((*ed)->port==tgt_pt))
	    {
	    e = *ed;
	    *ed = e->link;
	    SacFree (e)
	    deleted = TRUE;
	    break;
	    }
    assert (deleted);

    deleted = FALSE;
    for (ed=&(fg->nodes[tgt_nd].inputs[tgt_pt].back_edges); (*ed)!=NULL; ed=&((*ed)->link))
        if (((*ed)->node==src_nd) && ((*ed)->port==src_pt))
	    {
	    e = *ed;
	    *ed = e->link;
	    SacFree (e)
	    deleted = TRUE;
	    break;
	    }
    assert (deleted);
    }

/*  move input 'src' to input 'tgt' of the graph 'id'; this involves:
 *	- changing the 'Io_num' of the ND_G_INPUT or ND_G_INPUT_NEXT node
 *	- moving the information in the 'inputs' array
 *	- following the back edges and altering the target's port to this new port
 */
void move_graph_input (FuncGraph *fg, int id, int src, int tgt)
    {
    int g_in;

    if (src == tgt)
        return;

    g_in = fg->nodes[id].My_inputs[src];
    fg->nodes[g_in].Io_num = tgt;
    fg->nodes[id].inputs[tgt] = fg->nodes[id].inputs[src];

    retarget (fg, fg->nodes[id].inputs[tgt].back_edges, id, src, id, tgt);
    }

/*  follow the back edges to their sources, and sweep through the targets, changing
 *  <old_nd,old_pt> targets to <new_nd,new_pt>
 */
void retarget (FuncGraph *fg, Edge *bks, int old_nd, int old_pt, int new_nd, int new_pt)
    {
    Edge *bk;
    Edge *tg;
    int tnd, tpt;

    for (bk=bks; bk!=NULL; bk=bk->link)
        {
	tnd = bk->node;
	tpt = bk->port;

	for (tg=fg->nodes[tnd].outputs[tpt].targets; tg!=NULL; tg=tg->link)
	    if ((tg->node==old_nd) && (tg->port==old_pt))
	        {
		tg->node = new_nd;
		tg->port = new_pt;
		}
	}
    }

/*  follow the targets, and sweep through the backedges, changing <old_nd,old_pt>
 *  to <new_nd,new_pt>
 */
void retarget_backedges (FuncGraph *fg, Edge *tgs, int old_nd, int old_pt, int new_nd, int new_pt)
    {
    Edge *bk;
    Edge *tg;
    int nd, pt;

    for (tg=tgs; tg!=NULL; tg=tg->link)
        {
        nd = tg->node;
        pt = tg->port;

        for (bk=fg->nodes[nd].inputs[pt].back_edges; bk!=NULL; bk=bk->link)
            if ((bk->node == old_nd) && (bk->port == old_pt))
		{
		bk->node = new_nd;
                bk->port = new_pt;
		}
        }
    }

void remove_targets_to_this_port (FuncGraph *fg, Edge *bks, int nd, int pt)
    {
    Edge *bk;
    Edge *tg, **ptg;
    int node, port;

    for (bk=bks; bk!=NULL; bk=bk->link)
        {
        node = bk->node;
        port = bk->port;
 
        for (ptg=&(fg->nodes[node].outputs[port].targets); (*ptg)!=NULL; ptg=&((*ptg)->link))
            {
            tg = *ptg;

            if ((tg->node == nd) && (tg->port == pt))
                {
                *ptg = tg->link;
                SacFree (tg)
                break;
                }
            }
        }
    }

void remove_backedges_to_this_target (FuncGraph *fg, Edge *tgs, int nd, int pt)
    {
    Edge **bkk, *bk;
    Edge *tg;
    int node, port;

    for (tg=tgs; tg!=NULL; tg=tg->link)
        {
	node = tg->node;
	port = tg->port;

	for (bkk=&(fg->nodes[node].inputs[port].back_edges); (*bkk)!=NULL; )
	    {
	    bk = *bkk;
	    if ((bk->node == nd) && (bk->port == pt))
		{
	        *bkk = bk->link;
		SacFree (bk)
		}
	    else
	        bkk = &((*bkk)->link);
	    }
	}
    }
	        
void remove_any_edges_to_a_target_node (FuncGraph *fg, int src_nd, int src_pt, int tgt_nd)
    {
    Edge **ptg, *tg, **pbk, *bk;
    int nd, pt, bnd, bpt;

    for (ptg=&(fg->nodes[src_nd].outputs[src_pt].targets); (*ptg)!=NULL; )
        {
	tg = *ptg;
	nd = tg->node;
	pt = tg->port;

	if (nd == tgt_nd)
	    {
	    for (pbk=&(fg->nodes[nd].inputs[pt].back_edges); (*pbk)!=NULL; )
	        {
		bk = *pbk;
		bnd = bk->node;
		bpt = bk->port;

		if ((bnd==src_nd) && (bpt==src_pt))
		    {
		    *pbk = (*pbk)->link;
		    SacFree (bk)
		    }
		else
		    pbk = &((*pbk)->link);
		}

	    *ptg = tg->link;
	    SacFree (tg)
	    }
	else
	    ptg = &((*ptg)->link);
	}
    }

int transfer_const_to_targets (FuncGraph *fg, int id, int out_port, char *str)
    {
    Edge *tg;
    int keep, nd, port;

    keep = FALSE;

    /* transfer the constant to the node's targets */
    for (tg=fg->nodes[id].outputs[out_port].targets; tg!=NULL; tg=tg->link)
        {
        nd = tg->node;
        port = tg->port;

	if (fg->nodes[nd].inputs[port].back_edges == NULL)
	    {
	    show_ddcf_func (fg, stdout, FALSE);
	    }
	assert (fg->nodes[nd].inputs[port].back_edges != NULL);

        if (fg->nodes[nd].inputs[port].back_edges->link == NULL)
            {
            SacFree (fg->nodes[nd].inputs[port].back_edges)
            fg->nodes[nd].inputs[port].back_edges = NULL;
            fg->nodes[nd].inputs[port].is_const = TRUE;
	    /*
            strcpy (fg->nodes[nd].inputs[port].constval, str);
	    */
	    fg->nodes[nd].inputs[port].constval = copy_to_const (str);
            }
        else
	    assert (0);
        }

    return keep;
    }

void free_ddcf_node (FuncGraph *fg, int id, IntList **itlist)
    {
    SacFree (fg->nodes[id].inputs)
    SacFree (fg->nodes[id].outputs)
    fg->nodes[id].nodetype = ND_VOIDED;
    fg->nodes[id].pragmas = NULL;

    /* remove the node from the graph's list of nodes */
    if (itlist != NULL)
        delete_ele_from_intlist (id, itlist);
    }

void append_target_list (Edge **tg1, Edge *tg2)
    {
    while (*tg1 != NULL)
        tg1 = &((*tg1)->link);
    *tg1 = tg2;
    }

void free_edge_list (Edge **plist)
    {
    Edge *tmp, *list;

    list = *plist;
    while (list != NULL)
        {
        tmp = list;
        list = list->link;
        SacFree (tmp)
        }

    *plist = NULL;
    }

Edge *copy_edge_list (Edge *edges)
    {
    Edge *ret, **pret, *eg, *tmp;

    pret = &ret;
    for (eg=edges; eg!=NULL; eg=eg->link)
        {
	SacMalloc (tmp, Edge*, sizeof (Edge))
	tmp->node = eg->node;
	tmp->port = eg->port;
	*pret = tmp;
	pret = &(tmp->link);
	}
    *pret = NULL;

    return ret;
    }

int is_simple_node (FuncGraph *fg, int id)
    {
    switch (fg->nodes[id].nodetype)
        {
        case ND_SWITCH :
        case ND_CASE : 
        case ND_WHILE :
        case ND_FORALL :
        case ND_FORNXT :
        case ND_FUNC : 
        case ND_CROSS_PROD :
        case ND_DOT_PROD : 
        case ND_WHILE_PRED :
        case ND_GRAPH :
            return FALSE;
        default :
            return TRUE;
        }
    }

void copy_target_list (Edge **tgts, Edge *srcs)
    {
    Edge *tg, *tmp;

    for (tg=srcs; tg!=NULL; tg=tg->link)
        {
	SacMalloc (tmp, Edge*, sizeof (Edge))
	tmp->node = tg->node;
	tmp->port = tg->port;
	*tgts = tmp;
	tgts = &(tmp->link);
	}
    *tgts = NULL;
    }

Edge *malloc_target (int nd, int pt, Edge *link)
    {
    Edge *res;

    SacMalloc (res, Edge*, sizeof (Edge))
    res->node = nd;
    res->port = pt;
    res->link = link;

    return res;
    }

Edge *malloc_backedge (int nd, int pt, Edge *link)
    {
    Edge *res;

    SacMalloc (res, Edge*, sizeof (Edge))
    res->node = nd;
    res->port = pt;
    res->link = link;

    return res;
    }

void install_back_edges_for_node (FuncGraph *fg, int nd)
    {
    Edge *tgt;
    Edge *bk;
    int i;

    for (i=0; i<fg->nodes[nd].num_outputs; i++)
        for (tgt=fg->nodes[nd].outputs[i].targets; tgt!=NULL; tgt=tgt->link)
	    {
	    SacMalloc (bk, Edge*, sizeof (Edge))
	    bk->node = nd;
	    bk->port = i;
	    bk->link = fg->nodes[tgt->node].inputs[tgt->port].back_edges;
	    fg->nodes[tgt->node].inputs[tgt->port].back_edges = bk;
	    }
    }

void install_back_edges (FuncGraph *fg)
    {
    int i;

    for (i=0; i<fg->nodes_used; i++)
        install_back_edges_for_node (fg, i);
    }

int new_ddcf_node (FuncGraph *fg, DdcfType nty, int ninputs, int noutputs, Location loc)
    {
    int new_id;

    new_id = alloc_ddcf_node (fg, loc);
    fg->nodes[new_id].nodetype = nty;
    fg->nodes[new_id].num_inputs = ninputs;
    fg->nodes[new_id].inputs = make_input_ports (ninputs);
    fg->nodes[new_id].num_outputs = noutputs;
    fg->nodes[new_id].outputs = make_output_ports (noutputs);

    if (is_compound (nty))
        {
	fg->nodes[new_id].My_nodes = NULL;
	fg->nodes[new_id].My_inputs = NULL;
	fg->nodes[new_id].My_outputs = NULL;
	}
    else
        switch (nty)
            {
	    case ND_FCALL :
	        fg->nodes[new_id].Sym = NULL;
	        break;
	    case ND_ARRAYREF :
	    case ND_ELE_GEN :
	    case ND_SLICE_GEN:
	        fg->nodes[new_id].Reftypes = NULL;
	        break;
	    case ND_ARR_DEF :
	        fg->nodes[new_id].DefDims = NULL;
	        break;
	    default :
                break;
	    }

    return new_id;
    }

int new_ddcf_graph (FuncGraph *fg, DdcfType nty, int num_inputs, int num_outputs, Location loc)
    {
    int new_graph, i, gin, gout;

    new_graph = new_ddcf_node (fg, nty, num_inputs, num_outputs, loc);
    fg->nodes[new_graph].My_inputs = make_io_array (num_inputs);
    for (i=0; i<num_inputs; i++)
        {
        gin = new_ddcf_node (fg, ND_G_INPUT, 0, 1, loc);
        fg->nodes[gin].Io_num = i;
        fg->nodes[gin].My_graph = new_graph;
        fg->nodes[new_graph].My_inputs[i] = gin;
        add_to_list (gin, &(fg->nodes[new_graph].My_nodes));
        }
    fg->nodes[new_graph].My_outputs = make_io_array (num_outputs);
    for (i=0; i<num_outputs; i++)
        {
        gout = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, loc);
        fg->nodes[gout].Io_num = i;
        fg->nodes[gout].My_graph = new_graph;
        fg->nodes[new_graph].My_outputs[i] = gout;
        }

    return new_graph;
    }

int type_equal (TypeInfo *t1, TypeInfo *t2)
    {
    assert (t1!=NULL && t2!=NULL);

    if (t1->type != t2->type)
	{
        return FALSE;
	}
    if (t1->totsize != t2->totsize)
	{
        return FALSE;
	}
    if (t1->fracsize != t2->fracsize)
	{
        return FALSE;
	}
    return TRUE;
    }

void copy_type_info (TypeInfo *t0, TypeInfo *t1)
    {
    assert (t0 != NULL);
    assert (t1 != NULL);
    bcopy (t1, t0, sizeof (TypeInfo));
    }

InputPort *make_input_ports (int n)
    {
    InputPort *res;
    int i, j;

    if (n == 0)
        return NULL;

    SacMalloc (res, InputPort*, n * sizeof (InputPort))

    for (i=0; i<n; i++)
        {
	res[i].ty.stat = Okay;
	res[i].ty.type = Tnone;
	res[i].ty.totsize = 0;
	res[i].ty.fracsize = 0;
	res[i].ty.kind = Knone;
	res[i].ty.dims = 0;
	for (j=0; j<MAXRANK; j++)
	    res[i].ty.dim_sizes[j] = -1;
	res[i].ty.link = NULL;
	res[i].is_const = FALSE;
	res[i].constval = NULL;
	res[i].back_edges = NULL;
	res[i].maxval = ~(Uint64)0;
	}
    return res;
    }

OutputPort *make_output_ports (int n)
    {
    OutputPort *res;
    int i, j;

    if (n == 0)
        return NULL;

    SacMalloc (res, OutputPort*, n * sizeof (OutputPort))
    for (i=0; i<n; i++)
        {
	res[i].ty.stat = Okay;
	res[i].ty.type = Tnone;
	res[i].ty.totsize = 0;
	res[i].ty.fracsize = 0;
	res[i].ty.kind = Knone;
	res[i].ty.dims = 0;
	for (j=0; j<MAXRANK; j++)
	    res[i].ty.dim_sizes[j] = -1;
	res[i].ty.link = NULL;
	res[i].targets = NULL;
	res[i].maxval = ~(Uint64)0;
	}
    return res;
    }

int alloc_ddcf_node (FuncGraph *fg, Location loc)
    {
    DdcfNode *tmp;
    int cnt, i, j, new_index;

    if (fg->nodes_allocated <= fg->nodes_used)
        {
	/* see if there's a voided one we can use */
	for (i=0; i<fg->nodes_allocated; i++)
	    if (fg->nodes[i].nodetype == ND_VOIDED)
	        {
		fg->nodes[i].loc = loc;

		for (j=0; j<MAXRANK; j++)
		    fg->nodes[i].dim_sizes[j] = -1;

		return i;
		}

	/* ran out of space, so need to allocate bigger and copy */
	tmp = fg->nodes;
	cnt = fg->nodes_used;
	SacMalloc (fg->nodes, DdcfNode*, 2 * cnt * sizeof (DdcfNode))
	bcopy (tmp, fg->nodes, cnt * sizeof (DdcfNode));

	fg->nodes_allocated = 2 * cnt;
	SacFree (tmp)
	}

    new_index = fg->nodes_used;
    (fg->nodes_used)++;
    fg->nodes[new_index].loc = loc;

    for (i=0; i<MAXRANK; i++)
        fg->nodes[new_index].dim_sizes[i] = -1;
    fg->nodes[new_index].pragmas = NULL;

    return new_index;
    }

int find_generator_graph (FuncGraph *fg, int id)
    {
    IntList *it;
    DdcfType ty;

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	ty = fg->nodes[it->val].nodetype;

	if ((ty==ND_CROSS_PROD) || (ty==ND_DOT_PROD))
	    return (it->val);
	}

    assert (0);
    }

int find_switch_key (FuncGraph *fg, int id)
    {
    IntList *it;
    DdcfType ty;

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	ty = fg->nodes[it->val].nodetype;

	if (ty==ND_SWITCH_KEY)
	    return (it->val);
	}

    assert (0);
    }

int find_selector_node (FuncGraph *fg, int id)
    {
    IntList *it;
    DdcfType ty;

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	ty = fg->nodes[it->val].nodetype;

	if (ty==ND_SELECTORS)
	    return (it->val);
	}

    return -1;
    }

unsigned int set_ones[33] = {
    0xffffffff,
    0xfffffffe,
    0xfffffffc,
    0xfffffff8,
    0xfffffff0,
    0xffffffe0,
    0xffffffc0,
    0xffffff80,
    0xffffff00,
    0xfffffe00,
    0xfffffc00,
    0xfffff800,
    0xfffff000,
    0xffffe000,
    0xffffc000,
    0xffff8000,
    0xffff0000,
    0xfffe0000,
    0xfffc0000,
    0xfff80000,
    0xfff00000,
    0xffe00000,
    0xffc00000,
    0xff800000,
    0xff000000,
    0xfe000000,
    0xfc000000,
    0xf8000000,
    0xf0000000,
    0xe0000000,
    0xc0000000,
    0x80000000,
    0x00000000
    };

void eliminate_graph_input (FuncGraph *fg, int id, int idx)
    {
    int i, in_node;

    for (i=idx; i<fg->nodes[id].num_inputs-1; i++)
        {
	fg->nodes[id].My_inputs[i] = fg->nodes[id].My_inputs[i+1];
	fg->nodes[id].inputs[i] = fg->nodes[id].inputs[i+1];

	in_node = fg->nodes[id].My_inputs[i];
	fg->nodes[in_node].Io_num = i;

	retarget (fg, fg->nodes[id].inputs[i].back_edges, id, i+1, id, i);
	}

    (fg->nodes[id].num_inputs)--;
    }

int all_inputs_are_constant (FuncGraph *fg, int id)
    {
    int i;

    for (i=0; i<fg->nodes[id].num_inputs; i++)
        if (!fg->nodes[id].inputs[i].is_const)
	    return FALSE;

    return TRUE;
    }

int is_reduction (DdcfType nd)
    {
    switch (nd)
	{
	case ND_CONSTRUCT_ARRAY :
	case ND_CONSTRUCT_CONCAT :
	case ND_CONSTRUCT_TILE :
	case ND_REDUCE_SUM :
	case ND_REDUCE_MIN :
	case ND_REDUCE_MAX :
	case ND_REDUCE_AND :
	case ND_REDUCE_OR :
	case ND_REDUCE_VAL_AT_FIRST_MIN :
	case ND_REDUCE_VAL_AT_FIRST_MAX :
	case ND_REDUCE_VAL_AT_LAST_MIN :
	case ND_REDUCE_VAL_AT_LAST_MAX :
	case ND_REDUCE_VAL_AT_MINS :
	case ND_REDUCE_VAL_AT_MAXS :
	case ND_REDUCE_PRODUCT :
	case ND_REDUCE_MEAN :
	case ND_REDUCE_ST_DEV :
	case ND_REDUCE_MODE :
	case ND_REDUCE_MEDIAN :
	case ND_REDUCE_HIST :
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
	    return TRUE;
	default :
            break;
	}
    return FALSE;
    }

void destroy_graph (FuncGraph *fg, int id)
    {
    IntList *it;
    int nd, i;

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;

	if (is_compound (fg->nodes[nd].nodetype))
	    destroy_graph (fg, nd);

	for (i=0; i<fg->nodes[nd].num_inputs; i++)
	    {
	    free_edge_list (&(fg->nodes[nd].inputs[i].back_edges));
	    }
	SacFree (fg->nodes[nd].inputs)

	for (i=0; i<fg->nodes[nd].num_outputs; i++)
	    free_edge_list (&(fg->nodes[nd].outputs[i].targets));
	SacFree (fg->nodes[nd].outputs)

	fg->nodes[nd].nodetype = ND_VOIDED;
	}

    for (i=0; i<fg->nodes[id].num_outputs; i++)
        {
	nd = fg->nodes[id].My_outputs[i];
	free_edge_list (&(fg->nodes[nd].inputs[0].back_edges));
	fg->nodes[nd].nodetype = ND_VOIDED;
	}

    free_intlist (&(fg->nodes[id].My_nodes));
    SacFree (fg->nodes[id].My_inputs)
    SacFree (fg->nodes[id].My_outputs)

    fg->nodes[id].nodetype = ND_VOIDED;
    }

int *make_io_array (int n)
    {
    int i, *res;

    SacMalloc (res, int*, n * sizeof (int))

    for (i=0; i<n; i++)
        res[i] = -1;

    return res;
    }

int propagate_and_delete (FuncGraph *fg, int id, char *str, IntList **itlist)
    {
    int keep;

    keep = transfer_const_to_targets (fg, id, 0, str);

    /*
    printf ("cast value `%s' to `%s'\n", fg->nodes[id].inputs[0].constval, str);
    */

    /* free the CAST node */
    if (! keep)
	{
	free_edge_list (&(fg->nodes[id].outputs[0].targets));
	free_ddcf_node (fg, id, itlist);
	}

    return !keep;
    }

int get_loop_rank (FuncGraph *fg, int id)
    {
    IntList *it;
    int gen_id, sgen_id, rank;

    /* find the generator graph */
    gen_id = find_generator_graph (fg, id);

    if (fg->nodes[gen_id].nodetype == ND_CROSS_PROD)
	for (rank=0, it=fg->nodes[gen_id].My_nodes; it!=NULL; it=it->link)
	    {
	    sgen_id = it->val;

	    switch (fg->nodes[sgen_id].nodetype)
	        {
		case ND_ELE_GEN :
		    rank = incr_ele_gen_rank (fg, sgen_id, rank);
		    break;
		case ND_SLICE_GEN :
		  rank = incr_slice_gen_rank (fg, sgen_id, rank);
		  break;
		case ND_WINDOW_GEN :
		    rank = incr_window_gen_rank (fg, sgen_id, rank);
		    break;
		case ND_SCALAR_GEN :
		    rank = incr_scalar_gen_rank (fg, sgen_id, rank);
		    break;
		default :
                    break;
        	}
	    }
    else
        {
	for (it=fg->nodes[gen_id].My_nodes; it!=NULL; it=it->link)
	    {
	    sgen_id = it->val;

	    switch (fg->nodes[sgen_id].nodetype)
	        {
		case ND_ELE_GEN :
		    rank = incr_ele_gen_rank (fg, sgen_id, 0);
		    break;
		case ND_SLICE_GEN :
		  rank = incr_slice_gen_rank (fg, sgen_id, 0);
		  break;
		case ND_WINDOW_GEN :
		    rank = incr_window_gen_rank (fg, sgen_id, 0);
		    break;
		case ND_SCALAR_GEN :
		    rank = incr_scalar_gen_rank (fg, sgen_id, 0);
		    break;
		default :
                    break;
        	}
	    }
	}

    return rank;
    }

void elegen_to_window (FuncGraph *fg, int nd, int gen_graph, int loop)
    {
    InputPort *new_inputs;
    int rank, i, j, k, gout, io_num, aref_node;

    rank = fg->nodes[nd].inputs[0].ty.dims;
    new_inputs = make_input_ports (1 + 2*rank);
    new_inputs[0] = fg->nodes[nd].inputs[0];

    for (i=1, j=1, k=0; i<fg->nodes[nd].num_inputs; i++, j+=2, k++)
        {
	set_type_to_uint (&(new_inputs[j].ty), 32);
	new_inputs[j].is_const = TRUE;
	new_inputs[j].constval = copy_to_const ("1");
	new_inputs[j+1] = fg->nodes[nd].inputs[i];
	fg->nodes[nd].outputs[0].ty.dim_sizes[k] = 1;
	}

    SacFree (fg->nodes[nd].inputs)
    fg->nodes[nd].inputs = new_inputs;
    fg->nodes[nd].num_inputs = 1 + 2*rank;
    fg->nodes[nd].nodetype = ND_WINDOW_GEN;
    fg->nodes[nd].outputs[0].ty.kind = Array;
    fg->nodes[nd].outputs[0].ty.dims = rank;

    gout = fg->nodes[nd].outputs[0].targets->node;
    assert (fg->nodes[gout].nodetype == ND_G_OUTPUT);
    copy_type_info (&(fg->nodes[gout].inputs[0].ty), &(fg->nodes[nd].outputs[0].ty));
    io_num = fg->nodes[gout].Io_num;

    aref_node = new_ddcf_node (fg, ND_ARRAYREF, 1+rank, 1, fg->nodes[loop].loc);
    fg->nodes[aref_node].outputs[0] = fg->nodes[gen_graph].outputs[io_num];
    fg->nodes[gen_graph].outputs[io_num].targets = NULL;
    retarget_backedges (fg, fg->nodes[aref_node].outputs[0].targets, gen_graph, io_num, aref_node, 0);
    add_edge (fg, gen_graph, io_num, aref_node, 0);
    copy_type_info (&(fg->nodes[gen_graph].outputs[io_num].ty), &(fg->nodes[nd].outputs[0].ty));
    copy_type_info (&(fg->nodes[aref_node].inputs[0].ty), &(fg->nodes[nd].outputs[0].ty));

    for (i=1; i<rank+1; i++)
        {
	set_type_to_uint (&(fg->nodes[aref_node].inputs[i].ty), 32);
	fg->nodes[aref_node].inputs[i].is_const = TRUE;
	fg->nodes[aref_node].inputs[i].constval = copy_to_const ("0");
	}

    SacMalloc (fg->nodes[aref_node].Reftypes, char*, 1 + rank)
    for (i=0; i<rank; i++)
        fg->nodes[aref_node].Reftypes[i] = '_';
    fg->nodes[aref_node].Reftypes[i] = '\0';

    insert_val_after_in_intlist (aref_node, gen_graph, &(fg->nodes[loop].My_nodes));
    }

int incr_ele_gen_rank (FuncGraph *fg, int sgen_id, int i)
{
  int j, k;

  for (j=0, k=1; j<fg->nodes[sgen_id].inputs[0].ty.dims; j++)
    {
      i++;
      k++;
    }
  
  return i;
}

int incr_slice_gen_rank (FuncGraph *fg, int sgen_id, int i)
{
  int j, k;

  for (j=0, k=1; j<fg->nodes[sgen_id].inputs[0].ty.dims; j++)
    if (fg->nodes[sgen_id].Reftypes[j] != ':')
      {
	i++;
	k++;
      }

  return i;
}

int incr_window_gen_rank (FuncGraph *fg, int sgen_id, int i)
    {
    int j, k;

    for (j=0, k=1; j<fg->nodes[sgen_id].inputs[0].ty.dims; j++)
	{
	i++;
	k+=2;
	}
    
    return i;
    }

int incr_scalar_gen_rank (FuncGraph *fg, int sgen_id, int i)
    {
    int j, k;

    for (j=0, k=0; j<fg->nodes[sgen_id].num_outputs; j++)
        {
	i++;
	k += 3;
	}

    return i;
    }

PragCell *find_pragma (FuncGraph *fg, int nd, Pragma prag)
    {
    PragCell *pc;

    for (pc=fg->nodes[nd].pragmas; pc!=NULL; pc=pc->link)
        if (pc->prag == prag)
	    return pc;

    return NULL;
    }

int has_pragma (FuncGraph *fg, int nd, Pragma prag)
    {
    PragCell *pc;

    for (pc=fg->nodes[nd].pragmas; pc!=NULL; pc=pc->link)
        if (pc->prag == prag)
	    return TRUE;

    return FALSE;
    }

void eliminate_pragma (FuncGraph *fg, int nd, Pragma prag)
    {
    PragCell **ppc, *tmp;

    for (ppc=&(fg->nodes[nd].pragmas); (*ppc)!=NULL; ppc=&((*ppc)->link))
        if ((*ppc)->prag == prag)
	    {
	    tmp = *ppc;
	    *ppc = tmp->link;
	    SacFree (tmp)
	    return;
	    }
    }

void add_pragma (PragCell **prags, Pragma prag, int *vals)
    {
    PragCell *tmp;
    int i;

    SacMalloc (tmp, PragCell*, sizeof (PragCell))
    tmp->prag = prag;

    if (vals == NULL)
        for (i=0; i<MAXRANK; i++)
	    tmp->vals[i] = -1;
    else
        for (i=0; i<MAXRANK; i++)
	    tmp->vals[i] = vals[i];

    tmp->link = *prags;
    *prags = tmp;
    }

PragCell *copy_pragmas (PragCell *pc)
    {
    PragCell *tmp, *res=NULL, **p;
    int i;

    for (p=&res; pc!=NULL; pc=pc->link)
        {
	SacMalloc (tmp, PragCell*, sizeof (PragCell))
        tmp->prag = pc->prag;
        for (i=0; i<MAXRANK; i++)
            tmp->vals[i] = pc->vals[i];
        *p = tmp;
        p = &(tmp->link);
        }

    *p = NULL;
    return res;
    }

char *copy_to_const (char *str)
    {
    char *ret;
    int len;

    len = strlen (str);
    SacMalloc (ret, char*, 1 + len * sizeof (char))
    strcpy (ret, str);
    return ret;
    }

int is_compound (DdcfType ty)
    {
    switch (ty)
        {
        case ND_SWITCH :
        case ND_CASE :
        case ND_WHILE :
        case ND_FORALL :
        case ND_FORNXT :
        case ND_FUNC :
        case ND_CROSS_PROD :
        case ND_DOT_PROD :
        case ND_WHILE_PRED :
        case ND_GRAPH :
        case ND_RC_COMPUTE :
            return TRUE;
        default :
            return FALSE;
        }
    }

void insert_node_in_intlist (FuncGraph *fg, int id, IntList **list_copy)
    {
    IntList *it;
    int last_node, nd, n, i, snd;

    n = fg->nodes[id].num_inputs;

    for (it=*list_copy; it!=NULL; it=it->link)
        {
        nd = it->val;

        for (i=0; i<n; i++)
            if (!fg->nodes[id].inputs[i].is_const)
                {
                if (fg->nodes[id].inputs[i].back_edges != NULL)
		    {
                    snd = fg->nodes[id].inputs[i].back_edges->node;
                    if (snd == nd)
                        {
                        last_node = nd;
                        break;
                        }
		    }
                }
        }

    insert_val_after_in_intlist (id, last_node, list_copy);
    }

void set_type_to_bool (TypeInfo *tt)
    {
    tt->type = Bool;
    tt->totsize = 1;
    tt->fracsize = 0;
    tt->kind = Scalar;
    tt->dims = 0;
    }

void set_type_to_uint (TypeInfo *tt, int sz)
    {
    tt->type = Uint;
    tt->totsize = sz;
    tt->fracsize = 0;
    tt->kind = Scalar;
    tt->dims = 0;
    }

void set_type_to_int (TypeInfo *tt, int sz)
    {
    tt->type = Int;
    tt->totsize = sz;
    tt->fracsize = 0;
    tt->kind = Scalar;
    tt->dims = 0;
    }

unsigned int sc_pow2[32] = {
    0x1,
    0x2,
    0x4,
    0x8,
    0x10,
    0x20,
    0x40,
    0x80,
    0x100,
    0x200,
    0x400,
    0x800,
    0x1000,
    0x2000,
    0x4000,
    0x8000,
    0x10000,
    0x20000,
    0x40000,
    0x80000,
    0x100000,
    0x200000,
    0x400000,
    0x800000,
    0x1000000,
    0x2000000,
    0x4000000,
    0x8000000,
    0x10000000,
    0x20000000,
    0x40000000,
    0x80000000
    };

unsigned int mask_ones[33] = {
    0x0,
    0x1,
    0x3,
    0x7,
    0xf,
    0x1f,
    0x3f,
    0x7f,
    0xff,
    0x1ff,
    0x3ff,
    0x7ff,
    0xfff,
    0x1fff,
    0x3fff,
    0x7fff,
    0xffff,
    0x1ffff,
    0x3ffff,
    0x7ffff,
    0xfffff,
    0x1fffff,
    0x3fffff,
    0x7fffff,
    0xffffff,
    0x1ffffff,
    0x3ffffff,
    0x7ffffff,
    0xfffffff,
    0x1fffffff,
    0x3fffffff,
    0x7fffffff,
    0xffffffff
    };
