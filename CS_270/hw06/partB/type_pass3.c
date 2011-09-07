#include "sassy.h"
#include "node.h"
#include "deftab.h"
#include "error.h"

void type_pass3_bodyret (NODEPNT);
void type_pass3_stmt (NODEPNT);
void type_pass3_expr (NODEPNT);
Tstat inconsistent_pass3 (int, TypeInfo*);
Tstat compare_typenodes_pass3 (TypeInfo*, TypeInfo*);
int bit_width (int);
void set_type_info_pass3 (TypeInfo*, Type, int, int);
int is_complex_num_type (Type);
int is_non_complex_num_type (Type);
extern Tstat typecast_compatible[14][14];
extern Tstat type_compatible[14][14];
void make_composite_type (TypeInfo*, TypeInfo*, Type*, int*, int*);
void set_type_and_sizes (NODEPNT, Type*, int*, int*);
void check_arr_def_components (NODEPNT, Type);
void type_pass3_generators (NODEPNT);
void type_pass3_ele_gen (NODEPNT);
void type_pass3_slice_gen (NODEPNT);
void type_pass3_scalar_gen (NODEPNT);
void type_pass3_window_gen (NODEPNT);
int is_numeric (Type);
int non_int (Type);
TypeInfo *nth_type (TypeInfo*, int);
int tlist_length1 (TypeInfo*);

/* This pass assumes that all multiple-valued BODYRETs, etc, are
 * properly balanced with their target variable lists, and that
 * all expression nodes are annotated with empty TypeInfo structs.
 * It also assumes that all array/scalar and array-rank issues have
 * been checked and resolved. This pass looks at type-compatibility
 * issues.
 */
void type_pass3 (NODEPNT root)
    {
    NODEPNT n;
    TypeInfo *rtypes, *tt, *tr;
    Tstat st;
    int fdv, cnt;

    for (n=root; n!=NULL; n=n->link)
	{
	/* process the function's body */
        type_pass3_bodyret (n->child[3]);

	fdv = n->child[1]->Defval;
	rtypes = deftable[fdv].funcinfo->rettypes;

	/* check return values vs return types in decl */
	for (cnt=0, tt=n->child[3]->types, tr=rtypes;
			tt!=NULL && tr!=NULL; cnt++, tt=tt->link, tr=tr->link)
	    if (tt->stat == Okay)
	        {
	        st = compare_typenodes_pass3 (tt, tr);
	        if (st == Err)
		    handle_error (ErrTypePass3RetDeclMismatch,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc, cnt);
		}
	}
    }

void type_pass3_bodyret (NODEPNT br)
    {
    NODEPNT s, e;
    TypeInfo *t, *v;

    /*
    printf ("type_pass3_bodyret on node %d\n", br);
    fflush (stdout);
    */

    assert (br->nodetype == BODYRET);

    /* process its statement spine */
    for (s=br->child[0]; s!=NULL; s=s->link)
        type_pass3_stmt (s);

    /* process its return expressions */
    for (s=br->child[1]; s!=NULL; s=s->link)
        type_pass3_expr (s);

    /* copy the TypeInfo node information from the return expressions
     * to the BODYRET's TypeInfo nodes
     */
    for (t=br->types, e=br->child[1]; e!=NULL; e=e->link)
	for (v=e->types; v!=NULL; v=v->link)
	    {
	    set_type_info_pass3 (t, v->type, v->totsize, v->fracsize);
	    t=t->link;
	    }
    }

void type_pass3_stmt (NODEPNT s)
    {
    NODEPNT lhs, rhs, n;
    TypeInfo *t;
    int cnt;

    /*
    printf ("type_pass3_stmt on node %d\n", s);
    fflush (stdout);
    */

    switch (s->nodetype)
        {
	case ASSIGN :
	    if (s->child[1]->nodetype == ARR_SLICE)
	        {
	        Type ty;
	        int tsz, fsz;

	        set_type_and_sizes (s->child[0]->child[0], &ty, &tsz, &fsz);
	        set_type_info_pass3 (s->child[1]->types, ty, tsz, fsz);

	        /* check type compatibility between declared type and components */
	        check_arr_def_components (s->child[1], ty);
		}
	    else if (s->child[1]->nodetype != LOOP_INDICES)
	        {
	        /* process the expressions on the RHS */
	        for (rhs=s->child[1]; rhs!=NULL; rhs=rhs->link)
	            type_pass3_expr (rhs);

	        /* check consistency between LHS and RHS */
	        for (cnt=0, lhs=s->child[0], rhs=s->child[1]; rhs!=NULL; rhs=rhs->link)
	            for (t=rhs->types; t!=NULL; cnt++, t=t->link)
		        {
		        if (lhs->nodetype == IDENT)
	                    if (inconsistent_pass3 (lhs->Defval, t) == Err)
			        handle_error (ErrTypePass3TypeAcrossAssign,
			    		Fatal, s->Lineno, s->Srcfile, s->Srcfunc, cnt);

		        lhs=lhs->link;
		        }
		}
	    break;
	case ASSERT :
	case PRINT :
	    /* process the predicate expression */
	    type_pass3_expr (s->child[0]);

	    if ((s->child[0]->types->stat==Okay) &&
	    			(s->child[0]->types->type!=Bool))
		handle_error (ErrTypaPass3PrintAssertPredBool,
				Fatal, s->Lineno, s->Srcfile, s->Srcfunc);

	    for (n=s->child[1]; n!=NULL; n=n->link)
	        if (n->nodetype != STRING)
		    type_pass3_expr (n);
	    break;
	default :
            break;
	}
    }

