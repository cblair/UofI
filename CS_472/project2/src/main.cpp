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
	/*test_nodes();
	test_darray();
	test_trees();
	test_tree_copy();
	test_tree_replace();
	test_tree_crossover();
	*/
	test_tree_get_subtree();
	return(0);
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

	int i = 0;
	bool bored = false;
	while(!bored)
	{
		//tgp1->ss(dexpected);
		tgp1->gen(dexpected);
		double lowest_fitness = tgp1->get_lowest_fitness(dexpected);
		int mini = tgp1->get_lowest_fitness_index(dexpected);
		cout 	<< i << ": min fit = " 
			<< lowest_fitness
			<< ", min eval = " << tgp1->get_eval(mini) 
			<< ", avg fitness is " 
			<< tgp1->get_avg_fitness(dexpected) << endl;

		#ifdef DEBUG
		tgp1->print_fitnesses(dexpected);
		cout << "Min fitness is " << mini << " element. "
		 <<  "Its eval value is " << tgp1->get_eval(mini) << endl;
		int mini2 = tgp1->get_second_lowest_fitness_index(dexpected);
		cout << "Second min fitness is " << mini2 << " element. "
		 <<  "Its eval value is " << tgp1->get_eval(mini2) << endl;
		#endif

		if(lowest_fitness <= 0.001 || i > 100)
		{
			bored = true;
		}
		i++;
	}

	cout << "Lowest fitness tree:\n";
	tgp1->print_lowest_fitness_tree(dexpected);

	//cleanup
	delete tgp1;
	delete dp1;

	return(0);
}
