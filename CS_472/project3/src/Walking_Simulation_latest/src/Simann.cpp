#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SA.h"
//#include "Look.h"
#include "SAstructs.h"

#include "structs.h"
#include "system_params.h"

#include "ga_main.h"


extern "C" int submain(double **,double **,double **,double**);
extern "C" int submain_ga(double **,double **,double **,double**);

extern "C" OptStruct mdata; //new global data structure

extern "C" SDModelStruct sdm;

///////the following functions are transferred from formain.c/////////////
extern "C" char buffer[]; 
extern "C" void   init_qs(void);
extern "C" void   init_segments(void);
extern "C" void   init_joint_functions(void);
extern "C" void   init_q_restraint_functions(void);
extern "C" void set_prescribed_motion(SDModelStruct*, QType, int);
extern "C" ReturnCode read_parameters_file(char [], SDModelStruct*,
				char**, char**,char**, char**);
extern "C" ReturnCode mstrcpy(char*[], char[]);
extern "C" ReturnCode read_muscle_file(SDModelStruct*, char[]);
extern "C" void*  simm_malloc(unsigned int);
/////////////////////////////////////////////////////////////////
static void init_motion(void); //the function is from formain.c
							  

/********** Experimental data files ***********/
//char* emgfilename = "W60emgave.txt"; //placed in ExcGroups.h 
char* kineticfilename = "Control_Mean_Data_All.txt";
char* initvelfilename = "W60initvelave.txt";
char* initstatefilename = "W60initstateave.txt";
/**********************************************/

//extern "C" int PATTERN = EMG; //using block pattern:BLOCK, emg pattern:EMG No longer necessary

 

