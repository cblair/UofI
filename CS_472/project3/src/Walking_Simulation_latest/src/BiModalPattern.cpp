/********************* BiModalPattern.c ********************************************/
/*********************************************************************************/
/** The functions within this file can be used to implement a bi-modal Henning excitation **/
/** patterns within the existing optimization framework created for 2D walking. **/
/*********************************************************************************/
/********** Created by Craig McGowan: Mar2009 **************************************/
/*********************************************************************************/

/********Currently not set up to handle assymetry*********************************/
#include <math.h>
#include "SA.h"
#include "SAstructs.h"
#include "system_params.h" //required for change_init
#include "ExcGroups.h"
#include "universal.h"

extern "C" OptStruct mdata;

//Functions within this file




//external global variables (declared in other files)
extern SDModelStruct sdm; //declared in simann.c (main model structure)
extern OptStruct mdata; //declared in main.c (main optimization data structure)


/******************************************************************************/
/******************** generate_bimodal_patterns  ******************************/
/******************************************************************************/
/*** this function updates the muscle group templates based on the          ***/
/*** optimization parameters obtained from the simulated annealing program. ***/
/*** It receives the optimal parameters for 1 leg and assumes symmetry      ***/
/*** returns individual muscle excitation patterns to the forward simulation.       ***/
/*Modified from Jeff Rankin - Oct 2008	by C.McGowan April 2009 			***/
/******************************************************************************/
void update_BiModal_Pattern(double **group_exc_xR, double**group_exc_yR,int m,int flag_left){

//Does not use muscle pattern types.
//All muscle groups have 6 params: onset1, offset1, amplitude1, onset2, offset2, amplitude2



//	int m; //used exclusively to cycle through the muscle groups
	int i;	//indexing
	int n; // indexing the number of waveforms (nodes)
	int opt_param_num = 0;	//keeps track of the optimal parameter number to be assigned
	int total_excite_params = 0;	//used to verify the number of optimization parameters is the same as that required by the muscle group templates
	int step=0; //another indexer
	double peakEXC=0;

	double temp_exc_x[2][N_emg_step], temp_exc_x2[2][N_emg_step];
	int temp_exc_y[2][N_emg_step]; //these arrays temporarily hold the complete excitation curve for a single muscle group
	double exc_x[N_emg_step], EXC_x[N_emg_step];
	//double **group_exc_xR, **group_exc_yR; //these arrays store the final muscle group excitation patterns which are used to assign excitation values to individual muscles (right leg)
	//double **group_exc_xL, **group_exc_yL; //these arrays store the final muscle group excitation patterns which are used to assign excitation values to individual muscles (left leg)														   

	int onset[2], offset[2];
	double amp[2]; //arrays holding parameters for each muscle group
	int onset_temp, offset_temp;


	//int NumGrps_Leg;//Keeps track of the number of muscle groups per leg

	//FILE *data_file1; //saves the final excitation pattern (all muscles) to file for data checking

//	printf("Here\n");
	//this sets the number of muscle groups per leg (used for cycling between left and right legs)
/*	if (SYMMETRIC == 1){
		NumGrps_Leg = N_musG;
	}
	else{
		if (SYMMETRIC == 0){
			NumGrps_Leg = N_musG/2;
		}
	}*/	
	
	//allocate memory for the group excitation arrays


	
	
	//go through the muscle groups and assign the appropriate excitation parameter values for each muscle

	step=0;	
//	printf("\nparams=%f,%f,%f,%f,%f,%f\n",mdata.ExcPatterns[m].excite_params[0],mdata.ExcPatterns[m].excite_params[1],
//				mdata.ExcPatterns[m].excite_params[2],mdata.ExcPatterns[m].excite_params[3],mdata.ExcPatterns[m].excite_params[4],
//				mdata.ExcPatterns[m].excite_params[5]);


	for(n=0;n<2;n++){

			onset[n]=mdata.ExcPatterns[m].excite_params[step];
			offset[n]=mdata.ExcPatterns[m].excite_params[step+1];
			amp[n]=mdata.ExcPatterns[m].excite_params[step+2];
			step=step+3;
			
		}
//	printf("amp=%f, %f\n",amp[0],amp[1]);





		for (n=0;n<2;n++){ // number of nodes making up pattern
		//clear the temporary excitation values
		for (i=0;i<N_emg_step;i++){
			temp_exc_x[n][i] = 0.0;
			temp_exc_x2[n][i] = 0.0;
			temp_exc_y[n][i] = i;
		}
			
			if (offset[n] - onset[n] >0){ // Burst occures in one cycle

				for (i=0;i<N_emg_step;i++){

					//double cosBlock=(temp_exc_y[n][i]-onset[n])/(offset[n]-onset[n])*(2*3.14159);
					double num = (temp_exc_y[n][i]-onset[n]);
					double denom=(offset[n]-onset[n]);
					double cosBlock=(num/denom)*(2*3.14159);


				temp_exc_x[n][i] = (amp[n]/-2)  *  cos(cosBlock) + (amp[n]/2);
			//	printf("EXC=%f\n",temp_exc_x[n][i]);
			//	getchar();
				//		temp_exc_x[n][i] = ((amp[n]/-2)  *  cos(((temp_exc_y[n][i]-onset[n])/(offset[n]-onset[n]))*(2*3.14159))) + (amp[n]/2);
					if (temp_exc_y[n][i] < onset[n] || temp_exc_y[n][i] > offset[n]){
						temp_exc_x[n][i] = 0;
					}
				}
				
			}
			else if(offset[n] - onset[n] <0){ // Burst spans 2 cycles 
				onset_temp = 0;
				offset_temp = (100-onset[n]) + offset[n];

				for (i=0;i<N_emg_step;i++){

				//	double cosBlock2=(temp_exc_y[n][i]-onset_temp)/(offset_temp-onset_temp)*(2*3.14159);
					double num = (temp_exc_y[n][i]-onset_temp);
					double denom=(offset_temp-onset_temp);
					double cosBlock2=(num/denom)*(2*3.14159);



				temp_exc_x2[n][i] = (amp[n]/-2)  *  cos(cosBlock2) + (amp[n]/2);
				//	temp_exc_x2[n][i] = ((amp[n]/-2)  *  cos(((temp_exc_y[n][i]-onset_temp)/(offset_temp-onset_temp))*(2*3.14159))) + (amp[n]/2);
							//	
					if (temp_exc_y[n][i] < onset_temp || temp_exc_y[n][i] > offset_temp){
						temp_exc_x2[n][i] = 0;
					}
					
				}
				for (i=onset[n]; i<N_emg_step; i++){
						temp_exc_x[n][i]=temp_exc_x2[n][i-onset[n]];
				}
				for (i=0; i<offset[n]; i++){
					temp_exc_x[n][i]=temp_exc_x2[n][offset_temp-offset[n] +i];
				}
					
			}		
	
		}


		for (i=0;i<N_emg_step;i++){
			exc_x[i]=0.0;
			EXC_x[i]=0.0;
		}



		for (i=0;i<N_emg_step;i++){
			exc_x[i] = temp_exc_x[0][i] + temp_exc_x[1][i];
			//	printf("EXC=%f\n",temp_exc_x[1][i]);
			//	getchar();
		}

		
		for (i=0;i<N_emg_step;i++){// check to see if excitation is greater than 1
			if(exc_x[i] > peakEXC){
				peakEXC = exc_x[i];
			}
		}
		
//		printf("Peak=%f\n",peakEXC);
		if (peakEXC > 1.0){ // if peak is greater than 1, normalize pattern to have peak at 1
				
			for (i=0;i<N_emg_step;i++){
				EXC_x[i] = exc_x[i]/peakEXC;
	
			}
		}
		else{
			for (i=0;i<N_emg_step;i++){
			EXC_x[i] = exc_x[i];
			}
		}
		
		
		for (i=0;i<N_emg_step;i++){
			group_exc_xR[m][i] = temp_exc_y[0][i];
			group_exc_yR[m][i] = 	EXC_x[i];
			
		}
	


}	
		
		
		
	
	
