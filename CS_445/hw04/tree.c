///////////////////////////////////////////////////////////////////////////////
////Class:        CS 445
////Semester:     Fall 2011
////Assignment:   Homework 3
////Author:       Colby Blair
////File name:    tree.c
////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "tree.h"
#include "symtabc.h"
#include "parser.tab.h"
#include "main.h"

//symbol table stuff
static int maxTable_;          // this is how big the table is now since it can grow
static SymTabEntry *table_;    // the table is simply a fancy stack of SymTabEntries


/*
This is the structure I think I am creating

	+---------------+
	|tree*		|
	|		|
	|prodrule=1	|
	|nkids=1	|
	|kids=		|----------------------- ...more
	|leaf=NULL	|	|
	+---------------+	|
				|
			+---------------+
			|tree*		|
			|		|
			|prodrule=2	|
			|nkids=0	|
			|kids=NULL	|
			|leaf=		|---------------
			+---------------+		|
						+---------------+
						|tree_token*	|
						|		|
						|cat=256	|
						|text="int"	|
						|lineno=3	|
						|fname="main.as"|
						+---------------+
*/


struct tree *YY_TREE;


void tree_print_error()
{
	//TODO: actualy print error
}


int tree_init()
{
	//init the global tree
	YY_TREE = NULL;

	//init the global symbol table
	SymTab_init(tree_print_error);
}


int tree_del()
{
	//TODO: actually delete tree

	SymTab_free();
}


int treeprint(struct tree *t, int depth)
{
	if(t == NULL)
	{
		return(1);
	}

  	//printf("%*s %s: %d\n", depth*2, " ", 
	//humanreadable(t->prodrule), t->nkids);
	
	//terminal
	if(t->leaf != NULL)
	{
		printf("%d %*s = \"%s\": %d\n", 
					depth, 
					depth + strlen(t->prodrule),
					t->leaf->text,
					t->prodrule, t->nkids);
	
	}
	else
	{
		printf("%d %*s: %d\n", 	depth, depth + strlen(t->prodrule), 
				t->prodrule, t->nkids);
  	}

  	int i;
  	for(i=0; i < t->nkids; i++)
    	{
		treeprint(t->kids[i], depth+1);
	}
	
	//TODO: meaningful retval
	return(0);
}


//needs lots of trees as args
struct tree *tree_create_node(char *prodrule, int n_args, ...)
{
	//create the new node
	struct tree *retval = (struct tree*)malloc(sizeof(struct tree));
	//TODO: malloc check

	//init default member values
	retval->prodrule = strdup(prodrule); 
	retval->nkids = n_args;
	retval->leaf = NULL;
	
	//add kids from the tree args
	// start vargs
	register int i;
        va_list ap;
	// init kids
	int j;
	for(j = 0; j < MAX_KIDS; j++) {
		retval->kids[j] = NULL;
	}
	va_start(ap, n_args);
	// add kids
	// TODO: breaks if n_args bigger than MAX_KIDS
	for(i = 1; i <= n_args; i++) {
		retval->kids[i - 1] = va_arg(ap, struct tree*);
	}

        va_end(ap);

	return(retval);
}


//takes token attributes from lexer, and returns a tree node
struct tree *tree_create_node_from_token(int cat, char *text, int lineno, 
						char *fname)
{
	struct tree_token *new_token = (struct tree_token*)malloc(\
						sizeof(struct tree_token));
	new_token->cat = cat;
	new_token->text = strdup(text);
	new_token->lineno = lineno;
	new_token->fname = strdup(fname);

	struct tree *retval = (struct tree*)malloc(sizeof(struct tree));
	retval->prodrule = strdup(text);
	//retval->prodrule[strlen(retval->prodrule) - 1] = 0; //terminate
	retval->nkids = 0;
	retval->leaf = new_token;
	
	return(retval);
}


//generate three address code (TAC)
// so far only traverses the tree and:
// 1. pushes and pops symbols in a symbol table stack according to scope
int tree_gen_tac(struct tree *t, int depth)
{
	if(t == NULL)
	{
		return(1);
	}

	tree_update_sym_tab(t);

  	int i;
  	for(i=0; i < t->nkids; i++)
    	{
		tree_gen_tac(t->kids[i], depth+1);
	}
	
	//TODO: meaningful retval
	return(0);

}


void tree_get_subtree(char *prodrule, struct tree *from, struct tree **retval)
{
	if(from == NULL)
	{
		(*retval) = NULL;
		return;
	}

	//match
	if(strcmp(prodrule, from->prodrule) == 0)
	{
		(*retval) = from;
		return;
	}

	int i;
	for(i = 0; i < from->nkids; i++)
	{
		tree_get_subtree(prodrule, from->kids[i], &(*retval));
		if((*retval) != NULL)
		{
			return;
		}
	}

	//didn't find anything down this path
	(*retval) = NULL;
	return;
}


char *tree_get_opt_type(struct tree* t)
{
	char *type = NULL;

	//get larger type expression subtree
	struct tree *exp_subtree = NULL;
	tree_get_subtree("optionalTypeExpression", t, &exp_subtree);
	
	if(exp_subtree != NULL)
	{
		//get type subtree, which is usualy ident->Type
		struct tree *type_subtree = NULL;
		tree_get_subtree("ident", exp_subtree, &type_subtree);

		if(type_subtree != NULL)
		{
			type = type_subtree->kids[0]->prodrule);
		}
	}

	return(type);
}


int tree_update_sym_tab(struct tree *t)
{
	//terminals
	if(t->leaf != NULL)
	{
		//enter a scope	
		if(strcmp(t->prodrule, "package") == 0
			|| strcmp(t->prodrule, "class") == 0
			|| strcmp(t->prodrule, "function") == 0)
		
		{
			//symbol table scope
			SymTab_enter_scope(t->prodrule);

			#ifdef DEBUG_SYMTAB
			printf("Entering scope '%s'. New symtab:\n", 
								t->prodrule);
			SymTab_print(); //current symbol table
			#endif
		}
		//leave a scope
		else if(t->leaf->cat == RCURLY)
		{
			#ifdef DEBUG_SYMTAB
			printf("Leaving scope. ");
			printf(" Old symtab:\n");
			SymTab_print(); //current symbol table
			#endif
			SymTab_leave_scope(); //the current scope in SymTab

			#ifdef DEBUG_SYMTAB
			printf(" New symtab:\n");
			SymTab_print(); //current symbol table
			#endif
		}
	}
	//nonterminals
	else
	{
		//insert new variable definition in
		if(strcmp(t->prodrule, "variableDeclarator") == 0)
		{
			//TODO: need to use more generic getter
			struct tree_token *symbol = t->kids[0]->kids[0]->leaf;
			//Get optional type
			char *type = tree_get_opt_type(&t);
			printf("TS299: %s\n", type);
		
			//if variable already exists, fail
			bool status = SymTab_insert(symbol->text, table_);
			if(status == false)
			{
			 fprintf(stderr, 
			 "ERROR: variable '%s' redeclared in '%s' on line '%d'\n", 
			 symbol->text, symbol->fname, symbol->lineno);
			 exit(ERROR_SEMANTIC);
			}
		}
	}
	
}
