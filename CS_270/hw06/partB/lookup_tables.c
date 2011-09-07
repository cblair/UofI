#include "sassy.h"
#include "error.h"
#include "ddcf.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef unsigned char Uint8;
typedef unsigned short int Uint16;
typedef unsigned int Uint32;
typedef signed char Int8;
typedef short int Int16;
typedef int Int32;
typedef float Float32;
typedef double Double64;

void read_binary_file (int, void*, int);
int make_scalar_generator (FuncGraph*, int, int*);
void convert_to_lookup_table (FuncGraph*);
void dump_and_compile (FuncGraph*, char*, char*);
int contains_func_call (FuncGraph*, int);
void alter_for_lookup (FuncGraph*, char*, int, int*);
void fetch_inputs (FuncGraph*, int, char*, int);
int valid_char (int);
void get_data_item (FILE*, char*);
void convert_to_fix_hex (char*, TypeInfo*);

void lookup_tables ()
    {
    FuncGraph *fg;
    TypeInfo *ty;
    int tot_width;

    for (fg=fgraphs; fg!=NULL; fg=fg->link)
	{
	if (fg->nodes_used == 0)
	    continue;

	if (has_pragma (fg, 0, PRAG_LOOKUP))
	    {
	    for (ty=fg->params, tot_width=0; ty!=NULL; ty=ty->link)
		{
		if ((ty->type == Float) || (ty->type == Double) || is_complex_num_type (ty->type))
		    handle_error (ErrLookupBadInType, Deadly, 0, NULL, NULL, kind_str (ty->type));

		tot_width += ty->totsize;
		}

	    ty = fg->rets;
	    if (is_complex_num_type (ty->type))
		handle_error (ErrLookupBadOutType, Deadly, 0, NULL, NULL, kind_str (ty->type));

	    if (tot_width > 14)
		handle_error (ErrLookupInBitWidth, Deadly, 0, NULL, NULL, tot_width);

	    if (fg->rets->link != NULL)
		handle_error (ErrLookupMVR, Deadly, 0, NULL, NULL);

	    if (fg->rets->kind != Scalar)
		handle_error (ErrLookupRetNonScalar, Deadly, 0, NULL, NULL);

	    if (contains_func_call (fg, 0))
		handle_error (ErrLookupHasFuncCall, Deadly, 0, NULL, NULL);

	    if (!quiet)
		printf ("  converting function '%s' to a lookup table\n", fg->name);

	    convert_to_lookup_table (fg);
	    eliminate_pragma (fg, 0, PRAG_LOOKUP);
	    }
	}
    }

