/*  This collection of routines builds DDCF graphs from a type-annotated
 *  parse tree. The basic idea behind this approach is simple, but there
 *  are lots of details and special cases that have to be dealt with, as
 *  well as the expansion *  of various operators (e.g. array_sum) into
 *  explicit loops.
 *
 *  It's easiest to understand graph building by first ignoring the issue
 *  of subgraphs, and thinking only about the building of a flat graph
 *  from an expression. As the parse tree is traversed, the graph is being
 *  built. The graph has the same general form as the tree, but is upside
 *  down. Consider for example an encounter with an ADD node in the tree,
 *  with two *  subtrees that form its operands:
 *
 *                          |
 *                         ADD
 *                         / \
 *                        /   \
 *                     epx0   exp1
 *
 *  This will turn into a dataflow node in which the two operand
 *  expressions come as inputs from above:
 *
 *                      exp0     exp1
 *                        |       |
 *                        |       |
 *                      +-----------+
 *                      |           |
 *                      |  ND_ADD   |
 *                      +-----------+
 *                            |
 *                            |
 *
 *  When the ADD tree node is processed, the ND_ADD node is created (using
 *  the 'new_ddcf_node' function), specified with two input ports and one
 *  output port. Then each of the children are processed recursively, with
 *  information telling each that its graph output should target the ND_ADD
 *  node at the designated port (this is done using the 'handle_child'
 *  function.)
 *
 *  The heart of this process is the function 'make_ddcf_mvr_expr'. It has
 *  the header:
 *
 *   void make_ddcf_mvr_expr (NODEPNT m, FuncGraph *fg, AssocData *rets)
 *
 *  It is given a subtree pointer 'm', the graph environment it's currently
 *  working in 'fg', and a pointer 'rets' that is used to pass information
 *  back to the caller about the graph that was created. This function can
 *  handle multiple-return values. Consider as an example the RHS of the
 *  SA-C assignment:
 *
 *	uint8 a, uint8 b = x+y-2, 3*z;
 *
 *  The 'make_ddcf_mvr_expr' function will be called with a pointer to the
 *  subtree of the RHS, and with an AssocData array of size two passed as
 *  'rets'. (Note that the caller always knows how many values are coming
 *  back, so the correct size array 'rets' can always be allocated and
 *  passed to 'make_ddcf_mvr_expr'.) The function will create the graphs
 *  for the two RHS expressions, 'rets[0]' will be given the node and port
 *  number of the output of the first expression, and 'rets[1]' will be
 *  given the node and port number of the output of the second expression.
 *
 *  Sometimes a constant value is returned, which has no actual graph
 *  associated with it. For example:
 *
 *	uint8 a = 42;
 *
 *  In this case 'rets[0]' will have it 'is_const' field set to TRUE and
 *  'ref.constval' will hold the string "42".
 *
 *  Stepping one level higher in the process, when an assignment is handled,
 *  it first calls 'make_ddcf_mvr_expr' to create the graph of the RHS. It
 *  then pushes to a compile-time stack an association of the variable id
 *  with the <node,port> of the expression's output (or if the expression
 *  is a constant, an association with the string value of that constant.)
 *  This is the information that came back through the 'rets' parameter.
 *  Later references to that variable are looked up to discover where in
 *  the graph that value comes from. (See the function 'find_id'.)
 *
 *  Flat graphs are easy to understand. Unfortunately, the DDCF's hierarchical
 *  structure complicates the picture. Graph component generation, as described
 *  above, takes place within the environment of a particular subgraph being
 *  created. The process starts in the function's ND_FUNC graph, but within that
 *  subgraph other subgraphs may nest arbitrarily deeply. Each nest level
 *  of subgraph requires its own compile-time stack of variable associations.
 *  When a variable reference is encountered, the stacks are explored one by
 *  one. If the variable is found in the top-most stack, nothing needs to
 *  be done. But if it exists deeper in the stack-of-stacks (because it is
 *  defined in a higher-level enclosing graph) then it must be "threaded"
 *  through the input interface of each of the enclosing graphs to bring
 *  it into the current graph. The function 'import_local_variable' does
 *  this. When a graph at a current level is finished, its local stack on
 *  the stack-of-stacks is popped and discarded (because its local variables
 *  are no longer in scope.)
 *
 *  A subgraph (i.e. compound node) interfaces with its exterior via input
 *  and output ports. No edge is allowed to cross the graph's boundary without
 *  going explicitly through a port. Also, no input to a compound node
 *  is allowed to be a constant. (Instead the constant value should be
 *  propagated directly into the graph to the places where it's used.)
 *  When a compound node begins to be built, it is not yet known how many
 *  input ports it may need, so the node is allocated with the function
 *  'alloc_ddcf_node' which does not yet allocate the input and output port
 *  arrays for the node. As the node's internal graph is generated, and
 *  values are threaded as needed into the graph, a ND_G_INPUT node is
 *  created for each and put onto another compile-time stack-of-stacks.
 *  When the internal graph generation is done, the number of inputs is
 *  known, the input port array is allocated, and the appropriate associations
 *  between the input port array and the ND_G_INPUT nodes is created.
 *  See the function 'handle_graph_input_nodes'.
 *
 *  Nextified variables require special treatment. First, a nextified
 *  variable enters the compound node through a ND_G_INPUT_NEXT rather
 *  than through a ND_G_INPUT. Second, a ND_NEXT node within the graph
 *  will be associated with the input. Finally, recall that a constant
 *  is not allowed as an input to a compound node, but rather should have
 *  its value propagated to the locations in the graph that use it. But
 *  in the case of a nextified variable that gets an initial value from
 *  a constant it is *not* correct to propagate the value into the graph.
 *  Instead, a dummy ND_CAST node created to feed the graph's nextified
 *  input. The cast is really a no-op since it does not change the constant's
 *  value, and it will be elided later by the constant-folding optimization.
 *  Also, note that after the DDCF graph is built, a pass is made to insert
 *  a ND_FEED_NEXT node outside of every nextified input of every graph.
 *
 *  Switch structures also need special treatment. The ND_G_OUTPUTs of
 *  a ND_SWITCH graph are the only places where multiple edges can target
 *  a single input port, and all the values that target a given ND_G_OUTPUT,
 *  and come from the output ports of the various ND_CASE graphs, must be of
 *  identical type. The type system has already determined the result type
 *  of each swicth value by applying the "type widening" rules across the
 *  various case values. When generating a ND_CASE graph, each of its return
 *  values must be cast, inside the ND_CASE graph, to the return type of
 *  the switch. Also note that any array size information from the output
 *  of a ND_CASE graph must not be carried down to the switch's output,
 *  since array size information from one case does not imply that other
 *  case outputs have the same size.
 *
 *  Finally, type information is annotated throughout the DDCF graph, and
 *  requires numerous calls to 'copy_type_info' during DDCF graph generation.
 */


#include "sassy.h"
#include "node.h"
#include "deftab.h"
#include "ddcf.h"
#include "error.h"

void ddcf2c_func (FuncGraph*);
void build_ddcf_func (NODEPNT, FuncGraph*);
void build_graph_stmt_list (NODEPNT, FuncGraph*);
void make_ddcf_print_assert (NODEPNT, FuncGraph*);
int alloc_ddcf_node (FuncGraph*, Location);
int new_ddcf_node (FuncGraph*, DdcfType, int, int, Location);
void push_id (StackItem*, int, int, int, TypeInfo*, int);
void push_const (int, char*, TypeInfo*);
AssocData *find_id (StackItem*, int, int*);
int find_port (StackItem*, int, int);
OutputPort *make_output_ports (int);
InputPort *make_input_ports (int);
void copy_type_info (TypeInfo*, TypeInfo*);
void fcall (NODEPNT, FuncGraph*, AssocData*);
void extents (NODEPNT, FuncGraph*, AssocData*);
void cast (NODEPNT, FuncGraph*, AssocData*);
void bodyret (NODEPNT, FuncGraph*, AssocData*, int);
void intrincall (NODEPNT, FuncGraph*, AssocData*);
void arrayref (NODEPNT, FuncGraph*, AssocData*);
void arr_slice (NODEPNT, FuncGraph*, AssocData*);
void for_loop (NODEPNT, FuncGraph*, AssocData*);
void while_loop (NODEPNT, FuncGraph*, AssocData*);
void switch_construct (NODEPNT, FuncGraph*, AssocData*);
void conditional (NODEPNT, FuncGraph*, AssocData*);
void case_body (NODEPNT, FuncGraph*, AssocData*, int, TypeInfo*);
void case_selectors (NODEPNT, FuncGraph*, int);
void for_loop_seq (NODEPNT, FuncGraph*, AssocData*);
void import_nextified_variable (StackItem*, NODEPNT, FuncGraph*);
void create_nextified_inputs (NODEPNT, FuncGraph*);
void for_loop_par (NODEPNT, FuncGraph*, AssocData*);
Assoc *find_another (Assoc*);
void create_next_nodes (FuncGraph*);
void generator (NODEPNT, FuncGraph*, NODEPNT);
void compound_prod (NODEPNT, FuncGraph*, DdcfType, NODEPNT);
int scalar_gen (NODEPNT, FuncGraph*);
int ele_gen (NODEPNT, FuncGraph*);
int slice_gen (NODEPNT, FuncGraph*);
int window_gen (NODEPNT, FuncGraph*);
void array_reduce (NODEPNT, FuncGraph*, AssocData*);
void array_reduce_mxx_ind (NODEPNT, FuncGraph*, AssocData*);
int array_reduce_generator (NODEPNT, FuncGraph*);
int array_reduce_generator_mxx_ind (NODEPNT, FuncGraph*);
void accum (NODEPNT, FuncGraph*, AssocData*);
void array_accum (NODEPNT, FuncGraph*, AssocData*);
int set_arr_def_inputs (FuncGraph*, NODEPNT, int, int);
void make_ddcf_assignment (NODEPNT, FuncGraph*);
void import_local_variable (StackItem*, NODEPNT, FuncGraph*, AssocData*);
void make_ddcf_mvr_expr (NODEPNT, FuncGraph*, AssocData*);
void make_ddcf_expr_1_1 (NODEPNT, FuncGraph*, AssocData*);
void make_ddcf_expr_2_1 (NODEPNT, FuncGraph*, AssocData*);
void arr_conperim (NODEPNT, FuncGraph*, AssocData*);
void loop_reduce (NODEPNT, FuncGraph*, AssocData*);
void handle_child (NODEPNT, int, int, FuncGraph*);
void handle_graph_input_nodes (FuncGraph*, int);
void handle_graph_io_nodes (NODEPNT, FuncGraph*, AssocData*, int);
void create_graph_output_nodes (NODEPNT, FuncGraph*, int);
void connect_output_nodes (int, FuncGraph*, AssocData*, int);
int ret_val_count (TypeInfo*);
int type_equal (TypeInfo*, TypeInfo*);
void make_target (FuncGraph*, int, int, int, int);
void copy_nodeport (NodePort*, NodePort*);
void make_uint_const_input (FuncGraph*, int, int, int);
void make_int_const_input (FuncGraph*, int, int, int);
void make_bool_const_input (FuncGraph*, int, int, int);
int has_nextified (NODEPNT);
int current_graph ();
void push_graph (int);
void pop_graph (int, FuncGraph*);
int push_input_node (StackItem*, int);
void push_to_my_nodes (StackItem*, int);
void free_assocs (Assoc*);
void free_inputs (InputItem*);
void install_back_edges (FuncGraph*);
void install_back_edges_for_node (FuncGraph*, int);
void while_predicate (NODEPNT, FuncGraph*);
void make_const_string_from_string (char*, char*, TypeInfo*);
void make_const_string_from_iconst (char*, int, TypeInfo*);
int targeting_nextified_input (FuncGraph*, int, int);
void fix_next_inputs (FuncGraph*, int);
void insert_feed_next (FuncGraph*, Edge*, IntList**);
void show_typeinfo (TypeInfo*);
void get_intrin_arity (Intrinsic, int*, int*);
int bit_width (int);
void attach_pragmas (NODEPNT, FuncGraph*, int);

FuncGraph *fgraphs = NULL;
StackItem *stack = NULL;

void build_ddcf (NODEPNT n)
    {
    NODEPNT m;
    FuncGraph *fg;
    int i, fdv;

    for (m=n; m!=NULL; m=m->link)
	{
	fdv = m->child[1]->Defval;

	SacMalloc (fg, FuncGraph*, sizeof (FuncGraph))
	fg->name = find_sym (deftable[fdv].symval);
	fg->params = deftable[fdv].funcinfo->paramtypes;
	fg->rets = deftable[fdv].funcinfo->rettypes;
	SacMalloc (fg->nodes, DdcfNode*, 5 * sizeof (DdcfNode))
	fg->nodes_allocated = 5;
	fg->nodes_used = 0;
	fg->link = fgraphs;
	fgraphs = fg;

	build_ddcf_func (m, fg);
	fix_next_inputs (fg, 0);
	install_back_edges (fg);
	}

    for (i=0; i<defcnt; i++)
        if (deftable[i].tinfo.kind == Func)
	    if (deftable[i].funcinfo->stat == StatDecl)
	        {
		SacMalloc (fg, FuncGraph*, sizeof (FuncGraph))
		fg->name = find_sym (deftable[i].symval);
		fg->params = deftable[i].funcinfo->paramtypes;
		fg->rets = deftable[i].funcinfo->rettypes;
		fg->nodes = NULL;
		fg->nodes_allocated = 0;
		fg->nodes_used = 0;
		fg->link = fgraphs;
		fgraphs = fg;
		}
    }

void build_ddcf_func (NODEPNT n, FuncGraph *fg)
    {
    TypeInfo *tt, *paramtypes, *rettypes;
    NODEPNT m;
    int new_id, gin, gout;
    int fdv, nparams, nreturns;
    int out_node;
    int i;

    fdv = n->child[1]->Defval;
    nparams = deftable[fdv].funcinfo->nparams;
    paramtypes = deftable[fdv].funcinfo->paramtypes;
    nreturns = deftable[fdv].funcinfo->nreturns;
    rettypes = deftable[fdv].funcinfo->rettypes;

    /* create this function's graph node */
    new_id = new_ddcf_node (fg, ND_FUNC, nparams, nreturns, n->loc);
    attach_pragmas (n->pragma, fg, new_id);
    push_graph (new_id);
    fg->nodes[new_id].My_inputs = make_io_array (nparams);
    fg->nodes[new_id].My_outputs = make_io_array (nreturns);

    /* create ND_G_INPUT nodes for the params, and push them onto stack */
    for (i=0, tt=paramtypes; i<nparams && tt!=NULL; i++, tt=tt->link)
        {
	copy_type_info (&(fg->nodes[new_id].inputs[i].ty), tt);

	gin = new_ddcf_node (fg, ND_G_INPUT, 0, 1, n->loc);
	push_to_my_nodes (stack, gin);
	fg->nodes[gin].Io_num = i;
	fg->nodes[gin].My_graph = new_id;
	copy_type_info (&(fg->nodes[gin].outputs[0].ty), tt);

	fg->nodes[new_id].My_inputs[i] = gin;

	push_id (stack, deftable[fdv].funcinfo->paramdvs[i],
		gin, 0, &(deftable[deftable[fdv].funcinfo->paramdvs[i]].tinfo), FALSE);
	}

    /* create ND_G_OUTPUT nodes for the returns */
    for (i=0, tt=rettypes; i<nreturns && tt!=NULL; i++, tt=tt->link)
        {
	copy_type_info (&(fg->nodes[new_id].outputs[i].ty), tt);
	fg->nodes[new_id].outputs[i].targets = NULL;

	gout = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, n->loc);
	fg->nodes[gout].Io_num = i;
	fg->nodes[gout].My_graph = new_id;
	fg->nodes[new_id].My_outputs[i] = gout;
	copy_type_info (&(fg->nodes[gout].inputs[0].ty), tt);
	}

    /* build a graph with the statement list */
    build_graph_stmt_list (n->child[3]->child[0], fg);

    /* handle the return values */
    for (m=n->child[3]->child[1], tt=rettypes, i=0; m!=NULL; m=m->link, tt=tt->link, i++)
        {
	out_node = fg->nodes[new_id].My_outputs[i];

	if (type_equal (tt, m->types))
            handle_child (m, 0, out_node, fg);
	else
	    {
	    int cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, m->loc);

	    /* take care of the cast node's output */
	    make_target (fg, out_node, 0, cast_node, 0);
	    copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), tt);

	    /* take care of the cast node's input */
	    handle_child (m, 0, cast_node, fg);

	    push_to_my_nodes (stack, cast_node);
	    }
	}

    pop_graph (new_id, fg);
    }