void main()
{
	/*************** Parameters to be changed **********************/
	int START = 1; //1: start using previous x's
				   //0: resume previous SA procedures
				   //-1:start from scratch
	double T = 30.0; //SA starting temperature
	/***************************************************************/

	int i,j;
	int N_params; //number of parameters to be optimized
	int I1=0, J1=0, K1=0;
	int NEVALS = 0; //number of evaluation in SA loop

	double temperature = T;
	
	double f; //cost

	double *x; //parameters to be changed
	double *VM;//range of change in x
	double *LB;//lower boundary
	double *UB;//upper boundary

	double **XR,**YR,**XL,**YL;

	XR=new double*[N_mus]; if(XR==NULL) exit(0);
	YR=new double*[N_mus]; if(YR==NULL) exit(0);
	XL=new double*[N_mus]; if(XL==NULL) exit(0);
	YL=new double*[N_mus]; if(YL==NULL) exit(0);
	for(i=0;i<N_mus;i++){
		XR[i]=new double[N_emg_step]; if(XR[i]==NULL) exit(0);
		YR[i]=new double[N_emg_step]; if(YR[i]==NULL) exit(0);
		XL[i]=new double[N_emg_step]; if(XL[i]==NULL) exit(0);
		YL[i]=new double[N_emg_step]; if(YL[i]==NULL) exit(0);
	}

	//the number of parameters is different in EMG and in BLOCK
	//if(PATTERN == EMG) {N_params = N_params_emg + N_params_init;}
	//if(PATTERN == BLOCK) {N_params = N_params_block + N_params_init;}
	N_params = N_EXC_PARAMS + N_params_init;
	////////////////////////////////////////////////////////

	x=new double[N_params]; if(x==NULL) exit(0);
	VM=new double[N_params]; if(VM==NULL) exit(0);
	LB=new double[N_params]; if(LB==NULL) exit(0);
	UB=new double[N_params]; if(UB==NULL) exit(0);

	init_structure();//initialize structures
	
	
//	set_pattern_sources();
//	set_pattern_types();
//	get_cut_values();
	init_Exc_templates();
//	assign_muscle_excitations(group_exc_xR,group_exc_yR,group_exc_xL,group_exc_yL,XR,YR,XL,YL);
    
	//set_patterns(); //determine initial onset/offset timing
	
//printf("Here\n");	
	//read_cut_emg(); //set initial excitation patterns 
	//update_EMG_Exc_templates();

	read_kin(); //read experimental kinematic data used for tracking

	//set_bound(LB,UB);//define LB,UB for to-be-optimized parameters
	set_opt_param_bounds(LB,UB);
	
	init_simm(); //simm initialization, read shoe-elements, read the best initial velocities

	/************************************************************************************/
	//Run a simulaiton in order to get (set) cost function and best parameters so far.
	//Ideally, this run ends successfully (not collapsing). But even if not,
	//simulated annealing loop may find parameters that make the model walk automatically
	//if the initial "guess" of the parameters is close. 
	//If START=0, this function will read the previous SA data and not run a simulation	
	first_run(START,XR,YR,XL,YL,x,N_params,VM,UB,LB,
				&f,&I1,&J1,&K1,&NEVALS,&temperature);
	/************************************************************************************/


	if(temperature!=T) T=temperature; //this is for the case of START=0

	
	////////save the best data so far///////////
	for(j=0;j<N_mus;j++){
		for(i=0;i<N_emg_step;i++){
			mdata.XRp[j][i]=XR[j][i];
			mdata.YRp[j][i]=YR[j][i];
			mdata.XLp[j][i]=XL[j][i];
			mdata.YLp[j][i]=YL[j][i];
		}
	}
	save_best(x,NEVALS,f,T,VM,N_params);
	////////////////////////////////////////////

	/************** Simulated annealing loop *******************/
	#ifdef USE_GA
	ga_main(N_params, x, XR, YR, XL,YL);
	#else
	//file for r graphing data
	FILE *fp;
	char *fname = "sa_r_data.dat";
	fp = fopen(fname, "w");
	fprintf(fp, "min_fit\n");
	fclose(fp);
	s_annealing(N_params,NEVALS, T, f, x, XR, YR, XL,YL,
			  I1, J1, K1, VM, LB, UB);
	#endif
	/***********************************************************/

	
	delete [] XR;
	delete [] YR;
	delete [] XL;
	delete [] YL;
	delete [] x;
	delete [] VM;
	delete [] LB;
	delete [] UB;

}//end of main
/////////////////////////////////////////////////////////////////////////
/***********************************************************************/
/////////////////////////////////////////////////////////////////////////


//////////////////////////////////////
double drand48(void)
{
	return (double)rand()/RAND_MAX;
}
//////////////////////////////////////

///////////////////transferred from formain.c///////////////////////
/* INIT_MOTION: this function turns off prescribed motion for all
 * unconstrained q's. It does not set the constrained q's because
 * in the SD/FAST system description file they were set with
 * "prescribed = 0".  It does not need to turn on prescribed motion
 * for [tree] weld joints, which are implemented as prescribed pins,
 * because they are set with "prescribed = 1" in the system description
 * file.
 */

static void init_motion(void)
{

   set_prescribed_motion(&sdm,unconstrained_q,0);

}
/////////////////////////////////////////////////////////////////////


