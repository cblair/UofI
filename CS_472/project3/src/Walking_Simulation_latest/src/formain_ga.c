/*******************************************************************************

   FORMAIN.C

   Author: Peter Loan

   Date of creation: 22-OCT-95

   Copyright (c) 1996 MusculoGraphics, Inc.
   All rights reserved.

   Description: This file contains the main routine for a forward dynamics
		simulation, and several other utility routines that are
		independent of the musculoskeletal model in the simulation.
		You may want to copy this file and make changes to it as needed
		for your particular simulation.

*******************************************************************************/
#include "universal.h"
#include "system_params.h"

//#include "Look.h"
#include "SAstructs.h"
#include "SA.h"
#include "ExcGroups.h"

#include "ga_main.h"


/*************** DEFINES (for this file only) *********************************/
/*
#define BAUMGARTE_STAB 20.0

//////////////////////////////////////////////////////////////////
//functions added (or changed) in this file by Ko
void excitation_integ(double **, double **, double **, double **);
void calc_act(double, double [], double [], 
			  double [], int *);
void apply_ground_contact_forces(SDModelStruct*, double[], double[], double);
void calc_net_grfs(double *, double *, double *, double *); 
*/

extern double calc_metabolic_cost(MuscleStruct* ms);
extern void init_met_values();

/*************** STATIC GLOBAL VARIABLES (for this file only) *****************/
/*************** STATIC GLOBAL VARIABLES (for this file only) *****************/
//extern double max_time = 0.0;
extern double max_time;
//extern int nstep = 0;
extern int nstep;

/**************** GLOBAL VARIABLES (used in only a few files) *****************/
//extern SBoolean verbose = no;
extern SBoolean verbose;
//extern MotionData* external_forces = NULL;
extern MotionData* external_forces;

/**************** GLOBAL VARIABLES (used in only a few files) *****************/
extern SDModelStruct sdm;

/************Global structure****************/
//SeeThis mdata;
extern OptStruct mdata;
/********************************************/
/*************** EXTERNED VARIABLES (declared in another file) ****************/



/*************** PROTOTYPES for STATIC FUNCTIONS (for this file only) *********/

/*************** PROTOTYPES for STATIC FUNCTIONS (for this file only) *********/

static FILE* open_output_motion_file(char filename[], int num_datarows,
				     double range_start, double range_end,
				     double stepsize);
static void print_simulation_info(double t, double y[], double system_energy);

extern int PATTERN;

/************ Activation time constant **************/
extern double c1,c2;
extern int FRAME;

extern double rCOPglobal[3], lCOPglobal[3], rGRFglobal[3], lGRFglobal[3]; 

extern double pssvJtrq[10];
extern double pssvJtrqDamping[10];
//0: hip_r
//1: knee_r
//2: ankle_r
//3: midfoot_r
//4: mtp_r
//5: hip_l
//6: knee_l
//7: ankle_l
//8: midfoot_l
//9: mtp_l



