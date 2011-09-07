#include "sassy.h"
#include "node.h"

void show_tree_rec (NODEPNT, int);
void print_a_type (TypeInfo*, FILE*);
void rightshift (int);

char *node_str (Node ty)
    {
    switch (ty)
        {
        case ICONST :
	    return "ICONST";
        case ATYPE :
	    return "ATYPE";
        case ARR_CONPERIM :
	    return "ARR_CONPERIM";
        case TRIPLE :
	    return "TRIPLE";
        case FUNC :
	    return "FUNC";
        case FUNCDECL :
	    return "FUNCDECL";
        case COMPLEX :
	    return "COMPLEX";
        case REAL :
	    return "REAL";
        case IMAG :
	    return "IMAG";
        case IDENT :
	    return "IDENT";
        case INTNUM :
	    return "INTNUM";
        case FLOATNUM :
	    return "FLOATNUM";
        case RETTYPE :
	    return "RETTYPE";
        case BITSTYPE :
	    return "BITSTYPE";
        case FIXTYPE :
	    return "FIXTYPE";
        case UFIXTYPE :
	    return "UFIXTYPE";
        case INTTYPE :
	    return "INTTYPE";
        case UINTTYPE :
	    return "UINTTYPE";
        case FLOATTYPE :
	    return "FLOATTYPE";
        case DOUBLETYPE :
	    return "DOUBLETYPE";
        case CXINTTYPE :
	    return "CXINTTYPE";
        case CXFIXTYPE :
	    return "CXFIXTYPE";
        case CXFLOATTYPE :
	    return "CXFLOATTYPE";
        case CXDOUBLETYPE :
	    return "CXDOUBLETYPE";
        case UNKNOWNTYPE :
	    return "UNKNOWNTYPE";
        case COLON :
	    return "COLON";
        case BODYRET :
	    return "BODYRET";
        case FCALL :
	    return "FCALL";
        case ARRAYREF :
	    return "ARRAYREF";
        case ADD :
	    return "ADD";
        case SUB :
	    return "SUB";
        case MUL :
	    return "MUL";
        case DIV :
	    return "DIV";
        case MOD :
	    return "MOD";
        case LT :
	    return "LT";
        case GT :
	    return "GT";
        case LE :
	    return "LE";
        case GE :
	    return "GE";
        case NEQ :
	    return "NEQ";
        case EQ :
	    return "EQ";
        case AND :
	    return "AND";
        case OR :
	    return "OR";
        case NOT :
	    return "NOT";
        case NEG :
	    return "NEG";
        case FOR :
	    return "FOR";
        case WHILE :
	    return "WHILE";
        case SWITCH :
	    return "SWITCH";
        case CASE :
	    return "CASE";
        case CONDITIONAL :
	    return "CONDITIONAL";
        case ASSIGN :
	    return "ASSIGN";
        case EXTENTS :
	    return "EXTENTS";
        case REDUCE_SUM :
	    return "REDUCE_SUM";
        case REDUCE_MIN :
	    return "REDUCE_MIN";
        case REDUCE_MAX :
	    return "REDUCE_MAX";
        case REDUCE_AND :
	    return "REDUCE_AND";
        case REDUCE_OR :
	    return "REDUCE_OR";
        case REDUCE_PRODUCT :
	    return "REDUCE_PRODUCT";
        case REDUCE_MEAN :
	    return "REDUCE_MEAN";
        case REDUCE_ST_DEV :
	    return "REDUCE_ST_DEV";
        case REDUCE_MODE :
	    return "REDUCE_MODE";
        case REDUCE_MEDIAN :
	    return "REDUCE_MEDIAN";
        case CONSTRUCT_ARRAY :
	    return "CONSTRUCT_ARRAY";
        case CONSTRUCT_VECTOR :
	    return "CONSTRUCT_VECTOR";
        case CONSTRUCT_MATRIX :
	    return "CONSTRUCT_MATRIX";
        case CONSTRUCT_CUBE :
	    return "CONSTRUCT_CUBE";
        case CONSTRUCT_CONCAT :
	    return "CONSTRUCT_CONCAT";
        case CONSTRUCT_TILE :
	    return "CONSTRUCT_TILE";
        case TILDE :
	    return "TILDE";
        case BOOLTYPE :
	    return "BOOLTYPE";
        case WINDOW :
	    return "WINDOW";
        case WSPEC :
	    return "WSPEC";
        case WINFO :
	    return "WINFO";
        case EMPTY :
	    return "EMPTY";
        case ELE_GEN :
	    return "ELE_GEN";
        case SLICE_GEN :
	    return "SLICE_GEN";
        case SCALAR_GEN :
	    return "SCALAR_GEN";
        case ELE_SRCE :
	    return "ELE_SRCE";
        case DOT_PROD :
	    return "DOT_PROD";
        case CROSS_PROD :
	    return "CROSS_PROD";
        case TRUE_ND :
	    return "TRUE_ND";
        case FALSE_ND :
	    return "FALSE_ND";
        case LOCAL :
	    return "LOCAL";
        case PRINT :
	    return "PRINT";
        case ASSERT :
	    return "ASSERT";
        case STRING :
	    return "STRING";
        case ARR_SLICE :
	    return "ARR_SLICE";
        case ARR_REDUCE_SUM :
	    return "ARR_REDUCE_SUM";
        case ARR_REDUCE_MIN :
	    return "ARR_REDUCE_MIN";
        case ARR_REDUCE_MAX :
	    return "ARR_REDUCE_MAX";
        case ARR_REDUCE_AND :
	    return "ARR_REDUCE_AND";
        case ARR_REDUCE_OR :
	    return "ARR_REDUCE_OR";
        case ARR_REDUCE_MAX_INDICES :
	    return "ARR_REDUCE_MAX_INDICES";
        case ARR_REDUCE_MIN_INDICES :
	    return "ARR_REDUCE_MIN_INDICES";
        case ARR_REDUCE_PRODUCT :
	    return "ARR_REDUCE_PRODUCT";
        case ARR_REDUCE_MEAN :
	    return "ARR_REDUCE_MEAN";
        case ARR_REDUCE_ST_DEV :
	    return "ARR_REDUCE_ST_DEV";
        case ARR_REDUCE_MODE :
	    return "ARR_REDUCE_MODE";
        case ARR_REDUCE_MEDIAN :
	    return "ARR_REDUCE_MEDIAN";
        case CAST :
	    return "CAST";
        case BIT_AND :
	    return "BIT_AND";
        case BIT_OR :
	    return "BIT_OR";
        case BIT_EOR :
	    return "BIT_EOR";
        case LEFT_SHIFT :
	    return "LEFT_SHIFT";
        case RIGHT_SHIFT :
	    return "RIGHT_SHIFT";
        case NEXT :
	    return "NEXT";
        case REDUCE_HIST :
	    return "REDUCE_HIST";
        case ARR_REDUCE_HIST :
	    return "ARR_REDUCE_HIST";
        case ARR_CONCAT :
	    return "ARR_CONCAT";
        case ACCUM :
	    return "ACCUM";
        case ARR_ACCUM :
	    return "ARR_ACCUM";
        case INTRINSIC_FUNC :
	    return "INTRINSIC_FUNC";
        case INTRINCALL :
	    return "INTRINCALL";
        case LOOP_INDICES :
	    return "LOOP_INDICES";
        case REDUCE_VAL_AT_FIRST_MIN :
	    return "REDUCE_VAL_AT_FIRST_MIN";
        case REDUCE_VAL_AT_FIRST_MAX :
	    return "REDUCE_VAL_AT_FIRST_MAX";
        case REDUCE_VAL_AT_LAST_MIN :
	    return "REDUCE_VAL_AT_LAST_MIN";
        case REDUCE_VAL_AT_LAST_MAX :
	    return "REDUCE_VAL_AT_LAST_MAX";
        case REDUCE_VAL_AT_MINS :
	    return "REDUCE_VAL_AT_MINS";
        case REDUCE_VAL_AT_MAXS :
	    return "REDUCE_VAL_AT_MAXS";
        case NO_UNROLL :
	    return "NO_UNROLL";
        case NO_INLINE :
	    return "NO_INLINE";
        case NO_DFG :
	    return "NO_DFG";
        case LOOKUP :
	    return "LOOKUP";
        case STRIPMINE :
	    return "STRIPMINE";
        case NO_FUSE :
	    return "NO_FUSE";
        case BLOCK :
	    return "BLOCK";
        case VHDL :
	    return "VHDL";
	default :
	    assert (0);
	}
    }