void convert_to_lookup_table (FuncGraph *fg)
    {
    FuncGraph new_fg, *nfg;
    TypeInfo *ty;
    int loop_dims[MAXRANK];
    int new_graph, gout, new_id, i, pcnt, c_array, out_id, snd, spt, scalar_gen, table_size;
    char tmp_dt_file[256];

    nfg = &new_fg;
    nfg->name = "main";
    nfg->params = NULL;
    SacMalloc (nfg->rets, TypeInfo*, sizeof (TypeInfo))
    SacMalloc (nfg->nodes, DdcfNode*, 5 * sizeof (DdcfNode))
    nfg->nodes_allocated = 5;
    nfg->nodes_used = 0;
    nfg->link = NULL;

    /* count the number of parameters */;
    for (ty=fg->params, pcnt=0; ty!=NULL; ty=ty->link, pcnt++);

    /* create the return type of the new function */
    copy_type_info (nfg->rets, fg->rets);
    nfg->rets->kind = Array;
    nfg->rets->dims = pcnt;
    for (i=0; i<MAXRANK; i++)
        nfg->rets->dim_sizes[i] = -1;

    /* create the FUNC graph's shell and its single OUTPUT node */
    new_id = new_ddcf_node (nfg, ND_FUNC, 0, 1, fg->nodes[0].loc);
    nfg->nodes[new_id].My_inputs = NULL;
    nfg->nodes[new_id].My_outputs = make_io_array (1);
    nfg->nodes[new_id].My_nodes = NULL;
    gout = new_ddcf_node (nfg, ND_G_OUTPUT, 1, 0, fg->nodes[0].loc);
    nfg->nodes[new_id].My_outputs[0] = gout;
    nfg->nodes[gout].My_graph = new_id;
    nfg->nodes[gout].Io_num = 0;
    copy_type_info (&(nfg->nodes[gout].inputs[0].ty), nfg->rets);
    copy_type_info (&(nfg->nodes[new_id].outputs[0].ty), nfg->rets);

    /* copy the original function body into the new function graph's shell */
    new_graph = copy_graph (fg, nfg, 0);
    add_to_list (new_graph, &(nfg->nodes[new_id].My_nodes));
    nfg->nodes[new_graph].nodetype = ND_FORALL;

    /* insert CONSTRUCT_ARRAY node at the bottom of the copied graph */
    c_array = new_ddcf_node (nfg, ND_CONSTRUCT_ARRAY, 1, 1, fg->nodes[0].loc);
    out_id = nfg->nodes[new_graph].My_outputs[0];
    assert (nfg->nodes[out_id].inputs[0].back_edges != NULL);
    snd = nfg->nodes[out_id].inputs[0].back_edges->node;
    spt = nfg->nodes[out_id].inputs[0].back_edges->port;
    nfg->nodes[c_array].inputs[0] = nfg->nodes[out_id].inputs[0];
    nfg->nodes[out_id].inputs[0].back_edges = NULL;
    retarget (nfg, nfg->nodes[c_array].inputs[0].back_edges, out_id, 0, c_array, 0);
    copy_type_info (&(nfg->nodes[c_array].inputs[0].ty), &(nfg->nodes[snd].outputs[spt].ty));
    add_edge (nfg, c_array, 0, out_id, 0);
    copy_type_info (&(nfg->nodes[c_array].outputs[0].ty), nfg->rets);
    copy_type_info (&(nfg->nodes[out_id].inputs[0].ty), nfg->rets);
    append_intlist (&(nfg->nodes[new_graph].My_nodes), c_array);

    /* create and connect the generator */
    scalar_gen = make_scalar_generator (nfg, new_graph, loop_dims);

    for (i=0, table_size=1; i<pcnt; i++)
        table_size *= loop_dims[i];

    /* create an edge from the new_graph to the func's output */
    add_edge (nfg, new_graph, 0, gout, 0);
    copy_type_info (&(nfg->nodes[new_graph].outputs[0].ty), nfg->rets);

    dump_and_compile (nfg, fg->name, tmp_dt_file);

    /* done with the temporary function */
    SacFree (nfg->nodes)

    /* alter the body of the original function */
    alter_for_lookup (fg, tmp_dt_file, table_size, loop_dims);
    }

