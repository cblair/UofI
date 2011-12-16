#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "SA.h"
#include "SAstructs.h" //replaces "Look.h"
#include "structs.h"
#include "system_params.h"

//extern "C" SeeThis mdata;
extern "C" OptStruct mdata;

extern "C" SDModelStruct sdm;

extern char* kineticfilename;

extern "C" int PATTERN;

 

/*******************read experimental emg data***********************/
//replaced with new excitation code
/*void read_cut_emg(void)
{
	int i,m;
	int npP0,npP1,npP2,npZ0,npZ1;

	double emg_sd;

	int dummy;
	double emg_max[N_musG], emg_min[N_musG]; 
	double offset, offset1;
	double max_amp1, max_amp2, max_amp3;


	FILE *fp_exp_emg;

	/*********************************************/
	/****************EMG pattern******************/
	/*********************************************/
/*	if(PATTERN==EMG){
		if((fp_exp_emg=fopen(emgfilename,"r"))==NULL){
			printf("emgfile cannot be opened!!!");
			exit(0);
		}
		
		for(i=0;i<N_emg_step;i++){
			fscanf(fp_exp_emg,"%d",&dummy); //%gait cycle
			
			fscanf(fp_exp_emg,"%lf %lf", &mdata.emg_y[solG][i], &emg_sd);//read SOL
				
			fscanf(fp_exp_emg,"%lf %lf", &mdata.emg_y[gasG][i], &emg_sd);//read m_GAS
				
			fscanf(fp_exp_emg,"%lf %lf", &mdata.emg_y[smG][i], &emg_sd);//read BFlf
				
			fscanf(fp_exp_emg,"%lf %lf", &mdata.emg_y[taG][i], &emg_sd);//read TA
				
			fscanf(fp_exp_emg,"%lf %lf", &mdata.emg_y[rfG][i], &emg_sd);//read RF
				
			fscanf(fp_exp_emg,"%lf %lf", &mdata.emg_y[vas1G][i], &emg_sd);//read VAS
				
			fscanf(fp_exp_emg,"%lf %lf", &mdata.emg_y[gmaxG][i], &emg_sd);//read GMAX

			mdata.emg_y[vas2G][i]=mdata.emg_y[vas1G][i];
			mdata.emg_y[vas3G][i]=mdata.emg_y[vas1G][i];

			mdata.emg_y[bfshG][i]=1.0;
			mdata.emg_y[psoasG][i]=1.0;
			mdata.emg_y[gmedG][i]=1.0;
			mdata.emg_y[perlngG][i]=1.0;
			mdata.emg_y[flxdigG][i]=1.0;
			mdata.emg_y[extdigG][i]=1.0;

		}

		fclose(fp_exp_emg);


		//obtain max and min for normalization
		for(m=0;m<N_musG;m++){
			emg_max[m] = -100000.0;
			emg_min[m] = 100000.0;
		}

		for(m=0;m<N_musG;m++){
			emg_max[m] = -100000.0;
			emg_min[m] = 100000.0;
			for(i=0;i<N_emg_step-1;i++){ //"N_emg_step-1" is to prevent the last point (101) to be max or min (if allowed, it causes a minor problem)
				if(emg_max[m] < mdata.emg_y[m][i]) emg_max[m] = mdata.emg_y[m][i];
				if(emg_min[m] > mdata.emg_y[m][i]) emg_min[m] = mdata.emg_y[m][i];
			}
		}

		/////////////////////////////////////////////////////////////////////


		//normalize
		for(m=0;m<N_musG;m++){
			if((emg_max[m]-emg_min[m])!=0.0)
			for(i=0;i<N_emg_step;i++){
				mdata.emg_y[m][i] = (mdata.emg_y[m][i] - emg_min[m])/(emg_max[m]-emg_min[m]);
			}
//printf("m=%d, y=%f\n",m,mdata.emg_y[m][0]);
		}

		/*
		for(i=0;i<N_emg_step;i++){
			mdata.emg_y[tp][i]=mdata.emg_y[sol][i];
			mdata.emg_y[flxdig][i]=mdata.emg_y[sol][i];
			mdata.emg_y[flxhal][i]=mdata.emg_y[sol][i];

			mdata.emg_y[lgas][i]=mdata.emg_y[mgas][i]; 

			mdata.emg_y[pertert][i]=mdata.emg_y[ta][i];
			mdata.emg_y[extdig][i]=mdata.emg_y[ta][i];
			mdata.emg_y[exthal][i]=mdata.emg_y[ta][i];

			mdata.emg_y[bfsh][i]=1.0; 
			mdata.emg_y[psoas][i]=1.0; 
			mdata.emg_y[gmeda][i]=1.0; 
			mdata.emg_y[perlng][i]=1.0; 

			mdata.emg_y[bflh][i]=mdata.emg_y[sm][i]; 
			mdata.emg_y[iliac][i]=mdata.emg_y[psoas][i]; 
			mdata.emg_y[vas2][i]=mdata.emg_y[vas1][i]; 
			mdata.emg_y[vas3][i]=mdata.emg_y[vas1][i]; 
			mdata.emg_y[addmag][i]=mdata.emg_y[gmax][i]; 
			mdata.emg_y[gmedp][i]=mdata.emg_y[gmeda][i]; 
			mdata.emg_y[perbrev][i]=mdata.emg_y[perlng][i]; 
		}
		*/
		

