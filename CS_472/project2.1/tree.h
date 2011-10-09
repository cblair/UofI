#ifndef _TREE_H
#define _TREE_H

#include <time.h>
#include "tree_node.h"


#define MAX_BUF 200
class darray
{
private:
	int size;

public:
	double a[MAX_BUF];

	darray(int size);

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
	int nchildren;
	tree *children[MAX_CHILDREN];

public:
	tree(int, darray*);
	tree_node *gen_rand_tree_node(darray*);//where [non]terminal vals go
	bool print(int);
};


#endif