void build_graph_stmt_list (NODEPNT n, FuncGraph *fg)
    {
    NODEPNT m;

    for (m=n; m!=NULL; m=m->link)
	switch (m->nodetype)
	    {
	    case ASSIGN :
	        if (m->child[1]->nodetype != LOOP_INDICES)
	            make_ddcf_assignment (m, fg);
		break;
	    case PRINT :
	    case ASSERT :
		make_ddcf_print_assert (m, fg);
	        break;
	    default :
                break;
	    }
    }

void make_ddcf_print_assert (NODEPNT n, FuncGraph *fg)
    {
    NODEPNT m;
    int cnt, new_id, i;

    for (cnt=1, m=n->child[1]; m!=NULL; cnt++, m=m->link);

    if (n->nodetype == PRINT)
        new_id = new_ddcf_node (fg, ND_PRINT, cnt, 0, n->loc);
    else
        new_id = new_ddcf_node (fg, ND_ASSERT, cnt, 0, n->loc);

    handle_child (n->child[0], 0, new_id, fg);

    for (i=1, m=n->child[1]; m!=NULL; i++, m=m->link)
	switch (m->nodetype)
	    {
            case IDENT :
		handle_child (m, i, new_id, fg);
	        break;
	    case STRING :
    		fg->nodes[new_id].inputs[i].is_const = TRUE;
		fg->nodes[new_id].inputs[i].constval = copy_to_const (m->Strval);
    		fg->nodes[new_id].inputs[i].ty.stat = Okay;
	        break;
	    default :
		assert (0);
	    }

    push_to_my_nodes (stack, new_id);
    }

void make_ddcf_assignment (NODEPNT n, FuncGraph *fg)
    {
    NODEPNT m, r;
    TypeInfo *lhs_type;
    int cnt, i, j;
    AssocData *rets;

    /*
    printf ("entering 'make_ddcf_assignment', tree node %d\n", n); fflush (stdout);
    */

    for (r=n->child[0], m=n->child[1]; m!=NULL; m=m->link)
        {
	cnt = ret_val_count (m->types);
	SacMalloc (rets, AssocData*, cnt * sizeof (AssocData))

	make_ddcf_mvr_expr (m, fg, rets);

	for (i=0; i<cnt; i++)
	    {
	    if (r->nodetype == EMPTY)
		{
		r=r->link;
	        continue;
		}

	    lhs_type = &(deftable[r->Defval].tinfo);

	    if (rets[i].is_const)
		{
		if (type_equal (&(rets[i].ty), lhs_type))
		    {
	            push_const (r->Defval, rets[i].ref.constval, lhs_type);
		    }
		else
		    {
		    int cast_node;

		    /* need to typecast it */
    		    cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, m->loc);

		    fg->nodes[cast_node].inputs[0].is_const = TRUE;
		    fg->nodes[cast_node].inputs[0].constval = copy_to_const (rets[i].ref.constval);
    		    copy_type_info (&(fg->nodes[cast_node].inputs[0].ty), &(rets[i].ty));
    		    copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), lhs_type);

		    push_id (stack, r->Defval, cast_node,
		    		0, &(deftable[r->Defval].tinfo), FALSE);
	    	    push_to_my_nodes (stack, cast_node);
		    }
		}
	    else
		{
		int node, port;

		node = rets[i].ref.np.node;
		port = rets[i].ref.np.port;

		if (type_equal (&(fg->nodes[node].outputs[port].ty),
						&(deftable[r->Defval].tinfo)))
		    {
		    if (m->types->kind==Array)
		        {
			if (r->child[0]->nodetype != NEXT)
			    {
			    for (j=0; j<MAXRANK; j++)
			        if (deftable[r->Defval].tinfo.dim_sizes[j] >= 0)
				    if (rets[i].ty.dim_sizes[j] >= 0)
				        assert (deftable[r->Defval].tinfo.dim_sizes[j] ==
									rets[i].ty.dim_sizes[j]);
				    else
					{
				        rets[i].ty.dim_sizes[j] =
						deftable[r->Defval].tinfo.dim_sizes[j];
					fg->nodes[node].outputs[port].ty.dim_sizes[j] =
						deftable[r->Defval].tinfo.dim_sizes[j];
					}
			    }
			}

	            push_id (stack, r->Defval, node, port, &(rets[i].ty), FALSE);
		    }
		else
		    {
		    int new_id;

		    /* need to typecast it */
    		    new_id = new_ddcf_node (fg, ND_CAST, 1, 1, m->loc);

		    make_target (fg, new_id, 0, node, port);
    		    copy_type_info (&(fg->nodes[new_id].inputs[0].ty),
		    			&(fg->nodes[node].outputs[port].ty));
    		    copy_type_info (&(fg->nodes[new_id].outputs[0].ty), lhs_type);

		    /*  if this is a nextified assignment, we don't want to carry
		     *  the initial array size information into the new array,
		     *  because they are not necessarily the same size;
		     */
		    if (r->child[0]->nodetype == NEXT)
		        for (j=0; j<MAXRANK; j++)
			    fg->nodes[new_id].outputs[0].ty.dim_sizes[j] = -1;

		    push_id (stack, r->Defval, new_id, 0, &(deftable[r->Defval].tinfo), FALSE);
	    	    push_to_my_nodes (stack, new_id);
		    }
		}

	    r = r->link;
	    }

	SacFree (rets)
	}

    /*
    printf ("leaving 'make_ddcf_assignment'\n"); fflush (stdout);
    */
    }

void make_ddcf_mvr_expr (NODEPNT m, FuncGraph *fg, AssocData *rets)
    {
    AssocData *a;
    int loc;

    /*
    printf ("'make_ddcf_mvr_expr', node type '%s'\n", node_str (m->nodetype));
    fflush (stdout);
    */

    switch (m->nodetype)
        {
	case IDENT :
	    a = find_id (stack, m->Defval, &loc);
	    copy_type_info (&(rets[0].ty), &(a->ty));
	    rets[0].is_const = a->is_const;
	    if (a->is_const)
		{
		strcpy (rets[0].ref.constval, a->ref.constval);
		copy_type_info (&(rets[0].ty), &(a->ty));
		}
	    else if (loc)
		{
		copy_nodeport (&(rets[0].ref.np), &(a->ref.np));
		copy_type_info (&(rets[0].ty), &(a->ty));
		}
	    else
		import_local_variable (stack, m, fg, rets);
	    break;
	case TRUE_ND :
	    rets[0].is_const = TRUE;
	    strcpy (rets[0].ref.constval, "true");
	    copy_type_info (&(rets[0].ty), m->types);
	    break;
	case FALSE_ND :
	    rets[0].is_const = TRUE;
	    strcpy (rets[0].ref.constval, "false");
	    copy_type_info (&(rets[0].ty), m->types);
	    break;
	case FLOATNUM :
	case INTNUM :
	    rets[0].is_const = TRUE;
	    make_const_string_from_string (rets[0].ref.constval, m->Strval, m->types);
	    copy_type_info (&(rets[0].ty), m->types);
	    break;
	case ICONST :
	    rets[0].is_const = TRUE;
	    make_const_string_from_iconst (rets[0].ref.constval, m->Iconst, m->types);
	    copy_type_info (&(rets[0].ty), m->types);
	    break;
	case ARRAYREF :
	    arrayref (m, fg, rets);
	    break;
	case ARR_SLICE :
	    arr_slice (m, fg, rets);
	    break;
        case ARR_CONPERIM :
            arr_conperim (m, fg, rets);
            break;
        case ADD :
        case SUB :
        case MUL :
        case DIV :
        case MOD :
        case COMPLEX :
        case LT :
        case GT :
        case LE :
        case GE :
        case NEQ :
        case EQ :
        case AND :
        case OR :
        case BIT_AND :
        case BIT_OR :
        case BIT_EOR :
        case LEFT_SHIFT :
        case RIGHT_SHIFT :
	case ARR_CONCAT :
            make_ddcf_expr_2_1 (m, fg, rets);
            break;
	case REDUCE_SUM :
	case REDUCE_MIN :
	case REDUCE_MAX :
	case REDUCE_AND :
	case REDUCE_OR :
	case REDUCE_VAL_AT_FIRST_MAX :
	case REDUCE_VAL_AT_FIRST_MIN :
	case REDUCE_VAL_AT_LAST_MAX :
	case REDUCE_VAL_AT_LAST_MIN :
	case REDUCE_VAL_AT_MAXS :
	case REDUCE_VAL_AT_MINS :
	case REDUCE_PRODUCT :
	case REDUCE_MEAN :
	case REDUCE_ST_DEV :
	case REDUCE_MODE :
	case REDUCE_MEDIAN :
	case REDUCE_HIST :
	    loop_reduce (m, fg, rets);
	    break;
	case ACCUM :
	    accum (m, fg, rets);
	    break;
        case REAL :
        case IMAG :
        case NOT :
        case NEG :
	case CONSTRUCT_ARRAY:
	case CONSTRUCT_VECTOR:
	case CONSTRUCT_MATRIX:
	case CONSTRUCT_CUBE:
	case CONSTRUCT_TILE:
            make_ddcf_expr_1_1 (m, fg, rets);
            break;
	case CONSTRUCT_CONCAT:
	    if (m->child[1] != NULL)
                make_ddcf_expr_2_1 (m, fg, rets);
	    else
                make_ddcf_expr_1_1 (m, fg, rets);
            break;
	case FCALL :
	    fcall (m, fg, rets);
	    break;
	case INTRINCALL :
	    intrincall (m, fg, rets);
	    break;
	case EXTENTS :
	    extents (m, fg, rets);
	    break;
	case BODYRET :
	    bodyret (m, fg, rets, FALSE);
	    break;
	case CAST :
	    cast (m, fg, rets);
	    break;
	case FOR :
	    for_loop (m, fg, rets);
	    break;
	case WHILE :
	    while_loop (m, fg, rets);
	    break;
	case SWITCH :
	    switch_construct (m, fg, rets);
	    break;
	case CONDITIONAL :
	    conditional (m, fg, rets);
	    break;
	case ARR_REDUCE_SUM :
	case ARR_REDUCE_MIN :
	case ARR_REDUCE_MAX :
	case ARR_REDUCE_AND :
	case ARR_REDUCE_OR :
	case ARR_REDUCE_PRODUCT :
	case ARR_REDUCE_MEAN :
	case ARR_REDUCE_ST_DEV :
	case ARR_REDUCE_MODE :
	case ARR_REDUCE_MEDIAN :
	case ARR_REDUCE_HIST :
	    array_reduce (m, fg, rets);
	    break;
	case ARR_REDUCE_MAX_INDICES :
	case ARR_REDUCE_MIN_INDICES :
	    array_reduce_mxx_ind (m, fg, rets);
	    break;
	case ARR_ACCUM :
	    array_accum (m, fg, rets);
	    break;
	default :
	    assert (0);
	}

    /*
    printf ("leaving 'make_ddcf_mvr_expr'\n"); fflush (stdout);
    */
    }

void import_local_variable (StackItem *s,
			NODEPNT m, FuncGraph *fg, AssocData *rets)
    {
    AssocData *a;
    int gin, new_input_num, loc, cg;

    find_id (s, m->Defval, &loc);

    if (loc)
        return;

    import_local_variable (s->link, m, fg, rets);

    a = find_id (s, m->Defval, &loc);
    gin = new_ddcf_node (fg, ND_G_INPUT, 0, 1, m->loc);
    new_input_num = push_input_node (s, gin);
    cg = s->current_graph_id;
    fg->nodes[gin].Io_num = new_input_num;
    fg->nodes[gin].My_graph = cg;

    /* create an edge to the current graph */
    make_target (fg, cg, new_input_num, a->ref.np.node, a->ref.np.port);

    /* push an association so other refs to this IDENT use this ND_G_INPUT */
    push_id (s, m->Defval, gin, 0, &(a->ty), FALSE);

    copy_type_info (&(fg->nodes[gin].outputs[0].ty), &(a->ty));
    copy_type_info (&(rets[0].ty), &(a->ty));

    rets[0].ref.np.node = gin;
    rets[0].ref.np.port = 0;
    push_to_my_nodes (s, gin);
    }

void arr_conperim (NODEPNT m, FuncGraph *fg, AssocData *radt)
    {
    int new_id;

    new_id = new_ddcf_node (fg, ND_ARR_CONPERIM, 3, 1, m->loc);
    handle_child (m->child[0], 0, new_id, fg);
    handle_child (m->child[1], 1, new_id, fg);
    handle_child (m->child[2], 2, new_id, fg);

    copy_type_info (&(fg->nodes[new_id].outputs[0].ty), m->types);

    copy_type_info (&(radt[0].ty), m->types);
    radt[0].ref.np.node = new_id;
    radt[0].ref.np.port = 0;
    radt[0].is_const = FALSE;
    push_to_my_nodes (stack, new_id);
    }