//	}

	/***********************************************************/
	/**********************Block pattern************************/
	/***********************************************************/
/*	else{
		for(m=0;m<N_musG;m++){
			for(i=0;i<N_emg_step;i++){
				mdata.emg_y[m][i] = 1.0;
			}
		}
	}
	/***********************************************************/

	/////Assign y values
/*	for(m=0;m<N_musG;m++){

		if(mdata.ExcP[m].type == 1){
			npP0 = mdata.ExcP[m].pBlock[0].nPoint;
			npZ0 = mdata.ExcP[m].zBlock[0].nPoint;

			
			if(PATTERN==EMG){
				if(m!=bfshG && m!=psoasG && m!=gmedG && m!=perlngG && m!=flxdigG && m!=extdigG){
					offset=__max(mdata.emg_y[m][npZ0],mdata.emg_y[m][npZ0+npP0-1]);
				}
				else offset=0.0;
			}
			else offset=0.0; //BLOCK
			
			

			for(i=0;i<npP0;i++){
				mdata.ExcP[m].pBlock[0].px[i] = (double)(i+npZ0);
				mdata.ExcP[m].pBlock[0].py[i] = (mdata.emg_y[m][i+npZ0]-offset)*(1.0/(1.0-offset));//the last term is to make max. amplitude 1.
				//mdata.ExcP[m].pBlock[0].py[i] = mdata.emg_y[m][i+npZ0];
				if(mdata.ExcP[m].pBlock[0].py[i]<0.0){
					mdata.ExcP[m].pBlock[0].py[i]=0.0;
				}
			}
		}

		if(mdata.ExcP[m].type == 2){
			npP0 = mdata.ExcP[m].pBlock[0].nPoint;
			npP1 = mdata.ExcP[m].pBlock[1].nPoint;
			npZ0 = mdata.ExcP[m].zBlock[0].nPoint;

			
			if(PATTERN==EMG){
				if(m!=bfshG && m!=psoasG && m!=gmedG && m!=perlngG && m!=flxdigG && m!=extdigG){
					offset=__max(mdata.emg_y[m][npP0-1],mdata.emg_y[m][npP0+npZ0]);
				}
				else offset=0.0;
			}
			else offset=0.0; //BLOCK
			
			

			for(i=0;i<npP0;i++){
				mdata.ExcP[m].pBlock[0].px[i] = (double)i;
				mdata.ExcP[m].pBlock[0].py[i] = (mdata.emg_y[m][i]-offset)*(1.0/(1.0-offset));
				//mdata.ExcP[m].pBlock[0].py[i] = mdata.emg_y[m][i];
				if(mdata.ExcP[m].pBlock[0].py[i]<0.0){
					mdata.ExcP[m].pBlock[0].py[i]=0.0;
				}
			}//first lump
			

			for(i=0;i<npP1;i++){
				mdata.ExcP[m].pBlock[1].px[i] = (double)(i+npP0+npZ0);
				mdata.ExcP[m].pBlock[1].py[i] = (mdata.emg_y[m][i+npP0+npZ0]-offset)*(1.0/(1.0-offset));
				//mdata.ExcP[m].pBlock[1].py[i] = mdata.emg_y[m][i+npP0+npZ0];
				if(mdata.ExcP[m].pBlock[1].py[i]<0.0){
					mdata.ExcP[m].pBlock[1].py[i]=0.0;
				}
			}//second lump

		}

		if(mdata.ExcP[m].type == 3){
			npP0 = mdata.ExcP[m].pBlock[0].nPoint;
			npP1 = mdata.ExcP[m].pBlock[1].nPoint;
			npP2 = mdata.ExcP[m].pBlock[2].nPoint;
			npZ0 = mdata.ExcP[m].zBlock[0].nPoint;
			npZ1 = mdata.ExcP[m].zBlock[1].nPoint; 

			
			if(PATTERN==EMG){
				if(m!=bfshG && m!=psoasG && m!=gmedG && m!=perlngG && m!=flxdigG && m!=extdigG){
					offset=__max(mdata.emg_y[m][npP0-1],mdata.emg_y[m][npP0+npZ0+npP1+npZ1]);//side
					offset1=__max(mdata.emg_y[m][npP0+npZ0],mdata.emg_y[m][npP0+npZ0+npP1-1]);//center
				}
				else{
					offset=0.0;
					offset1=0.0;
				}
			}
			else{ //BLOCK
				offset=0.0;
				offset1=0.0;
			}
			
			
		
			for(i=0;i<npP0;i++){
				mdata.ExcP[m].pBlock[0].px[i] = (double)i;
				mdata.ExcP[m].pBlock[0].py[i] = (mdata.emg_y[m][i]-offset)*(1.0/(1.0-offset));
				//mdata.ExcP[m].pBlock[0].py[i] = mdata.emg_y[m][i];
				if(mdata.ExcP[m].pBlock[0].py[i]<0.0){
					mdata.ExcP[m].pBlock[0].py[i]=0.0;
				}

			}//first lump

			//obtain max value of the fisrt lump (to re-normalize later, the first normalization in the beginning of this 
			//subroutine was normalizing to the max value of the entire emg curve, not individual lobes.
			max_amp1=-100.0;
			for(i=0;i<npP0;i++){
				if(mdata.ExcP[m].pBlock[0].py[i]>max_amp1) max_amp1=mdata.ExcP[m].pBlock[0].py[i];
			}
			/////////////////////////////////////////////////////////

			for(i=0;i<npP1;i++){
				mdata.ExcP[m].pBlock[1].px[i] = (double)(i+npP0+npZ0);
				mdata.ExcP[m].pBlock[1].py[i] = (mdata.emg_y[m][i+npP0+npZ0]-offset1)*(1.0/(1.0-offset1));
				//mdata.ExcP[m].pBlock[1].py[i] = mdata.emg_y[m][i+npP0+npZ0];
				if(mdata.ExcP[m].pBlock[1].py[i]<0.0){
					mdata.ExcP[m].pBlock[1].py[i]=0.0;
				}

			}//second lump	
			//obtain max value of the second lump
			max_amp2=-100.0;
			for(i=0;i<npP1;i++){
				if(mdata.ExcP[m].pBlock[1].py[i]>max_amp2) max_amp2=mdata.ExcP[m].pBlock[1].py[i];
			}
			//////////////////////////////////////////////////////////

		
			for(i=0;i<npP2;i++){
				mdata.ExcP[m].pBlock[2].px[i] = (double)(i+npP0+npZ0+npP1+npZ1);
				mdata.ExcP[m].pBlock[2].py[i] = (mdata.emg_y[m][i+npP0+npZ0+npP1+npZ1]-offset)*(1.0/(1.0-offset));
				//mdata.ExcP[m].pBlock[2].py[i] = mdata.emg_y[m][i+npP0+npZ0+npP1+npZ1];
				if(mdata.ExcP[m].pBlock[2].py[i]<0.0){
					mdata.ExcP[m].pBlock[2].py[i]=0.0;
				}
			}//third lump
			//obtain max value of the third lump
			max_amp3=-100.0;
			for(i=0;i<npP2-1;i++){//"npP2-1" is to avoid the last point (101) to be maximum (if allowed, it causes a minor problem in changing emg pattern)
				if(mdata.ExcP[m].pBlock[2].py[i]>max_amp3) max_amp3=mdata.ExcP[m].pBlock[2].py[i];
			}

			//////////////////////////////////////////////////////////

			//Normalization
			for(i=0;i<npP0;i++){
				mdata.ExcP[m].pBlock[0].py[i] = mdata.ExcP[m].pBlock[0].py[i]/(__max(max_amp1,max_amp3));
			}//first lump

			for(i=0;i<npP1;i++){
				mdata.ExcP[m].pBlock[1].py[i] = mdata.ExcP[m].pBlock[1].py[i]/max_amp2;
			}//second lump	

			for(i=0;i<npP2;i++){
				mdata.ExcP[m].pBlock[2].py[i] = mdata.ExcP[m].pBlock[2].py[i]/(__max(max_amp1,max_amp3));
			}//third lump


			
		}

	}



}
/////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
/*******read experimental kinematic data****************/
void read_kin(void)
{
	int i,j;
	FILE *fp_exp_kin;
	
	double **exp_kin_ave;
	double **exp_kin_sd;
	double time_dummy, dummy;

	exp_kin_ave = new double *[N_kin]; if(exp_kin_ave==NULL) exit(0);
	exp_kin_sd = new double *[N_kin]; if(exp_kin_sd==NULL) exit(0);
	for(i=0;i<N_kin;i++){
		exp_kin_ave[i] = new double[N_cost_step]; if(exp_kin_ave[i]==NULL) exit(0);
		exp_kin_sd[i] = new double[N_cost_step]; if(exp_kin_sd[i]==NULL) exit(0);
	}


	if((fp_exp_kin=fopen(kineticfilename,"r"))==NULL){ //processed kinematic data
		printf("kinetic file '%s' cannot be opened!!!", kineticfilename);
		exit(0);
	}


	//ignore the initial condition (simulation data start after one integration
	fscanf(fp_exp_kin,"%lf",&time_dummy); 
	for(j=0;j<N_kin;j++){
		fscanf(fp_exp_kin,"%lf %lf", &dummy, &dummy);
	}

	//take data from 1 to 150 (150 data)
	for(i=0;i<N_cost_step;i++){
		fscanf(fp_exp_kin,"%lf",&time_dummy); 
		for(j=0;j<N_kin;j++){
			fscanf(fp_exp_kin,"%lf %lf", &exp_kin_ave[j][i], &exp_kin_sd[j][i]);
		}
	}

	for(i=0;i<N_cost_step;i++){
		mdata.exp_cost_ave[0][i]=exp_kin_ave[0][i]; //Hip_r flexion AVE
		mdata.exp_cost_sd[0][i]=exp_kin_sd[0][i];   //Hip_r flexion SD
		mdata.exp_cost_ave[1][i]=exp_kin_ave[1][i]; //Knee_r flexion AVE
		mdata.exp_cost_sd[1][i]=exp_kin_sd[1][i];   //Knee_r flexion SD
		mdata.exp_cost_ave[2][i]=exp_kin_ave[2][i]; //Ankle_r flexion AVE
		mdata.exp_cost_sd[2][i]=exp_kin_sd[2][i];   //Ankle_r flexion SD
		mdata.exp_cost_ave[3][i]=exp_kin_ave[3][i]; //Hip_l flexion AVE
		mdata.exp_cost_sd[3][i]=exp_kin_sd[3][i];   //Hip_l flexion SD
		mdata.exp_cost_ave[4][i]=exp_kin_ave[4][i]; //Knee_l flexion AVE
		mdata.exp_cost_sd[4][i]=exp_kin_sd[4][i];   //Knee_l flexion SD
		mdata.exp_cost_ave[5][i]=exp_kin_ave[5][i]; //Ankle_l flexion AVE
		mdata.exp_cost_sd[5][i]=exp_kin_sd[5][i];   //Ankle_l flexion SD
		mdata.exp_cost_ave[6][i]=exp_kin_ave[6][i]; //Hip_tx AVE
		mdata.exp_cost_sd[6][i]=exp_kin_sd[6][i];   //Hip_tx SD
		mdata.exp_cost_ave[7][i]=exp_kin_ave[7][i]; //Hip_ty AVE
		mdata.exp_cost_sd[7][i]=exp_kin_sd[7][i];   //Hip_ty SD
		mdata.exp_cost_ave[8][i]=exp_kin_ave[8][i]; //GrfZr AVE
		mdata.exp_cost_sd[8][i]=exp_kin_sd[8][i];   //GrfZr SD
		mdata.exp_cost_ave[9][i]=exp_kin_ave[9][i]; //Hip trq AVE
		mdata.exp_cost_sd[9][i]=exp_kin_sd[9][i];   //Hip trq SD
		mdata.exp_cost_ave[10][i]=exp_kin_ave[10][i]; //Knee trq AVE
		mdata.exp_cost_sd[10][i]=exp_kin_sd[10][i];   //Knee trq SD
		mdata.exp_cost_ave[11][i]=exp_kin_ave[11][i]; //Ankle trq AVE
		mdata.exp_cost_sd[11][i]=exp_kin_sd[11][i];   //Ankle trq SD
		mdata.exp_cost_ave[12][i]=exp_kin_ave[12][i]; //GrfZl AVE
		mdata.exp_cost_sd[12][i]=exp_kin_sd[12][i];   //GrfZl  SD
		mdata.exp_cost_ave[13][i]=exp_kin_ave[13][i]; //Hip_power trq AVE
		mdata.exp_cost_sd[13][i]=exp_kin_sd[13][i];   //Hip_power SD
		mdata.exp_cost_ave[14][i]=exp_kin_ave[14][i]; //Knee_power AVE
		mdata.exp_cost_sd[14][i]=exp_kin_sd[14][i];   //Knee_power SD
		mdata.exp_cost_ave[15][i]=exp_kin_ave[15][i]; //Ankle_power AVE
		mdata.exp_cost_sd[15][i]=exp_kin_sd[15][i];   //Ankle_power SD
		mdata.exp_cost_ave[16][i]=exp_kin_ave[16][i]; //GrfXl AVE
		mdata.exp_cost_sd[16][i]=exp_kin_sd[16][i];   //GrfXl SD
		mdata.exp_cost_ave[17][i]=exp_kin_ave[17][i]; //Ankle_vel AVE
		mdata.exp_cost_sd[17][i]=exp_kin_sd[17][i];   //Ankle_vel SD
		mdata.exp_cost_ave[18][i]=exp_kin_ave[18][i]; //Pelvis_rot AVE
		mdata.exp_cost_sd[18][i]=exp_kin_sd[18][i];   //Pelvis_rot SD
		mdata.exp_cost_ave[19][i]=exp_kin_ave[19][i]; //External power AVE
		mdata.exp_cost_sd[19][i]=exp_kin_sd[19][i];   //External power SD

	}

	fclose(fp_exp_kin);

	delete [] exp_kin_ave;
	delete [] exp_kin_sd;

}

