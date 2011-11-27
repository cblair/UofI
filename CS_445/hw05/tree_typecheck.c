///////////////////////////////////////////////////////////////////////////////
////Class:        CS 445
////Semester:     Fall 2011
////Assignment:   Homework 4
////Author:       Colby Blair
////File name:    tree_symtab_gen.h
////////////////////////////////////////////////////////////////////////////////

#include "tree.h"
#include "symtabc.h"
#include "tree_typecheck.h"
#include "parser.tab.h"
#include "main.h"


//get's the type of the leaf; determines a literal type, or symtab lookups
// a ident's type
char *tree_get_leaf_type(struct tree *t)
{
	if(t == NULL)
	{
		return(NULL);
	}

	//if t is an ident, return its type, defined in symtab
	if(strcmp(t->prodrule, "ident") == 0)
	{
		SymTabEntry *p = SymTab_lookup(t->kids[0]->prodrule);
		if(p == NULL)
		{
			return(NULL);
		}
		return(p->type);
	}
	//else if we hit the leaf, t is a literal. return the translated
	// type?
	else if(t->leaf != NULL)
	{
		return(tree_type_trans_lit(t->prodrule));
	}

	int i;
	for(i = 0; i < t->nkids; i++)
	{
		char *retval = tree_get_leaf_type(t->kids[i]);
		if(retval != NULL)
		{
			return(retval);
		}
	}

	//didn't find a leaf? should never happen
	return(NULL);
}


//translates a literal val, always a char*, to a real AS3 type
char *tree_type_trans_lit(char *lval)
{
	if(lval == NULL)
	{
		return(NULL);
	}
	
	//String
	// if begins and ends with quotes
	if(
		(lval[0] == '"' && lval[strlen(lval) - 1] == '"')
		||
		(lval[0] == '\'' && lval[strlen(lval) - 1] == '\'')
	)
	{
		return("String");
	}
	//int
	else if(
		atoi(lval) != 0 //char * to non-zero int
		|| strcmp(lval, "0") == 0 //zero int
	)
	{
		return("int");
	}
	//TODO: Number, unint, Boolean, Object

	//don't know, some user defined type
	return("user");
}


//returns 0 for True, 1 for False
int tree_type_is_assignable(char *to, char *from)
{
	//if a type for one is not yet defined, its ok
	if(to == NULL || from == NULL)
	{
		return(0);
	}

	//int
	if(
		strcmp(to, "int") == 0 && strcmp(from, "int") == 0
	)
	{
		return(0);
	}
	//String
	else if(
		strcmp(to, "String") == 0 && strcmp(from, "String") == 0
	)
	{
		return(0);
	}
	//Boolean
	else if(
		strcmp(to, "Boolean") == 0 && strcmp(from, "Boolean") == 0
	)
	{
		return(0);
	}
	//anything from a function
	else if( strcmp(from, "function") == 0)
	{
		return(0);
	}

	return(1);
}



int tree_typecheck_assignmentExpression(struct tree *t)
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

	return(0); //success
}