void make_ddcf_expr_2_1 (NODEPNT m, FuncGraph *fg, AssocData *radt)
    {
    int new_id;

    switch (m->nodetype)
        {
        case CONSTRUCT_CONCAT :
	    new_id = new_ddcf_node (fg, ND_CONSTRUCT_CONCAT_MASKED, 2, 1, m->loc);
            break;
        case ADD :
	    new_id = new_ddcf_node (fg, ND_ADD, 2, 1, m->loc);
            break;
        case SUB :
	    new_id = new_ddcf_node (fg, ND_SUB, 2, 1, m->loc);
            break;
        case MUL :
	    new_id = new_ddcf_node (fg, ND_MUL, 2, 1, m->loc);
            break;
        case DIV :
	    new_id = new_ddcf_node (fg, ND_DIV, 2, 1, m->loc);
            break;
        case MOD :
	    new_id = new_ddcf_node (fg, ND_MOD, 2, 1, m->loc);
            break;
        case COMPLEX :
	    new_id = new_ddcf_node (fg, ND_COMPLEX, 2, 1, m->loc);
            break;
        case LT :
	    new_id = new_ddcf_node (fg, ND_LT, 2, 1, m->loc);
            break;
        case GT :
	    new_id = new_ddcf_node (fg, ND_GT, 2, 1, m->loc);
            break;
        case LE :
	    new_id = new_ddcf_node (fg, ND_LE, 2, 1, m->loc);
            break;
        case GE :
	    new_id = new_ddcf_node (fg, ND_GE, 2, 1, m->loc);
            break;
        case NEQ :
	    new_id = new_ddcf_node (fg, ND_NEQ, 2, 1, m->loc);
            break;
        case EQ :
	    new_id = new_ddcf_node (fg, ND_EQ, 2, 1, m->loc);
            break;
        case AND :
	    new_id = new_ddcf_node (fg, ND_AND, 2, 1, m->loc);
            break;
        case OR :
	    new_id = new_ddcf_node (fg, ND_OR, 2, 1, m->loc);
            break;
        case BIT_AND :
	    new_id = new_ddcf_node (fg, ND_BIT_AND, 2, 1, m->loc);
            break;
        case BIT_OR :
	    new_id = new_ddcf_node (fg, ND_BIT_OR, 2, 1, m->loc);
            break;
        case BIT_EOR :
	    new_id = new_ddcf_node (fg, ND_BIT_EOR, 2, 1, m->loc);
            break;
        case LEFT_SHIFT :
	    new_id = new_ddcf_node (fg, ND_LEFT_SHIFT, 2, 1, m->loc);
            break;
        case RIGHT_SHIFT :
	    new_id = new_ddcf_node (fg, ND_RIGHT_SHIFT, 2, 1, m->loc);
            break;
        case ARR_CONCAT :
	    new_id = new_ddcf_node (fg, ND_ARR_CONCAT, 2, 1, m->loc);
            break;
	default :
            break;
        }

    handle_child (m->child[0], 0, new_id, fg);
    handle_child (m->child[1], 1, new_id, fg);

    copy_type_info (&(fg->nodes[new_id].outputs[0].ty), m->types);

    copy_type_info (&(radt[0].ty), m->types);
    radt[0].ref.np.node = new_id;
    radt[0].ref.np.port = 0;
    radt[0].is_const = FALSE;
    push_to_my_nodes (stack, new_id);
    }

void make_ddcf_expr_1_1 (NODEPNT m, FuncGraph *fg, AssocData *radt)
    {
    int new_id;

    switch (m->nodetype)
        {
        case REAL :
	    new_id = new_ddcf_node (fg, ND_REAL, 1, 1, m->loc);
            break;
        case IMAG :
	    new_id = new_ddcf_node (fg, ND_IMAG, 1, 1, m->loc);
            break;
        case NOT :
	    new_id = new_ddcf_node (fg, ND_NOT, 1, 1, m->loc);
            break;
        case NEG :
	    new_id = new_ddcf_node (fg, ND_NEG, 1, 1, m->loc);
            break;
        case CONSTRUCT_ARRAY :
	case CONSTRUCT_VECTOR:
	case CONSTRUCT_MATRIX:
	case CONSTRUCT_CUBE:
	    new_id = new_ddcf_node (fg, ND_CONSTRUCT_ARRAY, 1, 1, m->loc);
            break;
	case CONSTRUCT_CONCAT :
	    new_id = new_ddcf_node (fg, ND_CONSTRUCT_CONCAT, 1, 1, m->loc);
	    break;
	case CONSTRUCT_TILE :
	    new_id = new_ddcf_node (fg, ND_CONSTRUCT_TILE, 1, 1, m->loc);
	    break;
	default :
            break;
        }

    handle_child (m->child[0], 0, new_id, fg);

    copy_type_info (&(fg->nodes[new_id].outputs[0].ty), m->types);

    copy_type_info (&(radt[0].ty), m->types);
    radt[0].ref.np.node = new_id;
    radt[0].ref.np.port = 0;
    radt[0].is_const = FALSE;
    push_to_my_nodes (stack, new_id);
    }

void loop_reduce (NODEPNT m, FuncGraph *fg, AssocData *radt)
    {
    int new_id;

    switch (m->nodetype)
        {
	case REDUCE_SUM :
	    new_id = new_ddcf_node (fg, ND_REDUCE_SUM, 2, 1, m->loc);
            break;
	case REDUCE_MIN :
	    new_id = new_ddcf_node (fg, ND_REDUCE_MIN, 2, 1, m->loc);
            break;
	case REDUCE_MAX :
	    new_id = new_ddcf_node (fg, ND_REDUCE_MAX, 2, 1, m->loc);
            break;
	case REDUCE_AND :
	    new_id = new_ddcf_node (fg, ND_REDUCE_AND, 2, 1, m->loc);
            break;
	case REDUCE_OR :
	    new_id = new_ddcf_node (fg, ND_REDUCE_OR, 2, 1, m->loc);
            break;
	case REDUCE_VAL_AT_LAST_MAX :
	    {
	    NODEPNT n;
	    int cnt;

	    for (n=m->child[2], cnt=0; n!=NULL; n=n->link, cnt++);
	    new_id = new_ddcf_node (fg, ND_REDUCE_VAL_AT_LAST_MAX, cnt+2, 1, m->loc);
	    fg->nodes[new_id].VecSize = cnt;
	    }
            break;
	case REDUCE_VAL_AT_LAST_MIN :
	    {
	    NODEPNT n;
	    int cnt;

	    for (n=m->child[2], cnt=0; n!=NULL; n=n->link, cnt++);
	    new_id = new_ddcf_node (fg, ND_REDUCE_VAL_AT_LAST_MIN, cnt+2, 1, m->loc);
	    fg->nodes[new_id].VecSize = cnt;
	    }
            break;
	case REDUCE_VAL_AT_FIRST_MAX :
	    {
	    NODEPNT n;
	    int cnt;

	    for (n=m->child[2], cnt=0; n!=NULL; n=n->link, cnt++);
	    new_id = new_ddcf_node (fg, ND_REDUCE_VAL_AT_FIRST_MAX, cnt+2, 1, m->loc);
	    fg->nodes[new_id].VecSize = cnt;
	    }
            break;
	case REDUCE_VAL_AT_FIRST_MIN :
	    {
	    NODEPNT n;
	    int cnt;

	    for (n=m->child[2], cnt=0; n!=NULL; n=n->link, cnt++);
	    new_id = new_ddcf_node (fg, ND_REDUCE_VAL_AT_FIRST_MIN, cnt+2, 1, m->loc);
	    fg->nodes[new_id].VecSize = cnt;
	    }
            break;
	case REDUCE_VAL_AT_MAXS :
	    {
	    NODEPNT n;
	    int cnt;

	    for (n=m->child[2], cnt=0; n!=NULL; n=n->link, cnt++);
	    new_id = new_ddcf_node (fg, ND_REDUCE_VAL_AT_MAXS, cnt+2, 1, m->loc);
	    fg->nodes[new_id].VecSize = cnt;
	    }
            break;
	case REDUCE_VAL_AT_MINS :
	    {
	    NODEPNT n;
	    int cnt;

	    for (n=m->child[2], cnt=0; n!=NULL; n=n->link, cnt++);
	    new_id = new_ddcf_node (fg, ND_REDUCE_VAL_AT_MINS, cnt+2, 1, m->loc);
	    fg->nodes[new_id].VecSize = cnt;
	    }
            break;
	case REDUCE_PRODUCT :
	    new_id = new_ddcf_node (fg, ND_REDUCE_PRODUCT, 2, 1, m->loc);
            break;
	case REDUCE_MEAN :
	    new_id = new_ddcf_node (fg, ND_REDUCE_MEAN, 2, 1, m->loc);
            break;
	case REDUCE_ST_DEV :
	    new_id = new_ddcf_node (fg, ND_REDUCE_ST_DEV, 2, 1, m->loc);
            break;
	case REDUCE_MODE :
	    new_id = new_ddcf_node (fg, ND_REDUCE_MODE, 2, 1, m->loc);
            break;
	case REDUCE_MEDIAN :
	    new_id = new_ddcf_node (fg, ND_REDUCE_MEDIAN, 2, 1, m->loc);
            break;
	case REDUCE_HIST :
	    new_id = new_ddcf_node (fg, ND_REDUCE_HIST, 3, 1, m->loc);
            break;
	default :
            break;
        }

    handle_child (m->child[0], 0, new_id, fg);

    switch (m->nodetype)
        {
	case REDUCE_VAL_AT_FIRST_MAX :
	case REDUCE_VAL_AT_FIRST_MIN :
	case REDUCE_VAL_AT_LAST_MAX :
	case REDUCE_VAL_AT_LAST_MIN :
	case REDUCE_VAL_AT_MAXS :
	case REDUCE_VAL_AT_MINS :
            {
	    NODEPNT n;
	    int i;

	    for (n=m->child[2], i=1; n!=NULL; n=n->link, i++)
	        {
	        handle_child (n, i, new_id, fg);
	        }

            if (m->child[1] != NULL)
                handle_child (m->child[1], i, new_id, fg);
            else
                make_bool_const_input (fg, new_id, i, 1);
	    }
	    break;
	default :
            if (m->child[1] != NULL)
                handle_child (m->child[1], 1, new_id, fg);
            else
                make_bool_const_input (fg, new_id, 1, 1);
	    break;
	}

    if (m->nodetype == REDUCE_HIST)
	handle_child (m->child[2], 2, new_id, fg);

    copy_type_info (&(fg->nodes[new_id].outputs[0].ty), m->types);

    copy_type_info (&(radt[0].ty), m->types);
    radt[0].ref.np.node = new_id;
    radt[0].ref.np.port = 0;
    radt[0].is_const = FALSE;
    push_to_my_nodes (stack, new_id);
    }

void accum (NODEPNT m, FuncGraph *fg, AssocData *radt)
    {
    int new_id;

    switch (m->child[0]->nodetype)
        {
	case REDUCE_SUM :
	    new_id = new_ddcf_node (fg, ND_ACCUM_SUM, 4, 1, m->loc);
            break;
	case REDUCE_MIN :
	    new_id = new_ddcf_node (fg, ND_ACCUM_MIN, 4, 1, m->loc);
            break;
	case REDUCE_MAX :
	    new_id = new_ddcf_node (fg, ND_ACCUM_MAX, 4, 1, m->loc);
            break;
	case REDUCE_AND :
	    new_id = new_ddcf_node (fg, ND_ACCUM_AND, 4, 1, m->loc);
            break;
	case REDUCE_OR :
	    new_id = new_ddcf_node (fg, ND_ACCUM_OR, 4, 1, m->loc);
            break;
	case REDUCE_PRODUCT :
	    new_id = new_ddcf_node (fg, ND_ACCUM_PRODUCT, 4, 1, m->loc);
            break;
	case REDUCE_MEAN :
	    new_id = new_ddcf_node (fg, ND_ACCUM_MEAN, 4, 1, m->loc);
            break;
	case REDUCE_ST_DEV :
	    new_id = new_ddcf_node (fg, ND_ACCUM_ST_DEV, 4, 1, m->loc);
            break;
	case REDUCE_MEDIAN :
	    new_id = new_ddcf_node (fg, ND_ACCUM_MEDIAN, 4, 1, m->loc);
            break;
	case REDUCE_HIST :
	    new_id = new_ddcf_node (fg, ND_ACCUM_HIST, 5, 1, m->loc);
            break;
	default :
            break;
        }

    handle_child (m->child[0]->child[0], 0, new_id, fg);
    handle_child (m->child[2], 1, new_id, fg);
    handle_child (m->child[1], 2, new_id, fg);

    if (m->child[0]->nodetype == REDUCE_HIST)
        {
        handle_child (m->child[0]->child[2], 3, new_id, fg);
	if (m->child[0]->child[1] != NULL)
	    handle_child (m->child[0]->child[1], 4, new_id, fg);
	else
            make_bool_const_input (fg, new_id, 4, 1);
	}
    else
        if (m->child[0]->child[1] != NULL)
	    handle_child (m->child[0]->child[1], 3, new_id, fg);
	else
	    make_bool_const_input (fg, new_id, 3, 1);

    copy_type_info (&(fg->nodes[new_id].outputs[0].ty), m->types);

    copy_type_info (&(radt[0].ty), m->types);
    radt[0].ref.np.node = new_id;
    radt[0].ref.np.port = 0;
    radt[0].is_const = FALSE;
    push_to_my_nodes (stack, new_id);
    }

void handle_child (NODEPNT n, int port, int new_id, FuncGraph *fg)
    {
    AssocData adt;

    make_ddcf_mvr_expr (n, fg, &adt);

    if (adt.is_const)
        {
	fg->nodes[new_id].inputs[port].is_const = TRUE;
	fg->nodes[new_id].inputs[port].constval = copy_to_const (adt.ref.constval);
	}
    else
	{
	make_target (fg, new_id, port, adt.ref.np.node, adt.ref.np.port);
	}

    copy_type_info (&(fg->nodes[new_id].inputs[port].ty), &(adt.ty));
    }

void push_id (StackItem *st, int dv, int node,
		int port, TypeInfo *ty, int nextified_input)
    {
    Assoc *tmp;
    int i;

    /*
    printf ("push_id defval %d, <%d.%d>\n", dv, node, port);
    fflush (stdout);
    */

    SacMalloc (tmp, Assoc*, sizeof (Assoc))
    tmp->dv = dv;
    tmp->dt.is_const = FALSE;
    tmp->dt.ref.np.node = node;
    tmp->dt.ref.np.port = port;
    copy_type_info (&(tmp->dt.ty), ty);

    if (nextified_input)
        for (i=0; i<MAXRANK; i++)
	    tmp->dt.ty.dim_sizes[i] = -1;

    tmp->link = st->assocs;
    st->assocs = tmp;
    (st->assoc_idx)++;
    }

void push_const (int dv, char *constval, TypeInfo *ty)
    {
    Assoc *tmp;

    SacMalloc (tmp, Assoc*, sizeof (Assoc))
    tmp->dv = dv;
    tmp->dt.is_const = TRUE;
    strcpy (tmp->dt.ref.constval, constval);
    copy_type_info (&(tmp->dt.ty), ty);
    tmp->link = stack->assocs;
    stack->assocs = tmp;
    (stack->assoc_idx)++;
    }

/* search for defval 'dv' and return pointer to its stack entry */
AssocData *find_id (StackItem *s, int dv, int *is_local)
    {
    StackItem *ss;
    Assoc *a;

    for (ss=s; ss!=NULL; ss=ss->link)
        for (a=ss->assocs; a!=NULL; a=a->link)
	    if (a->dv == dv)
	        {
		*is_local = (s==ss);
		return &(a->dt);
		}

    assert (0);
    }

/* search for node 'nd' with port 'pt' and return its defval */
int find_port (StackItem *s, int nd, int pt)
    {
    StackItem *ss;
    Assoc *a;

    for (ss=s; ss!=NULL; ss=ss->link)
        for (a=ss->assocs; a!=NULL; a=a->link)
	    if (!(a->dt.is_const))
	        if ((a->dt.ref.np.node == nd) && (a->dt.ref.np.port == pt))
		    return a->dv;

    return -1;
    }