void alter_for_lookup (FuncGraph *fg, char *tmp_dt_file, int table_size, int *loop_dims)
    {
    IntList *it, *list_copy;
    int nd, in_id, out_id, ar_def, aref, num_inputs, i, hook_nd, hook_pt, cast_node;

    out_id = fg->nodes[0].My_outputs[0];
    num_inputs = fg->nodes[0].num_inputs;

    copy_intlist (&list_copy, &(fg->nodes[0].My_nodes));

    /* first destroy the interior nodes, except for the inputs and outputs */
    for (it=fg->nodes[0].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;

	if (fg->nodes[nd].nodetype != ND_G_INPUT)
	    {
	    for (i=0; i<fg->nodes[nd].num_inputs; i++)
	        free_edge_list (&(fg->nodes[nd].inputs[i].back_edges));
	    for (i=0; i<fg->nodes[nd].num_outputs; i++)
	        free_edge_list (&(fg->nodes[nd].outputs[i].targets));

	    if (is_compound (fg->nodes[nd].nodetype))
		{
	        destroy_graph (fg, nd);
		delete_ele_from_intlist (nd, &list_copy);
		}
	    else
	        free_ddcf_node (fg, nd, &list_copy);
	    }
	}

    free_intlist (&(fg->nodes[0].My_nodes));
    fg->nodes[0].My_nodes = list_copy;

    /* remove the stale targets and back edges from the input and output nodes */
    for (i=0; i<fg->nodes[0].num_inputs; i++)
        {
	in_id = fg->nodes[0].My_inputs[i];
	free_edge_list (&(fg->nodes[in_id].outputs[0].targets));
	}
    free_edge_list (&(fg->nodes[out_id].inputs[0].back_edges));

    /* create the lookup table array */
    ar_def = new_ddcf_node (fg, ND_ARR_DEF, table_size, 1, fg->nodes[0].loc);
    fg->nodes[ar_def].DefRank = num_inputs;
    SacMalloc (fg->nodes[ar_def].DefDims, int*, MAXRANK * sizeof (int))
    for (i=0; i<num_inputs; i++)
        fg->nodes[ar_def].DefDims[i] = loop_dims[i];

    append_intlist (&(fg->nodes[0].My_nodes), ar_def);

    for (i=0; i<table_size; i++)
	{
	fg->nodes[ar_def].inputs[i].is_const = TRUE;
        copy_type_info (&(fg->nodes[ar_def].inputs[i].ty), &(fg->nodes[out_id].inputs[0].ty));
	}
    copy_type_info (&(fg->nodes[ar_def].outputs[0].ty), &(fg->nodes[out_id].inputs[0].ty));
    fg->nodes[ar_def].outputs[0].ty.kind = Array;
    fg->nodes[ar_def].outputs[0].ty.dims = num_inputs;
    for (i=0; i<num_inputs; i++)
        fg->nodes[ar_def].outputs[0].ty.dim_sizes[i] = loop_dims[i];

    fetch_inputs (fg, ar_def, tmp_dt_file, table_size);

    aref = new_ddcf_node (fg, ND_ARRAYREF, 1+num_inputs, 1, fg->nodes[0].loc);
    SacMalloc (fg->nodes[aref].Reftypes, char*, 1 + num_inputs)
    for (i=0; i<num_inputs; i++)
        fg->nodes[aref].Reftypes[i] = '_';
    fg->nodes[aref].Reftypes[i] = '\0';

    append_intlist (&(fg->nodes[0].My_nodes), aref);

    add_edge (fg, ar_def, 0, aref, 0);
    copy_type_info (&(fg->nodes[aref].inputs[0].ty), &(fg->nodes[ar_def].outputs[0].ty));
    add_edge (fg, aref, 0, out_id, 0);
    copy_type_info (&(fg->nodes[aref].outputs[0].ty), &(fg->nodes[out_id].inputs[0].ty));

    /* connect the function's inputs to the indices of the 'aref' node */
    for (i=0; i<fg->nodes[0].num_inputs; i++)
        {
	in_id = fg->nodes[0].My_inputs[i];
	hook_nd = in_id;
	hook_pt = 0;

	if (fg->nodes[in_id].outputs[0].ty.type != Uint)
	    {
	    cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[0].loc);
	    add_edge (fg, hook_nd, hook_pt, cast_node, 0);
	    insert_val_after_in_intlist (cast_node, hook_nd, &(fg->nodes[0].My_nodes));
	    copy_type_info (&(fg->nodes[cast_node].inputs[0].ty), &(fg->nodes[hook_nd].outputs[hook_pt].ty));
	    copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), &(fg->nodes[hook_nd].outputs[hook_pt].ty));
	    fg->nodes[cast_node].outputs[0].ty.type = Bits;
	    hook_nd = cast_node;
	    hook_pt = 0;

	    cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, fg->nodes[0].loc);
	    add_edge (fg, hook_nd, hook_pt, cast_node, 0);
	    insert_val_after_in_intlist (cast_node, hook_nd, &(fg->nodes[0].My_nodes));
	    copy_type_info (&(fg->nodes[cast_node].inputs[0].ty), &(fg->nodes[hook_nd].outputs[hook_pt].ty));
	    copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), &(fg->nodes[hook_nd].outputs[hook_pt].ty));
	    fg->nodes[cast_node].outputs[0].ty.type = Uint;
	    hook_nd = cast_node;
	    hook_pt = 0;
	    }

	add_edge (fg, hook_nd, hook_pt, aref, i+1);
	copy_type_info (&(fg->nodes[aref].inputs[i+1].ty), &(fg->nodes[hook_nd].outputs[hook_pt].ty));
	}
    }

