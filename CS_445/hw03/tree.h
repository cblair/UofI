#ifndef _TREE_H
#define _TREE_H

#include <stdarg.h>

struct tree {
	//int prodrule; //not sure on how to set the int yet
	char *prodrule;
	int nkids;
	struct tree *kids[9];
	struct tree_token *leaf; //points to tree_token, terminals only
};

struct tree_token {
	int cat;
	char *text;
	int lineno;
	char *fname;
};

int tree_init();
int tree_del();
int treeprint(struct tree *, int);
struct tree *tree_create_node(char *, int n_args, ...);
struct tree *tree_create_node_from_token(int, char*, int, char*);

#endif
