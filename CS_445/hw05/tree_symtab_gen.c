///////////////////////////////////////////////////////////////////////////////
////Class:        CS 445
////Semester:     Fall 2011
////Assignment:   Homework 4
////Author:       Colby Blair
////File name:    tree_symtab_gen.h
////////////////////////////////////////////////////////////////////////////////

#include "tree.h"
#include "symtabc.h"
#include "tree_symtab_gen.h"
#include "parser.tab.h"
#include "main.h"


//TODO: need to split this function up
int tree_gen_assignmentExpression(struct tree *t)
{
	//Expression type check
	//see if assignment op is in tree
	struct tree *subtree = NULL;
	tree_get_subtree("=",t, &subtree);
	//if no 'op' branch, this is a right hand assignmentExpression with a
	// literal, which we'll ignore now and handle when we see the entire 
	//expression later (below)
	if(subtree == NULL)
	{
		return(0);
	}
	//else, this is an entire assignmentExpression to a identifier
	else 
	{	
		//get the token name
		char *tname = tree_get_ident(t);		

		SymTabEntry *p = SymTab_lookup(tname);
	
		//get the subtree symbol for report info
		struct tree *temp = NULL;
		tree_get_subtree(tname, t, &temp);
	
		//Use before declaration check
		if(p == NULL)
		{
			if(temp != NULL)
			{
				struct tree_token *symbol = temp->leaf;
		 		fprintf(stderr,
		  	 	"ERROR: identifier '%s' used before declared, in file '%s' on line %d\n", 
					tname, symbol->fname, symbol->lineno);
			}
			else
			{
				fprintf(stderr,
		  	 	"ERROR: identifier '%s' used before declared, and some horrible internal errors also occured.\n",
					tname); 
			}
		 
			exit(ERROR_SEMANTIC);
		}

		//Assignment to const check
		if(p->aux_flag != NULL 
			&& strcmp(p->aux_flag, "const") == 0)
		{
			if(temp != NULL)
		 	{
				struct tree_token *symbol = temp->leaf;
				fprintf(stderr,
				 "ERROR: assignment to constant '%s', in file '%s' on line %d\n", 
			 	tname, symbol->fname, symbol->lineno);
	
			}
			else
			{
				fprintf(stderr,
				 "ERROR: assignment to constant '%s', and some horrible internal errors also occured.\n",
				 tname);
			}

			exit(ERROR_SEMANTIC);
		}
	} //end else (if subtree != NULL)

	//Type assignment check
	//get left and right branches of tree
	struct tree *ltree, *rtree;
	ltree = t->kids[0]; //left hand side
	rtree = t->kids[2]; //right hand side

	//get ident on left branch
	struct tree *itree = NULL; //ident tree
	tree_get_subtree("ident", ltree, &itree);

	//get value on right value
	struct tree *vtree = NULL; //value tree
	tree_get_subtree("primaryExpression", rtree, &vtree);

	//get branch types	
	char *itype, *vtype;
	itype = tree_get_leaf_type(itree);
	vtype = tree_get_leaf_type(vtree);

	if(tree_type_is_assignable(itype, vtype) != 0)
	{
		fprintf(stderr, "ERROR: type '%s' and '%s' are not assignable, in file '' on line ''. Exiting...\n", itype, vtype);
		exit(ERROR_SEMANTIC);
	}

	//Generate tac
	//TODO: ...

	return(0); //success
}