void dump_and_compile (FuncGraph *nfg, char *name, char *tmp_dt_file)
    {
    char tmp_src_name[256], tmp_exec_name[256], xeq[256], *sassyhome;
    pid_t pid = 0;
    FILE *fp;

    pid = getpid ();
    sprintf (tmp_src_name, "/tmp/%s_tmp_lookup_src_%d.ddcf", name, pid);
    sprintf (tmp_exec_name, "/tmp/%s_tmp_lookup_src_%d", name, pid);
    sprintf (tmp_dt_file, "/tmp/%s_tmp_lookup_src_%d.dt", name, pid);

    if ((fp = fopen (tmp_src_name, "w")) == NULL)
	handle_error (ErrLookupFileOpen, Deadly, 0, NULL, NULL, tmp_src_name);

    show_ddcf_func (nfg, fp, TRUE);
    fclose (fp);

    if ((sassyhome = getenv ("SASSYHOME")) == NULL)
	handle_error (ErrLookupSassyhome, Deadly, 0, NULL, NULL);

    sprintf (xeq, "%s/scc -o %s %s", sassyhome, tmp_exec_name, tmp_src_name);
    if (system (xeq) != 0)
	handle_error (ErrLookupSpawnFail, Deadly, 0, NULL, NULL);

    sprintf (xeq, "/bin/rm -f %s", tmp_src_name);
    if (system (xeq) != 0)
	handle_error (ErrLookupRemoveFile, Deadly, 0, NULL, NULL, tmp_src_name);

    sprintf (xeq, "%s -ofb0 %s", tmp_exec_name, tmp_dt_file);
    if (system (xeq) != 0)
	handle_error (ErrLookupExec, Deadly, 0, NULL, NULL, tmp_exec_name);

    sprintf (xeq, "/bin/rm -f %s", tmp_exec_name);
    if (system (xeq) != 0)
	handle_error (ErrLookupTmpRemove, Deadly, 0, NULL, NULL, tmp_exec_name);
    }

