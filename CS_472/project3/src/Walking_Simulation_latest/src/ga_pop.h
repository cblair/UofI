#ifndef _GA_POP_H
#define _GA_POP_H

#include <stdlib.h>

#include "ga_i.h"

#define GA_MAX_POP_SIZE 10000

//Elitism - comment out if not wanted
#define GA_ELITISM


struct ga_pop
{
	struct ga_i *iarray[GA_MAX_POP_SIZE]; //individual array
	int ni; //n individuals
	double (*fitness)(struct ga_i*); //the fitness function
};


struct ga_pop *ga_pop_init(int, double *, int, 
				double (* fitness)(struct ga_i*), 
				double (* random)(double));
void ga_pop_del(struct ga_pop**);

int ga_pop_get_min_fitness_index(struct ga_pop *);
void ga_pop_get_max_fitnesses(struct ga_pop *, int*, int*);
void ga_pop_recalc_fitness_cache(struct ga_pop **);

double ga_pop_steady_state(struct ga_pop **);
double ga_pop_generational(struct ga_pop **);

void ga_pop_print_individuals(struct ga_pop*);
#endif
