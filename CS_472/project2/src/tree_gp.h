#ifndef _TREE_GP_H
#define _TREE_GP_H

#include "tree.h"
#include "darray.h"

#define MAX_TREE_BUF 500

//collects other tree classes and instances for gp ops
class tree_gp
{
private:
	int size;
	tree *a[MAX_TREE_BUF]; //an array of tree pointers

public:
	tree_gp(int, int, darray**);
	~tree_gp();

	int get_lowest_fitness_index(double);
	int get_second_lowest_fitness_index(double);
	
	double get_lowest_fitness(double);
	double get_avg_fitness(double);

	double get_eval(int); //return value from ind. eval

	//gp modes
	bool ss(double); //steady state
	bool gen(double); //generational

	bool print_fitnesses(double);
	bool print_lowest_fitness_tree(double);
};

#endif
