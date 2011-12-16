#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "ga_i.h"
#include "ga_pop.h"

//tournament selection, % to select for subpool for tournament selection
// % = k = (pop)size / K_SELECT
int K_SELECT = 1;

using namespace std;


struct ga_pop *ga_pop_init(int size, double *darray, 
				int nattr,
				double (* fitness)(struct ga_i*),
				double (* random)(double) )
{
	int i;
	ga_pop *retval = (ga_pop*)malloc(sizeof(ga_pop));

	//Required values
	if(fitness == NULL)
	{
		fprintf(stderr,
			"ERROR: you must register a fitness function. Exiting.\n");
		exit(1);
	}
	if(random == NULL)
	{
		fprintf(stderr,
			"ERROR: you must register a random function. Exiting.\n");
		exit(1);
	}
	if(size > GA_MAX_POP_SIZE)
	{
		fprintf(stderr,
			"ERROR: size of %d too big, max size is %d. Exiting.\n", GA_MAX_POP_SIZE);
		exit(1);
	}

	//Init inidividuals
	retval->ni = size;

	for(i = 0; i < size; i++)
	{
		retval->iarray[i] = ga_i_init(darray, nattr);

		//Init the random function pointer
		retval->iarray[i]->random = random;
	}

	//Init the fitness function pointer
	retval->fitness = fitness;

	//Init the fitness caches
	ga_pop_recalc_fitness_cache(&retval);

	return(retval);
}


void ga_pop_del(ga_pop **p)
{
	int i;
	for(i = 0; i < (*p)->ni; i++)
	{
		ga_i_del(&(*p)->iarray[i]);
	}

	free((*p));
	(*p) = NULL;
}


//TODO: cleanup
void ga_pop_tournament_selection(int indices[2], ga_pop *p)
{
	/* initialize random seed: */
	//srand ( time(NULL) );

	int i;
	int j;
	//k = % of all the attributes
	int k = p->ni / K_SELECT; 
	//int subset_indices[k];
	vector<int> subset_indices(k);

	if(k <= 1)
	{
		fprintf(stderr,
			"ERROR: Selection k = %d, too small for (pop)size %d and K_SELECT %d. K_SELECT is too big, no values will be selected. Set K_SELECTION lower. Exiting\n",
			k, p->ni, K_SELECT);
		exit(1);
	}
	
	//create a subset of indices for min1 and min2 selection
	for(i = 0; i < k; i++)
	{
		j = rand() % p->ni;
		subset_indices[i] = j;
	}

	//Select min1
	j = subset_indices[0];
	indices[0] = j;
	//double min1 = p->fitness(p->iarray[j]);
	double min1 = p->iarray[j]->fitness_cache;
	for(i = 1; i < k; i++)
	{
		j = subset_indices[i];
		//if(p->fitness(p->iarray[j]) < min1)
		if(p->iarray[j]->fitness_cache < min1)
		{
			//min1 = p->fitness(p->iarray[j]);
			min1 = p->iarray[j]->fitness_cache;
			indices[0] = j;
		}
	}

	//Select min2
	i = 0;
	j = subset_indices[i];
	//if min1's index is 0, then set min2's index to 1
	//double min2 = p->fitness(p->iarray[j]);
	double min2 = p->iarray[j]->fitness_cache;
	indices[1] = j;
	if(j == indices[0])
	{
		i = 1;
		j = subset_indices[i];
		//min2 = p->fitness(p->iarray[j]);
		min2 = p->iarray[j]->fitness_cache;
		indices[1] = j;
	}
	for(; i < k; i++)
	{
		j = subset_indices[i];
		if(p->iarray[j]->fitness_cache < min2
			&& j != indices[0])
		{
			min2 = p->iarray[j]->fitness_cache;
			indices[1] = j;
		}
	}
}


int ga_pop_get_min_fitness_index(struct ga_pop *p)
{
	if(p == NULL)
	{
		fprintf(stderr,
			"ERROR: trying to get min fit index of null pop. Exiting.\n");
		exit(1);
	}

	double min = p->iarray[0]->fitness_cache;
	int mini = 0;
	int i;
	for(i = 0; i < p->ni; i++)
	{
		if(p->iarray[i]->fitness_cache < min)
		{
			min = p->iarray[i]->fitness_cache;
			mini = i;
		}
	}

	return(mini);
}


void ga_pop_recalc_fitness_cache(struct ga_pop **p)
{
	int i;

	for(i = 0; i < (*p)->ni; i++)
	{
		(*p)->iarray[i]->fitness_cache = (*p)->fitness((*p)->iarray[i]);
	}
}


double ga_pop_get_min_fitness(struct ga_pop *p)
{
	if(p == NULL)
	{
		fprintf(stderr,
			"ERROR: trying to get min fit of null pop. Exiting.\n");
		exit(1);
	}


	int i = ga_pop_get_min_fitness_index(p);
	double fit = p->iarray[i]->fitness_cache;

	return(fit);
}


