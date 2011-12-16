#include "SA.h"
//#include "Look.h"
#include "SAstructs.h" //replaces "Look.h"
#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
extern "C" OptStruct mdata; //new global data structure

extern "C" SDModelStruct sdm;

//////////////////////////////////////////////////////////////
void init_structure(void)
{
	int i,j;

	/////////allocate memory for variables in mdata structure///////
	mdata.emg_y = new double *[N_mus]; if(mdata.emg_y==NULL) exit(0);
	mdata.XRp = new double *[N_mus]; if(mdata.XRp==NULL) exit(0);
	mdata.YRp = new double *[N_mus]; if(mdata.YRp==NULL) exit(0);
	mdata.XLp = new double *[N_mus]; if(mdata.XLp==NULL) exit(0);
	mdata.YLp = new double *[N_mus]; if(mdata.YLp==NULL) exit(0);
	for(i=0;i<N_mus;i++){
		mdata.emg_y[i] = new double [N_emg_step]; if(mdata.emg_y[i]==NULL) exit(0);
		mdata.XRp[i] = new double [N_emg_step]; if(mdata.XRp[i]==NULL) exit(0);
		mdata.YRp[i] = new double [N_emg_step]; if(mdata.YRp[i]==NULL) exit(0);
		mdata.XLp[i] = new double [N_emg_step]; if(mdata.XLp[i]==NULL) exit(0);
		mdata.YLp[i] = new double [N_emg_step]; if(mdata.YLp[i]==NULL) exit(0);
	}

	mdata.exp_cost_ave = new double *[N_kin]; if(mdata.exp_cost_ave==NULL) exit(0);
	mdata.exp_cost_sd = new double *[N_kin]; if(mdata.exp_cost_sd==NULL) exit(0);
	for(i=0;i<N_kin;i++){
		mdata.exp_cost_ave[i] = new double [N_cost_step]; if(mdata.exp_cost_ave[i]==NULL) exit(0);
		mdata.exp_cost_sd[i] = new double [N_cost_step]; if(mdata.exp_cost_sd[i]==NULL) exit(0);
	}
	
	mdata.init_y = new double [NQ];   
	mdata.init_dy = new double [NQ];

	mdata.hip_r_ang = new double [N_cost_step]; if(mdata.hip_r_ang==NULL) exit(0);
	mdata.knee_r_ang = new double [N_cost_step]; if(mdata.knee_r_ang==NULL) exit(0);
	mdata.ankle_r_ang = new double [N_cost_step]; if(mdata.ankle_r_ang==NULL) exit(0);
	mdata.hip_l_ang = new double [N_cost_step]; if(mdata.hip_l_ang==NULL) exit(0);
	mdata.knee_l_ang = new double [N_cost_step]; if(mdata.knee_l_ang==NULL) exit(0);
	mdata.ankle_l_ang = new double [N_cost_step]; if(mdata.ankle_l_ang==NULL) exit(0);
	mdata.hip_tx = new double [N_cost_step]; if(mdata.hip_tx==NULL) exit(0);
	mdata.hip_ty = new double [N_cost_step]; if(mdata.hip_ty==NULL) exit(0);
	mdata.hip_l_trq = new double [N_cost_step]; if(mdata.hip_l_trq==NULL) exit(0);
	mdata.knee_l_trq = new double [N_cost_step]; if(mdata.knee_l_trq==NULL) exit(0);
	mdata.ankle_l_trq = new double [N_cost_step]; if(mdata.ankle_l_trq==NULL) exit(0);
	mdata.hip_l_pwr = new double [N_cost_step]; if(mdata.hip_l_pwr==NULL) exit(0);
	mdata.knee_l_pwr = new double [N_cost_step]; if(mdata.knee_l_pwr==NULL) exit(0);
	mdata.ankle_l_pwr = new double [N_cost_step]; if(mdata.ankle_l_pwr==NULL) exit(0);
	mdata.grfz_r = new double [N_cost_step]; if(mdata.grfz_r==NULL) exit(0);
	mdata.grfx_l = new double [N_cost_step]; if(mdata.grfx_l==NULL) exit(0);
	mdata.grfz_l = new double [N_cost_step]; if(mdata.grfz_l==NULL) exit(0);
	mdata.grfx_r = new double [N_cost_step]; if(mdata.grfx_r==NULL) exit(0);
	mdata.ankle_l_vel = new double [N_cost_step]; if(mdata.ankle_l_vel==NULL) exit(0);
	mdata.segme_r = new double [N_cost_step]; if(mdata.segme_r==NULL) exit(0);
	mdata.segme_l = new double [N_cost_step]; if(mdata.segme_l==NULL) exit(0);
	mdata.pelvis_rot = new double [N_cost_step]; if(mdata.pelvis_rot==NULL) exit(0);
	mdata.extpwr = new double [N_cost_step]; if(mdata.extpwr==NULL) exit(0);//added 9-1-03
	mdata.sysme = new double [N_cost_step]; if(mdata.sysme==NULL) exit(0);//added 9-15-03
	mdata.syske = new double [N_cost_step]; if(mdata.syske==NULL) exit(0);//added 9-15-03
	mdata.syspe = new double [N_cost_step]; if(mdata.syspe==NULL) exit(0);//added 9-15-03
	mdata.muspower = new double [N_cost_step]; if(mdata.muspower==NULL) exit(0);//added 8-17-04
	mdata.musexc = new double [N_cost_step]; if(mdata.musexc==NULL) exit(0);//added 8-06-07
	mdata.metE = new double [N_cost_step]; if(mdata.metE==NULL) exit(0);//added 2-10-10

	//mdata.mus_pwr = new double [N_mus][N_step];
	//mdata.fib_pwr = new double [N_mus][N_step];
	//mdata.mus_frc = new double [N_mus][N_step];


	/////initialize these with 0.0///////
	for(j=0;j<N_mus;j++){
		for(i=0;i<N_emg_step;i++){
			mdata.emg_y[j][i]=0.0;
			mdata.XRp[j][i]=0.0;
			mdata.YRp[j][i]=0.0;
			mdata.XLp[j][i]=0.0;
			mdata.YLp[j][i]=0.0;
		}
	}

	for(j=0;j<N_kin;j++){
		for(i=0;i<N_cost_step;i++){
			mdata.exp_cost_ave[j][i]=0.0;
			mdata.exp_cost_sd[j][i]=0.0;
		}
	}
	
	
	for(i=0;i<NQ;i++){
		mdata.init_y[i]=0.0;
		mdata.init_dy[i]=0.0;
	}

	for(i=0;i<N_cost_step;i++){
		mdata.hip_r_ang[i]=0.0;
		mdata.knee_r_ang[i]=0.0;
		mdata.ankle_r_ang[i]=0.0;
		mdata.hip_l_ang[i]=0.0;
		mdata.knee_l_ang[i]=0.0;
		mdata.ankle_l_ang[i]=0.0;
		mdata.hip_tx[i]=0.0;
		mdata.hip_ty[i]=0.0;	
		mdata.hip_l_trq[i]=0.0;
		mdata.knee_l_trq[i]=0.0;
		mdata.ankle_l_trq[i]=0.0;
		mdata.hip_l_pwr[i]=0.0;
		mdata.knee_l_pwr[i]=0.0;
		mdata.ankle_l_pwr[i]=0.0;
		mdata.grfz_r[i]=0.0;
		mdata.grfx_l[i]=0.0;
		mdata.grfz_l[i]=0.0;
		mdata.grfx_r[i]=0.0;
		mdata.ankle_l_vel[i]=0.0;
		mdata.segme_r[i] = 0.0;
		mdata.segme_l[i] = 0.0;
		mdata.pelvis_rot[i]=0.0;
		mdata.extpwr[i]=0.0; //added 9-1-03
		mdata.sysme[i]=0.0; //added 9-15-03
		mdata.syske[i]=0.0; //added 9-15-03
		mdata.syspe[i]=0.0; //added 9-15-03
		mdata.muspower[i]=0.0; //added 8-17-04
		mdata.musexc[i] = 0.0; //added 8-06-07
		mdata.metE[i] = 0.0; //added 2-10-10
	}

	mdata.err_hip_r_ang = 0.0;
	mdata.err_knee_r_ang = 0.0;
	mdata.err_ankle_r_ang = 0.0;
	mdata.err_hip_l_ang = 0.0;
	mdata.err_knee_l_ang = 0.0;
	mdata.err_ankle_l_ang = 0.0;
	mdata.err_hip_tx = 0.0;
	mdata.err_hip_ty = 0.0;
	mdata.err_hip_l_trq = 0.0;
	mdata.err_knee_l_trq = 0.0;
	mdata.err_ankle_l_trq = 0.0;
	mdata.err_hip_l_pwr = 0.0;
	mdata.err_knee_l_pwr = 0.0;
	mdata.err_ankle_l_pwr = 0.0;
	mdata.err_grfz_l = 0.0;
	mdata.err_grfx_l = 0.0;
	mdata.err_ankle_l_vel = 0.0;
	mdata.err_grfz_r = 0.0;
	mdata.err_segme = 0.0;
	mdata.err_grfx_r = 0.0;
	mdata.err_pelvis_rot = 0.0;
	mdata.err_extpwr = 0.0; //added 9-1-03
	mdata.err_sysme = 0.0; //added 9-15-03
	mdata.err_syske = 0.0; //added 9-15-03
	mdata.err_syspe = 0.0; //added 9-15-03
	mdata.total_muspower = 0.0; //added 8-17-04
	mdata.total_musexc = 0.0; //added 8-06-07
	mdata.total_MusMetE = 0.0; //added 2-10-10

	///////////////////////////////////////////////////////////

	///////for motion-file variables////////	
	mdata.motion_y = new double *[NintegStep]; if(mdata.motion_y==NULL) exit(0);
	mdata.mus_dyn_act = new double *[NintegStep]; if(mdata.mus_dyn_act==NULL) exit(0);
	mdata.Rgrf = new double *[NintegStep]; if(mdata.Rgrf==NULL) exit(0);
	mdata.Rcop = new double *[NintegStep]; if(mdata.Rcop==NULL) exit(0);
	mdata.Lgrf = new double *[NintegStep]; if(mdata.Rgrf==NULL) exit(0);
	mdata.Lcop = new double *[NintegStep]; if(mdata.Rcop==NULL) exit(0);
	for(i=0;i<NintegStep;i++){
		mdata.motion_y[i] = new double [NQ]; if(mdata.motion_y[i]==NULL) exit(0);
		mdata.mus_dyn_act[i] = new double [N_mus*2]; if(mdata.mus_dyn_act[i]==NULL) exit(0);
		mdata.Rgrf[i] = new double [3]; if(mdata.Rgrf[i]==NULL) exit(0);
		mdata.Rcop[i] = new double [3]; if(mdata.Rcop[i]==NULL) exit(0);
		mdata.Lgrf[i] = new double [3]; if(mdata.Rgrf[i]==NULL) exit(0);
		mdata.Lcop[i] = new double [3]; if(mdata.Rcop[i]==NULL) exit(0);
	}

	//initialize
	for(i=0;i<NintegStep;i++){
		for(j=0;j<NQ;j++){
			mdata.motion_y[i][j]=0.0;
			mdata.mus_dyn_act[i][j]=0.0;
		}
	}

	///////////////////////////////////////////////////////////

	///////for tracking display
	mdata.track = new double *[N_track]; if(mdata.track==NULL) exit(0);
	for(i=0;i<N_track;i++){
		mdata.track[i] = new double[N_cost_step]; if(mdata.track[i]==NULL) exit(0);
	}
	//initialize
	for(i=0;i<N_track;i++){
		for(j=0;j<N_cost_step;j++){
			mdata.track[i][j]=0.0;
		}
	}

	///////for muscle activation patterns
	
	mdata.mus_actR = new double *[N_mus]; if(mdata.mus_actR==NULL) exit(0);
	mdata.mus_actL = new double *[N_mus]; if(mdata.mus_actL==NULL) exit(0);
	for(i=0;i<N_mus;i++){
		mdata.mus_actR[i] = new double [N_emg_step]; if(mdata.mus_actR[i]==NULL) exit(0);
		mdata.mus_actL[i] = new double [N_emg_step]; if(mdata.mus_actL[i]==NULL) exit(0);
	}
	//initialize
	for(i=0;i<N_mus;i++){
		for(j=0;j<N_emg_step;j++){
			mdata.mus_actR[i][j]=0.0;
			mdata.mus_actL[i][j]=0.0;
		}
	}
	

}
