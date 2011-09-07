#include "sassy.h"
#include "ddcf.h"
#include "error.h"

void print_a_type (TypeInfo*, FILE*);

typedef struct copyassoc {
    int orig;
    int copy;
    struct copyassoc *link;
    } CopyAssoc;

CopyAssoc *assoc_stk = NULL;
int assoc_stk_size = 0;
int assoc_idx;

void alloc_and_create_assocs (FuncGraph*, FuncGraph*, int);
void fill_in_node_info (FuncGraph*, FuncGraph*);
void push_assoc (int, int);
int lookup (int);

int copy_graph (FuncGraph *sfg, FuncGraph *tfg, int id)
    {
    int new_graph;

    assoc_idx = 0;
    assoc_stk_size = sfg->nodes_allocated;
    SacMalloc (assoc_stk, CopyAssoc*, assoc_stk_size * sizeof (CopyAssoc))

    new_graph = new_ddcf_node (tfg, sfg->nodes[id].nodetype,
        sfg->nodes[id].num_inputs, sfg->nodes[id].num_outputs, sfg->nodes[id].loc);
    push_assoc (id, new_graph);

    alloc_and_create_assocs (sfg, tfg, id);
    fill_in_node_info (sfg, tfg);

    SacFree (assoc_stk)
    assoc_stk_size = 0;

    return new_graph;
    }

/* node 'id' is a compound node */
void alloc_and_create_assocs (FuncGraph *sfg, FuncGraph *tfg, int id)
    {
    IntList *it;
    int i, cnd, nd;

    /* push the interior nodes */
    for (it=sfg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	cnd = it->val;
	nd = new_ddcf_node (tfg, sfg->nodes[cnd].nodetype,
		sfg->nodes[cnd].num_inputs, sfg->nodes[cnd].num_outputs,
		sfg->nodes[cnd].loc);
	push_assoc (cnd, nd);
	if (is_compound (sfg->nodes[cnd].nodetype))
	    alloc_and_create_assocs (sfg, tfg, cnd);
	}

    /* push the output nodes */
    for (i=0; i<sfg->nodes[id].num_outputs; i++)
        {
	cnd = sfg->nodes[id].My_outputs[i];
	nd = new_ddcf_node (tfg, sfg->nodes[cnd].nodetype,
	    	sfg->nodes[cnd].num_inputs, sfg->nodes[cnd].num_outputs,
		sfg->nodes[cnd].loc);
	push_assoc (cnd, nd);
	}
    }

