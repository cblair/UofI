#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "tree.h"

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


int tree_init()
{
	YY_TREE = NULL;
}


int tree_del()
{
	//TODO
}


int treeprint(struct tree *t, int depth)
{
	if(t == NULL)
	{
		return(1);
	}

  	int i;

  	//printf("%*s %s: %d\n", depth*2, " ", 
	//humanreadable(t->prodrule), t->nkids);
  	printf("%d %s: %d\n", depth, t->prodrule, t->nkids);
  
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
	retval->prodrule = strdup(prodrule); 
	retval->nkids = 1;
	
	//add kids from the tree args
	// start vargs
	register int i;
        va_list ap;
	va_start(ap, n_args);
	// add kids
	for(i = 1; i <= n_args; i++) {
		retval->kids[i - 1] = va_arg(ap, struct tree*);
	}

        va_end(ap);

	return(retval);
}


//takes token attributes from lexer, and returns a tree node
struct tree *tree_create_node_from_token(int cat, char *text, int lineno, char *fname)
{
	struct tree_token *new_token = (struct tree_token*)malloc(\
						sizeof(struct tree_token));
	new_token->cat = cat;
	new_token->text = strdup(text);
	new_token->lineno = lineno;
	new_token->fname = strdup(fname);

	struct tree *retval = (struct tree*)malloc(sizeof(struct tree));
	retval->prodrule = strdup(text);
	retval->prodrule[strlen(retval->prodrule) - 1] = 0; //terminate
	retval->nkids = 0;
	retval->leaf = new_token;
	
	return(retval);
}
