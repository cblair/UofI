#include <iostream>
#include <stdlib.h>
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
		this->a[i] = new tree(tree_depth, &(*dp));
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


double tree_gp::get_lowest_fitness(double dexpected)
{
	if(this == NULL)
	{
		return(-1.0);
	}

	return(this->a[this->get_lowest_fitness_index(dexpected)]->fitness(dexpected));
}


double tree_gp::get_avg_fitness(double dexpected)
{
	double sum = 0.0;
	for(int i = 1; i < this->size; i++)
	{
		sum += this->a[i]->fitness(dexpected);
	}

	return(sum / this->size);
}


//simple crosses over the two lowest (best) fitnesses
// converges to locals, but needs tournament selection on subset of
// pop. to get global, etc.
bool tree_gp::ss(double dexpected)
{
	//Selection
	int min1 = this->get_lowest_fitness_index(dexpected);
	int min2 = this->get_second_lowest_fitness_index(dexpected);

	//Crossover
	tree_crossover(&(this->a[min1]), &(this->a[min2]));

	//Mutate
	int rand_val;
	/* initialize random seed: */
	srand ( clock() );
	/* generate secret number: */
	// 0-n values
	rand_val = rand() % this->a[min1]->count_nonterms(); 
	mutate_nth_nonterm(&this->a[min1], rand_val, 0, 5, 
				&(this->a[min1]->dp));
}


//generational. like ss, needs some sort subpopulation tournament selection
bool tree_gp::gen(double dexpected)
{
	//Selection
	int min1 = this->get_lowest_fitness_index(dexpected);
	int min2 = this->get_second_lowest_fitness_index(dexpected);

	//Crossover
	tree_crossover(&(this->a[min1]), &(this->a[min2]));

	for(int i =0; i < this->size; i++)	
	{
		//Mutate
		int rand_val;
		/* initialize random seed: */
		srand ( clock() );
		/* generate secret number: */
		// 0-n values
		rand_val = rand() % this->a[min1]->count_nonterms(); 

		tree *mutant_child = NULL;
		this->a[min1]->copy(&mutant_child);
		mutate_nth_nonterm(&mutant_child, rand_val, 0, 5, 
				&(mutant_child->dp));

		//simple replacement that works ok
		/*
		delete this->a[i];
		this->a[i] = new tree(5, &(mutant_child->dp));
		delete mutant_child;
		*/
		
		//replace individual of the population
		delete this->a[i]; 
		this->a[i] = NULL;
		mutant_child->copy(&(this->a[i]));
		delete mutant_child;
	}

	return(true);
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


bool tree_gp::print_lowest_fitness_tree(double dexpected)
{
	if(this == NULL)
	{
		return(false);
	}

	int min = this->get_lowest_fitness_index(dexpected);
	this->a[min]->print(0);
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