void fill_in_node_info (FuncGraph *sfg, FuncGraph *tfg)
    {
    IntList *it;
    Edge *tg, **ptg;
    Edge *bk, **pbk;
    int i, j, n, len, orig, copy;

    for (j=0; j<assoc_idx; j++)
        {
	orig = assoc_stk[j].orig;
	copy = assoc_stk[j].copy;

	if (j>0)
	    {
	    /* create back edges or constants */
	    for (i=0; i<sfg->nodes[orig].num_inputs; i++)
	        {
	        if (sfg->nodes[orig].inputs[i].is_const)
		    {
		    tfg->nodes[copy].inputs[i].is_const = TRUE;
		    tfg->nodes[copy].inputs[i].constval = copy_to_const (sfg->nodes[orig].inputs[i].constval);
		    }
	        else
		    {
		    tfg->nodes[copy].inputs[i].is_const = FALSE;
	            for (bk=sfg->nodes[orig].inputs[i].back_edges,
	    			pbk=&(tfg->nodes[copy].inputs[i].back_edges);
				bk!=NULL; bk=bk->link, pbk=&((*pbk)->link))
		        {
			SacMalloc (*pbk, Edge*, sizeof (Edge))
		        (*pbk)->node = lookup (bk->node);
		        (*pbk)->port = bk->port;
		        }
	            *pbk = NULL;
		    }
	        }

	    /* create targets */
	    for (i=0; i<sfg->nodes[orig].num_outputs; i++)
	        {
	        for (tg=sfg->nodes[orig].outputs[i].targets,
	    			ptg=&(tfg->nodes[copy].outputs[i].targets);
				tg!=NULL; tg=tg->link, ptg=&((*ptg)->link))
	            {
		    SacMalloc (*ptg, Edge*, sizeof (Edge))
		    (*ptg)->node = lookup (tg->node);
		    (*ptg)->port = tg->port;
		    }
	        *ptg = NULL;
	        }
	    }

	for (i=0; i<sfg->nodes[orig].num_inputs; i++)
	    copy_type_info (&(tfg->nodes[copy].inputs[i].ty),
	    				&(sfg->nodes[orig].inputs[i].ty));

	for (i=0; i<sfg->nodes[orig].num_outputs; i++)
	    copy_type_info (&(tfg->nodes[copy].outputs[i].ty),
	    				&(sfg->nodes[orig].outputs[i].ty));

	/* transfer any special information */
	switch (sfg->nodes[orig].nodetype)
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
		/* create and fill in the My_inputs array */
		n = sfg->nodes[orig].num_inputs;
		SacMalloc (tfg->nodes[copy].My_inputs, int*, n * sizeof (int))
		for (i=0; i<n; i++)
		    {
		    tfg->nodes[copy].My_inputs[i] = lookup (sfg->nodes[orig].My_inputs[i]);
		    }

		/* create and fill in the My_outputs array */
		n = sfg->nodes[orig].num_outputs;
		SacMalloc (tfg->nodes[copy].My_outputs, int*, n * sizeof (int))
		for (i=0; i<n; i++)
		    tfg->nodes[copy].My_outputs[i] = lookup (sfg->nodes[orig].My_outputs[i]);

		tfg->nodes[copy].My_nodes = NULL;
		for (it=sfg->nodes[orig].My_nodes; it!=NULL; it=it->link)
		    append_intlist (&(tfg->nodes[copy].My_nodes), lookup (it->val));

		if ((sfg->nodes[orig].nodetype==ND_FORALL) ||
						(sfg->nodes[orig].nodetype==ND_FORNXT))
		    {
		    for (i=0; i<MAXRANK; i++)
	                tfg->nodes[copy].dim_sizes[i] = sfg->nodes[orig].dim_sizes[i];

		    tfg->nodes[copy].pragmas = copy_pragmas (sfg->nodes[orig].pragmas);
		    }
	        break;
	    case ND_NEXT :
	        tfg->nodes[copy].In_next_id = lookup (sfg->nodes[orig].In_next_id);
		break;
	    case ND_G_INPUT :
	    case ND_G_INPUT_NEXT :
	    case ND_G_OUTPUT :
	        tfg->nodes[copy].Io_num = sfg->nodes[orig].Io_num;
	        tfg->nodes[copy].My_graph = lookup (sfg->nodes[orig].My_graph);
		break;
	    case ND_FCALL :
		len = strlen (sfg->nodes[orig].Sym);
		SacMalloc (tfg->nodes[copy].Sym, char*, (1+len) * sizeof (char))
		strcpy (tfg->nodes[copy].Sym, sfg->nodes[orig].Sym);
		break;
	    case ND_INTRINCALL :
	        tfg->nodes[copy].Intrin_func = sfg->nodes[orig].Intrin_func;
		break;
	    case ND_ARRAYREF :
	    case ND_ELE_GEN :
	    case ND_SLICE_GEN:
		len = sfg->nodes[orig].inputs[0].ty.dims;
		SacMalloc (tfg->nodes[copy].Reftypes, char*, (1+len) * sizeof (char))
		for (i=0; i<len; i++)
		    tfg->nodes[copy].Reftypes[i] = sfg->nodes[orig].Reftypes[i];
		break;
	    case ND_SCALAR_GEN :
	        tfg->nodes[copy].Rank = sfg->nodes[orig].Rank;
		break;
	    case ND_ARR_DEF :
	        tfg->nodes[copy].DefRank = sfg->nodes[orig].DefRank;
		SacMalloc (tfg->nodes[copy].DefDims, int*, MAXRANK * sizeof (int))
		for (i=0; i<MAXRANK; i++)
		    tfg->nodes[copy].DefDims[i] = sfg->nodes[orig].DefDims[i];
		break;
	    case ND_REDUCE_VAL_AT_FIRST_MAX :
	    case ND_REDUCE_VAL_AT_FIRST_MIN :
	    case ND_REDUCE_VAL_AT_LAST_MAX :
	    case ND_REDUCE_VAL_AT_LAST_MIN :
	    case ND_REDUCE_VAL_AT_MAXS :
	    case ND_REDUCE_VAL_AT_MINS :
	        tfg->nodes[copy].VecSize = sfg->nodes[orig].VecSize;
		break;
	    default :
                break;
	    }
        }
    }

void push_assoc (int old_id, int new_id)
    {
    assert (assoc_idx != assoc_stk_size);

    assoc_stk[assoc_idx].orig = old_id;
    assoc_stk[assoc_idx].copy = new_id;
    assoc_idx++;
    }

int lookup (int id)
    {
    int i;

    for (i=0; i<assoc_idx; i++)
        if (assoc_stk[i].orig == id)
	    return assoc_stk[i].copy;

    assert (0);
    }
