//This file replaces Ko's SetPattern.cpp file and the function read_cut_EMG in FileReadWrite.cpp
//Created by Jeff Rankin - Oct 2008
#include <iostream>
#include "SA.h"
#include "SAstructs.h"
#include "structs.h"
#include "ExcGroups.h"

extern "C" OptStruct mdata; //declared in formain.c
extern "C" SDModelStruct sdm; //declared in sdfor.c

//functions in this file
void set_pattern_sources(void); //contains user flags to set the pattern sources
void set_pattern_types(void);	//contains user flags to set the pattern types
void get_cut_values(void);		//contains user flags to set the cut times for exper EMG data

void init_Exc_templates(void);	//this is the main function to allocate space and set up the excitation templates for each muscle group
void update_EMG_Exc_templates(void);	//this function updates the initial templates excitation values based on the pattern source, it also 
										//reads in the exper EMG data if necessary.
void assign_muscle_excitations(double **group_exc_xR,double **group_exc_yR, double **group_exc_xL,double **group_exc_yL,
							   double **XR,double **YR,double **XL,double **YL);	//assigns the individual muscle excitation patterns based on its muscle group


/***************************************************************/
/***************** assign_muscle_excitations *******************/
/***************************************************************/
/*** This function assigns muscle excitations to individual  ***/
/*** muscles based on its excitation group.                  ***/
/***					Created by Jeff Rankin - Oct 2008    ***/
/***************************************************************/
void assign_muscle_excitations(double **group_exc_xR,double **group_exc_yR,
							   double **group_exc_xL,double **group_exc_yL,
							   double **XR,double **YR,double **XL,double **YL){

	int m,g,i;

	int NumGrps_Leg = N_musG;//Keeps track of the number of muscle groups per leg

	//This first section is just a bunch of flags to assign specific muscles to the appropriate excitation group

	//right side muscles
	sdm.muscles[iliac_r].exc_group = ILG;
	sdm.muscles[psoas_r].exc_group = ILG;

//	sdm.muscles[adlng_r].exc_group = ALG_R;

	sdm.muscles[addmag_r].exc_group = GMAXG;
//	sdm.muscles[admg1_r].exc_group = AMG_R;
//	sdm.muscles[admg2_r].exc_group = AMG_R;
//	sdm.muscles[admg3_r].exc_group = AMG_R;

//	sdm.muscles[sar_r].exc_group = SARG_R;

	sdm.muscles[rf_r].exc_group = RFG;

//	sdm.muscles[mvas_r].exc_group = VMG_R;
//
//	sdm.muscles[lvas_r].exc_group = VLG_R;
//	sdm.muscles[ivas_r].exc_group = VLG_R;

	sdm.muscles[vas1_r].exc_group = VMG;

	sdm.muscles[vas2_r].exc_group = VLG;
	sdm.muscles[vas3_r].exc_group = VLG;

//	sdm.muscles[gmed1_r].exc_group = GMEDG_R;
//	sdm.muscles[gmed2_r].exc_group = GMEDG_R;
//	sdm.muscles[gmed3_r].exc_group = GMEDG_R;

	sdm.muscles[gmeda_r].exc_group = GMEDG;
	sdm.muscles[gmedp_r].exc_group = GMEDG;


//	sdm.muscles[tfl_r].exc_group = TFLG_R;

//	sdm.muscles[gmax1_r].exc_group = GMAXG_R;
//	sdm.muscles[gmax2_r].exc_group = GMAXG_R;
//	sdm.muscles[gmax3_r].exc_group = GMAXG_R;
	sdm.muscles[gmax_r].exc_group = GMAXG;

	sdm.muscles[sm_r].exc_group = HAMG;

	sdm.muscles[bflh_r].exc_group = HAMG;

	sdm.muscles[bfsh_r].exc_group = BFSHG;

	sdm.muscles[mgas_r].exc_group = GASG;

	sdm.muscles[lgas_r].exc_group = GASG;

	sdm.muscles[sol_r].exc_group = SOLG;
	sdm.muscles[tp_r].exc_group = SOLG;
	sdm.muscles[flxdig_r].exc_group = FLXDIGG;

	sdm.muscles[ta_r].exc_group = TAG;
	sdm.muscles[extdig_r].exc_group = EXTDIGG;

	sdm.muscles[perlng_r].exc_group = PERLNGG;
	sdm.muscles[flxhal_r].exc_group = FLXDIGG;
	sdm.muscles[pertert_r].exc_group = TAG;
	sdm.muscles[exthal_r].exc_group = EXTDIGG;
	sdm.muscles[perbrev_r].exc_group = PERLNGG;



	//if the patterns are not symmetric assign the left muscle excitations based on the muscle groups
	if (SYMMETRIC == 0){

	sdm.muscles[iliac_l].exc_group = ILG;
	sdm.muscles[psoas_l].exc_group = ILG;

//	sdm.muscles[adlng_l].exc_group = ALG_L;

	sdm.muscles[addmag_l].exc_group = GMAXG;
//	sdm.muscles[admg1_r].exc_group = AMG_R;
//	sdm.muscles[admg2_r].exc_group = AMG_R;
//	sdm.muscles[admg3_r].exc_group = AMG_R;

//	sdm.muscles[sar_r].exc_group = SARG_R;

	sdm.muscles[rf_l].exc_group = RFG;

//	sdm.muscles[mvas_r].exc_group = VMG_R;
//
//	sdm.muscles[lvas_r].exc_group = VLG_R;
//	sdm.muscles[ivas_r].exc_group = VLG_R;

	sdm.muscles[vas1_l].exc_group = VMG;

	sdm.muscles[vas2_l].exc_group = VLG;
	sdm.muscles[vas3_l].exc_group = VLG;

//	sdm.muscles[gmed1_r].exc_group = GMEDG_R;
//	sdm.muscles[gmed2_r].exc_group = GMEDG_R;
//	sdm.muscles[gmed3_r].exc_group = GMEDG_R;

	sdm.muscles[gmeda_l].exc_group = GMEDG;
	sdm.muscles[gmedp_l].exc_group = GMEDG;


//	sdm.muscles[tfl_r].exc_group = TFLG_R;

//	sdm.muscles[gmax1_r].exc_group = GMAXG_R;
//	sdm.muscles[gmax2_r].exc_group = GMAXG_R;
//	sdm.muscles[gmax3_r].exc_group = GMAXG_R;
	sdm.muscles[gmax_l].exc_group = GMAXG;

	sdm.muscles[sm_l].exc_group = HAMG;

	sdm.muscles[bflh_l].exc_group = HAMG;

	sdm.muscles[bfsh_l].exc_group = BFSHG;

	sdm.muscles[mgas_l].exc_group = GASG;

	sdm.muscles[lgas_l].exc_group = GASG;

	sdm.muscles[sol_l].exc_group = SOLG;
	sdm.muscles[tp_l].exc_group = SOLG;
	sdm.muscles[flxdig_l].exc_group = FLXDIGG;

	sdm.muscles[ta_l].exc_group = TAG;
	sdm.muscles[extdig_l].exc_group = EXTDIGG;

	sdm.muscles[perlng_l].exc_group = PERLNGG;
	sdm.muscles[flxhal_l].exc_group = FLXDIGG;
	sdm.muscles[pertert_l].exc_group = TAG;
	sdm.muscles[exthal_l].exc_group = EXTDIGG;
	sdm.muscles[perbrev_l].exc_group = PERLNGG;

		NumGrps_Leg = N_musG/2; //makes sure that the number of muscle groups in the for loop below works appropriately if non-symmetry is used
	}//end if SYMMETRIC
	

	//now that the musles have been assigned to groups, assign the appropriate excitation pattern to the individual muscles
	for (m=0;m<N_mus;m++){//cycle through the muscles (right leg)
		for (g=0;g<NumGrps_Leg;g++){//cycle through the muscle groups		
			//detects if the muscle is in the current excitation group
			if (sdm.muscles[m].exc_group == g){//it is in the group, assign excitation
				for (i=0;i<N_emg_step;i++){//cycle through the individual excitation points				
					XR[m][i] = group_exc_xR[g][i];
					YR[m][i] = group_exc_yR[g][i];					
				}//end for loop through points(i) 
				break; //breaks out of the muscle group loop after excitation pattern is assigned
			}//end if
		}//end for loop through muscle groups (g)
	}//end for loop through muscles (right side)

	if (SYMMETRIC == 1){
		for (m=0;m<N_mus;m++){//cycle through the muscles (left leg)
			for (g=0;g<NumGrps_Leg;g++){//cycle through the muscle groups		
				//detects if the muscle is in the current excitation group
				if (sdm.muscles[m].exc_group == g){//it is in the group, assign excitation
					for (i=0;i<N_emg_step;i++){//cycle through the individual excitation points				
						XL[m][i] = group_exc_xL[g][i];
						YL[m][i] = group_exc_yL[g][i];					
					}//end for loop through points(i) 
					break; //breaks out of the muscle group loop after excitation pattern is assigned
				}//end if
			}//end for loop through muscle groups (g)
		}//end for loop through muscles (left side)

	}//end SYMMETRIC == 1


	if (SYMMETRIC == 0){
		for (m=0;m<N_mus;m++){//cycle through the muscles (left leg)
			for (g=0;g<NumGrps_Leg;g++){//cycle through the muscle groups		
				//detects if the muscle is in the current excitation group
				if (sdm.muscles[m+N_mus].exc_group == (g+NumGrps_Leg) ){//it is in the group, assign excitation
					for (i=0;i<N_emg_step;i++){//cycle through the individual excitation points				
						XL[m][i] = group_exc_xL[g][i];
						YL[m][i] = group_exc_yL[g][i];					
					}//end for loop through points(i) 
					break; //breaks out of the muscle group loop after excitation pattern is assigned
				}//end if
			}//end for loop through muscle groups (g)
		}//end for loop through muscles (right side)
	}//end if SYMMETRIC == 0

}//end assign_muscle_excitations
/*************** end assign_muscle_excitations *******************/




