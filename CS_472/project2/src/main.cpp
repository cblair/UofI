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
	test_tree_replace();
	test_tree_crossover();
	test_tree_get_subtree();
	*/
	test_tree_copy();
	return(0);
	#endif

	//Main eval
	darray *dp1 = new darray(2, false);
	dp1->a[0] = .2;
	dp1->a[1] = .3;
	tree_gp *tgp1 = new tree_gp(100, 5, &dp1);
	//x^3 + 5y^3 - 4xy + 7
	//= (.2)^3 + 5(.3)^3 - 4(.2)(.3) + 7
	//= .008 + .135 - .24 + 7
	//= 6.903
	double dexpected = 6.903;

	int i = 0;
	bool bored = false;
	while(!bored)
	{
		tgp1->print_depths();
		tgp1->print_fitnesses(dexpected);
		return(1);

		//timing stuff
		clock_t stime, etime, ttime;
		int precision = 1000;
	  	stime = (clock () / CLOCKS_PER_SEC) * precision;

		//tgp1->ss(dexpected);
		tgp1->gen(dexpected);
	
		//double lowest_fitness = tgp1->get_lowest_fitness(dexpected);
		int mini = tgp1->select_lowest_fitness_index(dexpected);

	  	etime = (clock () / CLOCKS_PER_SEC) * precision;
		ttime = (etime - stime) / precision;
		cout 	<< i << ": min fit = " 
			//<< lowest_fitness
			//<< ", min eval = " << tgp1->get_eval(mini) 
			//<< ", avg fitness is " 
			//<< tgp1->get_avg_fitness(dexpected) 
			<< ", time = " << ttime
			<< endl;
		//if(lowest_fitness <= 0.001 || i > 1000)
		if(i >= 1000)
		{
			bored = true;
		}
		i++;
	}

	int mini = tgp1->select_lowest_fitness_index(dexpected);
	cout << "Lowest individual: eval = " << tgp1->get_eval(mini)
		<< ", tree:\n";
	tgp1->print_lowest_fitness_tree(dexpected);

	//cleanup
	delete tgp1;
	delete dp1;

	return(0);
}
