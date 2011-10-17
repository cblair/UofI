#include <iostream>
#include <time.h>

#include "tree_gp.h"
#include "darray.h"

using namespace std;

tree_gp::tree_gp(int size, int tree_depth, darray **dp)
{
	if(size > MAX_TREE_BUF)
	{
		this->size = 0;
		return;
	}

	this->size = size;

	for(int i = 0; i < this->size; i++)
	{
		this->a[i] = new tree(tree_depth, (*dp));
	}
}


tree_gp::~tree_gp()
{
	for(int i = 0; i < this->size; i++)
	{
		delete this->a[i];
	}
}


int tree_gp::get_lowest_fitness_index(double dexpected)
{
	int min = 0;
	for(int i = 1; i < this->size; i++)
	{
		if(this->a[i]->fitness(dexpected) < \
			this->a[min]->fitness(dexpected))
		{
			min = i;
		}
	}

	return(min);
}


int tree_gp::get_second_lowest_fitness_index(double dexpected)
{
	int min2 = 0;
	int min = this->get_lowest_fitness_index(dexpected);

	for(int i = 1; i < this->size; i++)
	{
		if(this->a[i]->fitness(dexpected) < \
			this->a[min2]->fitness(dexpected)
			&& i != min)
		{
			min2 = i;
		}
	}

	return(min2);
}


//simple crosses over the two lowest (best) fitnesses
// converges to locals, but needs tournament selection on subset of
// pop. to get global, etc.
bool tree_gp::ss(double dexpected)
{
	//Selection
	int min1 = this->get_lowest_fitness_index(dexpected);
	int min2 = this->get_second_lowest_fitness_index(dexpected);

	tree_crossover(&this->a[min1], &this->a[min2]);
	
	//Mutate
	int rand_val;
	/* initialize random seed: */
	srand ( clock() );
	/* generate secret number: */
	// 0-n values
	rand_val = rand() % this->a[min1]->count_nonterms(); 
	mutate_nth_nonterm(&this->a[min1], rand_val, 0, 5, 
				this->a[min1]->dp);
}


bool tree_gp::print_fitnesses(double dexpected)
{
	for(int i = 0; i < this->size; i++)
	{
		cout << i << ":" << this->a[i]->fitness(dexpected) << " ";
		if(i != 0 && (i % 5) == 0)
		{
			cout << endl;
		}
	}

	cout << endl;
	
	//how to fail?
	return(true);
}


double tree_gp::get_eval(int i)
{
	if(this == NULL || i >= this->size)
	{
		return(-1.0000);
	}

	return(this->a[i]->eval());
}