/******************************************************************/
/******************* set_pattern_sources **************************/
/******************************************************************/
/*** This function contains a list of user input flags indicat- ***/
/*** ing the excitation source for each muscle group (e.g. exper***/
/*** EMG).					Created by Jeff Rankin - Oct 2008   ***/
/******************************************************************/
void set_pattern_sources(void){	

	//Current Sources Implemented
		// 1 = EMG pattern.  This will create an excitation pattern that is based on experimental EMG
		// 2 = Block pattern.  This will create a pattern that is all on or all off
		// 3 = Gaussian
		// 4 = bimodel henning

	//Set the pattern sources here (1 flag for each muscle group)
	mdata.ExcPatterns[ILG].pattern_source = 4;
//	mdata.ExcPatterns[ALG_R].pattern_source = 3;
//	mdata.ExcPatterns[GMAX].pattern_source = 3;
//	mdata.ExcPatterns[SARG_R].pattern_source = 3;
	mdata.ExcPatterns[RFG].pattern_source = 4;//1;  
	mdata.ExcPatterns[VMG].pattern_source = 4;//1;
	mdata.ExcPatterns[VLG].pattern_source = 4;//1; // from VM pattern
	mdata.ExcPatterns[GMEDG].pattern_source = 4;//1;
//	mdata.ExcPatterns[TFLG_R].pattern_source = 3;
	mdata.ExcPatterns[GMAXG].pattern_source = 4;
	mdata.ExcPatterns[HAMG].pattern_source = 4;//1;
//	mdata.ExcPatterns[BFLHG_R].pattern_source = 1;
	mdata.ExcPatterns[BFSHG].pattern_source = 4;
	mdata.ExcPatterns[GASG].pattern_source = 4;//1;
//	mdata.ExcPatterns[LGG_R].pattern_source = 1; //from MG pattern
	mdata.ExcPatterns[SOLG].pattern_source = 4;//1;
	mdata.ExcPatterns[TAG].pattern_source = 4;//1;
	mdata.ExcPatterns[PERLNGG].pattern_source = 4;//1;
	mdata.ExcPatterns[FLXDIGG].pattern_source = 4;//1;
	mdata.ExcPatterns[EXTDIGG].pattern_source = 4;//1;



/*	//These pattern sources must be set properly if using assymetric left and right leg excitation patterns
	if (SYMMETRIC == 0){
		mdata.ExcPatterns[ILG_L].pattern_source = 3;
		mdata.ExcPatterns[ALG_L].pattern_source = 3;
		mdata.ExcPatterns[AMG_L].pattern_source = 3;
		mdata.ExcPatterns[SARG_L].pattern_source = 3;
		mdata.ExcPatterns[RFG_L].pattern_source = 1;
		mdata.ExcPatterns[VMG_L].pattern_source = 1;
		mdata.ExcPatterns[VLG_L].pattern_source = 1; // from VM pattern
		mdata.ExcPatterns[GMEDG_L].pattern_source = 1;
		mdata.ExcPatterns[TFLG_L].pattern_source = 3;
		mdata.ExcPatterns[GMAXG_L].pattern_source = 3;
		mdata.ExcPatterns[MHG_L].pattern_source = 1;
		mdata.ExcPatterns[BFLHG_L].pattern_source = 1;
		mdata.ExcPatterns[BFSHG_L].pattern_source = 3;
		mdata.ExcPatterns[MGG_L].pattern_source = 1;
		mdata.ExcPatterns[LGG_L].pattern_source = 1; // from MG pattern
		mdata.ExcPatterns[SOLG_L].pattern_source = 1;
		mdata.ExcPatterns[TAG_L].pattern_source = 1;
	}//end if symmetric == 0
*/
}//end set_pattern_sources
/************************ end set_pattern_sources **********************/


/***********************************************************************/
/*************************** set_pattern_types *************************/
/***********************************************************************/
/*** This function contains a list of user input flags indicating the***/
/*** excitation pattern type for each muscle group (e.g. On,Off,On   ***/
/*** excitation pattern).     Created by Jeff Rankin - Oct 2008      ***/
/***********************************************************************/

void set_pattern_types(void){

	//The first three pattern types were obtained from code written by Ko Sasaki.  These are:
		//1 = One excitation region pattern (off,on,off).  Requires 3 optimization parameters (on, off, mag).
		//2 = Two excitation region pattern (on,off,on).  Requires 3 optimization parameters (on, off, mag).
		//3 = Three excitation region pattern (on,off,on,off,on).  Requires 6 optimization parameters (on,off,on,off,on,mag1,mag2).
	
	//Note: If you change the pattern type, it is imperative that you also change the parameter file,
	//the parameter bounds,  and the cut times.

	//Set the pattern types here (1 flag for each muscle group)
	mdata.ExcPatterns[ILG].pattern_type = 1;
//	mdata.ExcPatterns[ALG_R].pattern_type = 1;
//	mdata.ExcPatterns[AMG_R].pattern_type = 1;
//	mdata.ExcPatterns[SARG_R].pattern_type = 1;
	mdata.ExcPatterns[RFG].pattern_type = 2;//3;
	mdata.ExcPatterns[VMG].pattern_type = 2;
	mdata.ExcPatterns[VLG].pattern_type = 2;
	mdata.ExcPatterns[GMEDG].pattern_type = 2; 
//	mdata.ExcPatterns[TFLG_R].pattern_type = 1;
	mdata.ExcPatterns[GMAXG].pattern_type = 2;
	mdata.ExcPatterns[HAMG].pattern_type = 2;
//	mdata.ExcPatterns[BFLHG_R].pattern_type = 2;
	mdata.ExcPatterns[BFSHG].pattern_type = 2;
	mdata.ExcPatterns[GASG].pattern_type = 2; // 1 for controls, 2 for IMH26
//	mdata.ExcPatterns[LGG_R].pattern_type = 2; // 1 for controls, 2 for IMH26
	mdata.ExcPatterns[SOLG].pattern_type = 2; // 1 for controls, 2 for IMH26
	mdata.ExcPatterns[TAG].pattern_type = 2; 
	mdata.ExcPatterns[PERLNGG].pattern_type = 2;
	mdata.ExcPatterns[FLXDIGG].pattern_type = 2;
	mdata.ExcPatterns[EXTDIGG].pattern_type = 2;



/*	//These pattern types must be set properly if using assymetric left and right leg excitation patterns
	if (SYMMETRIC == 0){
		mdata.ExcPatterns[ILG_L].pattern_type = 2;
		mdata.ExcPatterns[ALG_L].pattern_type = 2;
		mdata.ExcPatterns[AMG_L].pattern_type = 2;
		mdata.ExcPatterns[SARG_L].pattern_type = 2;
		mdata.ExcPatterns[RFG_L].pattern_type = 3;
		mdata.ExcPatterns[VMG_L].pattern_type = 1;
		mdata.ExcPatterns[VLG_L].pattern_type = 1;
		mdata.ExcPatterns[GMEDG_L].pattern_type =2; // 1 for controls, 2 for IMH26
		mdata.ExcPatterns[TFLG_L].pattern_type = 2;
		mdata.ExcPatterns[GMAXG_L].pattern_type = 1;
		mdata.ExcPatterns[MHG_L].pattern_type = 1;
		mdata.ExcPatterns[BFLHG_L].pattern_type = 2; // 1 for controls, 2 for IMH26
		mdata.ExcPatterns[BFSHG_L].pattern_type = 1;
		mdata.ExcPatterns[MGG_L].pattern_type = 2; 
		mdata.ExcPatterns[LGG_L].pattern_type = 2; 
		mdata.ExcPatterns[SOLG_L].pattern_type = 2;
		mdata.ExcPatterns[TAG_L].pattern_type = 1;
	}
*/
}//end set_pattern_types

