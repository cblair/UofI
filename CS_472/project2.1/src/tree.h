#ifndef _TREE_H
#define _TREE_H

#include <time.h>
#include "tree_node.h"

extern int SUM_TEMP;

#define MAX_BUF 200
class darray
{
private:
	int size;

public:
	double a[MAX_BUF];

	darray(int, bool);

	//getters
	double get_val(int);
	int get_size();	

	//debug
	bool print_vals();
};


#define MAX_CHILDREN 2

class tree
{
private:
	tree_node *tnp;
	darray *dp; //darray pointer, for tree_double use only

public:
	int nchildren;
	tree *children[MAX_CHILDREN];

	tree(int, darray*);
	~tree();
	tree_node *gen_rand_nonterm_tree_node(darray*);//[non]terminal vals
	tree_node *gen_rand_term_tree_node(darray*); //terminal vals

	double eval();
	double fitness(double);

	bool is_term();
	bool is_nonterm();

	int count_terms();
	int count_nonterms();

	bool print(int);
};


bool mutate_nth_nonterm(tree**,int, int, int, darray*);

#endif
