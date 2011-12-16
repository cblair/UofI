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

/*************** DEFINES (for this file only) *********************************/

#define BAUMGARTE_STAB 20.0

//////////////////////////////////////////////////////////////////
//functions added (or changed) in this file by Ko
void excitation_integ(double **, double **, double **, double **);
void calc_act(double, double [], double [], 
			  double [], int *);
void apply_ground_contact_forces(SDModelStruct*, double[], double[], double);
void calc_net_grfs(double *, double *, double *, double *); 

double calc_metabolic_cost(MuscleStruct* ms);
void init_met_values();

/*************** STATIC GLOBAL VARIABLES (for this file only) *****************/
/*************** STATIC GLOBAL VARIABLES (for this file only) *****************/
double max_time = 0.0;
int nstep = 0;

/**************** GLOBAL VARIABLES (used in only a few files) *****************/
SBoolean verbose = no;
MotionData* external_forces = NULL;

/**************** GLOBAL VARIABLES (used in only a few files) *****************/
extern SDModelStruct sdm;

/************Global structure****************/
//SeeThis mdata;
OptStruct mdata;
/********************************************/
/*************** EXTERNED VARIABLES (declared in another file) ****************/



/*************** PROTOTYPES for STATIC FUNCTIONS (for this file only) *********/

/*************** PROTOTYPES for STATIC FUNCTIONS (for this file only) *********/

static FILE* open_output_motion_file(char filename[], int num_datarows,
				     double range_start, double range_end,
				     double stepsize);
static void print_simulation_info(double t, double y[], double system_energy);

int PATTERN;

/************ Activation time constant **************/
double c1,c2;
int FRAME;

double rCOPglobal[3], lCOPglobal[3], rGRFglobal[3], lGRFglobal[3]; 

double pssvJtrq[10];
double pssvJtrqDamping[10];
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



int submain(double **XR,double **YR, double **XL,double **YL)
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



/***************************************************************************/
/* APPLY_PASSIVE_JOINT_TORQUES: applies passive joint torques based on Davy 
 * and Audu (1987).*/

void apply_passive_joint_torques(SDModelStruct* sdm, double q[], double u[])
{

double passive_trq;


/****** Apply passive joint torques ******/

/* Pelvis  
   passive_trq = (10000.0*q[pelvis_rotation] + 2000.0*u[pelvis_rotation]);
   sdhinget(pelvis,2,-passive_trq);
*/

/* MTP joints */

   //passive_trq = (25.0*q[mtp_angle_r] + 0.3*u[mtp_angle_r]);
   //passive_trq = (25.0*q[mtp_angle_r] + 0.03*u[mtp_angle_r]);
   passive_trq = (50.0*q[mtp_angle_r] + 5.30*u[mtp_angle_r]);
   sdhinget(mtp_r,0,-passive_trq);
pssvJtrq[mtpJ_r]=-passive_trq;
pssvJtrqDamping[mtpJ_r]=-0.03*u[mtp_angle_r];

   //passive_trq = (25.0*q[mtp_angle_l] + 0.3*u[mtp_angle_l]);
   //passive_trq = (25.0*q[mtp_angle_l] + 0.03*u[mtp_angle_l]);
   passive_trq = (50.0*q[mtp_angle_l] + 5.30*u[mtp_angle_l]);
   sdhinget(mtp_l,0,-passive_trq);
pssvJtrq[mtpJ_l]=-passive_trq;
pssvJtrqDamping[mtpJ_l]=-0.03*u[mtp_angle_l];

/* Midfoot joints */

   //passive_trq = (750.0*q[midfoot_angle_r] + 0.50*u[midfoot_angle_r]);
   passive_trq = (750.0*q[midfoot_angle_r] + 0.050*u[midfoot_angle_r]);
   sdhinget(midfoot_r,0,-passive_trq);
pssvJtrq[midfootJ_r]=-passive_trq;
pssvJtrqDamping[midfootJ_r]=-0.050*u[midfoot_angle_r];

   //passive_trq = (750.0*q[midfoot_angle_l] + 0.50*u[midfoot_angle_l]);
   passive_trq = (750.0*q[midfoot_angle_l] + 0.050*u[midfoot_angle_l]);
   sdhinget(midfoot_l,0,-passive_trq);
pssvJtrq[midfootJ_l]=-passive_trq;
pssvJtrqDamping[midfootJ_l]=-0.050*u[midfoot_angle_l];
 


/* Ankle joints */

      
   //passive_trq = (2.0*exp(-5.0*(q[ankle_angle_r]+0.5236))
	 //         -9.0*exp(-5.0*(0.3491-q[ankle_angle_r]))
	//	  -0.943*u[ankle_angle_r]);
   passive_trq = (2.0*exp(-5.0*(q[ankle_angle_r]+0.5236))
	          -9.0*exp(-5.0*(0.3491-q[ankle_angle_r]))
		  -0.0943*u[ankle_angle_r]);
   sdhinget(ankle_r,0,passive_trq);
pssvJtrq[ankleJ_r]=passive_trq;
pssvJtrqDamping[ankleJ_r]=-0.0943*u[ankle_angle_r];

   //passive_trq = (2.0*exp(-5.0*(q[ankle_angle_l]+0.5236))
	 //         -9.0*exp(-5.0*(0.3491-q[ankle_angle_l]))
	//	  -0.943*u[ankle_angle_l]);
   passive_trq = (2.0*exp(-5.0*(q[ankle_angle_l]+0.5236))
	          -9.0*exp(-5.0*(0.3491-q[ankle_angle_l]))
		  -0.0943*u[ankle_angle_l]);
   sdhinget(ankle_l,0,passive_trq);
pssvJtrq[ankleJ_l]=passive_trq;
pssvJtrqDamping[ankleJ_l]=-0.0943*u[ankle_angle_l];

 
/* Knee joints */
  
   //passive_trq =  (3.1*exp(-5.9*(q[knee_angle_r]+1.92))
	 //         -10.5*exp(-11.8*(-0.1-q[knee_angle_r]))
	//	  -1.0*u[knee_angle_r]);
   passive_trq =  (3.1*exp(-5.9*(q[knee_angle_r]+1.92))
	          -10.5*exp(-11.8*(-0.1-q[knee_angle_r]))
		  -0.1*u[knee_angle_r]);
   sdhinget(knee_r,2,passive_trq);
pssvJtrq[kneeJ_r]=passive_trq;
pssvJtrqDamping[kneeJ_r]=-0.1*u[knee_angle_r];

   //passive_trq =  (3.1*exp(-5.9*(q[knee_angle_l]+1.92))
	 //         -10.5*exp(-11.8*(-0.1-q[knee_angle_l]))
	//	  -1.0*u[knee_angle_l]);
   passive_trq =  (3.1*exp(-5.9*(q[knee_angle_l]+1.92))
	          -10.5*exp(-11.8*(-0.1-q[knee_angle_l]))
		  -0.1*u[knee_angle_l]);
   sdhinget(knee_l,2,passive_trq);
pssvJtrq[kneeJ_l]=passive_trq;
pssvJtrqDamping[kneeJ_l]=-0.1*u[knee_angle_l];
 

/* Hip joints */
  
   //passive_trq = (2.6*exp(-5.8*(q[hip_angle_r]-0.1745))
	 //         -8.7*exp(-1.3*(1.92-q[hip_angle_r]))
	//	  -1.09*u[hip_angle_r]);
   passive_trq = (2.6*exp(-5.8*(q[hip_angle_r]-0.1745))
	          -8.7*exp(-1.3*(1.92-q[hip_angle_r]))
		  -0.109*u[hip_angle_r]);
   sdhinget(hip_r,0,passive_trq);
pssvJtrq[hipJ_r]=passive_trq;
pssvJtrqDamping[hipJ_r]=-0.109*u[hip_angle_r];

   //passive_trq = (2.6*exp(-5.8*(q[hip_angle_l]-0.1745))
	 //         -8.7*exp(-1.3*(1.92-q[hip_angle_l]))
	//	  -1.09*u[hip_angle_l]);
   passive_trq = (2.6*exp(-5.8*(q[hip_angle_l]-0.1745))
	          -8.7*exp(-1.3*(1.92-q[hip_angle_l]))
		  -0.109*u[hip_angle_l]);
   sdhinget(hip_l,0,passive_trq);
pssvJtrq[hipJ_l]=passive_trq;
pssvJtrqDamping[hipJ_l]=-0.109*u[hip_angle_l];
}


/* SDUMOTION: this routine specifies the prescribed motion for
 * fixed joints, which are implemented as prescribed pins.
 */

int sdumotion(double t, double q[], double u[])
{

   prescribe_fixed_joints(&sdm);

}



