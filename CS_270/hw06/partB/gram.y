%{
#include "sassy.h"
#include "node.h"
#include "gram.tab.h"
#include "error.h"


void yyerror (char *s)
    {
    handle_error (ErrParse, Fatal, mylineno, filename, funcname);
    }

void fill_in_func_name (char *funcname, NODEPNT n)
    {
    int i;

    n->loc.func = funcname;

    for (i=0; i<NUM_CHILDREN; i++)
        if (n->child[i] != NULL)
	    fill_in_func_name (funcname, n->child[i]);

    if (n->link != NULL)
        fill_in_func_name (funcname, n->link);
    }

NODEPNT revlist_elifs (NODEPNT n)
    {
    NODEPNT m, tmp;

    tmp = NULL;

    while (n != NULL)
	{
	m = n->child[1]->child[2];
	n->child[1]->child[2] = tmp;
	tmp = n;
	n = m;
	}

    return tmp;
    }

NODEPNT revlist (NODEPNT n)
    {
    NODEPNT m, tmp;

    tmp = NULL;

    while (n != NULL)
	{
	m = n->link;
	n->link = tmp;
	tmp = n;
	n = m;
	}

    return tmp;
    }

%}
%token TOK_RETURN TOK_DOT TOK_CROSS TOK_STEP TOK_AT TOK_NEXT TOK_REAL TOK_IMAG
%token TOK_ARRAY TOK_VECTOR TOK_MATRIX TOK_CUBE TOK_ARR_CONPERIM TOK_COMPLEX
%token TOK_SUM TOK_MIN TOK_MAX TOK_RED_AND TOK_RED_OR TOK_MEDIAN TOK_CONCAT TOK_TILE
%token TOK_EXTENTS TOK_IF TOK_ELSE TOK_ELIF TOK_STRING TOK_PRINT TOK_ASSERT
%token TOK_IDENT TOK_INTNUM TOK_FOR TOK_WHILE TOK_GEN TOK_AND TOK_OR
%token TOK_EQUAL TOK_NE TOK_LE TOK_GE TOK_INT TOK_BOOL TOK_AT TOK_FIX
%token TOK_TRUE TOK_FALSE TOK_RIGHT_SHIFT TOK_LEFT_SHIFT TOK_FLOATNUM
%token TOK_ARR_SUM TOK_ARR_MIN TOK_ARR_MAX TOK_ARR_RED_AND TOK_ARR_RED_OR
%token TOK_ARR_MEDIAN TOK_WINDOW TOK_SWITCH TOK_CASE TOK_DEFAULT
%token TOK_PRODUCT TOK_ARR_MAX_INDICES
%token TOK_ARR_MIN_INDICES TOK_ARR_PRODUCT TOK_MEAN TOK_MODE TOK_BITS
%token TOK_ARR_MEAN TOK_ARR_MODE TOK_ARR_HIST TOK_HIST TOK_ACCUM
%token TOK_ARR_ACCUM TOK_ST_DEV TOK_ARR_ST_DEV TOK_FLOAT TOK_DOUBLE
%token TOK_INTRINSIC TOK_ARR_CONCAT TOK_FINAL TOK_LOOP_INDICES TOK_VAL_AT_MINS TOK_VAL_AT_MAXS
%token TOK_VAL_AT_FIRST_MIN TOK_VAL_AT_FIRST_MAX TOK_VAL_AT_LAST_MIN TOK_VAL_AT_LAST_MAX
%token TOK_EXPORT TOK_PRAGMA TOK_NO_UNROLL TOK_NO_INLINE TOK_LOOKUP
%token TOK_NO_DFG TOK_STRIPMINE TOK_NO_FUSE TOK_BLOCK TOK_VHDL
%right '?' ':'
%left TOK_OR
%left TOK_AND
%left '|'
%left '^'
%left '&'
%nonassoc TOK_EQUAL TOK_NE
%nonassoc '<' '>' TOK_LE TOK_GE
%left TOK_RIGHT_SHIFT TOK_LEFT_SHIFT
%left '+' '-'
%left '*' '/' '%'
%right UMINUS '!'
%start program
%%
program		: exports funcs
			{
			prog_tree = revlist ($2);
			}
		;
exports		: 	/* empty */
		| TOK_EXPORT export_list ';'
		;
export_list	: TOK_IDENT
			{
			add_to_export_list ($1->Symval);
			}
		| export_list ',' TOK_IDENT
			{
			add_to_export_list ($3->Symval);
			}
		;
funcs		: func
			{ $$ = $1; }
		| funcs func
			{
			$2->link = $1;
			$$ = $2;
			}
		;