///////////////////////////////////////////
//initialize simm
void init_simm(void)
{
	int j;
	char* muscle_file = NULL;
	char* kinetics_file = NULL;
	char* output_motion_file = "forward.mot";
	char* output_kinetics_file = "inverse.ktx";
	FILE *fp_shoe, *fp_state; 
	char dummy[50];
	char dumm;

	int shoe_array_size=132;
	sdm.shoe_element = (ShoeStruct*)simm_malloc(shoe_array_size*sizeof(ShoeStruct));


	/******************* Read in Shoe Data **********************
	************************************************************/

   fp_shoe = fopen("3DshoeORIGINAL_peripheral_3segfoot_COM.grf","r");   
  
   fscanf(fp_shoe,"%d",&sdm.num_shoe_elements);  

      for (j=0; j<sdm.num_shoe_elements; j++)
         {
         fscanf(fp_shoe,"%d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
			&sdm.shoe_element[j].segment,
			&sdm.shoe_element[j].coord[0],
			&sdm.shoe_element[j].coord[1],
			&sdm.shoe_element[j].coord[2],
			&sdm.shoe_element[j].param[0],
			&sdm.shoe_element[j].param[1],
			&sdm.shoe_element[j].param[2],
			&sdm.shoe_element[j].param[3],
			&sdm.shoe_element[j].param[4],
			&sdm.shoe_element[j].param[5],
			&sdm.shoe_element[j].param[6]);
         }  
    
   fclose(fp_shoe);

   /************read best initial velocities*******************/
   //This is for initializing.
   if((fp_state=fopen(initvelfilename,"r"))==NULL){ //processed kinematic data
		printf("initvel file '%s' cannot be opened!!!", initvelfilename);
		exit(0);
	}
        fscanf(fp_state,"%lf ", &mdata.hipR_w);
        fscanf(fp_state,"%lf ", &mdata.kneeR_w);
        fscanf(fp_state,"%lf ", &mdata.ankleR_w);
        fscanf(fp_state,"%lf ", &mdata.hipL_w);
        fscanf(fp_state,"%lf ", &mdata.kneeL_w);
        fscanf(fp_state,"%lf ", &mdata.ankleL_w);
        fscanf(fp_state,"%lf ", &mdata.legTx_v);
        fscanf(fp_state,"%lf ", &mdata.legTy_v);
        fscanf(fp_state,"%lf ", &mdata.pelvis_w);
   fclose(fp_state); 
   /////////////////////////////////////////////////////////////

   /*********** read experimental states (to overwrite) *********/
	if((fp_state=fopen(initstatefilename,"r"))==NULL){ //processed kinematic data
		printf("initvel file cannot be opened!!!");
		exit(0);
	}
	fgets(dummy,7,fp_state); fscanf(fp_state,"%lf",&mdata.legTx); dumm=fgetc(fp_state); //fgetc is for CR
	fgets(dummy,7,fp_state); fscanf(fp_state,"%lf",&mdata.legTy); dumm=fgetc(fp_state);
	fgets(dummy,11,fp_state); fscanf(fp_state,"%lf",&mdata.pelv_rot); dumm=fgetc(fp_state);
	fgets(dummy,6,fp_state); fscanf(fp_state,"%lf",&mdata.hipR); dumm=fgetc(fp_state);
	fgets(dummy,7,fp_state); fscanf(fp_state,"%lf",&mdata.kneeR); dumm=fgetc(fp_state);
	fgets(dummy,8,fp_state); fscanf(fp_state,"%lf",&mdata.ankleR); dumm=fgetc(fp_state);
	fgets(dummy,6,fp_state); fscanf(fp_state,"%lf",&mdata.hipL); dumm=fgetc(fp_state);
	fgets(dummy,7,fp_state); fscanf(fp_state,"%lf",&mdata.kneeL); dumm=fgetc(fp_state);
	fgets(dummy,8,fp_state); fscanf(fp_state,"%lf",&mdata.ankleL); dumm=fgetc(fp_state);
	
	fclose(fp_state);
	/*************************************************************/

	init_qs(); /* INIT_QS: this routine initializes the array of structures*/
	
	/*** Overwrite initial states (experimental data) ***/
	sdm.q[leg_tx].initial_value = mdata.legTx;
	sdm.q[leg_ty].initial_value = mdata.legTy-(LEG_TY_DIFF);
	sdm.q[pelvis_rotation].initial_value = mdata.pelv_rot;
	sdm.q[hip_angle_r].initial_value = mdata.hipR;
	sdm.q[knee_angle_r].initial_value = mdata.kneeR;
	sdm.q[ankle_angle_r].initial_value = mdata.ankleR;
	sdm.q[hip_angle_l].initial_value = mdata.hipL;
	sdm.q[knee_angle_l].initial_value = mdata.kneeL;
	sdm.q[ankle_angle_l].initial_value = mdata.ankleL;
	/****************************************************/

    init_segments(); /*set-up segment parameters*/

    if (read_parameters_file("forparams.txt",&sdm,&muscle_file,&kinetics_file,
			&output_motion_file,&output_kinetics_file) == code_bad) //read muscle file name and output file name (and kinetics file and its output name) here
    {
      fprintf(stderr, "Unable to load input parameters: forparams.txt not found.\n\n");
    }

    if (!muscle_file)
    {
      printf("Enter name of muscle file: ");
      fflush(stdout);
      if (scanf("%s", buffer) != 1)
      {
		fprintf(stderr,"Error reading name of muscle file.\n");
		exit(0);
      }
      mstrcpy(&muscle_file,buffer); //"muscle_file"=buffer (?)
    }

    init_joint_functions();
    init_q_restraint_functions();
    init_motion(); /* INIT_MOTION: this function turns off prescribed motion for all unconstrained q's.*/

    if (read_muscle_file(&sdm,muscle_file) == code_bad)
       exit(0);


}
/************************* End of init_simm ********************************/