/////////////////////////////////////////////////////////////////////////
//////////////////SAVE CURRENT FOR DEBUG////////////////////////////////

void save_current(double *x,int NEVALS,double fp,
			   double T, double *VM, int N_params)        
{
	int i,j;
	FILE *fp_write,*fp_error,*fp_init;
	//FILE *fp_opt,*fp_pwr,*fp_for;

	int m, opt_param_num;
	

	/***save SA info (so far) to see progress***/
	if(NEVALS == 0){
		if((fp_write=fopen("SAtempCOST.dat","w"))==NULL){
			printf("SAtempCOST.dat cannot be opened!!!");
			//exit(0);
		}
	}
	else{ //this is the case for START=0
		if((fp_write=fopen("SAtempCOST.dat","a"))==NULL){
			printf("SAtempCOST.dat cannot be opened!!!");
			//exit(0);
		}
	}

	fprintf(fp_write,"Cost = %lf\t# of function calls = %d\n",fp,NEVALS);
	fclose(fp_write);
							


	
	/***save EMG pattern***/
/*	if((fp_write=fopen("SAoptEMG.dat","w"))==NULL){
		printf("SAoptEMG.dat cannot be opened!!!");
		//exit(0);
	}
	for(i=0;i<N_emg_step;i++){
		for(j=0;j<N_mus;j++){
			fprintf(fp_write,"%lf %lf %lf %lf ",
			mdata.XRp[j][i], mdata.YRp[j][i], 
			mdata.XLp[j][i], mdata.YLp[j][i]);
		}
		fprintf(fp_write,"\n");
	}
	fclose(fp_write);
*/

						


}