/* SDUFORCE: this routine is used to apply forces to the body segments.
 * By default, it contains only two routines: the one that applies muscle
 * forces and the one that applies joint restraint forces. If you want to
 * apply other external forces to the segments, you can add in your own
 * code here. If the forces or torques you want to apply have been
 * pre-calculated for each time step, you can use load_motion_data() to
 * read them in, then use apply_external_forces() to apply them (just like
 * with inverse dynamics).
 */

int sduforce(double t, double q[], double u[])
{
   apply_muscle_forces(&sdm);
 
   apply_ground_contact_forces(&sdm,q,u,t);
 
   apply_passive_joint_torques(&sdm,q,u);

}



/* INIT_MOTION: this routine turns off prescribed motion for all
 * unconstrained q's. It does not set the constrained q's because
 * in the SD/FAST system description file they were set with
 * "prescribed = 0".  It does not need to turn on prescribed motion
 * for [tree] weld joints, which are implemented as prescribed pins,
 * because they are set with "prescribed = 1" in the system description
 * file.
 */

static void init_motion(void)
{

   //set_prescribed_motion(&sdm,unconstrained_q,0);

}


/***************************************************************************/
/*  CALC_JOINT_TORQUES based on a routine by Lisa Schutte and Chris Raasch
 *  --> it calculates total joint torques applied during motion */

void calc_joint_torques(double t,double y[],double ydot[],double joint_torque[])

{
  double q[NQ],u[NU],udot[NU],mult[NC];
  double omega,pv[3];
  int i,rk,multmap[NC],err,routine;
  
  sdclearerr();

  for(i=0;i<NQ;i++)
  {
    q[i] = y[i];
    u[i] = y[NQ+i];
    udot[i] = ydot[NQ+i];
  }

  sdmult(mult,&rk,multmap);
  sdstate(t,q,u);
  apply_ground_contact_forces(&sdm,q,u,t);
  /*apply_passive_joint_torques(&sdm,q,u);*/
  sdfulltrq(udot,mult,joint_torque);
  

/* Calculate knee torque due to translational and patella forces.
 * pv[] is partial velocity of the knee trans. or patella DOF; the
 * joint force for that DOF is multiplied by each partial velocity to
 * get the torque at the knee instantaneous center of rotation (icr)
 */

  omega = y[knee_angle_r+NQ];
  if( EQUAL_WITHIN_ERROR(omega,0.0) )
    pv[0] = 0.0;
  else
  {
    /* knee tx and ty */
    pv[0] = u[knee_r_tx]/omega;
    pv[1] = u[knee_r_ty]/omega;
    joint_torque[knee_angle_r] += joint_torque[knee_r_tx]*pv[0] +
                                  joint_torque[knee_r_ty]*pv[1];
    /* patella */
    pv[0] = u[tib_pat_r_tx]/omega;
    pv[1] = u[tib_pat_r_ty]/omega;
    pv[2] = u[tib_pat_r_r3]/omega;
    joint_torque[knee_angle_r] += joint_torque[tib_pat_r_tx]*pv[0] + 
                                  joint_torque[tib_pat_r_ty]*pv[1] +
				  joint_torque[tib_pat_r_r3]*pv[2];
  }

  omega = y[knee_angle_l+NQ];
  if( EQUAL_WITHIN_ERROR(omega,0.0) )
    pv[0] = 0.0;
  else
  {
    /* knee tx and ty */
    pv[0] = u[knee_l_tx]/omega;
    pv[1] = u[knee_l_ty]/omega;
    joint_torque[knee_angle_l] += joint_torque[knee_l_tx]*pv[0] +
                                  joint_torque[knee_l_ty]*pv[1];
    /* patella */
    pv[0] = u[tib_pat_l_tx]/omega;
    pv[1] = u[tib_pat_l_ty]/omega;
    pv[2] = u[tib_pat_l_r3]/omega;
    joint_torque[knee_angle_l] += joint_torque[tib_pat_l_tx]*pv[0] + 
                                  joint_torque[tib_pat_l_ty]*pv[1] +
				  joint_torque[tib_pat_l_r3]*pv[2];
  }

  
  sderror(&routine,&err);  /*Check for errors */

  if(err!=0)
    printf("Error %2i occurred in routine %2i\n",err,routine);

  }

///////////////////////////////////////////////////////////////////////////////
/* APPLY_GROUND_CONTACT_FORCES: this routine applies the ground contact forces 
 * to the body foot segments using the 2D model developed in Calgary. 
 */

void apply_ground_contact_forces(SDModelStruct* sdm, double y[], double dy[], double t)
{

   int i,j;
   ShoeStruct* ss;

   double npos[3], gpos[3], gvel[3], force[3], tmp[3], forcel[3];
   double tngspd, frifac, frifa2, grfa, grfb, grfc, grfd, grfe, grff, grfg;
   double netgrf[6],cop[6],r_netgrf[3],l_netgrf[3],r_cop[3],l_cop[3];
   double forceR[3],forceL[3],copR[3],copL[3];
   double npos1[3];

   double gvel1[3],lvel[3];


/* Definitions of ground reaction force variables
 *      npos.....NCBF position of ground reaction point
 *      gpos.....global position of ground reaction point
 *      vel......global velocity of ground reaction point
 *      for......force on ground reaction point (in global coordinates)
 *      grfa,grfb,grfc....g.r.f. parameters (diff. for each g.r.point)
 *      grfd,grfe,grff, grfg the rest of the g.r.f. parameters:
 *             from G. Coles: thesis, see below.
 *      tngspd...tangent speed: magnitue of velocity parallel to surface
 *      frifac...friction factor 
 *      frifa2...alternate friction factor (constraint) 
 */

/* Loop through all the ground reaction points, index i is the ground
 * contact element number 
 */
 
//printf("%d\n",frameindex);

   for (i=0; i<sdm->num_shoe_elements; i++)
      {
      ss = &sdm->shoe_element[i];

for(j=0;j<3;j++){
	force[j]=0.0;
	gvel[j]=0.0;
}
ss->power=0.0;

/******* First, convert the various parameters for this ground contact element */
 
	 for (j=0; j<3; j++)
	    npos[j]=ss->coord[j];
         
	 grfa = ss->param[0];
	 grfb = ss->param[1];
	 grfc = 1.0*ss->param[2];
         grfd = ss->param[3];
         grfe = ss->param[4];
         grff = ss->param[5];
         grfg = ss->param[6];

/******* Where:
 *           grfa ... coefficient for velocity independent term
 *           grfb ... power on displacement in velocity independent term
 *           grfc ... coefficient for the velocity dependent term
 *           grfd ... power on displacement in velocity dependent term
 *           grfe ... power on velocity in velocity dependent term
 *           grff ... area normalization term (approx. area associated with 
 *                    ground contact point div. by 32.2 cm^2)
 *           grfg ... coeficient of friction (Coulomb) */
 

/******* Determine the the postion and velocity of the ground reaction 
 *	 point in the global coordinate system (ground = body -1) */
 
	 sdpos (ss->segment,npos,gpos); 
	 sdvel (ss->segment,npos,gvel);

/******* Only proceed with the force calculation if this contact point 
 *       penatrates the surface */

	 if (gpos[1] < 0.0) 
	    {

	    force[1] = 2.0*grff*(grfa*(pow(-gpos[1],grfb)) + 
		      (gvel[1]/fabs(gvel[1]))*grfc*(pow(-gpos[1],grfd))*(pow(fabs(gvel[1]),grfe)));

/********** if the force is positive, continue, else, go to next point */
 
	    if (force[1] > 0.0) 
	       {
	       tngspd = sqrt(pow(gvel[0],2)+pow(gvel[2],2));
               frifac = 3000.0*tngspd*2.0*grff;
               frifa2 = grfg*force[1];

	       if (frifac > frifa2) 
	          frifac = frifa2;
	    

	       if (tngspd < 0.0000001) 
	          frifac=0.0;
               else
	          frifac = -1.0*frifac/tngspd;
	    


	       force[0] = frifac*gvel[0];
	       force[2] = frifac*gvel[2];

/********** Now, convert forces into shoe frame and apply the force */

	       sdtrans(ground,force,ss->segment,forcel);  

		   if (ss->segment == calcn_r || ss->segment == calcn_l){
			   npos1[0]=npos[0];//+0.025;
			   npos1[1]=npos[1];//+0.015;      
			   npos1[2]=npos[2];//+0.00;	
		   }
		
		   else if (ss->segment == midfoot_r || ss->segment == midfoot_l){     
			   npos1[0]=npos[0];//+0.025;   
			   npos1[1]=npos[1];//+0.015;   
			   npos1[2]=npos[2];//+0.00;	
		   }
		
		   else if (ss->segment == toes_r){ 
			   npos1[0]=npos[0];//+0.0346; 
			   npos1[1]=npos[1];//+0.006;  
			   npos1[2]=npos[2];//-0.0175;	
		   }
		
		   else { //toes_l  
			   npos1[0]=npos[0];//+0.0346;     
			   npos1[1]=npos[1];//+0.006;
			   npos1[2]=npos[2];//+0.0175;
		   }


		   sdvel (ss->segment,npos1,gvel1);
		   sdtrans(ground,gvel1,ss->segment,lvel);

		   //sdpointf(ss->segment,npos1,forcel);

		   	
		   
		   if(ss->segment==calcn_r || ss->segment==midfoot_r || ss->segment==toes_r){
			    if(FRAME<=70*5 || 100*5<=FRAME){
				   sdpointf(ss->segment,npos1,forcel); //apply individual forces

				   ss->power=forcel[0]*lvel[0]+forcel[1]*lvel[1]+forcel[2]*lvel[2];
			   }
	
			   
			   else{
				   //force[0]=force[0]*0.01;
				   //force[1]=force[1]*0.01;
				   //force[2]=force[2]*0.01;
				   force[0]=force[0]*0.0;
				   force[1]=force[1]*0.0;
				   force[2]=force[2]*0.0;
				   sdtrans(ground,force,ss->segment,forcel); 
				   sdpointf(ss->segment,npos1,forcel);	
				   
				   ss->power=forcel[0]*lvel[0]+forcel[1]*lvel[1]+forcel[2]*lvel[2];
			   }
			   
		   }
		   if(ss->segment==calcn_l || ss->segment==midfoot_l || ss->segment==toes_l){
			   if(FRAME<=11*5 || (45*5<=FRAME && FRAME<=120*5) || (150*5 <= FRAME)){
				   sdpointf(ss->segment,npos1,forcel); //apply individual forces

				   ss->power=forcel[0]*lvel[0]+forcel[1]*lvel[1]+forcel[2]*lvel[2];
			   }
			   
			   else{
				   //force[0]=force[0]*0.01;
				   //force[1]=force[1]*0.01;
				   //force[2]=force[2]*0.01;
				   force[0]=force[0]*0.0;
				   force[1]=force[1]*0.0;
				   force[2]=force[2]*0.0;
				   sdtrans(ground,force,ss->segment,forcel); 
				   sdpointf(ss->segment,npos1,forcel);	
				   
				   ss->power=forcel[0]*lvel[0]+forcel[1]*lvel[1]+forcel[2]*lvel[2];
			   }
			   
		   }
		   
		   
		   

	       }

	    else
	       {
	       force[0]=0.0;
	       force[1]=0.0;
	       force[2]=0.0;
	       }
	    }
         else
	    {               
	    force[0]=0.0;
	    force[1]=0.0;
	    force[2]=0.0;
	    }

	 if (pow(force[0],2) > 0.00000001) 
	    ss->grf[0]=force[0];
	 else
	    ss->grf[0]=0.0;
	 
	 if (pow(force[1],2) > 0.00000001) 
	    ss->grf[1]=force[1];
	 else
	    ss->grf[1]=0.0;
	 
	 if (pow(force[2],2) > 0.00000001) 
	    ss->grf[2]=force[2];
	 else
	    ss->grf[2]=0.0;
	 
	 ss->deformation=gpos[1];



      } /* end of shoe element loop */


	// compute net GRF at cop
	calc_net_grfs(y, dy, netgrf, cop);

	for(i=0;i<3;i++){
		rCOPglobal[i]=cop[i];
		lCOPglobal[i]=cop[i+3];
		rGRFglobal[i]=netgrf[i];
		lGRFglobal[i]=netgrf[i+3];
	}

	// Apply net GRF at cop
	//sdpointf(calcn_r,r_cop,r_netgrf);
	//sdpointf(calcn_l,l_cop,l_netgrf);



} /* end of routine */
/////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/* CALC_NET_GRFS: this routine computes the net ground reaction forces and 
   center-of-pressure (cop). */

