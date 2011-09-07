#include "sassy.h"
#include "node.h"
#include "deftab.h"
#include "error.h"

int *symvals = NULL;
int *defvals = NULL;
int stksize = 0;
int stkidx = 0;

void collect_func_names (NODEPNT);
void collect_params_and_locals (NODEPNT);
void collect_func_info (NODEPNT);
void pushpair (int, int);
void scope_rec (NODEPNT);
void handle_decl_def (NODEPNT);
int find_dv (int);
TypeInfo *make_type_list (NODEPNT);
TypeInfo *make_type_node ();
void set_type_and_sizes (NODEPNT, Type*, int*, int*);
void infer_type (NODEPNT, NODEPNT);
void check_consistency (NODEPNT, int, int, char*, char*);
Tstat compare_type_lists (NODEPNT, TypeInfo*, int, int, char*, char*);
void check_for_all_funcs_defined ();
void remove_funcdecls (NODEPNT*);
void check_nextified_loop_returns (NODEPNT);


/* handle scoping and tagging 'defval's on IDENT nodes in three
 * passes:
 *	1 - go down the top spine and get all the function names;
 *	    these names are all in scope through the entire program;
 *	2 - go through the function bodies, pushing and popping the
 *	    params and local decls as appropriate, and tagging all
 *	    IDENT nodes encountered;
 *	3 - again go down the top spine, handling each function; this
 *	    time we collect information about the function's params
 *	    and return-IDENTs and put it into its 'FUNCINFO' struct;
 */
void scoping (NODEPNT *root)
{
  collect_func_names (*root);
  collect_params_and_locals (*root);
  collect_func_info (*root);
  check_nextified_loop_returns (*root);
  if (module == FALSE)
    check_for_all_funcs_defined ();
  remove_funcdecls (root);
}


void collect_func_names (NODEPNT root)
{
  NODEPNT t;
  int dv;

  for (t=root; t!=NULL; t=t->link)
    {
      dv = find_dv (t->child[1]->Symval);

      if (dv == -1)
	{
	  /* not yet in table, so add it */
	  dv = newdef (Okay, Tnone, 0, 0, Func, 0, t->child[1]->Symval);

	  SacMalloc (deftable[dv].funcinfo, FUNCINFO*, sizeof (FUNCINFO))
            deftable[dv].funcinfo->nparams = 0;
	  deftable[dv].funcinfo->paramdvs = NULL;
	  deftable[dv].funcinfo->paramtypes = NULL;
	  deftable[dv].funcinfo->nlocals = 0;
	  deftable[dv].funcinfo->locdvs = NULL;
	  deftable[dv].funcinfo->nreturns = 0;
	  deftable[dv].funcinfo->stat = StatNone;
	  deftable[dv].funcinfo->rettypes = NULL;
	  deftable[dv].funcinfo->pragma = NULL;

	  pushpair (t->child[1]->Symval, dv);
	  t->child[1]->Defval = dv;
	}
      else
	t->child[1]->Defval = dv;
    }
}

void collect_params_and_locals (NODEPNT root)
{
  NODEPNT t, n;
  int stk;

  for (t=root; t!=NULL; t=t->link)
    if (t->nodetype == FUNC)
      {
	stk = stkidx;

	/* push params */
	for (n=t->child[2]; n!=NULL; n=n->link)
	  handle_decl_def (n);

	/* process the function body */
	scope_rec (t->child[3]);

	/* restore stack */
	stkidx = stk;
      }
}