void array_reduce_mxx_ind (NODEPNT m, FuncGraph *fg, AssocData *rets)
    {
    int new_id, gen_id, reduce, rank, i;

    new_id = alloc_ddcf_node (fg, m->loc);
    push_graph (new_id);
    fg->nodes[new_id].nodetype = ND_FORALL;
    attach_pragmas (m->pragma, fg, new_id);

    gen_id = array_reduce_generator_mxx_ind (m, fg);

    switch (m->nodetype)
        {
	case ARR_REDUCE_MAX_INDICES :
	    rank = m->child[0]->types->dims;
    	    reduce = new_ddcf_node (fg, ND_REDUCE_VAL_AT_MAXS, rank+2, 1, m->loc);
	    fg->nodes[reduce].VecSize = rank;
	    break;
	case ARR_REDUCE_MIN_INDICES :
	    rank = m->child[0]->types->dims;
    	    reduce = new_ddcf_node (fg, ND_REDUCE_VAL_AT_MINS, rank+2, 1, m->loc);
	    fg->nodes[reduce].VecSize = rank;
	    break;
	default :
            break;
	}

    /* connect the 'gen_id' node's outputs to the 'reduce' node's inputs */
    for (i=0; i<fg->nodes[gen_id].num_outputs; i++)
        {
	make_target (fg, reduce, i, gen_id, i);
	copy_type_info (&(fg->nodes[reduce].inputs[i].ty),
				&(fg->nodes[gen_id].outputs[i].ty));
	}

    /* if optional mask value is not there, we have to create a 'true' */
    if (m->child[1] == NULL)
        make_bool_const_input (fg, reduce, rank+1, 1);

    copy_type_info (&(fg->nodes[reduce].outputs[0].ty), m->types);
    push_to_my_nodes (stack, reduce);

    copy_type_info (&(rets[0].ty), m->types);
    rets[0].ref.np.node = reduce;
    rets[0].ref.np.port = 0;
    rets[0].is_const = FALSE;

    handle_graph_io_nodes (m, fg, rets, new_id);

    pop_graph (new_id, fg);
    push_to_my_nodes (stack, new_id);
    }

void array_reduce (NODEPNT m, FuncGraph *fg, AssocData *rets)
    {
    int new_id, gen_id, reduce, i;

    new_id = alloc_ddcf_node (fg, m->loc);
    push_graph (new_id);
    fg->nodes[new_id].nodetype = ND_FORALL;
    attach_pragmas (m->pragma, fg, new_id);

    gen_id = array_reduce_generator (m, fg);

    switch (m->nodetype)
        {
	case ARR_REDUCE_SUM :
    	    reduce = new_ddcf_node (fg, ND_REDUCE_SUM, 2, 1, m->loc);
	    break;
	case ARR_REDUCE_MIN :
    	    reduce = new_ddcf_node (fg, ND_REDUCE_MIN, 2, 1, m->loc);
	    break;
	case ARR_REDUCE_MAX :
    	    reduce = new_ddcf_node (fg, ND_REDUCE_MAX, 2, 1, m->loc);
	    break;
	case ARR_REDUCE_AND :
    	    reduce = new_ddcf_node (fg, ND_REDUCE_AND, 2, 1, m->loc);
	    break;
	case ARR_REDUCE_OR :
    	    reduce = new_ddcf_node (fg, ND_REDUCE_OR, 2, 1, m->loc);
	    break;
	case ARR_REDUCE_PRODUCT :
    	    reduce = new_ddcf_node (fg, ND_REDUCE_PRODUCT, 2, 1, m->loc);
	    break;
	case ARR_REDUCE_MEAN :
    	    reduce = new_ddcf_node (fg, ND_REDUCE_MEAN, 2, 1, m->loc);
	    break;
	case ARR_REDUCE_ST_DEV :
    	    reduce = new_ddcf_node (fg, ND_REDUCE_ST_DEV, 2, 1, m->loc);
	    break;
	case ARR_REDUCE_MODE :
    	    reduce = new_ddcf_node (fg, ND_REDUCE_MODE, 2, 1, m->loc);
	    break;
	case ARR_REDUCE_MEDIAN :
    	    reduce = new_ddcf_node (fg, ND_REDUCE_MEDIAN, 2, 1, m->loc);
	    break;
	case ARR_REDUCE_HIST :
    	    reduce = new_ddcf_node (fg, ND_REDUCE_HIST, 3, 1, m->loc);
	    break;
	default :
            break;
	}

    /* connect the 'gen_id' node's outputs to the 'reduce' node's inputs */
    for (i=0; i<fg->nodes[gen_id].num_outputs; i++)
        {
	make_target (fg, reduce, i, gen_id, i);
	copy_type_info (&(fg->nodes[reduce].inputs[i].ty),
				&(fg->nodes[gen_id].outputs[i].ty));
	}

    /* if optional mask value is not there, we have to create a 'true' */
    if (m->child[1] == NULL)
        make_bool_const_input (fg, reduce, 1, 1);

    if (m->nodetype == ARR_REDUCE_HIST)
        {
        handle_child (m->child[2], 2, reduce, fg);
	}

    copy_type_info (&(fg->nodes[reduce].outputs[0].ty), m->types);
    push_to_my_nodes (stack, reduce);

    copy_type_info (&(rets[0].ty), m->types);
    rets[0].ref.np.node = reduce;
    rets[0].ref.np.port = 0;
    rets[0].is_const = FALSE;

    handle_graph_io_nodes (m, fg, rets, new_id);

    pop_graph (new_id, fg);
    push_to_my_nodes (stack, new_id);
    }

int array_accum_generator (NODEPNT m, FuncGraph *fg)
    {
    int gen_id, elegen, labelgen, rank, i, maskgen, onode;

    if (m->child[0]->child[1] == NULL)
	{
        gen_id = new_ddcf_node (fg, ND_DOT_PROD, 2, 2, m->loc);
	fg->nodes[gen_id].My_outputs = make_io_array (2);
	}
    else
	{
        gen_id = new_ddcf_node (fg, ND_DOT_PROD, 3, 3, m->loc);
	fg->nodes[gen_id].My_outputs = make_io_array (3);
	}

    push_graph (gen_id);

    rank = m->child[0]->child[0]->types->dims;
    elegen = new_ddcf_node (fg, ND_ELE_GEN, rank+1, rank+1, m->loc);
    labelgen = new_ddcf_node (fg, ND_ELE_GEN, rank+1, rank+1, m->loc);

    for (i=0; i<rank; i++)
	{
        make_uint_const_input (fg, elegen, i+1, 1);
	set_type_to_uint (&(fg->nodes[elegen].outputs[i+1].ty), 32);
        make_uint_const_input (fg, labelgen, i+1, 1);
	set_type_to_uint (&(fg->nodes[labelgen].outputs[i+1].ty), 32);
	}

    push_to_my_nodes (stack, elegen);
    push_to_my_nodes (stack, labelgen);

    SacMalloc (fg->nodes[elegen].Reftypes, char*, rank * sizeof (char))
    SacMalloc (fg->nodes[labelgen].Reftypes, char*, rank * sizeof (char))
    for (i=0; i<rank; i++)
	{
	fg->nodes[elegen].Reftypes[i] = '~';
	fg->nodes[labelgen].Reftypes[i] = '~';
	}

    /* create subgraph for source array expression */
    handle_child (m->child[0]->child[0], 0, elegen, fg);
    copy_type_info (&(fg->nodes[elegen].outputs[0].ty), m->child[0]->child[0]->types);
    fg->nodes[elegen].outputs[0].ty.kind = Scalar;
    fg->nodes[elegen].outputs[0].ty.dims = 0;

    /* create subgraph for label array expression */
    handle_child (m->child[2], 0, labelgen, fg);
    copy_type_info (&(fg->nodes[labelgen].outputs[0].ty), m->child[2]->types);
    fg->nodes[labelgen].outputs[0].ty.kind = Scalar;
    fg->nodes[labelgen].outputs[0].ty.dims = 0;

    onode = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, m->loc);
    fg->nodes[onode].Io_num = 0;
    fg->nodes[onode].My_graph = gen_id;
    fg->nodes[gen_id].My_outputs[0] = onode;
    make_target (fg, onode, 0, elegen, 0);
    copy_type_info (&(fg->nodes[onode].inputs[0].ty),
    			&(fg->nodes[elegen].outputs[0].ty));
    copy_type_info (&(fg->nodes[gen_id].outputs[0].ty),
    			&(fg->nodes[elegen].outputs[0].ty));

    onode = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, m->loc);
    fg->nodes[onode].Io_num = 1;
    fg->nodes[onode].My_graph = gen_id;
    fg->nodes[gen_id].My_outputs[1] = onode;
    make_target (fg, onode, 0, labelgen, 0);
    copy_type_info (&(fg->nodes[onode].inputs[0].ty),
    			&(fg->nodes[labelgen].outputs[0].ty));
    copy_type_info (&(fg->nodes[gen_id].outputs[1].ty),
    			&(fg->nodes[labelgen].outputs[0].ty));

    /* create subgraph for mask array expression */
    if (m->child[0]->child[1] != NULL)
        {
        maskgen = new_ddcf_node (fg, ND_ELE_GEN, rank+1, rank+1, m->loc);
        push_to_my_nodes (stack, maskgen);

	SacMalloc (fg->nodes[maskgen].Reftypes, char*, rank * sizeof (char))
        for (i=0; i<rank; i++)
	    fg->nodes[maskgen].Reftypes[i] = '~';

        for (i=0; i<rank; i++)
            make_uint_const_input (fg, maskgen, i+1, 1);

	for (i=0; i<rank; i++)
	    set_type_to_uint (&(fg->nodes[maskgen].outputs[i+1].ty), 32);

        handle_child (m->child[0]->child[1], 0, maskgen, fg);
	copy_type_info (&(fg->nodes[maskgen].outputs[0].ty),
					m->child[0]->child[1]->types);
	fg->nodes[maskgen].outputs[0].ty.kind = Scalar;
	fg->nodes[maskgen].outputs[0].ty.dims = 0;
	onode = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, m->loc);
        fg->nodes[onode].Io_num = 2;
        fg->nodes[onode].My_graph = gen_id;
	fg->nodes[gen_id].My_outputs[2] = onode;
	make_target (fg, onode, 0, maskgen, 0);
	copy_type_info (&(fg->nodes[onode].inputs[0].ty),
			&(fg->nodes[maskgen].outputs[0].ty));
	copy_type_info (&(fg->nodes[gen_id].outputs[2].ty),
			&(fg->nodes[maskgen].outputs[0].ty));
	}

    handle_graph_input_nodes (fg, gen_id);

    pop_graph (gen_id, fg);
    push_to_my_nodes (stack, gen_id);

    return gen_id;
    }

int array_reduce_generator (NODEPNT m, FuncGraph *fg)
    {
    int gen_id, elegen, rank, i, maskgen, onode;

    if (m->child[1] == NULL)
	{
        gen_id = new_ddcf_node (fg, ND_DOT_PROD, 1, 1, m->loc);
	fg->nodes[gen_id].My_outputs = make_io_array (1);
	}
    else
	{
        gen_id = new_ddcf_node (fg, ND_DOT_PROD, 2, 2, m->loc);
	fg->nodes[gen_id].My_outputs = make_io_array (2);
	}

    push_graph (gen_id);

    rank = m->child[0]->types->dims;
    elegen = new_ddcf_node (fg, ND_ELE_GEN, rank+1, rank+1, m->loc);

    for (i=0; i<rank; i++)
	{
        make_uint_const_input (fg, elegen, i+1, 1);
	set_type_to_uint (&(fg->nodes[elegen].outputs[i+1].ty), 32);
	}

    SacMalloc (fg->nodes[elegen].Reftypes, char*, rank * sizeof (char))
    for (i=0; i<rank; i++)
	fg->nodes[elegen].Reftypes[i] = '~';

    /* create subgraph for source array expression */
    handle_child (m->child[0], 0, elegen, fg);
    copy_type_info (&(fg->nodes[elegen].outputs[0].ty), m->child[0]->types);
    fg->nodes[elegen].outputs[0].ty.kind = Scalar;
    fg->nodes[elegen].outputs[0].ty.dims = 0;

    onode = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, m->loc);
    fg->nodes[onode].Io_num = 0;
    fg->nodes[onode].My_graph = gen_id;
    fg->nodes[gen_id].My_outputs[0] = onode;
    make_target (fg, onode, 0, elegen, 0);
    copy_type_info (&(fg->nodes[onode].inputs[0].ty),
    			&(fg->nodes[elegen].outputs[0].ty));
    copy_type_info (&(fg->nodes[gen_id].outputs[0].ty),
    			&(fg->nodes[elegen].outputs[0].ty));

    /* create subgraph for mask array expression */
    if (m->child[1] != NULL)
        {
        maskgen = new_ddcf_node (fg, ND_ELE_GEN, rank+1, rank+1, m->loc);
        push_to_my_nodes (stack, maskgen);

	SacMalloc (fg->nodes[maskgen].Reftypes, char*, rank * sizeof (char))
        for (i=0; i<rank; i++)
	    fg->nodes[maskgen].Reftypes[i] = '~';

        for (i=0; i<rank; i++)
            make_uint_const_input (fg, maskgen, i+1, 1);

	for (i=0; i<rank; i++)
	    set_type_to_uint (&(fg->nodes[maskgen].outputs[i+1].ty), 32);

        handle_child (m->child[1], 0, maskgen, fg);
	copy_type_info (&(fg->nodes[maskgen].outputs[0].ty), m->child[1]->types);
	fg->nodes[maskgen].outputs[0].ty.kind = Scalar;
	fg->nodes[maskgen].outputs[0].ty.dims = 0;
	onode = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, m->loc);
	fg->nodes[onode].Io_num = 1;
	fg->nodes[onode].My_graph = gen_id;
	fg->nodes[gen_id].My_outputs[1] = onode;
	make_target (fg, onode, 0, maskgen, 0);
	copy_type_info (&(fg->nodes[onode].inputs[0].ty),
			&(fg->nodes[maskgen].outputs[0].ty));
	copy_type_info (&(fg->nodes[gen_id].outputs[1].ty),
			&(fg->nodes[maskgen].outputs[0].ty));
	}

    handle_graph_input_nodes (fg, gen_id);

    push_to_my_nodes (stack, elegen);

    pop_graph (gen_id, fg);
    push_to_my_nodes (stack, gen_id);

    return gen_id;
    }