func		: opt_func_prag header '(' types_w_ids ')' body_w_return ';'
			{
			NODEPNT typs = $2->child[3];
			$2->child[3] = NULL;
			$$ = make_node (FUNC, typs, $2, revlist ($4),
				$6, NULL, NULL, $2->Lineno, $2->Srcfile, $2->Srcfunc);
			$$->pragma = $1;
			funcname = NULL;
			}
		| opt_func_prag header '(' ')' body_w_return ';'
			{
			NODEPNT typs = $2->child[3];
			$2->child[3] = NULL;
			$$ = make_node (FUNC, typs, $2, NULL, $5, NULL,
				NULL, $2->Lineno, $2->Srcfile, $2->Srcfunc);
			$$->pragma = $1;
			funcname = NULL;
			}
		| opt_func_prag header '(' types_wo_ids ')' ';'
			{
			NODEPNT typs = $2->child[3];
			$2->child[3] = NULL;
			$$ = make_node (FUNCDECL, typs, $2, revlist ($4),
				NULL, NULL, NULL, $2->Lineno, $2->Srcfile, $2->Srcfunc);
			$$->pragma = $1;
			funcname = NULL;
			}
		| opt_func_prag header '(' types_w_ids ')' ';'
			{
			NODEPNT nn;
			NODEPNT typs = $2->child[3];
			$2->child[3] = NULL;
			$$ = make_node (FUNCDECL, typs, $2, revlist ($4),
				NULL, NULL, NULL, $2->Lineno, $2->Srcfile, $2->Srcfunc);
			$$->pragma = $1;
			funcname = NULL;
			for (nn=$$->child[2]; nn!=NULL; nn=nn->link)
			    nn->nodetype = ATYPE;
			}
		| opt_func_prag header '(' ')' ';'
			{
			NODEPNT typs = $2->child[3];
			$2->child[3] = NULL;
			$$ = make_node (FUNCDECL, typs, $2, NULL,
				NULL, NULL, NULL, $2->Lineno, $2->Srcfile, $2->Srcfunc);
			$$->pragma = $1;
			funcname = NULL;
			}
		;
opt_func_prag	:	{ $$ = NULL; }
		| TOK_PRAGMA '(' func_pragmas ')'
			{ $$ = $3; }
		;
func_pragmas	: func_pragma
			{ $$ = $1; }
		| func_pragmas ',' func_pragma
			{
			$3->link = $1;
			$$ = $3;
			}
		;