void scope_rec (NODEPNT n)
{
  int stk, dv, i;
  NODEPNT t;

  /*
    printf ("%s %d\n", node_str (n->nodetype), n);
    fflush (stdout);
  */

  switch (n->nodetype)
    {
    case FOR :
      stk = stkidx;
      scope_rec (n->child[0]);
      scope_rec (n->child[1]);
      stkidx = stk;
      break;
    case BODYRET :
      stk = stkidx;
      if (n->child[0] != NULL)
	scope_rec (n->child[0]);
      scope_rec (n->child[1]);
      stkidx = stk;
      break;
    case IDENT :
      dv = find_dv (n->Symval);

      if (dv < 0)
	{
	  handle_error (ErrScopeUndeclVar, Fatal,
			n->Lineno, n->Srcfile, n->Srcfunc, n->Symval);
	  dv = newdef (Err, Tnone, 0, 0, Knone, 0, n->Symval);
	  pushpair (n->Symval, dv);
	}

      n->Defval = dv;

      if (n->link != NULL)
	scope_rec (n->link);

      break;
    case ASSIGN :
      scope_rec (n->child[1]);

      for (t=n->child[0]; t!=NULL; t=t->link)
	{
	  if (t->nodetype != IDENT)
	    continue;

	  if (t->child[0]->nodetype == NEXT)
	    {
	      dv = find_dv (t->Symval);

	      if (dv < 0)
		{
		  handle_error (ErrScopeUndeclVar, Fatal,
				t->Lineno, t->Srcfile, t->Srcfunc, t->Symval);
		  dv = newdef (Err, Tnone, 0, 0, Knone, 0, t->Symval);
		  pushpair (t->Symval, dv);
		}

	      t->Defval = dv;

	      continue;
	    }

	  handle_decl_def (t);
	}

      if (n->link != NULL)
	scope_rec (n->link);
      break;
    case WINDOW :
      /* check source array expression */
      scope_rec (n->child[1]);

      /* check 'step' expressions */
      if (n->child[2] != NULL)
	scope_rec (n->child[2]);

      /* handle the 'at' IDENTs */
      for (t=n->child[3]; t!=NULL; t=t->link)
	if (t->nodetype == IDENT)
	  handle_decl_def (t);

      /* check the window size expressions */
      scope_rec (n->child[0]->child[1]);

      /* handle the window target variable */
      handle_decl_def (n->child[0]);

      /* if there was not a type for the target, see if we
       * can get it from the source; if not, tell user to
       supply the type
      */
      if (n->child[0]->child[0]->nodetype == UNKNOWNTYPE)
	infer_type (n->child[0], n->child[1]);

      if (n->link != NULL)
	scope_rec (n->link);
      break;
    case ELE_GEN :
    case SLICE_GEN :
      /* check source array expression */
      scope_rec (n->child[2]);

      /* check 'step' expressions */
      for (t=n->child[3]; t!=NULL; t=t->link)
	if (t->nodetype != EMPTY)
	  scope_rec (t);

      /* handle the 'at' IDENTS */
      for (t=n->child[4]; t!=NULL; t=t->link)
	if (t->nodetype == IDENT)
	  handle_decl_def (t);
	    
      /* handle the target array variable */
      handle_decl_def (n->child[0]);

      if (n->child[0]->child[0]->nodetype == UNKNOWNTYPE)
	infer_type (n->child[0], n->child[2]);

      if (n->link != NULL)
	scope_rec (n->link);
      break;
    case SCALAR_GEN :
      /* check the generator source expressions */
      scope_rec (n->child[1]);

      /* check the 'step' expressions */
      if (n->child[2] != NULL)
	scope_rec (n->child[2]);

      /* handle the target variables */
      for (t=n->child[0]; t!=NULL; t=t->link)
	if (t->nodetype == IDENT)
	  handle_decl_def (t);

      if (n->link != NULL)
	scope_rec (n->link);
      break;
    default :
      for (i=0; i<NUM_CHILDREN; i++)
	if (n->child[i] != NULL)
	  scope_rec (n->child[i]);

      if (n->link != NULL)
	scope_rec (n->link);
      break;
    }
}


