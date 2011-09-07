#include "sassy.h"
#include "node.h"
#include "deftab.h"
#include "error.h"

void type_pass2_bodyret (NODEPNT);
void type_pass2_stmt (NODEPNT);
void type_pass2_expr (NODEPNT);
void copy_typeinfo_pass2 (TypeInfo*, TypeInfo*);
void set_scalar (NODEPNT);
void set_array (NODEPNT, int);
Tstat inconsistent_pass2 (int, TypeInfo*);
Tstat check_expr (NODEPNT, Kind, Error, Node);
Tstat compare_typenodes_pass2 (TypeInfo*, TypeInfo*);
Tstat type_pass2_generators (NODEPNT);
Tstat type_pass2_scalar_gen (NODEPNT);
Tstat type_pass2_ele_gen (NODEPNT);
Tstat type_pass2_slice_gen (NODEPNT);
Tstat type_pass2_window_gen (NODEPNT);
void set_rank_of_constructors (NODEPNT, int);
void show_typeinfo (TypeInfo*);
void show_typeinfo_list (TypeInfo*);
Tstat check_array_dimensions (NODEPNT, int, int*);
Tstat check_array_scalars (NODEPNT);
Tstat check_for_case_val_overlap (NODEPNT);
void init_dim_sizes (NODEPNT);
void copy_dim_sizes (TypeInfo*, TypeInfo*);
void push_rank (int);
void pop_rank ();
int tlist_length1 (TypeInfo*);
Tstat any_error (TypeInfo*);

#define G_STK_SIZE 128

int g_rank_stk[G_STK_SIZE];
int g_rank_idx = 0;

/* This pass assumes that all multiple-valued BODYRETs, etc, are
 * properly balanced with their target variable lists, and that
 * all expression nodes are annotated with empty TypeInfo structs.
 * These routines fill in the 'kind', 'dims', and 'dimsizes'
 * fields of the TypeInfo nodes.
 */
void type_pass2 (NODEPNT root)
{
  NODEPNT n;
  TypeInfo *rtypes, *tt, *tr;
  Tstat st;
  int fdv, cnt;

  for (n=root; n!=NULL; n=n->link)
    {
      /* process the function's body */
      type_pass2_bodyret (n->child[3]);

      fdv = n->child[1]->Defval;
      rtypes = deftable[fdv].funcinfo->rettypes;

      /* check return values vs return types in decl */
      for (cnt=0, tt=n->child[3]->types, tr=rtypes;
	   tt!=NULL && tr!=NULL; cnt++, tt=tt->link, tr=tr->link)
	if (tt->stat == Okay)
	  {
	    st = compare_typenodes_pass2 (tt, tr);
	    if (st == Err)
	      handle_error (ErrTypePass2FuncRetMismatch, Fatal,
			    n->child[3]->Lineno, n->child[3]->Srcfile, 
			    n->child[3]->Srcfunc, cnt);
	  }
    }
}

void type_pass2_bodyret (NODEPNT br)
{
  NODEPNT s, e;
  TypeInfo *t, *v;

  /*
    printf ("type_pass2_bodyret on node %d\n", br);
    fflush (stdout);
  */

  assert (br->nodetype == BODYRET);

  /* process its statement spine */
  for (s=br->child[0]; s!=NULL; s=s->link)
    type_pass2_stmt (s);

  /* process its return expressions */
  for (s=br->child[1]; s!=NULL; s=s->link)
    type_pass2_expr (s);

  /* copy the TypeInfo node information from the return expressions
   * to the BODYRET's TypeInfo nodes
   */
  for (t=br->types, e=br->child[1]; e!=NULL; e=e->link)
    for (v=e->types; v!=NULL; v=v->link)
      {
	copy_typeinfo_pass2 (t, v);
	t=t->link;
      }
}

void type_pass2_stmt (NODEPNT s)
{
  NODEPNT lhs, rhs, n;
  TypeInfo *t;
  int cnt, rank;

  /*
    printf ("type_pass2_stmt on node %d\n", s);
    fflush (stdout);
  */

  switch (s->nodetype)
    {
    case ASSIGN :
      if (s->child[1]->nodetype == LOOP_INDICES)
	{
	  rank = g_rank_stk[g_rank_idx-1];

	  if (rank < 1)
	    handle_error (ErrTypePass2LoopIndOutsideOfLoop,
			  Fatal, s->Lineno, s->Srcfile, s->Srcfunc);
	  else
	    {
	      for (cnt=0, lhs=s->child[0]; lhs!=NULL; cnt++, lhs=lhs->link);

	      if (cnt != rank)
		{
		  printf ("cnt = %d, rank = %d\n", cnt, rank);
		  handle_error (ErrTypePass2LoopIndDimMismatch,
				Fatal, s->Lineno, s->Srcfile, s->Srcfunc);
		}

	      for (t=s->child[1]->types; t!=NULL; t=t->link)
		{
		  t->type = Uint;
		  t->totsize = 32;
		  t->fracsize = 0;
		  t->kind = Scalar;
		}

	      for (lhs=s->child[0]; lhs!=NULL; lhs=lhs->link)
		if (lhs->nodetype == IDENT)
		  if (deftable[lhs->Defval].tinfo.kind != Scalar)
		    handle_error (ErrTypePass2LoopIndScalar,
				  Fatal, s->Lineno, s->Srcfile, s->Srcfunc);
	    }
	}
      else if (s->child[1]->nodetype == ARR_SLICE)
	{
	  NODEPNT m;
	  Tstat st;
	  int extents[MAXRANK], ndim, i;

	  /* get the rank and extents */
	  for (m=s->child[0]->child[1], ndim=0; m!=NULL; m=m->link, ndim++)
	    extents[ndim] = m->Iconst;

	  st = check_array_dimensions (s->child[1]->child[0], ndim, extents);
	  if (st == Err)
	    s->child[1]->types->stat = Err;

	  st = check_array_scalars (s->child[1]->child[0]);
	  if (st == Err)
	    s->child[1]->types->stat = Err;

	  if (s->child[1]->types->stat == Okay)
	    {
	      s->child[1]->types->kind = Array;
	      s->child[1]->types->dims = ndim;
	      init_dim_sizes (s->child[1]);
	      for (i=0; i<ndim; i++)
		s->child[1]->types->dim_sizes[i] = extents[i];
	    }
	}
      else
	{
	  /* process the expressions on the RHS */
	  for (rhs=s->child[1]; rhs!=NULL; rhs=rhs->link)
	    type_pass2_expr (rhs);

	  /* check consistency between LHS and RHS */
	  for (cnt=0, lhs=s->child[0], rhs=s->child[1]; rhs!=NULL; rhs=rhs->link)
	    for (t=rhs->types; t!=NULL; cnt++, t=t->link)
	      {
		if (lhs->nodetype == IDENT)
		  if (inconsistent_pass2 (lhs->Defval, t) == Err)
		    handle_error (ErrTypePass2AssignValMismatch,
				  Fatal, s->Lineno, s->Srcfile, s->Srcfunc, cnt);

		lhs=lhs->link;
	      }
	}
      break;
    case ASSERT :
    case PRINT :
      /* process the predicate expression */
      type_pass2_expr (s->child[0]);

      if ((s->child[0]->types->stat==Okay) &&
	  (s->child[0]->types->kind!=Scalar))
	handle_error (ErrTypePass2PrintAssertPredScalar,
		      Fatal, s->Lineno, s->Srcfile, s->Srcfunc, cnt);

      for (n=s->child[1]; n!=NULL; n=n->link)
	if (n->nodetype != STRING)
	  type_pass2_expr (n);
      break;
    default :
      break;
    }
}

