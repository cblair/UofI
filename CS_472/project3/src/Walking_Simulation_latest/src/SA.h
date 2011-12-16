#include <time.h>
#include <math.h>
#include "ExcGroups.h" //this defines the number of muscles and muscle groups

#define SUBJECT_WEIGHT 80.0
#define MODEL_WEIGHT 75.1282
#define LEG_TY_DIFF 0.9691-0.93405 //0.9691 and 0.93405 is ASIS height of the subject-average (S4-S10) and model, respectively
//#define EMG 0 //emg excitation pattern Unnecessary
//#define BLOCK 1 //block pattern Unnecessary
//#define N_musG 15//9 (moved to ExcGroups.h)
//#define N_mus 25 //per leg (moved to ExcGroups.h)
//#define N_pPmus 3 //3 parameters of excitation func per muscle group
//#define N_params_emg 66// 48 //30 //33 (RF has two bursts) //replaced with NUM_EXC_PARAMS in ExcGroups.h
//#define N_params_block 70 //33+4
#define N_params_init 13//9 //initial state conditions
#define NS 20 //# of steps in inner loop
#define NT 5 //# of inner loops taken before temp. reduction

#define N_step 101 //number of steps determined in SIMM

//#define ENDCYCLE 100 
//#define STARTCYCLE 0

//#define N_emg_step 101 moved to ExcGroups.h
#define N_cost_step 165//150
#define N_track 26 //# of tracking quantity
//#define N_kin 19//number of kinematic variables
#define N_kin 20

#define MAX_X 1.348347  //max abscissa of one cycle (100%)
	  //MAT_Xs W40: 1.068392
			 //W60: 1.339458
			 //W80: 1.590523
		     //W100: 1.773614
			 //W120: 1.861432
			 //R40: 0.670697
			 //R60: 0.989235
			 //R80: 1.269862
			 //R100: 1.568555
			 //R120: 1.823407
#define MAX_TIME 1.03719 //time of one cycle (MAX_X divided by the speed in m/s)
      //Speeds: 40%: 0.8222
			  //60%: 1.2333
			  //80%: 1.6444
			  //100%: 2.0556
              //120%: 2.4667
				
#define NintegStep 825//750
#define N_track_step 165//150

#define PI 3.141592654
#define NJNT 15 
#define NQ 25   //see comments at sdderiv in model_d.c or sdfor.c
#define NU 25   //see comments at sdderiv in model_d.c or sdfor.c
#define NC 25   //see comments at sdderiv in model_d.c or sdfor.c
#define NBOD 16 //see comments at sdderiv in model_d.c or sdfor.c


double drand48(void);
double get_cost(void);
//void set_bound(double *, double *);
//void set_opt_param_bounds(double *, double *);

void write_datafile(double (*)[N_emg_step], double (*)[N_emg_step], char *, double []);
//void read_cut_emg(void);
//void get_cut_values(void);
//void make_cut_block(double *,
//				  double (*)[N_emg_step], double (*)[N_emg_step],
//				  double (*)[N_emg_step], double (*)[N_emg_step],
//				  double [N_musG+1], double [N_musG+1]);
void read_kin(void);
void save_best(double *,int,double,double,double *,int);
void save_current(double *,int,double,double,double *,int);
void save_motion(void);
void init_structure(void);

//void set_patterns(void);
//void set_pattern_types(void);
//void set_pattern_sources(void);
//void get_cut_values(void);
//void update_EMG_Exc_templates(void);
//void init_Exc_templates(void);
//void assign_muscle_excitations(double **group_exc_xR,double **group_exc_yR, double **group_exc_xL,double **group_exc_yL,
//							   double **XR,double **YR,double **XL,double **YL);	//assigns the individual muscle excitation patterns based on its muscle group
//void mod_emg(double **,double **, double **,double **,
//			 double *);
//void mod_block(
//			 double *, 
//			 double (*)[N_emg_step],double (*)[N_emg_step],
//			 double (*)[N_emg_step],double (*)[N_emg_step],
//			 double *, double [N_musG+1], double [N_musG+1]);

//void update_group_excitation_patterns(double **XR,double **YR,double **XL,double **YL, double *x);

void comp_left(double **,double **, double **,double **,
			   int);

void change_init(double *, int);

//void curveshift(double [N_emg_step], double [N_emg_step],
//				double **,double **,
//				double, int);

void init_simm(void);
void first_run(int,
			   double **, double **,
			   double **, double **,
			   double *, int, double *, double *, double *, double *,
			   int *, int *, int *, int *, double *);

void s_annealing(int, int, double, double,double *, 
			   double **, double **, double **,double **,
			   int, int, int, double *, double *, double *);

/**********************************************************************************************/
//These functions are used to handle the creation and modification of individual 
//muscle excitation patterns -These functions are a 'user friendly' update of Ko's original code
//Created/Modified by Jeff Rankin - Oct/Nov 2008

//This function creates the original excitation templates for each excitation group
void init_Exc_templates(void);  
//this updates the group excitation patterns and then assigns individual muscle excitations to the appropriate group
void update_group_excitation_patterns(double **XR,double **YR,double **XL,double **YL, double *xp); 
//this sets the parameter bounds for the optimization parameters
void set_opt_param_bounds(double *LB, double *UB);
//this updates the pattern for the bimodal excitation control
void update_BiModal_Pattern(double **group_XR, double**group_YR,int m,int flag_left);
/**********************************************************************************************/

