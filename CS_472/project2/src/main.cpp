#include <iostream>
#include "darray.h"
#include "tree_gp.h"

#ifdef DEBUG
#include "test.h"
#endif

using namespace std;


//x^3 + 5y^3 - 4xy + 7
double test_func(double x, double y)
{
	return( (x * x * x) + ( 5 * y * y *y) - (4 * x * y) + 7 );
}


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

	//Setup data
	darray *dp1 = new darray(2, false);
	dp1->a[0] = .2;
	dp1->a[1] = .3;

	//TODO: not finished with creating multiple eval points
	int dpa_size = 26;
	int lbound = -25.0; //lower bound
	int ubound = 25.0; //upper bound
	double step_size = ( (ubound - lbound) / dpa_size );
	darray *dpa[dpa_size];
	int i = 0;
	for(double x = lbound; x < ubound; x +=  )
	{
		for(double y = -25.0; y < 25.0; y += 5.0)
		{
			double z = test_func(x, y);
			cout << x << "," << y << "," << z << endl;
		}
		i++;
	}


	tree_gp *tgp1 = new tree_gp(100, 5, &dp1);
	//x^3 + 5y^3 - 4xy + 7
	//= (.2)^3 + 5(.3)^3 - 4(.2)(.3) + 7
	//= .008 + .135 - .24 + 7
	//= 6.903
	double dexpected = 6.903;

	//Main eval
	int i = 0;
	bool bored = false;
	while(!bored)
	{
		//timing stuff
		clock_t stime, etime, ttime;
		int precision = 1000;
	  	stime = (clock () / CLOCKS_PER_SEC) * precision;

		//tgp1->ss(dexpected);
		tgp1->gen(dexpected);
	
		double lowest_fitness = tgp1->get_lowest_fitness(dexpected);
		//int mini = tgp1->select_lowest_fitness_index(dexpected);

	  	etime = (clock () / CLOCKS_PER_SEC) * precision;
		ttime = (etime - stime) / precision;
		cout 	<< i << ": min fit = " 
			<< lowest_fitness
			//<< ", min eval = " << tgp1->get_eval(mini) 
			//<< ", avg fitness is " 
			//<< tgp1->get_avg_fitness(dexpected) 
			<< ", time = " << ttime
			<< endl;

		if(lowest_fitness <= 0.0001 || i > 200)
		{
			bored = true;
		}
		i++;
	}

	int mini = tgp1->select_lowest_fitness_index(dexpected);
	cout << "Lowest individual: eval = " << tgp1->get_eval(mini)
		<< ", tree:\n";
	tgp1->print_lowest_fitness_tree(dexpected);
	
	//how did we do?
	cout << "original func:\n";
	cout << "x, y, z\n";
	for(double x = -25.; x < 25.0; x += 5.0)
	{
		for(double y = -25.0; y < 25.0; y += 5.0)
		{
			//x^3 + 5y^3 - 4xy + 7
			double z = (x * x * x) + ( 5 * y * y *y) - (4 * x * y) + 7;
			cout << x << "," << y << "," << z << endl;
		}
	}

	cout << "our func:\n";
	cout << "x, y, z\n";
	for(double x = -25.; x < 25.0; x += 5.0)
	{
		for(double y = -25.0; y < 25.0; y += 5.0)
		{
			dp1->a[0] = x;
			dp1->a[1] = y;
			cout << x << "," << y << "," << tgp1->get_eval(mini)
				<< endl;
		}
	}

	//cleanup
	delete tgp1;
	delete dp1;

	return(0);
}