void ga_pop_get_max_fitnesses(struct ga_pop *p, int *max1, int *max2)
{
	int i;

	if(p->ni < 2)
	{
		fprintf(stderr,
			"ERROR: pop size of %d is too small. Exiting\n", p->ni);
		exit(1);
	}

	//Init indexes and values
	(*max1) = 0;
	(*max2) = 1;
	double max1_d = p->iarray[(*max1)]->fitness_cache;
	double max2_d = p->iarray[(*max2)]->fitness_cache;

	//switch if max2 greater
	if(max2_d > max1_d)
	{
		(*max1) = 1;
		(*max2) = 0;
		max1_d = p->iarray[(*max1)]->fitness_cache;
		max2_d = p->iarray[(*max2)]->fitness_cache;
	}

	//find max1
	for(i = 0; i < p->ni; i++)
	{
		if(p->iarray[i]->fitness_cache > max1_d)
		{
			(*max1) = i;
			max1_d = p->iarray[(*max1)]->fitness_cache;
		}
	}

	//find max2
	for(i = 0; i < p->ni; i++)
	{
		if(p->iarray[i]->fitness_cache > max2_d
			&& i != (*max1) )
		{
			(*max2) = i;
			max2_d = p->iarray[(*max2)]->fitness_cache;
		}
	}

	//this shouldn't happen, but if it does, lets kill it
	if((*max1) == (*max2))
	{
		fprintf(stderr, 
			"ERROR: max's are the same. Exitting\n");
		exit(1);
	}
}


//returns minimum fitness
double ga_pop_steady_state(struct ga_pop **p)
{
	int indices[2];
	ga_pop_tournament_selection(indices, (*p));
	
	int i1 = indices[0];
	int i2 = indices[1];

	#ifdef GA_ELITISM
	//preserve the best
	struct ga_i *i1p;
	struct ga_i *i2p;
	ga_i_copy(&i1p, &((*p)->iarray[i1]));
	ga_i_copy(&i2p, &((*p)->iarray[i2]));
	#endif

	//Crossover
	//ga_i_2p_crossover(&((*p)->iarray[i1]), &((*p)->iarray[i2]));

	//Mutate
	ga_i_uniform_mutation(&(*p)->iarray[i1]);
	ga_i_uniform_mutation(&(*p)->iarray[i2]);

	//Recalculate fitness
	// we're steady state, so we only need to recalce the 
	// 2 xover/mutated
	//ga_pop_recalc_fitness_cache(&(*p));
	(*p)->iarray[i1]->fitness_cache = (*p)->fitness((*p)->iarray[i1]);
	(*p)->iarray[i2]->fitness_cache = (*p)->fitness((*p)->iarray[i2]);

	//Elitism
	#ifdef GA_ELITISM
	int maxi1;
	int maxi2;
	//find max's (worst)
	ga_pop_get_max_fitnesses((*p), &maxi1, &maxi2);
	//delete them
	if(i1 != maxi1)
	{
		ga_i_del(&((*p)->iarray[maxi1]));
		ga_i_copy(&((*p)->iarray[maxi1]), &i1p);
	}
	if(i2 != maxi2)
	{
		ga_i_del(&((*p)->iarray[maxi2]));
		ga_i_copy(&((*p)->iarray[maxi2]), &i2p);
	}

	ga_i_del(&i1p);
	ga_i_del(&i2p);
	#endif

	double pop_min_fit = ga_pop_get_min_fitness((*p));

	return(pop_min_fit);
}


//returns minimum fitness
double ga_pop_generational(struct ga_pop **p)
{
	int i = 0;

	int indices[2];
	ga_pop_tournament_selection(indices, (*p));
	
	int i1 = indices[0];
	int i2 = indices[1];

	//Elitism
	#ifdef GA_ELITISM
	//preserve the best
	struct ga_i *i1p;
	struct ga_i *i2p;
	ga_i_copy(&i1p, &((*p)->iarray[i1]));
	ga_i_copy(&i2p, &((*p)->iarray[i2]));
	#endif

	//Crossover
	//ga_i_2p_crossover(&((*p)->iarray[i1]), &((*p)->iarray[i2]));
	ga_i_flip_crossover(&((*p)->iarray[i1]), &((*p)->iarray[i2]));

	//Mutate
	//ga_i_uniform_mutation(&(*p)->iarray[i1]);
	//ga_i_uniform_mutation(&(*p)->iarray[i2]);

	//Regenerate all
	for(i = 0; i < (*p)->ni; i++)
	{
		if(i == i1)
		{
			continue;
		}

		//Regenerate each
		ga_i_del(&((*p)->iarray[i]));
		(*p)->iarray[i] = NULL;
		ga_i_copy( &((*p)->iarray[i]), &((*p)->iarray[i1]));

		//Mutate
		ga_i_uniform_mutation(&(*p)->iarray[i]);	
	}

	ga_pop_recalc_fitness_cache(&(*p));

	//Elitism
	#ifdef GA_ELITISM
	int maxi1;
	int maxi2;
	//find max's (worst)
	ga_pop_get_max_fitnesses((*p), &maxi1, &maxi2);
	//delete them
	if(i1 != maxi1)
	{
		ga_i_del(&((*p)->iarray[maxi1]));
		ga_i_copy(&((*p)->iarray[maxi1]), &i1p);
	}
	if(i2 != maxi2)
	{
		ga_i_del(&((*p)->iarray[maxi2]));
		ga_i_copy(&((*p)->iarray[maxi2]), &i2p);
	}

	ga_i_del(&i1p);
	ga_i_del(&i2p);
	#endif

	double pop_min_fit = ga_pop_get_min_fitness((*p));

	return(pop_min_fit);
}



void ga_pop_print_individuals(struct ga_pop *p)
{
	int i;
	for(i = 0; i < p->ni; i++)
	{
		printf("%d: %f\n", i, p->iarray[i]->fitness_cache);
		//Yikes, TMI
		//ga_i_print_attr(p->iarray[i]);
	}
}