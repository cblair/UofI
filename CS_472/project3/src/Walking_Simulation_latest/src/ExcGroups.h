/******************************************************/
/****************** ExcGroups.h ***********************/
/******************************************************/
/*** This header file contains the user defined val-***/
/*** ues used in implementing excitation patterns   ***/
/*** They should be modified by the user as necessary**/
/***            Created by Jeff Rankin - Oct 2008   ***/
/******************************************************/
#ifndef EXCGROUPS_H
#define EXCGROUPS_H

#define N_emg_step 101	//DO NOT ALTER!  MUST BE 101 FOR THE EXCITATION CODE TO WORK!

#define SYMMETRIC 1		//1: Symmetric left and right leg excitations, 0:Asymmetric leg excitations
#define N_mus 25		//Total number of individual muscles for one leg (should match the number of muscles in the muscle file)

//These define the name of the data file containing the optimization parameters
//#define OPTPARAMFILE "SAstartXblock_Sym.dat"
#define OPTPARAMFILE "SAstartXbimodal.dat"  //update this to match the correct file

//This defines the name of the data file containing the experimental EMG data
#define emgfilename "W60emgave.txt"//"N/A";//"W100emgave.txt";

//Both N_musG and N_EXC_PARAMS should be updated by the user according
#if (SYMMETRIC == 1)
	#define N_musG 14	//the total number of muscle groups
	#define N_EXC_PARAMS 84//all bimodal //66-EMG used	//the total number of optimization parameters associated with the excitation data	
#else
	#define N_musG 30	//left and right muscle groups are uncoupled (should be 2X the value in SYMMETRIC == 1)
	#define N_EXC_PARAMS 132	//the total number of optimization parameters associated with the excitation data	
#endif

//The rest of this file defines the order of the muscle groups and individual muscles.  The muscle definition must match the
//order of the muscles in the muscle file.  Currently the the muscle groups and individual muscle definitions are based on
//muscles_17Groups.msl (Nov2008).

//right leg muscle groups
#define SOLG 0
#define GASG 1
#define HAMG 2
#define BFSHG 3
#define ILG 4
#define TAG 5
#define RFG 6
#define VMG 7
#define VLG 8
//#define vas3G 9// not used
#define GMAXG 9
#define GMEDG 10
#define PERLNGG 11
#define FLXDIGG 12
#define EXTDIGG 13

//left leg muscle groups

//none are defined, always doing symmetric at this point

//right leg muscles
#define sol_r 0
#define mgas_r 1
#define sm_r 2
#define bfsh_r 3
#define psoas_r 4
#define ta_r 5
#define rf_r 6
#define vas1_r 7
#define gmax_r 8
#define gmeda_r 9
#define perlng_r 10
#define tp_r 11
#define flxdig_r 12
#define flxhal_r 13
#define lgas_r 14
#define bflh_r 15
#define iliac_r 16
#define pertert_r 17
#define extdig_r 18
#define exthal_r 19
#define vas2_r 20
#define vas3_r 21
#define addmag_r 22
#define gmedp_r 23
#define perbrev_r 24

//left leg muscles
#define sol_l 25
#define mgas_l 26
#define sm_l 27
#define bfsh_l 28
#define psoas_l 29
#define ta_l 30
#define rf_l 31
#define vas1_l 32
#define gmax_l 33
#define gmeda_l 34
#define perlng_l 35
#define tp_l 36
#define flxdig_l 37
#define flxhal_l 38
#define lgas_l 39
#define bflh_l 40
#define iliac_l 41
#define pertert_l 42
#define extdig_l 43
#define exthal_l 44
#define vas2_l 45
#define vas3_l 46
#define addmag_l 47
#define gmedp_l 48
#define perbrev_l 49


#endif