void type_pass2_expr (NODEPNT n)
{
  Tstat st;

  assert (n != NULL);

  /*
    printf ("type_pass2_expr, node %d, type %s\n", n, node_str (n->nodetype));
    fflush (stdout);
  */

  switch (n->nodetype)
    {
    case IDENT :
      {
	int dv = n->Defval;

	assert (n->types != NULL);

	if (deftable[dv].tinfo.kind == Scalar)
	  set_scalar (n);
	else
	  {
	    n->types->kind = Array;
	    n->types->dims = deftable[dv].tinfo.dims;
	    copy_dim_sizes (n->types, &(deftable[dv].tinfo));
	  }
      }
      break;
    case INTNUM :
    case FLOATNUM :
    case ICONST :
    case TRUE_ND :
    case FALSE_ND :
      set_scalar (n);
      break;
    case NOT :
    case NEG :
    case REAL :
    case IMAG :
      type_pass2_expr (n->child[0]);
      st = check_expr (n->child[0], Scalar,
		       ErrTypePass2UnaryOpScalar, n->nodetype);

      if (st == Okay)
	set_scalar (n);
      break;
    case CONSTRUCT_ARRAY :
    case CONSTRUCT_VECTOR :
    case CONSTRUCT_MATRIX :
    case CONSTRUCT_CUBE :
      /* we can construct these out of both scalars and arrays */
      type_pass2_expr (n->child[0]);
      n->types->kind = Array;
      init_dim_sizes (n);

      /* note: loop node will have to set the rank */
      break;
    case CONSTRUCT_CONCAT :
      /* only arrays can be concatenated */
      type_pass2_expr (n->child[0]);
      st = check_expr (n->child[0], Array,
		       ErrTypePass2ConcatNeedsArray, 0);

      if (st == Okay)
	{
	  /* note: loop node will have to set the rank later */
	  n->types->kind = Array;

	  /* if there is a mask, be sure that it's a 1d array */
	  if (n->child[1] != NULL)
	    {
	      type_pass2_expr (n->child[1]);
	      st = check_expr (n->child[1], Array,
			       ErrTypePass2ConcatMaskNeeds1dArray, 0);
	      if (st == Okay)
		{
		  if (n->child[1]->types->dims != 1)
		    {
		      handle_error (ErrTypePass2ConcatMaskNeeds1dArray,
				    Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		      n->types->stat = Err;
		    }
		}
	      else
		n->types->stat = Err;
	    }
	}
      else
	n->types->stat = Err;
      break;
    case CONSTRUCT_TILE :
      /* only arrays can be tiled */
      type_pass2_expr (n->child[0]);
      st = check_expr (n->child[0], Array,
		       ErrTypePass2ConcatNeedsArray, 0);

      if (st == Okay)
	{
	  /* note: loop node will have to set the rank later */
	  n->types->kind = Array;
	}
      else
	n->types->stat = Err;
      break;
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
	Tstat st0=Okay, st1=Okay;

	/* these require scalar operands */
	type_pass2_expr (n->child[0]);
	st0 = check_expr (n->child[0], Scalar,
			  ErrTypePass2OpNeedsScalars, n->nodetype);

	type_pass2_expr (n->child[1]);
	st1 = check_expr (n->child[1], Scalar,
			  ErrTypePass2OpNeedsScalars, n->nodetype);

	if (st0==Okay && st1==Okay)
	  set_scalar (n);
	else
	  n->types->stat = Err;
      }
      break;
    case ARR_CONPERIM :
      {
	Tstat st0=Okay, st1=Okay, st2=Okay;

	/* check source array */
	type_pass2_expr (n->child[0]);
	st0 = check_expr (n->child[0], Array,
			  ErrTypePass2PerimSourceArray, 0);

	/* check perimeter width */
	type_pass2_expr (n->child[1]);
	st1 = check_expr (n->child[1], Scalar,
			  ErrTypePass2PerimWidthScalar, 0);

	/* check perimeter value */
	type_pass2_expr (n->child[2]);
	st2 = check_expr (n->child[2], Scalar,
			  ErrTypePass2PerimValScalar, 0);

	if (st0==Err || st1==Err || st2==Err)
	  n->types->stat = Err;
	else
	  /* HERE WE SHOULD SEE WHETHER ANY DIMENSIONS OF
	   * THE NEW ARRAY CAN BE STATICALLY KNOWN...
	   */
	  set_array (n, n->child[0]->types->dims);
      }
      break;
    case ARR_ACCUM :
      {
	Tstat st0=Okay, st1=Okay, st2=Okay;

	/* check the array reduction */
	type_pass2_expr (n->child[0]);
	st0 = n->child[0]->types->stat;

	/* check the accum range value */
	type_pass2_expr (n->child[1]);
	st1 = check_expr (n->child[1], Scalar,
			  ErrTypePass2AccumRangeScalar, 0);

	/* check the label array */
	type_pass2_expr (n->child[2]);
	st2 = check_expr (n->child[2], Array,
			  ErrTypePass2AccumLabelArray, 0);

	if (st0==Err || st1==Err || st2==Err)
	  n->types->stat = Err;

	/* check for matching ranks of source and label arrays */
	if (st0==Okay && st2==Okay)
	  {
	    if (n->child[0]->child[0]->types->dims != n->child[2]->types->dims)
	      {
		handle_error (ErrTypePass2AccumSourceLabelMismatch,
			      Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		n->types->stat = Err;
	      }
	  }

	if (n->types->stat == Okay)
	  /* HERE WE SHOULD SEE WHETHER ANY DIMENSIONS OF
	   * THE NEW ARRAY CAN BE STATICALLY KNOWN...
	   */
	  set_array (n, 1 + n->child[0]->types->dims);
      }
      break;
    case ACCUM :
      {
	Tstat st0=Okay, st1=Okay, st2=Okay;

	/* check the array reduction */
	type_pass2_expr (n->child[0]);
	st0 = n->child[0]->types->stat;

	/* check the accum range value */
	type_pass2_expr (n->child[1]);
	st1 = check_expr (n->child[1], Scalar,
			  ErrTypePass2LoopAccumValScalar, 0);

	/* check the label value */
	type_pass2_expr (n->child[2]);
	st2 = check_expr (n->child[2], Scalar,
			  ErrTypePass2LoopAccumLabelScalar, 0);

	if (st0==Err || st1==Err || st2==Err)
	  n->types->stat = Err;

	if (n->types->stat == Okay)
	  /* HERE WE SHOULD SEE WHETHER ANY DIMENSIONS OF
	   * THE NEW ARRAY CAN BE STATICALLY KNOWN...
	   */
	  set_array (n, 1 + n->child[0]->types->dims);
      }
      break;
    case CAST :
      {

	type_pass2_expr (n->child[1]);

	if (n->child[1]->types->stat == Okay)
	  {
	    if (n->child[1]->types->kind == Array)
	      {
		if (n->child[0]->child[1] == NULL)
		  {
		    handle_error (ErrTypePass2CastArrayToScalar,
				  Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		    n->types->stat = Err;
		  }
		else if (n->child[0]->child[2]->Iconst != n->child[1]->types->dims)
		  {
		    handle_error (ErrTypePass2CastArrayToArrayRankMismatch,
				  Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		    n->types->stat = Err;
		  }
		else
		  {
		    n->types->kind = Array;
		    n->types->dims = n->child[0]->child[2]->Iconst;
		  }
	      }
	    else
	      {
		if (n->child[0]->child[1] != NULL)
		  {
		    handle_error (ErrTypePass2CastScalarToArray,
				  Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		    n->types->stat = Err;
		  }
		else
		  {
		    n->types->kind = Scalar;
		    n->types->dims = 0;
		  }
	      }
	  }
	else
	  n->types->stat = Err;
      }
      break;
    case REDUCE_SUM :
    case REDUCE_PRODUCT :
    case REDUCE_MIN :
    case REDUCE_MAX :
    case REDUCE_AND :
    case REDUCE_OR :
    case REDUCE_MEDIAN :
    case REDUCE_ST_DEV :
    case REDUCE_MEAN :
      /* check reduction value */
      type_pass2_expr (n->child[0]);
      st = check_expr (n->child[0], Scalar,
		       ErrTypePass2LoopReductionScalar, 0);
      if (st == Err)
	n->types->stat = Err;

      /* check mask value */
      if (n->child[1] != NULL)
	{
	  type_pass2_expr (n->child[1]);
	  st = check_expr (n->child[1], Scalar,
			   ErrTypePass2LoopReductionMaskScalar, 0);
	  if (st == Err)
	    n->types->stat = Err;
	}

      if (n->types->stat == Okay)
	set_scalar (n);
      break;
    case REDUCE_MODE :
      {

	/* check reduction value */
	type_pass2_expr (n->child[0]);
	st = check_expr (n->child[0], Scalar,
			 ErrTypePass2LoopReductionScalar, 0);
	if (st == Err)
	  n->types->stat = Err;

	/* check mask value */
	if (n->child[1] != NULL)
	  {
	    type_pass2_expr (n->child[1]);
	    st = check_expr (n->child[1], Scalar,
			     ErrTypePass2LoopReductionMaskScalar, 0);
	    if (st == Err)
	      n->types->stat = Err;
	  }

	if (n->types->stat == Okay)
	  set_array (n, 1);
      }
      break;
    case REDUCE_VAL_AT_FIRST_MIN :
    case REDUCE_VAL_AT_FIRST_MAX :
    case REDUCE_VAL_AT_LAST_MIN :
    case REDUCE_VAL_AT_LAST_MAX :
      {
	NODEPNT m;

	/* check reduction value */
	type_pass2_expr (n->child[0]);
	st = check_expr (n->child[0], Scalar,
			 ErrTypePass2LoopReductionScalar, 0);
	if (st == Err)
	  n->types->stat = Err;

	for (m=n->child[2]; m!=NULL; m=m->link)
	  {
	    type_pass2_expr (m);
	    st = check_expr (m, Scalar, ErrTypePass2ValsAtScalar, 0);
	    if (st == Err)
	      n->types->stat = Err;
	  }

	/* check mask value */
	if (n->child[1] != NULL)
	  {
	    type_pass2_expr (n->child[1]);
	    st = check_expr (n->child[1], Scalar,
			     ErrTypePass2LoopReductionMaskScalar, 0);
	    if (st == Err)
	      n->types->stat = Err;
	  }

	if (n->types->stat == Okay)
	  set_array (n, 1);
      }
      break;
    case REDUCE_VAL_AT_MINS :
    case REDUCE_VAL_AT_MAXS :
      {
	NODEPNT m;

	/* check reduction value */
	type_pass2_expr (n->child[0]);
	st = check_expr (n->child[0], Scalar,
			 ErrTypePass2LoopReductionScalar, 0);
	if (st == Err)
	  n->types->stat = Err;

	for (m=n->child[2]; m!=NULL; m=m->link)
	  {
	    type_pass2_expr (m);
	    st = check_expr (m, Scalar, ErrTypePass2ValsAtScalar, 0);
	    if (st == Err)
	      n->types->stat = Err;
	  }

	/* check mask value */
	if (n->child[1] != NULL)
	  {
	    type_pass2_expr (n->child[1]);
	    st = check_expr (n->child[1], Scalar,
			     ErrTypePass2LoopReductionMaskScalar, 0);
	    if (st == Err)
	      n->types->stat = Err;
	  }

	if (n->types->stat == Okay)
	  /* HERE ONE OF THE DIMENSIONS IS KNOWN STATICALLY...  */
	  set_array (n, 2);
      }
      break;
    case REDUCE_HIST :
      {

	/* check reduction value */
	type_pass2_expr (n->child[0]);
	st = check_expr (n->child[0], Scalar,
			 ErrTypePass2LoopReductionScalar, 0);
	if (st == Err)
	  n->types->stat = Err;

	/* check range value */
	type_pass2_expr (n->child[2]);
	st = check_expr (n->child[2], Scalar,
			 ErrTypePass2LoopHistRangeScalar, 0);
	if (st == Err)
	  n->types->stat = Err;

	/* check mask value */
	if (n->child[1] != NULL)
	  {
	    type_pass2_expr (n->child[1]);
	    st = check_expr (n->child[1], Scalar,
			     ErrTypePass2LoopReductionMaskScalar, 0);
	    if (st == Err)
	      n->types->stat = Err;
	  }

	if (n->types->stat == Okay)
	  /* HERE WE SHOULD SEE WHETHER ANY DIMENSIONS OF
	   * THE NEW ARRAY CAN BE STATICALLY KNOWN...
	   */
	  set_array (n, 1);
      }
      break;
    case ARR_REDUCE_SUM :
    case ARR_REDUCE_PRODUCT :
    case ARR_REDUCE_MIN :
    case ARR_REDUCE_MAX :
    case ARR_REDUCE_AND :
    case ARR_REDUCE_OR :
    case ARR_REDUCE_MEDIAN :
    case ARR_REDUCE_ST_DEV :
    case ARR_REDUCE_MEAN :
      /* check reduction value */
      type_pass2_expr (n->child[0]);
      st = check_expr (n->child[0], Array,
		       ErrTypePass2ArrayReductionArray, 0);
      if (st == Err)
	n->types->stat = Err;

      /* check mask value */
      if (n->child[1] != NULL)
	{
	  type_pass2_expr (n->child[1]);
	  st = check_expr (n->child[1], Array,
			   ErrTypePass2ArrayReductionMaskArray, 0);
	  if (st == Err)
	    n->types->stat = Err;
	}

      /* make sure source and mask arrays have same rank */
      if ((n->types->stat == Okay) && (n->child[1] != NULL))
	{
	  if (n->child[0]->types->dims != n->child[1]->types->dims)
	    {
	      handle_error (ErrTypePass2ReductionSourceMaskMismatch,
			    Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	      n->types->stat = Err;
	    }
	}

      if (n->types->stat == Okay)
	set_scalar (n);
      break;
    case ARR_REDUCE_MODE :
      {

	/* check reduction value */
	type_pass2_expr (n->child[0]);
	st = check_expr (n->child[0], Array,
			 ErrTypePass2ArrayReductionArray, 0);
	if (st == Err)
	  n->types->stat = Err;

	/* check mask value */
	if (n->child[1] != NULL)
	  {
	    type_pass2_expr (n->child[1]);
	    st = check_expr (n->child[1], Array,
			     ErrTypePass2ArrayReductionMaskArray, 0);
	    if (st == Err)
	      n->types->stat = Err;
	  }

	/* make sure source and mask arrays have same rank */
	if ((n->types->stat == Okay) && (n->child[1] != NULL))
	  {
	    if (n->child[0]->types->dims != n->child[1]->types->dims)
	      {
		handle_error (ErrTypePass2ReductionSourceMaskMismatch,
			      Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		n->types->stat = Err;
	      }
	  }

	if (n->types->stat == Okay)
	  set_array (n, 1);
      }
      break;
    case ARR_REDUCE_MIN_INDICES :
    case ARR_REDUCE_MAX_INDICES :
      {

	/* check reduction value */
	type_pass2_expr (n->child[0]);
	st = check_expr (n->child[0], Array,
			 ErrTypePass2ArrayReductionArray, 0);
	if (st == Err)
	  n->types->stat = Err;

	/* check mask value */
	if (n->child[1] != NULL)
	  {
	    type_pass2_expr (n->child[1]);
	    st = check_expr (n->child[1], Array,
			     ErrTypePass2ArrayReductionMaskArray, 0);
	    if (st == Err)
	      n->types->stat = Err;
	  }

	/* make sure source and mask arrays have same rank */
	if ((n->types->stat == Okay) && (n->child[1] != NULL))
	  {
	    if (n->child[0]->types->dims != n->child[1]->types->dims)
	      {
		handle_error (ErrTypePass2ReductionSourceMaskMismatch,
			      Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		n->types->stat = Err;
	      }
	  }

	if (n->types->stat == Okay)
	  /* HERE ONE OF THE DIMENSIONS IS KNOWN STATICALLY...  */
	  set_array (n, 2);
      }
      break;
    case ARR_REDUCE_HIST :
      {

	/* check reduction value */
	type_pass2_expr (n->child[0]);
	st = check_expr (n->child[0], Array,
			 ErrTypePass2ArrayReductionArray, 0);
	if (st == Err)
	  n->types->stat = Err;

	/* check range value */
	type_pass2_expr (n->child[2]);
	st = check_expr (n->child[2], Scalar,
			 ErrTypePass2ArrayHistRangeScalar, 0);
	if (st == Err)
	  n->types->stat = Err;

	/* check mask value */
	if (n->child[1] != NULL)
	  {
	    type_pass2_expr (n->child[1]);
	    st = check_expr (n->child[1], Array,
			     ErrTypePass2ArrayReductionMaskArray, 0);
	    if (st == Err)
	      n->types->stat = Err;
	  }

	/* make sure source and mask arrays have same rank */
	if ((n->types->stat == Okay) && (n->child[1] != NULL))
	  {
	    if (n->child[0]->types->dims != n->child[1]->types->dims)
	      {
		handle_error (ErrTypePass2ReductionSourceMaskMismatch,
			      Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		n->types->stat = Err;
	      }
	  }

	if (n->types->stat == Okay)
	  /* HERE WE SHOULD SEE WHETHER ANY DIMENSIONS OF
	   * THE NEW ARRAY CAN BE STATICALLY KNOWN...
	   */
	  set_array (n, 1);
      }
      break;
    case ARR_CONCAT :
      {
	Tstat st0, st1;

	type_pass2_expr (n->child[0]);
	st0 = check_expr (n->child[0], Array,
			  ErrTypePass2ConcatNeedsArray, 0);
	type_pass2_expr (n->child[1]);
	st1 = check_expr (n->child[1], Array,
			  ErrTypePass2ConcatNeedsArray, 0);
	if ((st0 == Err) || (st1 == Err))
	  n->types->stat = Err;

	/* make sure the two arrays have same rank */
	if (n->types->stat == Okay)
	  {
	    if (n->child[0]->types->dims != n->child[1]->types->dims)
	      {
		handle_error (ErrTypePass2ConcatRankMismatch,
			      Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		n->types->stat = Err;
	      }
	  }

	if (n->types->stat == Okay)
	  /* HERE WE SHOULD SEE WHETHER ANY DIMENSIONS OF
	   * THE NEW ARRAY CAN BE STATICALLY KNOWN...
	   */
	  set_array (n, n->child[0]->types->dims);
      }
      break;
    case ARRAYREF :
      {
	NODEPNT m;
	Tstat st0, st1, st2;
	int dim_cnt, colon_cnt, srce_rank;

	/* process the index expresions */
	for (dim_cnt=0, colon_cnt=0, m=n->child[1]; m!=NULL; m=m->link)
	  {
	    dim_cnt++;
	    st0 = st1 = st2 = Okay;

	    if (m->nodetype == TRIPLE)
	      {
		colon_cnt++;

		if (m->child[0] != NULL)
		  {
		    type_pass2_expr (m->child[0]);
		    st0 = check_expr (m->child[0], Scalar,
				      ErrTypePass2TripleScalar, 0);
		  }
		if (m->child[1] != NULL)
		  {
		    type_pass2_expr (m->child[1]);
		    st1 = check_expr (m->child[1], Scalar,
				      ErrTypePass2TripleScalar, 0);
		  }
		if (m->child[2] != NULL)
		  {
		    type_pass2_expr (m->child[2]);
		    st2 = check_expr (m->child[2], Scalar,
				      ErrTypePass2TripleScalar, 0);
		  }

		if (st0==Err || st1==Err || st2==Err)
		  n->types->stat = Err;
	      }
	    else
	      {
		type_pass2_expr (m);
		st = check_expr (m, Scalar,
				 ErrTypePass2ArrayIndexScalar, 0);

		if (st == Err)
		  n->types->stat = Err;
	      }
	  }

	/* process the array source */
	type_pass2_expr (n->child[0]);
	st = check_expr (n->child[0], Array,
			 ErrTypePass2ArrayRefSource, 0);
	if (st == Err)
	  n->types->stat = Err;

	/* check the rank consistency */
	if (n->types->stat==Okay)
	  {
	    srce_rank = deftable[n->child[0]->Defval].tinfo.dims;
	    if (srce_rank != dim_cnt)
	      {
		handle_error (ErrTypePass2SubscriptRankMismatch,
			      Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		n->types->stat = Err;
	      }
	  }

	if (n->types->stat==Okay)
	  {
	    if (colon_cnt == 0)
	      set_scalar (n);
	    else
	      /* HERE WE SHOULD SEE WHETHER ANY DIMENSIONS OF
	       * THE NEW ARRAY CAN BE STATICALLY KNOWN...
	       */
	      set_array (n, colon_cnt);
	  }
      }
      break;
    case EXTENTS :
      {
	TypeInfo *t;
	int i;

	type_pass2_expr (n->child[0]);
	st = check_expr (n->child[0], Array,
			 ErrTypePass2ExtentsArray, 0);
	if (st == Err)
	  n->types->stat = Err;

	for (t=n->types; t!=NULL; t=t->link)
	  {
	    t->kind = Scalar;
	    t->dims = 0;
	    for (i=0; i<MAXRANK; i++)
	      t->dim_sizes[i] = -1;
	  }

	/* type_pass1 already checked for rank consistency */
      }
      break;
    case CONDITIONAL :
      {
	TypeInfo *tt, *tf, *tr;
	Tstat st;
	int cnt;

	/* check predicate expression */
	type_pass2_expr (n->child[0]);
	st = check_expr (n->child[0], Scalar,
			 ErrTypePass2CondPredScalar, 0);
	if (st == Err)
	  n->types->stat = Err;

	type_pass2_bodyret (n->child[1]);
	type_pass2_bodyret (n->child[2]);

	/* check consistency between true and false expressions */
	for (cnt=0, tt=n->child[1]->types, tf=n->child[2]->types, tr=n->types;
	     tt!=NULL && tf!=NULL && tr!=NULL;
	     cnt++, tt=tt->link, tf=tf->link, tr=tr->link)
	  {
	    if (tt->stat==Err || tf->stat==Err)
	      tr->stat = Err;
	    else
	      {
		st = compare_typenodes_pass2 (tt, tf);
		if (st == Err)
		  {
		    handle_error (ErrTypePass2CondTrueFalseInconsistent,
				  Fatal, n->Lineno, n->Srcfile, n->Srcfunc, cnt);
		    tr->stat = Err;
		  }
		else
		  copy_typeinfo_pass2 (tr, tt);
	      }
	  }
      }
      break;
    case WHILE :
      {
	TypeInfo *tt, *tr;

	/* check predicate expression */
	type_pass2_expr (n->child[0]);
	st = check_expr (n->child[0], Scalar,
			 ErrTypePass2WhilePredScalar, 0);
	if (st == Err)
	  n->types->stat = Err;

	type_pass2_bodyret (n->child[1]);

	set_rank_of_constructors (n->child[1], 1);

	for (tt=n->child[1]->types, tr=n->types;
	     tt!=NULL && tr!=NULL; tt=tt->link, tr=tr->link)
	  copy_typeinfo_pass2 (tr, tt);

	if (any_error (n->types))
	  n->types->stat = Err;
      }
      break;
    case FOR :
      {
	TypeInfo *tt, *tr;
	int gen_rank;

	/* check loop generators */
	st = type_pass2_generators (n->child[0]);
	if (st == Err)
	  {
	    n->types->stat = Err;
	    push_rank (-1);
	  }
	else
	  {
	    gen_rank = n->child[0]->types->dims;
	    push_rank (gen_rank);
	  }

	/* check loop body and returns */
	type_pass2_bodyret (n->child[1]);

	pop_rank ();

	/* fill in rank for constructor return expressions */
	if (n->types->stat != Err)
	  set_rank_of_constructors (n->child[1], gen_rank);

	for (tt=n->child[1]->types, tr=n->types;
	     tt!=NULL && tr!=NULL; tt=tt->link, tr=tr->link)
	  {
	    copy_typeinfo_pass2 (tr, tt);
	  }

	if (any_error (n->child[1]->types) || n->child[0]->types->stat == Err)
	  n->types->stat = Err;
      }
      break;
    case SWITCH :
      {
	NODEPNT m, c;
	TypeInfo *tt, *tr;
	Tstat st;
	int cnt;

	type_pass2_expr (n->child[0]);
	st = check_expr (n->child[0], Scalar,
			 ErrTypePass2SwitchExprScalar, 0);
	if (st == Err)
	  n->types->stat = Err;

	if (n->child[1] != NULL)
	  {
	    /* process the constant values in each CASE;
	     * this is probably not necessary, and errors
	     * here are not possible...
	     */
	    for (m=n->child[1]; m!=NULL; m=m->link)
	      for (c=m->child[0]; c!=NULL; c=c->link)
		type_pass2_expr (c);

	    st = check_for_case_val_overlap (n->child[1]);
	    if (st == Err)
	      n->types->stat = Err;

	    /* set the switch's types to those of the first case */
	    type_pass2_bodyret (n->child[1]->child[1]);
	    for (tt=n->child[1]->child[1]->types, tr=n->types;
		 tt!=NULL && tr!=NULL; tt=tt->link, tr=tr->link)
	      copy_typeinfo_pass2 (tr, tt);

	    /* now do the rest of the cases and compare */
	    for (m=n->child[1]->link; m!=NULL; m=m->link)
	      {
		type_pass2_bodyret (m->child[1]);
		for (cnt=0, tt=m->child[1]->types, tr=n->types;
		     tt!=NULL && tr!=NULL; cnt++, tt=tt->link, tr=tr->link)
		  if (tr->stat == Okay)
		    {
		      st = compare_typenodes_pass2 (tt, tr);
		      if (st == Err)
			{
			  handle_error (ErrTypePass2SwitchValInconsistent,
					Fatal, n->Lineno, n->Srcfile, n->Srcfunc, cnt);
			  tr->stat = Err;
			}
		    }
	      }

	    /* now do the default */
	    if (n->child[2] != NULL)
	      {
		type_pass2_bodyret (n->child[2]);
		for (cnt=0, tt=n->child[2]->types, tr=n->types;
		     tt!=NULL && tr!=NULL; cnt++, tt=tt->link, tr=tr->link)
		  if (tr->stat == Okay)
		    {
		      st = compare_typenodes_pass2 (tt, tr);
		      if (st == Err)
			{
			  handle_error (ErrTypePass2SwitchValInconsistent,
					Fatal, n->Lineno, n->Srcfile, n->Srcfunc, cnt);
			  tr->stat = Err;
			}
		    }
	      }
	  }
	else
	  {
	    /* default is by itself */
	    type_pass2_bodyret (n->child[2]);
	    for (tt=n->child[2]->types, tr=n->types;
		 tt!=NULL && tr!=NULL; tt=tt->link, tr=tr->link)
	      copy_typeinfo_pass2 (tr, tt);
	  }
      }
      break;
    case FCALL :
      {
	NODEPNT m;
	Tstat st;
	int fdv = n->child[0]->Defval;
	TypeInfo *ptypes = deftable[fdv].funcinfo->paramtypes;
	TypeInfo *rtypes = deftable[fdv].funcinfo->rettypes;
	TypeInfo *tt, *tr;
	int cnt;

	/* process the parameters */
	for (cnt=0, m=n->child[1], tt=ptypes;
	     m!=NULL && tt!=NULL; cnt++, m=m->link, tt=tt->link)
	  {
	    type_pass2_expr (m);
	    if (m->types->stat == Okay)
	      {
		st = compare_typenodes_pass2 (m->types, tt);
		if (st == Err)
		  {
		    handle_error (ErrTypePass2CallArgIncompatible,
				  Fatal, n->Lineno, n->Srcfile, n->Srcfunc, cnt);
		    m->types->stat = Err;
		  }
	      }
	  }

	/* transfer the return value types */
	for (tt=rtypes, tr=n->types; tt!=NULL && tr!=NULL; tt=tt->link, tr=tr->link)
	  copy_typeinfo_pass2 (tr, tt);
      }
      break;
    case INTRINCALL :
      {
	NODEPNT m;
	TypeInfo *tr;
	int cnt;

	/* process the parameters */
	for (cnt=0, m=n->child[1]; m!=NULL; cnt++, m=m->link)
	  {
	    type_pass2_expr (m);
	    if (m->types->stat == Okay)
	      {
		if (m->types->kind != Scalar)
		  {
		    handle_error (ErrTypePass2IntrinArgScalar,
				  Fatal, n->Lineno, n->Srcfile, n->Srcfunc, cnt);
		    m->types->stat = Err;
		  }
	      }
	  }

	/* transfer the return value types */
	for (tr=n->types; tr!=NULL; tr=tr->link)
	  {
	    tr->stat = Okay;
	    tr->kind = Scalar;
	    tr->dims = 0;
	  }
      }
      break;
    case BODYRET :
      type_pass2_bodyret (n);
      break;
    default :
      break;
    }
}

Tstat type_pass2_generators (NODEPNT n)
{
  NODEPNT m;
  Tstat st = Okay;
  int rank;

  switch (n->nodetype)
    {
    case SCALAR_GEN :
      type_pass2_scalar_gen (n);
      if (n->types->stat == Err)
	st = Err;
      break;
    case ELE_GEN :
      type_pass2_ele_gen (n);
      if (n->types->stat == Err)
	st = Err;
      break;
    case SLICE_GEN :
      type_pass2_slice_gen (n);
      if (n->types->stat == Err)
	st = Err;
      break;
    case WINDOW :
      type_pass2_window_gen (n);
      if (n->types->stat == Err)
	st = Err;
      break;
    case DOT_PROD :
      for (m=n->child[0]; m!=NULL; m=m->link)
	if (type_pass2_generators (m) == Err)
	  st = Err;

      for (rank=0, m=n->child[0]; m!=NULL; m=m->link)
	if (m->types->stat == Okay)
	  {
	    rank = m->types->dims;
	    break;
	  }

      if (rank != 0)
	{
	  for (m=n->child[0]; m!=NULL; m=m->link)
	    if ((m->types->stat==Okay) && (m->types->dims!=rank))
	      {
		handle_error (ErrTypePass2DotProdRanks,
			      Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		n->types->stat = Err;
		break;
	      }
	}

      if (n->types->stat == Okay)
	set_array (n, rank);
      break;
    case CROSS_PROD :
      for (m=n->child[0]; m!=NULL; m=m->link)
	if (type_pass2_generators (m) == Err)
	  st = Err;

      for (rank=0, m=n->child[0]; m!=NULL; m=m->link)
	if (m->types->stat == Okay)
	  rank += m->types->dims;
	else
	  {
	    n->types->stat = Err;
	    break;
	  }

      if (n->types->stat == Okay)
	set_array (n, rank);

      break;
    default :
      break;
    }

  return st;
}

Tstat type_pass2_window_gen (NODEPNT n)
{
  NODEPNT m;
  Tstat st;
  int cnt;

  /* process the source array expression */
  type_pass2_expr (n->child[1]);
  st = check_expr (n->child[1], Array,
		   ErrTypePass2WindowSourceArray, 0);
  if (st == Err)
    n->types->stat = Err;
  else
    {
      /* check target variable */
      if (n->child[0]->child[2]->Iconst != n->child[1]->types->dims)
	{
	  handle_error (ErrTypePass2WindowSourceTarget,
	    		Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	  n->types->stat = Err;
	}
      else
	{
	  /* check step expressions if they exist */
	  if (n->child[2] != NULL)
	    {
	      cnt = list_length (n->child[2]);
	      if (cnt != n->child[1]->types->dims)
		{
		  handle_error (ErrTypePass2WindowSourceStep,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		  n->types->stat = Err;
		}

	      /* check the step expressions */
	      if (n->types->stat == Okay)
		for (m=n->child[2]; m!=NULL; m=m->link)
		  {
		    type_pass2_expr (m);
		    st = check_expr (m, Scalar,
				     ErrTypePass2WindowStepScalar, 0);
		    if (st == Err)
		      n->types->stat = Err;
		  }
	    }

	  /* check the rank of the at targets if they exist */
	  if (n->child[3] != NULL)
	    {
	      cnt = list_length (n->child[3]);
	      if (cnt != n->child[1]->types->dims)
		{
		  handle_error (ErrTypePass2WindowSourceAt,
		    		Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		  n->types->stat = Err;
		}
	    }
	}
    }

  /* process the window size expressions */
  for (m=n->child[0]->child[1]; m!=NULL; m=m->link)
    {
      type_pass2_expr (m);
      st = check_expr (m, Scalar, ErrTypePass2WindowSizeScalar, 0);
      if (st == Err)
	n->types->stat = Err;
    }

  if (n->types->stat == Okay)
    set_array (n, n->child[1]->types->dims);

  return Okay;
}

Tstat type_pass2_ele_gen (NODEPNT n)
{
  NODEPNT m, p, a;
  Tstat st;
  int rank;		/* rank of the generator */
  int cnt;

  /* process the source array expression */
  type_pass2_expr (n->child[2]);
  st = check_expr (n->child[2], Array,
		   ErrTypePass2ElegenSourceArray, 0);
  if (st == Err)
    n->types->stat = Err;
  else if (n->child[1] != NULL)
    {
      handle_error (ErrTypePass2ElegenExtract,
		    Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
      n->types->stat = Err;
    }
  else
    {
      /* determine the rank of the generator */
      if (n->types->stat == Okay)
	{
	  rank = n->child[2]->types->dims;
	  set_array (n, rank);
	}

      /* check the 'step' spec */
      if (n->child[3] != NULL)
	{
	  cnt = list_length (n->child[3]);

	  /* positions in step spec must match source array rank */
	  if (cnt != n->child[2]->types->dims)
	    handle_error (ErrTypePass2ElegenRankStepMismatch,
			  Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	  else
	    {
	      /* no empties allowed in step spec */
	      for (m=n->child[3]; m!=NULL; m=m->link)
		if (m->nodetype == EMPTY)
		  handle_error (ErrTypePass2ElegenStepExprNeeded,
			        Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	    }
	}

      /* check the 'at' spec */
      if (n->child[4] != NULL)
	{
	  cnt = list_length (n->child[4]);
	  if (cnt != n->child[2]->types->dims)
	    handle_error (ErrTypePass2ElegenRankAtMismatch,
			  Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	}
    }

  return n->types->stat;
}

Tstat type_pass2_slice_gen (NODEPNT n)
{
  NODEPNT m, p, a;
  Tstat st;
  int pat_rank;	/* position count in extrcat pattern;
		 * must equal source array rank */
  int colon_cnt;	/* number of colons in extract pattern */
  int rank;		/* rank of the generator */
  int cnt;

  /* process the source array expression */
  type_pass2_expr (n->child[2]);
  st = check_expr (n->child[2], Array,
		   ErrTypePass2SlicegenSourceArray, 0);
  if (st == Err)
    n->types->stat = Err;
  else if (n->child[1] == NULL)
    {
      /* check that extract pattern present */
      handle_error (ErrTypePass2SlicegenNoExtract,
		    Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
      n->types->stat = Err;
    }
  else
    {
      /* check rank consistency with extract pattern */
      /* set 'pat_rank' and 'colon_cnt' variables */
      for (pat_rank=0, colon_cnt=0, m=n->child[1]; m!=NULL; 
	   pat_rank++, m=m->link)
	if (m->nodetype == COLON)
	  colon_cnt++;

      /* we must iterate in at least one dimension; otherwise error */
      if (pat_rank == colon_cnt)
	{
	  handle_error (ErrTypePass2SlicegenNoTilde,
			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	  n->types->stat = Err;
	}
      /* 'pat_rank' must match source array's rank */
      else if (pat_rank != n->child[2]->types->dims)
	{
	  handle_error (ErrTypePass2SlicegenRankMismatch,
			Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	  n->types->stat = Err;
	}

      /* determine the rank of the generator */
      if (n->types->stat == Okay)
	{
	  for (rank=0, m=n->child[1]; m!=NULL; m=m->link)
	    if (m->nodetype == TILDE)
	      rank++;
	  set_array (n, rank);
	}

      /* check the 'step' spec */
      if (n->child[3] != NULL)
	{
	  cnt = list_length (n->child[3]);

	  /* positions in step spec must match source array rank */
	  if (cnt != n->child[2]->types->dims)
	    handle_error (ErrTypePass2SlicegenRankStepMismatch,
			  Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	  else if (cnt==pat_rank)
	    {
	      /* EMPTY must correspond to COLON, and expr must correspond to TILDE */
	      for (p=n->child[1], m=n->child[3];
		   p!=NULL && m!=NULL; p=p->link, m=m->link)
		if (m->nodetype == EMPTY)
		  {
		    if (p->nodetype == TILDE)
		      handle_error (ErrTypePass2SlicegenStepExprNotAllowed,
			    	    Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		  }
		else if (p->nodetype == COLON)
		  handle_error (ErrTypePass2SlicegenStepExprNeeded,
				Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		else
		  {
		    type_pass2_expr (m);
		    st = check_expr (m, Scalar, ErrTypePass2SlicegenStepScalar, 0);
		    if (st == Err)
		      n->types->stat = Err;
		  }
	    }
	}

      /* check the 'at' spec */
      if (n->child[4] != NULL)
	{
	  cnt = list_length (n->child[4]);
	  if (cnt != n->child[2]->types->dims)
	    handle_error (ErrTypePass2SlicegenRankAtMismatch,
			  Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	  else if (cnt==pat_rank)
	    for (p=n->child[1], a=n->child[4];
		 p!=NULL && a!=NULL; p=p->link, a=a->link)
	      if (p->nodetype==COLON && a->nodetype!=EMPTY)
		handle_error (ErrTypePass2SlicegenAtEmpty,
			      Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	}
    }
  
  return n->types->stat;
}

Tstat type_pass2_scalar_gen (NODEPNT n)
{
  Tstat st;
  NODEPNT m;

  /* don't need to make sure targets are scalars; syntax enforces it */
        
  /* make sure source expressions are scalars */
  for (m=n->child[1]; m!=NULL; m=m->link)
    if (m->nodetype == TILDE)
      {
	type_pass2_expr (m->child[0]);
	st = check_expr (m->child[0], Scalar,
			 ErrTypePass2ScalarGenSourceScalar, 0);
	if (st == Err)
	  n->types->stat = Err;

	type_pass2_expr (m->child[1]);
	st = check_expr (m->child[1], Scalar,
			 ErrTypePass2ScalarGenSourceScalar, 0);
	if (st == Err)
	  n->types->stat = Err;
      }
    else
      {
	type_pass2_expr (m);
	st = check_expr (m, Scalar,
			 ErrTypePass2ScalarGenSourceScalar, 0);
	if (st == Err)
	  n->types->stat = Err;
      }

  /* check optional step expressions */
  if (n->child[2] != NULL)
    for (m=n->child[2]; m!=NULL; m=m->link)
      {
	type_pass2_expr (m);
	st = check_expr (m, Scalar,
			 ErrTypePass2ScalarGenSourceScalar, 0);
	if (st == Err)
	  n->types->stat = Err;
      }

  if (n->types->stat == Okay)
    {
      n->types->kind = Array;
      n->types->dims = list_length (n->child[0]);
    }

  return n->types->stat;
}

/* 'n' points to a loop's BODYRET node */
/* HERE NEED TO PASS IN DIM_SIZES FOR THE GENERATOR, AND
 * INSTALL DIM SIZES WHERE POSSIBLE IN THE RESULT ARRAYS
 */
void set_rank_of_constructors (NODEPNT n, int gen_rank)
{
  NODEPNT m;
  TypeInfo *tt;
  int i, cnt, component_rank;

  /* for any array constructors, we now determine the rank */
  for (tt=n->types, m=n->child[1]; m!=NULL; m=m->link)
    {
      if (! any_error (m->types))
	switch (m->nodetype)
	  {
	  case CONSTRUCT_ARRAY :
	    component_rank = m->child[0]->types->dims;
	    m->types->dims = tt->dims = gen_rank + component_rank;
	    break;
	  case CONSTRUCT_VECTOR :
	    component_rank = m->child[0]->types->dims;
	    m->types->dims = tt->dims = gen_rank + component_rank;
	    if (tt->dims != 1)
	      {
		handle_error (ErrTypePass2VectorRank,
			      Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		m->types->stat = Err;
	      }
	    break;
	  case CONSTRUCT_MATRIX :
	    component_rank = m->child[0]->types->dims;
	    m->types->dims = tt->dims = gen_rank + component_rank;
	    if (tt->dims != 2)
	      {
		handle_error (ErrTypePass2MatrixRank,
			      Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		m->types->stat = Err;
	      }
	    break;
	  case CONSTRUCT_CUBE :
	    component_rank = m->child[0]->types->dims;
	    m->types->dims = tt->dims = gen_rank + component_rank;
	    if (tt->dims != 3)
	      {
		handle_error (ErrTypePass2CubeRank,
			      Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		m->types->stat = Err;
	      }
	    break;
	  case CONSTRUCT_CONCAT :
	    component_rank = m->child[0]->types->dims;
	    m->types->dims = tt->dims = component_rank;
	    if (gen_rank > 1)
	      {
		handle_error (ErrTypePass2ConcatGenRank,
			      Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
		m->types->stat = Err;
	      }
	    break;
	  case CONSTRUCT_TILE :
	    component_rank = m->child[0]->types->dims;
	    m->types->dims = tt->dims =
	      (gen_rank>component_rank)?gen_rank:component_rank;
	    break;
	  default :
            break;
	  }

      cnt = tlist_length1 (m->types);
      for (i=0; i<cnt; i++)
	tt=tt->link;
    }
}

Tstat compare_typenodes_pass2 (TypeInfo *t1, TypeInfo *t2)
{

  switch (t1->kind)
    {
    case Scalar :
      if (t2->kind != Scalar)
	return Err;
      break;
    case Array :
      if (t2->kind != Array)
	return Err;
      else
	{
	  if (t1->dims != t2->dims)
	    return Err;

	  /*
	    for (i=0; i<MAXRANK; i++)
	    if (t1->dim_sizes[i]!=-1 &&
	    t2->dim_sizes[i]!=-1 &&
	    t1->dim_sizes[i] != t2->dim_sizes[i])
	    return Err;
	  */
	}
      break;
    default :
      assert (0);
    }

  return Okay;
}

void copy_typeinfo_pass2 (TypeInfo *target, TypeInfo *source)
{
  int i;

  target->stat = source->stat;
  target->kind = source->kind;
  target->dims = source->dims;
  for (i=0; i<MAXRANK; i++)
    target->dim_sizes[i] = source->dim_sizes[i];
}

void set_scalar (NODEPNT n)
{
  assert (n != NULL);

  assert (n->types != NULL);

  n->types->kind = Scalar;
  n->types->dims = 0;
  init_dim_sizes (n);
}

void set_array (NODEPNT n, int rank)
{
  assert (n->types != NULL);

  n->types->kind = Array;
  n->types->dims = rank;
  init_dim_sizes (n);
}

Tstat inconsistent_pass2 (int dv, TypeInfo *right_t)
{
  if (right_t->stat == Err)
    return Okay;

  switch (right_t->kind)
    {
    case Knone :
      assert (0);
    case Array :
      if (deftable[dv].tinfo.kind != Array)
	{
	  printf ("##1\n");
	  return Err;
	}

      if (deftable[dv].tinfo.dims != right_t->dims)
	{
	  printf ("##2, %d vs %d\n", deftable[dv].tinfo.dims, right_t->dims);
	  return Err;
	}

      break;
    case Scalar :
      if (deftable[dv].tinfo.kind != Scalar)
	{
	  printf ("##3\n");
	  return Err;
	}
      break;
    case Func :
      /* can this happen...? */
      {
	printf ("##4\n");
	return Err;
      }
      break;
    default :
      break;
    }

  return Okay;
}

Tstat check_expr (NODEPNT n, Kind kd, Error err, Node nty)
{
  if (n->types->stat==Okay)
    {
      if (n->types->kind != kd)
	{
	  handle_error (err, Fatal, n->Lineno, n->Srcfile, n->Srcfunc, nty);
	  return Err;
	}
      else
	return Okay;
    }
  else
    return Err;
}

void show_typeinfo (TypeInfo *t)
{
  int i;

  if (t->stat == Err)
    printf ("type: Err\n");
  else
    {
      printf ("type: Okay\n");
      printf ("  %s(%d)", type_str (t->type), t->type);
      switch (t->type)
	{
	case Uint :
	case Int :
	case Bits :
	case CxInt :
	  printf ("%d", t->totsize);
	  break;
	case Bool :
	case CxFloat :
	case CxDouble :
	case Tnone :
	case Unknown :
	  break;
	case Fix :
	case Ufix :
	case CxFix :
	  printf ("%d.%d", t->totsize, t->fracsize);
	  break;
	default :
           break;
	}
      if (t->kind == Array)
	{
	  printf ("[");
	  for (i=0; i<t->dims; i++)
	    {
	      if ((t->dim_sizes)[i] < 0)
		printf (":");
	      else
		printf ("%d", (t->dim_sizes)[i]);
	      if (i+1 < t->dims)
		printf (",");
	    }
	  printf ("]\n");
	}
      else
	printf ("\n");
    }
}

void show_typeinfo_list (TypeInfo *t)
{
  TypeInfo *tt;

  for (tt=t; tt!=NULL; tt=tt->link)
    show_typeinfo (tt);
}

/* 'n' points to a list of CASE nodes */
Tstat check_for_case_val_overlap (NODEPNT n)
{
  NODEPNT m, c;
  IntList *vals = NULL;
  Tstat st = Okay;
  int value;

  for (m=n; m!=NULL; m=m->link)
    for (c=m->child[0]; c!=NULL; c=c->link)
      {
	value = c->Iconst;
	if (in_list (value, &vals))
	  {
	    st = Err;
	    handle_error (ErrTypePass2DupCaseVal,
			  Fatal, c->Lineno, c->Srcfile, c->Srcfunc, value);
	  }
	else
	  add_to_list (value, &vals);
      }

  free_intlist (&vals);

  return st;
}

Tstat check_array_dimensions (NODEPNT n, int ndim, int *extents)
{
  NODEPNT m;
  int cnt;

  if (ndim == 1)
    {
      for (m=n, cnt=0; m!=NULL; m=m->link, cnt++)
	{
	  if (m->nodetype == ARR_SLICE)
	    {
	      handle_error (ErrTypePass2ArrConstDimMismatch,
			    Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	      return Err;
	    }
	}

      if (cnt != *extents)
	{
	  handle_error (ErrTypePass2ArrConstDimMismatch,
	    		Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	  return Err;
	}
    }
  else
    {
      for (m=n, cnt=0; m!=NULL; m=m->link, cnt++)
	if (m->nodetype != ARR_SLICE)
	  {
	    handle_error (ErrTypePass2ArrConstDimMismatch,
			  Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	    return Err;
	  }
	else
	  {
	    if (check_array_dimensions (m->child[0], ndim-1,
					extents+1) == Err)
	      return Err;
	  }

      if (cnt != *extents)
	{
	  handle_error (ErrTypePass2ArrConstDimMismatch,
	    		Fatal, n->Lineno, n->Srcfile, n->Srcfunc);
	  return Err;
	}
    }

  return Okay;
}

Tstat check_array_scalars (NODEPNT n)
{
  NODEPNT m;
  Tstat st, stret=Okay;

  if (n == NULL)
    return Okay;

  if (n->nodetype == ARR_SLICE)
    for (m=n; m!=NULL; m=m->link)
      {
	st = check_array_scalars (m->child[0]);
	if (st == Err)
	  stret = Err;
      }
  else
    for (m=n; m!=NULL; m=m->link)
      {
	type_pass2_expr (m);
	st = check_expr (m, Scalar, ErrTypePass2ArrayEleScalar, 0);
	if (st == Err)
	  stret = Err;
      }

  return stret;
}

void init_dim_sizes (NODEPNT n)
{
  int i;

  for (i=0; i<MAXRANK; i++)
    n->types->dim_sizes[i] = -1;
}

void copy_dim_sizes (TypeInfo *target, TypeInfo *source)
{
  int i;

  for (i=0; i<MAXRANK; i++)
    (target->dim_sizes)[i] = (source->dim_sizes)[i];
}

void push_rank (int n)
{
  if (g_rank_idx >= G_STK_SIZE)
    handle_error (ErrTypePass2StackSize, Deadly, 0, NULL, NULL);

  g_rank_stk[g_rank_idx++] = n;
}

void pop_rank ()
{
  g_rank_idx--;
  assert (g_rank_idx >= 0);
}