void collect_func_info (NODEPNT root)
{
  NODEPNT t, tt;
  int fdv, nparams, nreturns, i;

  for (t=root; t!=NULL; t=t->link)
    {
      fdv = t->child[1]->Defval;
      nparams = list_length (t->child[2]);
      nreturns = list_length (t->child[0]);

      switch (deftable[fdv].funcinfo->stat)
	{
	case StatNone :
	  deftable[fdv].funcinfo->nparams = nparams;
	  deftable[fdv].funcinfo->nreturns = nreturns;

	  if (t->nodetype == FUNC)
	    {
	      deftable[fdv].funcinfo->stat = StatDef;

	      if (nparams > 0)
		{
		  SacMalloc (deftable[fdv].funcinfo->paramdvs, int*, nparams * sizeof (int))
		    for (i=0, tt=t->child[2]; i<nparams; i++, tt=tt->link)
		      deftable[fdv].funcinfo->paramdvs[i] = tt->Defval;
		}
	    }
	  else
	    {
	      deftable[fdv].funcinfo->stat = StatDecl;
	      deftable[fdv].funcinfo->pragma = t->pragma;
	      t->pragma = NULL;
	    }

	  deftable[fdv].funcinfo->paramtypes =
	    make_type_list (t->child[2]);

	  deftable[fdv].funcinfo->rettypes =
	    make_type_list (t->child[0]);
	  break;
	case StatDecl :
	  if (t->nodetype == FUNC)
	    {
	      deftable[fdv].funcinfo->stat = StatDef;

	      if (nparams > 0)
		{
		  SacMalloc (deftable[fdv].funcinfo->paramdvs, int*, nparams * sizeof (int))
		    for (i=0, tt=t->child[2]; i<nparams; i++, tt=tt->link)
		      deftable[fdv].funcinfo->paramdvs[i] = tt->Defval;
		}
	    }
		
	  check_consistency (t, fdv, t->Lineno, t->Srcfile, t->Srcfunc);

	  break;
	case StatDef :
	  if (t->nodetype == FUNC)
	    handle_error (ErrScopeDupFuncName, Fatal, t->child[1]->Lineno,
			  t->child[1]->Srcfile, t->child[1]->Srcfunc, t->child[1]->Symval);
	  else
	    check_consistency (t, fdv, t->Lineno, t->Srcfile, t->Srcfunc);

	  break;
	}
    }
}


void pushpair (int sval, int dval)
{
  int *new_space;

  if (stksize == 0)
    {
      SacMalloc (symvals, int*, 10 * sizeof (int))
	SacMalloc (defvals, int*, 10 * sizeof (int))
	stksize = 10;
    }

  if (stksize == stkidx)
    {
      SacMalloc (new_space, int*, 2 * stksize * sizeof (int))
        bcopy (symvals, new_space, stksize * sizeof (int));
      SacFree (symvals)
        symvals = new_space;

      SacMalloc (new_space, int*, 2 * stksize * sizeof (int))
        bcopy (defvals, new_space, stksize * sizeof (int));
      SacFree (defvals)
        defvals = new_space;

      stksize *= 2;
    }

  symvals[stkidx] = sval;
  defvals[stkidx++] = dval;
}


int find_dv (int sym)
{
  int i;

  for (i=stkidx-1; i>=0; i--)
    if (symvals[i] == sym)
      return defvals[i];

  return -1;
}


void handle_decl_def (NODEPNT t)
{
  NODEPNT nt;
  Type ty;
  Kind kd;
  int dv, tsz, fsz, dims, i;

  assert (t->child[0] != NULL);

  set_type_and_sizes (t->child[0], &ty, &tsz, &fsz);

  kd = (t->child[1]==NULL) ? Scalar : Array;

  if (kd == Array)
    dims = t->child[2]->Iconst;
  else
    dims = 0;

  dv = newdef (Okay, ty, tsz, fsz, kd, dims, t->Symval);

  for (nt=t->child[1], i=0; nt!=NULL; nt=nt->link, i++)
    if (nt->nodetype == ICONST)
      deftable[dv].tinfo.dim_sizes[i] = nt->Iconst;

  pushpair (t->Symval, dv);
  t->Defval = dv;
}


TypeInfo *make_type_list (NODEPNT n)
{
  TypeInfo *ret=NULL, **p, *tmp;
  Type ty;
  int tsz, fsz;
  NODEPNT r, nt;
  int i;

  p = &ret;

  for (r=n; r!=NULL; r=r->link)
    {
      tmp = make_type_node ();

      set_type_and_sizes (r->child[0], &ty, &tsz, &fsz);
      tmp->type = ty;
      tmp->totsize = tsz;
      tmp->fracsize = fsz;

      if (r->child[1] != NULL)
	{
	  tmp->kind = Array;

	  for (nt=r->child[1], i=0; nt!=NULL; nt=nt->link, i++)
	    if (nt->nodetype == ICONST)
	      tmp->dim_sizes[i] = nt->Iconst;
	    else
	      tmp->dim_sizes[i] = -1;

	  tmp->dims = r->child[2]->Iconst;
	}
      else
	{
	  tmp->kind = Scalar;
	  tmp->dims = 0;
	}

      *p = tmp;
      p = &(tmp->link);
    }

  *p = NULL;
  return ret;
}