int array_reduce_generator_mxx_ind (NODEPNT m, FuncGraph *fg)
    {
    int gen_id, elegen, rank, i, maskgen, onode;

    rank = m->child[0]->types->dims;

    if (m->child[1] == NULL)
	{
        gen_id = new_ddcf_node (fg, ND_DOT_PROD, 1, rank+1, m->loc);
	fg->nodes[gen_id].My_outputs = make_io_array (rank+1);
	}
    else
	{
        gen_id = new_ddcf_node (fg, ND_DOT_PROD, 2, rank+2, m->loc);
	fg->nodes[gen_id].My_outputs = make_io_array (rank+2);
	}

    push_graph (gen_id);

    elegen = new_ddcf_node (fg, ND_ELE_GEN, rank+1, rank+1, m->loc);

    push_to_my_nodes (stack, elegen);

    SacMalloc (fg->nodes[elegen].Reftypes, char*, rank * sizeof (char))
    for (i=0; i<rank; i++)
	fg->nodes[elegen].Reftypes[i] = '~';

    for (i=0; i<rank; i++)
        make_uint_const_input (fg, elegen, i+1, 1);

    /* create subgraph for source array expression */
    handle_child (m->child[0], 0, elegen, fg);
    copy_type_info (&(fg->nodes[elegen].outputs[0].ty), m->child[0]->types);
    fg->nodes[elegen].outputs[0].ty.kind = Scalar;
    fg->nodes[elegen].outputs[0].ty.dims = 0;

    onode = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, m->loc);
    fg->nodes[onode].Io_num = 0;
    fg->nodes[onode].My_graph = gen_id;
    fg->nodes[gen_id].My_outputs[0] = onode;
    make_target (fg, onode, 0, elegen, 0);
    copy_type_info (&(fg->nodes[onode].inputs[0].ty),
    			&(fg->nodes[elegen].outputs[0].ty));
    copy_type_info (&(fg->nodes[gen_id].outputs[0].ty),
    			&(fg->nodes[elegen].outputs[0].ty));

    for (i=0; i<rank; i++)
	{
	set_type_to_uint (&(fg->nodes[elegen].outputs[i+1].ty), 32);
	onode = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, m->loc);
	fg->nodes[onode].Io_num = i+1;
	fg->nodes[onode].My_graph = gen_id;
	fg->nodes[gen_id].My_outputs[i+1] = onode;
	make_target (fg, onode, 0, elegen, i+1);
	copy_type_info (&(fg->nodes[onode].inputs[0].ty),
			&(fg->nodes[elegen].outputs[i+1].ty));
        copy_type_info (&(fg->nodes[gen_id].outputs[i+1].ty),
    			&(fg->nodes[elegen].outputs[i+1].ty));
	}

    /* create subgraph for mask array expression */
    if (m->child[1] != NULL)
        {
        maskgen = new_ddcf_node (fg, ND_ELE_GEN, rank+1, rank+1, m->loc);
        push_to_my_nodes (stack, maskgen);

	SacMalloc (fg->nodes[maskgen].Reftypes, char*, rank * sizeof (char))
        for (i=0; i<rank; i++)
	    fg->nodes[maskgen].Reftypes[i] = '~';

        for (i=0; i<rank; i++)
	    {
            make_uint_const_input (fg, maskgen, i+1, 1);
	    set_type_to_uint (&(fg->nodes[maskgen].outputs[i+1].ty), 32);
	    }

        handle_child (m->child[1], 0, maskgen, fg);
	copy_type_info (&(fg->nodes[maskgen].outputs[0].ty), m->child[1]->types);
	fg->nodes[maskgen].outputs[0].ty.kind = Scalar;
	fg->nodes[maskgen].outputs[0].ty.dims = 0;
	onode = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, m->loc);
	fg->nodes[onode].Io_num = rank+1;
	fg->nodes[onode].My_graph = gen_id;
	fg->nodes[gen_id].My_outputs[rank+1] = onode;
	make_target (fg, onode, 0, maskgen, 0);
	copy_type_info (&(fg->nodes[onode].inputs[0].ty),
			&(fg->nodes[maskgen].outputs[0].ty));
	copy_type_info (&(fg->nodes[gen_id].outputs[rank+1].ty),
			&(fg->nodes[maskgen].outputs[0].ty));
	}

    handle_graph_input_nodes (fg, gen_id);

    pop_graph (gen_id, fg);
    push_to_my_nodes (stack, gen_id);

    return gen_id;
    }

void array_accum (NODEPNT m, FuncGraph *fg, AssocData *rets)
    {
    int new_id, gen_id, reduce, rank;

    rank = m->child[0]->child[0]->types->dims;

    new_id = alloc_ddcf_node (fg, m->loc);
    push_graph (new_id);
    fg->nodes[new_id].nodetype = ND_FORALL;
    attach_pragmas (m->pragma, fg, new_id);

    gen_id = array_accum_generator (m, fg);

    switch (m->child[0]->nodetype)
        {
	case ARR_REDUCE_SUM :
    	    reduce = new_ddcf_node (fg, ND_ACCUM_SUM, 4, 1, m->loc);
	    break;
	case ARR_REDUCE_MIN :
    	    reduce = new_ddcf_node (fg, ND_ACCUM_MIN, 4, 1, m->loc);
	    break;
	case ARR_REDUCE_MAX :
    	    reduce = new_ddcf_node (fg, ND_ACCUM_MAX, 4, 1, m->loc);
	    break;
	case ARR_REDUCE_AND :
    	    reduce = new_ddcf_node (fg, ND_ACCUM_AND, 4, 1, m->loc);
	    break;
	case ARR_REDUCE_OR :
    	    reduce = new_ddcf_node (fg, ND_ACCUM_OR, 4, 1, m->loc);
	    break;
	case ARR_REDUCE_PRODUCT :
    	    reduce = new_ddcf_node (fg, ND_ACCUM_PRODUCT, 4, 1, m->loc);
	    break;
	case ARR_REDUCE_MEAN :
    	    reduce = new_ddcf_node (fg, ND_ACCUM_MEAN, 4, 1, m->loc);
	    break;
	case ARR_REDUCE_ST_DEV :
    	    reduce = new_ddcf_node (fg, ND_ACCUM_ST_DEV, 4, 1, m->loc);
	    break;
	case ARR_REDUCE_MEDIAN :
    	    reduce = new_ddcf_node (fg, ND_ACCUM_MEDIAN, 4, 1, m->loc);
	    break;
	case ARR_REDUCE_HIST :
    	    reduce = new_ddcf_node (fg, ND_ACCUM_HIST, 5, 1, m->loc);
	    break;
	default :
            break;
	}

    /* connect generator's outputs to reduce node's inputs */
    make_target (fg, reduce, 0, gen_id, 0);
    copy_type_info (&(fg->nodes[reduce].inputs[0].ty),
				&(fg->nodes[gen_id].outputs[0].ty));
    make_target (fg, reduce, 1, gen_id, 1);
    copy_type_info (&(fg->nodes[reduce].inputs[1].ty),
				&(fg->nodes[gen_id].outputs[1].ty));
    if (m->child[0]->child[1] != NULL)
        {
	if (m->child[0]->nodetype == ARR_REDUCE_HIST)
	    {
    	    make_target (fg, reduce, 4, gen_id, 2);
    	    copy_type_info (&(fg->nodes[reduce].inputs[4].ty),
				&(fg->nodes[gen_id].outputs[2].ty));
	    }
	else
	    {
    	    make_target (fg, reduce, 3, gen_id, 2);
    	    copy_type_info (&(fg->nodes[reduce].inputs[3].ty),
				&(fg->nodes[gen_id].outputs[2].ty));
	    }
	}
    else
        {
	if (m->child[0]->nodetype == ARR_REDUCE_HIST)
	    make_bool_const_input (fg, reduce, 4, 1);
	else
	    make_bool_const_input (fg, reduce, 3, 1);
	}

    if (m->child[0]->nodetype == ARR_REDUCE_HIST)
        {
        handle_child (m->child[0]->child[2], 3, reduce, fg);
	}

    handle_child (m->child[1], 2, reduce, fg);

    copy_type_info (&(fg->nodes[reduce].outputs[0].ty), m->types);
    push_to_my_nodes (stack, reduce);

    copy_type_info (&(rets[0].ty), m->types);
    rets[0].ref.np.node = reduce;
    rets[0].ref.np.port = 0;
    rets[0].is_const = FALSE;

    handle_graph_io_nodes (m, fg, rets, new_id);

    pop_graph (new_id, fg);
    push_to_my_nodes (stack, new_id);
    }

void intrincall (NODEPNT m, FuncGraph *fg, AssocData *rets)
    {
    TypeInfo *tt;
    Intrinsic ty;
    NODEPNT n;
    int nparams, nreturns, new_id;
    int i;

    ty = m->child[0]->Intrinval;
    get_intrin_arity (ty, &nparams, &nreturns);

    /* create the node */
    new_id = new_ddcf_node (fg, ND_INTRINCALL, nparams, nreturns, m->loc);
    fg->nodes[new_id].Intrin_func = ty;

    for (i=0, n=m->child[1]; n!=NULL; n=n->link, i++)
        handle_child (n, i, new_id, fg);

    for (i=0, tt=m->types; tt!=NULL; i++, tt=tt->link)
	{
        copy_type_info (&(fg->nodes[new_id].outputs[i].ty), tt);

        copy_type_info (&(rets[i].ty), tt);
	rets[i].ref.np.node = new_id;
	rets[i].ref.np.port = i;
	rets[i].is_const = FALSE;
	}

    push_to_my_nodes (stack, new_id);
    }

void arr_slice (NODEPNT m, FuncGraph *fg, AssocData *radt)
    {
    int i, new_id, num_values, rank, *dims;

    rank = m->types->dims;
    SacMalloc (dims, int*, rank * sizeof (int))

    for (i=0, num_values=1; i<rank; i++)
	{
        num_values *= m->types->dim_sizes[i];
	dims[i] = m->types->dim_sizes[i];
	}

    /* create the node */
    new_id = new_ddcf_node (fg, ND_ARR_DEF, num_values, 1, m->loc);
    fg->nodes[new_id].DefRank = rank;
    fg->nodes[new_id].DefDims = dims;

    set_arr_def_inputs (fg, m, new_id, 0);

    copy_type_info (&(fg->nodes[new_id].outputs[0].ty), m->types);

    copy_type_info (&(radt[0].ty), m->types);
    radt[0].ref.np.node = new_id;
    radt[0].ref.np.port = 0;
    radt[0].is_const = FALSE;
    push_to_my_nodes (stack, new_id);
    }

int set_arr_def_inputs (FuncGraph *fg, NODEPNT n, int new_id, int port)
    {
    int np;

    if (n==NULL)
        return port;

    if (n->nodetype == ARR_SLICE)
	{
        np = set_arr_def_inputs (fg, n->child[0], new_id, port);
	if (n->link != NULL)
	    np = set_arr_def_inputs (fg, n->link, new_id, np);
	return np;
	}
    else
	{
        handle_child (n, port, new_id, fg);
	np = port+1;
	if (n->link != NULL)
	    np = set_arr_def_inputs (fg, n->link, new_id, np);
        return np;
	}
    }

void arrayref (NODEPNT m, FuncGraph *fg, AssocData *radt)
    {
    NODEPNT n;
    char *refs;
    int i, d, cnt, new_id, rank;
    int extents_id;

    rank = m->child[0]->types->dims;
    SacMalloc (refs, char*, rank * sizeof (char))

    /* determine the number of inputs */
    for (i=0, cnt=1, n=m->child[1]; n!=NULL; i++, n=n->link)
        if (n->nodetype == TRIPLE)
	    {
	    refs[i] = ':';
	    cnt += 3;
	    }
	else
	    {
	    refs[i] = '_';
	    cnt++;
	    }

    /* create the node */
    new_id = new_ddcf_node (fg, ND_ARRAYREF, cnt, 1, m->loc); 
    fg->nodes[new_id].Reftypes = refs;

    /* connect the source array to the first port */
    handle_child (m->child[0], 0, new_id, fg);

    /* create a ND_EXTENTS node if needed (it's needed if any TRIPLE
     * is missing its child[0])
     */
    for (n=m->child[1]; n!=NULL; n=n->link)
        if ((n->nodetype == TRIPLE) && (n->child[1] == NULL))
	    {
	    extents_id = new_ddcf_node (fg, ND_EXTENTS, 1, rank, n->loc);
	    handle_child (m->child[0], 0, extents_id, fg);

    	    push_to_my_nodes (stack, extents_id);

	    for (i=0; i<rank; i++)
	        set_type_to_uint (&(fg->nodes[extents_id].outputs[i].ty), 31);
	    break;
	    }

    /* do the array subscripts: exprs and triples */
    for (i=1, d=0, n=m->child[1]; n!=NULL; n=n->link, d++)
	{
	if (n->nodetype == TRIPLE)
            {
	    if (n->child[0] == NULL)
		make_uint_const_input (fg, new_id, i++, 0);
	    else
    	        handle_child (n->child[0], i++, new_id, fg);

	    if (n->child[1] == NULL)
	        {
		int add_id = new_ddcf_node (fg, ND_ADD, 2, 1, n->loc);
    	        push_to_my_nodes (stack, add_id);
		make_int_const_input (fg, add_id, 1, -1);
    		make_target (fg, add_id, 0, extents_id, d);
		copy_type_info (&(fg->nodes[add_id].inputs[0].ty),
					&(fg->nodes[extents_id].outputs[d].ty));
		make_target (fg, new_id, i++, add_id, 0);
		set_type_to_int (&(fg->nodes[add_id].outputs[0].ty), 32);
		set_type_to_int (&(fg->nodes[new_id].inputs[i-1].ty), 32);
		}
	    else
    	        handle_child (n->child[1], i++, new_id, fg);

	    if (n->child[2] == NULL)
		make_uint_const_input (fg, new_id, i++, 1);
	    else
    	        handle_child (n->child[2], i++, new_id, fg);
	    }
	else
	    {
    	    handle_child (n, i++, new_id, fg);
	    }
	}

    copy_type_info (&(fg->nodes[new_id].outputs[0].ty), m->types);

    copy_type_info (&(radt[0].ty), m->types);
    radt[0].ref.np.node = new_id;
    radt[0].ref.np.port = 0;
    radt[0].is_const = FALSE;
    push_to_my_nodes (stack, new_id);
    }

void for_loop (NODEPNT m, FuncGraph *fg, AssocData *rets)
    {

    if (has_nextified (m))
        for_loop_seq (m, fg, rets);
    else
        for_loop_par (m, fg, rets);
    }

void for_loop_par (NODEPNT m, FuncGraph *fg, AssocData *rets)
    {
    NODEPNT n, loop_indices=NULL;
    int new_id;

    new_id = alloc_ddcf_node (fg, m->loc);
    push_graph (new_id);
    fg->nodes[new_id].nodetype = ND_FORALL;
    attach_pragmas (m->pragma, fg, new_id);

    for (n=m->child[1]->child[0]; n!=NULL; n=n->link)
        if ((n->nodetype==ASSIGN) && (n->child[1]->nodetype==LOOP_INDICES))
	    {
	    loop_indices = n;
	    break;
	    }

    generator (m->child[0], fg, loop_indices);
    bodyret (m->child[1], fg, rets, FALSE);

    handle_graph_io_nodes (m, fg, rets, new_id);

    pop_graph (new_id, fg);
    push_to_my_nodes (stack, new_id);
    }

