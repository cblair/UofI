#include "sassy.h"
#include "node.h"
#include "deftab.h"
#include "error.h"

/*  This module does a variety of flow and variable-related checks on
 *  a SA-C program:
 *	1 - It checks the use of illegal "nextified" variables:
 *		- 'next' outside of a 'for' or 'while' loop
 *		- a variable nextified more than once
 *		- a variable that is created and nextified in the same
 *		  loop body
 *		- a loop generator target variable that is nextified in
 *		  that loop
 *		- NOTE: DO WE ALSO NEED TO MAKE SURE A NEXTIFIED VARIABLE
 *		  IS NOT USED IN A GENERATOR EXPRESSION?
 *	2 - It checks for generator target variables that are referenced
 *	    in the same generator. For example, the variable 'i' in
 *		for uint8 i <-- [n] cross uint8 j <-- [i]
 *	3 - It checks for 'range' expressions that use a variable whose
 *	    value is created within the loop or its generator(s)
 */

void flow_check_nextified (NODEPNT);
void check_stmt_spine_nextified (NODEPNT, int, IntList**);
void check_expr_nextified (NODEPNT);
void locals_from_generators (NODEPNT, IntList**);
void flow_check_loop_gens (NODEPNT);
void gather_gen_targets (NODEPNT, IntList**);
void gather_assign_targets (NODEPNT, IntList**);
void check_for_target_use (NODEPNT, IntList**);
void check_for_range_use (NODEPNT, IntList**);
void check_for_ident_use (NODEPNT, IntList**);
int in_list (int, IntList**);
void add_to_list (int, IntList**);
void free_intlist (IntList**);


void flow (NODEPNT root)
    {
    flow_check_nextified (root);
    flow_check_loop_gens (root);
    }


void flow_check_loop_gens (NODEPNT n)
    {
    IntList *targets = NULL;
    int i;

    for (i=0; i<NUM_CHILDREN; i++)
	if (n->child[i] != NULL)
	    flow_check_loop_gens (n->child[i]);

    if (n->link != NULL)
	flow_check_loop_gens (n->link);

    switch (n->nodetype)
	{
	case FOR :
	    gather_gen_targets (n->child[0], &targets);

	    check_for_target_use (n->child[0], &targets);

	    gather_assign_targets (n->child[1]->child[0], &targets);

	    check_for_range_use (n->child[1]->child[1], &targets);

	    free_intlist (&targets);
	    break;
	default :
            break;
	}
    }


void check_for_range_use (NODEPNT n, IntList **targets)
    {
    int i;

    if (n == NULL)
	return;

    switch (n->nodetype)
	{
	case ACCUM :
	    check_for_range_use (n->child[0], targets);
	    check_for_range_use (n->child[2], targets);
	    check_for_ident_use (n->child[1], targets);
	    break;
	case REDUCE_HIST :
	    check_for_range_use (n->child[0], targets);
	    check_for_range_use (n->child[1], targets);
	    check_for_ident_use (n->child[2], targets);
	    break;
	default :
    	    for (i=0; i<NUM_CHILDREN; i++)
		if (n->child[i] != NULL)
		    check_for_range_use (n->child[i], targets);
	}

    if (n->link != NULL)
	check_for_range_use (n->link, targets);
    }

    
void check_for_ident_use (NODEPNT n, IntList **list)
    {
    int i;

    if (n->nodetype == IDENT)
	{
	if (in_list (n->Defval, list))
	    handle_error (ErrFlowLoopVarUsedInRange,
	    		Fatal, n->Lineno, n->Srcfile, n->Srcfunc, n->Symval);
	}

    for (i=0; i<NUM_CHILDREN; i++)
	if (n->child[i] != NULL)
	    check_for_ident_use (n->child[i], list);

    if (n->link != NULL)
	check_for_ident_use (n->link, list);
    }


void gather_assign_targets (NODEPNT n, IntList **targets)
    {
    NODEPNT m;

    for ( ; n!=NULL; n=n->link)
	if (n->nodetype == ASSIGN)
	    for (m=n->child[0]; m!=NULL; m=m->link)
		add_to_list (m->Defval, targets);
    }