/* 'n' is a type node */
void set_type_and_sizes (NODEPNT n, Type *type, int *totsize, int *fracsize)
{
  switch (n->nodetype)
    {
    case INTTYPE :
      *type = Int;
      *totsize = n->Totsize;
      *fracsize = 0;
      break;
    case UINTTYPE :
      *type = Uint;
      *totsize = n->Totsize;
      *fracsize = 0;
      break;
    case FIXTYPE :
      *type = Fix;
      *totsize = n->Totsize;
      *fracsize = n->Fracsize;
      break;
    case UFIXTYPE :
      *type = Ufix;
      *totsize = n->Totsize;
      *fracsize = n->Fracsize;
      break;
    case FLOATTYPE :
      *type = Float;
      *totsize = n->Totsize;
      *fracsize = 0;
      break;
    case DOUBLETYPE :
      *type = Double;
      *totsize = n->Totsize;
      *fracsize = 0;
      break;
    case CXINTTYPE :
      *type = CxInt;
      *totsize = n->Totsize;
      *fracsize = 0;
      break;
    case CXFIXTYPE :
      *type = CxFix;
      *totsize = n->Totsize;
      *fracsize = n->Fracsize;
      break;
    case CXFLOATTYPE :
      *type = CxFloat;
      *totsize = n->Totsize;
      *fracsize = 0;
      break;
    case CXDOUBLETYPE :
      *type = CxDouble;
      *totsize = n->Totsize;
      *fracsize = 0;
      break;
    case BOOLTYPE :
      *type = Bool;
      *totsize = n->Totsize;
      *fracsize = 0;
      break;
    case BITSTYPE :
      *type = Bits;
      *totsize = n->Totsize;
      *fracsize = 0;
      break;
    case UNKNOWNTYPE :
      *type = Unknown;
      *totsize = 0;
      *fracsize = 0;
      break;
    default :
      assert (0);
    }
}


void infer_type (NODEPNT target, NODEPNT source)
{
  Type ty;
  int tdv, tsz, fsz;

  /*
    printf ("'infer_type', target %d, source %d\n", target, source);
    fflush (stdout);
  */

  tdv = target->Defval;

  switch (source->nodetype)
    {
    case IDENT :
      {
	int sdv = source->Defval;
	ty = deftable[sdv].tinfo.type;
	tsz = deftable[sdv].tinfo.totsize;
	fsz = deftable[sdv].tinfo.fracsize;
      }
      break;
    case ARRAYREF :
      {
	int sdv = source->child[0]->Defval;
	ty = deftable[sdv].tinfo.type;
	tsz = deftable[sdv].tinfo.totsize;
	fsz = deftable[sdv].tinfo.fracsize;
      }
      break;
    default :
      /* we'll infer it later... */
      ty = Unknown;
      tsz = 0;
      fsz = 0;
      /*
	handle_error (ErrScopeNeedTargetType, Fatal,
	target->Lineno, target->Srcfile, target->Srcfunc, target->Symval);
	return;
      */
    }

  deftable[tdv].tinfo.type = ty;
  deftable[tdv].tinfo.totsize = tsz;
  deftable[tdv].tinfo.fracsize = fsz;
}


void check_consistency (NODEPNT n, int fdv, int line, char *file, char *func)
{
  if (compare_type_lists (n->child[2],
			  deftable[fdv].funcinfo->paramtypes, fdv, line, file, func) == Okay)
    compare_type_lists (n->child[0],
			deftable[fdv].funcinfo->rettypes, fdv, line, file, func);
}