void for_loop_seq (NODEPNT m, FuncGraph *fg, AssocData *rets)
    {
    NODEPNT n, loop_indices=NULL;
    int new_id;

    new_id = alloc_ddcf_node (fg, m->loc);
    push_graph (new_id);
    fg->nodes[new_id].nodetype = ND_FORNXT;
    attach_pragmas (m->pragma, fg, new_id);

    for (n=m->child[1]->child[0]; n!=NULL; n=n->link)
        if ((n->nodetype==ASSIGN) && (n->child[1]->nodetype==LOOP_INDICES))
	    {
	    loop_indices = n;
	    break;
	    }

    generator (m->child[0], fg, loop_indices);

    /* create inputs for nextified variables */
    create_nextified_inputs (m->child[1]->child[0], fg);

    bodyret (m->child[1], fg, rets, TRUE);

    handle_graph_io_nodes (m, fg, rets, new_id);
    create_next_nodes (fg);

    pop_graph (new_id, fg);
    push_to_my_nodes (stack, new_id);
    }

void create_nextified_inputs (NODEPNT stmts, FuncGraph *fg)
    {
    NODEPNT n, nn;
    AssocData *a;
    int gin, new_input_num, loc, cg, i;

    for (n=stmts; n!=NULL; n=n->link)
	{
	if (n->nodetype == ASSIGN)
            for (nn=n->child[0]; nn!=NULL; nn=nn->link)
		{
	        if ((nn->nodetype == IDENT) && (nn->child[0]->nodetype == NEXT))
		    {
		    /* bring the variable down to the enclosing graph */
		    import_nextified_variable (stack->link, nn, fg);

    		    /* get reference from enclosing graph */
    		    a = find_id (stack->link, nn->Defval, &loc);

    		    /* make an input node to this graph */
    		    gin = new_ddcf_node (fg, ND_G_INPUT_NEXT, 0, 1, nn->loc);
    		    push_to_my_nodes (stack, gin);
    		    new_input_num = push_input_node (stack, gin);
    		    cg = stack->current_graph_id;
    		    fg->nodes[gin].Io_num = new_input_num;
    		    fg->nodes[gin].My_graph = cg;

    		    /* create an edge to the current graph */
		    if (a->is_const)
			{
			/* this is a hack; we haven't yet created the input ports
			 * for the 'cg' node, so we can't create a constant input
			 * to it; so we create a dummy cast node outside the graph
			 */
			int cast_nd = new_ddcf_node (fg, ND_CAST, 1, 1, nn->loc);
			push_to_my_nodes (stack->link, cast_nd);
	    		fg->nodes[cast_nd].inputs[0].is_const = TRUE;
			fg->nodes[cast_nd].inputs[0].constval = copy_to_const (a->ref.constval);
			copy_type_info (&(fg->nodes[cast_nd].inputs[0].ty),
						&(deftable[nn->Defval].tinfo));
			copy_type_info (&(fg->nodes[cast_nd].outputs[0].ty),
					&(fg->nodes[cast_nd].inputs[0].ty));
    		        make_target (fg, cg, new_input_num, cast_nd, 0);
			}
		    else
			{
    		        make_target (fg, cg, new_input_num,
						a->ref.np.node, a->ref.np.port);
			}

    		    /* push an association */
    		    push_id (stack, nn->Defval, gin, 0, &(deftable[nn->Defval].tinfo), TRUE);

    		    copy_type_info (&(fg->nodes[gin].outputs[0].ty),
		    			&(deftable[nn->Defval].tinfo));

		    for (i=0; i<MAXRANK; i++)
		        fg->nodes[gin].outputs[0].ty.dim_sizes[i] = -1;
		    }
		}
	}
    }

void import_nextified_variable (StackItem *s, NODEPNT m, FuncGraph *fg)
    {
    AssocData *a;
    int gin, new_input_num, loc, cg;

    find_id (s, m->Defval, &loc);

    if (loc)
        return;

    import_nextified_variable (s->link, m, fg);

    /* get reference from enclosing graph */
    a = find_id (s, m->Defval, &loc);
    if (a->is_const)
        return;

    /* make an input node to this graph */
    gin = new_ddcf_node (fg, ND_G_INPUT, 0, 1, m->loc);
    push_to_my_nodes (s, gin);
    new_input_num = push_input_node (s, gin);
    cg = s->current_graph_id;
    fg->nodes[gin].Io_num = new_input_num;
    fg->nodes[gin].My_graph = cg;

    /* create an edge to the current graph */
    make_target (fg, cg, new_input_num, a->ref.np.node, a->ref.np.port);

    /* push an association so other refs to this IDENT use this ND_G_INPUT */
    push_id (s, m->Defval, gin, 0, &(deftable[m->Defval].tinfo), FALSE);
    copy_type_info (&(fg->nodes[gin].outputs[0].ty),
    				&(deftable[m->Defval].tinfo));
    }

void while_loop (NODEPNT m, FuncGraph *fg, AssocData *rets)
    {
    int new_id;

    new_id = alloc_ddcf_node (fg, m->loc);
    push_graph (new_id);
    fg->nodes[new_id].nodetype = ND_WHILE;

    /* create inputs for nextified variables */
    create_nextified_inputs (m->child[1]->child[0], fg);

    while_predicate (m->child[0], fg);

    bodyret (m->child[1], fg, rets, TRUE);

    handle_graph_io_nodes (m, fg, rets, new_id);
    create_next_nodes (fg);

    pop_graph (new_id, fg);
    push_to_my_nodes (stack, new_id);
    }

void while_predicate (NODEPNT m, FuncGraph *fg)
    {
    AssocData a;
    int new_id;

    new_id = alloc_ddcf_node (fg, m->loc);
    push_graph (new_id);
    fg->nodes[new_id].nodetype = ND_WHILE_PRED;

    make_ddcf_mvr_expr (m, fg, &a);
    handle_graph_io_nodes (m, fg, &a, new_id);

    pop_graph (new_id, fg);
    push_to_my_nodes (stack, new_id);
    }

void conditional (NODEPNT m, FuncGraph *fg, AssocData *rets)
    {
    int new_id, pred_id;
    int i, num_outputs, sel_id, case_id;

    new_id = alloc_ddcf_node (fg, m->loc);
    push_graph (new_id);
    fg->nodes[new_id].nodetype = ND_SWITCH;

    create_graph_output_nodes (m, fg, new_id);
    num_outputs = fg->nodes[new_id].num_outputs;

    /* do the expression */
    pred_id = new_ddcf_node (fg, ND_SWITCH_KEY, 1, 0, m->loc);
    handle_child (m->child[0], 0, pred_id, fg);
    push_to_my_nodes (stack, pred_id);

    /* do the true part */
    case_id = alloc_ddcf_node (fg, m->child[1]->loc);
    push_graph (case_id);
    sel_id = new_ddcf_node (fg, ND_SELECTORS, 1, 0, m->child[1]->loc);
    make_bool_const_input (fg, sel_id, 0, 1);
    case_body (m->child[1], fg, rets, case_id, m->types);
    connect_output_nodes (num_outputs, fg, rets, new_id);

    push_to_my_nodes (stack, sel_id);
    pop_graph (case_id, fg);
    push_to_my_nodes (stack, case_id);

    /* do the false part */
    case_id = alloc_ddcf_node (fg, m->child[2]->loc);
    push_graph (case_id);
    case_body (m->child[2], fg, rets, case_id, m->types);
    connect_output_nodes (num_outputs, fg, rets, new_id);

    pop_graph (case_id, fg);
    push_to_my_nodes (stack, case_id);

    handle_graph_input_nodes (fg, new_id);

    for (i=0; i<num_outputs; i++)
        {
	rets[i].is_const = FALSE;
	rets[i].ref.np.node = new_id;
	rets[i].ref.np.port = i;
	}

    pop_graph (new_id, fg);
    push_to_my_nodes (stack, new_id);
    }

void switch_construct (NODEPNT m, FuncGraph *fg, AssocData *rets)
    {
    NODEPNT n;
    int new_id, key_id, case_id, num_outputs;
    int i;

    new_id = alloc_ddcf_node (fg, m->loc);
    push_graph (new_id);
    fg->nodes[new_id].nodetype = ND_SWITCH;

    create_graph_output_nodes (m, fg, new_id);
    num_outputs = fg->nodes[new_id].num_outputs;

    /* do the expression */
    key_id = new_ddcf_node (fg, ND_SWITCH_KEY, 1, 0, m->loc);
    handle_child (m->child[0], 0, key_id, fg);
    push_to_my_nodes (stack, key_id);

    /* do the cases */
    for (n=m->child[1]; n!=NULL; n=n->link)
	{
	case_id = alloc_ddcf_node (fg, n->loc);
	push_graph (case_id);
	case_selectors (n->child[0], fg, case_id);
	case_body (n->child[1], fg, rets, case_id, m->types);
        connect_output_nodes (num_outputs, fg, rets, new_id);

        pop_graph (case_id, fg);
        push_to_my_nodes (stack, case_id);
	}

    /* do the default */
    if (m->child[2] != NULL)
	{
	case_id = alloc_ddcf_node (fg, m->child[2]->loc);
	push_graph (case_id);
	case_body (m->child[2], fg, rets, case_id, m->types);
        connect_output_nodes (num_outputs, fg, rets, new_id);

        pop_graph (case_id, fg);
        push_to_my_nodes (stack, case_id);
	}

    handle_graph_input_nodes (fg, new_id);

    for (i=0; i<num_outputs; i++)
        {
	rets[i].is_const = FALSE;
	rets[i].ref.np.node = new_id;
	rets[i].ref.np.port = i;
	}

    pop_graph (new_id, fg);
    push_to_my_nodes (stack, new_id);
    }

void case_body (NODEPNT m, FuncGraph *fg, AssocData *rets, int case_id, TypeInfo *tys)
    {
    TypeInfo *ty;
    int i;

    fg->nodes[case_id].nodetype = ND_CASE;
    bodyret (m, fg, rets, FALSE);

    /* here we have to cast the CASE outputs to the SWITCH output types */
    for (ty=tys, i=0; ty!=NULL; ty=ty->link, i++)
	if (! type_equal (ty, &(rets[i].ty)))
            {
	    int cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, m->loc);

	    /* take care of the cast node's input */
	    if (rets[i].is_const)
	        {
		fg->nodes[cast_node].inputs[0].is_const = TRUE;
		fg->nodes[cast_node].inputs[0].constval = copy_to_const (rets[i].ref.constval);
		}
	    else
	        {
		fg->nodes[cast_node].inputs[0].is_const = FALSE;
		make_target (fg, cast_node, 0, rets[i].ref.np.node, rets[i].ref.np.port);
		}
	    copy_type_info (&(fg->nodes[cast_node].inputs[0].ty), &(rets[i].ty));
    	    push_to_my_nodes (stack, cast_node);

	    /* take care of the cast node's output */
	    rets[i].is_const = FALSE;
	    rets[i].ref.np.node = cast_node;
	    rets[i].ref.np.port = 0;
	    rets[i].ty = *ty;
	    copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), ty);
	    }

    handle_graph_io_nodes (m, fg, rets, case_id);
    }

void case_selectors (NODEPNT m, FuncGraph *fg, int case_graph)
    {
    NODEPNT n;
    int sel_id, cnt, i;
    char tmp[128];

    for (n=m, cnt=0; n!=NULL; n=n->link, cnt++);

    sel_id = new_ddcf_node (fg, ND_SELECTORS, cnt, 0, m->loc);

    for (n=m, i=0; n!=NULL; n=n->link, i++)
        {
	fg->nodes[sel_id].inputs[i].is_const = TRUE;
	sprintf (tmp, "%d", n->Iconst);
	fg->nodes[sel_id].inputs[i].constval = copy_to_const (tmp);
	copy_type_info (&(fg->nodes[sel_id].inputs[i].ty), n->types);
	}

    push_to_my_nodes (stack, sel_id);
    }

void create_next_nodes (FuncGraph *fg)
    {
    Assoc *a, *other;
    int new_id, other_id, i;

    for (a=stack->assocs; a!=NULL; a=a->link)
	{
	if ((other = find_another (a)) != NULL)
	    {
	    other_id = other->dt.ref.np.node;
	    if (fg->nodes[other_id].nodetype == ND_G_INPUT_NEXT)
	        {
	        new_id = new_ddcf_node (fg, ND_NEXT, 1, 0, fg->nodes[other_id].loc);
    		push_to_my_nodes (stack, new_id);
	        fg->nodes[new_id].In_next_id = other_id;

		if (a->dt.is_const)
		    {
		    fg->nodes[new_id].inputs[0].is_const = TRUE;
		    fg->nodes[new_id].inputs[0].constval = copy_to_const (a->dt.ref.constval);
		    }
		else
	            make_target (fg, new_id, 0, a->dt.ref.np.node, a->dt.ref.np.port);

		for (i=0; i<MAXRANK; i++)
		    fg->nodes[new_id].inputs[0].ty.dim_sizes[i] = -1;
		copy_type_info (&(fg->nodes[new_id].inputs[0].ty),
					&(fg->nodes[other_id].outputs[0].ty));
		}
	    }
        }
    }

Assoc *find_another (Assoc *a)
    {
    Assoc *aa;
    int dv;

    assert (a!=NULL);

    dv = a->dv;

    for (aa=a->link; aa!=NULL; aa=aa->link)
        if (aa->dv == dv)
	    return aa;

    return NULL;
    }


void handle_graph_io_nodes (NODEPNT m, FuncGraph *fg, AssocData *rets, int graph_id)
    {
    int i;
    int num_outputs;

    /* gather the ND_G_INPUT nodes */
    handle_graph_input_nodes (fg, graph_id);

    /* create the ND_G_OUTPUT nodes */
    create_graph_output_nodes (m, fg, graph_id);
    num_outputs = fg->nodes[graph_id].num_outputs;
    connect_output_nodes (num_outputs, fg, rets, graph_id);

    for (i=0; i<num_outputs; i++)
        {
	rets[i].is_const = FALSE;
	rets[i].ref.np.node = graph_id;
	rets[i].ref.np.port = i;
	}
    }

void handle_graph_input_nodes (FuncGraph *fg, int graph_id)
    {
    InputItem *ip;
    int num_inputs, i, input_id;

    num_inputs = stack->inputs_idx;
    fg->nodes[graph_id].num_inputs = num_inputs;
    fg->nodes[graph_id].inputs = make_input_ports (num_inputs);
    fg->nodes[graph_id].My_inputs = make_io_array (num_inputs);

    for (i=stack->inputs_idx-1, ip=stack->inputs; ip!=NULL; ip=ip->link, i--)
	{
	input_id = ip->input_id;
	fg->nodes[graph_id].My_inputs[i] = input_id;
	copy_type_info (&(fg->nodes[graph_id].inputs[i].ty),
				&(fg->nodes[input_id].outputs[0].ty));
	}
    }

void connect_output_nodes (int num_outputs,
			FuncGraph *fg, AssocData *rets, int graph_id)
    {
    int i, gout;

    for (i=0; i<num_outputs; i++)
        {
	gout = fg->nodes[graph_id].My_outputs[i];

	if (rets[i].is_const)
	    {
	    fg->nodes[gout].inputs[0].is_const = TRUE;
	    fg->nodes[gout].inputs[0].constval = copy_to_const (rets[i].ref.constval);
	    }
	else
	    {
	    make_target (fg, gout, 0, rets[i].ref.np.node, rets[i].ref.np.port);
	    }

	copy_type_info (&(fg->nodes[gout].inputs[0].ty), &(rets[i].ty));
	copy_type_info (&(fg->nodes[graph_id].outputs[i].ty), &(rets[i].ty));
	}
    }