func_pragma	: TOK_NO_INLINE
			{
			$$ = make_node (NO_INLINE, NULL, NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_LOOKUP
			{
			$$ = make_node (LOOKUP, NULL, NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_VHDL TOK_STRING
			{
			$$ = make_node (VHDL, NULL, NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			$$->child[0] = $2;
			}
		;
header		: types_wo_ids TOK_IDENT
			{
			funcname = find_sym ($2->Symval);
			fill_in_func_name (funcname, $1);
			$2->loc.func = funcname;
			$2->child[3] = revlist ($1);
			$$ = $2;
			}
		;
types_wo_ids	: type_wo_id
			{ $$ = $1; }
		| types_wo_ids ',' type_wo_id
			{
			$3->link = $1;
			$$ = $3;
			}
		;
type_wo_id	: type optbrackets
			{
			NODEPNT tmp;
			if ($2 != NULL)
			    {
			    tmp = make_node (ICONST, NULL, NULL, NULL,
					NULL, NULL, NULL, $1->Lineno, $1->Srcfile, $1->Srcfunc);
			    tmp->Iconst = list_length ($2);
			    }
			else
			    tmp = NULL;
			$$ = make_node (ATYPE, $1, $2, tmp, NULL, NULL,
				NULL, $1->Lineno, $1->Srcfile, $1->Srcfunc);
			}
		;
types_w_ids	: type_w_id
			{ $$ = $1; }
		| types_w_ids ',' type_w_id
			{
			$3->link = $1;
			$$ = $3;
			}
		;
type_w_id	: type TOK_IDENT optbrackets
			{
			NODEPNT tmp;
			if ($3 != NULL)
			    {
			    tmp = make_node (ICONST, NULL, NULL, NULL,
					NULL, NULL, NULL, $2->Lineno, $2->Srcfile, $2->Srcfunc);
			    tmp->Iconst = list_length ($3);
			    }
			else
			    tmp = NULL;
			$2->child[0] = $1;
			$2->child[1] = $3;
			$2->child[2] = tmp;
			$$ = $2;
			}
		;
base_type	: TOK_INT
			{ $$ = $1; }
		| TOK_FIX
			{ $$ = $1; }
		| TOK_BITS
			{ $$ = $1; }
		| TOK_BOOL
			{
			$$ = make_node (BOOLTYPE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->Totsize = 1;
			}
		| TOK_FLOAT
			{
			$$ = make_node (FLOATTYPE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->Totsize = 32;
			}
		| TOK_DOUBLE
			{
			$$ = make_node (DOUBLETYPE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->Totsize = 64;
			}
		;
type		: base_type
			{ $$ = $1; }
		| TOK_COMPLEX base_type
			{
			switch ($2->nodetype)
			    {
			    case INTTYPE :
			        $2->nodetype = CXINTTYPE;
				break;
			    case FIXTYPE :
			        $2->nodetype = CXFIXTYPE;
				break;
			    case FLOATTYPE :
			        $2->nodetype = CXFLOATTYPE;
				$2->Totsize = 32;
				break;
			    case DOUBLETYPE :
			        $2->nodetype = CXDOUBLETYPE;
				$2->Totsize = 64;
				break;
			    default :
			        handle_error (ErrParseComplexType, Fatal,
				    $2->Lineno, $2->Srcfile, $2->Srcfunc, $2->nodetype);
			    }
			$$ = $2;
			}
		;
optbrackets	:	{ $$ = NULL; }
		| '[' optsizes ']'
			{ $$ = revlist ($2); }
		;
optsizes	: optsize
			{ $$ = $1; }
		| optsizes ',' optsize
			{
			$3->link = $1;
			$$ = $3;
			}
		;
optsize		: ':'
			{
			$$ = make_node (COLON, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_INTNUM
			{
			char *tmp = $1->Strval;
			$1->nodetype = ICONST;
			$1->Iconst = int_from_string (tmp);
			free (tmp);
			$$ = $1;
			}
		;
for_loop	: opt_loop_prag TOK_FOR generators '{' stmts '}' TOK_RETURN '(' ret_exprs ')'
			{
			NODEPNT tmp;
			tmp = make_node (BODYRET, revlist ($5), revlist ($9), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$ = make_node (FOR, $3, tmp, NULL,
				NULL, NULL, NULL, $3->Lineno, $3->Srcfile, $3->Srcfunc);
			$$->pragma = $1;
			}
		| opt_loop_prag TOK_FOR generators TOK_RETURN '(' ret_exprs ')'
			{
			NODEPNT tmp;
			tmp = make_node (BODYRET, NULL, revlist ($6), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$ = make_node (FOR, $3, tmp, NULL,
				NULL, NULL, NULL, $3->Lineno, $3->Srcfile, $3->Srcfunc);
			$$->pragma = $1;
			}
		;
opt_loop_prag	:	{ $$ = NULL; }
		| TOK_PRAGMA '(' loop_pragmas ')'
			{ $$ = $3; }
		;
loop_pragmas	: loop_pragma
			{ $$ = $1; }
		| loop_pragmas ',' loop_pragma
			{
			$3->link = $1;
			$$ = $3;
			}
		;
loop_pragma	: TOK_NO_UNROLL
			{
			$$ = make_node (NO_UNROLL, NULL, NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_NO_DFG
			{
			$$ = make_node (NO_DFG, NULL, NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_STRIPMINE '(' const_list ')'
			{
			$$ = make_node (STRIPMINE, revlist ($3), NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_BLOCK '(' const_list ')'
			{
			$$ = make_node (BLOCK, revlist ($3), NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_NO_FUSE
			{
			$$ = make_node (NO_FUSE, NULL, NULL, NULL, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		;
generators	: simple_gen opt_cmpnds
			{
			if ($2 == NULL)
			    $$ = $1;
			else
			    {
			    $1->link = $2->child[0];
			    $2->child[0] = $1;
			    $$ = $2;
			    }
			}
opt_cmpnds	:	{ $$ = NULL; }
		| more_dots
			{
			NODEPNT tmp;
			tmp = revlist ($1);
			$$ = make_node (DOT_PROD, tmp, NULL, NULL,
				NULL, NULL, NULL, tmp->Lineno, tmp->Srcfile, tmp->Srcfunc);
			}
		| more_crosses
			{
			NODEPNT tmp;
			tmp = revlist ($1);
			$$ = make_node (CROSS_PROD, tmp, NULL, NULL,
				NULL, NULL, NULL, tmp->Lineno, tmp->Srcfile, tmp->Srcfunc);
			}
		;
more_dots	: TOK_DOT simple_gen
			{
			$$ = $2;
			}
		| more_dots TOK_DOT simple_gen
			{
			$3->link = $1;
			$$ = $3;
			}
		;
more_crosses	: TOK_CROSS simple_gen
			{
			$$ = $2;
			}
		| more_crosses TOK_CROSS simple_gen
			{
			$3->link = $1;
			$$ = $3;
			}
		;
simple_gen	: TOK_IDENT opt_extract TOK_GEN expr opt_elegen_step opt_at
			{
			$1->child[0] = make_node (UNKNOWNTYPE, NULL,
				NULL, NULL, NULL, NULL, NULL, $1->Lineno, $1->Srcfile, $1->Srcfunc);
			if ($2 != NULL)
			    {
			    NODEPNT n, *p;
			    int i, cnt;

			    /* count the COLONS */
			    for (cnt=0, n=$2; n!=NULL; n=n->link)
			        if (n->nodetype == COLON)
				    cnt++;

			    /* build a string of COLONs */
			    for (p=&($1->child[1]), i=0; i<cnt; i++)
			        {
				n = make_node (COLON, NULL, NULL, NULL,
					NULL, NULL, NULL, $1->Lineno, $1->Srcfile, $1->Srcfunc);
				*p = n;
				p = &(n->link);
				}

			    if (cnt > 0)
			        {
			        $1->child[2] = make_node (ICONST, NULL, NULL,
			    		NULL, NULL, NULL, NULL, $1->Lineno, $1->Srcfile, $1->Srcfunc);
			        $1->child[2]->Iconst = cnt;
			        $$ = make_node (SLICE_GEN, $1, $2, $4, $5, $6,
					    NULL, $1->Lineno, $1->Srcfile, $1->Srcfunc);
				}
			    else
			        $$ = make_node (ELE_GEN, $1, NULL, $4, $5, $6,
						NULL, $1->Lineno, $1->Srcfile,
						$1->Srcfunc);

			    }
			else
			  $$ = make_node (ELE_GEN, $1, NULL, $4, $5, $6,
					  NULL, $1->Lineno, $1->Srcfile, $1->Srcfunc);
			}
		| cl4 TOK_GEN '[' cl2 ']' opt_expr_step
			{
			$$ = make_node (SCALAR_GEN, revlist ($1), revlist ($4),
				$6, NULL, NULL, NULL, $1->Lineno, $1->Srcfile, $1->Srcfunc);
			}
		| TOK_WINDOW TOK_IDENT '[' cl7 ']' TOK_GEN expr opt_expr_step opt_at
			{
			NODEPNT n;
			$2->child[0] = make_node (UNKNOWNTYPE, NULL,
				NULL, NULL, NULL, NULL, NULL, $2->Lineno, $2->Srcfile, $2->Srcfunc);
			$2->child[1] = revlist ($4);

			for (n=$2->child[1]; n!=NULL; n=n->link)
			    if (n->nodetype == INTNUM)
			        {
				char *tmp = n->Strval;
				n->nodetype = ICONST;
				n->Iconst = int_from_string (tmp);
				free (tmp);
				}

			$2->child[2] = make_node (ICONST, NULL,
				NULL, NULL, NULL, NULL, NULL, $2->Lineno, $2->Srcfile, $2->Srcfunc);
			$2->child[2]->Iconst = list_length ($2->child[1]);
			$$ = make_node (WINDOW, $2, $7, $8, $9,
				NULL, NULL, $2->Lineno, $2->Srcfile, $2->Srcfunc);
			}
		;
opt_extract	:	{ $$ = NULL; }
		| '(' colon_twids ')'
			{ $$ = revlist ($2); }
		;
colon_twids	: colon_twid
			{ $$ = $1; }
		| colon_twids ',' colon_twid
			{
			$3->link = $1;
			$$ = $3;
			}
		;
colon_twid	: ':'
			{
			$$ = make_node (COLON, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| '~'
			{
			$$ = make_node (TILDE, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		;
opt_elegen_step	: 	{ $$ = NULL; }
		| TOK_STEP '(' opt_exprs ')'
			{ $$ = revlist ($3); }
		;
opt_exprs	: emp_expr
			{ $$ = $1; }
		| opt_exprs ',' emp_expr
			{
			$3->link = $1;
			$$ = $3;
			}
		;
emp_expr	: '_'
			{
			$$ = make_node (EMPTY, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr
			{ $$ = $1; }
		;
opt_expr_step	: 	{ $$ = NULL; }
		| TOK_STEP '(' exprs ')'
			{ $$ = revlist ($3); }
		;
opt_at		:	{ $$ = NULL; }
		| at_spec
			{ $$ = $1; }
		;
at_spec		: TOK_AT '(' cl4 ')'
			{ $$ = revlist ($3); }
		;
cl2		: ele2
			{ $$ = $1; }
		| cl2 ',' ele2
			{
			$3->link = $1;
			$$ = $3;
			}
		;
ele2		: expr '~' expr
			{
			$$ = make_node (TILDE, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr
			{ $$ = $1; }
		;
cl9		: ele9
			{ $$ = $1; }
		| cl9 ',' ele9
			{
			$3->link = $1;
			$$ = $3;
			}
		;
ele9		: type_w_id
			{ $$ = $1; }
		| TOK_NEXT TOK_IDENT
			{
			$2->child[0] = make_node (NEXT, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$ = $2;
			}
		| '_'
			{
			$$ = make_node (EMPTY, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		;
cl4		: ele4
			{ $$ = $1; }
		| cl4 ',' ele4
			{
			$3->link = $1;
			$$ = $3;
			}
		;
ele4		: type_w_id
			{ $$ = $1; }
		| '_'
			{
			$$ = make_node (EMPTY, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		;
cl7		: expr
			{ $$ = $1; }
		| cl7 ',' expr
			{
			$3->link = $1;
			$$ = $3;
			}
		;
ret_exprs	: ret_expr
			{ $$ = $1; }
		| ret_exprs ',' ret_expr
			{
			$3->link = $1;
			$$ = $3;
			}
		;
ret_expr	: TOK_FINAL '(' TOK_IDENT ')'
			{ $$ = $3; }
		| loop_reduction
			{ $$ = $1; }
		| struct_op '(' expr ')'
			{
			$1->child[0] = $3;
			$$ = $1;
			}
		| struct_op '(' expr ',' expr ')'
			{
			if ($1->nodetype != CONSTRUCT_CONCAT)
			    handle_error (ErrParseMaskOnStructOp, Fatal,
			    	$1->Lineno, $1->Srcfile, $1->Srcfunc);
			$1->child[0] = $3;
			$1->child[1] = $5;
			$$ = $1;
			}
		| TOK_ACCUM '(' loop_reduction ',' expr ',' expr ')'
			{
			$$ = make_node (ACCUM, $3, $5, $7,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			switch ($3->nodetype)
			    {
			    case REDUCE_VAL_AT_MINS :
			    case REDUCE_VAL_AT_MAXS :
			    case REDUCE_VAL_AT_FIRST_MIN :
			    case REDUCE_VAL_AT_FIRST_MAX :
			    case REDUCE_VAL_AT_LAST_MIN :
			    case REDUCE_VAL_AT_LAST_MAX :
			    case REDUCE_MODE :
			        handle_error (ErrParseBadReduceInAccum, Fatal,
			            $3->Lineno, $3->Srcfile, $3->Srcfunc);
			    default :
                                break;
			    }
			}
		;
loop_reduction	: reduce_op '(' expr ')'
			{
			if ($1->nodetype == REDUCE_HIST)
			    handle_error (ErrParseHistNeedsRange, Fatal,
			        $1->Lineno, $1->Srcfile, $1->Srcfunc);
			$1->child[0] = $3;
			$$ = $1;
			}
		| reduce_op '(' expr ',' expr ')'
			{
			if ($1->nodetype == REDUCE_HIST)
			    $1->child[2] = $5; /* range */
			else
			    $1->child[1] = $5; /* mask */
			$1->child[0] = $3;
			$$ = $1;
			}
		| reduce_op '(' expr ',' expr ',' expr ')'
			{
			if ($1->nodetype != REDUCE_HIST)
			    handle_error (ErrParseRangeOnlyForHist, Fatal,
			        $1->Lineno, $1->Srcfile, $1->Srcfunc);
			$1->child[0] = $3;
			$1->child[1] = $7;
			$1->child[2] = $5;
			$$ = $1;
			}
		| TOK_VAL_AT_MINS '(' expr ',' value_cluster ',' expr ')'
			{
			$$ = make_node (REDUCE_VAL_AT_MINS, $3, $7, $5, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_VAL_AT_MINS '(' expr ',' value_cluster ')'
			{
			$$ = make_node (REDUCE_VAL_AT_MINS, $3, NULL, $5, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_VAL_AT_MAXS '(' expr ',' value_cluster ',' expr ')'
			{
			$$ = make_node (REDUCE_VAL_AT_MAXS, $3, $7, $5, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_VAL_AT_MAXS '(' expr ',' value_cluster ')'
			{
			$$ = make_node (REDUCE_VAL_AT_MAXS, $3, NULL, $5, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_VAL_AT_FIRST_MIN '(' expr ',' value_cluster ',' expr ')'
			{
			$$ = make_node (REDUCE_VAL_AT_FIRST_MIN, $3, $7, $5, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_VAL_AT_FIRST_MIN '(' expr ',' value_cluster ')'
			{
			$$ = make_node (REDUCE_VAL_AT_FIRST_MIN, $3, NULL, $5, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_VAL_AT_FIRST_MAX '(' expr ',' value_cluster ',' expr ')'
			{
			$$ = make_node (REDUCE_VAL_AT_FIRST_MAX, $3, $7, $5, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_VAL_AT_FIRST_MAX '(' expr ',' value_cluster ')'
			{
			$$ = make_node (REDUCE_VAL_AT_FIRST_MAX, $3, NULL, $5, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_VAL_AT_LAST_MIN '(' expr ',' value_cluster ',' expr ')'
			{
			$$ = make_node (REDUCE_VAL_AT_LAST_MIN, $3, $7, $5, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_VAL_AT_LAST_MIN '(' expr ',' value_cluster ')'
			{
			$$ = make_node (REDUCE_VAL_AT_LAST_MIN, $3, NULL, $5, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_VAL_AT_LAST_MAX '(' expr ',' value_cluster ',' expr ')'
			{
			$$ = make_node (REDUCE_VAL_AT_LAST_MAX, $3, $7, $5, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		| TOK_VAL_AT_LAST_MAX '(' expr ',' value_cluster ')'
			{
			$$ = make_node (REDUCE_VAL_AT_LAST_MAX, $3, NULL, $5, NULL, NULL, NULL,
					mylineno, handle_filename (filename), funcname);
			}
		;
value_cluster	: '{' exprs '}'
			{
			$$ = revlist ($2);
			}
		;
arr_reduce_op	: opt_loop_prag TOK_ARR_SUM
			{
			$$ = make_node (ARR_REDUCE_SUM, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->pragma = $1;
			}
		| opt_loop_prag TOK_ARR_MIN
			{
			$$ = make_node (ARR_REDUCE_MIN, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->pragma = $1;
			}
		| opt_loop_prag TOK_ARR_MAX
			{
			$$ = make_node (ARR_REDUCE_MAX, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->pragma = $1;
			}
		| opt_loop_prag TOK_ARR_RED_AND
			{
			$$ = make_node (ARR_REDUCE_AND, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->pragma = $1;
			}
		| opt_loop_prag TOK_ARR_RED_OR
			{
			$$ = make_node (ARR_REDUCE_OR, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->pragma = $1;
			}
		| opt_loop_prag TOK_ARR_MEDIAN
			{
			$$ = make_node (ARR_REDUCE_MEDIAN, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->pragma = $1;
			}
		| opt_loop_prag TOK_ARR_MAX_INDICES
			{
			$$ = make_node (ARR_REDUCE_MAX_INDICES, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->pragma = $1;
			}
		| opt_loop_prag TOK_ARR_MIN_INDICES
			{
			$$ = make_node (ARR_REDUCE_MIN_INDICES, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->pragma = $1;
			}
		| opt_loop_prag TOK_ARR_PRODUCT
			{
			$$ = make_node (ARR_REDUCE_PRODUCT, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->pragma = $1;
			}
		| opt_loop_prag TOK_ARR_MEAN
			{
			$$ = make_node (ARR_REDUCE_MEAN, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->pragma = $1;
			}
		| opt_loop_prag TOK_ARR_ST_DEV
			{
			$$ = make_node (ARR_REDUCE_ST_DEV, NULL, NULL,
				NULL, NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->pragma = $1;
			}
		| opt_loop_prag TOK_ARR_MODE
			{
			$$ = make_node (ARR_REDUCE_MODE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->pragma = $1;
			}
		| opt_loop_prag TOK_ARR_HIST
			{
			$$ = make_node (ARR_REDUCE_HIST, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->pragma = $1;
			}
		| opt_loop_prag TOK_ARR_CONCAT
			{
			$$ = make_node (ARR_CONCAT, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$->pragma = $1;
			}
		;
reduce_op	: TOK_SUM
			{
			$$ = make_node (REDUCE_SUM, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_MIN
			{
			$$ = make_node (REDUCE_MIN, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_MAX
			{
			$$ = make_node (REDUCE_MAX, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_RED_AND
			{
			$$ = make_node (REDUCE_AND, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_RED_OR
			{
			$$ = make_node (REDUCE_OR, NULL, NULL, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_MEDIAN
			{
			$$ = make_node (REDUCE_MEDIAN, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_PRODUCT
			{
			$$ = make_node (REDUCE_PRODUCT, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_MEAN
			{
			$$ = make_node (REDUCE_MEAN, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_ST_DEV
			{
			$$ = make_node (REDUCE_ST_DEV, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_MODE
			{
			$$ = make_node (REDUCE_MODE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_HIST
			{
			$$ = make_node (REDUCE_HIST, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		;
struct_op	: TOK_ARRAY
			{
			$$ = make_node (CONSTRUCT_ARRAY, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_VECTOR
			{
			$$ = make_node (CONSTRUCT_VECTOR, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_MATRIX
			{
			$$ = make_node (CONSTRUCT_MATRIX, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_CUBE
			{
			$$ = make_node (CONSTRUCT_CUBE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_CONCAT
			{
			$$ = make_node (CONSTRUCT_CONCAT, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_TILE
			{
			$$ = make_node (CONSTRUCT_TILE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		;
stmts		:	{
			$$ = NULL;
			}
		| stmts stmt
			{
			if ($2 != NULL)
			    {
			    $2->link = $1;
			    $$ = $2;
			    }
			else
			    $$ = $1;
			}
		;
stmt		: cl9 '=' exprs ';'
			{
			$$ = make_node (ASSIGN, revlist ($1), revlist ($3), NULL,
				NULL, NULL, NULL, $2->Lineno, $2->Srcfile, $2->Srcfunc);
			}
		| cl9 '=' TOK_LOOP_INDICES '(' ')' ';'
			{
			NODEPNT m = make_node (LOOP_INDICES, NULL,
				NULL, NULL, NULL, NULL, NULL, mylineno,
				handle_filename (filename), funcname);
			$$ = make_node (ASSIGN, revlist ($1), m, NULL,
				NULL, NULL, NULL, $2->Lineno, $2->Srcfile, $2->Srcfunc);
			}
		| cl9 '=' arr_slice ';'
			{
			NODEPNT m;
			for (m=$1->child[1]; m!=NULL; m=m->link)
			    if (m->nodetype != ICONST)
			        {
			        handle_error (ErrParseArrayDefNeedsSize,
			    		Fatal, $1->Lineno, $1->Srcfile, $1->Srcfunc);
				break;
				}
			$$ = make_node (ASSIGN, $1, $3, NULL,
				NULL, NULL, NULL, $1->Lineno, $1->Srcfile, $1->Srcfunc);
			}
		| TOK_PRINT '(' expr ',' pr_entities ')' ';'
			{
			$$ = make_node (PRINT, $3, revlist ($5), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_ASSERT '(' expr ',' pr_entities ')' ';'
			{
			$$ = make_node (ASSERT, $3, revlist ($5), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| error ';'
			{ $$ = NULL; }
		;
pr_entities	: pr_entity
			{ $$ = $1; }
		| pr_entities ',' pr_entity
			{
			$3->link = $1;
			$$ = $3;
			}
		;
pr_entity	: TOK_IDENT
			{ $$ = $1; }
		| TOK_STRING
			{ $$ = $1; }
		;
exprs		: expr
			{ $$ = $1; }
		| exprs ',' expr
			{
			$3->link = $1;
			$$ = $3;
			}
		;
expr		: TOK_IDENT '(' exprs ')'
			{
			$$ = make_node (FCALL, $1, revlist ($3), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_INTRINSIC '(' exprs ')'
			{
			$$ = make_node (INTRINCALL, $1, revlist ($3), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_ARR_CONPERIM '(' expr ',' expr ',' expr ')'
			{
			$$ = make_node (ARR_CONPERIM, $3, $5, $7,
				NULL, NULL, NULL, $3->Lineno, $3->Srcfile, $3->Srcfunc);
			}
		| TOK_EXTENTS '(' TOK_IDENT ')'
			{
			$$ = make_node (EXTENTS, $3, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| '{' stmt stmts '}' TOK_RETURN '(' exprs ')'
			{
			if ($2 != NULL)
			    $2->link = revlist ($3);
			$$ = make_node (BODYRET, $2, revlist ($7), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_IDENT '(' ')'
			{
			$$ = make_node (FCALL, $1, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_IDENT
			{ $$ = $1; }
		| TOK_IDENT '[' array_indices ']'
			{
			$$ = make_node (ARRAYREF, $1, revlist ($3), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_INTNUM
			{ $$ = $1; }
		| TOK_FLOATNUM
			{ $$ = $1; }
		| TOK_TRUE
			{
			$$ = make_node (TRUE_ND, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_FALSE
			{
			$$ = make_node (FALSE_ND, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| arr_reduce_expr
			{ $$ = $1; }
		| switch
			{ $$ = $1; }
		| TOK_ARR_ACCUM '(' arr_reduce_expr ',' expr ',' expr ')'
			{
			$$ = make_node (ARR_ACCUM, $3, $5, $7,
						NULL, NULL, NULL, $3->Lineno, $3->Srcfile, $3->Srcfunc);
			switch ($3->nodetype)
			    {
			    case ARR_REDUCE_MAX_INDICES :
			    case ARR_REDUCE_MIN_INDICES :
			    case ARR_REDUCE_MODE :
			        handle_error (ErrParseBadReduceInAccum,
			    		Fatal, $3->Lineno, $3->Srcfile, $3->Srcfunc);
			    default :
                                break;
			    }
			}
		| '(' expr ',' expr ')'
			{
			$$ = make_node (COMPLEX, $2, $4, NULL,
				NULL, NULL, NULL, $2->Lineno, $2->Srcfile, $2->Srcfunc);
			}
		| TOK_REAL '(' expr ')'
			{
			$$ = make_node (REAL, $3, NULL, NULL,
				NULL, NULL, NULL, $3->Lineno, $3->Srcfile, $3->Srcfunc);
			}
		| TOK_IMAG '(' expr ')'
			{
			$$ = make_node (IMAG, $3, NULL, NULL,
				NULL, NULL, NULL, $3->Lineno, $3->Srcfile, $3->Srcfunc);
			}
		| expr '+' expr
			{
			$$ = make_node (ADD, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr '-' expr
			{
			$$ = make_node (SUB, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr '*' expr
			{
			$$ = make_node (MUL, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr '/' expr
			{
			$$ = make_node (DIV, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr '%' expr
			{
			$$ = make_node (MOD, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr '<' expr
			{
			$$ = make_node (LT, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr '>' expr
			{
			$$ = make_node (GT, $1, $3, NULL,	
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr TOK_LE expr
			{
			$$ = make_node (LE, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr TOK_GE expr
			{
			$$ = make_node (GE, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr TOK_NE expr
			{
			$$ = make_node (NEQ, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr TOK_EQUAL expr
			{
			$$ = make_node (EQ, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr TOK_AND expr
			{
			$$ = make_node (AND, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr TOK_OR expr
			{
			$$ = make_node (OR, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr '&' expr
			{
			$$ = make_node (BIT_AND, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr '|' expr
			{
			$$ = make_node (BIT_OR, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr '^' expr
			{
			$$ = make_node (BIT_EOR, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr TOK_LEFT_SHIFT expr
			{
			$$ = make_node (LEFT_SHIFT, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| expr TOK_RIGHT_SHIFT expr
			{
			$$ = make_node (RIGHT_SHIFT, $1, $3, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| '!' expr
			{
			$$ = make_node (NOT, $2, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| '-' expr %prec UMINUS
			{
			if ((($2->nodetype==INTNUM) && (! is_hex ($2->Strval)) &&
				(! is_binary ($2->Strval))) || ($2->nodetype==FLOATNUM))
			    {
			    char *str;
			    SacMalloc (str, char*, 2 + strlen ($2->Strval))
			    strcpy (str, "-");
			    strcat (str, $2->Strval);
			    free ($2->Strval);
			    $2->Strval = str;
			    $$ = $2;
			    }
			else
			    $$ = make_node (NEG, $2, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| '(' type_wo_id ')' expr %prec UMINUS
			{
			$$ = make_node (CAST, $2, $4, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| '(' expr ')'
			{ $$ = $2; }
		| for_loop
			{ $$ = $1; }
		| while_loop
			{ $$ = $1; }
		| conditional
			{ $$ = $1; }
		;
switch		: TOK_SWITCH '(' expr ')' '{' cases opt_default '}'
			{
			if (($6==NULL) && ($7==NULL))
			    handle_error (ErrParseNoCaseOrDefault, 
			    		Fatal, $3->Lineno, $3->Srcfile, $3->Srcfunc);

			$$ = make_node (SWITCH, $3, revlist ($6), $7,
				NULL, NULL, NULL, $3->Lineno, $3->Srcfile, $3->Srcfunc);
			}
		;
cases		:	{ $$ = NULL; }
		| cases case
			{
			$2->link = $1;
			$$ = $2;
			}
		;
case		: TOK_CASE const_list ':' body_w_return
			{
			$$ = make_node (CASE, revlist ($2), $4, NULL,
				NULL, NULL, NULL, $2->Lineno, $2->Srcfile, $2->Srcfunc);
			}
		;
const_list	: constval
			{ $$ = $1; }
		| const_list ',' constval
			{
			$3->link = $1;
			$$ = $3;
			}
		;
constval	: TOK_INTNUM
			{
			char *tmp = $1->Strval;
			int val = int_from_string (tmp);
			free (tmp);
			$1->nodetype = ICONST;
			$1->Iconst = val;
			$$ = $1;
			}
		| '-' TOK_INTNUM
			{
			char *tmp = $2->Strval;
			int val = int_from_string (tmp);
			free (tmp);
			$2->nodetype = ICONST;
			$2->Iconst = -val;
			$$ = $2;
			}
		;
opt_default	:	{ $$ = NULL; }
		| TOK_DEFAULT ':' body_w_return
			{
			$$ = $3;
			}
		;
arr_reduce_expr : arr_reduce_op '(' expr ')'
			{
			if ($1->nodetype == ARR_REDUCE_HIST)
			    handle_error (ErrParseHistNeedsRange,
			    		Fatal, $1->Lineno, $1->Srcfile, $1->Srcfunc);
			else if ($1->nodetype == ARR_CONCAT)
			    handle_error (ErrParseConcatNeedsTwoArrays,
			    		Fatal, $1->Lineno, $1->Srcfile, $1->Srcfunc);
			$1->child[0] = $3;
			$$ = $1;
			}
		| arr_reduce_op '(' expr ',' expr ')'
			{
			if ($1->nodetype == ARR_REDUCE_HIST)
			    $1->child[2] = $5;
			else
			    $1->child[1] = $5;
			$1->child[0] = $3;
			$$ = $1;
			}
		| arr_reduce_op '(' expr ',' expr ',' expr ')'
			{
			if ($1->nodetype != ARR_REDUCE_HIST)
			    handle_error (ErrParseRangeOnlyForHist,
			    		Fatal, $1->Lineno, $1->Srcfile, $1->Srcfunc);
			$1->child[0] = $3;
			$1->child[1] = $7;
			$1->child[2] = $5;
			$$ = $1;
			}
		;
arr_slice	: '{' '}'
			{
			$$ = make_node (ARR_SLICE, NULL, NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| '{' exprs '}'
			{
			$$ = make_node (ARR_SLICE, revlist ($2), NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| '{' slices '}'
			{
			$$ = make_node (ARR_SLICE, revlist ($2), NULL, NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		;
slices		: arr_slice
			{ $$ = $1; }
		| slices ',' arr_slice
			{
			$3->link = $1;
			$$ = $3;
			}
		;
while_loop	: TOK_WHILE '(' expr ')' '{' stmts '}' TOK_RETURN '(' ret_exprs ')'
			{
			NODEPNT tmp;
			tmp = make_node (BODYRET, revlist ($6), revlist ($10), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			$$ = make_node (WHILE, $3, tmp, NULL,
				NULL, NULL, NULL, $3->Lineno, $3->Srcfile, $3->Srcfunc);
			}
		;
conditional	: TOK_IF '(' expr ')' body_w_return elifs TOK_ELSE body_w_return
			{
			NODEPNT tmp, *p;
			tmp = revlist_elifs ($6);
			for (p=&tmp; (*p)!=NULL; p=&((*p)->child[1]->child[2]));
			*p = $8;
			$$ = make_node (CONDITIONAL, $3, $5, tmp,
				NULL, NULL, NULL, $3->Lineno, $3->Srcfile, $3->Srcfunc);
			}
		| expr '?' expr ':' expr
			{
			NODEPNT t1, t2;
			t1 = make_node (BODYRET, NULL, $3, NULL,
				NULL, NULL, NULL, $3->Lineno, $3->Srcfile, $3->Srcfunc);
			t2 = make_node (BODYRET, NULL, $5, NULL,
				NULL, NULL, NULL, $5->Lineno, $5->Srcfile, $5->Srcfunc);
			$$ = make_node (CONDITIONAL, $1, t1, t2,
				NULL, NULL, NULL, $1->Lineno, $1->Srcfile, $1->Srcfunc);
			}
		;
elifs		: 	{ $$ = NULL; }
		| elifs elif
			{
			$2->child[1]->child[2] = $1;
			$$ = $2;
			}
		;
elif		: TOK_ELIF '(' expr ')' body_w_return
			{
			NODEPNT tmp;
			tmp = make_node (CONDITIONAL, $3, $5, NULL,
				NULL, NULL, NULL, $3->Lineno, $3->Srcfile, $3->Srcfunc);
			$$ = make_node (BODYRET, NULL, tmp, NULL,
				NULL, NULL, NULL, $3->Lineno, $3->Srcfile, $3->Srcfunc);
			}
		;
body_w_return	: '{' stmts '}' TOK_RETURN '(' exprs ')'
			{
			$$ = make_node (BODYRET, revlist ($2), revlist ($6), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		| TOK_RETURN '(' exprs ')'
			{
			$$ = make_node (BODYRET, NULL, revlist ($3), NULL,
				NULL, NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		;
array_indices	: array_index
			{ $$ = $1; }
		| array_indices ',' array_index
			{
			$3->link = $1;
			$$ = $3;
			}
		;
array_index	: expr
			{ $$ = $1; }
		| triple
			{ $$ = $1; }
		;
triple		: colon_bounds
			{ $$ = $1; }
		| colon_bounds ':' expr
			{
			$1->child[2] = $3;
			$$ = $1;
			}
		;
colon_bounds	: opt_expr ':' opt_expr
			{
			$$ = make_node (TRIPLE, $1, $3, NULL, NULL,
				NULL, NULL, mylineno, handle_filename (filename), funcname);
			}
		;
opt_expr	:	{ $$ = NULL; }
		| expr
			{ $$ = $1; }
		;
%%