void gather_gen_targets (NODEPNT n, IntList **targets)
    {
    NODEPNT m;

    /*
    printf ("entering 'gather_gen_targets'\n"); fflush (stdout);
    */

    switch (n->nodetype)
	{
	case WINDOW :
	    add_to_list (n->child[0]->Defval, targets);
	    for (m=n->child[3]; m!=NULL; m=m->link)
		if (m->nodetype == IDENT)
		    add_to_list (m->Defval, targets);
	    break;
	case ELE_GEN :
	case SLICE_GEN:
	    add_to_list (n->child[0]->Defval, targets);
	    for (m=n->child[4]; m!=NULL; m=m->link)
		if (m->nodetype == IDENT)
		    add_to_list (m->Defval, targets);
	    break;
	case SCALAR_GEN :
	    for (m=n->child[0]; m!=NULL; m=m->link)
		add_to_list (m->Defval, targets);
	    break;
	case DOT_PROD :
	case CROSS_PROD :
	    for (m=n->child[0]; m!=NULL; m=m->link)
		gather_gen_targets (m, targets);
	default :
            break;
	}

    /*
    printf ("leaving 'gather_gen_targets'\n"); fflush (stdout);
    */
    }


void check_for_target_use (NODEPNT n, IntList **targets)
    {
    int i;

    if (n == NULL)
	return;

    /*
    printf ("entering 'check_for_target_use', node %d\n", n); fflush (stdout);
    */

    switch (n->nodetype)
	{
	case WINDOW :
	    check_for_target_use (n->child[0]->child[1], targets);
	    check_for_target_use (n->child[1], targets);
	    check_for_target_use (n->child[2], targets);
	    if (n->link != NULL)
		check_for_target_use (n->link, targets);
	    break;
	case ELE_GEN :
	case SLICE_GEN :
	    check_for_target_use (n->child[2], targets);
	    check_for_target_use (n->child[3], targets);
	    if (n->link != NULL)
		check_for_target_use (n->link, targets);
	    break;
	case SCALAR_GEN :
	    check_for_target_use (n->child[1], targets);
	    check_for_target_use (n->child[2], targets);
	    if (n->link != NULL)
		check_for_target_use (n->link, targets);
	    break;
	case IDENT :
	    if (in_list (n->Defval, targets))
	    	handle_error (ErrFlowGenVarUsedInGen,
		    		Fatal, n->Lineno, n->Srcfile, n->Srcfunc, n->Symval);
	    for (i=0; i<NUM_CHILDREN; i++)
		if (n->child[i] != NULL)
		    check_for_target_use (n->child[i], targets);
	    if (n->link != NULL)
		check_for_target_use (n->link, targets);
	    break;
	default :
	    for (i=0; i<NUM_CHILDREN; i++)
		if (n->child[i] != NULL)
		    check_for_target_use (n->child[i], targets);
	    if (n->link != NULL)
		check_for_target_use (n->link, targets);
	    break;
	}

    /*
    printf ("leaving 'check_for_target_use'\n"); fflush (stdout);
    */
    }


void flow_check_nextified (NODEPNT root)
    {
    NODEPNT n;

    /*
    printf ("entering 'flow_check_nextified'\n"); fflush (stdout);
    */

    for (n=root; n!=NULL; n=n->link)
	if (n->child[3] != NULL)
	    {
	    check_stmt_spine_nextified (n->child[3]->child[0], FALSE, NULL);
	    check_expr_nextified (n->child[3]->child[1]);
	    }

    /*
    printf ("leaving 'flow_check_nextified'\n"); fflush (stdout);
    */
    }


void check_stmt_spine_nextified (NODEPNT spine, int legal, IntList **local_list)
    {
    NODEPNT n, m;
    IntList *next_list = NULL;

    /*
    printf ("entering 'check_stmt_spine_nextified'\n"); fflush (stdout);
    */

    /* for each ASSIGN node in this spine... */
    for (n=spine; n!=NULL; n=n->link)
        {
	if ((n->nodetype == PRINT) || (n->nodetype == ASSERT))
	    {
	    check_expr_nextified (n->child[0]);
	    continue;
	    }

	if (! legal)
	    {
	    /* for each IDENT on the LHS... */
	    for (m=n->child[0]; m!=NULL; m=m->link)
		{
		if (m->nodetype != IDENT)
		    continue;

		assert (m->child[0] != NULL);

	        if (m->child[0]->nodetype == NEXT)
	    	    handle_error (ErrFlowIllegalNextifiedVar,
		    		Fatal, m->Lineno, m->Srcfile, m->Srcfunc, m->Symval);
		}
	    }
	else
	    {
	    /* for each IDENT on the LHS... */
	    for (m=n->child[0]; m!=NULL; m=m->link)
		{
		if (m->nodetype != IDENT)
		    continue;

		assert (m->child[0] != NULL);

	        if (m->child[0]->nodetype == NEXT)
		    {
		    if (in_list (m->Defval, local_list))
		        handle_error (ErrFlowLocalNextifiedVar,
				Fatal, m->Lineno, m->Srcfile, m->Srcfunc, m->Symval);
		    else if (in_list (m->Defval, &next_list))
		        handle_error (ErrFlowAlreadyNextifiedVar,
				Fatal, m->Lineno, m->Srcfile, m->Srcfunc, m->Symval);
		    else
		        add_to_list (m->Defval, &next_list);
		    }
		else
		    {
		    add_to_list (m->Defval, local_list);
		    }
		}
	    }

        check_expr_nextified (n->child[1]);
	}

    free_intlist (&next_list);

    /*
    printf ("leaving 'check_stmt_spine_nextified'\n"); fflush (stdout);
    */
    }


