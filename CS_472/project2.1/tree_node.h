#ifndef _TREE_NODE_H
#define _TREE_NODE_H

#include <iostream>
#include <stdarg.h>

using namespace std; //for string

#define NTYPES 6 //how many types? see tree_node::node_type

class tree_node
{
//public enum here so private members can see
public:
	enum node_type
	{
		plus,
		minus,
		multi,
		div,
		tree_double,
		tree_var
	};

private:
	node_type ntype; //type of node (see node_type)
	double dval; //for tree_double types only
	double *ddp; //darray-double-pointer

public:

	tree_node(tree_node::node_type val, int n_args, ...);
	bool print_ntype();
};

#endif
