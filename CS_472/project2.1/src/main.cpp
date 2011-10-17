#include <iostream>
#include "darray.h"
#include "tree_gp.h"

#ifdef DEBUG
#include "test.h"
#endif

using namespace std;

int main()
{
	#ifdef DEBUG
	test_nodes();
	test_darray();
	test_trees();
	test_tree_copy();
	test_tree_replace();
	test_tree_crossover();
	exit(1);
	#endif

	//Main eval
	darray *dp1 = new darray(2, false);
	dp1->a[0] = .2;
	dp1->a[1] = .3;
	tree_gp *tgp1 = new tree_gp(500, 5, &dp1);
	//x^3 + 5y^3 - 4xy + 7
	//= (.2)^3 + 5(.3)^3 - 4(.2)(.3) + 7
	//= .008 + .135 - .24 + 7
	//= 6.903
	double dexpected = 6.903;

	for(int i = 0; i < 500; i++)
	{
		tgp1->ss(dexpected);
		tgp1->print_fitnesses(dexpected);
		int mini = tgp1->get_lowest_fitness_index(dexpected);
		cout << "Min fitness is " << mini << " element. "
		 <<  "Its eval value is " << tgp1->get_eval(mini) << endl;
		int mini2 = tgp1->get_second_lowest_fitness_index(dexpected);
		cout << "Second min fitness is " << mini2 << " element. "
		 <<  "Its eval value is " << tgp1->get_eval(mini2) << endl;
	}

	return(0);
}