void create_graph_output_nodes (NODEPNT m, FuncGraph *fg, int graph_id)
    {
    TypeInfo *tt;
    int i, gout, num_outputs;

    for (num_outputs=0, tt=m->types; tt!=NULL; tt=tt->link, num_outputs++);

    fg->nodes[graph_id].num_outputs = num_outputs;
    fg->nodes[graph_id].outputs = make_output_ports (num_outputs);
    fg->nodes[graph_id].My_outputs = make_io_array (num_outputs);
    for (i=0; i<num_outputs; i++)
        {
	gout = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, m->loc);
	fg->nodes[gout].Io_num = i;
	fg->nodes[gout].My_graph = current_graph ();
	fg->nodes[graph_id].My_outputs[i] = gout;
	}
    }

void generator (NODEPNT m, FuncGraph *fg, NODEPNT loop_indices)
    {

    switch (m->nodetype)
        {
	case SCALAR_GEN :
	case ELE_GEN :
	case SLICE_GEN :
	case WINDOW :
	    compound_prod (m, fg, ND_DOT_PROD, loop_indices);
	    break;
	case DOT_PROD :
	    compound_prod (m->child[0], fg, ND_DOT_PROD, loop_indices);
	    break;
	case CROSS_PROD :
	    compound_prod (m->child[0], fg, ND_CROSS_PROD, loop_indices);
	    break;
	default :
            break;
	}
    }

void compound_prod (NODEPNT m, FuncGraph *fg, DdcfType gtype, NODEPNT loop_indices)
    {
    NODEPNT n;
    int new_id, num_outputs=0, gout, i, j, c, gid, dv, lid;
    int gens[64], gen_idx=0;

    new_id = alloc_ddcf_node (fg, m->loc);
    push_graph (new_id);
    fg->nodes[new_id].nodetype = gtype;

    /* process each of the generators, saving their node ids */
    for (n=m; n!=NULL; n=n->link)
	{
        switch (n->nodetype)
	    {
	    case SCALAR_GEN :
	        gid = scalar_gen (n, fg);
	        break;
	    case ELE_GEN :
	        gid = ele_gen (n, fg);
	        break;
	    case SLICE_GEN :
	      gid = slice_gen (n, fg);
	      break;
	    case WINDOW :
	        gid = window_gen (n, fg);
	        break;
	    default :
		assert (0);
	    }

	gens[gen_idx++] = gid;
	num_outputs += fg->nodes[gid].num_outputs;
	}

    handle_graph_input_nodes (fg, new_id);

    if (loop_indices != NULL)
        {
	int cnt;

	for (n=loop_indices->child[0], cnt=0; n!=NULL; n=n->link, cnt++);
        lid = new_ddcf_node (fg, ND_LOOP_INDICES, 0, cnt, loop_indices->loc); 
        push_to_my_nodes (stack, lid);

        for (n=loop_indices->child[0], i=0; n!=NULL; n=n->link, i++)
	    {
            if (n->nodetype == IDENT)
	        {
	        push_id (stack, n->Defval, lid, i, &(deftable[n->Defval].tinfo), FALSE);
    	        copy_type_info (&(fg->nodes[lid].outputs[i].ty),
    					&(deftable[n->Defval].tinfo));
	        }
	    else
	        set_type_to_uint (&(fg->nodes[lid].outputs[i].ty), 32);
	    }

	num_outputs += cnt;
	}

    /* allocate outputs; this may be more than needed, since some generator
     * outputs may not be used and won't be brought out of the compound
     * node
     */
    fg->nodes[new_id].outputs = make_output_ports (num_outputs);
    fg->nodes[new_id].My_outputs = make_io_array (num_outputs);

    /* transfer all the associations out of the compound node; first the generators */
    for (i=0, c=0; i<gen_idx; i++)
	for (j=0; j<fg->nodes[gens[i]].num_outputs; j++)
            {
	    dv = find_port (stack, gens[i], j);
	    if (dv >= 0)
	        {
	        gout = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, m->loc);
	        fg->nodes[gout].Io_num = c;
	        fg->nodes[gout].My_graph = current_graph ();
	        fg->nodes[new_id].My_outputs[c] = gout;
	        make_target (fg, gout, 0, gens[i], j);
	        copy_type_info (&(fg->nodes[gout].inputs[0].ty),
	    			&(fg->nodes[gens[i]].outputs[j].ty));
	        copy_type_info (&(fg->nodes[new_id].outputs[c].ty),
	    			&(fg->nodes[gens[i]].outputs[j].ty));
	        push_id (stack->link, dv, new_id, c, &(deftable[dv].tinfo), FALSE);
	        c++;
	        }
	    }

    /* now the loop_indices, if they exist */
    if (loop_indices != NULL)
	for (j=0; j<fg->nodes[lid].num_outputs; j++)
            {
	    dv = find_port (stack, lid, j);
	    if (dv >= 0)
	        {
	        gout = new_ddcf_node (fg, ND_G_OUTPUT, 1, 0, loop_indices->loc);
	        fg->nodes[gout].Io_num = c;
	        fg->nodes[gout].My_graph = current_graph ();
	        fg->nodes[new_id].My_outputs[c] = gout;
	        make_target (fg, gout, 0, lid, j);
	        copy_type_info (&(fg->nodes[gout].inputs[0].ty),
	    			&(fg->nodes[lid].outputs[j].ty));
	        copy_type_info (&(fg->nodes[new_id].outputs[c].ty),
	    			&(fg->nodes[lid].outputs[j].ty));
	        push_id (stack->link, dv, new_id, c, &(deftable[dv].tinfo), FALSE);
	        c++;
	        }
	    }

    fg->nodes[new_id].num_outputs = c;

    pop_graph (new_id, fg);
    push_to_my_nodes (stack, new_id);
    }

int scalar_gen (NODEPNT m, FuncGraph *fg)
    {
    NODEPNT n, tg, src, step;
    int new_id, cnt, i, new_sz;;

    /* count the generators */
    for (n=m->child[0], cnt=0; n!=NULL; n=n->link, cnt++);

    new_id = new_ddcf_node (fg, ND_SCALAR_GEN, 3*cnt, cnt, m->loc);
    fg->nodes[new_id].Rank = cnt;

    for (tg=m->child[0], src=m->child[1], step=m->child[2], i=0;
    			tg!=NULL; tg=tg->link, src=src->link, i++)
	{
	if (src->nodetype != TILDE)
	    {
	    int add_id = new_ddcf_node (fg, ND_ADD, 2, 1, tg->loc);
	    handle_child (src, 0, add_id, fg);
    	    push_to_my_nodes (stack, add_id);
	    make_int_const_input (fg, add_id, 1, -1);
	    make_uint_const_input (fg, new_id, 3*i, 0);
	    make_target (fg, new_id, 3*i+1, add_id, 0);
	    new_sz = fg->nodes[add_id].inputs[0].ty.totsize;
	    if ((fg->nodes[add_id].inputs[0].ty.type==Uint) && (new_sz<32))
	        new_sz++;
	    set_type_to_int (&(fg->nodes[add_id].outputs[0].ty), new_sz);
	    copy_type_info (&(fg->nodes[new_id].inputs[3*i+1].ty),
	    			&(fg->nodes[add_id].outputs[0].ty));
	    }
	else
	    {
	    handle_child (src->child[0], 3*i, new_id, fg);
	    handle_child (src->child[1], 3*i+1, new_id, fg);
	    }

	if (step != NULL)
	    {
	    handle_child (step, 3*i+2, new_id, fg);
	    step = step->link;
	    }
	else
	    make_uint_const_input (fg, new_id, 3*i+2, 1);

	/* create an output */
	if (tg->nodetype == IDENT)
	    {
	    push_id (stack, tg->Defval, new_id, i, &(deftable[tg->Defval].tinfo), FALSE);
	    copy_type_info (&(fg->nodes[new_id].outputs[i].ty),
	    				&(deftable[tg->Defval].tinfo));
	    }
	else
	    set_type_to_uint (&(fg->nodes[new_id].outputs[i].ty), 32);
	}
    push_to_my_nodes (stack, new_id);

    return new_id;
    }

int ele_gen (NODEPNT m, FuncGraph *fg)
{
  NODEPNT n;
  char *rtypes;
  int new_id, rank, grank, i, j;

  rank = m->child[2]->types->dims;

  /* set the slice characteristics */
  SacMalloc (rtypes, char*, rank * sizeof (char))
    /* REmove later???? */
    for (i=0; i<rank; i++)
      rtypes[i] = '~';
  grank = rank;

  new_id = new_ddcf_node (fg, ND_ELE_GEN, grank+1, grank+1, m->loc);
  fg->nodes[new_id].Reftypes = rtypes;

  /* process the source array */
  handle_child (m->child[2], 0, new_id, fg);

  push_id (stack, m->child[0]->Defval, new_id,
	   0, &(deftable[m->child[0]->Defval].tinfo), FALSE);
  copy_type_info (&(fg->nodes[new_id].outputs[0].ty),
		  &(deftable[m->child[0]->Defval].tinfo));

  /* handle the 'step' inputs if they exist */
  if (m->child[3] == NULL)
    {
      for (i=0; i<grank; i++)
	make_uint_const_input (fg, new_id, i+1, 1);
    }
  else
    {
      for (n=m->child[3], i=1; n!=NULL; n=n->link)
	if (n->nodetype != EMPTY)
	  {
	    handle_child (n, i, new_id, fg);
	    i++;
	  }
    }

  /* handle the 'at' targets */
  if (m->child[4] == NULL)
    {
      for (i=0; i<grank; i++)
	set_type_to_uint (&(fg->nodes[new_id].outputs[i+1].ty), 32);
    }
  else
    {
      for (n=m->child[4], i=0, j=1; n!=NULL; n=n->link, i++)
	if (fg->nodes[new_id].Reftypes[i] == '~')
	  {
	    if (n->nodetype == IDENT)
	      {
		push_id (stack, n->Defval, new_id, j, 
			 &(deftable[n->Defval].tinfo), FALSE);
		copy_type_info (&(fg->nodes[new_id].outputs[j].ty),
				&(deftable[n->Defval].tinfo));
	      }
	    else
	      set_type_to_uint (&(fg->nodes[new_id].outputs[j].ty), 32);

	    j++;
	  }
    }

  push_to_my_nodes (stack, new_id);

  return new_id;
}

int slice_gen (NODEPNT m, FuncGraph *fg)
{
  NODEPNT n;
  char *rtypes;
  int new_id, rank, grank, i, j;

  rank = m->child[2]->types->dims;

  /* set the slice characteristics */
  SacMalloc (rtypes, char*, rank * sizeof (char))
    for (n=m->child[1], i=0, grank=0; n!=NULL; n=n->link, i++)
      if (n->nodetype == TILDE)
	{
	  rtypes[i] = '~';
	  grank++;
	}
      else
	rtypes[i] = ':';

  new_id = new_ddcf_node (fg, ND_SLICE_GEN, grank+1, grank+1, m->loc);
  fg->nodes[new_id].Reftypes = rtypes;

  /* process the source array */
  handle_child (m->child[2], 0, new_id, fg);

  push_id (stack, m->child[0]->Defval, new_id,
	   0, &(deftable[m->child[0]->Defval].tinfo), FALSE);
  copy_type_info (&(fg->nodes[new_id].outputs[0].ty),
		  &(deftable[m->child[0]->Defval].tinfo));

  /* handle the 'step' inputs if they exist */
  if (m->child[3] == NULL)
    {
      for (i=0; i<grank; i++)
	make_uint_const_input (fg, new_id, i+1, 1);
    }
  else
    {
      for (n=m->child[3], i=1; n!=NULL; n=n->link)
	if (n->nodetype != EMPTY)
	  {
	    handle_child (n, i, new_id, fg);
	    i++;
	  }
    }

  /* handle the 'at' targets */
  if (m->child[4] == NULL)
    {
      for (i=0; i<grank; i++)
	set_type_to_uint (&(fg->nodes[new_id].outputs[i+1].ty), 32);
    }
  else
    {
      for (n=m->child[4], i=0, j=1; n!=NULL; n=n->link, i++)
	if (fg->nodes[new_id].Reftypes[i] == '~')
	  {
	    if (n->nodetype == IDENT)
	      {
		push_id (stack, n->Defval, new_id, j, &(deftable[n->Defval].tinfo), FALSE);
		copy_type_info (&(fg->nodes[new_id].outputs[j].ty),
				&(deftable[n->Defval].tinfo));
	      }
	    else
	      set_type_to_uint (&(fg->nodes[new_id].outputs[j].ty), 32);

	    j++;
	  }
    }

  push_to_my_nodes (stack, new_id);

  return new_id;
}

int window_gen (NODEPNT m, FuncGraph *fg)
    {
    NODEPNT n, step;
    int new_id, rank, i;

    /* get the array's rank */
    rank = m->child[1]->types->dims;

    new_id = new_ddcf_node (fg, ND_WINDOW_GEN, 2*rank+1, rank+1, m->loc);

    handle_child (m->child[1], 0, new_id, fg);

    /* handle the window dimension and step inputs */
    for (n=m->child[0]->child[1], step=m->child[2], i=0; n!=NULL; n=n->link, i++)
        {
	handle_child (n, 2*i+1, new_id, fg);

	if (step != NULL)
	    {
	    handle_child (step, 2*i+2, new_id, fg);
	    step = step->link;
	    }
	else
	    make_uint_const_input (fg, new_id, 2*i+2, 1);
	}

    push_id (stack, m->child[0]->Defval, new_id,
    		0, &(deftable[m->child[0]->Defval].tinfo), FALSE);
    copy_type_info (&(fg->nodes[new_id].outputs[0].ty),
    					&(deftable[m->child[0]->Defval].tinfo));

    /* handle the 'at' targets */
    if (m->child[3] == NULL)
	for (i=0; i<rank; i++)
	    set_type_to_uint (&(fg->nodes[new_id].outputs[i+1].ty), 32);
    else
        for (n=m->child[3], i=1; n!=NULL; n=n->link, i++)
            if (n->nodetype == IDENT)
	        {
	        push_id (stack, n->Defval, new_id, i, &(deftable[n->Defval].tinfo), FALSE);
    	        copy_type_info (&(fg->nodes[new_id].outputs[i].ty),
    					&(deftable[n->Defval].tinfo));
	        }

    push_to_my_nodes (stack, new_id);

    return new_id;
    }

