#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <limits>

using namespace std;

//taken from SA stuff to run simulation
#include "SA.h"
//#include "Look.h"
#include "SAstructs.h"

#include "structs.h"
#include "system_params.h"

extern "C" int submain(double **,double **,double **,double**);
extern "C" int submain_ga(double **,double **,double **,double**);

extern "C" OptStruct mdata; //new global data structure

//end SA stuff

//place holders for the muscle data structures, for use in fitness 
// function
double **XR_GLOBAL;
double **XL_GLOBAL;
double **YR_GLOBAL;
double **YL_GLOBAL;
int N_MUS_GLOBAL;
int N_EMG_STEP_GLOBAL;
int N_PARAMS_GLOBAL;

//ga stuff
#include "ga_i.h"
#include "ga_pop.h"

double GA_UBOUND;
double GA_LBOUND;
double GA_DIFF_SCALE;

//mutation selection, % to mutate = k = nattr / K_MUT
extern int K_MUT;

//how many points flip crossover will do
extern int K_FLIP_XOVER;

//tournament selection, % to select for subpool for tournament selection
// % = k = (pop)size / K_SELECT
extern int K_SELECT;


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


double muscle_fitness(struct ga_i *p)
{
	int i = 0;
	int j = 0;
	double *x = p->attr;

	//update / init the data
	update_group_excitation_patterns(XR_GLOBAL,YR_GLOBAL,XL_GLOBAL,
									YL_GLOBAL,x);
	change_init(x,N_PARAMS_GLOBAL); //initial velocities

	double **XRa, **XLa ;//abscissa adjusted
	XRa=new double *[N_MUS_GLOBAL];
	XLa=new double *[N_MUS_GLOBAL];
	for(i=0;i<N_mus;i++){ 
		XRa[i]=new double[N_emg_step]; if(XRa[i]==NULL) exit(0);
		XLa[i]=new double[N_emg_step]; if(XLa[i]==NULL) exit(0);
	}

	for(j=0;j<N_MUS_GLOBAL;j++)
	{
		for(i=0;i<N_EMG_STEP_GLOBAL;i++)
		{
			XRa[j][i]=XR_GLOBAL[j][i]*MAX_X/100.0;
			XLa[j][i]=XL_GLOBAL[j][i]*MAX_X/100.0;
		}
	}//adjust abscissa

	//run the sim each iteration
	int BAD = submain_ga(XRa,YR_GLOBAL,XLa,YL_GLOBAL);
	//save_motion(); //write motion file even walking was not completed

	double retval = get_cost();
	
	//if it didn't walk, put highest cost possible
	if(BAD == 1)
	{
			printf(" Didn't walk.\n");
			retval = numeric_limits<float>::infinity( );
	}

	printf(" Cost/fitness: %f\n", retval);
	return(retval);
}


double simple_random(double current_val)
{
	int precision = 2; //the most decimal values allowed
	int scale = pow((double)10, precision);

	//double lbound = -5.12;
	//double ubound = 5.12;
	double lbound = GA_LBOUND; // = -5.0;
	double ubound = GA_UBOUND; //100.0;

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
	double diff_scale = GA_DIFF_SCALE;
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

int ga_test()
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

		i++;

		if(min_fit == 0.0 || i > 100000)
		{
			bored = 1;
		}
	}

	return(0);

}


void ga_main(int N_params, double *x, double **XR, double **YR, double **XL, 
				double **YL)
{
	int i;
	int j;
	int ngen = 0;
	int pop_size = 10;
	const int nattr = 96;
	int bored = 0;
	double min_fit;

	// ga setting setup
	GA_LBOUND = -5.000;
	GA_UBOUND = 100.00;
	GA_DIFF_SCALE = 800.000;
	K_MUT = 96;
	K_SELECT = 1; // select best from all population
	K_FLIP_XOVER = 3;

	//set the global muscle data structure so they can be accessed
	// by the fitness function
	XR_GLOBAL = XR;
	XL_GLOBAL = XL;
	YR_GLOBAL = YR;
	YL_GLOBAL = YL;
	N_MUS_GLOBAL = N_mus;
	N_EMG_STEP_GLOBAL = N_emg_step;
	N_PARAMS_GLOBAL = N_params;

	/*
	GA_LBOUND = -5.12;
	GA_UBOUND = 5.12;
	K_MUT = 5;
	GA_DIFF_SCALE = 10.000;
	ga_test();
	return;
	*/

	printf("GA: %d individuals, %d attributes\n", pop_size, nattr);

	//x is what we will optimize. copy in nattr of x, to be sure of size
	double d1[nattr];
	for(i = 0; i < nattr; i++)
	{
		d1[i] = x[i];
	}

	FILE *fp;
	char *fname = "ga_r_data.dat";
	fp = fopen(fname, "w");
	fprintf(fp, "min_fit\n");
	fclose(fp);

	//init ga pop
	// pop size = N_mus, nattr = N_emg_step
	struct ga_pop *p = ga_pop_init(pop_size, d1, nattr, muscle_fitness, 
					simple_random);

	ngen = 0;
	while(bored == 0)
	{
		//do our ga
		min_fit = ga_pop_generational(&p);
		//min_fit = ga_pop_steady_state(&p);
		int mini = ga_pop_get_min_fitness_index(p);
		printf("Generation: %d, min fitness [%d]: %f\n", 
						ngen, mini, min_fit);
		//ga_i_print_attr(p->iarray[mini]);
		ga_pop_print_individuals(p);

		//set x to the best / min fitness (min fitness is min cost)
		x = p->iarray[mini]->attr;
	
		//write out the min to an R data file
		fp = fopen(fname, "a");
		fprintf(fp, "[%d] %f\n", ngen + 1, min_fit);
		fclose(fp);

		ngen++;

		if(min_fit <= 0.0 || ngen > 100000)
		{
			bored = 1;
		}
	}
}