// cop is in the global frame for graphics purpose
void calc_net_grfs(double y[], double dy[], double *netgrf, double *cop)  
{
   int i,j;
   double sum[6];
   ShoeStruct* ss;


    double npos[3],gpos[3];
	double CalcComPosR[3],CalcComPosL[3];
	double MglobalR[3], MglobalL[3];
	double COPglobalR[3], COPglobalL[3], GRFglobalR[3],GRFglobalL[3];
	
	double CopCalcComR[3],CopCalcComL[3];

	double dummy[3];



     
	for (j=0; j<6; j++){         
		netgrf[j]=0.0;        
		sum[j]=0.0;       
		cop[j]=0.0;	 
	}

	for(j=0;j<3;j++){		  
		GRFglobalR[j]=0.0;	 
		GRFglobalL[j]=0.0;
	}

      
	///// Compute the net grf (right)
	for (i=0; i<sdm.num_shoe_elements/2; i++){		
		ss = &sdm.shoe_element[i];		
		for(j=0;j<3;j++){
			GRFglobalR[j]=GRFglobalR[j]+ss->grf[j];		
		}
	}

	///// Compute the net grf (left)
	for (i=sdm.num_shoe_elements/2; i<sdm.num_shoe_elements; i++){		
		ss = &sdm.shoe_element[i];		
		for(j=0;j<3;j++){
			GRFglobalL[j]=GRFglobalL[j]+ss->grf[j];		
		}
	}

	///// Compute the net moment (right)
	for(j=0;j<3;j++){
		MglobalR[j]=0.0;
	}
	for (i=0; i<sdm.num_shoe_elements/2; i++){		
		ss = &sdm.shoe_element[i];	
		for(j=0;j<3;j++){
			npos[j]=ss->coord[j];		
		}
		sdpos (ss->segment,npos,gpos);
	
		MglobalR[0]=MglobalR[0]+gpos[1]*(ss->grf[2])-gpos[2]*(ss->grf[1]);
		MglobalR[1]=MglobalR[1]+gpos[2]*(ss->grf[0])-gpos[0]*(ss->grf[2]);
		MglobalR[2]=MglobalR[2]+gpos[0]*(ss->grf[1])-gpos[1]*(ss->grf[0]);
	
	}

	///// Compute the net moment (left)
	for(j=0;j<3;j++){
		MglobalL[j]=0.0;
	}
	for (i=sdm.num_shoe_elements/2; i<sdm.num_shoe_elements; i++){		
		ss = &sdm.shoe_element[i];	
		for(j=0;j<3;j++){
			npos[j]=ss->coord[j];		
		}
		sdpos (ss->segment,npos,gpos);
	
		MglobalL[0]=MglobalL[0]+gpos[1]*(ss->grf[2])-gpos[2]*(ss->grf[1]);
		MglobalL[1]=MglobalL[1]+gpos[2]*(ss->grf[0])-gpos[0]*(ss->grf[2]);
		MglobalL[2]=MglobalL[2]+gpos[0]*(ss->grf[1])-gpos[1]*(ss->grf[0]);
//if(ss->grf[2]>0.0000001)
//printf("%f \n",gpos[2]);	
	}
//printf("%f %f \n",MglobalL[0], MglobalL[1]);
	/////////////////////////////////////////////////
	// Compute COP (Assume 0 for vertical position)
	//###########################
	//Equations are
	//M0=P1*F2-P2*F1 (around a/p axis)
	//M1=P2*F0-P0*F2 (around vertical axis)
	//M2=P0*F1-P1*F0 (around m/l axis)
	//Assume, P1=0, P0=M2/F1, P2=-M0/F1

	//In this  model, 0:ML, 1:AP, 2: vertical,so
	//M0=P1*F2-P2*F1 (around m/l axis)
	//M1=P2*F0-P0*F2 (around a/p axis)
	//M2=P0*F1-P1*F0 (around vertical axis)
	//so, P2=0, P0=-M1/F2, P1=M0/F2
	//###########################

	// right side
	if(pow(GRFglobalR[1],2) > 0.00000001) {
		COPglobalR[0]=MglobalR[2]/GRFglobalR[1];
		COPglobalR[1]=0.0;
		COPglobalR[2]=-MglobalR[0]/GRFglobalR[1];
		
	}
	else{
		for(i=0;i<3;i++){
			COPglobalR[i]=0.0;
		}
	}

	//left side
	if(pow(GRFglobalL[1],2) > 0.00000001) {
		COPglobalL[0]=MglobalL[2]/GRFglobalL[1];
		COPglobalL[1]=0.0;
		COPglobalL[2]=-MglobalL[0]/GRFglobalL[1];
		
	}
	else{
		for(i=0;i<3;i++){
			COPglobalL[i]=0.0;
		}
	}


	///for checking for M1
	//printf("%f %f\n",COPglobalR[2]*GRFglobalR[0]-COPglobalR[0]*GRFglobalR[2]-MglobalR[1],
	//				 COPglobalL[2]*GRFglobalL[0]-COPglobalL[0]*GRFglobalL[2]-MglobalL[1]);


/*
	// compute COP in local body frame
	for(i=0;i<3;i++){
		dummy[i]=0.0;
	}
	
	sdpos (calcn_r,dummy,CalcComPosR); //calcR COM position in global frame
	sdpos (calcn_l,dummy,CalcComPosL); //calcL COM position in global frame

	for(i=0;i<3;i++){
		CopCalcComR[i]=COPglobalR[i]-CalcComPosR[i]; //COP position wrt calcR COM in global frame
		CopCalcComL[i]=COPglobalL[i]-CalcComPosL[i]; //COP position wrt calcL COM in global frame
	}

	sdtrans(ground,CopCalcComR,calcn_r,CopCalcComR); //COP position wrt calnR COM in calcR frame
	sdtrans(ground,CopCalcComL,calcn_l,CopCalcComL); //COP position wrt calnL COM in calcL frame
		
		
	CopCalcOrgR[0]=CopCalcComR[0]-Rcalc_Com_to_Origin0; //COP position wrt calnR orgin in calcR frame
	CopCalcOrgR[1]=CopCalcComR[1]-Rcalc_Com_to_Origin1;
	CopCalcOrgR[2]=CopCalcComR[2]-Rcalc_Com_to_Origin2;

	CopCalcOrgL[0]=CopCalcComL[0]-Lcalc_Com_to_Origin0; //COP position wrt calnL orgin in calcL frame
	CopCalcOrgL[1]=CopCalcComL[1]-Lcalc_Com_to_Origin1;
	CopCalcOrgL[2]=CopCalcComL[2]-Lcalc_Com_to_Origin2;
*/

	//// return GRF and COP in ground frame 
	for(j=0;j<3;j++){
		netgrf[j]=GRFglobalR[j];
		//cop[j]=CopCalcOrgR[j];
		cop[j]=COPglobalR[j];
		netgrf[j+3]=GRFglobalL[j];
		//cop[j+3]=CopCalcOrgL[j];
		cop[j+3]=COPglobalL[j];
	}



}////// end of the routine //////////
/*
void calc_net_grfs(double y[], double dy[], double *netgrf, double *cop)  
{
   int i,j;
   double sum[6];
   ShoeStruct* ss;

	double GRFglobal[3],GRFlocal[3];

   // Sum GRF forces over all right and left shoe elements 

      for (j=0; j<6; j++)
         {
         netgrf[j]=0.0;
         sum[j]=0.0;
         cop[j]=0.0;
	 }

	  for(j=0;j<3;j++){
		  GRFglobal[j]=0.0;
	  }

      /// Compute the net grf 
      for (i=0; i<sdm.num_shoe_elements; i++)
	  {
		ss = &sdm.shoe_element[i];

		for(j=0;j<3;j++){
			GRFlocal[j]=0.0;
			GRFglobal[j]=ss->grf[j];
		}
		sdtrans(ground,GRFglobal,ss->segment,GRFlocal);

		if (ss->segment == calcn_r)
	    {
			netgrf[0] += GRFlocal[0];
	  		netgrf[1] += GRFlocal[1];
	  		netgrf[2] += GRFlocal[2];

			sum[0] += ss->coord[0]*GRFlocal[1];
	  		sum[1] += ss->coord[1]*GRFlocal[0];
	    }

		if (ss->segment == calcn_l)
	    {
			netgrf[3] += GRFlocal[0];
	  		netgrf[4] += GRFlocal[1];
	  		netgrf[5] += GRFlocal[2];

			sum[3] += ss->coord[0]*GRFlocal[1];
	  		sum[4] += ss->coord[1]*GRFlocal[0];
	    }

	  }

   /// Compute the center of pressure for calcn_r 

	  if(netgrf[1] == 0.0)
	     cop[0] = 0.0;
	  else
	     cop[0] = sum[0]/netgrf[1];

	  if(netgrf[0] == 0.0)
	     cop[1] = 0.0;
	  else
	     cop[1] = sum[1]/netgrf[0];

   /// Compute the center of pressure for calcn_l 

	  if(netgrf[4] == 0.0)
	     cop[3] = 0.0;
	  else
	     cop[3] = sum[3]/netgrf[4];

	  if(netgrf[3] == 0.0)
	     cop[4] = 0.0;
	  else
	     cop[4] = sum[4]/netgrf[3];

}/// end of the routine 
*/

