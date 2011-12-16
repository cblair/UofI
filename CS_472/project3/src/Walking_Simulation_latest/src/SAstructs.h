#ifndef SASTRUCTS_H
#define SASTRUCTS_H

#include "basic.h"
#include "SA.h"
#include "ExcGroups.h"

STRUCT{
	int nPoint; //number of zero points (template)	
} ZeroRegion;  //Created by Jeff Rankin - Oct 2008, based on Ko Sasaki code

STRUCT{ 
	int cut1; //template time that the excitation region turns on (used particularly for cutting the experimental EMG)
	int cut2; //this is the template time that the excitation region turns off (used particularly for cutting the experimental EMG)
	int nPoint; //number of points within the excitation region
	double *x; //x values of the excitation region
	double *y; //excitation magnitude at each x value
} ExcRegion; //Created by Jeff Rankin - Oct 2008, based on Ko Sasaki code

STRUCT{
	int pattern_type;			//this keeps track of the type of excitation pattern this muscle is using
								//1:one hill, 2:2 hills, 3:3 hills, 4:2 hills in the middle
	int pattern_source;			//This keeps track of the source data for the muscle excitation pattern. For now there
								//are only 2 options. 1 = using experimental EMG, 2 = using a block excitation pattern
	int num_excite_params;		//this keeps track of the number of optimization parameters for the given excitation pattern type

	double *excite_params;		//this stores the current parameter values used in modifying the excitation patterns during each simulation

	int NumExciteRegions;
	int NumZeroRegions;

	ExcRegion  *onBlock;		//keeps track of the x and y values of the excitation regions during the simulation
	ZeroRegion *offBlock;		//keeps track of the x values that are at 0 excitation value during the simulation
	
} ExcPattern_Template;	//Created by Jeff Rankin - Oct 2008, based on Ko Sasaki code


STRUCT {
	double **emg_y; //emg curve (processed) y value
	double **XRp; //modulated excitation curve (x-right)
	double **YRp; //modulated excitation curve (y-right)
	double **XLp; //modulated excitation curve (x-left)
	double **YLp; //modulated excitation curve (y-left)
	double  *init_y, *init_dy; //"optimal" initial states

	ExcPattern_Template ExcPatterns[N_musG]; //muscle activation patterns structures //naming modified for new exc code

	double *hip_r_ang;
	double *knee_r_ang;
	double *ankle_r_ang;
	double *hip_l_ang;
	double *knee_l_ang;
	double *ankle_l_ang;
	double *hip_tx;
	double *hip_ty;
	double *hip_l_trq;
	double *knee_l_trq;
	double *ankle_l_trq;
	double *hip_l_pwr;
	double *knee_l_pwr;
	double *ankle_l_pwr;
	double *grfz_l;
	double *grfz_r;
	double *grfx_l;
	double *grfx_r;
	double *ankle_l_vel;
	double *segme_r;
	double *segme_l;
	double *pelvis_rot;
	double *extpwr; //added 9-1-03
	double *sysme; //added 9-15-03
	double *syske; //added 9-15-03
	double *syspe; //added 9-15-03
	double *muspower; //added 8-17-04
	double Lgrf_on; //added 10-21-04
	double *musexc; //added 8-06-07
	double *metE; //added 2-10-10

	//double (*mus_pwr)[N_step];
	//double (*fib_pwr)[N_step];
	//double (*mus_frc)[N_step];

	double err_hip_r_ang;
	double err_knee_r_ang;
	double err_ankle_r_ang;
	double err_hip_l_ang;
	double err_knee_l_ang;
	double err_ankle_l_ang;
	double err_hip_tx;
	double err_hip_ty;
	double err_hip_l_trq;
	double err_knee_l_trq;
	double err_ankle_l_trq;
	double err_hip_l_pwr;
	double err_knee_l_pwr;
	double err_ankle_l_pwr;
	double err_grfz_l;
	double err_grfx_l;
	double err_ankle_l_vel;
	double err_grfz_r;
	double err_grfx_r;
	double err_segme;
	double err_pelvis_rot;
	double err_extpwr; //added 9-1-03
	double err_sysme; //added 9-15-03
	double err_syske; //added 9-15-03
	double err_syspe; //added 9-15-03
	double total_muspower; //added 8-17-04
	double err_Lgrf_on; //added 10-21-04
	double total_musexc; //added 8-06-07
	double total_MusMetE; //added 2-10-10

	double err_adjusted_hip_r_ang;
	double err_adjusted_knee_r_ang;
	double err_adjusted_ankle_r_ang;
	double err_adjusted_hip_l_ang;
	double err_adjusted_knee_l_ang;
	double err_adjusted_ankle_l_ang;
	double err_adjusted_hip_tx;
	double err_adjusted_hip_ty;
	double err_adjusted_hip_l_trq;
	double err_adjusted_knee_l_trq;
	double err_adjusted_ankle_l_trq;
	double err_adjusted_hip_l_pwr;
	double err_adjusted_knee_l_pwr;
	double err_adjusted_ankle_l_pwr;
	double err_adjusted_grfz_l;
	double err_adjusted_grfx_l;
	double err_adjusted_ankle_l_vel;
	double err_adjusted_grfz_r;
	double err_adjusted_grfx_r;
	double err_adjusted_segme;
	double err_adjusted_pelvis_rot;
	double err_adjusted_extpwr; //added 9-1-03
	double err_adjusted_sysme; //added 9-15-03
	double err_adjusted_syske; //added 9-15-03
	double err_adjusted_syspe; //added 9-15-03
	double total_adjusted_muspower; //added 8-17-04
	double err_Lgrf_on_adjusted; //added 10-21-04
	double total_adjusted_musexc; //added 8-06-07
	double err_adjusted_NetSystemWork; //added 2-26-08
	double err_adjusted_NetTendonWork; //added 2-26-08
	double total_adjusted_MusMetE; //added 8-06-07

	double **exp_cost_ave; //experimental cost function
	double **exp_cost_sd; //experimental cost function

	///////////initial best velocities//////////////
	double hipR_w,kneeR_w,ankleR_w,hipL_w,kneeL_w,ankleL_w;
    double legTx_v,legTy_v,pelvis_w;
	double midR_w,mtpR_w,midL_w,mtpL_w; // added 4-2-08

	//////initial experimental states (to overwrite)//////
	double hipR,kneeR,ankleR,hipL,kneeL,ankleL;
	double legTx,legTy,pelv_rot;


	///////////for a motion file////////////////
	double **motion_y;
	double **mus_dyn_act;
	int nstep;
	double dt;
	char *motion_file_name;

	double **Rgrf, **Rcop, **Lgrf, **Lcop; //added 1-16-08

	///////////tracking display////////////////
	double **track;

	///////////Muscle activation////////////
	double **mus_actR, **mus_actL;


	/////////// Net system work /// added 2-25-08
	double NetSystemWork;
	double NetTendonWork;


} OptStruct;

#endif