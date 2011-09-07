#include "sassy.h"
#include "node.h"
#include "deftab.h"
#include "error.h"

void type_pass1_bodyret (NODEPNT);
void type_pass1_stmt (NODEPNT);
void type_pass1_expr_rec (NODEPNT);
Tstat type_pass1_generators (NODEPNT);
Tstat type_pass1_scalar_gen (NODEPNT);
Tstat type_pass1_window_gen (NODEPNT);
Tstat type_pass1_ele_gen (NODEPNT);
Tstat type_pass1_slice_gen (NODEPNT);
Tstat type_pass1_slice_rec (NODEPNT);
TypeInfo *make_type_list1 (int);
Tstat check_child_as_single (NODEPNT, Error, int);
Tstat any_error (TypeInfo*);
Tstat any_error_exprs (NODEPNT);
TypeInfo *make_type_node ();
int tlist_length1 (TypeInfo*);
int tlist_length_exprs (NODEPNT);
void get_intrin_arity (Intrinsic, int*, int*);

void type_pass1 (NODEPNT root)
    {
    NODEPNT n, m, r;

    for (n=root; n!=NULL; n=n->link)
	{
        type_pass1_bodyret (n->child[3]);

        /* check arity of this function */
	for (m=n->child[0], r=n->child[3]->child[1];
				m!=NULL && r!=NULL; m=m->link, r=r->link);
	
	if (m!=NULL || r!=NULL)
	    handle_error (ErrTypePass1FuncArity,
		    		Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	}
    }

void type_pass1_bodyret (NODEPNT br)
    {
    NODEPNT s, e;
    Tstat st=Okay;
    int cnt;

    /*
    printf ("type_pass1_bodyret on node %d\n", br);
    fflush (stdout);
    */

    assert (br->nodetype == BODYRET);

    /* process its statement spine */
    for (s=br->child[0]; s!=NULL; s=s->link)
        type_pass1_stmt (s);

    /* process the return expressions */
    for (cnt=0, e=br->child[1]; e!=NULL; e=e->link)
	{
        type_pass1_expr_rec (e);
	if (any_error (e->types) == Err)
	    st = Err;
	cnt += tlist_length1 (e->types);
	}

    br->types = make_type_list1 (cnt);
    }

void type_pass1_stmt (NODEPNT s)
    {
    NODEPNT e, tg;
    int cnt, tcnt;

    /*
    printf ("type_pass1_stmt on node %d\n", s);
    fflush (stdout);
    */

    switch (s->nodetype)
        {
	case ASSIGN :
	    if (s->child[1]->nodetype == LOOP_INDICES)
	        {
		/* count the LHS */
		for (tg=s->child[0], tcnt=0; tg!=NULL; tg=tg->link, tcnt++);
		s->child[1]->types = make_type_list1 (tcnt);
		}
	    else
	        {
	        /* do the expressions on the RHS, and count them */
	        for (e=s->child[1], cnt=0; e!=NULL; e=e->link)
		    {
	            type_pass1_expr_rec (e);
		    cnt += tlist_length1 (e->types);
		    }

	        if (! any_error_exprs (s->child[1]))
	            {
	            /* count the targets on the LHS */
	            for (tg=s->child[0], tcnt=0; tg!=NULL; tg=tg->link, tcnt++);

	            if (cnt != tcnt)
		        handle_error (ErrTypePass1AssignMismatch,
		    		Fatal, s->Lineno, s->Srcfile, s->Srcfunc);
		    }
		}

	    break;
	case ASSERT :
	case PRINT :
	    /* do the boolean expression */
	    check_child_as_single (s->child[0],
	    	    ErrTypePass1PrintAssertPredMV, 0);

	    for (e=s->child[1]; e!=NULL; e=e->link)
	        if (e->nodetype != STRING)
		    check_child_as_single (e,
		    	ErrTypePass1PrintAssertValMV, 0);
	    break;
	default :
	    assert (0);
	}
    }

void type_pass1_expr_rec (NODEPNT n)
    {

    /*
    printf ("type_pass1_expr_rec on node %d, type %s\n", n, node_str (n->nodetype));
    fflush (stdout);
    */

    /* for every expression node, create TypeInfo structures */
    switch (n->nodetype)
        {
	/* leaf-nodes */
	case IDENT :
	case INTNUM :
	case FLOATNUM :
	case ICONST :
	case TRUE_ND :
	case FALSE_ND :
	    n->types = make_type_node ();
	    n->types->stat = Okay;
	    break;
	/* unary operators */
	case NOT :
	case NEG :
	case REAL :
	case IMAG :
	case CONSTRUCT_ARRAY :
	case CONSTRUCT_VECTOR :
	case CONSTRUCT_MATRIX :
	case CONSTRUCT_CUBE :
	case CONSTRUCT_TILE :
	    {
	    Tstat st0;

	    n->types = make_type_node ();
	    st0 = check_child_as_single (n->child[0],
	    	    ErrTypePass1UnaryValMV, n->nodetype);
	    n->types->stat = st0;
	    }
	    break;
	/* binary operators */
	case ADD :
	case SUB :
	case MUL :
	case DIV :
	case MOD :
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
	case COMPLEX :
	    {
	    Tstat st0, st1;

	    n->types = make_type_node ();

	    /* do LHS */
	    st0 = check_child_as_single (n->child[0],
	    	    ErrTypePass1BinaryValMV, n->nodetype);

	    /* do RHS */
	    st1 = check_child_as_single (n->child[1],
	    	    ErrTypePass1BinaryValMV, n->nodetype);

	    if (st0==Err || st1==Err)
	        n->types->stat = Err;
	    }
	    break;
	/* trinary operators */
	case ARR_CONPERIM :
	case ARR_ACCUM :
	case ACCUM :
	    {
	    Tstat st0, st1, st2;

	    n->types = make_type_node ();

	    /* do LHS */
	    st0 = check_child_as_single (n->child[0],
	    	    ErrTypePass1TrinaryValMV, n->nodetype);

	    /* do middle */
	    st1 = check_child_as_single (n->child[1],
	    	    ErrTypePass1TrinaryValMV, n->nodetype);

	    /* do RHS */
	    st2 = check_child_as_single (n->child[2],
	    	    ErrTypePass1TrinaryValMV, n->nodetype);

	    if (st0==Err || st1==Err || st2==Err)
	        n->types->stat = Err;
	    }
	    break;
	/* various special cases */
        case CAST :
	    {
	    Tstat st1;

	    n->types = make_type_node ();
	    st1 = check_child_as_single (n->child[1],
	    	    ErrTypePass1CastMV, 0);
	    n->types->stat = st1;
	    }
	    break;
	case REDUCE_SUM :
	case REDUCE_PRODUCT :
	case REDUCE_MIN :
	case REDUCE_MAX :
	case REDUCE_AND :
	case REDUCE_OR :
	case REDUCE_MEDIAN :
	case REDUCE_HIST :
	case REDUCE_MODE :
	case REDUCE_ST_DEV :
	case REDUCE_MEAN :
	case ARR_REDUCE_SUM :
	case ARR_REDUCE_PRODUCT :
	case ARR_REDUCE_MIN :
	case ARR_REDUCE_MAX :
	case ARR_REDUCE_AND :
	case ARR_REDUCE_OR :
	case ARR_REDUCE_MEDIAN :
	case ARR_REDUCE_MIN_INDICES :
	case ARR_REDUCE_MAX_INDICES :
	case ARR_REDUCE_HIST :
	case ARR_CONCAT :
	case ARR_REDUCE_MODE :
	case ARR_REDUCE_ST_DEV :
	case ARR_REDUCE_MEAN :
	case CONSTRUCT_CONCAT :
	    {
	    Tstat st0=Okay, st1=Okay, st2=Okay;

	    n->types = make_type_node ();

	    st0 = check_child_as_single (n->child[0],
	    	    ErrTypePass1ReductionArg0MV, 0);

	    if (n->child[1] != NULL)
	        st1 = check_child_as_single (n->child[1],
			    ErrTypePass1ReductionArg1MV, 0);

	    if (n->child[2] != NULL)
	        st2 = check_child_as_single (n->child[2],
			    ErrTypePass1ReductionArg2MV, 0);

	    if (st0==Err || st1==Err || st2==Err)
	        n->types->stat = Err;
	    }
	    break;
	case REDUCE_VAL_AT_FIRST_MIN :
	case REDUCE_VAL_AT_FIRST_MAX :
	case REDUCE_VAL_AT_LAST_MIN :
	case REDUCE_VAL_AT_LAST_MAX :
	case REDUCE_VAL_AT_MINS :
	case REDUCE_VAL_AT_MAXS :
	    {
	    Tstat st0=Okay, st1=Okay, st2=Okay;
	    NODEPNT m;

	    n->types = make_type_node ();

	    st0 = check_child_as_single (n->child[0],
	    	    ErrTypePass1ReductionArg0MV, 0);

	    for (m=n->child[2]; m!=NULL; m=m->link)
	        if (check_child_as_single (m, ErrTypePass1ValsAt1MV, 0) == Err)
		    st1 = Err;

	    if (n->child[1] != NULL)
	        st2 = check_child_as_single (n->child[1],
			    ErrTypePass1ReductionArg2MV, 0);

	    if (st0==Err || st1==Err || st2==Err)
	        n->types->stat = Err;
	    }
	    break;
	case ARRAYREF :
	    {
	    NODEPNT s;
	    Tstat st0=Okay, st1=Okay, stx;

	    n->types = make_type_node ();

	    if (n->child[0] != NULL)
	        st0 = check_child_as_single (n->child[0],
			    ErrTypePass1ArrayRefMV, 0);

	    /* go down the list of triples and expressions */
	    for (s=n->child[1]; s!=NULL; s=s->link)
	        if (s->nodetype == TRIPLE)
		    {
		    if (s->child[0] != NULL)
			{
		        stx = check_child_as_single (s->child[0],
				    ErrTypePass1ArraySubscrMV, 0);
			if (stx == Err)
			    st1 = Err;
			}
		    if (s->child[1] != NULL)
			{
		        stx = check_child_as_single (s->child[1],
				    ErrTypePass1ArraySubscrMV, 0);
			if (stx == Err)
			    st1 = Err;
			}
		    if (s->child[2] != NULL)
			{
		        stx = check_child_as_single (s->child[2],
				    ErrTypePass1ArraySubscrMV, 0);
			if (stx == Err)
			    st1 = Err;
			}
		    }
		else
		    {
	            stx = check_child_as_single (s,
		    	    ErrTypePass1ArraySubscrMV, 0);
		    if (stx == Err)
		        st1 = Err;
		    }

	    if (st0==Err || st1==Err)
	        n->types->stat = Err;
	    }
	    break;
	case ARR_SLICE :
	    n->types = make_type_node ();

	    if (type_pass1_slice_rec (n->child[0]) == Err)
	        n->types->stat = Err;

	    break;
	/* expressions with possible multiple-values */
	case EXTENTS :
	    {
	    int dv, dims;

	    n->child[0]->types = make_type_node ();

	    dv = n->child[0]->Defval;

	    if (deftable[dv].tinfo.kind != Array)
		{
		handle_error (ErrTypePass1ExtentsNonarray, Fatal,
			n->child[0]->Lineno, n->child[0]->Srcfile, n->child[0]->Srcfunc);
		n->types = make_type_node ();
		n->types->stat = Err;
		}
	    else
	        {
	        dims = deftable[dv].tinfo.dims;
		n->types = make_type_list1 (dims);
		n->types->stat = Okay;
		}
	    }
	    break;
	case CONDITIONAL :
	    {
	    Tstat st0=Okay, st1=Okay, st2=Okay;
	    int tcnt=0, fcnt=0;

	    /* check predicate */
	    st0 = check_child_as_single (n->child[0],
	    	    ErrTypePass1CondPredMV, 0);

	    /* handle true expressions */
	    type_pass1_bodyret (n->child[1]);
	    st1 = any_error (n->child[1]->types);

	    /* handle false expressions */
	    type_pass1_bodyret (n->child[2]);
	    st2 = any_error (n->child[2]->types);

	    if (st0==Err || st1==Err)
	        {
		n->types = make_type_node ();
		n->types->stat = Err;
		}
	    else
	        {
		tcnt = tlist_length1 (n->child[1]->types);
		fcnt = tlist_length1 (n->child[2]->types);

	        if (tcnt != fcnt)
		    {
		    n->types = make_type_node ();
		    n->types->stat = Err;
		    handle_error (ErrTypePass1UnbalancedCond,
		    		Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		    }
	        else
	            n->types = make_type_list1 (tcnt);
		}
	    }
	    break;
	case WHILE :
	    {
	    Tstat st0=Okay, st1=Okay;
	    int cnt;

	    /* check predicate */
	    st0 = check_child_as_single (n->child[0],
	    	    ErrTypePass1WhilePredMV, 0);

	    type_pass1_bodyret (n->child[1]);
	    st1 = any_error (n->child[1]->types);

	    if (st0==Err || st1==Err)
	        {
		n->types = make_type_node ();
		n->types->stat = Err;
		}
	    else
	        {
	        cnt = tlist_length1 (n->child[1]->types);
	        n->types = make_type_list1 (cnt);
		}
	    }
	    break;
	case FOR :
	    {
	    Tstat st0=Okay, st1;
	    int cnt;

	    st0 = type_pass1_generators (n->child[0]);

	    type_pass1_bodyret (n->child[1]);
	    st1 = any_error (n->child[1]->types);

	    if (st0==Err || st1==Err)
	        {
		n->types = make_type_node ();
		n->types->stat = Err;
		}
	    else
	        {
	        cnt = tlist_length1 (n->child[1]->types);
	        n->types = make_type_list1 (cnt);
		}
	    }
	    break;
	case SWITCH :
	    {
	    NODEPNT m, w;
	    Tstat st0=Okay, st1=Okay, st2=Okay;
	    int cnt, err=FALSE;

	    /* check the switch's expression */
	    st0 = check_child_as_single (n->child[0],
	    	    ErrTypePass1SwitchExprMV, 0);

	    /* process each CASE node */
	    for (m=n->child[1]; m!=NULL; m=m->link)
	        {
		/* this part will simply tag each value with a TypeInfo */
		for (w=m->child[0]; w!=NULL; w=w->link)
		    type_pass1_expr_rec (w);

		/* process the return values for this CASE node */
		type_pass1_bodyret (m->child[1]);
		if (any_error (m->child[1]->types))
		    st1 = Err;
		}

	    /* process the 'default' returns */
	    if (n->child[2] != NULL)
		{
	        type_pass1_bodyret (n->child[2]);
		if (any_error (n->child[2]->types))
		    st2 = Err;
		}

	    /* if there is at least one CASE node... */
	    if (n->child[1] != NULL)
	        {
		cnt = tlist_length1 (n->child[1]->child[1]->types);

		/* check every other CASE node */
		for (m=n->child[1]->link; m!=NULL; m=m->link)
		    if (cnt != tlist_length1 (m->child[1]->types))
			{
			err = TRUE;
			handle_error (ErrTypePass1SwitchValMismatch,
					Fatal, m->Lineno, m->Srcfile, m->Srcfunc);
			break;
			}

		/* check the 'default' returns */
		if ((err == FALSE) && (n->child[2] != NULL) &&
				(cnt != tlist_length1 (n->child[2]->types)))
		    {
		    err = TRUE;
		    handle_error (ErrTypePass1SwitchDefaultMismatch, Fatal,
		    	n->child[2]->Lineno, n->child[2]->Srcfile, n->child[2]->Srcfunc);
		    }
		}
	    else
	        {
		/* there were no CASE nodes */
		if (n->child[2] == NULL)
		    assert (0);
		else
		    cnt = tlist_length1 (n->child[2]->types);
		}

	    if (err==TRUE || st0==Err || st1==Err || st2==Err)
	        {
		n->types = make_type_node ();
		n->types->stat = Err;
		}
	    else
	        n->types = make_type_list1 (cnt);
	    }
	    break;
	case FCALL :
	    {
	    NODEPNT prm;
	    Tstat st=Okay;
	    int fdv = n->child[0]->Defval;
	    int cnt;

	    if (deftable[fdv].tinfo.kind != Func)
		{
	        handle_error (ErrTypePass1CallNonFunc,
			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		st = Err;
		}
	    else
	        {
	        for (cnt=0, prm=n->child[1]; prm!=NULL; cnt++, prm=prm->link)
		    {
		    if (check_child_as_single (prm,
					ErrTypePass1ParamMV, 0) == Err)
		        st = Err;
		    }

	        if (cnt != deftable[fdv].funcinfo->nparams)
		    {
	            handle_error (ErrTypePass1ParamCount,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		    st = Err;
		    }
		}

	    if (st == Okay)
	        n->types = make_type_list1 (deftable[fdv].funcinfo->nreturns);
	    else
	        {
		n->types = make_type_node ();
		n->types->stat = Err;
		}
	    }
	    break;
	case INTRINCALL :
	    {
	    NODEPNT prm;
	    Tstat st=Okay;
	    int cnt, nparams, nrets;

	    for (cnt=0, prm=n->child[1]; prm!=NULL; cnt++, prm=prm->link)
		{
		if (check_child_as_single (prm,
				ErrTypePass1ParamMV, 0) == Err)
		    st = Err;
		}

	    /* check arity of intrinsic function */
	    get_intrin_arity (n->child[0]->Intrinval, &nparams, &nrets);

	    if (cnt != nparams)
		{
	        handle_error (ErrTypePass1ParamCount,
			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		st = Err;
		}

	    if (st == Okay)
	        n->types = make_type_list1 (nrets);
	    else
	        {
		n->types = make_type_node ();
		n->types->stat = Err;
		}
	    }
	    break;
	case BODYRET :
	    type_pass1_bodyret (n);
	    break;
	default :
	    assert (0);
	}
    }

Tstat type_pass1_generators (NODEPNT gen)
    {
    Tstat st = Okay, st0;
    NODEPNT n;

    switch (gen->nodetype)
        {
	case SCALAR_GEN :
	    st = type_pass1_scalar_gen (gen);
	    break;
	case ELE_GEN :
	    st = type_pass1_ele_gen (gen);
	    break;
	case SLICE_GEN :
	    st = type_pass1_slice_gen (gen);
	    break;
	case WINDOW :
	    st = type_pass1_window_gen (gen);
	    break;
	case DOT_PROD :
	case CROSS_PROD :
	    for (n=gen->child[0]; n!=NULL; n=n->link)
	        {
		st0 = type_pass1_generators (n);
		if (st0 == Err)
		    st = Err;
		}

    	    gen->types = make_type_node ();
    	    if (st == Err)
		gen->types->stat = Err;

	    break;
	default :
	    assert (0);
	}

    return st;
    }

Tstat type_pass1_window_gen (NODEPNT gen)
    {
    NODEPNT n, g;
    Tstat st = Okay, st0;

    /* check window size expressions */
    for (n=gen->child[0]->child[1]; n!=NULL; n=n->link)
        if (check_child_as_single (n, ErrTypePass1WindowSizeMV, 0) == Err)
	    st = Err;

    /* check source expression */
    if (check_child_as_single (gen->child[1],
    				ErrTypePass1WindowSourceMV, 0) == Err)
        st = Err;

    /* check optional step expressions */
    if (gen->child[2] != NULL)
        for (g=gen->child[2]; g!=NULL; g=g->link)
	    {
	    st0 = check_child_as_single (g,
	    	    ErrTypePass1WindowStepMV, 0);
	    if (st0 == Err)
	        st = Err;
	    }

    gen->types = make_type_node ();
    if (st == Err)
        gen->types->stat = Err;

    return st;
    }

Tstat type_pass1_scalar_gen (NODEPNT gen)
    {
    NODEPNT g;
    Tstat st = Okay, st0, st1;
    int target_cnt, srce_cnt, step_cnt;

    for (g=gen->child[0]; g!=NULL; g=g->link)
	g->types = make_type_node ();

    target_cnt = list_length (gen->child[0]);
    srce_cnt = list_length (gen->child[1]);
    step_cnt = list_length (gen->child[2]);

    if (target_cnt != srce_cnt)
	{
	handle_error (ErrTypePass1ScalarGenMismatch,
		Fatal, gen->Lineno, gen->Srcfile, gen->Srcfunc);
	st = Err;
	}
    else if ((step_cnt != 0) && (step_cnt != target_cnt))
	{
	handle_error (ErrTypePass1ScalarGenStepMismatch,
		Fatal, gen->Lineno, gen->Srcfile, gen->Srcfunc);
	st = Err;
	}
    else
        {
	for (g=gen->child[1]; g!=NULL; g=g->link)
	    {
	    if (g->nodetype == TILDE)
	        {
		st0 = check_child_as_single (g->child[0],
			    ErrTypePass1ScalarGenExprMV, 0);
		st1 = check_child_as_single (g->child[1],
			    ErrTypePass1ScalarGenExprMV, 0);
		}
	    else
		{
	        st0 = check_child_as_single (g,
			    ErrTypePass1ScalarGenExprMV, 0);
		st1 = Okay;
		}

	    if ((st0==Err) || (st1==Err))
	        st = Err;
	    }

	if (gen->child[2] != NULL)
	    for (g=gen->child[2]; g!=NULL; g=g->link)
		{
	        st0 = check_child_as_single (g,
			    ErrTypePass1ScalarGenStepMV, 0);
		if (st0 == Err)
		    st = Err;
		}
	}

    gen->types = make_type_node ();
    if (st == Err)
        gen->types->stat = Err;

    return st;
    }

Tstat type_pass1_ele_gen (NODEPNT gen)
    {
    NODEPNT g;
    Tstat st, st0;

    st = check_child_as_single (gen->child[2],
    	    ErrTypePass1EleGenSourceMV, 0);

    /* check optional step expressions */
    for (g=gen->child[3]; g!=NULL; g=g->link)
	if (g->nodetype != EMPTY)
	    {
	    st0 = check_child_as_single (g,
	    	    ErrTypePass1ElegenStepMV, 0);
	    if (st0 == Err)
	        st = Err;
	    }

    gen->types = make_type_node ();
    if (st == Err)
        gen->types->stat = Err;

    return st;
    }

Tstat type_pass1_slice_gen (NODEPNT gen)
    {
    NODEPNT g;
    Tstat st, st0;

    st = check_child_as_single (gen->child[2],
    	    ErrTypePass1SliceGenSourceMV, 0);

    /* check optional step expressions */
    for (g=gen->child[3]; g!=NULL; g=g->link)
	if (g->nodetype != EMPTY)
	    {
	    st0 = check_child_as_single (g,
	    	    ErrTypePass1SlicegenStepMV, 0);
	    if (st0 == Err)
	        st = Err;
	    }

    gen->types = make_type_node ();
    if (st == Err)
        gen->types->stat = Err;

    return st;
    }

Tstat type_pass1_slice_rec (NODEPNT s)
    {
    NODEPNT n;
    Tstat st = Okay, st0;

    for (n=s; n!=NULL; n=n->link)
        if (n->nodetype == ARR_SLICE)
	    {
	    st0 = type_pass1_slice_rec (n->child[0]);
	    if (st0 == Err)
	        st = Err;
	    }
	else
	    {
	    st0 = check_child_as_single (n,
	    	    ErrTypePass1ArrayDefValMV, 0);
	    if (st0 == Err)
	        st = Err;
	    }

    return st;
    }

TypeInfo *make_type_list1 (int n)
    {
    TypeInfo *ret=NULL, **p;
    int i;

    p = &(ret);
    for (i=0; i<n; i++)
	{
	*p = make_type_node ();
	p = &((*p)->link);
	}
    return ret;
    }

Tstat check_child_as_single (NODEPNT n, Error err, int v)
    {
    type_pass1_expr_rec (n);

    if (n->types->stat == Okay)
	{
	if (n->types->link != NULL)
	    {
	    handle_error (err, Fatal, n->Lineno, n->Srcfile, n->Srcfunc, v);
	    return Err;
	    }
	else
	    return Okay;
	}
    else
	{
        return Err;
	}
    }

Tstat any_error (TypeInfo *t)
    {
    TypeInfo *p;

    for (p=t; p!=NULL; p=p->link)
        if (p->stat == Err)
	    return Err;

    return Okay;
    }

Tstat any_error_exprs (NODEPNT e)
    {
    NODEPNT n;

    for (n=e; n!=NULL; n=n->link)
        if (any_error (n->types))
	    return Err;

    return Okay;
    }

TypeInfo *make_type_node ()
    {
    TypeInfo *t;
    int i;

    SacMalloc (t, TypeInfo*, sizeof (TypeInfo))
    t->stat = Okay;
    t->type = Unknown;
    t->totsize = 0;
    t->fracsize = 0;
    t->kind = Knone;
    t->dims = 0;
    for (i=0; i<MAXRANK; i++)
        (t->dim_sizes)[i] = -1;
    t->link = NULL;
    return t;
    }

int tlist_length1 (TypeInfo *t)
    {
    TypeInfo *tt;
    int cnt;

    for (cnt=0, tt=t; tt!=NULL; tt=tt->link, cnt++);
    return cnt;
    }

int tlist_length_exprs (NODEPNT e)
    {
    NODEPNT n;
    int cnt;

    for (n=e; n!=NULL; n=n->link)
        cnt += tlist_length1 (n->types);

    return cnt;
    }

void get_intrin_arity (Intrinsic ty, int *nparams, int *nrets)
    {
    switch (ty)
        {
	case IntrinSin :
	case IntrinCos :
	case IntrinTan :
	case IntrinAsin :
	case IntrinAcos :
	case IntrinAtan :
	case IntrinSinh :
	case IntrinCosh :
	case IntrinTanh :
	case IntrinAsinh :
	case IntrinAcosh :
	case IntrinAtanh :
	case IntrinSqrt :
	case IntrinCbrt :
	case IntrinExp :
	case IntrinLog :
	case IntrinLog10 :
	case IntrinExpm1 :
	case IntrinLog1p :
	case IntrinCeil :
	case IntrinFabs :
	case IntrinFloor :
	case IntrinRint :
	    *nparams = 1;
	    *nrets = 1;
	    break;
	case IntrinHypot :
	case IntrinAtan2 :
	case IntrinFmod :
	case IntrinCopysign :
	case IntrinPow :
	case IntrinLdexp :
	    *nparams = 2;
	    *nrets = 1;
	    break;
	case IntrinModf :
	case IntrinFrexp :
	    *nparams = 1;
	    *nrets = 2;
	    break;
	default :
	    assert (0);
	}
    }
