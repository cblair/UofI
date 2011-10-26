///////////////////////////////////////////////////////////////////////////////
////Class:        CS 445
////Semester:     Fall 2011
////Assignment:   Homework 3
////Author:       Colby Blair
////File name:    tree.h
/////////////////////////////////////////////////////////////////////////////////

#ifndef _TREE_H
#define _TREE_H

#define MAX_KIDS 9

#include <stdarg.h>

struct tree {
	//int prodrule; //not sure on how to set the int yet
	char *prodrule;
	int nkids;
	struct tree *kids[MAX_KIDS];
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
int tree_gen_tac(struct tree *, int);

//getters of subtree members
void tree_get_subtree(char *, struct tree*, struct tree**);
char *tree_get_opt_type(struct tree*);

//internal only
//int tree_update_sym_tab(struct tree *);

#endif