void fetch_inputs (FuncGraph *fg, int ar_def, char *tmp_dt_file, int table_size)
    {
    int i, dt_size;
    int fd;
    char str[128], xeq[256];
    void *mem_space;

    if ((fd = open (tmp_dt_file, O_RDONLY, 0)) < 0)
	handle_error (ErrLookupDataOpen, Deadly, 0, NULL, NULL, tmp_dt_file);

    switch (fg->nodes[ar_def].outputs[0].ty.type)
        {
	case Uint :
	case Int :
	case Ufix :
	case Fix :
	case Bool :
	case Bits :
	case Float :
	case Double :
	    if (fg->nodes[ar_def].outputs[0].ty.totsize <= 8)
	        dt_size = 1;
	    else if (fg->nodes[ar_def].outputs[0].ty.totsize <= 16)
	        dt_size = 2;
	    else if (fg->nodes[ar_def].outputs[0].ty.totsize <= 32)
	        dt_size = 4;
	    else
	        dt_size = 8;
	    break;
	default :
	    assert (0);
	}

    SacMalloc (mem_space, void*, dt_size*table_size)
    read_binary_file (fd, mem_space, dt_size*table_size);

    switch (fg->nodes[ar_def].outputs[0].ty.type)
        {
	case Uint :
	    if (fg->nodes[ar_def].outputs[0].ty.totsize <= 8)
	        {
		Uint8 *p;
		p = (Uint8*) mem_space;
		for (i=0; i<table_size; i++)
		    {
		    sprintf (str, "%u", (Uint32)*(p++));
		    SacMalloc (fg->nodes[ar_def].inputs[i].constval, char*, 1 + strlen (str))
		    strcpy (fg->nodes[ar_def].inputs[i].constval, str);
		    }
		}
	    else if (fg->nodes[ar_def].outputs[0].ty.totsize <= 16)
	        {
		Uint16 *p;
		p = (Uint16*) mem_space;
		for (i=0; i<table_size; i++)
		    {
		    sprintf (str, "%u", *(p++));
		    SacMalloc (fg->nodes[ar_def].inputs[i].constval, char*, 1 + strlen (str))
		    strcpy (fg->nodes[ar_def].inputs[i].constval, str);
		    }
		}
	    else if (fg->nodes[ar_def].outputs[0].ty.totsize <= 32)
	        {
		Uint32 *p;
		p = (Uint32*) mem_space;
		for (i=0; i<table_size; i++)
		    {
		    sprintf (str, "%u", *(p++));
		    SacMalloc (fg->nodes[ar_def].inputs[i].constval, char*, 1 + strlen (str))
		    strcpy (fg->nodes[ar_def].inputs[i].constval, str);
		    }
		}
	    break;
	case Int :
	    if (fg->nodes[ar_def].outputs[0].ty.totsize <= 8)
	        {
		Int8 *p, val;
		p = (Int8*) mem_space;
		for (i=0; i<table_size; i++)
		    {
		    val = *(p++);
		    val = sign_extend (val, fg->nodes[ar_def].outputs[0].ty.totsize, 32);
		    sprintf (str, "%d", val);
		    SacMalloc (fg->nodes[ar_def].inputs[i].constval, char*, 1 + strlen (str))
		    strcpy (fg->nodes[ar_def].inputs[i].constval, str);
		    }
		}
	    else if (fg->nodes[ar_def].outputs[0].ty.totsize <= 16)
	        {
		Int16 *p, val;
		p = (Int16*) mem_space;
		for (i=0; i<table_size; i++)
		    {
		    val = *(p++);
		    val = sign_extend (val, fg->nodes[ar_def].outputs[0].ty.totsize, 32);
		    sprintf (str, "%d", val);
		    SacMalloc (fg->nodes[ar_def].inputs[i].constval, char*, 1 + strlen (str))
		    strcpy (fg->nodes[ar_def].inputs[i].constval, str);
		    }
		}
	    else if (fg->nodes[ar_def].outputs[0].ty.totsize <= 32)
	        {
		Int32 *p, val;
		p = (Int32*) mem_space;
		for (i=0; i<table_size; i++)
		    {
		    val = *(p++);
		    val = sign_extend (val, fg->nodes[ar_def].outputs[0].ty.totsize, 32);
		    sprintf (str, "%d", val);
		    SacMalloc (fg->nodes[ar_def].inputs[i].constval, char*, 1 + strlen (str))
		    strcpy (fg->nodes[ar_def].inputs[i].constval, str);
		    }
		}
	    break;
	case Ufix :
	case Fix :
	case Bits :
	case Bool :
	    if (fg->nodes[ar_def].outputs[0].ty.totsize <= 8)
	        {
		Uint8 *p;
		p = (Uint8*) mem_space;
		for (i=0; i<table_size; i++)
		    {
		    sprintf (str, "0x%x", *(p++));
		    SacMalloc (fg->nodes[ar_def].inputs[i].constval, char*, 1 + strlen (str))
		    strcpy (fg->nodes[ar_def].inputs[i].constval, str);
		    }
		}
	    else if (fg->nodes[ar_def].outputs[0].ty.totsize <= 16)
	        {
		Uint16 *p;
		p = (Uint16*) mem_space;
		for (i=0; i<table_size; i++)
		    {
		    sprintf (str, "0x%x", *(p++));
		    SacMalloc (fg->nodes[ar_def].inputs[i].constval, char*, 1 + strlen (str))
		    strcpy (fg->nodes[ar_def].inputs[i].constval, str);
		    }
		}
	    else if (fg->nodes[ar_def].outputs[0].ty.totsize <= 32)
	        {
		Uint32 *p;
		p = (Uint32*) mem_space;
		for (i=0; i<table_size; i++)
		    {
		    sprintf (str, "0x%x", *(p++));
		    SacMalloc (fg->nodes[ar_def].inputs[i].constval, char*, 1 + strlen (str))
		    strcpy (fg->nodes[ar_def].inputs[i].constval, str);
		    }
		}
	    break;
	case Float :
	    {
	    Float32 *p;
	    p = (Float32*) mem_space;
	    for (i=0; i<table_size; i++)
		{
		sprintf (str, "%20.10e", *(p++));
		SacMalloc (fg->nodes[ar_def].inputs[i].constval, char*, 1 + strlen (str))
		strcpy (fg->nodes[ar_def].inputs[i].constval, str);
		}
	    }
	    break;
	case Double :
	    {
	    Double64 *p;
	    p = (Double64*) mem_space;
	    for (i=0; i<table_size; i++)
		{
		sprintf (str, "%30.20g", *(p++));
		SacMalloc (fg->nodes[ar_def].inputs[i].constval, char*, 1 + strlen (str))
		strcpy (fg->nodes[ar_def].inputs[i].constval, str);
		}
	    }
	    break;
	default :
	    assert (0);
	}

    close (fd);
    SacFree (mem_space)

    sprintf (xeq, "/bin/rm -f %s", tmp_dt_file);
    if (system (xeq) != 0)
	handle_error (ErrLookupTmpFileRemove, Deadly, 0, NULL, NULL, tmp_dt_file);
    }