void check_expr_nextified (NODEPNT expr)
    {
    IntList *locals = NULL;
    int i;

    /*
    printf ("entering 'check_expr_nextified'\n"); fflush (stdout);
    */

    assert (expr != NULL);

    switch (expr->nodetype)
        {
	case FOR :
	    /* put generator variables into the 'locals' list */
	    locals_from_generators (expr->child[0], &locals);
	    check_stmt_spine_nextified (expr->child[1]->child[0], TRUE, &locals);
	    free_intlist (&locals);
	    check_expr_nextified (expr->child[1]->child[1]);
	    break;
	case WHILE :
	    check_expr_nextified (expr->child[0]);
	    check_stmt_spine_nextified (expr->child[1]->child[0], TRUE, &locals);
	    free_intlist (&locals);
	    check_expr_nextified (expr->child[1]->child[1]);
	    break;
	case CONDITIONAL :
	    check_expr_nextified (expr->child[0]);
	    check_stmt_spine_nextified (expr->child[1]->child[0], FALSE, &locals);
	    free_intlist (&locals);
	    check_expr_nextified (expr->child[1]->child[1]);
	    check_stmt_spine_nextified (expr->child[2]->child[0], FALSE, &locals);
	    free_intlist (&locals);
	    check_expr_nextified (expr->child[2]->child[1]);
	    break;
	default :
	    for (i=0; i<NUM_CHILDREN; i++)
	        if (expr->child[i] != NULL)
		    check_expr_nextified (expr->child[i]);
	}

    if (expr->link != NULL)
	check_expr_nextified (expr->link);

    /*
    printf ("leaving 'check_expr_nextified'\n"); fflush (stdout);
    */
    }


void locals_from_generators (NODEPNT gens, IntList **locals)
    {
    NODEPNT m;

    /*
    printf ("entering 'locals_from_generators'\n"); fflush (stdout);
    */

    switch (gens->nodetype)
        {
	case SCALAR_GEN :
	    for (m=gens->child[0]; m!=NULL; m=m->link)
	        add_to_list (m->Defval, locals);

	    check_expr_nextified (gens->child[1]);
	    if (gens->child[2] != NULL)
	        check_expr_nextified (gens->child[2]);
	    break;
	case ELE_GEN :
	case SLICE_GEN :
	    m = gens->child[0];
	    add_to_list (m->Defval, locals);

	    for (m=gens->child[4]; m!=NULL; m=m->link)
	        if (m->nodetype == IDENT)
		    add_to_list (m->Defval, locals);

	    check_expr_nextified (gens->child[2]);
	    if (gens->child[3] != NULL)
	        check_expr_nextified (gens->child[2]);
	    break;
	case WINDOW :
	    m = gens->child[0];
	    add_to_list (m->Defval, locals);

	    for (m=gens->child[3]; m!=NULL; m=m->link)
	        if (m->nodetype == IDENT)
		    add_to_list (m->Defval, locals);

	    check_expr_nextified (gens->child[0]->child[1]);
	    check_expr_nextified (gens->child[1]);
	    if (gens->child[2] != NULL)
	        check_expr_nextified (gens->child[2]);
	    break;
	case DOT_PROD :
	case CROSS_PROD :
	    for (m=gens->child[0]; m!=NULL; m=m->link)
		locals_from_generators (m, locals);
	    break;
	default :
	    assert (0);
	}

    /*
    printf ("leaving 'locals_from_generators'\n"); fflush (stdout);
    */
    }
