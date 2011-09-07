#include "sassy.h"
#include "ddcf.h"
#include "error.h"

void func_inline_rec (FuncGraph*, FuncGraph*, int);
void replace_in_intlist (IntList**, int, int);
void append_target_list (Edge**, Edge*);
FuncGraph *find_funcgraph (char*);
void build_call_graph_matrix (int**, FuncGraph**, int);
int f0_calls_f1 (FuncGraph*, FuncGraph*);
int find_call (FuncGraph*, IntList*, char*);
void toposort_fgs (FuncGraph*, FuncGraph**, int**, FuncGraph**, int*, int);
void elim_if_not_exported (FuncGraph*);
int inlineable (FuncGraph*);
void delete_funcgraphs ();

char *strs[1024];
int str_idx;

void func_inline ()
    {
    FuncGraph *fg;
    FuncGraph **fg_refs;
    FuncGraph **fg_seq;
    int fg_seq_idx = 0;
    int f_cnt;
    int **matrix, i, j;

    str_idx = 0;

    /* count the number of functions */
    for (fg=fgraphs, f_cnt=0; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
	    f_cnt++;

    SacMalloc (fg_seq, FuncGraph**, f_cnt * sizeof (FuncGraph*))
    SacMalloc (fg_refs, FuncGraph**, f_cnt * sizeof (FuncGraph*))

    for (fg=fgraphs, i=0; fg!=NULL; fg=fg->link)
        if (fg->nodes_used > 0)
	    fg_refs[i++] = fg;

    SacMalloc (matrix, int**, f_cnt * sizeof (int*))
    for (i=0; i<f_cnt; i++)
	{
	SacMalloc (matrix[i], int*, f_cnt * sizeof (int))

	for (j=0; j<f_cnt; j++)
	    matrix[i][j] = 0;
	}

    build_call_graph_matrix (matrix, fg_refs, f_cnt);

    /*
    {
    int i, j;

    for (i=0; i<f_cnt; i++)
        {
	for (j=0; j<f_cnt; j++)
	    printf ("%4d", matrix[i][j]);
	printf ("\n");
	}
    }
    */

    /* topologically sort the call graph */
    for (i=0; i<f_cnt; i++)
        toposort_fgs (fg_refs[i], fg_refs, matrix, fg_seq, &fg_seq_idx, f_cnt);

    /* do the inlining */
    for (i=0; i<f_cnt; i++)
	{
        for (j=i+1; j<f_cnt; j++)
	    func_inline_rec (fg_seq[j], fg_seq[i], 0);

	if (export_list != NULL)
	    elim_if_not_exported (fg_seq[i]);
	}

    /* free all the temp structures */
    for (i=0; i<f_cnt; i++)
        SacFree (matrix[i])
    SacFree (matrix)
    SacFree (fg_refs)
    SacFree (fg_seq)

    delete_funcgraphs ();
    }

void elim_if_not_exported (FuncGraph *ffg)
    {
    StringList *s;

    if (has_pragma (ffg, 0, PRAG_NO_INLINE) || has_pragma (ffg, 0, PRAG_LOOKUP))
	return;

    for (s=export_list; s!=NULL; s=s->link)
        if (strcmp (s->str, ffg->name) == 0)
	    return;

    if (!quiet)
        printf ("  eliminating function '%s'\n", ffg->name);
    strs[str_idx++] = ffg->name;
    SacFree (ffg->nodes)
    ffg->nodes = NULL;
    ffg->nodes_allocated = 0;
    ffg->nodes_used = 0;
    }

void build_call_graph_matrix (int **matrix, FuncGraph **fg_refs, int f_cnt)
    {
    int i, j;

    for (i=0; i<f_cnt; i++)
        for (j=0; j<f_cnt; j++)
	    if ((i!=j) && (f0_calls_f1 (fg_refs[i], fg_refs[j])))
	        matrix[i][j] = 1;
    }

int f0_calls_f1 (FuncGraph *fg0, FuncGraph *fg1)
    {
    if (find_call (fg0, fg0->nodes[0].My_nodes, fg1->name))
        return TRUE;

    return FALSE;
    }

int find_call (FuncGraph *fg, IntList *its, char *name)
    {
    IntList *it;
    int ret;

    for (it=its; it!=NULL; it=it->link)
        if (fg->nodes[it->val].nodetype == ND_FCALL)
	    {
	    if (strcmp (fg->nodes[it->val].Sym, name) == 0)
	        return TRUE;
	    }
	else if (is_compound (fg->nodes[it->val].nodetype))
	    {
	    ret = find_call (fg, fg->nodes[it->val].My_nodes, name);
	    if (ret)
	        return TRUE;
	    }

    return FALSE;
    }

void toposort_fgs (FuncGraph *fg, FuncGraph **fg_refs, int **matrix,
			FuncGraph **fg_seq, int *fg_seq_idx, int f_cnt)
    {
    int i, my_ref;

    /* if we've been here before, just return */
    for (i=0; i<(*fg_seq_idx); i++)
        if (fg_seq[i] == fg)
	    return;

    /* get my index number */
    for (i=0; i<f_cnt; i++)
        if (fg_refs[i] == fg)
	    {
	    my_ref = i;
	    break;
	    }

    /* toposort any that I call */
    for (i=0; i<f_cnt; i++)
        if (matrix[my_ref][i])
	    toposort_fgs (fg_refs[i], fg_refs, matrix, fg_seq, fg_seq_idx, f_cnt);

    /* push me onto the result stack */
    fg_seq[(*fg_seq_idx)++] = fg;
    }

/* 'id' is the current node in 'fg' that is being processed;
 * 'ffg' is the function we want to inline into 'fg';
 */
void func_inline_rec (FuncGraph *fg, FuncGraph *ffg, int id)
    {
    IntList *it, *new_list;
    int nd, new_graph, i;

    copy_intlist (&new_list, &(fg->nodes[id].My_nodes));

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;

	if (is_compound (fg->nodes[nd].nodetype))
	    func_inline_rec (fg, ffg, nd);

	if ((fg->nodes[nd].nodetype == ND_FCALL) &&
			(strcmp (fg->nodes[nd].Sym, ffg->name) == 0) && (inlineable (ffg)))
	    {
	    if (ffg->nodes_used > 0)
	        {
		if (!quiet)
	            printf ("  inlining function '%s' in '%s'\n", fg->nodes[nd].Sym, fg->name);

		new_graph = copy_graph (ffg, fg, 0);

		fg->nodes[new_graph].inputs = fg->nodes[nd].inputs;
		fg->nodes[nd].inputs = NULL;
		for (i=0; i<fg->nodes[new_graph].num_inputs; i++)
		    retarget (fg, fg->nodes[new_graph].inputs[i].back_edges,
		    					nd, i, new_graph, i);

		fg->nodes[new_graph].outputs = fg->nodes[nd].outputs;
		fg->nodes[nd].outputs = NULL;
		for (i=0; i<fg->nodes[new_graph].num_outputs; i++)
		    retarget_backedges (fg,
		    	fg->nodes[new_graph].outputs[i].targets, nd, i, new_graph, i);

		replace_in_intlist (&(fg->nodes[id].My_nodes), nd, new_graph);
		replace_in_intlist (&new_list, nd, new_graph);

		fg->nodes[nd].nodetype = ND_VOIDED;
		fg->nodes[new_graph].nodetype = ND_GRAPH;

		lift_graph (fg, new_graph, &new_list);
		}
	    }
	}

    free_intlist (&(fg->nodes[id].My_nodes));
    fg->nodes[id].My_nodes = new_list;
    }

