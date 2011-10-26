#ifndef _TREE_GP_H
#define _TREE_GP_H

#include "tree.h"
#include "darray.h"

#define MAX_TREE_BUF 500

#ifdef DEBUG_TREE_GP
#define DEBUG_TREE_GP_MSG(arg) (cout << arg << endl)
#else
#define DEBUG_TREE_GP_MSG(arg) ;
#endif

//collects other tree classes and instances for gp ops
class tree_gp
{
private:
	int size;
	tree *a[MAX_TREE_BUF]; //an array of tree pointers

public:
	int k; //percentage of selected indiv for tournament selection
	
	tree_gp(int, int, darray**);
	~tree_gp();

	//Selection
	int select_lowest_fitness_index(double);
	int select_second_lowest_fitness_index(double);
	void select_lowest_tournament_fitness_indices(double, int*, int*);

	double get_lowest_fitness(double);
	double get_avg_fitness(double);

	double get_eval(int); //return value from ind. eval

	//gp modes
	bool ss(double); //steady state
	bool gen(double); //generational

	bool print_fitnesses(double);
	bool print_depths();
	bool print_lowest_fitness_tree(double);
};

#endif
