#include <iostream>
#include "main.h"
#include "tree_node.h"
#include "tree.h"


bool test_nodes()
{
	//create nodes
	darray *dp = new darray(200);
	tree_node *tp;
	tp = new tree_node(tree_node::plus, 0);
	tp = new tree_node(tree_node::minus, 0);
	tp = new tree_node(tree_node::multi, 0);
	tp = new tree_node(tree_node::div, 0);
	tp = new tree_node(tree_node::tree_double, 1, 2.001);
	tp = new tree_node(tree_node::tree_var, 1, dp);
}


bool test_darray()
{
	darray *dp = new darray(200);
	dp->print_vals();
}


bool test_trees()
{
	tree *tp;
	darray *dp = new darray(200);
	tp = new tree(5, dp);
	tp = new tree(5, dp);
	tp = new tree(5, dp);
	tp = new tree(5, dp);
	tp = new tree(5, dp);
	tp = new tree(5, dp);

	tp->print(0);
}