FuncGraph *find_funcgraph (char *s)
    {
    FuncGraph *fg;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
        if (strcmp (fg->name, s) == 0)
	    return fg;

    assert (0);
    }

/*  'id' is the graph that is to be lifted
 *  'new_list' is a copy of the parent graph's list; after return,
 *	replace the parent's old list with this one;
 */
void lift_graph (FuncGraph *fg, int id, IntList **new_list)
    {
    IntList *local_list;
    int i, nd, pt;
    int g_in, g_out;

    local_list = fg->nodes[id].My_nodes;

    /* for each graph input, connect the incoming edge into the graph instead of
     * to the graph's input;
     */
    for (i=0; i<fg->nodes[id].num_inputs; i++)
        {
	g_in = fg->nodes[id].My_inputs[i];

	if (fg->nodes[id].inputs[i].is_const)
	    {
	    transfer_const_to_targets (fg, g_in, 0, fg->nodes[id].inputs[i].constval);
	    free_edge_list (&(fg->nodes[g_in].outputs[0].targets));
	    }
	else
	    {
	    assert (fg->nodes[id].inputs[i].back_edges != NULL);
	    nd = fg->nodes[id].inputs[i].back_edges->node;
	    pt = fg->nodes[id].inputs[i].back_edges->port;
	    remove_targets_to_this_port (fg, fg->nodes[id].inputs[i].back_edges, id, i);
	    free_edge_list (&(fg->nodes[id].inputs[i].back_edges));
	    retarget_backedges (fg, fg->nodes[g_in].outputs[0].targets, g_in, 0, nd, pt);
	    append_target_list (&(fg->nodes[nd].outputs[pt].targets),
	    				fg->nodes[g_in].outputs[0].targets);
	    }

	delete_ele_from_intlist (g_in, &local_list);

	SacFree (fg->nodes[g_in].outputs)
	fg->nodes[g_in].nodetype = ND_VOIDED;
	}
    
    for (i=0; i<fg->nodes[id].num_outputs; i++)
        {
	g_out = fg->nodes[id].My_outputs[i];

        if (fg->nodes[g_out].inputs[0].is_const)
	    {
	    transfer_const_to_targets (fg, id, i, fg->nodes[g_out].inputs[0].constval);
	    free_edge_list (&(fg->nodes[id].outputs[i].targets));
	    }
	else if (fg->nodes[g_out].inputs[0].back_edges != NULL)
	    {
	    nd = fg->nodes[g_out].inputs[0].back_edges->node;
	    pt = fg->nodes[g_out].inputs[0].back_edges->port;
	    remove_targets_to_this_port (fg, fg->nodes[g_out].inputs[0].back_edges, g_out, 0);
	    free_edge_list (&(fg->nodes[g_out].inputs[0].back_edges));
	    retarget_backedges (fg, fg->nodes[id].outputs[i].targets, id, i, nd, pt);
	    append_target_list (&(fg->nodes[nd].outputs[pt].targets),
	    				fg->nodes[id].outputs[i].targets);
	    }
	else
	    {
	    Edge *edgs, *ed;
	    edgs = copy_edge_list (fg->nodes[id].outputs[i].targets);
	    for (ed=edgs; ed!=NULL; ed=ed->link)
	        remove_edge (fg, id, i, ed->node, ed->port);
	    free_edge_list (&edgs);
	    }

	SacFree (fg->nodes[g_out].inputs)
	fg->nodes[g_out].nodetype = ND_VOIDED;
	}

    SacFree (fg->nodes[id].inputs)
    SacFree (fg->nodes[id].outputs)
    fg->nodes[id].nodetype = ND_VOIDED;

    replace_val_with_list (id, new_list, &local_list);
    }

int inlineable (FuncGraph *fg)
    {
    if (has_pragma (fg, 0, PRAG_NO_INLINE) || has_pragma (fg, 0, PRAG_LOOKUP))
	return FALSE;

    return TRUE;
    }

void delete_funcgraphs ()
    {
    FuncGraph **pfg, *tmp;
    int i;

    for (pfg=&fgraphs; (*pfg)!=NULL; )
        {
	tmp = *pfg;
	
	for (i=0; i<str_idx; i++)
	    if (strcmp (strs[i], tmp->name) == 0)
		{
	        *pfg = tmp->link;
		SacFree (tmp)
		break;
		}

	if (i==str_idx)
	    pfg=&((*pfg)->link);
	}
    }