char *intrin_str (Intrinsic v)
    {
    switch (v)
        {
	case IntrinSin :
	    return "sin";
	case IntrinCos :
	    return "cos";
	case IntrinTan :
	    return "tan";
	case IntrinAsin :
	    return "asin";
	case IntrinAcos :
	    return "acos";
	case IntrinAtan :
	    return "atan";
	case IntrinAtan2 :
	    return "atan2";
	case IntrinSinh :
	    return "sinh";
	case IntrinCosh :
	    return "cosh";
	case IntrinTanh :
	    return "tanh";
	case IntrinAsinh :
	    return "asinh";
	case IntrinAcosh :
	    return "acosh";
	case IntrinAtanh :
	    return "atanh";
	case IntrinSqrt :
	    return "sqrt";
	case IntrinCbrt :
	    return "cbrt";
	case IntrinPow :
	    return "pow";
	case IntrinModf :
	    return "modf";
	case IntrinExp :
	    return "exp";
	case IntrinFrexp :
	    return "frexp";
	case IntrinLdexp :
	    return "ldexp";
	case IntrinLog :
	    return "log";
	case IntrinLog10 :
	    return "log10";
	case IntrinExpm1 :
	    return "expm1";
	case IntrinLog1p :
	    return "log1p";
	case IntrinCeil :
	    return "ceil";
	case IntrinFabs :
	    return "fabs";
	case IntrinFloor :
	    return "floor";
	case IntrinFmod :
	    return "fmod";
	case IntrinCopysign :
	    return "copysign";
	case IntrinHypot :
	    return "hypot";
	case IntrinRint :
	    return "rint";
	default :
	    assert (0);
	}
    }