/********************* end set_pattern_types ***********************/

/*******************************************************************/
/************************ get_cut_values ***************************/
/*******************************************************************/
/*** This function contains a list of user input flags indicating***/
/*** the cut times for each of the muscle groups.  Even if exper ***/
/*** EMG is not used, 'dummy' cut times must be set up properly  ***/
/*** to make sure that the other pattern source templates are    ***/
/*** initialized properly (e.g. a type 1 muscle must always have ***/
/*** its 'cut1' value lower than its 'cut2' value.  All of the   ***/
/*** values input here should go from 0 to 100 (for percent of a ***/
/*** cycle) and be integer values. Created By Jeff Rankin Oct 2008**/
/*******************************************************************/

void get_cut_values(void){

	//Implement the times (for cutting the EMG data), these should be modified by the user as necessary
	//based on the specific experimental EMG data file and/or changes in pattern type.  Type 1 and 3
	//patterns should always have its first cut time as 0, and the last cut time as 100.
	
	
	//ILG_R - Type 1
	//excitation region
	mdata.ExcPatterns[ILG].onBlock[0].cut1 = 50;
	mdata.ExcPatterns[ILG].onBlock[0].cut2 = 80;

	//ALG_R - Type 1
	//excitation region
//	mdata.ExcPatterns[ALG_R].onBlock[0].cut1 = 50;
//	mdata.ExcPatterns[ALG_R].onBlock[0].cut2 = 80;

	/*//AMG_R - Type 2
	//first excitation region
	mdata.ExcPatterns[AMG_R].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[AMG_R].onBlock[0].cut2 = 19;
	//second excitation region
	mdata.ExcPatterns[AMG_R].onBlock[1].cut1 = 88;
	mdata.ExcPatterns[AMG_R].onBlock[1].cut2 = 100;*/

	//AMG_R - Type 1
	//excitation region
//	mdata.ExcPatterns[AMG_R].onBlock[0].cut1 = 50;
//	mdata.ExcPatterns[AMG_R].onBlock[0].cut2 = 80;

	//SARG_R - Type 1
	//excitation region
//	mdata.ExcPatterns[SARG_R].onBlock[0].cut1 = 50;
//	mdata.ExcPatterns[SARG_R].onBlock[0].cut2 = 80;

/*	//RFG_R - Type 3
	//first excitation region
	mdata.ExcPatterns[RFG].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[RFG].onBlock[0].cut2 = 33;
	//second excitation region
	mdata.ExcPatterns[RFG].onBlock[1].cut1 = 63; 
	mdata.ExcPatterns[RFG].onBlock[1].cut2 = 81; 
	//third excitation region
	mdata.ExcPatterns[RFG].onBlock[2].cut1 = 99; 
	mdata.ExcPatterns[RFG].onBlock[2].cut2 = 100;
*/
	//RFG - Type 2
	//first excitation region
	mdata.ExcPatterns[RFG].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[RFG].onBlock[0].cut2 = 40;
	//second excitation region
	mdata.ExcPatterns[RFG].onBlock[1].cut1 = 95; 
	mdata.ExcPatterns[RFG].onBlock[1].cut2 = 100; 


	//VMG_R - Type 2
	//first excitation region
	mdata.ExcPatterns[VMG].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[VMG].onBlock[0].cut2 = 40;
	//second excitation region
	mdata.ExcPatterns[VMG].onBlock[1].cut1 = 99;
	mdata.ExcPatterns[VMG].onBlock[1].cut2 = 100;

	//VLG_R - Type 2
	//first excitation region
	mdata.ExcPatterns[VLG].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[VLG].onBlock[0].cut2 = 40;
	//second excitation region
	mdata.ExcPatterns[VLG].onBlock[1].cut1 = 95;
	mdata.ExcPatterns[VLG].onBlock[1].cut2 = 100;

	//GMEDG_R - Type 2 
	//first excitation region
	mdata.ExcPatterns[GMEDG].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[GMEDG].onBlock[0].cut2 = 40;
	//second excitation region
	mdata.ExcPatterns[GMEDG].onBlock[1].cut1 = 99;
	mdata.ExcPatterns[GMEDG].onBlock[1].cut2 = 100;
	
	/*//TFLG_R - Type 2
	//first excitation region
	mdata.ExcPatterns[TFLG_R].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[TFLG_R].onBlock[0].cut2 = 30;
	//second excitation region
	mdata.ExcPatterns[TFLG_R].onBlock[1].cut1 = 85;
	mdata.ExcPatterns[TFLG_R].onBlock[1].cut2 = 100;*/

	//TFLG_R - Type 1
	//excitation region
//	mdata.ExcPatterns[TFLG_R].onBlock[0].cut1 = 50;
//	mdata.ExcPatterns[TFLG_R].onBlock[0].cut2 = 80;

	//GMAXG_R - Type 2
	//first excitation region
	mdata.ExcPatterns[GMAXG].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[GMAXG].onBlock[0].cut2 = 19;
	//second excitation region
	mdata.ExcPatterns[GMAXG].onBlock[1].cut1 = 88;
	mdata.ExcPatterns[GMAXG].onBlock[1].cut2 = 100;

	//MHG_R - Type 2
	//first excitation region
	mdata.ExcPatterns[HAMG].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[HAMG].onBlock[0].cut2 = 46; 
	//second excitation region
	mdata.ExcPatterns[HAMG].onBlock[1].cut1 = 82;
	mdata.ExcPatterns[HAMG].onBlock[1].cut2 = 100;

	//BFLHG_R - Type 2
	//first excitation region
//	mdata.ExcPatterns[BFLHG_R].onBlock[0].cut1 = 0;
//	mdata.ExcPatterns[BFLHG_R].onBlock[0].cut2 = 48;
	//second excitation region
//	mdata.ExcPatterns[BFLHG_R].onBlock[1].cut1 = 81;
//	mdata.ExcPatterns[BFLHG_R].onBlock[1].cut2 = 100;

	//BFSH_R - Type 2
	//first excitation region
	mdata.ExcPatterns[BFSHG].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[BFSHG].onBlock[0].cut2 = 25;
	//second excitation region
	mdata.ExcPatterns[BFSHG].onBlock[1].cut1 = 60;
	mdata.ExcPatterns[BFSHG].onBlock[1].cut2 = 100;

	//MGG_R - Type 2 (for IMH26)
	//first excitation region
	mdata.ExcPatterns[GASG].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[GASG].onBlock[0].cut2 = 56;
	//second excitation region
	mdata.ExcPatterns[GASG].onBlock[1].cut1 = 99;
	mdata.ExcPatterns[GASG].onBlock[1].cut2 = 100;

	/*//MGG_R - Type 1 (for Controls)
	//excitation region
	mdata.ExcPatterns[MGG_R].onBlock[0].cut1 = 20; //10;
	mdata.ExcPatterns[MGG_R].onBlock[0].cut2 = 52; //60;
	*/

	//LGG_R - Type 2 (for IMH26)
	//first excitation region
//	mdata.ExcPatterns[LGG_R].onBlock[0].cut1 = 0;
//	mdata.ExcPatterns[LGG_R].onBlock[0].cut2 = 56;
	//second excitation region
//	mdata.ExcPatterns[LGG_R].onBlock[1].cut1 = 99;
//	mdata.ExcPatterns[LGG_R].onBlock[1].cut2 = 100;

	/*
	//LGG_R - Type 1 (for Controls)
	//excitation region
	mdata.ExcPatterns[LGG_R].onBlock[0].cut1 = 10;
	mdata.ExcPatterns[LGG_R].onBlock[0].cut2 = 60;
	*/

	//SOLG_R - Type 2 (for IMH26)
	//first excitation region
	mdata.ExcPatterns[SOLG].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[SOLG].onBlock[0].cut2 = 52;
	//second excitation region
	mdata.ExcPatterns[SOLG].onBlock[1].cut1 = 94;
	mdata.ExcPatterns[SOLG].onBlock[1].cut2 = 100;

	/*
	//SOLG_R - Type 1 (for Controls)
	//excitation region
	mdata.ExcPatterns[SOLG_R].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[SOLG_R].onBlock[0].cut2 = 58;
	*/

	//TAG_R - Type 2
	//first excitation region
	mdata.ExcPatterns[TAG].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[TAG].onBlock[0].cut2 = 30;
	//second excitation region
	mdata.ExcPatterns[TAG].onBlock[1].cut1 = 59; 
	mdata.ExcPatterns[TAG].onBlock[1].cut2 = 100;

		//PERLNG - Type 2
	//first excitation region
	mdata.ExcPatterns[PERLNGG].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[PERLNGG].onBlock[0].cut2 = 52;
	//second excitation region
	mdata.ExcPatterns[PERLNGG].onBlock[1].cut1 = 94;
	mdata.ExcPatterns[PERLNGG].onBlock[1].cut2 = 100;

		//FLXDIGG - Type 2 (for IMH26)
	//first excitation region
	mdata.ExcPatterns[FLXDIGG].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[FLXDIGG].onBlock[0].cut2 = 52;
	//second excitation region
	mdata.ExcPatterns[FLXDIGG].onBlock[1].cut1 = 94;
	mdata.ExcPatterns[FLXDIGG].onBlock[1].cut2 = 100;


		//EXTDIGG - Type 2
	//first excitation region
	mdata.ExcPatterns[EXTDIGG].onBlock[0].cut1 = 0;
	mdata.ExcPatterns[EXTDIGG].onBlock[0].cut2 = 30;
	//second excitation region
	mdata.ExcPatterns[EXTDIGG].onBlock[1].cut1 = 59; 
	mdata.ExcPatterns[EXTDIGG].onBlock[1].cut2 = 100;
	

/*	//If using assymetric left and right legs then these cut times must be set.  These values dont change SYMMETRIC legs.
	if (SYMMETRIC == 0){
		
		//ILG_L - Type 2
		//first excitation region
		mdata.ExcPatterns[ILG_L].onBlock[0].cut1 = 0;
		mdata.ExcPatterns[ILG_L].onBlock[0].cut2 = 50;
		//second excitation region
		mdata.ExcPatterns[ILG_L].onBlock[1].cut1 = 80;
		mdata.ExcPatterns[ILG_L].onBlock[1].cut2 = 100;

		//ALG_L - Type 2
		//first excitation region
		mdata.ExcPatterns[ALG_L].onBlock[0].cut1 = 0;
		mdata.ExcPatterns[ALG_L].onBlock[0].cut2 = 50;
		//second excitation region
		mdata.ExcPatterns[ALG_L].onBlock[1].cut1 = 80;
		mdata.ExcPatterns[ALG_L].onBlock[1].cut2 = 100;
*/
		/*//AMG_L - Type 1
		//excitation region
		mdata.ExcPatterns[AMG_L].onBlock[0].cut1 = 19;
		mdata.ExcPatterns[AMG_L].onBlock[0].cut2 = 88;*/
/*		
		//AMG_L - Type 2
		//first excitation region
		mdata.ExcPatterns[AMG_L].onBlock[0].cut1 = 0;
		mdata.ExcPatterns[AMG_L].onBlock[0].cut2 = 50;
		//second excitation region
		mdata.ExcPatterns[AMG_L].onBlock[1].cut1 = 80;
		mdata.ExcPatterns[AMG_L].onBlock[1].cut2 = 100;

		//SARG_L - Type 2
		//first excitation region
		mdata.ExcPatterns[SARG_L].onBlock[0].cut1 = 0;
		mdata.ExcPatterns[SARG_L].onBlock[0].cut2 = 50;
		//second excitation region
		mdata.ExcPatterns[SARG_L].onBlock[1].cut1 = 80;
		mdata.ExcPatterns[SARG_L].onBlock[1].cut2 = 100;

		//RFG_L - Type 3
		//first excitation region
		mdata.ExcPatterns[RFG_L].onBlock[0].cut1 = 0;
		mdata.ExcPatterns[RFG_L].onBlock[0].cut2 = 30;
		//second excitation region
		mdata.ExcPatterns[RFG_L].onBlock[1].cut1 = 40;
		mdata.ExcPatterns[RFG_L].onBlock[1].cut2 = 78;
		//third excitation region
		mdata.ExcPatterns[RFG_L].onBlock[2].cut1 = 89;
		mdata.ExcPatterns[RFG_L].onBlock[2].cut2 = 100;

		//VMG_L - Type 1
		//excitation region
		mdata.ExcPatterns[VMG_L].onBlock[0].cut1 = 25;
		mdata.ExcPatterns[VMG_L].onBlock[0].cut2 = 85;		

		//VLG_L - Type 1
		//excitation region
		mdata.ExcPatterns[VLG_L].onBlock[0].cut1 = 25;
		mdata.ExcPatterns[VLG_L].onBlock[0].cut2 = 85;

		//GMEDG_L - Type 2 (for IMH26)
		//first excitation region
		mdata.ExcPatterns[GMEDG_L].onBlock[0].cut1 = 0;
		mdata.ExcPatterns[GMEDG_L].onBlock[0].cut2 = 1;
		//second excitation region
		mdata.ExcPatterns[GMEDG_L].onBlock[1].cut1 = 30;
		mdata.ExcPatterns[GMEDG_L].onBlock[1].cut2 = 100;
*/
		/*
		//GMEDG_L - Type 1 (for Controls)
		//excitation region
		mdata.ExcPatterns[GMEDG_L].onBlock[0].cut1 = 42;
		mdata.ExcPatterns[GMEDG_L].onBlock[0].cut2 = 88;
		*/

		/*//TFLG_L - Type 1
		//excitation region
		mdata.ExcPatterns[TFLG_L].onBlock[0].cut1 = 30;
		mdata.ExcPatterns[TFLG_L].onBlock[0].cut2 = 85;*/
/*
		//TFLG_L - Type 2
		//first excitation region
		mdata.ExcPatterns[TFLG_L].onBlock[0].cut1 = 0;
		mdata.ExcPatterns[TFLG_L].onBlock[0].cut2 = 18;
		//second excitation region
		mdata.ExcPatterns[TFLG_L].onBlock[1].cut1 = 30;
		mdata.ExcPatterns[TFLG_L].onBlock[1].cut2 = 100;

		//GMAXG_L - Type 1
		//excitation region
		mdata.ExcPatterns[GMAXG_L].onBlock[0].cut1 = 19;
		mdata.ExcPatterns[GMAXG_L].onBlock[0].cut2 = 88;

		//MHG_L - Type 1
		//excitation region
		mdata.ExcPatterns[MHG_L].onBlock[0].cut1 = 32;
		mdata.ExcPatterns[MHG_L].onBlock[0].cut2 = 88;

		//BFLHG_L - Type 2 (for IMH26)
		//first excitation region
		mdata.ExcPatterns[BFLHG_L].onBlock[0].cut1 = 0;
		mdata.ExcPatterns[BFLHG_L].onBlock[0].cut2 = 18;
		//second excitation region
		mdata.ExcPatterns[BFLHG_L].onBlock[1].cut1 = 30;
		mdata.ExcPatterns[BFLHG_L].onBlock[1].cut2 = 100;
*/
		/*
		//BFLHG_L - Type 1 (for controls)
		//excitation region
		mdata.ExcPatterns[BFLHG_L].onBlock[0].cut1 = 25;
		mdata.ExcPatterns[BFLHG_L].onBlock[0].cut2 = 82;
		*/
/*
		//BFSH_L - Type 2
		//first excitation region
		mdata.ExcPatterns[BFSHG_L].onBlock[0].cut1 = 0;
		mdata.ExcPatterns[BFSHG_L].onBlock[0].cut2 = 25;
		//second excitation region
		mdata.ExcPatterns[BFSHG_L].onBlock[1].cut1 = 60;
		mdata.ExcPatterns[BFSHG_L].onBlock[1].cut2 = 100;

		//MGG_L - Type 2
		//first excitation region
		mdata.ExcPatterns[MGG_L].onBlock[0].cut1 = 0;
		mdata.ExcPatterns[MGG_L].onBlock[0].cut2 = 22;
		//second excitation region
		mdata.ExcPatterns[MGG_L].onBlock[1].cut1 = 44;
		mdata.ExcPatterns[MGG_L].onBlock[1].cut2 = 100;
		
		//LGG_L - Type 2
		//first excitation region
		mdata.ExcPatterns[LGG_L].onBlock[0].cut1 = 0;
		mdata.ExcPatterns[LGG_L].onBlock[0].cut2 = 22;
		//second excitation region
		mdata.ExcPatterns[LGG_L].onBlock[1].cut1 = 44;
		mdata.ExcPatterns[LGG_L].onBlock[1].cut2 = 100;
		
		//SOLG_L - Type 2
		//first excitation region
		mdata.ExcPatterns[SOLG_L].onBlock[0].cut1 = 0;
		mdata.ExcPatterns[SOLG_L].onBlock[0].cut2 = 26;
		//second excitation region
		mdata.ExcPatterns[SOLG_L].onBlock[1].cut1 = 38;
		mdata.ExcPatterns[SOLG_L].onBlock[1].cut2 = 100;

		//TAG_L - Type 1
		//excitation region
		mdata.ExcPatterns[TAG_L].onBlock[0].cut1 = 20;
		mdata.ExcPatterns[TAG_L].onBlock[0].cut2 = 58;

	}//end if SYMMETRIC == 0
*/
}//end get cut values