/////////////////////////////////////////////////////////////////
/******** Run one simulation (mainly) to obtain a cost *********/
void first_run(int START,
			   double **XR, double **YR,
			   double **XL, double **YL,
			   double *x,int N_params,
			   double *VM, double *UB, double *LB, double *f,
			   int *I1, int *J1, int *K1, int *NEVALS, double *T)
{
	int i,j;
	int BAD=1;
	int NEVALS1, N_while_loop;
	double **XRa, **XLa ;//abscissa adjusted
	FILE *fp_read1, *fp_read2;
	char dummy[40];

	XRa=new double *[N_mus]; if(XRa==NULL) exit(0);
	XLa=new double *[N_mus]; if(XLa==NULL) exit(0);
	for(i=0;i<N_mus;i++){ 
		XRa[i]=new double[N_emg_step]; if(XRa[i]==NULL) exit(0);
		XLa[i]=new double[N_emg_step]; if(XLa[i]==NULL) exit(0);
	}

	if(START==1){//using the best x's but not using previous SA information
		/*if(PATTERN==EMG){ 
			if((fp_read1=fopen("SAstartXemg.dat","r"))==NULL){
				printf("The best onset file has to be provided.\n");
				exit(0);
			}
		}
		if(PATTERN==BLOCK){
			if((fp_read1=fopen("SAstartXblock.dat","r"))==NULL){
				printf("The best onset file has to be provided.\n");
				exit(0);
			}
		}*/
		//load the parameter file
		if((fp_read1=fopen(OPTPARAMFILE,"r"))==NULL){
			printf("The best onset file has to be provided.\n");
			exit(0);
		}
		
		for(i=0;i<N_params;i++){
			fscanf(fp_read1,"%lf",&x[i]);
		}
		fclose(fp_read1);

		change_init(x,N_params);//assign initial state velocities (last 9 parameters)

		//change excitation parameters according to previous "best" results.
//		mod_emg(XR,YR,XL,YL,x);
        update_group_excitation_patterns(XR,YR,XL,YL,x);
			
		for(j=0;j<N_mus;j++){
			for(i=0;i<N_emg_step;i++){
				XRa[j][i]=XR[j][i]*MAX_X/100.0;
				XLa[j][i]=XL[j][i]*MAX_X/100.0;
			}
		}//adjust abscissa
			
		//run simulation
		//BAD = submain(XRa,YR,XLa,YL);
		BAD = submain_ga(XRa,YR,XLa,YL);

		save_motion(); //write motion file even walking was not completed

		if(BAD==1){
			printf("\nNot Walking correctly...\n");
			printf("Not in annealing-loop yet...\n");
		}

		*f=get_cost(); //cost function
		printf(" Cost: %f\n", (*f));

		for(i=0;i<N_params;i++){
			VM[i] = (UB[i] - LB[i]);
		} //VM is step length for x

	}//end of if (START==1)
	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////
	if(START==0){//resume previous SA
		printf("Resume simulated annealing...\n\n");
		/*if((fp_read1=fopen("SAoptEMG.dat","r"))==NULL){
			printf("SAoptEMG.dat does not exist.\n");
			exit(0);
		}

		for(i=0;i<N_emg_step;i++){//read "optimal" excitation patterns
			for(j=0;j<N_mus;j++){
				fscanf(fp_read1,"%lf %lf %lf %lf",
				&XR[j][i], &YR[j][i], &XL[j][i], &YL[j][i]);
			}
		}
		fclose(fp_read1);

		for(j=0;j<N_mus;j++){
			for(i=0;i<N_emg_step;i++){
				XRa[j][i]=XR[j][i]*MAX_X/100.0;
				XLa[j][i]=XL[j][i]*MAX_X/100.0;
			}
		}//adjust abscissa
		*/
		/*if(PATTERN==EMG){ 
			if((fp_read1=fopen("SAstartXemg.dat","r"))==NULL){
				printf("The best onset file has to be provided.\n");
				exit(0);
			}
		}
		if(PATTERN==BLOCK){
			if((fp_read1=fopen("SAstartXblock.dat","r"))==NULL){
				printf("The best onset file has to be provided.\n");
				exit(0);
			}
		}*/
		//load the optimization parameter file
		if((fp_read1=fopen(OPTPARAMFILE,"r"))==NULL){
			printf("The best onset file has to be provided.\n");
			exit(0);
		}
		
		for(i=0;i<N_params;i++){
			fscanf(fp_read1,"%lf",&x[i]);
		}
		fclose(fp_read1);
		change_init(x,N_params);//assign initial state velocities (last 9 parameters)
	
		
		//change excitation parameters according to previous "best" results.
		//mod_emg(XR,YR,XL,YL,x);
		update_group_excitation_patterns(XR,YR,XL,YL,x);

		for(j=0;j<N_mus;j++){
			for(i=0;i<N_emg_step;i++){
				XRa[j][i]=XR[j][i]*MAX_X/100.0;
				XLa[j][i]=XL[j][i]*MAX_X/100.0;
			}
		}//adjust abscissa
			
		//run simulation
		//BAD = submain(XRa,YR,XLa,YL); 
		BAD = submain_ga(XRa,YR,XLa,YL); 
		save_motion(); //write motion file even walking was not completed
		*f=get_cost(); //cost function (this is to create SAoptERROR.dat)
		printf(" Cost: %f\n", (*f));

		if((fp_read2=fopen("SAoptINFO.dat","r"))==NULL){
			printf("SAoptINFO.dat could not read\n");
			exit(0);
		}
		//these are "optimal" info for previous SA run
		fscanf(fp_read2,"%s",dummy); //read name
		fscanf(fp_read2,"%d",&(*NEVALS));
		fscanf(fp_read2,"%lf",&(*f));
		fscanf(fp_read2,"%lf",&(*T));
		for(i=0;i<N_params;i++){
			fscanf(fp_read2,"%lf",&VM[i]);
		}
		for(i=0;i<N_params;i++){
			fscanf(fp_read2,"%lf",&x[i]);
		}
		fclose(fp_read2);
		
		change_init(x,N_params);//assign initial state velocities (last 9 parameters)

		///////adjustment for iteration	
		if(*NEVALS<=N_params*NS){
			*J1=*NEVALS/(N_params);
			*K1=*NEVALS%(N_params);
		}
		if((*NEVALS>N_params*NS) && (*NEVALS<=N_params*NS*NT)){
			*I1=*NEVALS/(N_params*NS);
			*J1=(*NEVALS-N_params*NS)/(N_params);
			*K1=(*NEVALS-N_params*NS)%(N_params);
		}
		if(*NEVALS>N_params*NS*NT){
			N_while_loop = *NEVALS/(N_params*NS*NT);
			NEVALS1 = *NEVALS - N_while_loop*N_params*NS*NT;
			if(NEVALS1<=N_params*NS){
				*J1=NEVALS1/(N_params);
				*K1=NEVALS1%(N_params);
			}
			//if((NEVALS1>N_params*NS) && (NEVALS1<=N_params*NS*NT)){
			if(NEVALS1>N_params*NS){
				*I1=NEVALS1/(N_params*NS);
				*J1=(NEVALS1-N_params*NS)/(N_params);
				*K1=(NEVALS1-N_params*NS)%(N_params);
			}
		}

		printf("\nNumber of calls=%d\n",*NEVALS);
		printf("T=%lf\n\n",*T);
		
	}//end of if(START==0)
	/////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////
	if(START==-1){//start from scratch completely

		while(BAD==1){ //pedaling goes backward
			for (i=0; i<N_params;i++){
				x[i] = (LB[i]+UB[i])/2 + (UB[i] - LB[i])/2 * (2 * drand48() - 1.0);
			}
		
			//mod_emg(XR,YR,XL,YL,x);
			update_group_excitation_patterns(XR,YR,XL,YL,x);
				
			for(j=0;j<N_mus;j++){
				for(i=0;i<N_emg_step;i++){
					XRa[j][i]=XR[j][i]*MAX_X/100.0;
					XLa[j][i]=XL[j][i]*MAX_X/100.0;
				}
			}//adjust abscissa

			//run simulation
			//BAD = submain(XRa,YR,XLa,YL); //0:walk correctly
			BAD = submain_ga(XRa,YR,XLa,YL); 
			if(BAD==1){
				printf("\nNot walking correctly...\n");
				printf("Not in annealing-loop yet...\n");
			}
		}//end of while loop

		//Need to adjust the step to compute cost function with experimental data
		*f=get_cost(); //cost function
		printf(" Cost: %f\n", (*f));

		for(i=0;i<N_params;i++){	
			VM[i] = (UB[i] - LB[i]); 
		} //VM is step length for x

	}//end of if(START==-1)

	delete [] XRa;
	delete [] XLa;
}
//////////////////////end of first_run///////////////////////////////////////////////////