void show_tree (NODEPNT n)
    {
    show_tree_rec (n, 0);
    }

/* a simple tree printing function */
void show_tree_rec (NODEPNT n, int indent)
    {
    TypeInfo *tt;
    int i;
    char *id_string;

    rightshift (indent);

    printf ("%s  %p, line %d", node_str (n->nodetype), n, n->Lineno);
    if (n->Srcfile != NULL)
        printf (", file '%s'", n->Srcfile);
    if (n->Srcfunc != NULL)
        printf (", func '%s'", n->Srcfunc);

    if (n->types != NULL)
        {
	tt = n->types;

	printf (" (");

	print_a_type (tt, stdout);

	printf (") ");
	}

    switch (n->nodetype)
	{
	case INTTYPE :
	case UINTTYPE :
	case BOOLTYPE :
	case FLOATTYPE :
	case DOUBLETYPE :
	case CXINTTYPE :
	case CXFLOATTYPE :
	case CXDOUBLETYPE :
	    printf (", totsize %d\n", n->Totsize);
	    break;
	case UFIXTYPE :
	case FIXTYPE :
	case CXFIXTYPE :
	    printf (", totsize %d, ", n->Totsize);
	    printf ("fracsize %d\n", n->Fracsize);
	    break;
	case IDENT :
	    id_string = find_sym (n->Symval);
	    if (id_string == NULL)
		id_string = "---";
	    printf (", sval %d, '%s', dval %d\n",
		n->Symval, id_string, n->Defval);
	    break;
	case STRING :
	case INTNUM :
	case FLOATNUM :
	    printf (", strval '%s'\n", n->Strval);
	    break;
	case ICONST :
	    printf (", xval %d\n", n->Iconst);
	    break;
	case INTRINSIC_FUNC :
	    printf (", intrin %s\n", intrin_str (n->Intrinval));
	default :
	    printf ("\n");
	    break;
	}

    indent += 2;

    /* show the pragmas */
    if (n->pragma != NULL)
        {
        printf ("pragmas:\n");
	rightshift (indent);
	show_tree_rec (n->pragma, indent);
	}

    /* show the children */
    for (i=0; i<NUM_CHILDREN; i++)
	{
	if (n->child[i] == NULL)
	    continue;
        rightshift (indent);
	printf ("ch %d:\n", i);
	indent += 2;
        if (n->child[i] == NULL)
	    {
            rightshift (indent);
	    printf ("<null>\n");
	    }
        else
	    show_tree_rec (n->child[i], indent);
	indent -= 2;
	}

    if (n->link != NULL)
	{
        rightshift (indent);
        printf ("link:\n");
        indent += 2;
	show_tree_rec (n->link, indent);
	indent -= 2;
	}

    indent -= 2;
    }

void rightshift (int n)
    {
    int i;

    for (i=0; i<n; i++)
	printf (" ");
    }
