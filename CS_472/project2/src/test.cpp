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
	tp = new tree_node(tree_node::plus, 0.0, NULL);
	delete tp;
	tp = new tree_node(tree_node::minus, 0.0, NULL);
	delete tp;
	tp = new tree_node(tree_node::multi, 0.0, NULL);
	delete tp;
	tp = new tree_node(tree_node::div, 0.0, NULL);
	delete tp;
	tp = new tree_node(tree_node::tree_double, 2.001, NULL);
	delete tp;
	tp = new tree_node(tree_node::tree_var, 0.0, &dp);
	delete tp;
	delete dp;

	//copy test
	cout << "Tree node copy test\n";
	cout << " Plus:\n";
	darray *dp1 = new darray(10, true);
	tree_node *tnp1;
	tree_node *tnp2;
	tnp1 = new tree_node(tree_node::plus, 0.0, NULL);
	tnp1->copy(&tnp2);
	tnp1->print_members();
	delete tnp1;
	tnp2->print_members();
	delete tnp2;
	delete dp1;

	cout << " Tree var:\n";
	dp1 = new darray(2, false);
	dp1->a[0] = 5;
	dp1->a[1] = 7;
	cout << "TS45: " << dp1 << endl;
	tnp1 = new tree_node(tree_node::tree_var, 0.0, &dp1);
	tnp1->copy(&tnp2);
	tnp1->print_members();
	delete tnp1;
	tnp2->print_members();
	dp1->a[0] = 9;
	dp1->a[1] = 9;
	tnp2->print_members();

	delete dp1;
}


bool test_darray()
{
	darray *dp1 = new darray(5, true);
	darray *dp2;
	dp1->copy(&dp2);

	cout << "test_darray: dp1: \n";
	dp1->print_vals();
	delete dp1;
	cout << "test_darray: dp2: \n";
	dp2->print_vals();
	delete dp2;
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


bool test_tree_copy()
{
	//Crossover test
	darray *dp1 = new darray(2, false);
	darray *dp2 = new darray(2, false);
	dp1->a[0] = 0.2;
	dp1->a[1] = 0.3;
	dp2->a[0] = 5;
	dp2->a[1] = 7;
	tree *tp1 = new tree(5, dp1);
	tree *tp2 = NULL;

	tp1->copy(&tp2);
	cout << "Tree copy test\n";
	cout << " Tree 1:\n";
	//tp1->print(0);
	cout << " " << tp1->eval() << endl;
	delete tp1;
	cout << " Tree 2:\n";
	//tp2->print(0);
	cout << " " << tp2->eval() << endl;
	
	delete tp2;
	delete dp1;
	delete dp2;
}


bool test_tree_replace()
{
	darray *dp1 = new darray(2, false);
	dp1->a[0] = 0.2;
	dp1->a[1] = 0.3;
	
	tree *tp1 = new tree(5, dp1);
	tree *tp2 = new tree(5, dp1);

	cout << "Tree replace test\n";
	cout << "Tree 2:\n";
	tp2->print(0);

	cout << "Tree 1 before replace:\n";
	tp1->print(0);
	SUM_TEMP = 0;
	tree_replace_nth_nonterm(&tp1, &tp2, 4);	
	delete tp2;
	cout << "Tree 1 after replace:\n";
	tp1->print(0);
	

	delete tp1;
}


bool test_tree_get_subtree()
{
	darray *dp1 = new darray(2, false);
	dp1->a[0] = 0.2;
	dp1->a[1] = 0.3;
	
	tree *tp1 = new tree(5, dp1);
	tree *tp2 = new tree(5, dp1);

	SUM_TEMP = 0;
	tree *tp3 = tp1->get_nth_nonterm_subtree(7);
	cout << "Tree get subtree: original tree:\n";
	tp1->print(0);
	cout << "subtree:\n";
	tp3->print(0);

	cout << "Replacing subtree with:\n";
	delete tp3;
	tp3 = new tree(5, dp1);
	tp3->print(0);
	cout << "Original tree now:\n";
	tp1->print(0);
}


bool test_tree_crossover()
{
	darray *dp1 = new darray(2, false);
	dp1->a[0] = 0.2;
	dp1->a[1] = 0.3;
	
	tree *tp1 = new tree(5, dp1);
	tree *tp2 = new tree(5, dp1);

	cout << "Tree crossover test:\n";
	cout << "Tree 1 before crossover:\n";
	tp1->print(0);
	cout << "Tree 2 before crossover:\n";
	tp2->print(0);
	tree_crossover(&tp1, &tp2);
	cout << "Tree 1 after crossover:\n";
	tp1->print(0);
	cout << "Tree 2 after crossover:\n";
	tp2->print(0);
}
