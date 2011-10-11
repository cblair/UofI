#ifndef _TREE_NODE_H
#define _TREE_NODE_H

#include <iostream>
#include <stdarg.h>

using namespace std; //for string

//how many types? see tree_node::node_type
// used in tree::gen_rand_node()
#define NTYPES 4 

//how many terminal types? see tree_node::node_type
// used in tree_gen_rand_term_tree_node()
#define NTERMTYPES 2 

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
		tree_double, 	//terminal
		tree_var,	//terminal
		null
	};

private:
	node_type ntype; //type of node (see node_type)
	double dval; //for tree_double types only
	double *ddp; //darray-double-pointer

public:

	tree_node(tree_node::node_type val, int n_args, ...);
	double get_dval();
	double get_ddp_val();
	tree_node::node_type get_ntype();
	bool print_ntype();
};

#endif