/////////////////////////////////////////////////////////////////////////////////


/*************************************************************************/
/*  CALC_JOINT_POWERS  --> it calculates sagittal plane total joint powers 
    applied during motion */

void calc_joint_powers(double joint_torque[], double joint_power[])

{
  
  double temp1[3], temp2[3], temp3[3], temp4[3];
  double hipavel, kneavel, ankavel;
  int err,routine;

  /* Right Leg */

      sdangvel(femur_r,temp1);            
      sdangvel(tibia_r,temp2);
      sdangvel(talus_r,temp3);
      sdangvel(pelvis,temp4);

      hipavel=temp1[2]-temp4[2];
      kneavel=temp2[2]-temp1[2];
      ankavel=temp3[2]-temp2[2];

      joint_power[hip_angle_r] = joint_torque[hip_angle_r]*hipavel;
      joint_power[knee_angle_r] = joint_torque[knee_angle_r]*kneavel;
      joint_power[ankle_angle_r] = joint_torque[ankle_angle_r]*ankavel;


  /* Left Leg */

      sdangvel(femur_l,temp1);            
      sdangvel(tibia_l,temp2);
      sdangvel(talus_l,temp3);
 
      hipavel=temp1[2]-temp4[2];
      kneavel=temp2[2]-temp1[2];
      ankavel=temp3[2]-temp2[2];

      joint_power[hip_angle_l] = joint_torque[hip_angle_l]*hipavel;
      joint_power[knee_angle_l] = joint_torque[knee_angle_l]*kneavel;
      joint_power[ankle_angle_l] = joint_torque[ankle_angle_l]*ankavel;

  
  sderror(&routine,&err);  /*Check for errors */

  if(err!=0)
    printf("Error %2i occurred in routine %2i\n",err,routine);

}


/***************************************************************************/

/***************************************************************************/
/*  SEGMENT_ME  --> it calculates the mechanical energy of individual segments */

