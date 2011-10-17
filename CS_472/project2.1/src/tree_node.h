#ifndef _TREE_NODE_H
#define _TREE_NODE_H

#include <iostream>

#include "darray.h"

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
	int dpi;     //index the ddp points to in dp
	darray *dp; //darray pointer
	double *ddp; //double pointer to rand element in this->dp

public:

	tree_node(tree_node::node_type, double, darray**);
	bool copy(tree_node**);
	double get_dval();
	double get_ddp_val();
	tree_node::node_type get_ntype();

	bool set_ddp(int);

	bool print_ntype();
	bool print_dval();
	bool print_ddp();
	bool print_members();
};

#endif