void type_pass3_expr (NODEPNT n)
    {

    assert (n != NULL);

    /*
    printf ("type_pass3_expr, node %d\n", n);
    fflush (stdout);
    printf ("  type %s\n", node_str (n->nodetype));
    fflush (stdout);
    */

    switch (n->nodetype)
        {
        case IDENT :
	    {
	    int dv = n->Defval;

	    assert (n->types != NULL);

	    set_type_info_pass3 (n->types, deftable[dv].tinfo.type,
	    	deftable[dv].tinfo.totsize, deftable[dv].tinfo.fracsize);
	    }
	    break;
        case INTNUM :
	    {
	    int val = int_from_string (n->Strval);

	    if (is_hex (n->Strval) || is_binary (n->Strval))
		{
		n->types->type = Bits;
		}
	    else
	        {
		assert (n->types != NULL);
	        if (val & 0x80000000)
	            n->types->type = Int;
	        else
	            n->types->type = Uint;
		}

	    n->types->totsize = bit_width (val);
	    n->types->fracsize = 0;
	    }
	    break;
        case ICONST :
	    {
	    int val = n->Iconst;
	    if (val & 0x80000000)
	        n->types->type = Int;
	    else
	        n->types->type = Uint;
	    n->types->totsize = bit_width (val);
	    n->types->fracsize = 0;
	    }
	    break;
        case FLOATNUM :
	    {
	    int lngth = strlen (n->Strval);
	    char ch = (n->Strval)[lngth-1];
	    if (ch=='F' || ch=='f')
	        {
		n->types->type = Float;
		n->types->totsize = 32;
		n->types->fracsize = 0;
		}
	    else
	        {
		n->types->type = Double;
		n->types->totsize = 64;
		n->types->fracsize = 0;
		}
	    }
	    break;
        case TRUE_ND :
        case FALSE_ND :
	    n->types->type = Bool;
	    n->types->totsize = 1;
            break;
        case NOT :
	    type_pass3_expr (n->child[0]);
	    if (n->child[0]->types->stat==Okay)
	        {
		if (n->child[0]->types->type != Bool)
		    {
		    handle_error (ErrTypePass3NotNeedsBool,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		    n->types->stat = Err;
		    }
		else
		    {
		    n->types->type = Bool;
		    n->types->totsize = 1;
		    }
		}
	    else
	        n->types->stat = Err;
	    break;
        case NEG :
	    type_pass3_expr (n->child[0]);
	    if (n->child[0]->types->stat==Okay)
	        {
		switch (n->child[0]->types->type)
		    {
		    case Uint :
		        n->types->type = Int;
			n->types->totsize = n->child[0]->types->totsize + 1;
			break;
		    case Int :
		        n->types->type = Int;
			n->types->totsize = n->child[0]->types->totsize;
			break;
		    case Ufix :
		        n->types->type = Fix;
			n->types->totsize = n->child[0]->types->totsize + 1;
			n->types->fracsize = n->child[0]->types->fracsize;
			break;
		    case Fix :
		        n->types->type = Fix;
			n->types->totsize = n->child[0]->types->totsize;
			n->types->fracsize = n->child[0]->types->fracsize;
			break;
		    case Float :
		        n->types->type = Float;
			n->types->totsize = 32;
			break;
		    case Double :
		        n->types->type = Double;
			n->types->totsize = 64;
			break;
		    default :
			handle_error (ErrTypePass3NegateIllegalType,
					Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
			n->types->stat = Err;
		    }
		}
	    else
	        n->types->stat = Err;
	    break;
        case REAL :
        case IMAG :
	    type_pass3_expr (n->child[0]);
	    if (n->child[0]->types->stat==Okay)
	        {
		switch (n->child[0]->types->type)
		    {
		    case CxInt :
		        n->types->type = Int;
			n->types->totsize = n->child[0]->types->totsize;
			break;
		    case CxFix :
		        n->types->type = Fix;
			n->types->totsize = n->child[0]->types->totsize;
			n->types->fracsize = n->child[0]->types->fracsize;
			break;
		    case CxFloat :
		        n->types->type = Float;
			n->types->totsize = 32;
			break;
		    case CxDouble :
		        n->types->type = Double;
			n->types->totsize = 64;
			break;
		    default :
			handle_error (ErrTypePass3RealImag,
					Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		        n->types->stat = Err;
		    }
		}
	    else
	        n->types->stat = Err;
	    break;
        case CONSTRUCT_ARRAY :
        case CONSTRUCT_VECTOR :
        case CONSTRUCT_MATRIX :
        case CONSTRUCT_CUBE :
        case CONSTRUCT_CONCAT :
        case CONSTRUCT_TILE :
	    type_pass3_expr (n->child[0]);
	    if (n->child[0]->types->stat==Okay)
		{
		set_type_info_pass3 (n->types, n->child[0]->types->type,
			n->child[0]->types->totsize, n->child[0]->types->fracsize);

		if (n->child[1] != NULL)
		    {
		    type_pass3_expr (n->child[1]);
		    if (n->child[1]->types->stat==Okay)
		        {
		        if (n->child[1]->types->type != Bool)
			    {
			    handle_error (ErrTypePass3ConcatMaskNeedsBool,
					Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
			    n->child[1]->types->stat = Err;
			    }
			}
		    }
		}
	    else
	        n->types->stat = Err;
	    break;
        case ADD :
        case SUB :
        case MUL :
        case DIV :
	    {
	    TypeInfo *t0, *t1;
	    Type ty;
	    int tsz, fsz;

	    type_pass3_expr (n->child[0]);
	    type_pass3_expr (n->child[1]);

	    if (n->child[0]->types->stat==Err || n->child[1]->types->stat==Err)
	        n->types->stat = Err;
	    else
	        {
		t0 = n->child[0]->types;
		t1 = n->child[1]->types;

		if ((t0->type==Bits) || (t0->type==Bool) || (t1->type==Bits) || (t1->type==Bool))
		    {
		    handle_error (ErrTypePass3ArithIllegalType,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc, n->nodetype);
		    n->types->stat = Err;
		    }
		else
		    {
		    if (type_compatible[t0->type][t1->type] == Err)
		        {
		        handle_error (ErrTypePass3ASMDCompatible,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc, n->nodetype);
		        n->types->stat = Err;
		        }
		    else
		        {
		        make_composite_type (t0, t1, &ty, &tsz, &fsz);
		        set_type_info_pass3 (n->types, ty, tsz, fsz);
		        }
		    }
		}
	    }
	    break;
        case MOD :
	    {
	    TypeInfo *t0, *t1;
	    Type ty;
	    int tsz, fsz;

	    type_pass3_expr (n->child[0]);
	    type_pass3_expr (n->child[1]);

	    if (n->child[0]->types->stat==Err || n->child[1]->types->stat==Err)
	        n->types->stat = Err;
	    else
	        {
		t0 = n->child[0]->types;
		t1 = n->child[1]->types;

		if ((t0->type!=Int && t0->type!=Uint) || (t1->type!=Int && t1->type!=Uint))
		    {
		    handle_error (ErrTypePass3ModOperand,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		    n->types->stat = Err;
		    }
		else
		    {
		    make_composite_type (t0, t1, &ty, &tsz, &fsz);
		    set_type_info_pass3 (n->types, ty, tsz, fsz);
		    }
		}
	    }
	    break;
        case LT :
        case GT :
        case LE :
        case GE :
	    {
	    Type t0, t1;

	    type_pass3_expr (n->child[0]);
	    type_pass3_expr (n->child[1]);

	    if (n->child[0]->types->stat==Err || n->child[1]->types->stat==Err)
	        n->types->stat = Err;
	    else
	        {
		t0 = n->child[0]->types->type;
		t1 = n->child[1]->types->type;

		if (type_compatible[t0][t1] == Err)
		    {
		    handle_error (ErrTypePass3IneqIncompatible,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc, n->nodetype);
		    n->types->stat = Err;
		    }
		else
		    {
		    if (is_complex_num_type (t0) || is_complex_num_type (t1))
		        {
			handle_error (ErrTypePass3IneqComplex,
					Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
			n->types->stat = Err;
			}
		    else
		        {
			n->types->type = Bool;
			n->types->totsize = 1;
			}
		    }
		}
	    }
	    break;
        case NEQ :
        case EQ :
	    {
	    Type t0, t1;

	    type_pass3_expr (n->child[0]);
	    type_pass3_expr (n->child[1]);

	    if (n->child[0]->types->stat==Err || n->child[1]->types->stat==Err)
	        n->types->stat = Err;
	    else
	        {
		t0 = n->child[0]->types->type;
		t1 = n->child[1]->types->type;

		if (type_compatible[t0][t1] == Err)
		    {
		    handle_error (ErrTypePass3EqCompatible,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc, n->nodetype);
		    n->types->stat = Err;
		    }
		else
		    {
		    n->types->type = Bool;
		    n->types->totsize = 1;
		    }
		}
	    }
	    break;
        case AND :
        case OR :
	    {
	    Type t0, t1;

	    type_pass3_expr (n->child[0]);
	    type_pass3_expr (n->child[1]);

	    if (n->child[0]->types->stat==Err || n->child[1]->types->stat==Err)
	        n->types->stat = Err;
	    else
	        {
		t0 = n->child[0]->types->type;
		t1 = n->child[1]->types->type;

		if ((t0 != Bool) || (t1 != Bool))
		    {
		    handle_error (ErrTypePass3AndOrBool,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc, n->nodetype);
		    n->types->stat = Err;
		    }
		else
		    {
		    n->types->type = Bool;
		    n->types->totsize = 1;
		    }
		}
	    }
	    break;
        case BIT_AND :
        case BIT_OR :
        case BIT_EOR :
	    {
	    TypeInfo *t0, *t1;
	    Type ty;
	    int tsz, fsz;

	    type_pass3_expr (n->child[0]);
	    type_pass3_expr (n->child[1]);

	    if (n->child[0]->types->stat==Err || n->child[1]->types->stat==Err)
	        n->types->stat = Err;
	    else
	        {
		t0 = n->child[0]->types;
		t1 = n->child[1]->types;

		if ((t0->type != Bits) || (t1->type != Bits))
		    {
		    handle_error (ErrTypePass3BitOperands,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc, n->nodetype);
		    n->types->stat = Err;
		    }
		else
		    {
		    make_composite_type (t0, t1, &ty, &tsz, &fsz);
		    set_type_info_pass3 (n->types, ty, tsz, fsz);
		    }
		}
	    }
	    break;
        case LEFT_SHIFT :
        case RIGHT_SHIFT :
	    {
	    Type t0, t1;

	    type_pass3_expr (n->child[0]);
	    type_pass3_expr (n->child[1]);

	    if (n->child[0]->types->stat==Err || n->child[1]->types->stat==Err)
	        n->types->stat = Err;
	    else
	        {
		t0 = n->child[0]->types->type;
		t1 = n->child[1]->types->type;

		if ((t0 != Bits) || non_int (t1))
		    {
		    handle_error (ErrTypePass3ShiftOperands,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc, n->nodetype);
		    n->types->stat = Err;
		    }
		else
		    set_type_info_pass3 (n->types, Bits, n->child[0]->types->totsize, 0);
		}
	    }
	    break;
        case COMPLEX :
	    {
	    TypeInfo *t0, *t1;
	    Type ty;
	    int tsz, fsz;

	    type_pass3_expr (n->child[0]);
	    type_pass3_expr (n->child[1]);

	    if (n->child[0]->types->stat==Err || n->child[1]->types->stat==Err)
	        n->types->stat = Err;
	    else
	        {
		t0 = n->child[0]->types;
		t1 = n->child[1]->types;

		if (type_compatible[t0->type][t1->type] == Err)
		    {
		    handle_error (ErrTypePass3ComplexIncompatible,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		    n->types->stat = Err;
		    }
		else
		    {
		    make_composite_type (t0, t1, &ty, &tsz, &fsz);

		    switch (ty)
		        {
			case Uint :
			    set_type_info_pass3 (n->types, CxInt, tsz+1, fsz);
			    break;
			case Int :
			    set_type_info_pass3 (n->types, CxInt, tsz, fsz);
			    break;
			case Ufix :
			    set_type_info_pass3 (n->types, CxFix, tsz+1, fsz);
			    break;
			case Fix :
			    set_type_info_pass3 (n->types, CxFix, tsz, fsz);
			    break;
			case Float :
			    set_type_info_pass3 (n->types, CxFloat, tsz, fsz);
			    break;
			case Double :
			    set_type_info_pass3 (n->types, CxDouble, tsz, fsz);
			    break;
			default :
			    handle_error (ErrTypePass3ComplexComponent,
			    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
			    n->types->stat = Err;
			}
		    }
		}
	    }
	    break;
        case ARR_CONPERIM :
	    {
	    TypeInfo *t0, *t1, *t2;

	    type_pass3_expr (n->child[0]);
	    type_pass3_expr (n->child[1]);
	    type_pass3_expr (n->child[2]);

	    if (n->child[0]->types->stat==Err || n->child[1]->types->stat==Err ||
	    			n->child[2]->types->stat==Err)
		n->types->stat = Err;
	    else
	        {
		t0 = n->child[0]->types;
		t1 = n->child[1]->types;
		t2 = n->child[2]->types;

		if (non_int (t1->type))
		    {
		    handle_error (ErrTypePass3PerimeterWidthType,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		    n->types->stat = Err;
		    }

		if (type_compatible[t0->type][t2->type] == Err)
		    {
		    handle_error (ErrTypePass3PerimeterIncompatible,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		    n->types->stat = Err;
		    }
		else
		    set_type_info_pass3 (n->types, t0->type, t0->totsize, t0->fracsize);
		}
	    }
	    break;
        case ARR_ACCUM :
        case ACCUM :
	    type_pass3_expr (n->child[0]);
	    if (n->child[0]->types->stat == Err)
	        n->types->stat = Err;
	    else
	        set_type_info_pass3 (n->types, n->child[0]->types->type,
			n->child[0]->types->totsize, n->child[0]->types->fracsize);

	    /* check range expression */
	    type_pass3_expr (n->child[1]);
	    if (non_int (n->child[1]->types->type))
		handle_error (ErrTypePass3AccumRangeInteger,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);

	    /* check label expression */
	    type_pass3_expr (n->child[2]);
	    if (non_int (n->child[2]->types->type))
		handle_error (ErrTypePass3AccumLabelInteger,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	    break;
        case CAST :
	    {
	    Type ty;
	    int tsz, fsz;

	    type_pass3_expr (n->child[1]);

	    if (n->child[1]->types->stat == Err)
	        n->types->stat = Err;
	    else
	        {
		/* get the cast's type and size information */
		set_type_and_sizes (n->child[0]->child[0], &ty, &tsz, &fsz);

	        if (typecast_compatible[n->child[1]->types->type][ty] == Err)
		    {
		    handle_error (ErrTypePass3IllegalTypecast,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		    n->types->stat = Err;
		    }
		else
		    {
		    if (n->child[1]->nodetype == FLOATNUM)
	                {
		        /* eliminate the cast, and directly tag the float
		         * with the cast's type
		         */
		        n->nodetype = FLOATNUM;
		        n->Strval = n->child[1]->Strval;

			SacFree (n->child[1]->types)
			n->child[1]->types = NULL;
			free_tree (n->child[0]);
			SacFree (n->child[1])
			n->child[0] = NULL;
			n->child[1] = NULL;
		        }

		    set_type_info_pass3 (n->types, ty, tsz, fsz);
		    }
		}
	    }
	    break;
        case REDUCE_SUM :
        case ARR_REDUCE_SUM :
        case REDUCE_PRODUCT :
        case ARR_REDUCE_PRODUCT :
        case REDUCE_MEDIAN :
        case ARR_REDUCE_MEDIAN :
        case REDUCE_MODE :
        case ARR_REDUCE_MODE :
        case REDUCE_ST_DEV :
        case ARR_REDUCE_ST_DEV :
        case REDUCE_MEAN :
        case ARR_REDUCE_MEAN :
	    /* these operate on all numeric types, including complex,
	     * and return the same type
	     */
	    {
	    type_pass3_expr (n->child[0]);

	    if (n->child[0]->types->stat == Err)
	        n->types->stat = Err;
	    else if (! is_numeric (n->child[0]->types->type))
	        {
		handle_error (ErrTypePass3ReduceNeedsNumeric,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		n->types->stat = Err;
		}
	    else
		set_type_info_pass3 (n->types, n->child[0]->types->type,
			n->child[0]->types->totsize, n->child[0]->types->fracsize);

	    /* check mask expression, if it exists */
	    if (n->child[1] != NULL)
		{
	        type_pass3_expr (n->child[1]);
		if (n->child[1]->types->stat == Okay)
		    if (n->child[1]->types->type != Bool)
			handle_error (ErrTypePass3MaskNeedsBool,
					Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		}
	    }
	    break;
        case REDUCE_MIN :
        case ARR_REDUCE_MIN :
        case REDUCE_MAX :
        case ARR_REDUCE_MAX :
	    /* these operate on all non-complex numeric types
	     * and return the same type
	     */
	    {
	    type_pass3_expr (n->child[0]);

	    if (n->child[0]->types->stat == Err)
	        n->types->stat = Err;
	    else if (! is_numeric (n->child[0]->types->type))
	        {
		handle_error (ErrTypePass3ReduceNeedsNumeric,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		n->types->stat = Err;
		}
	    else if (is_complex_num_type (n->child[0]->types->type))
	        {
		handle_error (ErrTypePass3ReduceNeedsNoncomplex,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		n->types->stat = Err;
		}
	    else
		set_type_info_pass3 (n->types, n->child[0]->types->type,
			n->child[0]->types->totsize, n->child[0]->types->fracsize);

	    /* check mask expression, if it exists */
	    if (n->child[1] != NULL)
		{
	        type_pass3_expr (n->child[1]);
		if (n->child[1]->types->stat == Okay)
		    if (n->child[1]->types->type != Bool)
			handle_error (ErrTypePass3MaskNeedsBool,
					Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		}
	    }
	    break;
        case REDUCE_AND :
        case ARR_REDUCE_AND :
        case REDUCE_OR :
        case ARR_REDUCE_OR :
	    /* these operate on Bool and Bits types
	     * and return the same type
	     */
	    {
	    Type ty;

	    type_pass3_expr (n->child[0]);

	    if (n->child[0]->types->stat == Err)
	        n->types->stat = Err;
	    else
	        {
		ty = n->child[0]->types->type;

		if (ty!=Bool && ty!=Bits)
	            {
		    handle_error (ErrTypePass3ReduceNeedsBoolBits,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		    n->types->stat = Err;
		    }
	        else
		    set_type_info_pass3 (n->types, n->child[0]->types->type,
		    	n->child[0]->types->totsize, n->child[0]->types->fracsize);
		}

	    /* check mask expression, if it exists */
	    if (n->child[1] != NULL)
		{
	        type_pass3_expr (n->child[1]);
		if (n->child[1]->types->stat == Okay)
		    if (n->child[1]->types->type != Bool)
			handle_error (ErrTypePass3MaskNeedsBool,
					Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		}
	    }
	    break;
        case ARR_REDUCE_MIN_INDICES :
        case ARR_REDUCE_MAX_INDICES :
	    /* these operate on all non-complex numeric types
	     * and return Uint32
	     */
	    {
	    type_pass3_expr (n->child[0]);

	    if (n->child[0]->types->stat == Err)
	        n->types->stat = Err;
	    else if (! is_numeric (n->child[0]->types->type))
	        {
		handle_error (ErrTypePass3ReduceNeedsNumeric,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		n->types->stat = Err;
		}
	    else if (is_complex_num_type (n->child[0]->types->type))
	        {
		handle_error (ErrTypePass3ReduceNeedsNoncomplex,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		n->types->stat = Err;
		}
	    else
		set_type_info_pass3 (n->types, Uint, 32, 0);

	    /* check mask expression, if it exists */
	    if (n->child[1] != NULL)
		{
	        type_pass3_expr (n->child[1]);
		if (n->child[1]->types->stat == Okay)
		    if (n->child[1]->types->type != Bool)
			handle_error (ErrTypePass3MaskNeedsBool,
					Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		}
	    }
	    break;
        case REDUCE_VAL_AT_FIRST_MIN :
        case REDUCE_VAL_AT_FIRST_MAX :
        case REDUCE_VAL_AT_LAST_MIN :
        case REDUCE_VAL_AT_LAST_MAX :
        case REDUCE_VAL_AT_MINS :
        case REDUCE_VAL_AT_MAXS :
	    {
	    type_pass3_expr (n->child[0]);

	    if (n->child[0]->types->stat == Err)
	        n->types->stat = Err;
	    else if (! is_numeric (n->child[0]->types->type))
	        {
		handle_error (ErrTypePass3ReduceNeedsNumeric,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		n->types->stat = Err;
		}
	    else if (is_complex_num_type (n->child[0]->types->type))
	        {
		handle_error (ErrTypePass3ReduceNeedsNoncomplex,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		n->types->stat = Err;
		}
	    else
		{
		NODEPNT m;
		Tstat st = Okay;

		for (m=n->child[2]; m!=NULL; m=m->link)
		    {
		    type_pass3_expr (m);
		    if (m->types->stat == Err)
		        st = Err;
		    }

		if (st==Okay)
		    for (m=n->child[2]; m!=NULL; m=m->link)
			if (m->link != NULL)
		            if (type_compatible[m->types->type][m->link->types->type] == Err)
			        {
			        handle_error (ErrTypePass3ValsAtIncompatTypes,
			    		Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
			        st = Err;
			        break;
			        }

		if (st == Err)
		    n->types->stat = Err;
		else
		    {
		    TypeInfo tt;
		    Type ty;
		    int tsz, fsz;

		    tt.type = n->child[2]->types->type;
		    tt.totsize = n->child[2]->types->totsize;
		    tt.fracsize = n->child[2]->types->fracsize;

		    for (m=n->child[2]->link; m!=NULL; m=m->link)
			{
		        make_composite_type (&tt, m->types, &ty, &tsz, &fsz);
			tt.type = ty;
			tt.totsize = tsz;
			tt.fracsize = fsz;
			}

		    set_type_info_pass3 (n->types, tt.type, tt.totsize, tt.fracsize);
		    }
		}

	    /* check mask expression, if it exists */
	    if (n->child[1] != NULL)
		{
	        type_pass3_expr (n->child[1]);
		if (n->child[2]->types->stat == Okay)
		    if (n->child[1]->types->type != Bool)
			handle_error (ErrTypePass3MaskNeedsBool,
					Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		}
	    }
	    break;
        case REDUCE_HIST :
        case ARR_REDUCE_HIST :
	    /* this operates on Uint and Int and returns Uint32 */
	    {
	    Type ty;

	    type_pass3_expr (n->child[0]);

	    if (n->child[0]->types->stat == Err)
	        n->types->stat = Err;
	    else
	        {
		ty = n->child[0]->types->type;

		if (non_int (ty))
	            {
		    handle_error (ErrTypePass3HistNeedsInt,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		    n->types->stat = Err;
		    }
	        else
		    set_type_info_pass3 (n->types, Uint, 32, 0);
		}

	    /* check range expression */
	    type_pass3_expr (n->child[2]);
	    if (n->child[2]->types->stat == Okay)
	        {
		ty = n->child[2]->types->type;
		if (non_int (ty))
		    handle_error (ErrTypePass3HistExprNeedsInt,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		}

	    /* check mask expression, if it exists */
	    if (n->child[1] != NULL)
		{
	        type_pass3_expr (n->child[1]);
		if (n->child[1]->types->stat == Okay)
		    if (n->child[1]->types->type != Bool)
			handle_error (ErrTypePass3MaskNeedsBool,
					Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		}
	    }
	    break;
        case ARR_CONCAT :
	    {
	    TypeInfo *t0, *t1;
	    Type ty;
	    int tsz, fsz;

	    type_pass3_expr (n->child[0]);
	    type_pass3_expr (n->child[1]);

	    if (n->child[0]->types->stat==Err || n->child[1]->types->stat==Err)
	        n->types->stat = Err;
	    else
	        {
		t0 = n->child[0]->types;
		t1 = n->child[1]->types;

		if (type_compatible[t0->type][t1->type] == Err)
		    {
		    handle_error (ErrTypePass3ConcatCompatible,
		    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc, n->nodetype);
		    n->types->stat = Err;
		    }
		else
		    {
		    make_composite_type (t0, t1, &ty, &tsz, &fsz);
		    set_type_info_pass3 (n->types, ty, tsz, fsz);
		    }
		}
	    }
	    break;
        case ARRAYREF :
	    {
	    NODEPNT m;
	    Type ty;

	    type_pass3_expr (n->child[0]);

	    if (n->child[0]->types->stat == Err)
	        n->types->stat = Err;
	    else
		set_type_info_pass3 (n->types, n->child[0]->types->type,
			n->child[0]->types->totsize, n->child[0]->types->fracsize);

	    /* check the subscript expression */
	    for (m=n->child[1]; m!=NULL; m=m->link)
	        {
		if (m->nodetype == TRIPLE)
		    {
		    if (m->child[0] != NULL)
			{
		        type_pass3_expr (m->child[0]);
			if (m->child[0]->types->stat == Okay)
			    {
			    ty = m->child[0]->types->type;
			    if (non_int (ty))
				handle_error (ErrTypePass3ArrayIndexNeedsInt,
						Fatal, m->Lineno, m->Srcfile, m->Srcfunc);
			    }
			}
		    if (m->child[1] != NULL)
			{
		        type_pass3_expr (m->child[1]);
			if (m->child[1]->types->stat == Okay)
			    {
			    ty = m->child[1]->types->type;
			    if (non_int (ty))
				handle_error (ErrTypePass3ArrayIndexNeedsInt,
						Fatal, m->Lineno, m->Srcfile, m->Srcfunc);
			    }
			}
		    if (m->child[2] != NULL)
			{
		        type_pass3_expr (m->child[2]);
			if (m->child[2]->types->stat == Okay)
			    {
			    ty = m->child[2]->types->type;
			    if (non_int (ty))
				handle_error (ErrTypePass3ArrayIndexNeedsInt,
						Fatal, m->Lineno, m->Srcfile, m->Srcfunc);
			    }
			}
		    }
		else
		    {
		    type_pass3_expr (m);
		    if (m->types->stat == Okay)
		        {
			ty = m->types->type;
			if (non_int (ty))
			    handle_error (ErrTypePass3ArrayIndexNeedsInt,
						Fatal, m->Lineno, m->Srcfile, m->Srcfunc);
			}
		    }
		}
	    }
	    break;
        case EXTENTS :
	    {
	    TypeInfo *t;

	    type_pass3_expr (n->child[0]);
	    
	    for (t=n->types; t!=NULL; t=t->link)
		set_type_info_pass3 (t, Uint, 32, 0);
	    }
	    break;
        case CONDITIONAL :
	    {
	    TypeInfo *tt, *tf, *tr;
	    Type ty;
	    Tstat st;
	    int cnt, tsz, fsz;

	    type_pass3_expr (n->child[0]);
	    
	    /* check predicate expression */
	    if ((n->child[0]->types->stat==Okay) && (n->child[0]->types->type!=Bool))
		handle_error (ErrTypePass3CondNeedsBool,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);

	    type_pass3_bodyret (n->child[1]);
	    type_pass3_bodyret (n->child[2]);

            /* check consistency between true and false expressions */
            for (cnt=0, tt=n->child[1]->types, tf=n->child[2]->types, tr=n->types;
                                tt!=NULL && tf!=NULL && tr!=NULL;
                                cnt++, tt=tt->link, tf=tf->link, tr=tr->link)
                {
                if (tt->stat==Err || tf->stat==Err)
                    tr->stat = Err;
                else
                    {
                    st = type_compatible[tt->type][tf->type];
                    if (st == Err)
                        {
			handle_error (ErrTypePass3CondTypeIncompatibleTF,
						Fatal, n->Lineno, n->Srcfile, n->Srcfunc, cnt);
                        tr->stat = Err;
                        }
                    else
		        {
			make_composite_type (tt, tf, &ty, &tsz, &fsz);
			set_type_info_pass3 (tr, ty, tsz, fsz);
			}
                    }
                }
	    }
	    break;
        case WHILE :
	    {
	    TypeInfo *tt, *tr;

	    type_pass3_expr (n->child[0]);
	    
	    /* check predicate expression */
	    if ((n->child[0]->types->stat==Okay) && (n->child[0]->types->type!=Bool))
		handle_error (ErrTypePass3WhileNeedsBool,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);

	    type_pass3_bodyret (n->child[1]);

	    for (tt=n->child[1]->types, tr=n->types;
	    		tt!=NULL && tr!=NULL; tt=tt->link, tr=tr->link)
		set_type_info_pass3 (tr, tt->type, tt->totsize, tt->fracsize);
	    }
	    break;
        case FOR :
	    {
	    TypeInfo *tt, *tr;

	    type_pass3_generators (n->child[0]);

	    type_pass3_bodyret (n->child[1]);

	    for (tt=n->child[1]->types, tr=n->types;
	    		tt!=NULL && tr!=NULL; tt=tt->link, tr=tr->link)
		if (tt->stat == Okay)
		    set_type_info_pass3 (tr, tt->type, tt->totsize, tt->fracsize);
		else
		    tr->stat = Err;
	    }
	    break;
        case SWITCH :
	    {
	    NODEPNT m, mm;
	    TypeInfo *tt, *tr, *ntt;
	    int nret, i;
	    Type ty;
	    int tsz, fsz;

	    type_pass3_expr (n->child[0]);
	    tt = n->child[0]->types;

	    if (tt->stat == Okay)
	        switch (tt->type)
		    {
		    case Int :
		    case Uint :
		    case Bits :
		        break;
		    default :
			handle_error (ErrTypePass3SwitchExprType,
					Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		    }

	    /* tag the ICONST nodes in the case selectors */
	    for (m=n->child[1]; m!=NULL; m=m->link)
		for (mm=m->child[0]; mm!=NULL; mm=mm->link)
		    type_pass3_expr (mm);

	    /* check consistency of return types of all cases, and determine
	     * the composite type for each return value
	     */
	    if (n->child[1] != NULL)
	        {
		/* process the BODYRET of each CASE */
		for (m=n->child[1]; m!=NULL; m=m->link)
		    type_pass3_expr (m->child[1]);

		if (n->child[2] != NULL)
		    type_pass3_expr (n->child[2]);

		nret = tlist_length1 (n->types);

		/* for each of the return values, check compatibility and
		 * determine the composite type
		 */
		for (i=0, tr=n->types; i<nret; i++, tr=tr->link)
		    {
		    ntt = nth_type (n->child[1]->child[1]->types, i);

		    /* if this one has an error, just skip this pass */
		    if (ntt->stat == Err)
		        continue;

		    set_type_info_pass3 (tr, ntt->type, ntt->totsize, ntt->fracsize);

		    /* for each of the other CASE nodes... */
		    for (m=n->child[1]->link; m!=NULL; m=m->link)
		        {
			ntt = nth_type (m->child[1]->types, i);

			if (type_compatible[tr->type][ntt->type] == Err)
			    handle_error (ErrTypePass3SwitchValueIncompatible,
			    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc, i);
			else
			    {
			    make_composite_type (tr, ntt, &ty, &tsz, &fsz);
			    set_type_info_pass3 (tr, ty, tsz, fsz);
			    }
			}

		    if (n->child[2] != NULL)
		        {
			ntt = nth_type (n->child[2]->types, i);

			if (type_compatible[tr->type][ntt->type] == Err)
			    handle_error (ErrTypePass3SwitchValueIncompatible,
			    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc, i);
			else
			    {
			    make_composite_type (tr, ntt, &ty, &tsz, &fsz);
			    set_type_info_pass3 (tr, ty, tsz, fsz);
			    }
			}
		    }
		}
	    else
		{
		type_pass3_bodyret (n->child[2]);

	        for (tr=n->types, tt=n->child[2]->types;
				tr!=NULL && tt!=NULL; tr=tr->link, tt=tt->link)
		    if (tt->stat == Okay)
		        set_type_info_pass3 (tr, tt->type, tt->totsize, tt->fracsize);
		    else
		        tr->stat = Err;
		}
	    }
	    break;
        case FCALL :
	    {
	    NODEPNT m;
	    int fdv = n->child[0]->Defval;
	    int cnt;
	    TypeInfo *param, *tr, *ret;

	    /* check parameter types */
	    for (param=deftable[fdv].funcinfo->paramtypes, m=n->child[1], cnt=0;
	    		param!=NULL && m!=NULL; param=param->link, m=m->link, cnt++)
	        {
		type_pass3_expr (m);

		if (m->types->stat == Okay)
		    if (compare_typenodes_pass3 (param, m->types) == Err)
			handle_error (ErrTypePass3ParamIncompatible,
					Fatal, n->Lineno, n->Srcfile, n->Srcfunc, cnt);
		}

	    /* fill in types for this node */
	    for (ret=deftable[fdv].funcinfo->rettypes, tr=n->types;
	    		ret!=NULL && tr!=NULL; ret=ret->link, tr=tr->link)
		set_type_info_pass3 (tr, ret->type, ret->totsize, ret->fracsize);
	    }
	    break;
        case INTRINCALL :
	    {
	    NODEPNT m;
	    Tstat st = Okay;
	    int cnt;

	    /* check parameter types */
	    for (m=n->child[1], cnt=0; m!=NULL; m=m->link, cnt++)
	        {
		type_pass3_expr (m);

		if (m->types->stat == Okay)
		    if (! is_non_complex_num_type (m->types->type))
			{
			handle_error (ErrTypePass3IntrinArg,
					Fatal, n->Lineno, n->Srcfile, n->Srcfunc, cnt);
			st = Err;
			}
		}

	    /* fill in types for this node */
	    if (st == Okay)
	        switch (n->child[0]->Intrinval)
	            {
		    case IntrinFrexp :
		        /* the first return type is the same as the arg */
		        n->types->type = Double;
		        n->types->totsize = 64;
		        n->types->fracsize = 0;

		        /* the second return type is an int */
		        n->types->link->type = Int;
		        n->types->link->totsize = 32;
		        n->types->link->fracsize = 0;
		        break;
		    case IntrinModf :
		        /* the return types are the same as the arg */
		        n->types->type = Double;
			n->types->totsize = 64;
			n->types->fracsize = 0;
		        n->types->link->type = Double;
			n->types->link->totsize = 64;
			n->types->link->fracsize = 0;
		        break;
		    default :
		        n->types->type = Double;
			n->types->totsize = 64;
			n->types->fracsize = 0;
			break;
		    }
	    }
	    break;
        case BODYRET :
	    type_pass3_bodyret (n);
	    break;
	default :
            break;
	}
    }

void type_pass3_generators (NODEPNT n)
    {
    NODEPNT m;

    switch (n->nodetype)
        {
	case ELE_GEN :
	    type_pass3_ele_gen (n);
	    break;
	case SLICE_GEN :
	    type_pass3_slice_gen (n);
	    break;
	case SCALAR_GEN :
	    type_pass3_scalar_gen (n);
	    break;
	case WINDOW :
	    type_pass3_window_gen (n);
	    break;
	case DOT_PROD :
	case CROSS_PROD :
	    for (m=n->child[0]; m!=NULL; m=m->link)
	        type_pass3_generators (m);
	    break;
	default :
            break;
	}
    }

void type_pass3_ele_gen (NODEPNT n)
    {
    NODEPNT m;
    Type target_ty, source_ty, ty;
    int dv;

    type_pass3_expr (n->child[2]);

    /* check compatibility between generator source and target */
    if (n->child[2]->types->stat == Err)
        n->types->stat = Err;
    else
        {
	dv = n->child[0]->Defval;
	target_ty = deftable[dv].tinfo.type;
	source_ty = n->child[2]->types->type;

	if (target_ty == Unknown)
	    {
	    deftable[dv].tinfo.type = source_ty;
	    deftable[dv].tinfo.totsize = n->child[2]->types->totsize;
	    deftable[dv].tinfo.fracsize = n->child[2]->types->fracsize;
	    }
	else if (type_compatible[target_ty][source_ty] == Err)
	    {
	    handle_error (ErrTypePass3ElegenGenTargetIncompatible,
	    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	    n->types->stat = Err;
	    }
	}

    /* check step expressions */
    for (m=n->child[3]; m!=NULL; m=m->link)
	{
	if (m->nodetype != EMPTY)
            {
	    type_pass3_expr (m);
	    if (m->types->stat==Okay && non_int (m->types->type))
		{
		printf ("step type is %d\n", m->types->type);
	        handle_error (ErrTypePass3ElegenStepInt,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		}
	    }
	}

    /* check types of 'at' targets */
    for (m=n->child[4]; m!=NULL; m=m->link)
        if (m->nodetype == IDENT)
	    {
	    ty = deftable[m->Defval].tinfo.type;
	    if (non_int (ty))
		handle_error (ErTypePass3ElegenAtInt,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	    }
    }

void type_pass3_slice_gen (NODEPNT n)
    {
    NODEPNT m;
    Type target_ty, source_ty, ty;
    int dv;

    type_pass3_expr (n->child[2]);

    /* check compatibility between generator source and target */
    if (n->child[2]->types->stat == Err)
        n->types->stat = Err;
    else
        {
	dv = n->child[0]->Defval;
	target_ty = deftable[dv].tinfo.type;
	source_ty = n->child[2]->types->type;

	if (target_ty == Unknown)
	    {
	    deftable[dv].tinfo.type = source_ty;
	    deftable[dv].tinfo.totsize = n->child[2]->types->totsize;
	    deftable[dv].tinfo.fracsize = n->child[2]->types->fracsize;
	    }
	else if (type_compatible[target_ty][source_ty] == Err)
	    {
	    handle_error (ErrTypePass3SlicegenGenTargetIncompatible,
	    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	    n->types->stat = Err;
	    }
	}

    /* check step expressions */
    for (m=n->child[3]; m!=NULL; m=m->link)
	{
	if (m->nodetype != EMPTY)
            {
	    type_pass3_expr (m);
	    if (m->types->stat==Okay && non_int (m->types->type))
		{
		printf ("step type is %d\n", m->types->type);
	        handle_error (ErrTypePass3SlicegenStepInt,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		}
	    }
	}

    /* check types of 'at' targets */
    for (m=n->child[4]; m!=NULL; m=m->link)
        if (m->nodetype == IDENT)
	    {
	    ty = deftable[m->Defval].tinfo.type;
	    if (non_int (ty))
		handle_error (ErTypePass3SlicegenAtInt,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	    }
    }

void type_pass3_scalar_gen (NODEPNT n)
    {
    Type ty;
    NODEPNT m;

    /* check target variables */
    for (m=n->child[0]; m!=NULL; m=m->link)
	if (m->nodetype == IDENT)
            {
	    ty = deftable[m->Defval].tinfo.type;
	    if (non_int (ty))
		handle_error (ErrTypePass3ScalargenTargetInt,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	    }

    /* check source expressions */
    for (m=n->child[1]; m!=NULL; m=m->link)
	if (m->nodetype == TILDE)
	    {
	    type_pass3_expr (m->child[0]);
	    if (m->child[0]->types->stat==Okay && non_int (m->child[0]->types->type))
		handle_error (ErrTypePass3ScalargenSourceInt,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	    type_pass3_expr (m->child[1]);
	    if (m->child[1]->types->stat==Okay && non_int (m->child[1]->types->type))
		handle_error (ErrTypePass3ScalargenSourceInt,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	    }
	else
	    {
	    type_pass3_expr (m);
	    if (m->types->stat==Okay && non_int (m->types->type))
		handle_error (ErrTypePass3ScalargenSourceInt,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	    }

    /* check step expressions */
    for (m=n->child[2]; m!=NULL; m=m->link)
        {
	type_pass3_expr (m);
	if (m->types->stat==Okay && non_int (m->types->type))
	    handle_error (ErrTypePass3ScalargenStepInt,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	}
    }

void type_pass3_window_gen (NODEPNT n)
    {
    NODEPNT m;
    Type target_ty, source_ty, ty;
    int dv;

    /* check the window size expression */
    for (m=n->child[0]->child[1]; m!=NULL; m=m->link)
        {
	type_pass3_expr (m);
	if (m->types->stat == Okay)
	    {
	    ty = m->types->type;
	    if (non_int (ty))
		handle_error (ErrTypePass3WindowSizeNeedsInt,
			Fatal, m->Lineno, m->Srcfile, m->Srcfunc);
	    }
	}

    type_pass3_expr (n->child[1]);

    /* check compatibility between generator source and target */
    if (n->child[1]->types->stat == Err)
        n->types->stat = Err;
    else
        {
	dv = n->child[0]->Defval;
	target_ty = deftable[dv].tinfo.type;
	source_ty = n->child[1]->types->type;

	if (target_ty == Unknown)
	    {
	    deftable[dv].tinfo.type = source_ty;
	    deftable[dv].tinfo.totsize = n->child[1]->types->totsize;
	    deftable[dv].tinfo.fracsize = n->child[1]->types->fracsize;
	    }
	else if (type_compatible[target_ty][source_ty] == Err)
	    {
	    handle_error (ErrTypePass3WindowgenTargetIncompatible,
	    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	    n->types->stat = Err;
	    }
	}

    /* check step expressions */
    for (m=n->child[2]; m!=NULL; m=m->link)
        {
	type_pass3_expr (m);
	if (m->types->stat==Okay && non_int (m->types->type))
	    handle_error (ErrTypePass3WindowgenStepInt,
	    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	}

    /* check types of 'at' targets */
    for (m=n->child[3]; m!=NULL; m=m->link)
        if (m->nodetype == IDENT)
	    {
	    ty = deftable[m->Defval].tinfo.type;
	    if (non_int (ty))
	        handle_error (ErrTypePass3WindowgenAtInt,
	    			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	    }
    }

void check_arr_def_components (NODEPNT n, Type ty)
    {
    NODEPNT m;

    if (n == NULL)
        return;

    if (n->nodetype == ARR_SLICE)
        for (m=n; m!=NULL; m=m->link)
            check_arr_def_components (m->child[0], ty);
    else
        for (m=n; m!=NULL; m=m->link)
            {
            type_pass3_expr (m);
	    if (typecast_compatible[ty][m->types->type] == Err)
		handle_error (ErrTypePass3ArraydefIncompatible,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
            }
    }

TypeInfo *nth_type (TypeInfo *tt, int v)
    {
    int i;

    for (i=0; i<v; i++)
        tt=tt->link;

    return tt;
    }

void set_type_info_pass3 (TypeInfo *target, Type ty, int tsz, int fsz)
    {
    assert (target != NULL);

    target->type = ty;
    target->totsize = tsz;
    target->fracsize = fsz;
    }

int non_int (Type ty)
    {
    if (ty==Int || ty==Uint)
        return FALSE;
    else
        return TRUE;
    }

Tstat inconsistent_pass3 (int dv, TypeInfo *t)
    {
    Type ty;

    ty = deftable[dv].tinfo.type;
    return typecast_compatible[ty][t->type];
    }

Tstat compare_typenodes_pass3 (TypeInfo *t1, TypeInfo *t2)
    {
    return type_compatible[t1->type][t2->type];
    }

int bit_width (int val)
    {
    unsigned long bit, v = val;
    int cnt, width;

    if (v & 0x80000000)
        {
        v ^= 0xffffffff;

        for (cnt=0, bit = 0x80000000; bit>0; cnt++, bit=bit>>1)
            if (v & bit)
                break;

        width = 32-cnt+1;
        }
    else
        {
        for (cnt=0, bit = 0x80000000; bit>0; cnt++, bit=bit>>1)
            if (v & bit)
                break;

        width = 32-cnt;
        }

    return width==0?1:width;
    /*
    return width;
    */
    }

int is_numeric (Type ty)
    {
    switch (ty)
        {
	case Int :
	case Uint :
	case Fix :
	case Ufix :
	case Float :
	case Double :
	case CxInt :
	case CxFix :
	case CxFloat :
	case CxDouble :
	    return TRUE;
	default :
	    return FALSE;
	}
    }

int is_complex_num_type (Type ty)
    {
    switch (ty)
        {
	case CxInt :
	case CxFix :
	case CxFloat :
	case CxDouble :
	    return TRUE;
	default :
            break;
	}
    return FALSE;
    }

int is_non_complex_num_type (Type ty)
    {
    switch (ty)
        {
	case Uint :
	case Int :
	case Ufix :
	case Fix :
	case Float :
	case Double :
	    return TRUE;
	default :
            break;
	}
    return FALSE;
    }

/* this table shows the compatibilities for 2-operand arithmetic operators */
Tstat type_compatible[14][14] = {
 /*          */ /*  Tnone   Unknown Bits    Bool    Uint    Int     Ufix    Fix     Float   Double  CxInt   CxFix   CxFloat  CxDouble */
 /* --------------------------------------------------------------------------------------------------------------------------------- */
 /* Tnone    */      {Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,    Okay},
 /* Unknown  */      {Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,    Okay},
 /* Bits     */      {Okay,   Okay,   Okay,   Err,    Err,    Err,    Err,    Err,    Err,    Err,    Err,    Err,    Err,     Err}, 
 /* Bool     */      {Okay,   Okay,   Err,    Okay,   Err,    Err,    Err,    Err,    Err,    Err,    Err,    Err,    Err,     Err}, 
 /* Uint     */      {Okay,   Okay,   Err,    Err,    Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Err,    Err,    Err,     Err}, 
 /* Int      */      {Okay,   Okay,   Err,    Err,    Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Err,    Err,    Err,     Err}, 
 /* Ufix     */      {Okay,   Okay,   Err,    Err,    Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Err,    Err,    Err,     Err}, 
 /* Fix      */      {Okay,   Okay,   Err,    Err,    Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Err,    Err,    Err,     Err}, 
 /* Float    */      {Okay,   Okay,   Err,    Err,    Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Err,    Err,    Err,     Err}, 
 /* Double   */      {Okay,   Okay,   Err,    Err,    Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Err,    Err,    Err,     Err}, 
 /* CxInt    */      {Okay,   Okay,   Err,    Err,    Err,    Err,    Err,    Err,    Err,    Err,    Okay,   Okay,   Okay,    Okay},
 /* CxFix    */      {Okay,   Okay,   Err,    Err,    Err,    Err,    Err,    Err,    Err,    Err,    Okay,   Okay,   Okay,    Okay},
 /* CxFloat  */      {Okay,   Okay,   Err,    Err,    Err,    Err,    Err,    Err,    Err,    Err,    Okay,   Okay,   Okay,    Okay},
 /* CxDouble */      {Okay,   Okay,   Err,    Err,    Err,    Err,    Err,    Err,    Err,    Err,    Okay,   Okay,   Okay,    Okay}
 };

/* this table shows the typecast compatibilities (i.e., cast from Type A -> Type B is Okay/Err */
Tstat typecast_compatible[14][14] = {
 /*          */ /*  Tnone   Unknown Bits    Bool    Uint    Int     Ufix    Fix     Float   Double  CxInt   CxFix   CxFloat  CxDouble */
 /* --------------------------------------------------------------------------------------------------------------------------------- */
 /* Tnone    */      {Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,    Okay},
 /* Unknown  */      {Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,    Okay},
 /* Bits     */      {Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Err,    Err,    Err,    Err,    Err,     Err}, 
 /* Bool     */      {Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Err,    Err,    Err,     Err}, 
 /* Uint     */      {Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Err,    Err,    Err,     Err}, 
 /* Int      */      {Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Err,    Err,    Err,     Err}, 
 /* Ufix     */      {Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Err,    Err,    Err,     Err}, 
 /* Fix      */      {Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Err,    Err,    Err,     Err}, 
 /* Float    */      {Okay,   Okay,   Err,    Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Err,    Err,    Err,     Err}, 
 /* Double   */      {Okay,   Okay,   Err,    Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Okay,   Err,    Err,    Err,     Err}, 
 /* CxInt    */      {Okay,   Okay,   Err,    Err,    Err,    Err,    Err,    Err,    Err,    Err,    Okay,   Okay,   Okay,    Okay},
 /* CxFix    */      {Okay,   Okay,   Err,    Err,    Err,    Err,    Err,    Err,    Err,    Err,    Okay,   Okay,   Okay,    Okay},
 /* CxFloat  */      {Okay,   Okay,   Err,    Err,    Err,    Err,    Err,    Err,    Err,    Err,    Okay,   Okay,   Okay,    Okay},
 /* CxDouble */      {Okay,   Okay,   Err,    Err,    Err,    Err,    Err,    Err,    Err,    Err,    Okay,   Okay,   Okay,    Okay}
 };
