#include <iostream>
#include "main.h"
#include "tree_node.h"
#include "tree.h"


//Pardon the memory leaks, just testing


bool test_nodes()
{
	//create nodes
	darray *dp = new darray(200, true);
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
	darray *dp = new darray(200, true);
	dp->print_vals();
}


bool test_trees()
{
	//test making lots of trees
	tree *tp;
	darray *dp = new darray(200, true);
	tp = new tree(5, dp);
	tp = new tree(5, dp);
	tp = new tree(5, dp);
	tp = new tree(5, dp);
	tp = new tree(5, dp);

	//eval a tree
	dp = new darray(2, false);
	dp->a[0] = 0.2;
	dp->a[1] = 0.3;
	tp = new tree(5, dp);
	tp->print(0);

	cout << "Tree has " << tp->count_terms() << " terminal(s).\n";
	cout << "Tree has " << tp->count_nonterms() << " non-terminal(s).\n";

	int n = 10;
	cout << "Term mutation on " << n << " terminal\n";
	SUM_TEMP = 0;
	mutate_nth_nonterm(&tp, n, 0, 5, dp);
	cout << "After mutation:\n";
	tp->print(0);

	//x^3 + 5y^3 - 4xy + 7
	//= (.2)^3 + 5(.3)^3 - 4(.2)(.3) + 7
	//= .008 + .135 - .24 + 7
	//= 6.903
	dp->a[0] = 0.2;
	dp->a[1] = 0.3;
	cout << "Tree fitness: " << tp->fitness(6.903) << endl;
	cout << "Tree eval 1: " << tp->eval() << endl;

	//x^3 + 5y^3 - 4xy + 7
	//
	dp->a[0] = 5;
	dp->a[1] = 7;
	cout << "Tree eval 2: " << tp->eval() << endl;

	//x^3 + 5y^3 - 4xy + 7
	//
	dp->a[0] = 13;
	dp->a[1] = 20;
	cout << "Tree eval 3: " << tp->eval() << endl;


}