/****************** end get_cut_values *******************/


/*****************************************************************/
/******************** update_EMG_Exc_templates *******************/
/*****************************************************************/
/*** This function updates the excitation template for each    ***/
/*** muscle group based on the pattern source (they are cur-   ***/
/*** rently initialized to default values.  This function also ***/
/*** performs the task of reading in the experimental EMG file ***/
/*** if necessary.  As such, the user should verify that the   ***/
/*** format of the EMG file is the same as that assumed in this***/
/*** function.  This function replaces Ko Sasaki's read_cut_EMG***/
/*** function.  Created/Modified by Jeff Rankin - Oct 2008     ***/
/*****************************************************************/
void update_EMG_Exc_templates(void){

	int i,m;			//used in loops/indexing
	int exper_emg = 0;	//for determining if experimental EMG is used
	double dummy;		//used to discard unnecessary file data
	FILE *fp_exp_emg;	//used in reading in the experimental EMG data file
	double emg_max[N_musG], emg_min[N_musG]; //used in normalizing the experimental EMG data

	int npnt_on0,npnt_on1,npnt_on2;	//these are used to keep track of the number of points in each of the excitation regions (out of 101)
	int npnt_off0,npnt_off1;		//these are used to keep track of the number of points in each of the off regions (out of 101)

	double max_amp0, max_amp1, max_amp2; //used to renormalize the type3 muscle (see description below for why this is needed)	

	double exc_min, exc_max;	//these are used to remove the offset of just the excitation region of the EMG data and renormalize


	//Main Code

	//Check to see if any of the muscle groups are using EMG data.
	//If any are, then read in the experimental data EMG file.
	for (m=0;m<N_musG;m++){
		if (mdata.ExcPatterns[m].pattern_source == 1){//EMG is being used
			exper_emg = 1;
			break;
		}
	}//end for loop (m)
	
	if (exper_emg){//EMG data is required
		
		//read in the experimental EMG data file
		if((fp_exp_emg=fopen(emgfilename,"r"))==NULL){
				printf("emgfile cannot be opened!!!");
				exit(0);
			}			
		for(i=0;i<N_emg_step;i++){

			//read in the %gait cycle and throw it out
			fscanf(fp_exp_emg,"%lf",&dummy);

			//read in the experimental EMG data for each muscle group 		
			//right side
			fscanf(fp_exp_emg,"%lf", &mdata.emg_y[ILG][i]);			
			//fscanf(fp_exp_emg,"%lf", &mdata.emg_y[ALG_R][i]);
			//fscanf(fp_exp_emg,"%lf", &mdata.emg_y[AMG_R][i]);
			//fscanf(fp_exp_emg,"%lf", &mdata.emg_y[SARG_R][i]);
			fscanf(fp_exp_emg,"%lf", &mdata.emg_y[RFG][i]);
			fscanf(fp_exp_emg,"%lf", &mdata.emg_y[VMG][i]);
			fscanf(fp_exp_emg,"%lf", &mdata.emg_y[VLG][i]);
			fscanf(fp_exp_emg,"%lf", &mdata.emg_y[GMEDG][i]);
			//fscanf(fp_exp_emg,"%lf", &mdata.emg_y[TFLG_R][i]);
			fscanf(fp_exp_emg,"%lf", &mdata.emg_y[GMAXG][i]);
			fscanf(fp_exp_emg,"%lf", &mdata.emg_y[HAMG][i]);
			//fscanf(fp_exp_emg,"%lf", &mdata.emg_y[BFLHG_R][i]);
			fscanf(fp_exp_emg,"%lf", &mdata.emg_y[BFSHG][i]);
			fscanf(fp_exp_emg,"%lf", &mdata.emg_y[GASG][i]);
			//fscanf(fp_exp_emg,"%lf", &mdata.emg_y[LGG_R][i]);
			fscanf(fp_exp_emg,"%lf", &mdata.emg_y[SOLG][i]);
			fscanf(fp_exp_emg,"%lf", &mdata.emg_y[TAG][i]);
			fscanf(fp_exp_emg,"%lf", &mdata.emg_y[PERLNGG][i]);
			fscanf(fp_exp_emg,"%lf", &mdata.emg_y[FLXDIGG][i]);
			fscanf(fp_exp_emg,"%lf", &mdata.emg_y[EXTDIGG][i]);


			if (SYMMETRIC == 0){//if using non-symmetric patterns then read the left side data into at temporary excitation array, otherwise just discard it
				//left side
/*				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[ILG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[ALG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[AMG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[SARG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[RFG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[VMG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[VLG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[GMEDG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[TFLG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[GMAXG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[MHG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[BFLHG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[BFSHG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[MGG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[LGG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[SOLG_L][i]);
				fscanf(fp_exp_emg,"%lf", &mdata.emg_y[TAG_L][i]);
*/			}
			else{//this means we doing SYMMETRIC excitation patterns and the left side EMG doesnt matter.
				for (m=0;m<N_musG;m++){
					fscanf(fp_exp_emg,"%lf",&dummy);
				}
			}

		}//end for i loop (reading in exper EMG file)
		fclose(fp_exp_emg);
		
		
		//Now that the Experimental EMG has been obtained, normalize the data and eliminate the offset
		
		//obtain the maximum and minimum EMG values
		for(m=0;m<N_musG;m++){
			emg_max[m] = -100000.0;
			emg_min[m] = 100000.0;
			for(i=0;i<N_emg_step-1;i++){ //"N_emg_step-1" is to prevent the last point (101) to be max or min (if allowed, it causes a minor problem)
				if(emg_max[m] < mdata.emg_y[m][i])  //this means that the EMG value is the new max value
				{
					emg_max[m] = mdata.emg_y[m][i];
				}
				if(emg_min[m] > mdata.emg_y[m][i])  //this means that the EMG value is the new min value
				{
					emg_min[m] = mdata.emg_y[m][i];
				}
			}//end for loop (i)
		}//end for loop(m)

		//normalize the data and remove the offset
		for(m=0;m<N_musG;m++){
			for(i=0;i<N_emg_step;i++){
				mdata.emg_y[m][i] = (mdata.emg_y[m][i] - emg_min[m])/(emg_max[m]-emg_min[m]);
			}//end for loop (i)
		}//end for loop (m)

		//NOTE: Even though we have removed the offset at this point, it may be necessary to remove offsets within each excitaiton region by
		//obtaining the minimum value within the region and renormalizing this to be 0.  This should not be necessary if the EMG cut times are
		//picked so that they are equal to 0.  However, all of the muslce types have this normalization process commented implemented. 
		//It can be commented out as desired.
	}//end if (EMG)

	//Now that we have read in and normalized the EMG data, go through each of the muscle groups, determine if it is using EMG data
	//as the data source and update the excitation pattern accordingly
	for (m=0;m<N_musG;m++){	//go through the muscle groups
		
		//this switch goes between the different pattern types
		switch (mdata.ExcPatterns[m].pattern_type){

			case 1://type 1 pattern - 1 excitation region, 2 zero regions (off, on, off pattern)
				//get the number of points in the excitation and off regions
				npnt_on0 = mdata.ExcPatterns[m].onBlock[0].nPoint; //on region
				npnt_off0 = mdata.ExcPatterns[m].offBlock[0].nPoint; //off region 1
				npnt_off1 = mdata.ExcPatterns[m].offBlock[1].nPoint; //off region 2
				
				//check to see if EMG is used
				if (mdata.ExcPatterns[m].pattern_source == 1){					

					//go through the region of the pattern that is cut out for the excitation region and 
					//renormalize the region (removing any offset)
					exc_min = 1000;
					exc_max = -1000;
					//determine the minimum and maximum excitation values within the excitation region
					for (i=0;i<npnt_on0;i++){
						if (mdata.emg_y[m][i+npnt_off0] < exc_min){
							exc_min = mdata.emg_y[m][i+npnt_off0];
						}
						if (mdata.emg_y[m][i+npnt_off0] > exc_max){
							exc_max = mdata.emg_y[m][i+npnt_off0];
						}
					}					
					//now that we have the minimum value, renormalize the data (removing the offset)
					for (i=0;i<npnt_on0;i++){					
						mdata.emg_y[m][i+npnt_off0] = (mdata.emg_y[m][i+npnt_off0]-exc_min)*(1.0/(exc_max-exc_min));
					}
					//end renormalization

					//Besides the normalization, there should not be anything else that needs to be done as the 
					//temporary excitation arrays are saved as the file is being read in.

				}//end if (EMG is used)
				
				//else if (mdata.ExcPatterns[m].pattern_source == 2){//using a block pattern source 
				else if (mdata.ExcPatterns[m].pattern_source == 2 || mdata.ExcPatterns[m].pattern_source == 3|| mdata.ExcPatterns[m].pattern_source == 4){//using block or Gauss pattern source 
					//set all of the excitation values to 1, overwriting the exper EMG values (this makes it block pattern)
					for(i=0;i<N_emg_step;i++){
						mdata.emg_y[m][i] = 1.0;
					}//end loop (i)
				}//end  if (Block Pattern)
				
				else {//other patterns can be added here
					printf("Unrecognized Pattern Source for Muscle Group %i\n", m);
					exit(0);
				}//end else (other patterns)

				
				//update the excitation values in the template based on the pattern source

				//excitation region
				for(i=0;i<npnt_on0;i++){//cycle through all of the points in the region					
					
					//update the x-values
					mdata.ExcPatterns[m].onBlock[0].x[i] = (double)(i+npnt_off0);
					//update the y values				
					mdata.ExcPatterns[m].onBlock[0].y[i] = (mdata.emg_y[m][i+npnt_off0]);
				
					//make sure the excitation does not go below 0 (if it does there is an error in the code)
					if(mdata.ExcPatterns[m].onBlock[0].y[i]<0.0){
						printf("Negative Excitation Values in the Template\nSetting to 0\n");					
						mdata.ExcPatterns[m].onBlock[0].y[i]=0.0;				
					}
				}//end cycle through excitation region points (i loop)
				break;



			case 2: //Type 2 Pattern - 2 excitation regions, 1 zero region (on, off, on pattern)
				npnt_on0 = mdata.ExcPatterns[m].onBlock[0].nPoint;//number of points in the first excitation region
				npnt_on1 = mdata.ExcPatterns[m].onBlock[1].nPoint;//number of points in the second excitation region
				npnt_off0 = mdata.ExcPatterns[m].offBlock[0].nPoint;//number of points in the off region
				
				//check to see if EMG is used
				if (mdata.ExcPatterns[m].pattern_source == 1) {//using an EMG pattern
					
					//go through each region of the pattern that is cut out for excitation regions and 
					//renormalize these regions (removing any offset)
					
					//region 1
					exc_min = 1000;
					exc_max = -1000;
					//determine the minimum and maximum excitation values within the excitation region
					for (i=0;i<npnt_on0;i++){
						if (mdata.emg_y[m][i] < exc_min){
							exc_min = mdata.emg_y[m][i];
						}
						if (mdata.emg_y[m][i] > exc_max){
							exc_max = mdata.emg_y[m][i];
						}
					}					
					//now that we have the minimum value, renormalize the data (removing the offset)
					for (i=0;i<npnt_on0;i++){					
						mdata.emg_y[m][i] = (mdata.emg_y[m][i]-exc_min)*(1.0/(exc_max-exc_min));
					}

					//region 2
					exc_min = 1000;
					exc_max = -1000;
					//determine the minimum and maximum excitation values within the excitation region
					for (i=0;i<npnt_on1;i++){
						if (mdata.emg_y[m][i+npnt_on0+npnt_off0] < exc_min){
							exc_min = mdata.emg_y[m][i+npnt_on0+npnt_off0];
						}
						if (mdata.emg_y[m][i+npnt_on0+npnt_off0] > exc_max){
							exc_max = mdata.emg_y[m][i+npnt_on0+npnt_off0];
						}
					}					
					//now that we have the minimum value, renormalize the data (removing the offset)
					for (i=0;i<npnt_on1;i++){					
						mdata.emg_y[m][i+npnt_on0+npnt_off0] = (mdata.emg_y[m][i+npnt_on0+npnt_off0]-exc_min)*(1.0/(exc_max-exc_min));
					}
					//end renormalization
				}//end if (EMG is used
				
				//else if (mdata.ExcPatterns[m].pattern_source == 2){//using a block pattern					
				else if (mdata.ExcPatterns[m].pattern_source == 2 || mdata.ExcPatterns[m].pattern_source == 3|| mdata.ExcPatterns[m].pattern_source == 4){//using block or Gauss pattern source 
					//set all of the excitation values to 1, overwriting the experimental EMG values (this makes it block pattern)
					for(i=0;i<N_emg_step;i++){
						mdata.emg_y[m][i] = 1.0;
					}//end loop (i)
				}

				else {//other patterns can be added here
					printf("Unrecognized Pattern Source for Muscle Group %i\n", m);
					exit(0);
				}//end else (other patterns)


				//update the excitation values in the template based on the pattern source

				//excitation region 1
				for(i=0;i<npnt_on0;i++){//cycle through all of the points in the region
					
					//update the x-values
					mdata.ExcPatterns[m].onBlock[0].x[i] = (double)(i);
					//update the y values				
					mdata.ExcPatterns[m].onBlock[0].y[i] = (mdata.emg_y[m][i]);
				
					//make sure the excitation does not go below 0 (if it does there is an error in the code)
					if(mdata.ExcPatterns[m].onBlock[0].y[i]<0.0){
						printf("Negative Excitation Values in the Template\nSetting to 0\n");					
						mdata.ExcPatterns[m].onBlock[0].y[i]=0.0;				
					}
				}//end cycle through excitation region 1 points (i loop)

				//excitation region 2				
				for(i=0;i<npnt_on1;i++){//cycle through all of the points in the region
					
					//update the x-values
					mdata.ExcPatterns[m].onBlock[1].x[i] = (double)(i+npnt_on0+npnt_off0);
					//update the y values				
					mdata.ExcPatterns[m].onBlock[1].y[i] = (mdata.emg_y[m][i+npnt_on0+npnt_off0]);
				
					//make sure the excitation does not go below 0 (if it does there is an error in the code)
					if(mdata.ExcPatterns[m].onBlock[1].y[i]<0.0){
						printf("Negative Excitation Values in the Template\nSetting to 0\n");					
						mdata.ExcPatterns[m].onBlock[1].y[i]=0.0;				
					}
				}//end cycle through excitation region 2 points (i loop)
				break;



			case 3: //Type 3 Pattern - 3 excitation regions 2 zero regions (on, off, on, off, on pattern)
				npnt_on0 = mdata.ExcPatterns[m].onBlock[0].nPoint;//number of points in the first excitation region
				npnt_on1 = mdata.ExcPatterns[m].onBlock[1].nPoint;//number of points in the second excitation region
				npnt_on2 = mdata.ExcPatterns[m].onBlock[2].nPoint;//number of points in the third excitation region
				npnt_off0 = mdata.ExcPatterns[m].offBlock[0].nPoint;//number of points in the first off region
				npnt_off1 = mdata.ExcPatterns[m].offBlock[1].nPoint;//number of points in the second off region

				//check to see if using an EMG pattern
				if (mdata.ExcPatterns[m].pattern_source == 1) {

					//go through each region of the pattern that is cut out for excitaiton regions and 
					//renormalize these regions (removing any offset)

					//region 1
					exc_min = 1000;
					exc_max = -1000;
					//determine the minimum and maximum excitation values within the excitation region
					for (i=0;i<npnt_on0;i++){
						if (mdata.emg_y[m][i] < exc_min){
							exc_min = mdata.emg_y[m][i];
						}
						if (mdata.emg_y[m][i] > exc_max){
							exc_max = mdata.emg_y[m][i];
						}
					}					
					//now that we have the minimum value, renormalize the data (removing the offset)
					for (i=0;i<npnt_on0;i++){					
						mdata.emg_y[m][i] = (mdata.emg_y[m][i]-exc_min)*(1.0/(exc_max-exc_min));
					}

					//region 2
					exc_min = 1000;
					exc_max = -1000;
					//determine the minimum and maximum excitation values within the excitation region
					for (i=0;i<npnt_on1;i++){
						if (mdata.emg_y[m][i+npnt_on0+npnt_off0] < exc_min){
							exc_min = mdata.emg_y[m][i+npnt_on0+npnt_off0];
						}
						if (mdata.emg_y[m][i+npnt_on0+npnt_off0] > exc_max){
							exc_max = mdata.emg_y[m][i+npnt_on0+npnt_off0];
						}
					}					
					//now that we have the minimum value, renormalize the data (removing the offset)
					for (i=0;i<npnt_on1;i++){					
						mdata.emg_y[m][i+npnt_on0+npnt_off0] = (mdata.emg_y[m][i+npnt_on0+npnt_off0]-exc_min)*(1.0/(exc_max-exc_min));
					}

					//excitation region 3
					for(i=0;i<npnt_on2;i++){
						if (mdata.emg_y[m][i+npnt_on0+npnt_off0+npnt_on1+npnt_off1] < exc_min){
							exc_min = mdata.emg_y[m][i+npnt_on0+npnt_off0+npnt_on1+npnt_off1];
						}
						if (mdata.emg_y[m][i+npnt_on0+npnt_off0+npnt_on1+npnt_off1] > exc_max){
							exc_max = mdata.emg_y[m][i+npnt_on0+npnt_off0+npnt_on1+npnt_off1];
						}
					}					
					//now that we have the minimum value, renormalize the data (removing the offset)
					for (i=0;i<npnt_on2;i++){					
						mdata.emg_y[m][i+npnt_on0+npnt_off0+npnt_on1+npnt_off1] = (mdata.emg_y[m][i+npnt_on0+npnt_off0+npnt_on1+npnt_off1]-exc_min)*(1.0/(exc_max-exc_min));
					}

					//end renormalization
				}//end if EMG
				
				//else if (mdata.ExcPatterns[m].pattern_source == 2){//using a block pattern
				else if (mdata.ExcPatterns[m].pattern_source == 2 || mdata.ExcPatterns[m].pattern_source == 3 || mdata.ExcPatterns[m].pattern_source == 4){//using block or Gauss pattern source 					
					//set all of the excitation values to 1, overwriting the exper EMG values (this makes it block pattern)
					for(i=0;i<N_emg_step;i++){
						mdata.emg_y[m][i] = 1.0;
					}//end loop (i)					
				}

				else {//other patterns can be added here
					printf("Unrecognized Pattern Source for Muscle Group %i\n", m);
					exit(0);
				}//end else (other patterns)

				//update the excitation values in the template based on the pattern source

				//excitation region 1
				for(i=0;i<npnt_on0;i++){//cycle through all of the points in the region
					
					//update the x-values
					mdata.ExcPatterns[m].onBlock[0].x[i] = (double)(i);
					//update the y values				
					mdata.ExcPatterns[m].onBlock[0].y[i] = (mdata.emg_y[m][i]);
				
					//make sure the excitation does not go below 0 (if it does there is an error in the code)
					if(mdata.ExcPatterns[m].onBlock[0].y[i]<0.0){
						printf("Negative Excitation Values in the Template\nSetting to 0\n");					
						mdata.ExcPatterns[m].onBlock[0].y[i]=0.0;				
					}
				}//end cycle through excitation region 1 points (i loop)

				
				//obtain max value of the first excitation region (to re-normalize later, the first normalization in the beginning of this 
				//subroutine was normalizing to the max value of the entire emg curve, not individual lobes.
				max_amp0=-100.0;
				for(i=0;i<npnt_on0;i++){
					if(mdata.ExcPatterns[m].onBlock[0].y[i]>max_amp0){	//if the value is larger than the current max value, then set it as the new max value					
						max_amp0 = mdata.ExcPatterns[m].onBlock[0].y[i];
					}
				}//end loop through points (i loop)

				
				//excitation region 2
				for(i=0;i<npnt_on1;i++){//cycle through all of the points in the region

					//update the x-values
					mdata.ExcPatterns[m].onBlock[1].x[i] = (double)(i+npnt_on0+npnt_off0);					
					//update the y values				
					mdata.ExcPatterns[m].onBlock[1].y[i] = (mdata.emg_y[m][i+npnt_on0+npnt_off0]);					
				
					//make sure the excitation does not go below 0 (if it does there is an error in the code)
					if(mdata.ExcPatterns[m].onBlock[1].y[i]<0.0){
						printf("Negative Excitation Values in the Template\nSetting to 0\n");					
						mdata.ExcPatterns[m].onBlock[1].y[i]=0.0;				
					}
				}//end cycle through excitation region 2 points (i loop)

				//obtain max value of the second excitation region
				max_amp1=-100.0;
				for(i=0;i<npnt_on1;i++){
					if(mdata.ExcPatterns[m].onBlock[1].y[i]>max_amp1){
						max_amp1 = mdata.ExcPatterns[m].onBlock[1].y[i];
					}
				}//end loop through point (i loop)				
				
				
				//excitation region 3
				for(i=0;i<npnt_on2;i++){//cycle through all of the points in the region
					
					//update the x-values
					mdata.ExcPatterns[m].onBlock[2].x[i] = (double)(i+npnt_on0+npnt_off0+npnt_on1+npnt_off1);
					//update the y values				
					mdata.ExcPatterns[m].onBlock[2].y[i] = (mdata.emg_y[m][i+npnt_on0+npnt_off0+npnt_on1+npnt_off1]);
				
					//make sure the excitation does not go below 0 (if it does there is an error in the code)
					if(mdata.ExcPatterns[m].onBlock[2].y[i]<0.0){
						printf("Negative Excitation Values in the Template\nSetting to 0\n");					
						mdata.ExcPatterns[m].onBlock[2].y[i]=0.0;				
					}
				}//end cycle through excitation region 3 points (i loop)
				
				//obtain max value of the third excitation region
				max_amp2=-100.0;
				for(i=0;i<npnt_on2-1;i++){//"npnt_on2-1" is to avoid the last point (101) to be maximum (if allowed, it causes a minor problem in changing emg pattern) (from Ko)
					if(mdata.ExcPatterns[m].onBlock[2].y[i]>max_amp2){	//this means replace current value as the new max
						max_amp2 = mdata.ExcPatterns[m].onBlock[2].y[i];
					}
				}//end loop through point (i loop)
				
				
				//Now that we have set the excitation patterns for each of the excitaiton regions and obtained the max value,
				//we renormalize each region to its individual maximum value (making them independent parameters from each other).				
				for(i=0;i<npnt_on0;i++){//first excitation region
					mdata.ExcPatterns[m].onBlock[0].y[i] = mdata.ExcPatterns[m].onBlock[0].y[i]/(__max(max_amp0,max_amp2));					
				}//end for (i) loop

				for(i=0;i<npnt_on1;i++){//second excitation region
					mdata.ExcPatterns[m].onBlock[1].y[i] = mdata.ExcPatterns[m].onBlock[1].y[i]/max_amp1;
				}//end for (i) loop

				for(i=0;i<npnt_on2;i++){//third excitation region
					mdata.ExcPatterns[m].onBlock[2].y[i] = mdata.ExcPatterns[m].onBlock[2].y[i]/(__max(max_amp0,max_amp2));					
				}//end for (i) loop

				break;		
			
			
			default://for error checking
				printf("Unrecognized Excitation Pattern Type for Muscle Group %i\n", m);
				getchar();
				exit(0);
		}//end switch (pattern type)

		/*if(m==BFLHG_R)
		{
			for(i=0;i<mdata.ExcPatterns[m].NumExciteRegions; i++)
			{
				printf("Excite Region %d\n",i);
				for(int j=0;j<mdata.ExcPatterns[m].onBlock[i].nPoint;j++)
				{
					printf("x: %lf, y: %lf\n",mdata.ExcPatterns[m].onBlock[i].x[j],mdata.ExcPatterns[m].onBlock[i].y[j]);
				}
				getchar();
			}
		}*/
	}//end for loop (m) (goind through the muscle groups)

}//end update_EMG_Exc_templates