void fcall (NODEPNT m, FuncGraph *fg, AssocData *rets)
    {
    TypeInfo *tt, *ptypes;
    NODEPNT n;
    int fdv, nparams, nreturns, new_id;
    int i;

    fdv = m->child[0]->Defval;
    nparams = deftable[fdv].funcinfo->nparams;
    ptypes = deftable[fdv].funcinfo->paramtypes;
    nreturns = deftable[fdv].funcinfo->nreturns;

    /* create the node */
    if (deftable[fdv].funcinfo->pragma != NULL)
	{
        new_id = new_ddcf_node (fg, ND_VHDL_CALL, nparams, nreturns, m->loc);
	attach_pragmas (deftable[fdv].funcinfo->pragma, fg, new_id);
	}
    else
        new_id = new_ddcf_node (fg, ND_FCALL, nparams, nreturns, m->loc);

    fg->nodes[new_id].Sym = find_sym (deftable[fdv].symval);

    for (i=0, n=m->child[1], tt=ptypes; n!=NULL; n=n->link, i++, tt=tt->link)
	{
	if (type_equal (tt, n->types))
	    handle_child (n, i, new_id, fg);
	else
	    {
	    int cast_node = new_ddcf_node (fg, ND_CAST, 1, 1, n->loc);

	    /* take care of the cast node's output */
	    make_target (fg, new_id, i, cast_node, 0);
	    copy_type_info (&(fg->nodes[cast_node].outputs[0].ty), tt);

	    /* take care of the cast node's input */
	    handle_child (n, 0, cast_node, fg);
    	    push_to_my_nodes (stack, cast_node);
	    }

	copy_type_info (&(fg->nodes[new_id].inputs[i].ty), tt);
	}

    for (i=0, tt=m->types; tt!=NULL; i++, tt=tt->link)
	{
        copy_type_info (&(fg->nodes[new_id].outputs[i].ty), tt);

        copy_type_info (&(rets[i].ty), tt);
	rets[i].ref.np.node = new_id;
	rets[i].ref.np.port = i;
	rets[i].is_const = FALSE;
	}

    push_to_my_nodes (stack, new_id);
    }

void bodyret (NODEPNT m, FuncGraph *fg, AssocData *rets, int seq_loop)
    {
    NODEPNT n;
    TypeInfo *t;
    int i, j;

    build_graph_stmt_list (m->child[0], fg);

    for (i=0, n=m->child[1]; n!=NULL; n=n->link)
        {
	make_ddcf_mvr_expr (n, fg, rets+i);

	/*  if it's a sequential loop return, and this is an IDENT, then this
	 *  is the final value of a nextified variable; we need to eliminate
	 *  the array size information that came in from the deftable, because
	 *  the nextified size may not be the same as the initial size;
	 */
	if (seq_loop && n->nodetype == IDENT)
	    for (j=0; j<MAXRANK; j++)
	        rets[i].ty.dim_sizes[j] = -1;
	for (t=n->types; t!=NULL; t=t->link, i++);
	}
    }

void cast (NODEPNT m, FuncGraph *fg, AssocData *rets)
    {
    int new_id;

    /* create the node */
    new_id = new_ddcf_node (fg, ND_CAST, 1, 1, m->loc);

    handle_child (m->child[1], 0, new_id, fg);

    copy_type_info (&(fg->nodes[new_id].outputs[0].ty), m->types);

    copy_type_info (&(rets[0].ty), m->types);
    rets[0].ref.np.node = new_id;
    rets[0].ref.np.port = 0;
    rets[0].is_const = FALSE;
    push_to_my_nodes (stack, new_id);
    }

void extents (NODEPNT m, FuncGraph *fg, AssocData *rets)
    {
    TypeInfo *tt;
    int rank, new_id, i;

    rank = m->child[0]->types->dims;

    /* create the node */
    new_id = new_ddcf_node (fg, ND_EXTENTS, 1, rank, m->loc);

    handle_child (m->child[0], 0, new_id, fg);

    for (i=0, tt=m->types; tt!=NULL; i++, tt=tt->link)
	{
	copy_type_info (&(fg->nodes[new_id].outputs[i].ty), tt);

        copy_type_info (&(rets[i].ty), tt);
	rets[i].ref.np.node = new_id;
	rets[i].ref.np.port = i;
	rets[i].is_const = FALSE;
	}
    push_to_my_nodes (stack, new_id);
    }

int ret_val_count (TypeInfo *t)
    {
    int ac = 0;

    for (; t!=NULL; t=t->link)
	ac++;

    return ac;
    }

void make_target (FuncGraph *fg, int tgt_node,
				int tgt_port, int src_node, int src_port)
    {
    Edge *tmp;

    /*
    printf ("make_target: <%d.%d> ==> <%d.%d>\n", src_node, src_port, tgt_node, tgt_port);
    fflush (stdout);
    */

    /* create forward edge */
    SacMalloc (tmp, Edge*, sizeof (Edge))
    tmp->node = tgt_node;
    tmp->port = tgt_port;
    tmp->link = fg->nodes[src_node].outputs[src_port].targets;
    fg->nodes[src_node].outputs[src_port].targets = tmp;
    }

void copy_nodeport (NodePort *v1, NodePort *v2)
    {
    v1->node = v2->node;
    v1->port = v2->port;
    }

void make_int_const_input (FuncGraph *fg, int node, int port, int value)
    {
    char tmp[128];

    fg->nodes[node].inputs[port].is_const = TRUE;
    sprintf (tmp, "%d", value);
    fg->nodes[node].inputs[port].constval = copy_to_const (tmp);
    fg->nodes[node].inputs[port].ty.stat = Okay;
    fg->nodes[node].inputs[port].ty.type = Int;
    fg->nodes[node].inputs[port].ty.totsize = bit_width (value);
    fg->nodes[node].inputs[port].ty.fracsize = 0;
    fg->nodes[node].inputs[port].ty.kind = Scalar;
    fg->nodes[node].inputs[port].ty.dims = 0;
    fg->nodes[node].inputs[port].ty.link = NULL;
    }

void make_uint_const_input (FuncGraph *fg, int node, int port, int value)
    {
    char tmp[128];

    fg->nodes[node].inputs[port].is_const = TRUE;
    sprintf (tmp, "%u", value);
    fg->nodes[node].inputs[port].constval = copy_to_const (tmp);
    fg->nodes[node].inputs[port].ty.stat = Okay;
    fg->nodes[node].inputs[port].ty.type = Uint;
    fg->nodes[node].inputs[port].ty.totsize = bit_width (value);
    fg->nodes[node].inputs[port].ty.fracsize = 0;
    fg->nodes[node].inputs[port].ty.kind = Scalar;
    fg->nodes[node].inputs[port].ty.dims = 0;
    fg->nodes[node].inputs[port].ty.link = NULL;
    }

void make_bool_const_input (FuncGraph *fg, int node, int port, int value)
    {

    fg->nodes[node].inputs[port].is_const = TRUE;
    if (value)
	fg->nodes[node].inputs[port].constval = copy_to_const ("true");
    else
	fg->nodes[node].inputs[port].constval = copy_to_const ("false");
    fg->nodes[node].inputs[port].ty.stat = Okay;
    fg->nodes[node].inputs[port].ty.type = Bool;
    fg->nodes[node].inputs[port].ty.totsize = bit_width (value);
    fg->nodes[node].inputs[port].ty.fracsize = 0;
    fg->nodes[node].inputs[port].ty.kind = Scalar;
    fg->nodes[node].inputs[port].ty.dims = 0;
    fg->nodes[node].inputs[port].ty.link = NULL;
    }

int has_nextified (NODEPNT m)
    {
    NODEPNT n, nn;

    for (n=m->child[1]->child[0]; n!=NULL; n=n->link)
	if (n->nodetype == ASSIGN)
            for (nn=n->child[0]; nn!=NULL; nn=nn->link)
	        if (nn->nodetype == IDENT && nn->child[0]->nodetype == NEXT)
		    return TRUE;

    return FALSE;
    }

int current_graph ()
    {
    return stack->current_graph_id;
    }

void push_graph (int id)
    {
    StackItem *tmp;

    SacMalloc (tmp, StackItem*, sizeof (StackItem))
    tmp->current_graph_id = id;
    tmp->assocs = NULL;
    tmp->assoc_idx = 0;
    tmp->inputs = NULL;
    tmp->inputs_idx = 0;
    tmp->my_nodes = NULL;
    tmp->link = stack;
    stack = tmp;
    }

void pop_graph (int id, FuncGraph *fg)
    {
    StackItem *tmp;

    fg->nodes[id].My_nodes = stack->my_nodes;
    reverse_intlist (&(fg->nodes[id].My_nodes));

    tmp = stack;
    stack = tmp->link;

    free_assocs (tmp->assocs);
    free_inputs (tmp->inputs);
    SacFree (tmp)
    }

void free_assocs (Assoc *p)
    {
    Assoc *tmp;

    while (p != NULL)
        {
	tmp = p;
	p = p->link;
	SacFree (tmp)
	}
    }

void free_inputs (InputItem *p)
    {
    InputItem *tmp;

    while (p != NULL)
        {
	tmp = p;
	p = p->link;
	SacFree (tmp)
	}
    }

int push_input_node (StackItem *s, int id)
    {
    InputItem *tmp;

    SacMalloc (tmp, InputItem*, sizeof (InputItem))
    tmp->input_id = id;
    tmp->link = s->inputs;
    s->inputs = tmp;
    return (s->inputs_idx)++;
    }

void push_to_my_nodes (StackItem *s, int id)
    {
    IntList *tmp;

    SacMalloc (tmp, IntList*, sizeof (IntList))
    tmp->val = id;
    tmp->link = s->my_nodes;
    s->my_nodes = tmp;
    }

void make_const_string_from_string (char *tgt, char *src, TypeInfo *ty)
    {
    int val;
    double dval;

    switch (ty->type)
        {
	case Uint :
	case Int :
	    val = int_from_string (src);
	    sprintf (tgt, "%d", val);
	    break;
	case Ufix :
	case Fix :
	    if (is_float (src))
	        {
		sscanf (src, "%lf", &dval);
		val = (int)(dval * sc_pow2[ty->fracsize]);
		}
	    else
	        {
	        val = int_from_string (src);
	        val = val << ty->fracsize;
		}
	    val &= mask_ones[ty->totsize];
	    sprintf (tgt, "0x%x", val);
	    break;
	case Bits :
	    val = int_from_string (src);
	    val &= mask_ones[ty->totsize];
	    sprintf (tgt, "0x%x", val);
	    break;
	case Float :
	case Double :
	    strcpy (tgt, src);
	    break;
	default :
	    assert (0);
	}
    }

void make_const_string_from_iconst (char *tgt, int iconst, TypeInfo *ty)
    {
    int val;

    switch (ty->type)
        {
	case Uint :
	case Int :
	    val = iconst;
	    sprintf (tgt, "%d", val);
	    break;
	case Ufix :
	case Fix :
	    val = iconst;
	    val = val << ty->fracsize;
	    val &= mask_ones[ty->totsize];
	    sprintf (tgt, "0x%x", val);
	    break;
	case Bits :
	    val = iconst;
	    val &= mask_ones[ty->totsize];
	    sprintf (tgt, "0x%x", val);
	    break;
	case Float :
	case Double :
	    sprintf (tgt, "%d.0", iconst);
	    break;
	default :
	    assert (0);
	}
    }

/* 'id' is a graph node */
void fix_next_inputs (FuncGraph *fg, int id)
    {
    IntList *it;
    Edge *tg;
    int j, node, port, nd;

    for (it=fg->nodes[id].My_nodes; it!=NULL; it=it->link)
        {
	nd = it->val;

        switch (fg->nodes[nd].nodetype)
	    {
	    case ND_VOIDED :
	    case ND_FEED_NEXT :
	        break;
	    default :
		for (j=0; j<fg->nodes[nd].num_outputs; j++)
		    for (tg=fg->nodes[nd].outputs[j].targets; tg!=NULL; tg=tg->link)
		        {
			node = tg->node;
			port = tg->port;
			if (targeting_nextified_input (fg, node, port))
			    insert_feed_next (fg, tg, &(it->link));
			}
		break;
	    }

	/* recursively process graph nodes */
	switch (fg->nodes[nd].nodetype)
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
		fix_next_inputs (fg, nd);
		break;
	    default :
                break;
	    }
	}
    }

void insert_feed_next (FuncGraph *fg, Edge *tg, IntList **itt)
    {
    int nd;
    int node, port;

    node = tg->node;
    port = tg->port;
    nd = new_ddcf_node (fg, ND_FEED_NEXT, 1, 1, fg->nodes[node].loc);
    tg->node = nd;
    tg->port = 0;
    copy_type_info (&(fg->nodes[nd].inputs[0].ty), &(fg->nodes[node].inputs[port].ty));
    SacMalloc (fg->nodes[nd].outputs[0].targets, Edge*, sizeof (Edge))
    fg->nodes[nd].outputs[0].targets->node = node;
    fg->nodes[nd].outputs[0].targets->port = port;
    fg->nodes[nd].outputs[0].targets->link = NULL;
    copy_type_info (&(fg->nodes[nd].outputs[0].ty), &(fg->nodes[node].inputs[port].ty));
    add_to_list (nd, itt);
    }

int targeting_nextified_input (FuncGraph *fg, int nd, int port)
    {
    int in_node;

    if ((fg->nodes[nd].nodetype == ND_WHILE) || (fg->nodes[nd].nodetype == ND_FORNXT))
        {        in_node = fg->nodes[nd].My_inputs[port];

        if (fg->nodes[in_node].nodetype == ND_G_INPUT_NEXT)
            return TRUE;
        }

    return FALSE;
    }

void attach_pragmas (NODEPNT n, FuncGraph *fg, int id)
    {
    PragCell *tmp;
    NODEPNT m, mm;
    int i;

    for (m=n; m!=NULL; m=m->link)
	{
	SacMalloc (tmp, PragCell*, sizeof (PragCell))
	tmp->str = NULL;
	tmp->link = fg->nodes[id].pragmas;
	fg->nodes[id].pragmas = tmp;

        switch (m->nodetype)
	    {
	    case NO_INLINE :
		// printf ("found NO_INLINE\n"); fflush (stdout);
		tmp->prag = PRAG_NO_INLINE;
		break;
	    case NO_UNROLL :
		// printf ("found NO_UNROLL\n"); fflush (stdout);
		tmp->prag = PRAG_NO_UNROLL;
		break;
	    case NO_DFG :
		// printf ("found NO_DFG\n"); fflush (stdout);
		tmp->prag = PRAG_NO_DFG;
		break;
	    case LOOKUP :
		// printf ("found LOOKUP\n"); fflush (stdout);
		tmp->prag = PRAG_LOOKUP;
		break;
	    case STRIPMINE :
		// printf ("found STRIPMINE\n"); fflush (stdout);
	        tmp->prag = PRAG_STRIPMINE;
		for (mm=m->child[0], i=0; mm!=NULL; mm=mm->link, i++)
		    tmp->vals[i] = mm->Iconst;
		for ( ; i<MAXRANK; i++)
		    tmp->vals[i] = -1;
		break;
	    case BLOCK :
		// printf ("found BLOCK\n"); fflush (stdout);
	        tmp->prag = PRAG_BLOCK;
		for (mm=m->child[0], i=0; mm!=NULL; mm=mm->link, i++)
		    tmp->vals[i] = mm->Iconst;
		for ( ; i<MAXRANK; i++)
		    tmp->vals[i] = -1;
		break;
	    case NO_FUSE :
		// printf ("found NO_FUSE\n"); fflush (stdout);
		tmp->prag = PRAG_NO_FUSE;
		break;
	    case VHDL :
		// printf ("found VHDL\n"); fflush (stdout);
	        tmp->prag = PRAG_VHDL;
		SacMalloc (tmp->str, char*, 1 + strlen (m->child[0]->Strval));
		strcpy (tmp->str, m->child[0]->Strval);
		break;
	    default :
		assert (0);
	        break;
	    }
	}
    }