int submain_ga(double **XR,double **YR, double **XL,double **YL)
		    //these arguments are excitaion patterns//
{
    int BAD=0;
	int index_5;

	MuscleStruct* ms;

   int i, j, nstep = NintegStep, fcnt, err, *lock, which;
   //double t = 0.0, dt = MAX_TIME*1.5/NintegStep, ctol = 0.0000001, tol = 0.0001;
   double t = 0.0, dt = MAX_TIME*1.5/750.0, ctol = 0.0000001, tol = 0.0001;
   double *y, *dy, *work, param[1], step, system_energy=0.0;
   double *joint_torque,*joint_power;
   double **muscle_E;
   static double forces[NJNT][3], torques[NJNT][3], f[3];
   static double mus_pwr[NintegStep][N_mus*2], fib_pwr[NintegStep][N_mus*2],mus_frc[NintegStep][N_mus*2];
   static double track[NintegStep][30];

   double tx_var,ankavel,temp3[3],temp2[3];

   double zero[3],postemp[3],veltemp[3],angveltemp[3],segme_total[25];
   double masstemp, inertemp[3][3],pos[NBOD];
   double inertia[NBOD],mass[NBOD],vel[NBOD][3],angvel[NBOD];

   double ke,lm[3],am[3],mtot,cm[3],icm[3][3], extpower;

   double muscle_power;

   double muscle_exc;

   double MusMetabEnergy=0.0;

   double LgrfOn;

   //added 2-25-08
   int LgrfOn1, LgrfOn2;
   double net_musculotendon_work=0.0; 
   double net_passivejnt_work=0.0;
   double net_shoe_work=0.0;
   double net_tendon_work=0.0;
   double net_system_work;
   double omegaJ[10];


   char* output_motion_file = "forward.mot";
   FILE *fp_write1;

   //int shoe_array_size=132;
   static double grf[NintegStep][6];
   ShoeStruct* ss;

   //ga_test();

   //initialize grf vectors to zero
   for(i=0;i<NintegStep;i++){
	   for(j=0;j<6;j++){
		   grf[i][j] = 0.0;
	   }
   }

/*
   printf("\nDynamics Pipeline Forward Simulation.\n\n");
*/
/* 
   muscle_E=calloc(nstep,sizeof(double*));
	if(muscle_E == NULL) exit(0);
	for(i=0;i<nstep;i++){
		muscle_E[i]=calloc(N_mus*2,sizeof(double));
		if(muscle_E[i] == NULL) exit(0);
	}
*/   


   sdm.neq = sdm.nu + sdm.nq + count_muscle_states(&sdm);

   lock = (int*)simm_malloc(sdm.nq*sizeof(int));
   y = (double*)simm_malloc(sdm.neq*sizeof(double));
   dy = (double*)simm_malloc(sdm.neq*sizeof(double));
   work = (double*)simm_malloc(6*sdm.neq*sizeof(double));
   joint_torque = (double*)calloc(sdm.nq,sizeof(double));
   joint_power = (double*)calloc(sdm.nq,sizeof(double));


   for (i=0; i<sdm.neq; i++)
      y[i] = dy[i] = 0.0;

   for (i=0; i<sdm.nq; i++)
       y[i] = sdm.q[i].initial_value/sdm.q[i].conversion;

   step = dt;

/******************* Read in Muscle onset/offset and **********************
 ******************* step function excitation values **********************/

	for(j=0; j<N_mus*2; j++){

		sdm.muscles[j].excitation->numpoints = N_emg_step;//make space
		if (realloc_function(sdm.muscles[j].excitation,sdm.muscles[j].excitation->numpoints) == code_bad)
		exit(0);
	}

	  for(j=0; j<N_mus; j++){
		  //for (i=0; i<sdm.muscles[j].excitation->numpoints; i++){
		  for (i=0; i<N_emg_step; i++){
			  sdm.muscles[j].excitation->x[i] = XR[j][i];
			  sdm.muscles[j].excitation->y[i] = YR[j][i];
		  }
	  }//for right leg

	  for(j=N_mus; j<N_mus*2; j++){
		  //for (i=0; i<sdm.muscles[j].excitation->numpoints; i++){
		  for (i=0; i<N_emg_step; i++){
			  sdm.muscles[j].excitation->x[i] = XL[j-N_mus][i];
			  sdm.muscles[j].excitation->y[i] = YL[j-N_mus][i];
		  }
	  }//for left leg

	  //////////////just for checking excitation patterns	  
		if((fp_write1=fopen("ExcPattern","w"))==NULL){
		printf("ExcPattern cannot be opened!!!");
		exit(0);
		}	

		for(i=0;i<N_emg_step;i++){
			for(j=0;j<N_mus;j++){
				fprintf(fp_write1,"%lf %lf %lf %lf ",
				(sdm.muscles[j].excitation->x[i])*100.0/MAX_X, 
				sdm.muscles[j].excitation->y[i],
				(sdm.muscles[j+N_mus].excitation->x[i])*100.0/MAX_X,
				sdm.muscles[j+N_mus].excitation->y[i]);
			}
			fprintf(fp_write1,"\n");
		}
		fclose(fp_write1);				
	   ////////////////end of checking


/***Integrate the excitation and assign the initial value of activation***/
   excitation_integ(XR,YR,XL,YL);
 
/************************ Read in initial state ***************************
 **************************************************************************/

    y[sdm.nq + leg_tx]=mdata.legTx_v;
	y[sdm.nq + leg_ty]=mdata.legTy_v;
	y[sdm.nq + pelvis_rotation]=mdata.pelvis_w;
	y[sdm.nq + hip_angle_r]=mdata.hipR_w;
	y[sdm.nq + knee_angle_r]=mdata.kneeR_w;
	y[sdm.nq + ankle_angle_r]=mdata.ankleR_w;
	y[sdm.nq + midfoot_angle_r]=mdata.midR_w;
	y[sdm.nq + mtp_angle_r]=mdata.mtpR_w;
	y[sdm.nq + hip_angle_l]=mdata.hipL_w;
	y[sdm.nq + knee_angle_l]=mdata.kneeL_w;
	y[sdm.nq + ankle_angle_l]=mdata.ankleL_w;
	y[sdm.nq + midfoot_angle_l]=mdata.midL_w;
	y[sdm.nq + mtp_angle_l]=mdata.mtpL_w;
	

	dy[leg_tx]=mdata.legTx_v;
	dy[leg_ty]=mdata.legTy_v;
	dy[pelvis_rotation]=mdata.pelvis_w;
	dy[hip_angle_r]=mdata.hipR_w;
	dy[knee_angle_r]=mdata.kneeR_w;
	dy[ankle_angle_r]=mdata.ankleR_w;
	dy[midfoot_angle_r]=mdata.midR_w;
	dy[mtp_angle_r]=mdata.mtpR_w;
	dy[hip_angle_l]=mdata.hipL_w;
	dy[knee_angle_l]=mdata.kneeL_w;
	dy[ankle_angle_l]=mdata.ankleL_w;
	dy[midfoot_angle_l]=mdata.midL_w;
	dy[mtp_angle_l]=mdata.mtpL_w;


/*****************************************************************************/

   /* Open SIMM motion file for storing simulation results */

	mdata.motion_file_name = output_motion_file;
	mdata.nstep = nstep;
	mdata.dt = dt;

   //save initial conditions
   //initial states maybe changed by sdassemble
   j=0;
   for (i=0; i<sdm.nq; i++){
	   //if (sdm.q[i].type == unconstrained_q){
		   mdata.init_y[j] = y[i];
		   mdata.init_dy[j] = dy[i];
		   j++;
	   //}
   }
   
/////////////////////////////////////////////////////////////


   sdinit();

   for (i=0; i<sdm.nq; i++)
   {
      if (sdm.q[i].type == prescribed_q)
         lock[i] = 1;
      else
         lock[i] = 0;
   }

   sdassemble(t,y,lock,ctol,500,&fcnt,&err);
   if (err != 0)
   {
      fprintf(stderr,"Assembly failed, err = %d\n", err);
      fprintf(stderr,"Closest solution:\n");
      for (i=0; i<sdm.nq; i++)
         if (sdm.q[i].type == unconstrained_q)
            fprintf(stderr,"%8.3lf ", y[i]*sdm.q[i].conversion);
      fprintf(stderr,"\n");
      exit(0);
   }
   if (verify_assembly(&sdm,y) == code_bad)
      exit(0);


   sdinitvel(t,y,lock,ctol,500,&fcnt,&err);
   if (err != 0)
   {
      fprintf(stderr,"Velocity analysis failed\n");
      exit(0);
   }

   /* Set the Baumgarte stabilization constants. This type of
    * stabilization should help reduce the errors from constraint
    * violations. The position and velocity errors are multiplied by
    * constants so that the integrator "takes them more seriously."
    * The first argument to sdstab() is for position, the second one
    * is for velocity. Feel free to adjust the value of BAUMGARTE_STAB
    * if the integration still involves high errors or is proceeding
    * too slowly. Set the value to 0 to turn off stabilization.
    */

   sdstab(2*BAUMGARTE_STAB,BAUMGARTE_STAB*BAUMGARTE_STAB);

   sdstate(t,y,&y[sdm.nq]);

   init_muscle_states(&sdm,y);
   init_met_values();

   tx_var = 0.0;


///////////////////////////////////////////////////////////////////////////

   index_5=0;//index to count every 5 step

   mdata.Lgrf_on=0.0;

   LgrfOn1=100000;
   LgrfOn2=100000; 
/***************************************************************/
/***************************************************************/
   for (i=0; i<nstep; i++) //integration loop
   {

	   FRAME=i;

	   if(y[leg_ty]<0.3){ //collapsing
		   BAD=1;
		   break;
	   }

/* Reset the leg displacement for the muscle excitation patterns */
   if (y[leg_tx] > MAX_X) 
	{
	y[leg_tx] = y[leg_tx] - MAX_X;
	tx_var = MAX_X;
	}

	
   for (j=0; j<sdm.num_muscles; j++){
         sdm.muscles[j].excitation_level = calc_excitation(&sdm.muscles[j],t,y);
		 }

      calc_derivatives(t,y,dy,param,&err);

      sdvinteg(calc_derivatives,&t,y,dy,param,dt,&step,sdm.neq,
	       tol,work,&err,&which);


      if (err != 0)
      {
         printf("Simulation stopped:\n");
         if (err == 3)
            printf("%s out of range. value = %lf, range = %lf to %lf\n",
		   sdm.q[which-1].name, y[which-1]*sdm.q[which-1].conversion,
		   sdm.q[which-1].range_start, sdm.q[which-1].range_end);
         else
            printf("sdvinteg error status %d, error number %d\n", err, which);
		 BAD=1;
         break;
      }

      assign_muscle_states(&sdm,t,y);

/*** compute system mechanical energy ***/

      for(j=0;j<3;j++)
         zero[j]=0.0;

      for(j=0;j<NBOD;j++)
         {
         sdgetiner(j,inertemp);
            inertia[j] =inertemp[2][2];
         sdgetmass(j,&masstemp);
            mass[j] =  masstemp;
         sdvel(j,zero,veltemp);
            vel[j][0] = veltemp[0];
            vel[j][1] = veltemp[1];
         sdangvel(j,angveltemp);
            angvel[j] = angveltemp[2];
		 sdpos(j,zero,postemp);
			pos[j] = postemp[1];
         }

      segment_me(segme_total, inertia, mass, vel, angvel, pos);


/******* Compute net joint moments */

   calc_joint_torques(t, y, dy, joint_torque);

   calc_joint_powers(joint_torque, joint_power);


muscle_power=0.0;
muscle_exc=0.0;
//MusMetabEnergy=0.0;
 	  
   for (j=0; j<N_mus*2; j++)
      {
      ms = &sdm.muscles[j]; 
      //mus_pwr[i][j] = sdm.muscles[j].force*calc_muscle_tendon_velocity(ms);
	  muscle_power=muscle_power+pow((sdm.muscles[j].force*calc_muscle_tendon_velocity(ms)),2.0);
//	    fib_pwr[i][j] = sdm.muscles[j].fiber_velocity*sdm.muscles[j].fiber_active_force;
//      mus_frc[i][j] = sdm.muscles[j].force;
      }
   
	
//if((i>=50*5 && i<=65*5) || (i>=135*5)){ 
	for (j=0; j<N_mus*2; j++){
		//if(j==sol_l || j==mgas_l || j==perlng_l || j==ta_l || //ankle
		//	j==vas1_l || j==sm_l || j==bfsh_l){ //knee
		//	muscle_exc=muscle_exc + sdm.muscles[j].excitation_level;
		//}
		muscle_exc=muscle_exc + sdm.muscles[j].excitation_level;
	}
//}
/////////////////////////////Muscle Energetics///////////////////////////////////////////////////////////////////////
  
   
 calc_metabolic_cost(ms);
 for(j=0;j<N_mus*2;j++){ 
//	   muscle_E[i][j]=sdm.muscles[j].total_energy;
//	   printf("E=%f\n",sdm.muscles[j].total_energy);
//	   getchar();
	   if(i>=250 && i<=750){	   
//	   MusMetabEnergy=MusMetabEnergy+(muscle_E[i][j]*dt);
		MusMetabEnergy=MusMetabEnergy+(sdm.muscles[j].total_energy*dt);
		
	   }
   }

//printf("MusE= %f\n", MusMetabEnergy);

/*
for(j=0;j<N_mus*2;j++){ 
	   Ham[i][j]=sdm.muscles[j].Ham;
	  // printf("E=%f\n",muscle_E[i][j-N_mus]);
   }

for(j=0;j<N_mus*2;j++){ 
	   Hsl[i][j]=sdm.muscles[j].Hsl;
	  // printf("E=%f\n",muscle_E[i][j-N_mus]);
   }


for(j=0;j<N_mus*2;j++){ 
	   Hmw[i][j]=sdm.muscles[j].Hmw;
	  // printf("E=%f\n",muscle_E[i][j-N_mus]);
   }
*/
/*for(j=0;j<N_mus*2;j++){ 
	   TestVar[i][j]=sdm.muscles[j].TestVar;
	  // printf("E=%f\n",muscle_E[i][j-N_mus]);
   }*/

  // getchar();
  // printf("Energy = %f\n",muscle_E);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    /* Write Qs and muscle activations to a SIMM motion file */
	for (j=0; j<sdm.nq; j++){
		if (sdm.q[j].type == unconstrained_q){
			if (j == leg_tx){
				mdata.motion_y[i][j] = y[j]*sdm.q[j].conversion+tx_var;
			}
			else{
				mdata.motion_y[i][j] = y[j]*sdm.q[j].conversion;
			}
		}
	}
	 
	for (j=0; j<sdm.num_muscles; j++){
		 mdata.mus_dyn_act[i][j] = sdm.muscles[j].dynamic_activation;     
	}

	//added 1-16-08
	for(j=0;j<3;j++){
		mdata.Rgrf[i][j]=rGRFglobal[j];
		mdata.Rcop[i][j]=rCOPglobal[j];
		mdata.Lgrf[i][j]=lGRFglobal[j]; 
		mdata.Lcop[i][j]=lCOPglobal[j];
	}
	mdata.Rcop[i][0]=mdata.Rcop[i][0]+tx_var;
	mdata.Lcop[i][0]=mdata.Lcop[i][0]+tx_var;
	////////////////////////////////////////

     
/* Sum GRF forces over all right shoe elements */

      for (j=0; j<sdm.num_shoe_elements/2; j++)
	 {
	  ss = &sdm.shoe_element[j];
	  grf[i][0]+=ss->grf[0]/(9.81*MODEL_WEIGHT);
	  grf[i][1]+=ss->grf[1]/(9.81*MODEL_WEIGHT);
	  grf[i][2]+=ss->grf[2]/(9.81*MODEL_WEIGHT);
	 }
      for (j=sdm.num_shoe_elements/2; j<sdm.num_shoe_elements; j++)
	 {
	  ss = &sdm.shoe_element[j];
	  grf[i][3]+=ss->grf[0]/(9.81*MODEL_WEIGHT);
	  grf[i][4]+=ss->grf[1]/(9.81*MODEL_WEIGHT);
	  grf[i][5]+=ss->grf[2]/(9.81*MODEL_WEIGHT);
	 }

LgrfOn=((double)i)/5.0;
if(mdata.Lgrf_on==0.0){
	if(25.0<=LgrfOn){
		if(grf[i][4]>0.01){
			mdata.Lgrf_on=LgrfOn; // % gait cycle where L heel strike occurs
			LgrfOn1=i;
		}
	}
}


	//################################################//
    // find the next left heel strike (around 150% gait cycle); added 2-25-08
	if(i>=135*5 && i<=165*5){
		if(grf[i][4]>0.01){
			LgrfOn2=i-1;
		}
	}

	if(i>=LgrfOn1 && i<=LgrfOn2){

		//net musculotendon work
		for (j=0; j<N_mus*2; j++){ 
			ms = &sdm.muscles[j]; 
			net_musculotendon_work=net_musculotendon_work+
				(-1.0)*(sdm.muscles[j].force)*(calc_muscle_tendon_velocity(ms))*dt;

			net_tendon_work=net_tendon_work+
				(-1.0)*(calc_muscle_tendon_velocity(ms)-sdm.muscles[j].fiber_velocity)*(sdm.muscles[j].force)*dt;
		}


		//net passive joint work
		omegaJ[hipJ_r]=dy[hip_angle_r];
		omegaJ[kneeJ_r]=dy[knee_angle_r];
		omegaJ[ankleJ_r]=dy[ankle_angle_r];
		omegaJ[midfootJ_r]=dy[midfoot_angle_r];
		omegaJ[mtpJ_r]=dy[mtp_angle_r];
		omegaJ[hipJ_l]=dy[hip_angle_l];
		omegaJ[kneeJ_l]=dy[knee_angle_l];
		omegaJ[ankleJ_l]=dy[ankle_angle_l];
		omegaJ[midfootJ_l]=dy[midfoot_angle_l];
		omegaJ[mtpJ_l]=dy[mtp_angle_l];
	 
		for(j=0;j<10;j++){
			net_passivejnt_work=net_passivejnt_work+omegaJ[j]*pssvJtrq[j]*dt;
		}

		//net shoe work
		for (j=0; j<sdm.num_shoe_elements; j++){//both right and left
			ss = &sdm.shoe_element[j];
			net_shoe_work=net_shoe_work+dt*(ss->power);
		}


		//total net system work
		net_system_work=net_musculotendon_work+
					    net_passivejnt_work+
					    net_shoe_work;

	}//end of if(i>=LgrfOn1 && i<=LgrfOn2){


	
	
//printf("%d:%f %f\n",i,net_system_work, net_tendon_work);


	//################################################//



/* Compute the left ankle angular velocity */
        
      sdangvel(tibia_l,temp2);
      sdangvel(calcn_l,temp3);

      ankavel=temp3[2]-temp2[2];

/* Compute external power (based on left side) */
	  sdmom(lm,am,&ke);
      sdsys(&mtot,cm,icm);

	  extpower=grf[i][3]*(lm[0]/mtot) +
				grf[i][4]*(lm[1]/mtot); //this is "BW-normalized" value


	  ///////////////////Save data for tracking///////////////////////
		
	  if((i%5)==0){ //every 5 step
		//kinematic data
			mdata.hip_r_ang[index_5] = y[hip_angle_r]*sdm.q[hip_angle_r].conversion;//from rad to ang
			mdata.knee_r_ang[index_5] = y[knee_angle_r]*sdm.q[knee_angle_r].conversion;
			mdata.ankle_r_ang[index_5] = y[ankle_angle_r]*sdm.q[ankle_angle_r].conversion;
			mdata.hip_l_ang[index_5] = y[hip_angle_l]*sdm.q[hip_angle_l].conversion;
			mdata.knee_l_ang[index_5] = y[knee_angle_l]*sdm.q[knee_angle_l].conversion;
			mdata.ankle_l_ang[index_5] = y[ankle_angle_l]*sdm.q[ankle_angle_l].conversion;
			mdata.hip_tx[index_5] = y[leg_tx]+tx_var;
			mdata.hip_ty[index_5] = y[leg_ty]*sdm.q[leg_ty].conversion;
			mdata.ankle_l_vel[index_5] = y[sdm.nq + ankle_angle_l]*sdm.q[knee_angle_l].conversion;	  
			mdata.pelvis_rot[index_5] = y[pelvis_rotation]*sdm.q[pelvis_rotation].conversion;
	  //kinetic data
	  //calc_joint_torques(t, y, dy, joint_torque);
			mdata.hip_l_trq[index_5] = joint_torque[hip_angle_l]/MODEL_WEIGHT;
			mdata.knee_l_trq[index_5] = joint_torque[knee_angle_l]/MODEL_WEIGHT;
			mdata.ankle_l_trq[index_5] = joint_torque[ankle_angle_l]/MODEL_WEIGHT;

	  //calc_joint_powers(joint_torque, joint_power);
			mdata.hip_l_pwr[index_5]=joint_power[hip_angle_l]/MODEL_WEIGHT;
			mdata.knee_l_pwr[index_5]=joint_power[knee_angle_l]/MODEL_WEIGHT;
			mdata.ankle_l_pwr[index_5]=joint_power[ankle_angle_l]/MODEL_WEIGHT;

	  //GRFs
			mdata.grfx_r[index_5] = grf[i][0];
			mdata.grfz_r[index_5] = grf[i][1];
			mdata.grfx_l[index_5] = grf[i][3]; 
			mdata.grfz_l[index_5] = grf[i][4];

	  //total segment mechanical energy
			mdata.segme_r[index_5] = segme_total[1];
			mdata.segme_l[index_5] = segme_total[2];

	  //external power (added 9-1-03)
			mdata.extpwr[index_5] = extpower;

      //muscle power (added 8-17-04)
			mdata.muspower[index_5] = muscle_power;

	  //muscle excitation (added 8-06-07)
			mdata.musexc[index_5] = muscle_exc;

	  //muscle metabolic cost (added 2-10-10)
			mdata.metE[index_5] = MusMetabEnergy;

	  //total system mechanical energy (added 9-15-03)
			mdata.sysme[index_5] = segme_total[0]+segme_total[1]+segme_total[2];
			mdata.syske[index_5] = ke;
			mdata.syspe[index_5] = segme_total[3]+segme_total[7]+segme_total[11]+segme_total[15]+segme_total[19]+segme_total[21]+segme_total[23];
			//mdata.syske[index_5] = mdata.sysme[index_5]-mdata.syspe[index_5];

	  //tracking
	  		mdata.track[0][index_5] = y[hip_angle_r]*sdm.q[hip_angle_r].conversion;
			mdata.track[1][index_5] = y[knee_angle_r]*sdm.q[knee_angle_r].conversion;
			mdata.track[2][index_5] = y[ankle_angle_r]*sdm.q[ankle_angle_r].conversion;
			mdata.track[3][index_5] = y[hip_angle_l]*sdm.q[hip_angle_l].conversion;
			mdata.track[4][index_5] = y[knee_angle_l]*sdm.q[knee_angle_l].conversion;
			mdata.track[5][index_5] = y[ankle_angle_l]*sdm.q[ankle_angle_l].conversion;
			mdata.track[6][index_5] = y[leg_tx]+tx_var;
			mdata.track[7][index_5] = y[leg_ty]+LEG_TY_DIFF;
			mdata.track[8][index_5] = grf[i][1];
			mdata.track[9][index_5] = joint_torque[hip_angle_l]/MODEL_WEIGHT;
			mdata.track[10][index_5] = joint_torque[knee_angle_l]/MODEL_WEIGHT;
			mdata.track[11][index_5] = joint_torque[ankle_angle_l]/MODEL_WEIGHT;
			mdata.track[12][index_5] = grf[i][4];
			mdata.track[13][index_5] = joint_power[hip_angle_l]/MODEL_WEIGHT;
			mdata.track[14][index_5] = joint_power[knee_angle_l]/MODEL_WEIGHT;
			mdata.track[15][index_5] = joint_power[ankle_angle_l]/MODEL_WEIGHT;
			mdata.track[16][index_5] = grf[i][3];
			mdata.track[17][index_5] = ankavel;
			mdata.track[18][index_5] = segme_total[1];
			mdata.track[19][index_5] = segme_total[2];
			mdata.track[20][index_5] = grf[i][0]; //ant-post grf of the right
			mdata.track[21][index_5] = y[pelvis_rotation]*sdm.q[pelvis_rotation].conversion;
			mdata.track[22][index_5] = extpower;
			mdata.track[23][index_5] = segme_total[0]+segme_total[1]+segme_total[2];
			mdata.track[24][index_5] = segme_total[3]+segme_total[7]+segme_total[11]+segme_total[15]+segme_total[19]+segme_total[21]+segme_total[23];
			mdata.track[25][index_5] = ke;
			//mdata.track[25][index_5] = mdata.sysme[index_5]-mdata.syspe[index_5];
			index_5++;
		}
	 

	  ///////////////////////////////////////////////////////////////
	  ///////////////////////////////////////////////////////////////
	  print_simulation_info(t,y,system_energy);

   } /* end of iteration loop */

   mdata.NetSystemWork=net_system_work;
   mdata.NetTendonWork=net_tendon_work;

   free(lock);
   free(y);
   free(dy);
   free(work);
   free(joint_torque);
   free(joint_power);  

   if(BAD==1){ 
	   return BAD;
   } //didn't walk correctly

   else{
	   sdprinterr(stdout);
	   return 0; //pedal went forward
   }
   //printf("\n\nSimulation completed.\n");

}//end of main
///////////////////////////////////////////////////////////////////
/*****************************************************************/
///////////////////////////////////////////////////////////////////

static void print_simulation_info(double t, double y[], double system_energy)
{

   int i;

   /* Write Qs to stdout to show progress of simulation */

   if (verbose == yes)
   {
      printf("ga t=%6.4lf | ", t);
      for (i=0; i<sdm.nq; i++)
	 if (sdm.q[i].type == unconstrained_q)
	    printf("%8.3lf ", y[i] * sdm.q[i].conversion);
      printf("  sys_energy= %10.3lf\n", system_energy);
   }
   else
   {
      printf("\rga t = %6.4lf", t);
      fflush(stdout);
   }

   /* Print muscle info to stdout */

   if (verbose == yes)
   {
      for (i=0; i<sdm.num_muscles; i++)
      {
	 printf("muscle %10s: activation = %7.5lf force = %7.2lf\n",
		sdm.muscles[i].name, sdm.muscles[i].dynamic_activation,
		sdm.muscles[i].force);
      }
      printf("********************************************************************************\n");
   }

}