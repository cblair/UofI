#include <stdio.h>
#include <stdlib.h>

#include "ga_i.h"
#include "ga_pop.h"


struct ga_pop *ga_pop_init(int size, double darray[], int nattr,
				double (* fitness)(struct ga_i*),
				double (* random)(double) )
{
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


	struct ga_pop *retval = (struct ga_pop*)malloc(sizeof(struct ga_pop));	

	//Init inidividuals
	retval->ni = size;
	int i;
	for(i = 0; i < size; i++)
	{
		retval->iarray[i] = ga_i_init(darray, nattr);
	
		//Init the random function pointer
		retval->iarray[i]->random = random;
	}


	//Init the fitness function pointer
	retval->fitness = fitness;


	return(retval);
}


void ga_pop_del(struct ga_pop **p)
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
void ga_pop_tournament_selection(int indices[2], struct ga_pop *p)
{
	/* initialize random seed: */
	//srand ( time(NULL) );

	int i;
	int j;
	//k = % of all the attributes
	int k = p->ni / 10; 
	
	//create a subset of indices for min1 and min2 selection
	int subset_indices[k];
	for(i = 0; i < k; i++)
	{
		j = rand() % p->ni;
		subset_indices[i] = j;
	}

	//Select min1
	j = subset_indices[0];
	indices[0] = j;
	double min1 = p->fitness(p->iarray[j]);
	for(i = 1; i < k; i++)
	{
		j = subset_indices[i];
		if(p->fitness(p->iarray[j]) < min1)
		{
			min1 = p->fitness(p->iarray[j]);
			indices[0] = j;
		}
	}

	//Select min2
	i = 0;
	j = subset_indices[i];
	//if min1's index is 0, then set min2's index to 1
	double min2 = p->fitness(p->iarray[j]);
	indices[1] = j;
	if(j == indices[0])
	{
		i = 1;
		j = subset_indices[i];
		min2 = p->fitness(p->iarray[j]);
		indices[1] = j;
	}
	for(; i < k; i++)
	{
		j = subset_indices[i];
		if(p->fitness(p->iarray[j]) < min2
			&& j != indices[0])
		{
			min2 = p->fitness(p->iarray[j]);
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

	double min = p->fitness(p->iarray[0]);
	int mini = 0;
	int i;
	for(i = 0; i < p->ni; i++)
	{
		if(p->fitness(p->iarray[i]) < min)
		{
			min = p->fitness(p->iarray[i]);
			mini = i;
		}
	}

	return(mini);
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
	double fit = p->fitness(p->iarray[i]);

	//printf("TS159: %f\n", fit);
	//return(fit);
	return(p->fitness(p->iarray[i]));
}


//returns minimum fitness
double ga_pop_steady_state(struct ga_pop **p)
{
	int indices[2];
	ga_pop_tournament_selection(indices, (*p));
	
	int i1 = indices[0];
	int i2 = indices[1];

	//Crossover
	ga_i_2p_crossover(&((*p)->iarray[i1]), &((*p)->iarray[i2]));

	//Mutate
	ga_i_uniform_mutation(&(*p)->iarray[i1]);
	ga_i_uniform_mutation(&(*p)->iarray[i2]);

	/*
	printf("TS168: %d %d: %f %f\n", i1, i2, (*p)->fitness((*p)->iarray[i1]),
						(*p)->fitness((*p)->iarray[i1]));
	ga_i_print_attr((*p)->iarray[i1]);
	ga_i_print_attr((*p)->iarray[i2]);
	*/

	double pop_min_fit = ga_pop_get_min_fitness((*p));

	return(pop_min_fit);
}


//returns minimum fitness
double ga_pop_generational(struct ga_pop **p)
{
	int indices[2];
	ga_pop_tournament_selection(indices, (*p));
	
	int i1 = indices[0];
	int i2 = indices[1];

	//Crossover
	ga_i_2p_crossover(&((*p)->iarray[i1]), &((*p)->iarray[i2]));

	//Mutate
	ga_i_uniform_mutation(&(*p)->iarray[i1]);
	ga_i_uniform_mutation(&(*p)->iarray[i2]);

	//Regenerate all
	int i;
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
		
		//printf("TS168: %d: %f\n", i, (*p)->fitness((*p)->iarray[i]));
		if( (*p)->fitness((*p)->iarray[i]) == 0.0)
		{
			ga_i_print_attr((*p)->iarray[i]);
			ga_i_print_attr((*p)->iarray[i1]);
			exit(1);
		}
	}
	/*
	printf("TS168: %d %d: %f %f\n", i1, i2, (*p)->fitness((*p)->iarray[i1]),
						(*p)->fitness((*p)->iarray[i1]));
	ga_i_print_attr((*p)->iarray[i1]);
	ga_i_print_attr((*p)->iarray[i2]);
	*/

	double pop_min_fit = ga_pop_get_min_fitness((*p));
	//printf("TS236 %f\n", pop_min_fit);

	//return(pop_min_fit);
	return(ga_pop_get_min_fitness((*p)));
}



void ga_pop_print_individuals(struct ga_pop *p)
{
	int i;
	for(i = 0; i < p->ni; i++)
	{
		printf("%d:\n", i);
		ga_i_print_attr(p->iarray[i]);
	}
}