void segment_me(double segme[], double inertia[], double mass[],
         	    double vel[][3], double angvel[], double pos[])
{
   int i;
   double mep[NBOD];
   double gravity=9.80665, me[NBOD], mer[NBOD], meh[NBOD], mev[NBOD];


   /* For each body, compute the segment me (BJ's Dissertation pg.85) */

   for(i=0;i<NBOD;i++)
      {
      me[i] = 0.5*inertia[i]*angvel[i]*angvel[i]
                   + 0.5*mass[i]*vel[i][0]*vel[i][0]
                   + 0.5*mass[i]*vel[i][1]*vel[i][1]
                   + mass[i]*gravity*pos[i];

      mer[i] = 0.5*inertia[i]*angvel[i]*angvel[i]; 

      mep[i] = mass[i]*gravity*pos[i];;

      meh[i] = 0.5*mass[i]*vel[i][0]*vel[i][0];

      mev[i] = 0.5*mass[i]*vel[i][1]*vel[i][1];
      }
   
/* Sum segment ME components of interest for the segments */
 
/* pelvis, right leg, left leg */

      segme[0] = me[pelvis];                  
      segme[1] = me[femur_r]+me[tibia_r]+me[patella_r]+me[talus_r]+me[calcn_r];
      segme[2] = me[femur_l]+me[tibia_l]+me[patella_l]+me[talus_l]+me[calcn_l];                       


/* break the TRUNK segment ME components up */

    /* trunk potential */
	segme[3] = mep[pelvis];

    /* trunk rotational */
	segme[4] = mer[pelvis];

    /* trunk horizontal */
	segme[5] = meh[pelvis];

    /* trunk vertical */
	segme[6] = mev[pelvis];


/* break the THIGH segment ME components up */

    /* thigh potential */
	segme[7] = mep[femur_l];

    /* thigh rotational */
	segme[8] = mer[femur_l];

    /* thigh horizontal */
	segme[9] = meh[femur_l];

    /* thigh vertical */
	segme[10] = mev[femur_l];


/* break the SHANK segment ME components up */

    /* shank potential */
	segme[11] = mep[tibia_l]+mep[patella_l];

    /* shank rotational */
	segme[12] = mer[tibia_l]+mer[patella_l];

    /* shank horizontal */
	segme[13] = meh[tibia_l]+meh[patella_l];

    /* shank vertical */
	segme[14] = mev[tibia_l]+mev[patella_l];


/* break the FOOT segment ME components up */

    /* foot potential */
	segme[15] = mep[talus_l]+mep[calcn_l];

    /* foot rotational */
	segme[16] = mer[talus_l]+mer[calcn_l];

    /* foot horizontal */
	segme[17] = meh[talus_l]+meh[calcn_l];

    /* foot vertical */
	segme[18] = mev[talus_l]+mev[calcn_l];


/* Write the Contralateral leg segment PE and KE*/

    /* Thigh PE */
	segme[19] = mep[femur_r];

    /* Thigh KE */
	segme[20] = mer[femur_r]+meh[femur_r]+mev[femur_r];

    /* Shank PE */
	segme[21] = mep[tibia_r]+mep[patella_r];

    /* Shank KE */
	segme[22] = mer[tibia_r]+meh[tibia_r]+mev[tibia_r]+mer[patella_r]+meh[patella_r]+mev[patella_r];

    /* Foot PE */
	segme[23] = mep[talus_r]+mep[calcn_r];

    /* Foot KE */
	segme[24] = mer[talus_r]+meh[talus_r]+mev[talus_r]+mer[calcn_r]+meh[calcn_r]+mev[calcn_r];

}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
/////////////Compute activation by integrating excitation//////////////////
void excitation_integ(double **XR,double **YR, double **XL,double **YL)
{
	///////////parameters for integration of excitation//////////////
	int i, j;
	int err1,which1;
	int neq1=1;
	double t1=0.0,dt1=0.01, tol1=0.000001;
	double act[1],dact[1], work1[6], param1[1], step;
	double m_act[N_mus*2];

	//for checking patterns
	//double act_patternXR[N_mus][N_emg_step];
	//double act_patternYR[N_mus][N_emg_step];
	//double act_patternXL[N_mus][N_emg_step];
	//double act_patternYL[N_mus][N_emg_step];

	MuscleStruct* ms;


	/////////////////////////////////////////////////////////////////
	for(i=0;i<6;i++) work1[i]=0.0;

	/////////////////////////
	//// Right leg
	/////////////////////////
	for(j=0;j<N_mus;j++){
		ms = &sdm.muscles[j];
		//if(PATTERN==EMG){
			c1 = get_muscle_param_value(ms,"activation1"); //timescaled
			c2 = get_muscle_param_value(ms,"activation2");	
		//}
		//else{//block
		//	c1=0.4615;//timescaled, corresponding to 50msec
		//	c2=1.5385;//timescaled, corresponding to 65msec
		//}
		
		act[0]=YR[j][0];
		dact[0]=0.0;
		t1=0.0;
		for(i=1;i<N_emg_step;i++){
			dt1=(XR[j][i]-XR[j][i-1])*MAX_TIME/MAX_X; //convert from leg-tx to time
			param1[0]=YR[j][i];

			sdvinteg(calc_act,&t1,act,dact,param1,dt1,
			&step,neq1,tol1,work1,&err1,&which1);

			if(i==N_emg_step-1)
				m_act[j]=act[0];

			//for checking patterns
			//act_patternXR[j][i] = XR[j][i];
			//act_patternYR[j][i] = act[0];
			//mdata.mus_actR[j][i]=act[0]; //to save
		}
	}//for right leg

	///////////////////////////////
	//// Left leg
	///////////////////////////////
	for(i=0;i<6;i++) work1[i]=0.0;
	
	for(j=N_mus;j<2*N_mus;j++){
		ms = &sdm.muscles[j];
		//if(PATTERN==EMG){
			c1 = get_muscle_param_value(ms,"activation1"); //timescaled
			c2 = get_muscle_param_value(ms,"activation2");	
		//}
		
		//else{//block
		//	c1=0.4615;//timescaled, corresponding to 50msec
		//	c2=1.5385;//timescaled, corresponding to 65msec
		//}

		act[0]=YL[j-N_mus][0];
		dact[0]=0.0;
		t1=0.0;
		for(i=1;i<N_emg_step;i++){

			dt1=(XL[j-N_mus][i]-XL[j-N_mus][i-1])*MAX_TIME/MAX_X; //convert from leg-tx to time
			param1[0]=YL[j-N_mus][i];

			sdvinteg(calc_act,&t1,act,dact,param1,dt1,
			&step,neq1,tol1,work1,&err1,&which1);

			if(i==N_emg_step-1)
				m_act[j]=act[0];

			//for checking patterns
			//act_patternXL[j][i] = XL[j][i];
			//act_patternYL[j][i] = act[0];
			//mdata.mus_actL[j][i]=act[0]; //to save
		}
	}//for left leg

	//assign initial activation values
	for (j=0; j<sdm.num_muscles; j++){	
		sdm.muscles[j].dynamic_activation = m_act[j];
		sdm.muscles[j].init_act = m_act[j];
	//printf("m_act=%lf\n",m_act[j]);
	}

	//assign initial values
	
	//for(j=0;j<N_mus;j++){
	//	mdata.mus_actR[j][0]=m_act[j];
	//	mdata.mus_actL[j][0]=m_act[j+N_mus];
	//}
	


}
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
/////////////For excitation integration using sdvinteg///////////////////
void calc_act(double time1, double act[], double dact[], double param1[],
				  int *status)
{
	double exc;

	exc=param1[0];

	if (exc > act[0])
   		dact[0]=10.0*(exc-act[0])*(c1*exc+c2); // 10 is for de-timescale
	else
   		dact[0]=10.0*(exc-act[0])*c2;
	
}
////////////////////////////////////////////////////////////////////////