void convert_to_fix_hex (char *str, TypeInfo *ty)
    {
    double dval;
    int ival;

    sscanf (str, "%lf", &dval);
    dval *= sc_pow2[ty->fracsize];
    ival = dval;
    sprintf (str, "0x%x", ival & mask_ones[ty->totsize]);
    }

void get_data_item (FILE *fp, char *str)
    {
    int ch, i;

    i = 0;

    do  {
        ch = fgetc (fp);
        } while (!valid_char (ch));

    do  {
        str[i++] = ch;
	ch = fgetc (fp);
	} while (valid_char (ch));

    str[i] = '\0';
    }

int valid_char (int ch)
    {
    return isalnum (ch) || (ch == '.') || (ch == '-') || (ch == '+');
    }

int make_scalar_generator (FuncGraph *nfg, int new_graph, int *loop_dims)
    {
    int n_inputs, i, k, in_id, dot_gen, sc_gen, gout, cast_node, hook_nd, hook_pt;
    char str[128];

    n_inputs = nfg->nodes[new_graph].num_inputs;

    /* determine the dimensions */
    for (i=0; i<n_inputs; i++)
        {
	in_id = nfg->nodes[new_graph].My_inputs[i];
	loop_dims[i] = sc_pow2[nfg->nodes[in_id].outputs[0].ty.totsize];
	}

    /* create the generator graph and the scalar generator within it */
    dot_gen = new_ddcf_node (nfg, ND_DOT_PROD, 0, n_inputs, nfg->nodes[new_graph].loc);
    nfg->nodes[dot_gen].My_outputs = make_io_array (n_inputs);
    add_to_list (dot_gen, &(nfg->nodes[new_graph].My_nodes));
    sc_gen = new_ddcf_node (nfg, ND_SCALAR_GEN, n_inputs*3, n_inputs, nfg->nodes[new_graph].loc);
    nfg->nodes[sc_gen].Rank = n_inputs;
    add_to_list (sc_gen, &(nfg->nodes[dot_gen].My_nodes));

    /* set all the inputs of the scalar generator to their constant values, create
     * output nodes for the generator graph, and connect the scalar outputs to
     * graph's output nodes
     */
    for (i=0, k=0; i<n_inputs; i++, k+=3)
        {
	in_id = nfg->nodes[new_graph].My_inputs[i];

	/* set the three constant inputs of the scalar generator */
	nfg->nodes[sc_gen].inputs[k].is_const = TRUE;
	SacMalloc (nfg->nodes[sc_gen].inputs[k].constval, char*, 2 * sizeof (char))
	strcpy (nfg->nodes[sc_gen].inputs[k].constval, "0");
	set_type_to_uint (&(nfg->nodes[sc_gen].inputs[k].ty), 32);
	nfg->nodes[sc_gen].inputs[k+1].is_const = TRUE;
	sprintf (str, "%d", loop_dims[i]-1);
	SacMalloc (nfg->nodes[sc_gen].inputs[k+1].constval, char*, 1 + strlen (str))
	strcpy (nfg->nodes[sc_gen].inputs[k+1].constval, str);
	set_type_to_uint (&(nfg->nodes[sc_gen].inputs[k+1].ty), 32);
	nfg->nodes[sc_gen].inputs[k+2].is_const = TRUE;
	SacMalloc (nfg->nodes[sc_gen].inputs[k+2].constval, char*, 2 * sizeof (char))
	strcpy (nfg->nodes[sc_gen].inputs[k+2].constval, "1");
	set_type_to_uint (&(nfg->nodes[sc_gen].inputs[k+2].ty), 32);

	/* create an output node for the graph */
	gout = new_ddcf_node (nfg, ND_G_OUTPUT, 1, 0, nfg->nodes[new_graph].loc);
	nfg->nodes[gout].My_graph = dot_gen;
	nfg->nodes[gout].Io_num = i;
	nfg->nodes[dot_gen].My_outputs[i] = gout;

	add_edge (nfg, sc_gen, i, gout, 0);
	set_type_to_uint (&(nfg->nodes[sc_gen].outputs[i].ty), nfg->nodes[in_id].outputs[0].ty.totsize);
	copy_type_info (&(nfg->nodes[gout].inputs[0].ty), &(nfg->nodes[sc_gen].outputs[i].ty));
	copy_type_info (&(nfg->nodes[dot_gen].outputs[i].ty), &(nfg->nodes[sc_gen].outputs[i].ty));
	}

    /* replace the edges from the graph inputs with edges from the generator graph */
    for (i=0; i<n_inputs; i++)
        {
        in_id = nfg->nodes[new_graph].My_inputs[i];

	hook_nd = dot_gen;
	hook_pt = i;

	/* here we may have to cast the generator output to the old function's intut
	 * type, going through a 'bits' step to preserve the bit pattern
	 */
	if (nfg->nodes[in_id].outputs[0].ty.type != Uint)
	    {
	    cast_node = new_ddcf_node (nfg, ND_CAST, 1, 1, nfg->nodes[new_graph].loc);
	    add_edge (nfg, hook_nd, hook_pt, cast_node, 0);
	    copy_type_info (&(nfg->nodes[cast_node].inputs[0].ty), &(nfg->nodes[hook_nd].outputs[hook_pt].ty));
	    copy_type_info (&(nfg->nodes[cast_node].outputs[0].ty), &(nfg->nodes[hook_nd].outputs[hook_pt].ty));
	    nfg->nodes[cast_node].outputs[0].ty.type = Bits;
	    insert_val_after_in_intlist (cast_node, hook_nd, &(nfg->nodes[new_graph].My_nodes));
	    hook_nd = cast_node;
	    hook_pt = 0;

	    cast_node = new_ddcf_node (nfg, ND_CAST, 1, 1, nfg->nodes[new_graph].loc);
	    add_edge (nfg, hook_nd, hook_pt, cast_node, 0);
	    copy_type_info (&(nfg->nodes[cast_node].inputs[0].ty), &(nfg->nodes[hook_nd].outputs[hook_pt].ty));
	    copy_type_info (&(nfg->nodes[cast_node].outputs[0].ty), &(nfg->nodes[in_id].outputs[0].ty));
	    insert_val_after_in_intlist (cast_node, hook_nd, &(nfg->nodes[new_graph].My_nodes));
	    hook_nd = cast_node;
	    hook_pt = 0;
	    }

	nfg->nodes[hook_nd].outputs[hook_pt] = nfg->nodes[in_id].outputs[0];
	nfg->nodes[in_id].outputs[0].targets = NULL;
	retarget_backedges (nfg, nfg->nodes[hook_nd].outputs[hook_pt].targets, in_id, 0, hook_nd, hook_pt);
	free_ddcf_node (nfg, in_id, &(nfg->nodes[new_graph].My_nodes));
	}

    /* the loop graph no longer has inputs */
    nfg->nodes[new_graph].num_inputs = 0;

    return dot_gen;
    }

int contains_func_call (FuncGraph *fg, int id)
    {
    IntList *it;
    int nd;

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
	{
	nd = it->val;

        if (fg->nodes[nd].nodetype == ND_FCALL)
	    return TRUE;

	if (is_compound (fg->nodes[nd].nodetype))
	    if (contains_func_call (fg, nd))
	        return TRUE;
	}

    return FALSE;
    }