/********************* end update_EMG_Exc_templates *******************************/


/**********************************************************************************/
/************************** init_Exc_templates ************************************/
/**********************************************************************************/
/*** This function initializes the excitation templates for each of the muscle  ***/
/*** excitation groups.  This function basically uses user input flags to set up***/
/*** each group based on the excitation pattern type and the excitation pattern ***/
/*** source (e.g. experimental EMG or Block sources).  This function was based  ***/
/*** on Ko Sasaki's 'set_patterns' function.  Created by Jeff Rankin - Oct 2008 ***/
/**********************************************************************************/
void init_Exc_templates(void) {
	
	int i,j,k; //used for indexing
	int check_points, neg_points;//used to verify that the number of points were created properly

	
	
	
	//Main Code
	printf("Initializing Muscle Excitation Group Templates\n");

	//check to make sure that the flag for symmetry is appropriately set
	if (SYMMETRIC == 0 || SYMMETRIC == 1){		
	}
	else{
		printf("Check your flag for symmetry\n");		
		exit(0);
	}

	//get the user defined flags for the muscle group pattern sources and muscle group pattern types	
	set_pattern_sources();
	printf("Pattern Sources Set\n");
	set_pattern_types();
	printf("Pattern Types Set\n");


	//now allocate the appropriate amount of space in the arrays/structure based on the pattern type
	for (i=0;i<N_musG;i++){//cycle through each muscle group
		//This switch is for the different pattern types		
		switch (mdata.ExcPatterns[i].pattern_type)
		{
			case 1://1 excitation region, 2 zero regions (off, on, off pattern)
				mdata.ExcPatterns[i].NumExciteRegions = 1;
				mdata.ExcPatterns[i].NumZeroRegions = 2;
				mdata.ExcPatterns[i].onBlock = new ExcRegion[mdata.ExcPatterns[i].NumExciteRegions];
				mdata.ExcPatterns[i].offBlock = new ZeroRegion[mdata.ExcPatterns[i].NumZeroRegions];
				break;
			case 2://2 excitation regions 1 zero region (on, off, on pattern)
				mdata.ExcPatterns[i].NumExciteRegions = 2;
				mdata.ExcPatterns[i].NumZeroRegions = 1;
				mdata.ExcPatterns[i].onBlock = new ExcRegion[mdata.ExcPatterns[i].NumExciteRegions];
				mdata.ExcPatterns[i].offBlock = new ZeroRegion[mdata.ExcPatterns[i].NumZeroRegions];
				break;
			case 3://3 excitation regions 2 zero regions (on, off, on, off, on pattern)
				mdata.ExcPatterns[i].NumExciteRegions = 3;
				mdata.ExcPatterns[i].NumZeroRegions = 2;
				mdata.ExcPatterns[i].onBlock = new ExcRegion[mdata.ExcPatterns[i].NumExciteRegions];
				mdata.ExcPatterns[i].offBlock = new ZeroRegion[mdata.ExcPatterns[i].NumZeroRegions];
				break;
			default:				
				printf("Missing Pattern Type.  Check your pattern declarations\n");
				printf("and make sure the appropriate pattern has been created.\n");
				printf("Muscle Group %i\n",i);
				exit(0);
		}//end switch
	}//end cycle through groups to allocate space

	//Now that space has been allocated, fill in the data structures with the appropriate data

	//First implement the times (for cutting the EMG data), these should be modified by the user as necessary
	//based on the specific experimental EMG data file and/or changes in pattern type
	get_cut_values();	


	//Now that the cut values have been input, determine the number of points in each of the on and off excitation regions and allocate space for the excitation templates
	//NOTE: This function assumes that the total number of points desired is 101, that is the data is normalized to 100% with points at every 1% interval
	for (i=0;i<N_musG;i++){
		//This switch is for the different pattern types
		switch (mdata.ExcPatterns[i].pattern_type)
		{
			case 1: //Excitation Pattern 1
				
				//on region
				mdata.ExcPatterns[i].onBlock[0].nPoint = mdata.ExcPatterns[i].onBlock[0].cut2 - 
															  mdata.ExcPatterns[i].onBlock[0].cut1 + 1;
				//off region 1
				mdata.ExcPatterns[i].offBlock[0].nPoint = mdata.ExcPatterns[i].onBlock[0].cut1;
				//off region 2
				mdata.ExcPatterns[i].offBlock[1].nPoint = 100 - mdata.ExcPatterns[i].onBlock[0].cut2;
				break;

			case 2: //Excitation Pattern 2
				
				//on region 1
				mdata.ExcPatterns[i].onBlock[0].nPoint = mdata.ExcPatterns[i].onBlock[0].cut2 + 1;
				//on region 2
				mdata.ExcPatterns[i].onBlock[1].nPoint = 100 - mdata.ExcPatterns[i].onBlock[1].cut1 + 1;
				//off region
				mdata.ExcPatterns[i].offBlock[0].nPoint = mdata.ExcPatterns[i].onBlock[1].cut1 -
															   mdata.ExcPatterns[i].onBlock[0].cut2 - 1;
				break;

			case 3: //Excitation Pattern 3

				//on region 1
				mdata.ExcPatterns[i].onBlock[0].nPoint = mdata.ExcPatterns[i].onBlock[0].cut2 + 1;
				//on region 2
				mdata.ExcPatterns[i].onBlock[1].nPoint = mdata.ExcPatterns[i].onBlock[1].cut2 -
															  mdata.ExcPatterns[i].onBlock[1].cut1 + 1;
				//on region 3
				mdata.ExcPatterns[i].onBlock[2].nPoint = 100 - mdata.ExcPatterns[i].onBlock[2].cut1 + 1;
				//off region 1
				mdata.ExcPatterns[i].offBlock[0].nPoint = mdata.ExcPatterns[i].onBlock[1].cut1 -
															   mdata.ExcPatterns[i].onBlock[0].cut2 - 1;
				//off region 2
				mdata.ExcPatterns[i].offBlock[1].nPoint = mdata.ExcPatterns[i].onBlock[2].cut1 -
															   mdata.ExcPatterns[i].onBlock[1].cut2 - 1;
				break;

			default://for error checking

				printf("Error allocating Excitation Pattern Template Memory\n");
				exit(0);
				break;
		}//end switch

		//Check to make sure the the point values for each of the pattern templates are all reasonable (e.g. not negative) and add up to
		//the total number of points that there should be (N_emg_steps).  If not, flag the muscle group for the user and quit.
		check_points = 0;
		neg_points = 0;
		for (j=0;j<mdata.ExcPatterns[i].NumExciteRegions;j++){
			check_points = check_points + mdata.ExcPatterns[i].onBlock[j].nPoint;
			if (mdata.ExcPatterns[i].onBlock[j].nPoint < 0){			
				neg_points = 1;
			}
		}
		for (j=0;j<mdata.ExcPatterns[i].NumZeroRegions;j++){
			check_points = check_points + mdata.ExcPatterns[i].offBlock[j].nPoint;
			if (mdata.ExcPatterns[i].offBlock[j].nPoint < 0){			
				neg_points = 1;
			}
		}
		if (check_points != N_emg_step || neg_points == 1){
			printf("Error Creating the Points in the Template for Muscle Group %i\tTotal Points: %i\t NegPoints: %i\n", i,check_points,neg_points);
			exit(0);
		}
		//for data verification
		/*for (k=0;k<mdata.ExcPatterns[i].NumExciteRegions;k++){
			printf("Region %i, Total Points = %i\n", k, mdata.ExcPatterns[i].onBlock[k].nPoint);
			for (j=0;j<mdata.ExcPatterns[i].onBlock[k].nPoint;j++){
				printf("Row %i Values X %f Y %f\n", j, mdata.ExcPatterns[i].onBlock[k].x[j],mdata.ExcPatterns[i].onBlock[k].y[j]);
			}
			getchar();
		}*/
		//End Data Checking


		//allocate space for the excitation arrays based on the number of points and initialize the arrays to 1 (maximal excitation).					
		for(k=0;k<mdata.ExcPatterns[i].NumExciteRegions;k++){//memory allocation for excitation regions
			mdata.ExcPatterns[i].onBlock[k].x = new double[mdata.ExcPatterns[i].onBlock[k].nPoint];
			mdata.ExcPatterns[i].onBlock[k].y = new double[mdata.ExcPatterns[i].onBlock[k].nPoint];
			for(j=0;j<mdata.ExcPatterns[i].onBlock[k].nPoint;j++){//initialize values
				mdata.ExcPatterns[i].onBlock[k].y[j] = 1.0;//initialize with 1.0 (dummy);
				mdata.ExcPatterns[i].onBlock[k].x[j] = (double)(mdata.ExcPatterns[i].onBlock[k].cut1 + j);	//percent of complete cycle value
			} //end initialize values (j loop)
		}//end initialize patterns (k loop)

		//Note: the off regions of the excitation pattern are handled when the templates are
		//updated with the optimal parameters.		
	} //end loop through the muscle groups (i loop)

	printf("Templates Have Been Initialized\n");

	//update the muscle excitation group templates based on the data source fpr the pattern
	update_EMG_Exc_templates();

	printf("Templates Have Been Updated based on Muscle Pattern Source\n");

	//set the number of parameter values that are to be optimized based on the pattern type and source and allocate the necessary space
	for(i=0;i<N_musG;i++){
		if((mdata.ExcPatterns[i].pattern_type == 1) || (mdata.ExcPatterns[i].pattern_type == 2)){
			if (mdata.ExcPatterns[i].pattern_source == 3){
				mdata.ExcPatterns[i].num_excite_params = 4; //on, off, magnitude, shape
			}
			else if (mdata.ExcPatterns[i].pattern_source == 4){
				mdata.ExcPatterns[i].num_excite_params = 6; //on, off, magnitude, on, off, magnitude,
			}

			else{			
				mdata.ExcPatterns[i].num_excite_params = 3; //on, off, and magnitude
			}
		}
		else if(mdata.ExcPatterns[i].pattern_type == 3){
			if (mdata.ExcPatterns[i].pattern_source == 3){			
				mdata.ExcPatterns[i].num_excite_params = 8; //on, off, magnitude, shape for first region & on, off, mag, shape for second region
			}
			else if (mdata.ExcPatterns[i].pattern_source == 4){
				mdata.ExcPatterns[i].num_excite_params = 6; //on, off, magnitude, on, off, magnitude,
			}
			else{
				mdata.ExcPatterns[i].num_excite_params = 6; //on, off, magnitude for first region & on, off, mag for second region
			}			
		}

		else{
			printf("Incompatible Muscle Pattern Type or Source.  Please Fix Values\n");
			printf("Muscle Group Number %i\n",i);
			exit(0);
		}
		mdata.ExcPatterns[i].excite_params = new double[mdata.ExcPatterns[i].num_excite_params];
//		printf("Group %i -> Parameters %i\n",i,mdata.ExcPatterns[i].num_excite_params);
	}//end loop through the muscles
	printf("The required number of excitation parameters per group have been stored\n");
	
}//end init_Exc_templates

/**************************** end init_Exc_templates *************************************/