static void print_simulation_info(double t, double y[], double system_energy)
{

   int i;

   /* Write Qs to stdout to show progress of simulation */

   if (verbose == yes)
   {
      printf("t=%6.4lf | ", t);
      for (i=0; i<sdm.nq; i++)
	 if (sdm.q[i].type == unconstrained_q)
	    printf("%8.3lf ", y[i] * sdm.q[i].conversion);
      printf("  sys_energy= %10.3lf\n", system_energy);
   }
   else
   {
      printf("\rt = %6.4lf", t);
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


  
////////////////////////////UMBERGER METABOLIC COST///////////////////////////////////////////////////
double calc_metabolic_cost(MuscleStruct* ms){

	int i;
	double total_energy;//total energy used by an individual muscle
	double A, Aam, As;
	double alphas_st, alphas_ft, alphal;
	double S;
	double Vce_bar, Vce_bar_maxft, Vce_bar_maxst;
	double Ham, Hsl, Hmw;//activation-maintenance, shortening-lengthening, and mechanical work heat rates
	double st_temp1, st_temp2;
	double norm_length, pennation_angle, ca,pssv_force;
	double all_mus_energy;
	double Fiso;//normalize Force to maximum isometric force
	all_mus_energy = 0.0;

	//////////////Cycle through all of the muscles and calculate the energy liberated in each one (W).////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//for(i=0;i<sdm.num_muscles; i++){ //right side
	//for(i=N_mus; i<N_mus*2; i++){ //left side	
	for(i=0; i<N_mus*2; i++){ //both legs
		ms = &sdm.muscles[i];

		//reset the energy variable
		total_energy =0.0;
		Fiso = 0.0;
		
		/////////////////////////////////////////////////////////////////////////////////////
		/////Calculate the values of the variables in the metabolic energy equation//////////
		/////////////////////////////////////////////////////////////////////////////////////
		//Determine A
		if(sdm.muscles[i].excitation_level > sdm.muscles[i].dynamic_activation)//excitation greater than activation
			A = sdm.muscles[i].excitation_level;
		else
			A = (sdm.muscles[i].excitation_level + sdm.muscles[i].dynamic_activation)/2;
		
		//set the other values related to A
		Aam = pow(A,.6);
		As = pow(A,2.0);
	//	sdm.muscles[i].TestVar=Aam;
		//Set the Value of S (1.0 anaerobic, 1.5 aerobic)
		S = 1.5;
		
		//Determine the value for the normalized contraction velocities
		Vce_bar_maxft = 12.0;//maximum fiber velocity = 10Xoptimal fiber length(used 12 in Umberger model)
		Vce_bar_maxst = Vce_bar_maxft/2.5;//slow twitch is the fast twitch value / 2.5
		Vce_bar = sdm.muscles[i].fiber_velocity/sdm.muscles[i].lce_opt; //current normalized velocity
		
		//Determine the values for the alpha variables
		alphas_st = 100/Vce_bar_maxst;
		alphas_ft = 153/Vce_bar_maxft;
		alphal = 0.3 * alphas_st;
		
		//Determine the value for Ham, Hmw
		if(sdm.muscles[i].excitation_level < sdm.muscles[i].percent_st){
			Ham = 25*Aam;
		}
		else{
			Ham = (25 * pow(sdm.muscles[i].percent_st,0.6)) + (153 * (sdm.muscles[i].excitation_level - sdm.muscles[i].percent_st));
		}

//		Ham = 1.28*sdm.muscles[i].percent_ft*100.0 + 25;// (W/kg)
//		Hmw = sdm.muscles[i].force*sdm.muscles[i].fiber_velocity/sdm.muscles[i].mus_mass;//(W/kg)
	  
	   norm_length = (ms->fiber_length) / (*(ms->optimal_fiber_length));
	   pennation_angle = calc_pennation(norm_length,1.0,(*(ms->pennation_angle)));
       ca = cos(DEG_TO_RAD*pennation_angle);


	   pssv_force=(*(ms->max_isometric_force)*calc_nonzero_passive_force(ms,norm_length,0.0));
	   if(pssv_force <0.0){
		   pssv_force = 0.0;
	   }

	   //fib_pwr[i][j-N_mus]=ca*ca*(sdm.muscles[j].force/ca - pssv_force)*sdm.muscles[j].fiber_velocity; //ca*ca is for force and velocity
       Hmw=((sdm.muscles[i].force/ca - pssv_force)*sdm.muscles[i].fiber_velocity)/sdm.muscles[i].mus_mass;//(W/kg)
		//printf("Test Muscle %i\n",i);
		//getchar();


		
		////////////////////////////////////////////////
		///////Calculate the metabolic cost/////////////
		////////////////////////////////////////////////

		//Determine the amount of energy liberated by the slowtwitch fibers
	   st_temp1 = alphas_st*Vce_bar;//sdm.muscles[i].percent_st;
		st_temp2 = alphas_st*Vce_bar_maxst;//*sdm.muscles[i].percent_st;

		if(-st_temp1 > st_temp2)//energy released can not be greater than the maximum possible for slow twitch fibers
			st_temp1 = -st_temp2;
		
		//Determine which side of the optimum muscle length the muscle is operating on and perform energy calculation
		if(sdm.muscles[i].fiber_length <= sdm.muscles[i].lce_opt){//if contractile element length is less than the optimal length
			if(sdm.muscles[i].fiber_velocity <= 0){//shortening
				if(sdm.muscles[i].excitation_level < sdm.muscles[i].percent_st){
						Hsl = (-st_temp1)*As*S;
				}
				else{
						Hsl = ((-st_temp1)*pow(sdm.muscles[i].percent_st,2.0)*S) - (alphas_ft* Vce_bar*sdm.muscles[i].percent_ft*pow((sdm.muscles[i].excitation_level - sdm.muscles[i].percent_st),2.0))*S;
				
				}
				
			}
		    else{//lengthening
				Hsl = alphal*A*S*Vce_bar;
				Hmw=Hmw*0.0;  //-0.3;
			}	
			
		//	if(sdm.muscles[i].fiber_velocity <= 0){//shortening
		//		Hsl = ((-st_temp1) -( alphas_ft* Vce_bar*  (sdm.muscles[i].percent_ft)))   *As*S;
		//	}
		//	else{//lengthening
		//		Hsl = alphal*A*S*Vce_bar;
		//		Hmw=Hmw*0;
		//	}
			total_energy = Ham*S + Hsl - Hmw;//(W/kg)
			//total_energy = Ham*Aam*S + Hsl - Hmw;//(W/kg)
			sdm.muscles[i].Ham = Ham*S;//(W/kg)
			//sdm.muscles[i].Ham = Ham*Aam*S;//(W/kg)
			sdm.muscles[i].Hsl = Hsl;//(W/kg)
			sdm.muscles[i].Hmw = -Hmw;//(W/kg)
		}
		else{//contractile element length is greater than optimal length
			Fiso = calc_active_force(ms,sdm.muscles[i].fiber_length/sdm.muscles[i].lce_opt);//determines the ratio of force to active force based on the force-length curve only
			
			if(sdm.muscles[i].fiber_velocity <= 0){//shortening
				if(sdm.muscles[i].excitation_level < sdm.muscles[i].percent_st){
						Hsl = (-st_temp1)*As*S*Fiso;
				}
				else{
						Hsl = ((-st_temp1)*pow(sdm.muscles[i].percent_st,2.0)*S*Fiso) - (alphas_ft* Vce_bar*sdm.muscles[i].percent_ft* pow((sdm.muscles[i].excitation_level - sdm.muscles[i].percent_st),2.0))*S*Fiso;
				
				}
			}
			else{//lengthening
				Hsl = alphal*A*S*Vce_bar*Fiso;
				Hmw=Hmw*0.0; // -0.3;
			}
			
			
			
			
		//	if(sdm.muscles[i].fiber_velocity <= 0){//shortening
		//		Hsl = -(st_temp1+(alphas_ft*Vce_bar*sdm.muscles[i].percent_ft))*As*S*Fiso;
		//	}
		//	else{//lengthening
		//		Hsl = alphal*A*S*Vce_bar*Fiso;
		//		Hmw=Hmw*0;
		//	}
			total_energy = (0.4*Ham+ 0.6 *Ham*Fiso)*S + Hsl - Hmw;//(W/kg)
		//	total_energy = (0.4*Ham+ 0.6 *Ham*Fiso)*Aam*S + Hsl - Hmw;//(W/kg)
			sdm.muscles[i].Ham = Ham*S;//(W/kg)
		//	sdm.muscles[i].Ham = Ham*Aam*S;//(W/kg)
			sdm.muscles[i].Hsl = Hsl;//(W/kg)
			sdm.muscles[i].Hmw = -Hmw;//(W/kg)
		}

		//If the total energy is less than one, make it equal to one (resting rate of muscle)
		if(total_energy < 1.0)
			total_energy = 1.0; //resting heat rate (W/kg)
		
		//sum up the energy for all of the muscles
		all_mus_energy = all_mus_energy + total_energy*sdm.muscles[i].mus_mass; //(W)
		sdm.muscles[i].total_energy=total_energy*sdm.muscles[i].mus_mass; //(W)
		//printf("mus_mass=%f  ",sdm.muscles[i].mus_mass);
//		printf("allE=%f  ",all_mus_energy);
//		getchar();
//		sdm.muscles[i].Ham=sdm.muscles[i].Ham*sdm.muscles[i].mus_mass; //(W)
//		sdm.muscles[i].Hsl=sdm.muscles[i].Hsl*sdm.muscles[i].mus_mass; //(W)
//		sdm.muscles[i].Hmw=sdm.muscles[i].Hmw*sdm.muscles[i].mus_mass; //(W)



	}//end muscle loop
//	printf("E=%f\n",all_mus_energy);
//	getchar();
	return all_mus_energy;	
		
}

void init_met_values() {
	int i;

		//Initialize the optimal fiber lengths (from Muscle file)
	sdm.muscles[0].lce_opt = sdm.muscles[25].lce_opt = .030;// SOL
	sdm.muscles[1].lce_opt = sdm.muscles[26].lce_opt = .045;// MGAS
	sdm.muscles[2].lce_opt = sdm.muscles[27].lce_opt = .080;// SM
	sdm.muscles[3].lce_opt = sdm.muscles[28].lce_opt = .173;// BFsh
	sdm.muscles[4].lce_opt = sdm.muscles[29].lce_opt = .104;// PSOAS
	sdm.muscles[5].lce_opt = sdm.muscles[30].lce_opt = .098;// TA
	sdm.muscles[6].lce_opt = sdm.muscles[31].lce_opt = .084;// RF
	sdm.muscles[7].lce_opt = sdm.muscles[32].lce_opt = .087;// VAS_1
	sdm.muscles[8].lce_opt = sdm.muscles[33].lce_opt = .144;// GMAX
	sdm.muscles[9].lce_opt = sdm.muscles[34].lce_opt = .0353;// GMED_a 
	sdm.muscles[10].lce_opt = sdm.muscles[35].lce_opt = .049;// PER_Long
	sdm.muscles[11].lce_opt = sdm.muscles[36].lce_opt = .031;// TP
	sdm.muscles[12].lce_opt = sdm.muscles[37].lce_opt = .034;// FDL
	sdm.muscles[13].lce_opt = sdm.muscles[38].lce_opt = .043;// FLX_Hal
	sdm.muscles[14].lce_opt = sdm.muscles[39].lce_opt = .064;// LGAS
	sdm.muscles[15].lce_opt = sdm.muscles[40].lce_opt = .109;// BFlh
	sdm.muscles[16].lce_opt = sdm.muscles[41].lce_opt = .100;// IL
	sdm.muscles[17].lce_opt = sdm.muscles[42].lce_opt = .079;// PertTert
	sdm.muscles[18].lce_opt = sdm.muscles[43].lce_opt = .102;// EDL
	sdm.muscles[19].lce_opt = sdm.muscles[44].lce_opt = .111;// EXT_Hal
	sdm.muscles[20].lce_opt = sdm.muscles[45].lce_opt = .087;// VAS_2
	sdm.muscles[21].lce_opt = sdm.muscles[46].lce_opt = .087;// VAS_3
	sdm.muscles[22].lce_opt = sdm.muscles[47].lce_opt = .131;// AddMag
	sdm.muscles[23].lce_opt = sdm.muscles[48].lce_opt = .0646;// GMED_p
	sdm.muscles[24].lce_opt = sdm.muscles[49].lce_opt = .050;// PerBrev


		//Initialize the percent_ft fibers (From Johnson et al, 1973)
	sdm.muscles[0].percent_ft = sdm.muscles[25].percent_ft = .123;// SOL
	sdm.muscles[1].percent_ft = sdm.muscles[26].percent_ft = .492;// MGAS
	sdm.muscles[2].percent_ft = sdm.muscles[27].percent_ft = .331;// SM -- no data, used value for BFlh
	sdm.muscles[3].percent_ft = sdm.muscles[28].percent_ft = .331;// BFsh -- no data, used value for BFlh
	sdm.muscles[4].percent_ft = sdm.muscles[29].percent_ft = .508;// PSOAS
	sdm.muscles[5].percent_ft = sdm.muscles[30].percent_ft = .270;// TA
	sdm.muscles[6].percent_ft = sdm.muscles[31].percent_ft = .620;// RF
	sdm.muscles[7].percent_ft = sdm.muscles[32].percent_ft = .538;// VAS_1
	sdm.muscles[8].percent_ft = sdm.muscles[33].percent_ft = .476;// GMAX
	sdm.muscles[9].percent_ft = sdm.muscles[34].percent_ft = .476;// GMED_a -- no data, used value for GMAX
	sdm.muscles[10].percent_ft = sdm.muscles[35].percent_ft = .375;// PER_Long
	sdm.muscles[11].percent_ft = sdm.muscles[36].percent_ft = .40;// TP -- no data, value from Umberger 2005
	sdm.muscles[12].percent_ft = sdm.muscles[37].percent_ft = .527;// FDL
	sdm.muscles[13].percent_ft = sdm.muscles[38].percent_ft = .527;// FLX_Hal -- no data, used value for FDL
	sdm.muscles[14].percent_ft = sdm.muscles[39].percent_ft = .531;// LGAS
	sdm.muscles[15].percent_ft = sdm.muscles[40].percent_ft = .331;// BFlh
	sdm.muscles[16].percent_ft = sdm.muscles[41].percent_ft = .508;// IL
	sdm.muscles[17].percent_ft = sdm.muscles[42].percent_ft = .375;// PertTert -- no data, used value for PER_Long
	sdm.muscles[18].percent_ft = sdm.muscles[43].percent_ft = .547;// EDL
	sdm.muscles[19].percent_ft = sdm.muscles[44].percent_ft = .547;// EXT_Hal -- no data, used value for EDL
	sdm.muscles[20].percent_ft = sdm.muscles[45].percent_ft = .538;// VAS_2
	sdm.muscles[21].percent_ft = sdm.muscles[46].percent_ft = .538;// VAS_3
	sdm.muscles[22].percent_ft = sdm.muscles[47].percent_ft = .416;// AddMag
	sdm.muscles[23].percent_ft = sdm.muscles[48].percent_ft = .476;// GMED_p -- no data, used value for GMAX
	sdm.muscles[24].percent_ft = sdm.muscles[49].percent_ft = .375;// PerBrev -- no data, used value for PER_Long

	for(i=0; i<N_mus*2; i++){
		sdm.muscles[i].percent_st = (1-sdm.muscles[i].percent_ft);
	}



/*	//Initialize STsat - activation level at which all slow twitch fiber are active
	//Calculated as (1-%ft)/((%ft*1.46)+(1-%ft)) -- assumes fast twitch stress is 46% higher than slow twitch
	sdm.muscles[0].STsat = sdm.muscles[25].STsat = .8300;// SOL
	sdm.muscles[1].STsat = sdm.muscles[26].STsat = .4142;// MGAS
	sdm.muscles[2].STsat = sdm.muscles[27].STsat = .5806;// SM -- no data, used value for BFlh
	sdm.muscles[3].STsat = sdm.muscles[28].STsat = .5806;// BFsh -- no data, used value for BFlh
	sdm.muscles[4].STsat = sdm.muscles[29].STsat = .3988;// PSOAS
	sdm.muscles[5].STsat = sdm.muscles[30].STsat = .6494;// TA
	sdm.muscles[6].STsat = sdm.muscles[31].STsat = .2953;// RF
	sdm.muscles[7].STsat = sdm.muscles[32].STsat = .3703;// VAS_1
	sdm.muscles[8].STsat = sdm.muscles[33].STsat = .4299;// GMAX
	sdm.muscles[9].STsat = sdm.muscles[34].STsat = .4299;// GMED_a -- no data, used value for GMAX
	sdm.muscles[10].STsat = sdm.muscles[35].STsat = .5330;// PER_Long
	sdm.muscles[11].STsat = sdm.muscles[36].STsat = .5068;// TP -- no data, value from Umberger 2005
	sdm.muscles[12].STsat = sdm.muscles[37].STsat = .3807;// FDL
	sdm.muscles[13].STsat = sdm.muscles[38].STsat = .3807;// FLX_Hal -- no data, used value for FDL
	sdm.muscles[14].STsat = sdm.muscles[39].STsat = .3769;// LGAS
	sdm.muscles[15].STsat = sdm.muscles[40].STsat = .5806;// BFlh
	sdm.muscles[16].STsat = sdm.muscles[41].STsat = .3988;// IL
	sdm.muscles[17].STsat = sdm.muscles[42].STsat = .5330;// PertTert -- no data, used value for PER_Long
	sdm.muscles[18].STsat = sdm.muscles[43].STsat = .3619;// EDL
	sdm.muscles[19].STsat = sdm.muscles[44].STsat = .3619;// EXT_Hal -- no data, used value for EDL
	sdm.muscles[20].STsat = sdm.muscles[45].STsat = .3703;// VAS_2
	sdm.muscles[21].STsat = sdm.muscles[46].STsat = .3703;// VAS_3
	sdm.muscles[22].STsat = sdm.muscles[47].STsat = .4902;// AddMag
	sdm.muscles[23].STsat = sdm.muscles[48].STsat = .4902;// GMED_p -- no data, used value for GMAX
	sdm.muscles[24].STsat = sdm.muscles[49].STsat = .5330;// PerBrev -- no data, used value for PER_Long
*/	
	//Initialize the Muscle masses (Calculated from muscle file max force assuming 250kPa max muscle stress)
	//////////////////////////////mass = ((Fmax/250,000)*opt fiber length)*1059.7 [density of muscle]
	sdm.muscles[0].mus_mass = sdm.muscles[25].mus_mass = .8322;// SOL
	sdm.muscles[1].mus_mass = sdm.muscles[26].mus_mass = .2546;// MGAS
	sdm.muscles[2].mus_mass = sdm.muscles[27].mus_mass = .6911;// SM
	sdm.muscles[3].mus_mass = sdm.muscles[28].mus_mass = .3681;// BFsh 
	sdm.muscles[4].mus_mass = sdm.muscles[29].mus_mass = .3306;// PSOAS
	sdm.muscles[5].mus_mass = sdm.muscles[30].mus_mass = .6854;// TA
	sdm.muscles[6].mus_mass = sdm.muscles[31].mus_mass = .4162;// RF
	sdm.muscles[7].mus_mass = sdm.muscles[32].mus_mass = .9404;// VAS_1
	sdm.muscles[8].mus_mass = sdm.muscles[33].mus_mass = .9156;// GMAX
	sdm.muscles[9].mus_mass = sdm.muscles[34].mus_mass = .1485;// GMED_a
	sdm.muscles[10].mus_mass = sdm.muscles[35].mus_mass = .1566;// PER_Long
	sdm.muscles[11].mus_mass = sdm.muscles[36].mus_mass = .1669;// TP
	sdm.muscles[12].mus_mass = sdm.muscles[37].mus_mass = .0447;// FDL
	sdm.muscles[13].mus_mass = sdm.muscles[38].mus_mass = .0587;// FLX_Hal
	sdm.muscles[14].mus_mass = sdm.muscles[39].mus_mass = .1590;// LGAS
	sdm.muscles[15].mus_mass = sdm.muscles[40].mus_mass = .4965;// BFlh
	sdm.muscles[16].mus_mass = sdm.muscles[41].mus_mass = .4010;// IL
	sdm.muscles[17].mus_mass = sdm.muscles[42].mus_mass = .0301;// PertTert
	sdm.muscles[18].mus_mass = sdm.muscles[43].mus_mass = .1474;// EDL
	sdm.muscles[19].mus_mass = sdm.muscles[44].mus_mass = .0508;// EXT_Hal 
	sdm.muscles[20].mus_mass = sdm.muscles[45].mus_mass = .9404;// VAS_2
	sdm.muscles[21].mus_mass = sdm.muscles[46].mus_mass = .9404;// VAS_3
	sdm.muscles[22].mus_mass = sdm.muscles[47].mus_mass = .8329;// AddMag
	sdm.muscles[23].mus_mass = sdm.muscles[48].mus_mass = .1429;// GMED_p
	sdm.muscles[24].mus_mass = sdm.muscles[49].mus_mass = .0738;// PerBrev -- no data, used value for PER_Long


/*		//Initialize the Muscle masses (Calculated from muscle file max force assuming 250kPa max muscle stress)
	//////////////////////////////mass = ((Fmax/200,000)*opt fiber length)*1059.7 [density of muscle]
	sdm.muscles[0].mus_mass = sdm.muscles[25].mus_mass = 1.0402;// SOL
	sdm.muscles[1].mus_mass = sdm.muscles[26].mus_mass = .3183;// MGAS
	sdm.muscles[2].mus_mass = sdm.muscles[27].mus_mass = .8639;// SM
	sdm.muscles[3].mus_mass = sdm.muscles[28].mus_mass = .385; //0.3681;// BFsh 
	sdm.muscles[4].mus_mass = sdm.muscles[29].mus_mass = .4133;// PSOAS
	sdm.muscles[5].mus_mass = sdm.muscles[30].mus_mass = .8568;// TA
	sdm.muscles[6].mus_mass = sdm.muscles[31].mus_mass = .5203;// RF
	sdm.muscles[7].mus_mass = sdm.muscles[32].mus_mass = 1.1755;// VAS_1
	sdm.muscles[8].mus_mass = sdm.muscles[33].mus_mass = 1.1445;// GMAX
	sdm.muscles[9].mus_mass = sdm.muscles[34].mus_mass = .1225;// GMED_a
	sdm.muscles[10].mus_mass = sdm.muscles[35].mus_mass = .1958;// PER_Long
	sdm.muscles[11].mus_mass = sdm.muscles[36].mus_mass = .2086;// TP
	sdm.muscles[12].mus_mass = sdm.muscles[37].mus_mass = .0558;// FDL
	sdm.muscles[13].mus_mass = sdm.muscles[38].mus_mass = .0734;// FLX_Hal
	sdm.muscles[14].mus_mass = sdm.muscles[39].mus_mass = .1987;// LGAS
	sdm.muscles[15].mus_mass = sdm.muscles[40].mus_mass = .6209;// BFlh
	sdm.muscles[16].mus_mass = sdm.muscles[41].mus_mass = .5012;// IL
	sdm.muscles[17].mus_mass = sdm.muscles[42].mus_mass = .0377;// PertTert
	sdm.muscles[18].mus_mass = sdm.muscles[43].mus_mass = .1843;// EDL
	sdm.muscles[19].mus_mass = sdm.muscles[44].mus_mass = .0635;// EXT_Hal 
	sdm.muscles[20].mus_mass = sdm.muscles[45].mus_mass = 1.1755;// VAS_2
	sdm.muscles[21].mus_mass = sdm.muscles[46].mus_mass = 1.1755;// VAS_3
	sdm.muscles[22].mus_mass = sdm.muscles[47].mus_mass = 1.0412;// AddMag
	sdm.muscles[23].mus_mass = sdm.muscles[48].mus_mass = .1787;// GMED_p
	sdm.muscles[24].mus_mass = sdm.muscles[49].mus_mass = .0922;// PerBrev -- no data, used value for PER_Long
*/
	
/*	//Initialize the Muscle masses (Calculated from muscle file max force assuming 190kPa max muscle stress for slow twitch)
	//and 277kPa for fast twitch (Stienen et al, 1996)
	//////////////////////////////mass_slow = ((Fmax*STsat/190,000)*opt fiber length)*1059.7 [density of muscle]
	//////////////////////////////mass_fast = ((Fmax*(1-STsat)/277,000)*opt fiber length)*1059.7 [density of muscle]
	sdm.muscles[0].mus_mass = sdm.muscles[25].mus_mass = 1.037;// SOL
	sdm.muscles[1].mus_mass = sdm.muscles[26].mus_mass = .2734;// MGAS
	sdm.muscles[2].mus_mass = sdm.muscles[27].mus_mass = .7896;// SM
	sdm.muscles[3].mus_mass = sdm.muscles[28].mus_mass =  .3516; //0.3681;// BFsh 
	sdm.muscles[4].mus_mass = sdm.muscles[29].mus_mass = .3529;// PSOAS
	sdm.muscles[5].mus_mass = sdm.muscles[30].mus_mass = .8026;// TA
	sdm.muscles[6].mus_mass = sdm.muscles[31].mus_mass = .4265;// RF
	sdm.muscles[7].mus_mass = sdm.muscles[32].mus_mass = .9926;// VAS_1
	sdm.muscles[8].mus_mass = sdm.muscles[33].mus_mass = .9890;// GMAX
	sdm.muscles[9].mus_mass = sdm.muscles[34].mus_mass = .1604;// GMED_a
	sdm.muscles[10].mus_mass = sdm.muscles[35].mus_mass = .1758;// PER_Long
	sdm.muscles[11].mus_mass = sdm.muscles[36].mus_mass = .1816;// TP
	sdm.muscles[12].mus_mass = sdm.muscles[37].mus_mass = .0474;// FDL
	sdm.muscles[13].mus_mass = sdm.muscles[38].mus_mass = .0622;// FLX_Hal
	sdm.muscles[14].mus_mass = sdm.muscles[39].mus_mass = .1682;// LGAS
	sdm.muscles[15].mus_mass = sdm.muscles[40].mus_mass = .5674;// BFlh
	sdm.muscles[16].mus_mass = sdm.muscles[41].mus_mass = .4280;// IL
	sdm.muscles[17].mus_mass = sdm.muscles[42].mus_mass = .0338;// PertTert
	sdm.muscles[18].mus_mass = sdm.muscles[43].mus_mass = .1551;// EDL
	sdm.muscles[19].mus_mass = sdm.muscles[44].mus_mass = .0535;// EXT_Hal 
	sdm.muscles[20].mus_mass = sdm.muscles[45].mus_mass = .9926;// VAS_2
	sdm.muscles[21].mus_mass = sdm.muscles[46].mus_mass = .9926;// VAS_3
	sdm.muscles[22].mus_mass = sdm.muscles[47].mus_mass = .9205;// AddMag
	sdm.muscles[23].mus_mass = sdm.muscles[48].mus_mass = .1580;// GMED_p
	sdm.muscles[24].mus_mass = sdm.muscles[49].mus_mass = .0828;// PerBrev -- no data, used value for PER_Long
*/	
/*	Jeff's data for cycling model
	sdm.muscles[0].mus_mass = sdm.muscles[15].mus_mass = .587;
	sdm.muscles[1].mus_mass = sdm.muscles[16].mus_mass = .424;
	sdm.muscles[2].mus_mass = sdm.muscles[17].mus_mass = .576;
	sdm.muscles[3].mus_mass = sdm.muscles[18].mus_mass = .368;
	sdm.muscles[4].mus_mass = sdm.muscles[19].mus_mass = .496;
	sdm.muscles[5].mus_mass = sdm.muscles[20].mus_mass = .571;
	sdm.muscles[6].mus_mass = sdm.muscles[21].mus_mass = .347;
	sdm.muscles[7].mus_mass = sdm.muscles[22].mus_mass = .784;
	sdm.muscles[8].mus_mass = sdm.muscles[23].mus_mass = .763;
	sdm.muscles[9].mus_mass = sdm.muscles[24].mus_mass = .255;
	sdm.muscles[10].mus_mass = sdm.muscles[25].mus_mass = .414;
	sdm.muscles[11].mus_mass = sdm.muscles[26].mus_mass = .334;
	sdm.muscles[12].mus_mass = sdm.muscles[27].mus_mass = .784;
	sdm.muscles[13].mus_mass = sdm.muscles[28].mus_mass = .784;
	sdm.muscles[14].mus_mass = sdm.muscles[29].mus_mass = .694;*/
}