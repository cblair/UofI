#ifndef _NODE_H
#define _NODE_H

#include "sactypes.h"

typedef enum {
    ICONST,
    ATYPE,
    ARR_CONPERIM,
    TRIPLE,
    COMPLEX,
    REAL,
    IMAG,
    FUNC,
    FUNCDECL,
    IDENT,
    INTNUM,
    FLOATNUM,
    RETTYPE,
    BITSTYPE,
    INTTYPE,
    UINTTYPE,
    FIXTYPE,
    UFIXTYPE,
    FLOATTYPE,
    DOUBLETYPE,
    CXINTTYPE,
    CXFIXTYPE,
    CXFLOATTYPE,
    CXDOUBLETYPE,
    UNKNOWNTYPE,
    COLON,
    BODYRET,
    FCALL,
    ARRAYREF,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    LT,
    GT,
    LE,
    GE,
    NEQ,
    EQ,
    AND,
    OR,
    NOT,
    NEG,
    FOR,
    WHILE,
    SWITCH,
    CASE,
    CONDITIONAL,
    FORALL,
    ASSIGN,
    EXTENTS,
    REDUCE_SUM,
    REDUCE_MIN,
    REDUCE_MAX,
    REDUCE_AND,
    REDUCE_OR,
    REDUCE_PRODUCT,
    REDUCE_MEAN,
    REDUCE_ST_DEV,
    REDUCE_MODE,
    REDUCE_MEDIAN,
    CONSTRUCT_ARRAY,
    CONSTRUCT_VECTOR,
    CONSTRUCT_MATRIX,
    CONSTRUCT_CUBE,
    CONSTRUCT_CONCAT,
    CONSTRUCT_TILE,
    TILDE,
    BOOLTYPE,
    WINDOW,
    WSPEC,
    WINFO,
    EMPTY,
    ELE_GEN,
    SCALAR_GEN,
    ELE_SRCE,
    DOT_PROD,
    CROSS_PROD,
    TRUE_ND,
    FALSE_ND,
    LOCAL,
    STRING,
    PRINT,
    ASSERT,
    ARR_SLICE,
    ARR_REDUCE_SUM,
    ARR_REDUCE_MIN,
    ARR_REDUCE_MAX,
    ARR_REDUCE_AND,
    ARR_REDUCE_OR,
    ARR_REDUCE_MAX_INDICES,
    ARR_REDUCE_MIN_INDICES,
    ARR_REDUCE_PRODUCT,
    ARR_REDUCE_MEAN,
    ARR_REDUCE_ST_DEV,
    ARR_REDUCE_MODE,
    ARR_REDUCE_MEDIAN,
    CAST,
    BIT_AND,
    BIT_OR,
    BIT_EOR,
    LEFT_SHIFT,
    RIGHT_SHIFT,
    NEXT,
    REDUCE_HIST,
    ARR_REDUCE_HIST,
    ARR_CONCAT,
    ACCUM,
    ARR_ACCUM,
    INTRINSIC_FUNC,
    INTRINCALL,
    LOOP_INDICES,
    REDUCE_VAL_AT_FIRST_MIN,
    REDUCE_VAL_AT_FIRST_MAX,
    REDUCE_VAL_AT_LAST_MIN,
    REDUCE_VAL_AT_LAST_MAX,
    REDUCE_VAL_AT_MINS,
    REDUCE_VAL_AT_MAXS,
    NO_UNROLL,
    NO_INLINE,
    NO_DFG,
    LOOKUP,
    STRIPMINE,
    NO_FUSE,
    BLOCK,
    VHDL,
    SLICE_GEN
    } Node;

typedef struct node *NODEPNT;

#define NUM_CHILDREN 5

#define Symval val.ident_vals.symval
#define Defval val.ident_vals.defval
#define Totsize val.type_vals.totsize
#define Fracsize val.type_vals.fracsize
#define Strval val.strval
#define Iconst val.iconst
#define Intrinval val.intrinval
#define Lineno loc.line
#define Srcfile loc.file
#define Srcfunc loc.func

typedef struct node
    {
    Node nodetype;
    NODEPNT child[NUM_CHILDREN];
    NODEPNT pragma;
    NODEPNT link;
    TypeInfo *types;
    Location loc;
    union {
	struct {
	    int symval;
	    int defval;
	    } ident_vals;	/* relevant for IDENT nodes */
	struct {
	    int totsize;
	    int fracsize;
	    } type_vals;	/* relevant for type nodes */
	int iconst;		/* relevant for ICONST nodes */
	char *strval;		/* relevant for STRING, INTNUM, and FLOATNUM */
	Intrinsic intrinval;	/* relevant for INTRINSIC_FUNC */
	} val;
    } NODE;

#define MALLOC_NODE(x,ty) {\
        int i;\
        x = (NODEPNT) malloc (sizeof (NODE));\
        x->link = NULL;\
        x->pragma = NULL;\
	x->Lineno = 0;\
	x->Srcfile = NULL;\
	x->Srcfunc = NULL;\
	x->types = NULL;\
        for (i=0; i<NUM_CHILDREN; i++)\
            x->child[i] = NULL;\
        x->nodetype = ty;}\

extern NODEPNT prog_tree;
NODEPNT make_node (int, NODEPNT, NODEPNT, NODEPNT, NODEPNT,
		NODEPNT, NODEPNT, int, char*, char*);
void free_tree (NODEPNT);
int list_length (NODEPNT);
char *node_str (Node);
char *intrin_str (Intrinsic);
void types (NODEPNT);
NODEPNT copy_node (NODEPNT);
NODEPNT copy_tree (NODEPNT);
TypeInfo *copy_types_list (TypeInfo*);
TypeInfo *copy_typeinfo (TypeInfo*);
void scoping (NODEPNT*);
void flow (NODEPNT);
void tree2c (char*, NODEPNT);
void tree2sassy (NODEPNT);

#endif