//////////////////////////////////////////////////////////////////////


/////////////////Save SA best results///////////////////////////
void save_best(double *xopt,int OPTEVAL,double fopt,
			   double T, double *VM, int N_params)
{
	int i,j;
	FILE *fp_write,*fp_error,*fp_init;
	//FILE *fp_opt,*fp_pwr,*fp_for;

	int m, opt_param_num;
	

	/***save SA info (so far) to see progress***/
	if(OPTEVAL == 0){
		if((fp_write=fopen("SAoptCOST.dat","w"))==NULL){
			printf("SAoptCOST.dat cannot be opened!!!");
			//exit(0);
		}
	}
	else{ //this is the case for START=0
		if((fp_write=fopen("SAoptCOST.dat","a"))==NULL){
			printf("SAoptCOST.dat cannot be opened!!!");
			//exit(0);
		}
	}

	fprintf(fp_write,"Cost = %lf\t# of function calls = %d\n",fopt,OPTEVAL);
	fclose(fp_write);
							
	/***save the best parameters (x's) for the next run***/
	//replaced with the next set of code
	/*if(PATTERN==EMG){
		if((fp_write=fopen("SAstartXemg.dat","w"))==NULL){
			printf("Cannot open SAstartX.dat to write\n");
		}
	}
	if(PATTERN==BLOCK){
		if((fp_write=fopen("SAstartXblock.dat","w"))==NULL){
			printf("Cannot open SAstartX.dat to write\n");
		}
	}
	for(i=0;i<N_params;i++){
		if(i==N_params-13){
			fprintf(fp_write,"\n");
		}
		fprintf(fp_write,"%lf\n",xopt[i]);
	}
	fclose(fp_write);*/

	if((fp_write=fopen(OPTPARAMFILE,"w"))==NULL){
		printf("Cannot open SAstartX.dat to write\n");
	}

	opt_param_num = 0;
		
	for (m=0;m<N_musG;m++){		
		for (i=0;i<mdata.ExcPatterns[m].num_excite_params;i++){
			fprintf(fp_write,"%lf\n",xopt[opt_param_num+i]);			
		}//end i loop
		opt_param_num += mdata.ExcPatterns[m].num_excite_params;
		fprintf(fp_write,"\n");
	}//end loop through the muscle group excitation parameters

	//append the other parameters to the file
	for(i=0;i<N_params_init;i++){
		fprintf(fp_write,"%lf\n",xopt[i+N_EXC_PARAMS]);
	}
	fclose(fp_write);


	/***save SA info (so far) to resume SA again***/
	if((fp_write=fopen("SAoptINFO.dat","w"))==NULL){
		printf("SAoptINFO.dat cannot be opened!!!");
		//exit(0);
	}
	fprintf(fp_write,"%s\n",kineticfilename);
	fprintf(fp_write,"%d\n",OPTEVAL);
	fprintf(fp_write,"%lf\n",fopt);
	fprintf(fp_write,"%lf\n\n",T);
	for(i=0;i<N_params;i++){
		fprintf(fp_write,"%lf\n",VM[i]);
	}
	fprintf(fp_write,"\n");
	for(i=0;i<N_params;i++){
		fprintf(fp_write,"%lf\n",xopt[i]);
	}
	fclose(fp_write);

	
	/***save EMG pattern***/
	if((fp_write=fopen("SAoptEMG.dat","w"))==NULL){
		printf("SAoptEMG.dat cannot be opened!!!");
		//exit(0);
	}
	for(i=0;i<N_emg_step;i++){
		for(j=0;j<N_mus;j++){
			fprintf(fp_write,"%lf %lf %lf %lf ",
			mdata.XRp[j][i], mdata.YRp[j][i], 
			mdata.XLp[j][i], mdata.YLp[j][i]);
		}
		fprintf(fp_write,"\n");
	}
	fclose(fp_write);

	/***save error profile***/
	if((fp_error=fopen("SAoptERROR.dat","w"))==NULL){
		printf("SAoptERROR.dat cannot be opened!!!");
		//exit(0);
	}
	fprintf(fp_error,"Hip_r_ang (adjusted):\t%lf (%lf)\n",mdata.err_hip_r_ang,mdata.err_adjusted_hip_r_ang);
	fprintf(fp_error,"Knee_r_ang (adjusted):\t%lf (%lf)\n",mdata.err_knee_r_ang,mdata.err_adjusted_knee_r_ang);
	fprintf(fp_error,"Ankle_r_ang (adjusted):\t%lf (%lf)\n",mdata.err_ankle_r_ang,mdata.err_adjusted_ankle_r_ang);
	fprintf(fp_error,"Hip_l_ang (adjusted):\t%lf (%lf)\n",mdata.err_hip_l_ang,mdata.err_adjusted_hip_l_ang);
	fprintf(fp_error,"Knee_l_ang (adjusted):\t%lf (%lf)\n",mdata.err_knee_l_ang,mdata.err_adjusted_knee_l_ang);
	fprintf(fp_error,"Ankle_l_ang (adjusted):\t%lf (%lf)\n",mdata.err_ankle_l_ang,mdata.err_adjusted_ankle_l_ang);
	fprintf(fp_error,"Tx (adjusted):\t\t%lf (%lf)\n",mdata.err_hip_tx,mdata.err_adjusted_hip_tx);
	fprintf(fp_error,"Ty (adjusted):\t\t%lf (%lf)\n",mdata.err_hip_ty,mdata.err_adjusted_hip_ty);
	fprintf(fp_error,"Hip_l_trq (adjusted):\t%lf (%lf)\n",mdata.err_hip_l_trq,mdata.err_adjusted_hip_l_trq);
	fprintf(fp_error,"Knee_l_trq (adjusted):\t%lf (%lf)\n",mdata.err_knee_l_trq,mdata.err_adjusted_knee_l_trq);
	fprintf(fp_error,"Ankle_l_trq (adjusted):\t%lf (%lf)\n",mdata.err_ankle_l_trq,mdata.err_adjusted_ankle_l_trq);
	fprintf(fp_error,"Hip_l_pwr (adjusted):\t%lf (%lf)\n",mdata.err_hip_l_pwr,mdata.err_adjusted_hip_l_pwr);
	fprintf(fp_error,"Knee_l_pwr (adjusted):\t%lf (%lf)\n",mdata.err_knee_l_pwr,mdata.err_adjusted_knee_l_pwr);
	fprintf(fp_error,"Ankle_l_pwr (adjusted):\t%lf (%lf)\n",mdata.err_ankle_l_pwr,mdata.err_adjusted_ankle_l_pwr);
	fprintf(fp_error,"GrfZ_l (adjusted):\t%lf (%lf)\n",mdata.err_grfz_l,mdata.err_adjusted_grfz_l);
	fprintf(fp_error,"GrfX_l (adjusted):\t%lf (%lf)\n",mdata.err_grfx_l,mdata.err_adjusted_grfx_l);
	fprintf(fp_error,"GrfZ_r (adjusted):\t%lf (%lf)\n",mdata.err_grfz_r,mdata.err_adjusted_grfz_r);
	fprintf(fp_error,"GrfX_r (adjusted):\t%lf (%lf)\n",mdata.err_grfx_r,mdata.err_adjusted_grfx_r);
	fprintf(fp_error,"Ank_ang_vel (adjusted):\t%lf (%lf)\n",mdata.err_ankle_l_vel,mdata.err_adjusted_ankle_l_vel);
	fprintf(fp_error,"Pelvis_rot (adjusted):\t%lf (%lf)\n",mdata.err_pelvis_rot,mdata.err_adjusted_pelvis_rot);
	fprintf(fp_error,"MechEngy (adjusted):\t%lf (%lf)\n",mdata.err_segme,mdata.err_adjusted_segme);
	fprintf(fp_error,"Extpwr (adjusted):\t%lf (%lf)\n",mdata.err_extpwr,mdata.err_adjusted_extpwr);
	fprintf(fp_error,"SysME (adjusted):\t%lf (%lf)\n",mdata.err_sysme,mdata.err_adjusted_sysme);
	fprintf(fp_error,"SysKE (adjusted):\t%lf (%lf)\n",mdata.err_syske,mdata.err_adjusted_syske);
	fprintf(fp_error,"SysPE (adjusted):\t%lf (%lf)\n",mdata.err_syspe,mdata.err_adjusted_syspe);
	fprintf(fp_error,"MusPwr (adjusted):\t%lf (%lf)\n",mdata.total_muspower,mdata.total_adjusted_muspower);
    fprintf(fp_error,"MusExc (adjusted):\t%lf (%lf)\n",mdata.total_musexc,mdata.total_adjusted_musexc);
	fprintf(fp_error,"lGrfOn (adjusted):\t%lf (%lf)\n",mdata.err_Lgrf_on,mdata.err_Lgrf_on_adjusted);
	fprintf(fp_error,"NetMEE (adjusted):\t%lf (%lf)\n",mdata.NetSystemWork,mdata.err_adjusted_NetSystemWork);
	fprintf(fp_error,"NetTen (adjusted):\t%lf (%lf)\n",mdata.NetTendonWork,mdata.err_adjusted_NetTendonWork);
	fprintf(fp_error,"Met Cost (adjusted):\t%lf (%lf)\n",mdata.total_MusMetE,mdata.total_adjusted_MusMetE);
	fprintf(fp_error,"\n");

	fclose(fp_error);
						
	/***save initial conditions***/
	if((fp_init=fopen("SAinit.best","w"))==NULL){
		printf("init.state could not be opened\n");
		//	exit(0);
	}
	for(i=0; i<NQ; i++){
		fprintf(fp_init,"%lf %lf\n",mdata.init_y[i],
				mdata.init_dy[i]);
	}  
	fclose(fp_init);

	/*** save tracking data ***/
	if((fp_write=fopen("SAtrack.dat","w"))==NULL){
		printf("track.dat cannot be opened!!!");
	}

	for(i=0;i<N_track_step;i++){
		for(j=0;j<N_track;j++){
			fprintf(fp_write,"%lf ",mdata.track[j][i]);
		}
		fprintf(fp_write,"\n");
	}
	fclose(fp_write);

}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//save motion file

