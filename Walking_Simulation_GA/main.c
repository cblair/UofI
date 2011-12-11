#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "ga_i.h"
#include "ga_pop.h"


double simple_fitness(struct ga_i *p)
{
	double sum = 0.0;
	int i;
	for(i = 0; i < p->nattr; i++)
	{
		sum += pow(p->attr[i], 2);
	}

	if(sum < 0.0)
	{
		fprintf(stderr,
			"ERROR: negative fitness value. Exiting.\n");
		ga_i_print_attr(p);
		for(i = 0; i < p->nattr; i++)
		{
			printf(" %f\n", p->attr[i]);
		}
		exit(1);
	}
	
	return(sum);
}


double simple_random(double current_val)
{
	int precision = 2; //the most decimal values allowed
	int scale = pow(10, precision);

	double lbound = -5.12;
	double ubound = 5.12;

	//get a random value in the range using ints
	int range = (ubound * scale) - (lbound * scale);
	int irand = rand() % range;
	
	//the new random value is a random value in the range, scaled 
	// down to the original precision
	double drand = ubound - ( ((double)irand) / scale );

	//return(drand);

	//get the difference between the rand value and the current value
	double diff = current_val - drand; //can be neg or pos

	//scale down the difference
	//double diff_scale = 10.000;
	double diff_scale = 40.000;
	diff = diff / diff_scale;

	//return the scaled down change of current value
	double retval = current_val - diff;
	
	//gaurantee max precision
	retval = retval * (10 * precision);
	retval = ceil(retval);
	retval = retval / (10 * precision);

	//if we go beyond the bounds, limit
	if(retval < lbound)
	{
		retval = lbound + (lbound / scale);
	}
	else if (retval > ubound)
	{
		retval = ubound - (ubound / scale);
	}

	return(retval);
}


int main()
{
	int i;

	/* initialize random seed: */
	srand ( time(NULL) );

	double d1[] = { \
			-5.0000, -4.0000, -3.0000, -2.0000, -1.0000, \
			0.00000, 1.0000, 2.0000, 3.0000, 4.0000, \
			-5.0000, -4.0000, -3.0000, -2.0000, -1.0000, \
			0.00000, 1.0000, 2.0000, 3.0000, 4.0000, \
			-5.0000, -4.0000, -3.0000, -2.0000, -1.0000, \
			0.00000, 1.0000, 2.0000, 3.0000, 4.0000
			};
	struct ga_pop *p = ga_pop_init(500, d1, 30, simple_fitness, 
					simple_random);


	//Crossover tests
	/*
	p->iarray[0]->attr[0] = 6.0000;
	p->iarray[0]->attr[1] = 7.0000;
	p->iarray[0]->attr[2] = 8.0000;
	p->iarray[0]->attr[3] = 9.0000;
	p->iarray[0]->attr[4] = 10.0000;

	printf("TS66\n");
	ga_i_print_attr(p->iarray[0]);
	ga_i_print_attr(p->iarray[1]);
	ga_i_2p_crossover(&(p->iarray[0]), &(p->iarray[1]));
	ga_i_print_attr(p->iarray[0]);
	ga_i_print_attr(p->iarray[1]);
	*/

	//Random tests
	/*
	p->iarray[0]->attr[0] = -5.12;
	for(i = 0; i < 100; i++)
	{
		p->iarray[0]->attr[0] = \
			p->iarray[0]->random(p->iarray[0]->attr[0]);
		printf("TS65: %f\n", p->iarray[0]->attr[0]);

		//
		int j;
		for(j = 0; j < p->iarray[i]->nattr; j++)
		{
			printf("TS65: %f\n", 
			 p->iarray[i]->random(p->iarray[i]->attr[j]));
		}
		//
	}
	exit(1);
	*/

	//Mutation test
	/*
	ga_i_print_attr(p->iarray[0]);
	ga_i_uniform_mutation(&(p->iarray[0]));
	ga_i_print_attr(p->iarray[0]);
	*/

	//Fitness function
	/*
	p->iarray[0]->attr[0] = 6.0000;
	p->iarray[0]->attr[1] = 7.0000;
	p->iarray[0]->attr[2] = 8.0000;
	p->iarray[0]->attr[3] = 9.0000;
	p->iarray[0]->attr[4] = 10.0000;
	//printf("TS93: %f\n", p->fitness(p->iarray[0]));

	p->iarray[1]->attr[0] = 1.0000;
	p->iarray[1]->attr[1] = 0.0000;
	p->iarray[1]->attr[2] = 0.0000;
	p->iarray[1]->attr[3] = 1.0000;
	p->iarray[1]->attr[4] = 0.0000;
	//printf("TS100: %f\n", p->fitness(p->iarray[1]));

	for(i = 2; i < 50; i++)
	{
		p->iarray[i]->attr[0] = 0.0000;
		p->iarray[i]->attr[1] = 0.0000;
		p->iarray[i]->attr[2] = 2.0000 + i;
		p->iarray[i]->attr[3] = 0.0000;
		p->iarray[i]->attr[4] = 0.0000;
		//printf("TS107: %f\n", p->fitness(p->iarray[2]));
	}

	for(i = 50; i < 100; i++)
	{
		p->iarray[i]->attr[0] = 0.0000;
		p->iarray[i]->attr[1] = 0.0000;
		p->iarray[i]->attr[2] = 1.0000 + i - 50;
		p->iarray[i]->attr[3] = 0.0000;
		p->iarray[i]->attr[4] = 0.0000;
		//printf("TS107: %f\n", p->fitness(p->iarray[2]));
	}
	*/

	//Steady state  / Generational test
	FILE *fp;
	char *fname = "graph_in.dat";
	fp = fopen(fname, "w");
	fprintf(fp, "min_fit\n");

	i = 0;
	int bored = 0;
	double min_fit;
	while(!bored)
	{
		min_fit = ga_pop_generational(&p);
		//min_fit = ga_pop_steady_state(&p);
		int mini = ga_pop_get_min_fitness_index(p);
		printf("Generation: %d, min fitness [%d]: %f\n", 
						i, mini, min_fit);
		ga_i_print_attr(p->iarray[mini]);
	
		//write out the min to an R data file
		fprintf(fp, "[%d] %f\n", i + 1, min_fit);

		i++;

		if(min_fit == 0.0 || i > 100000)
		{
			bored = 1;
		}
	}
	fclose(fp);

	return(0);
}
