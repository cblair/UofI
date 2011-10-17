#ifndef _TREE_H
#define _TREE_H

#include <time.h>

#include "tree_node.h"
#include "darray.h"


#ifdef DEBUG_TREE
#define DEBUG_TREE_MSG(arg) (cout << arg << endl)
#else
#define DEBUG_TREE_MSG(arg) ;
#endif


extern int SUM_TEMP;


#define MAX_CHILDREN 2

class tree
{
private:

public:
	//members
	tree_node *tnp;
	darray *dp; //darray pointer, for tree_double use only
	int nchildren;
	int depth;  //how deep the current tree is
	tree *children[MAX_CHILDREN];

	//methods
	tree(int, darray*);
	~tree();
	bool copy(tree**);
	tree_node *gen_rand_nonterm_tree_node(darray*);//[non]terminal vals
	tree_node *gen_rand_term_tree_node(darray*); //terminal vals

	double eval();
	double fitness(double);

	bool is_term();
	bool is_nonterm();

	int count_terms();
	int count_nonterms();

	bool crossover(tree**,tree**);

	bool print(int);
	bool print_tnp_ntype();
};

//External tree stuff
bool mutate_nth_nonterm(tree**,int, int, int, darray*);
bool tree_replace_nth_nonterm(tree**,tree**, int);
bool tree_crossover(tree**,tree**);

#endif