Tstat compare_type_lists (NODEPNT n, TypeInfo *tt,
			  int fdv, int line, char *file, char *func)
{
  NODEPNT m, r;
  TypeInfo *t;
  Type ty;
  int i, tsz, fsz;

  for (m=n, t=tt; m!=NULL && t!=NULL; m=m->link, t=t->link)
    {
      set_type_and_sizes (m->child[0], &ty, &tsz, &fsz);

      /* check type and totsizes */
      if ((ty != t->type) || (tsz != t->totsize))
	{
	  handle_error (ErrScopeInconsistentDecl, Fatal,
	    		line, file, func, deftable[fdv].symval);
	  return Err;
	}

      /* check fracsize if needed */
      switch (ty)
	{
	case Fix :
	case Ufix :
	case CxFix :
	  if (fsz != t->fracsize)
	    {
	      handle_error (ErrScopeInconsistentDecl, Fatal,
			    line, file, func, deftable[fdv].symval);
	      return Err;
	    }
	default :
            break;
	}

      /* check array/scalar mismatch */
      if ((m->child[1]!=NULL && t->kind==Scalar) ||
	  (m->child[1]==NULL && t->kind!=Scalar))
	{
	  handle_error (ErrScopeInconsistentDecl, Fatal,
	    		line, file, func, deftable[fdv].symval);
	  return Err;
	}

      if (t->kind == Array)
	{
	  /* check ranks */
	  if (m->child[2]->Iconst != t->dims)
	    {
	      handle_error (ErrScopeInconsistentDecl, Fatal,
			    line, file, func, deftable[fdv].symval);
	      return Err;
	    }

	  /* check for constants in sizes */
	  for (r=m->child[1], i=0; r!=NULL; r=r->link, i++)
	    if (r->nodetype == ICONST)
	      {
		if (t->dim_sizes[i] != r->Iconst)
		  {
		    handle_error (ErrScopeInconsistentDecl, Fatal,
				  line, file, func, deftable[fdv].symval);
		    return Err;
		  }
	      }
	    else
	      {
		if (t->dim_sizes[i] != -1)
		  {
		    handle_error (ErrScopeInconsistentDecl, Fatal,
				  line, file, func, deftable[fdv].symval);
		    return Err;
		  }
	      }
	}
    }

  if ((m==NULL && t!=NULL) || (m!=NULL && t==NULL))
    {
      handle_error (ErrScopeInconsistentDecl, Fatal,
		    line, file, func, deftable[fdv].symval);
      return Err;
    }

  return Okay;
}


void check_for_all_funcs_defined ()
{
  int i;

  for (i=0; i<defcnt; i++)
    if ((deftable[i].tinfo.kind==Func) && (deftable[i].funcinfo->stat!=StatDef))
      {
	handle_error (ErrScopeUndefinedFunc,
		      Fatal, 0, NULL, NULL, deftable[i].symval);
      }
}


void remove_funcdecls (NODEPNT *p)
{
  NODEPNT tmp;

  while ((*p) != NULL)
    if ((*p)->nodetype == FUNCDECL)
      {tmp = *p;
      *p = tmp->link;
      tmp->link = NULL;
      free_tree (tmp);
      }
    else
      p = &((*p)->link);
}

void check_nextified_loop_returns (NODEPNT n)
{
  IntList *it = NULL;
  NODEPNT m, d;
  int i;

  for (i=0; i<NUM_CHILDREN; i++)
    if (n->child[i] != NULL)
      check_nextified_loop_returns (n->child[i]);

  if (n->link != NULL)
    check_nextified_loop_returns (n->link);

  if ((n->nodetype==FOR) || (n->nodetype==WHILE))
    {
      /* gather a list of all nextified variables */
      for (m=n->child[1]->child[0]; m!=NULL; m=m->link)
	if (m->nodetype==ASSIGN)
	  for (d=m->child[0]; d!=NULL; d=d->link)
	    if ((d->nodetype==IDENT) && (d->child[0]->nodetype==NEXT))
	      add_to_list (d->Defval, &it);

      for (m=n->child[1]->child[1]; m!=NULL; m=m->link)
	if ((m->nodetype==IDENT) && (!in_list (m->Defval, &it)))
	  handle_error (ErrScopeIllegalNextified,
	    		Fatal, m->Lineno, m->Srcfile, m->Srcfunc, m->Symval);

      free_intlist (&it);
    }
}