/*********************Simulated Annealing loop**************************/
void s_annealing(int N_params, int nevals, double temperature, double fcost,
			   double *x, double **XR, double **YR, double **XL,double **YL,
			   int I1, int J1, int K1, double *VM, double *LB, double *UB )
{
	int i,ii,j,jj,k,g,h,a,m,k_random,same;
	int BAD, nup, nrej, nnew, ndown, lnobds, quit;
	int NOBDS = 0;
	int NACC = 0;
    int OPTEVAL = 0;
	int NEVALS = nevals;
	int MAXEVAL = 500000;
    double RT = 0.5; //temperature reduction factor
	int NEPS = 4; //# of function evaluations used in stop criterion
    double EPS = 1.0e-6; //stop criterion
	double f = fcost;
	double T = temperature;
	double fopt, fp, p, ratio, temp1, temp2;

    int *done = new int[N_params];
	int *nacp = new int[N_params];
	double *xopt = new double[N_params];
	double *xp = new double[N_params];
	double *c = new double[N_params];
	double *fstar = new double[NEPS];

	double **XRa,**XLa;


	XRa=new double *[N_mus]; if(XRa==NULL) exit(0);
	XLa=new double *[N_mus]; if(XLa==NULL) exit(0);
	for(i=0;i<N_mus;i++){
		XRa[i]=new double[N_emg_step]; if(XRa[i]==NULL) exit(0);
		XLa[i]=new double[N_emg_step]; if(XLa[i]==NULL) exit(0);
	}
	
	FILE *fid1;

	/////////////////SA initializing process//////////////////////
	if((fid1 = fopen("siman.log", "w"))==NULL){
		printf("siman.log cannot be opened!!!");
		exit(0);
	}
    fclose(fid1);

	for(i=0; i<NEPS; i++)
	{ fstar[i] = 1.0/1.0e-10; }

	srand( (unsigned)time( NULL ) ); //seed for random # generator
	//////////////////////////////////////////////////////////////

	fopt = f; 
	fstar[0] = f;

	for(i=0;i<N_params;i++){
		xopt[i]=x[i];
	}
	
	int ngen = 0;
	while(1) //do until convergence
	{
		BAD=1;
		nup = 0;
		nrej = 0;
		nnew = 0;
		ndown = 0;
		lnobds = 0;

		for (i=0; i<N_params;i++){
			nacp[i] = 0;
		} //number of acceptance

		for (i=I1;i<NT;i++){ //# times through NS loops before T reduction
			for (j=J1;j<NS;j++){ //# times through function before VM adjustment
				
				m=0; //for shuffling
				same=0; //for shuffling
				for(ii=0;ii<N_params;ii++){
					done[ii]=777; //these are dummy values for shuffling
				}

				for (k=K1;k<N_params;k++){ //go through all parameters
				
					///shuffling///	
					while(1){
						k_random=rand()%(N_params);
						//	printf("\n\nk:%d k_random = %d\n",k,k_random);
						for(ii=0;ii<N_params;ii++){
							if(done[ii]==k_random){
								same=1;
								break;
							}
						}
						if(same==0){
							done[m]=k_random;
							m++;
							break;
						}
						else same=0;
					}
					///end of shuffling///					
					//Now k_random is k	
					

					if (NEVALS >= MAXEVAL){
						printf("too many function evaluations\n");
						printf("fopt = %lf\n",fopt);
					}

					/* generate xp, trial value of x */
					for (g=0;g<N_params;g++){
						xp[g] = x[g]; 
					}
					

					//xp[k] = x[k] + VM[k] * (2 * drand48() - 1.0);
					xp[k_random] = x[k_random] + VM[k_random] * (2 * drand48() - 1.0);

					//if ((xp[k] < LB[k]) || (xp[k] > UB[k])){
					//	xp[k] = LB[k] + (UB[k] - LB[k]) * drand48();
					if ((xp[k_random] < LB[k_random]) || (xp[k_random] > UB[k_random])){
						xp[k_random] = LB[k_random] + (UB[k_random] - LB[k_random]) * drand48();
						lnobds++;
						NOBDS++;
					}
//printf("\nk_random = %d xp=%f\n",k_random,xp[k_random]);
					//mod_emg(XR,YR,XL,YL,xp);
					update_group_excitation_patterns(XR,YR,XL,YL,xp);

					change_init(xp,N_params); //iniial velocities

					for(jj=0;jj<N_mus;jj++){
						for(ii=0;ii<N_emg_step;ii++){
							XRa[jj][ii]=XR[jj][ii]*MAX_X/100.0;
							XLa[jj][ii]=XL[jj][ii]*MAX_X/100.0;
						}
					}//adjust abscissa
					BAD = submain(XRa,YR,XLa,YL);
					//BAD = submain_ga(XRa,YR,XLa,YL); 

				if(BAD==1) printf("\tWalking was not completed...\n");//and go to the next simulation

				else{ //running was completed

					NEVALS++;
					printf("\nNEVALS=%d\n",NEVALS);
					
					fp=get_cost();
					printf("COST=%f\n",fp);

					//save current results
					save_current(x,NEVALS,fp,T,VM,N_params);

					//write out the min to an R data file
					FILE *filep;
					char *fname = "sa_r_data.dat";
					filep = fopen(fname, "a");
					fprintf(filep, "[%d] %f\n", ngen + 1, fp);
					fclose(filep);
					ngen++;

					if (fp <= f){
						for (g=0;g<N_params;g++){
							x[g] = xp[g];
						}
						f=fp;
						NACC++;
						//nacp[k] = nacp[k] + 1;
						nacp[k_random] = nacp[k_random] + 1;
						nup++;




						/* if smaller than any previous point, record as new optimum */
						if (fp < fopt){
							for (g=0;g<N_params;g++){
								xopt[g] = xp[g]; 
							}
							fopt=fp;
							OPTEVAL = NEVALS;
							nnew++;
							
							//////////best excitation pattern////////////
							for(jj=0;jj<N_mus;jj++){
								for(ii=0;ii<N_emg_step;ii++){
									mdata.XRp[jj][ii]=XR[jj][ii];
									mdata.YRp[jj][ii]=YR[jj][ii];
									mdata.XLp[jj][ii]=XL[jj][ii];
									mdata.YLp[jj][ii]=YL[jj][ii];
								}
							} 
							/////////////////////////////////////////////
							//save the best results
							save_best(xopt,OPTEVAL,fopt,T,VM,N_params);
							//save motion file
							save_motion();
						}
					}

					else {/* function value increases */
						
						p = exp((f - fp)/T);
						if (drand48() < p) {/* accept although higher */						
							for (g=0;g<N_params;g++){
								x[g] = xp[g];
							}

							f = fp;
							NACC++;
							//nacp[k] = nacp[k] + 1;
							nacp[k_random] = nacp[k_random] + 1;
							ndown++;
						}
						else  //reject
							nrej = nrej + 1;
				
					}/* end else*/
				}/*end of else (BAD==0)*/
			   } /* end k */
			   K1=0; //reset to 0
			} /* end j */
			J1=0; //reset to 0

			/* adjust maximal step size vm */
			for (h=0;h<N_params;h++){
				c[h] = 2;
			}

			fid1 = fopen("siman.log","a");
			fprintf(fid1,"-------------------------------------------------------\n");
			fprintf(fid1,"NEVALS: %d\n",NEVALS);
			fprintf(fid1,"Temperature: %10.5f\n\n",T);
			fprintf(fid1,"Parameter   Ratio    Stepsize\n");
			fprintf(fid1,"---------   -----    --------\n");
	
			for (h=0;h<N_params;h++){
				temp1 = nacp[h];
				temp2 = NS;
				ratio = temp1/temp2;
				//printf("h = %i   nacp(h) = %i   ratio = %lf\n",h,nacp[h],ratio);
				if (ratio > 0.6)
					VM[h] = VM[h] * (1 + c[h] * (ratio - 0.6) / 0.4);
				else if (ratio < 0.4)
					VM[h] = VM[h] / (1 + c[h] * ((0.4 - ratio)/ 0.4));
				if (VM[h] > (UB[h] -LB[h]))
					VM[h] = UB[h] - LB[h];
				fprintf(fid1,"%6d     %7.5f %8.4f\n",h,ratio,VM[h]); 
			}
			fclose(fid1);

			for(h=0;h<N_params;h++){  /*reset number of acceptances*/
			  nacp[h] = 0; 
			}
		} /* end i */
		I1=0; //reset to 0

		printf("Intermediate results before temp change\n");
		printf("T = %lf\n", T);
		printf("xopt = %lf %lf\n", xopt[0], xopt[1]);
		printf("fopt = %lf\n", fopt);
    
		/* check termination criteria */
		fstar[0] = f;
		quit = ((fstar[0] - fopt) <= EPS);
		
		for (a=0;a<NEPS;a++){
			if (fabs(fstar[a] - f) > EPS)
				quit = 0;
		}

		if (quit){
			printf("simulated annealing achieved termination after %d evals\n", NEVALS);
			//return fopt;
			printf("fopt = %lf\n",fopt);
		}

		/* reduce temperature */
		T = T * RT;
		fstar[3] = fstar[2];
		fstar[2] = fstar[1];
		fstar[1] = fstar[0];
		/* continue from current optimum */
	
		for (g=0;g<N_params;g++){
			x[g] = xopt[g];
		}
		f = fopt;
    }  /* end while */


	delete [] done;
	delete [] nacp;
	delete [] xopt;
	delete [] xp;
	delete [] c;
	delete [] fstar;
	delete [] XRa;
	delete [] XLa;


}

/************end of simulated annealing function******************/
///////////////////////////////////////////////////////////////////




