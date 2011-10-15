#include <iostream>
#include <stdlib.h>
#include "main.h"
#include "tree_node.h"
#include "tree.h"


bool test_nodes()
{
	//create nodes
	darray *dp = new darray(200, true);
	tree_node *tp;
	tp = new tree_node(tree_node::plus, 0);
	delete tp;
	tp = new tree_node(tree_node::minus, 0);
	delete tp;
	tp = new tree_node(tree_node::multi, 0);
	delete tp;
	tp = new tree_node(tree_node::div, 0);
	delete tp;
	tp = new tree_node(tree_node::tree_double, 1, 2.001);
	delete tp;
	tp = new tree_node(tree_node::tree_var, 1, dp);
	delete tp;

	delete dp;
}


bool test_darray()
{
	darray *dp = new darray(200, true);
	dp->print_vals();
	delete dp;
}


bool test_trees()
{
	tree *tp;
	darray *dp = new darray(200, true);
	dp->a[0] = 0.2;
	dp->a[1] = 0.3;

	//test making lots of trees
	for(int i = 0; i < 500; i++)
	{
		tp = new tree(5, dp);
		delete tp;
	}
	delete dp;
	cout << "Finished bulk tree creation test\n";

	//eval a tree
	cout << "Eval tree test\n";
	dp = new darray(2, false);
	dp->a[0] = 0.2;
	dp->a[1] = 0.3;
	tp = new tree(5, dp);
	tp->print(0);
	cout << "Tree has " << tp->count_terms() << " terminal(s).\n";
	cout << "Tree has " << tp->count_nonterms() << " non-terminal(s).\n";
	delete dp;
	delete tp;

	//deep tree eval
	cout << "Deep tree eval time test: ";
	clock_t stime, etime, ttime;
	int precision = 1000;
  	stime = (clock () / CLOCKS_PER_SEC) * precision;
	dp = new darray(2, false);
	dp->a[0] = 0.2;
	dp->a[1] = 0.3;
	tp = new tree(16, dp);
  	etime = (clock () / CLOCKS_PER_SEC) * precision;
	ttime = (etime - stime) / precision;
	cout << ttime << " second(s)\n";
	//tp->print(0);
	delete dp;
	delete tp;

	//Mutate test
	tp = new tree(5, dp);
	dp = new darray(2, false);
	dp->a[0] = 0.2;
	dp->a[1] = 0.3;
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

	delete dp;
}


bool tree_crossover_test()
{
	//Crossover test
	darray *dp1 = new darray(2, false);
	darray *dp2 = new darray(2, false);
	dp1->a[0] = 0.2;
	dp1->a[1] = 0.3;
	dp2->a[0] = 5;
	dp2->a[1] = 7;
	tree *tp1 = new tree(5, dp1);
	tree *tp2 = new tree(5, dp2);
	
	
	delete tp1;
	delete tp2;
	delete dp1;
	delete dp2;
}