void save_motion(void)
{
	int i,j;
	FILE* fp_motion;

	fp_motion = fopen(mdata.motion_file_name,"w");
    fprintf(fp_motion,"name sd_motion\n");
    fprintf(fp_motion,"datacolumns %d\n", sdm.num_gencoords + sdm.num_muscles + 18);
    fprintf(fp_motion,"datarows %d\n", mdata.nstep);
    fprintf(fp_motion,"range 0.0 %lf\n", (double)mdata.nstep*mdata.dt);
    fprintf(fp_motion,"keys g_key\n");
    fprintf(fp_motion,"wrap\n");
    fprintf(fp_motion,"endheader\n\n");

	fprintf(fp_motion,"%s ", "ground_floor1_px");
    //for (i=0; i<sdm.nq; i++){
	for (i=1; i<sdm.nq; i++){//skip "leg_tx"
		if (sdm.q[i].type == unconstrained_q)			
         fprintf(fp_motion,"%s ", sdm.q[i].name);
		
	}
    fprintf(fp_motion,"\n");
    

    for (i=0; i<sdm.num_muscles; i++)
       fprintf(fp_motion,"%s ", sdm.muscles[i].name);
    fprintf(fp_motion,"\n");


	////added 1-16-08
	//right
	fprintf(fp_motion,"ground_force_px ground_force_py ground_force_pz ground_force_vx ground_force_vy ground_force_vz ground_force_cr ground_force_cg ground_force_cb "); 
	//left
	fprintf(fp_motion,"ground_force_px ground_force_py ground_force_pz ground_force_vx ground_force_vy ground_force_vz ground_force_cr ground_force_cg ground_force_cb ");



	for(i=0;i<mdata.nstep;i++){
		for (j=0; j<sdm.nq; j++){
			if (sdm.q[j].type == unconstrained_q){
			//fprintf(fp_motion,"%9.4lf ", mdata.motion_y[i][j]);
				if(j==0)//leg_tx, to move floor backward
					mdata.motion_y[i][j]=(-1.0)*mdata.motion_y[i][j];
			fprintf(fp_motion,"%lf ", mdata.motion_y[i][j]);
			}
		}
     	 
		for (j=0; j<sdm.num_muscles; j++){
	     //fprintf(fp_motion,"%7.5lf ", mdata.mus_dyn_act[i][j]);
		 fprintf(fp_motion,"%lf ", mdata.mus_dyn_act[i][j]);
		}

		/// added 1-16-08
		//adjust leg-tx
		mdata.Rcop[i][0]=mdata.Rcop[i][0]+mdata.motion_y[i][0]; //"+" because it has been adjusted above
		mdata.Lcop[i][0]=mdata.Lcop[i][0]+mdata.motion_y[i][0];

		for(j=0;j<3;j++){
			fprintf(fp_motion,"%lf ", mdata.Rcop[i][j]);
		}

		for(j=0;j<3;j++){
			fprintf(fp_motion,"%lf ", mdata.Rgrf[i][j]);
		}

		fprintf(fp_motion,"1 1 0 "); //color

		for(j=0;j<3;j++){
			fprintf(fp_motion,"%lf ", mdata.Lcop[i][j]);
		}

		for(j=0;j<3;j++){
			fprintf(fp_motion,"%lf ", mdata.Lgrf[i][j]);
		}

		fprintf(fp_motion,"0 1 1 "); //color
		/////







	fprintf(fp_motion,"\n");
	}
    

	fclose(fp_motion);
}


