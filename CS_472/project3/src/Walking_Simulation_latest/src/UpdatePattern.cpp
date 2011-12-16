//This file replaces Ko Sasaki's ModPattern.cpp.
//Created by Jeff Rankin - Oct 2008
#include <iostream>
#include "SA.h"
#include "SAstructs.h"
#include "system_params.h" //required for change_init
#include "ExcGroups.h"

extern "C" OptStruct mdata;

//Functions within this file


//this changes the initial velocities of the generalized coordinates (called directly by simulated annealing)
void change_init(double *x, int N_params); 
//this function updates the muscle group excitation patterns based on the optimization parameters
void update_group_excitation_patterns(double **XR,double **YR,double **XL,double **YL, double *x);
//computes left side excitation patterns assuming symmetry 
void comp_left(double **bezXR,double **bezYR,double **bezXL,double **bezYL,int m); 
//alters the pattern so that the excitation regions are at the right time in the cycle
void patternshift(double pX[N_emg_step], double pY[N_emg_step],double **XR,double **YR,double shift, int m);
//modifies the excitation of a single group based on the optimization parameters
void update_single_group_pattern(double **muscle_exc_x, double **muscle_exc_y, int m, int Leg, int NumGrps_Leg);

//Functions not within this file but called in this file
//Assigns the muscle group excitations to the appropriate individual muscles
void assign_muscle_excitations(double **group_exc_xR,double **group_exc_yR,double **group_exc_xL,double **group_exc_yL,
							   double **XR,double **YR,double **XL,double **YL);

extern "C" int create_gauss_template(double shape_factor1, double shape_factor2, int exc_pattern);


/******************************************************************************/
/******************** update_group_excitation_patterns ************************/
/******************************************************************************/
/*** this function updates the muscle group templates based on the          ***/
/*** optimization parameters obtained from the simulated annealing program. ***/
/*** It receives the optimal parameters and returns the left and right leg  ***/
/*** individual muscle excitation patterns to the forward simulation.       ***/
/***			Created by Jeff Rankin - Oct 2008							***/
/******************************************************************************/
void update_group_excitation_patterns(double **XR,double **YR,double **XL,double **YL, double *xp){

	int m; //used exclusively to cycle through the muscle groups
	int i;	//indexing
	int opt_param_num = 0;	//keeps track of the optimal parameter number to be assigned
	int total_excite_params = 0;	//used to verify the number of optimization parameters is the same as that required by the muscle group templates
	int flag_left=-2; // to be used later if nessacerry
	int foo;

	double temp_exc_x[N_emg_step], temp_exc_y[N_emg_step]; //these arrays temporarily hold the complete excitation curve for a single muscle group
	double **group_exc_xR, **group_exc_yR; //these arrays store the final muscle group excitation patterns which are used to assign excitation values to individual muscles (right leg)
	double **group_exc_xL, **group_exc_yL; //these arrays store the final muscle group excitation patterns which are used to assign excitation values to individual muscles (left leg)														   

	int NumGrps_Leg;//Keeps track of the number of muscle groups per leg

	FILE *data_file1; //saves the final excitation pattern (all muscles) to file for data checking

//	printf("Here\n");
	//this sets the number of muscle groups per leg (used for cycling between left and right legs)
	if (SYMMETRIC == 1){
		NumGrps_Leg = N_musG;
	}
	else{
		if (SYMMETRIC == 0){
			NumGrps_Leg = N_musG/2;
		}
	}	
	
	//allocate memory for the group excitation arrays
	group_exc_xR = new double*[NumGrps_Leg];
	group_exc_yR = new double*[NumGrps_Leg];
	group_exc_xL = new double*[NumGrps_Leg];
	group_exc_yL = new double*[NumGrps_Leg];
	for (m=0;m<NumGrps_Leg;m++){
		group_exc_xR[m] = new double[N_emg_step];
		group_exc_yR[m] = new double[N_emg_step];
		group_exc_xL[m] = new double[N_emg_step];
		group_exc_yL[m] = new double[N_emg_step];
	}
	//initialize memory values to 1.0
	for (m=0;m<NumGrps_Leg;m++){
		for(i=0;i<N_emg_step;i++){
			group_exc_xR[m][i] = (double) i;
			group_exc_yR[m][i] = 1.0;
			group_exc_xL[m][i] = (double) i;
			group_exc_yL[m][i] = 1.0;
		}//end i loop

	}//end initialization of arrays (m loop)



	//check to make sure the number of excitation parameters within the SA code is the same as the number of parameters calculated when initializing the arrays
	//NOTE: This check will need to be modified in the future to account for individual muscle magnitude changes
	for (m=0;m<N_musG;m++){
		total_excite_params = total_excite_params + mdata.ExcPatterns[m].num_excite_params;
	}//end loop through groups
	if (total_excite_params != N_EXC_PARAMS){
		printf("Excitation Parameters Mismatch.  Verify your parameters.\n  Template Params = %i, Opt Exc Params = %i\n", total_excite_params, N_EXC_PARAMS);
		exit(0);
	}

	
	//go through the muscle groups and assign the appropriate excitation parameter values based on the current optimization values
	for (m=0;m<N_musG;m++){
		for(i=0;i<mdata.ExcPatterns[m].num_excite_params;i++){
			mdata.ExcPatterns[m].excite_params[i] = xp[i + opt_param_num];
		}
		opt_param_num = opt_param_num + mdata.ExcPatterns[m].num_excite_params;//used to keep track of what excitation parameter (from SA code) is to be assigned
	}


//	printf("Parameters Assigned to Muscle Excitation Groups\n");
/*	//for data checking - makes sure that the parameters are changing based in SA process
	if( (data_file1 = fopen("OptParamsChange_Jeff.xls","a")) == NULL){
		printf("Exc Pattern Check File Can Not Be Opened\n");
		exit(0);
	}
	
	for (m=0;m<N_musG;m++){		
		for(i=0;i<mdata.ExcPatterns[m].num_excite_params;i++){
			fprintf(data_file1,"%lf\t",mdata.ExcPatterns[m].excite_params[i]);
		}		
	}
	fprintf(data_file1,"\n");
	fclose(data_file1);*/
//	printf("Modifying pattern according to the optimal parameters\n");


	//now that we have the optimization parameters, update the muscle group excitation patterns accordingly
	for (m=0;m<NumGrps_Leg;m++){//cycle through the muscle groups

		//clear the temporary excitation values
		for (i=0;i<N_emg_step;i++){
			temp_exc_x[i] = 0.0;
			temp_exc_y[i] = 0.0;
		}
		if (mdata.ExcPatterns[m].pattern_source != 4 ){
				
		update_single_group_pattern(group_exc_xR,group_exc_yR,m,1,NumGrps_Leg);//note the 1 in the function parameters indicates right leg
		}
		else{
			update_BiModal_Pattern(group_exc_xR,group_exc_yR,m,flag_left);
		
		//	void update_BiModal_Pattern(double **group_XR, double**group_YR,int m,int flag_left);
		}

	}//end cycle through the right leg groups

	
	
	
	if (SYMMETRIC == 1) {//this means the left leg excitations are just 180 degrees out of phase of the right legs (not receiving individual excitation parameters)
		for(m=0;m<NumGrps_Leg;m++){//cycle through the muscle groups					
			//create the left leg excitation array		
			comp_left(group_exc_xR,group_exc_yR,group_exc_xL,group_exc_yL,m);
		}//end m loop
	}//end if SYMMETRIC == 1


	
	if (SYMMETRIC == 0){	//this is a non-symmetric leg
		//This function is basically a copy of the function that we are now using and just calculates the left leg			
		//this means that it is necessary to loop through the left leg in the same manner we calculated the right leg
		
		for (m=NumGrps_Leg;m<2*NumGrps_Leg;m++){			
			update_single_group_pattern(group_exc_xL,group_exc_yL,m,0,NumGrps_Leg);			
		}//end loop through left leg muscles (m loop)		
	}//end if SYMMETRIC == 0
 


	//assign the group excitations to the individual muscles
	assign_muscle_excitations(group_exc_xR,group_exc_yR,group_exc_xL,group_exc_yL,XR,YR,XL,YL);

//	printf("Done assigning individual muscle excitaiton data\n");

	//Save the Excitation Patterns to file for data checking
	if( (data_file1 = fopen("ExcPattern.dat","w")) == NULL){
		printf("Exc Pattern Check File Can Not Be Opened\n");
		exit(0);
	}

	for(i=0;i<N_emg_step;i++){
		for(m=0;m<N_mus;m++){
			//if (m == VASMEDG){
			fprintf(data_file1, "%lf\t%lf\t%lf\t%lf\t", XR[m][i],YR[m][i],XL[m][i],YL[m][i]);
			//}
		}
		fprintf(data_file1,"\n");
	}
	fclose(data_file1);
	//end save data check file


	//free memory
	for (m=0;m<NumGrps_Leg;m++){		
			free(group_exc_xR[m]);
			free(group_exc_yR[m]);
			free(group_exc_xL[m]);
			free(group_exc_yL[m]);
	}//end m loop
	free(group_exc_xR);
	free(group_exc_yR);
	free(group_exc_xL);
	free(group_exc_yL);


}//end update_group_excitation_patterns
/******************** end update_group_excitation_patterns **************************/




/*************************************************************************************/
/*********************** update_single_group_pattern *********************************/
/*************************************************************************************/
/*** This function is basically a big switch (based on pattern type) that performs ***/
/*** the calculations necessary to modify the template of a single muscle group    ***/
/*** excitation pattern based on the current optimization parameters. The function ***/
/*** then places the pattern in the appropriate location in the muscle group excit-***/
/*** ation array.					Created by Jeff Rankin - Oct 2008			   ***/
/*************************************************************************************/
//Function Variables
//group_exc_x -> excitation array for all of the muscle groups for a single leg (x-values)
//group_exc_y -> excitation array for all of the muscle groups for a single leg (y-values)
//m -> the index of the muscle group pattern to be updated
//Leg -> if this is a left leg or right leg muscle (required for indexing reasons)
//NGrps_Leg -> the total number of muscle groups per leg (required for indexing)
void update_single_group_pattern(double **group_exc_x, double **group_exc_y, int m, int Leg, int NumGrps_Leg){

	int i;	//for indexing

	double onregion1_x[N_emg_step], onregion1_y[N_emg_step];	//these are used to create the excitation pattern in the excitaiton regions
	double onregion2_x[N_emg_step], onregion2_y[N_emg_step];	//these are used to create the excitation pattern in the excitaiton regions
	double temp_exc_x[N_emg_step], temp_exc_y[N_emg_step]; //these arrays temporarily hold the complete excitation curve for a single muscle group
	double deltx_new;	//this is used to change the excitation and off region abscissas

	int index0, index1, index2;	//used to keep track of indices while building up arrays

	int excite_index; //used for indexing in saving the complete muscle group excitation arrays

	if (Leg == 1){//getting excitations for right leg muscle groups
		excite_index = m;
	}
	if (Leg == 0){//getting excitations for left leg muscle groups
		excite_index = m-NumGrps_Leg;
	}

	//determine the muscle group type and update the values accordingly
	switch (mdata.ExcPatterns[m].pattern_type)
	{
			case 1: //Type 1 pattern

			//first clear the values (set to 0) of the temporary excitation array
			for (i=0;i<mdata.ExcPatterns[m].onBlock[0].nPoint;i++){
				onregion1_x[i] = 0.0;
				onregion1_y[i] = 0.0;
			}//end for (clear) loop

			//if the pattern is Gaussian then we need to update the template values (based on the shape parameter)	
			if (mdata.ExcPatterns[m].pattern_source == 3){
				create_gauss_template(mdata.ExcPatterns[m].excite_params[3], 2.0, m);				
			}

			//obtain the template values and store them temporarily for modification
			for (i=0;i<mdata.ExcPatterns[m].onBlock[0].nPoint;i++){ //cycle through all of the points in the excitation region
				onregion1_x[i] = mdata.ExcPatterns[m].onBlock[0].x[i] - 
								 (double) mdata.ExcPatterns[m].onBlock[0].cut1; //this changes the indexing to go from 0->nPnts instead of starting at 'cut time' 
				onregion1_y[i] = mdata.ExcPatterns[m].onBlock[0].y[i];
			}//end for loop (i)

			//now modify the template so that it goes from the onset and offset times given by the optimization parameters (basically create a new spline of values with 
			//a different abscissa
			deltx_new = (mdata.ExcPatterns[m].excite_params[1] - mdata.ExcPatterns[m].excite_params[0]) //the new time abscissa
						 /((double)(mdata.ExcPatterns[m].onBlock[0].cut2 - mdata.ExcPatterns[m].onBlock[0].cut1)); //the template abscissa value

			//change the abscissa and amplification for the excitation region
			for (i=0;i<mdata.ExcPatterns[m].onBlock[0].nPoint;i++){//cycle through all of the points in the excitaiton region
				onregion1_x[i] = onregion1_x[i] * deltx_new; //updates the abscissa
				onregion1_y[i] = onregion1_y[i] * mdata.ExcPatterns[m].excite_params[2];//updates the amplitude
			}

			//now that we have modified the on region, create the off regions (by modifying the abscissa and having the excitation values equal to 0)
			//and add them to the excitation region to make a complete excitation pattern (101 data points)
			for (i=0;i<mdata.ExcPatterns[m].onBlock[0].nPoint;i++){	//the first points are the excitation points
				temp_exc_x[i] = onregion1_x[i];
				temp_exc_y[i] = onregion1_y[i];
			}//end i loop				
			//calculate the new abscissa value for the off regions
			deltx_new = (100-temp_exc_x[mdata.ExcPatterns[m].onBlock[0].nPoint-1]) //this is the new abscissa range
						/(double)(mdata.ExcPatterns[m].offBlock[0].nPoint+mdata.ExcPatterns[m].offBlock[1].nPoint); //this is the old abscissa range (equal to the total number of template points)
			//append the off regions to the excitation region to complete the pattern
			for (i=0;i<(mdata.ExcPatterns[m].offBlock[0].nPoint+mdata.ExcPatterns[m].offBlock[1].nPoint);i++){//all of the of regions are temporarily combined and added to the end in this case
				temp_exc_x[i+mdata.ExcPatterns[m].onBlock[0].nPoint] = (temp_exc_x[mdata.ExcPatterns[m].onBlock[0].nPoint-1]) +  //increment the off abscissa values from the last value in the excitation region
																			(double)(i + 1)*deltx_new;
				temp_exc_y[i+mdata.ExcPatterns[m].onBlock[0].nPoint] = 0.0; //the muscle is off in these regions
			}//end creating the temporary excitation curves (i loop)

			//the last thing that we need to do is shift the pattern so that the excitation region starts at the right place (right now it starts at time = 0)
			//we want it to start at the optimization onset value.  We also store the result in a new array that has all of the final muscle group patterns
			patternshift(temp_exc_x,temp_exc_y,group_exc_x,group_exc_y,mdata.ExcPatterns[m].excite_params[0],excite_index);
			break;		


		case 2:	//type 2 pattern
	
			//create indexes for setting up the temporary excitation arrays
			index0 = mdata.ExcPatterns[m].onBlock[0].nPoint;
			index1 = mdata.ExcPatterns[m].onBlock[1].nPoint;

			//clear the temporary excitation arrays//
			for (i=0;i<(index0+(index1-1));i++){//clear enough excitation points for both excitation regions
				onregion1_x[i] = 0.0;
				onregion1_y[i] = 0.0;
			}//end for (clear) loop

			//if the pattern is Gaussian then we need to update the template values (based on the shape parameter)	
			if (mdata.ExcPatterns[m].pattern_source == 3){
				create_gauss_template(mdata.ExcPatterns[m].excite_params[3], 2.0, m);				
			}

	
			//now modify the template so that it goes from the onset and offset times given by the optimization parameters (basically create a new spline of values with 
			//a different abscissa
			
			//change the indexing of the two excitation blocks (from the template) so that they are at the beginning of the temporary excitation array
			//obtain the template values and store them temporarily for modification
			for (i=0;i<(index1-1);i++){ //cycle through all of the points in the second excitation region (goes first)
				onregion1_x[i] = mdata.ExcPatterns[m].onBlock[1].x[i] - 
								 (double) mdata.ExcPatterns[m].onBlock[1].cut1; //this changes the indexing to go from 0->nPnts instead of starting at 'cut time' 
				onregion1_y[i] = mdata.ExcPatterns[m].onBlock[1].y[i];
			}//end for loop (i)
			for (i=0;i<index0;i++){ //cycle through all of the points in the first excitation region and append them to the second excitaiton region
				onregion1_x[i+(index1-1)] = (100-(double)mdata.ExcPatterns[m].onBlock[1].cut1) + (double)i;									 					
				onregion1_y[i+(index1-1)] = mdata.ExcPatterns[m].onBlock[0].y[i];
			}//end for loop (i)
			
			//calculate the abscissa conversion
			deltx_new = ( (100-mdata.ExcPatterns[m].excite_params[0]) + mdata.ExcPatterns[m].excite_params[1]) //the new time abscissa (both excitation blocks
						 /( (100-(double)(mdata.ExcPatterns[m].onBlock[1].cut1) + mdata.ExcPatterns[m].onBlock[0].cut2)); //the template abscissa value (both excitation blocks)				

		
			//change the abscissa and amplification for the excitation regions
			for (i=0;i<(index0+index1-1);i++){//cycle through all of the points in the excitation regions					
				onregion1_x[i] = onregion1_x[i] * deltx_new; //updates the abscissa
				onregion1_y[i] = onregion1_y[i] * mdata.ExcPatterns[m].excite_params[2];//updates the amplitude
			}//end modification of excitation regions
			

			//now that we have modified the excitation regions, create the off regions (by modifying the abscissa and having the excitation values equal to 0)
			//and add them to the excitation region to make a complete excitation pattern (101 data points)
			for (i=0;i<(index0+index1-1);i++){	//the first points are the excitation points
				temp_exc_x[i] = onregion1_x[i];
				temp_exc_y[i] = onregion1_y[i];
			}//end i loop				
			//calculate the new abscissa value for the off regions
			deltx_new = (100-onregion1_x[index0+index1-2]) //this is the new abscissa range (the -2 comes from the -1 in the second excitation region and then a second -1 due to indexing)
						/(double)(mdata.ExcPatterns[m].offBlock[0].nPoint+1); //this is the old abscissa range (equal to the total number of template points)
			//append the off regions to the excitation region to complete the pattern
			for (i=0;i<(mdata.ExcPatterns[m].offBlock[0].nPoint+1);i++){//all of the of regions are temporarily combined and added to the end in this case
				temp_exc_x[i+index0+index1-1] = (onregion1_x[index0+index1-2]) + (double)(i + 1)*deltx_new; //increment the off abscissa values from the last value in the excitation region																				
				temp_exc_y[i+index0+index1-1] = 0.0; //the muscle is off in these regions
			}//end creating the temporary excitation curves (i loop)

			//the last thing that we need to do is shift the pattern so that the excitation region starts at the right place (right now it starts at time = 0)
			//we want it to start at the optimization onset value.  We also store the result in a new array that has all of the final muscle group patterns								
			patternshift(temp_exc_x,temp_exc_y,group_exc_x,group_exc_y,mdata.ExcPatterns[m].excite_params[0],excite_index);
			break;				

		case 3:	//type 3 pattern

			//set the indices
			index0 = mdata.ExcPatterns[m].onBlock[0].nPoint;
			index1 = mdata.ExcPatterns[m].onBlock[1].nPoint;
			index2 = mdata.ExcPatterns[m].onBlock[2].nPoint;

			//clear the temporary excitation arrays//
			for (i=0;i<(index0+index2-1);i++){//clear enough excitation points for the first excitation region (regions 1 and 3 in template)
				onregion1_x[i] = 0.0;
				onregion1_y[i] = 0.0;
			}//end for (clear) loop
			for (i=0;i<index1;i++){//clear enough excitation points for the middle excitation region (region 2 in template)
				onregion2_x[i] = 0.0;
				onregion2_y[i] = 0.0;
			}//end for (clear) loop

			//if the pattern is Gaussian then we need to update the template values (based on the shape parameter)	
			if (mdata.ExcPatterns[m].pattern_source == 3){
				create_gauss_template(mdata.ExcPatterns[m].excite_params[5], mdata.ExcPatterns[m].excite_params[7], m);				
			}

			//now modify the template so that it goes from the onset and offset times given by the optimization parameters (basically create a new spline of values with 
			//a different abscissa

			//first modify the middle excitation region (region 2 in template)
			//change the indexing of the second excitation block (from the template) so that it goes from 0 and
			//obtain the template values for modification
			for (i=0;i<index1;i++){ //cycle through all of the points in the second excitation region
				onregion2_x[i] = mdata.ExcPatterns[m].onBlock[1].x[i] - 
								 (double) mdata.ExcPatterns[m].onBlock[1].cut1; //this changes the indexing to go from 0->nPnts instead of starting at 'cut time' 					
				onregion2_y[i] = mdata.ExcPatterns[m].onBlock[1].y[i];
			}//end for loop (i)

			//calculate the abscissa conversion for the middle excitation region
			deltx_new = (mdata.ExcPatterns[m].excite_params[2] - mdata.ExcPatterns[m].excite_params[1]) //the new time abscissa (second excitation block)
						 /((double)(mdata.ExcPatterns[m].onBlock[1].cut2 - mdata.ExcPatterns[m].onBlock[1].cut1)); //the template abscissa value (second excitation block)				

			//change the abscissa and amplification for the middle excitation regions
			for (i=0;i<index1;i++){//cycle through all of the points in the middle excitation region
				onregion2_x[i] = onregion2_x[i] * deltx_new; //updates the abscissa
				if (mdata.ExcPatterns[m].pattern_source == 3){
					onregion2_y[i] = onregion2_y[i] * mdata.ExcPatterns[m].excite_params[6];//updates the amplitude of the middle excitation region
				}
				else{
					onregion2_y[i] = onregion2_y[i] * mdata.ExcPatterns[m].excite_params[5];//updates the amplitude of the middle excitation region
				}

			}//end modification of middle excitation region

			//now modify the excitation region spanning then ends (regions 1 and 3 in template)

			//change the indexing of the 1st and 3rd excitation blocks (from the template) so that it corresponds to the parameter shift
			//obtain the template values for modification
			for (i=0;i<index2-1;i++){ //cycle through all of the points in the 3rd template region
				onregion1_x[i] = mdata.ExcPatterns[m].onBlock[2].x[i] - 
								 (double) mdata.ExcPatterns[m].onBlock[1].cut1; //this changes the indexing 
				onregion1_y[i] = mdata.ExcPatterns[m].onBlock[2].y[i];
			}//end for loop (i)
			//add the template first excitation region to the third region (changing the index appropriately)
			for (i=0;i<index0;i++){ //cycle through all of the points in the 1st template region
				onregion1_x[i+index2-1] = (double)i + 100 - (double) mdata.ExcPatterns[m].onBlock[1].cut1; //this changes the indexing 
				onregion1_y[i+index2-1] = mdata.ExcPatterns[m].onBlock[0].y[i];
			}//end for loop (i)

			//now that we have all of the points in a single array, update the excitation pattern according to the parameter values
			//calculate the abscissa conversion for the first/third excitation region
			deltx_new = (100 - mdata.ExcPatterns[m].excite_params[3] + mdata.ExcPatterns[m].excite_params[0]) //the new time abscissa (first/third excitation block)
						 /(100 - (double)mdata.ExcPatterns[m].onBlock[2].cut1 + (double)mdata.ExcPatterns[m].onBlock[0].cut2); //the template abscissa value (first/third excitation block)				

			//change the abscissa and amplification for the first/third excitation region
			for (i=0;i<(index0+index2-1);i++){//cycle through all of the points in the first/third excitation region
				onregion1_x[i] = (onregion1_x[i] - (mdata.ExcPatterns[m].onBlock[2].cut1 - mdata.ExcPatterns[m].onBlock[1].cut1)) * deltx_new + (mdata.ExcPatterns[m].excite_params[3]-mdata.ExcPatterns[m].excite_params[1]); //updates the abscissa
				onregion1_y[i] = onregion1_y[i] * mdata.ExcPatterns[m].excite_params[4];//updates the amplitude of the first/third excitation region
			}//end modification of middle excitation region

			//now that we have modified both of the excitation regions, create a complete excitation pattern (adding in the off regions)


			//add the middle excitaiton region points to the temporary excitation pattern array (middle region is first)
			for (i=0;i<index1;i++){	
				temp_exc_x[i] = onregion2_x[i];
				temp_exc_y[i] = onregion2_y[i];
			}//end i loop


			//now append the first off region to the temporary array
			//first, compute the abscissa conversion
			deltx_new = ( onregion1_x[0] - onregion2_x[index1-1] )	//this is the new abscissa range
						/(double)(mdata.ExcPatterns[m].offBlock[1].nPoint+1);
			//now append the off region to the temporary excitation pattern
			for (i=0;i<(mdata.ExcPatterns[m].offBlock[1].nPoint);i++){	
				temp_exc_x[i+index1] = onregion2_x[index1-1] + (double)(i+1)*deltx_new;
				temp_exc_y[i+index1] = 0.0;
			}//end i loop
			//now append the excitation values from the 1/3rd excitaiton regions to the temporary excitation array
			for (i=0;i<(index0+index2-1);i++){//cycle through the excitation points
				temp_exc_x[i+index1+mdata.ExcPatterns[m].offBlock[1].nPoint] = onregion1_x[i];
				temp_exc_y[i+index1+mdata.ExcPatterns[m].offBlock[1].nPoint] = onregion1_y[i];
			}//end appending the first/third excitation region

			//lastly, append the final off region to complete the temporary excitation curve
			//calculate the abscissa conversion
			deltx_new = (100 - onregion1_x[index0+(index2-1)-1])	//new time abscissa
						/(double)(mdata.ExcPatterns[m].offBlock[0].nPoint+1);
			//append the final data to the temporary excitation pattern
			for (i=0;i<mdata.ExcPatterns[m].offBlock[0].nPoint+1;i++){				
				temp_exc_x[i + index1 + mdata.ExcPatterns[m].offBlock[1].nPoint + index0 + (index2 - 1)] =
									onregion1_x[index0+(index2-1)-1] + (double)(i+1)*deltx_new;									;
				temp_exc_y[i + index1 + mdata.ExcPatterns[m].offBlock[1].nPoint + index0 + (index2 - 1)] = 0.0;
			}

			//We now have a complete temporary excitation pattern.  The final thing that we need to do is shift the pattern according to the optimal parameters
			patternshift(temp_exc_x,temp_exc_y,group_exc_x,group_exc_y,mdata.ExcPatterns[m].excite_params[1],excite_index);
			break;

		default:
			printf("Could not Identify the Excitation Pattern Type for Modification\n");
			exit(0);
			break;
	}//end switch


}//end update_single_group_pattern

/******************** end update_single_group_pattern *******************/




/************************************************************************/
/**************************** patternshift ******************************/
/************************************************************************/
/*** This function shifts a muscle group excitation pattern so that the**/
/*** excitation region timing is that indicated by the optimization	  ***/
/*** parameters.  This is code was originally writtne by Ko Sasaki and***/
/*** was modified slightly by Jeff Rankin - Oct 2008	              ***/
/************************************************************************/
void patternshift(double pX[N_emg_step], double pY[N_emg_step],double **XR,double **YR,double shift, int m)
{
	int i;			//indexing
	int overpoints; //this keeps track of how many points are above a complete cycle in the temporarily shifted array
	int pass_point;	//keeps track of the index of the point in the shifted pattern that goes above a complete cycle (for use in shifting points
					//to the beginning of the new array).
	int flag;		//keeps track of where points in the original pattern should be placed at the beginning of the shifted pattern
	double shiftX[N_emg_step], shiftY[N_emg_step]; //temporarily hold the shifted pattern values
	double overX[N_emg_step], overY[N_emg_step];	//these are used to help shift the new pattern indexes	
	double difference1, difference2;	//these keep track of the difference in abscissa values of the last point below a complete cycle and 
										//the first point above a complete cycle and the complete cycle value (100).  Used in properly creating 
										//the shifted array.

	
	//check to see if there is no shift.  If so just copy that values over into the muscle group array and exit
	if(shift == 0.0){
		//now that the modified pattern for the group (based on the optimization values) has been created, store it so that it can be assigned to specific muscles
  		for (i=0;i<N_emg_step;i++){ //cycle through all of the data points
			XR[m][i] = pX[i];
			YR[m][i] = pY[i];
		}//end saving modified muscle group pattern (i loop)
		return;
	}//end if shift == 0

	//the rest of this code executes if the shift did not equal 0
	//set counters
	overpoints = 0;
	//set default difference values
	difference1 = 0.0;
	difference2 = 0.0;


	//if the shift is greater than 0 then rearrange the points so the excitation region is at the right time
	if(shift>0.0){

		//make sure that this shift does not put the point pass a complete cycle (100).  If it does redo the abscissa accordingly
		if((pX[0] + shift)>=100.0){
			for(i=0;i<N_emg_step;i++){
				pX[i] = pX[i] - 100.0;
			}
		}//end if

		//now that we know the shift will not result in having the initial excitaiton region begin at a point greater than 100
		//go through and shift the pattern so that the excitation and off regions are at the appropriate locations in the array		
		flag = 0;
		for(i=0;i<N_emg_step;i++){//cyle through all of the data points
			shiftX[i] = pX[i] + shift;//create the new pattern x values based on the original pattern and the shift value
			shiftY[i] = pY[i];//the y values are not altered

			//find out if the new x value for the given point is above 100 if it is, flag the point and store the subsequent
			//array values in temporary arrays (overX, overY)
			if(shiftX[i] > 100.0){
				if(flag==0){//if it is the first point keep track of the point and the differences between the two points around the complete cycle mark (100)
					pass_point = i;
					difference1 = shiftX[pass_point] - 100.0;
					difference2 = 100.0 - shiftX[pass_point-1];					
					overX[0] = shiftX[pass_point]-100;//future start point (x-value)
					overY[0] = shiftY[pass_point];//future start point (y_value)
					overpoints = 1;
					flag=1;
				}
				else{
					overX[i-pass_point] = shiftX[i] - 100.0;//need to subtract 100 to shift it to the beginning of the cycle
					overY[i-pass_point] = shiftY[i];
					overpoints = overpoints + 1;
				}
			}//end detect if the abscissa of the point is > 100
		}//end for loop (cycle through data points)

		//Now that we know how many shifted data points had abscissa values greater than 100
		//and placed thme in a seperate array we can generate the new complete array with these values
		//at the beginning (thus making the excitation region start at the right value)
		
		//if there were points above 100, save the points that were over 100 as the new start points
		for(i=0;i<overpoints;i++){			
			XR[m][i] = overX[i] - difference1;
			YR[m][i] = overY[i];
		}
		//append the rest of the points to the excitation pattern
		for(i=overpoints;i<N_emg_step;i++){
			XR[m][i] = shiftX[i-overpoints] + difference2;
			YR[m][i] = shiftY[i-overpoints];
		}
	}

//This works in the same way the if shift>0.0 (NOTE: This section has not been commented completely Jeff R)
	else{//shift<0.0

		if((pX[0] + shift) <= -100.0){
			for(i=0;i<N_emg_step;i++){
				pX[i] = pX[i] + 100.0;
			}
		}
		flag = 0;
		for(i=0;i<N_emg_step;i++){
			shiftX[i] = pX[i] + shift;
			shiftY[i] = pY[i];

			if(flag==0){

				if(shiftX[i] >= 0.0){
					pass_point = i;
					difference1 = shiftX[pass_point] - 0.0;
					difference2 = 0.0 - shiftX[pass_point-1]; //negative value					
					flag=1;
				}
				else{ //shiftX[i]<0.0
					overX[i] = shiftX[i];
					overY[i] = shiftY[i];
				}
			}
		}

		for(i=0;i<N_emg_step-pass_point;i++){
			XR[m][i] = shiftX[i+pass_point] - difference1;
			YR[m][i] = shiftY[i+pass_point];
		}
		for(i=0;i<pass_point;i++){
			XR[m][i+N_emg_step-pass_point] = overX[i] + difference2 + 100.0;
			YR[m][i+N_emg_step-pass_point] = overY[i];
		}
	}//end of else

	//make sure the the first and last point are the same excitation value
	XR[m][0] = 0.0;
	XR[m][N_emg_step-1] = 100.0;
	YR[m][N_emg_step-1] = YR[m][0];

}//pattern shift

/*************** end pattern_shift*************/



/**********************************************/
/**************  comp_left ********************/
/**********************************************/
/*** This function takes the excitation pat-***/
/*** terns of the right side and creates    ***/
/*** left side patterns assuming symmetry   ***/
/***			Created by Ko Sasaki	 	***/
/**********************************************/
//Note: This code is not commented at all.
void comp_left(double **bezXR,double **bezYR,
			   double **bezXL,double **bezYL,
			   int m)
{
	double **bezXLdummy;
	int i, j, memo;
	double index, diff1,diff2;


	bezXLdummy=(double**)calloc(N_mus,sizeof(double*));
	if(bezXLdummy == NULL) exit(0);
	for(i=0;i<N_mus;i++){
		bezXLdummy[i]=(double*)calloc(N_emg_step,sizeof(double));
		if(bezXLdummy[i] == NULL) exit(0);
	}

	memo=0;
	index=0;

	for(i=0;i<N_emg_step;i++){
		bezXLdummy[m][i] = bezXR[m][i] + 50.0;

		if(bezXLdummy[m][i] >= 100.0){
			if(index==0){
				diff1=bezXLdummy[m][i] - 100.0;
				diff2=100.0 - bezXLdummy[m][i-1];
				memo=i-1;
				index = 1;
			}
		
		}
	}

	j=0;
	i=N_emg_step-1;
	while(j<=memo){
		bezXL[m][i] = bezXLdummy[m][memo-j]+diff2;
		bezYL[m][i] = bezYR[m][memo-j];
		j++;
		i--;
	}

	j=0;
	i=0;
	while(j<N_emg_step-1 - memo){
		bezXL[m][i] = bezXR[m][memo+1+i]-50.0 -diff1; 
		bezYL[m][i] = bezYR[m][memo+1+i];

		j++;
		i++;
	}


	bezXL[m][0]=0.0;     //to make sure (I think these are not necessary, though)
	bezXL[m][100]=100.0;


	for(i=0;i<N_mus;i++){
  	   free(bezXLdummy[i]);
    }
	free(bezXLdummy);
}
/****************** end comp_left ***********************/




/********************************************************/
/******************* change_init ************************/
/********************************************************/
/*** This function changes the initial velocities     ***/
/*** based on optimized parameter values.             ***/
/***					Created by Ko Sasaki          ***/
/********************************************************/
///////////////////////////////////////////////////////////////
//Change initial velocities (to be optimized parameters)
void change_init(double *x, int N_params)
{
	//last nine of x are the initial velocities

	mdata.legTx_v = x[N_params-13];
    mdata.legTy_v = x[N_params-12];
    mdata.pelvis_w = x[N_params-11];
	mdata.hipR_w = x[N_params-10];
    mdata.kneeR_w = x[N_params-9];
    mdata.ankleR_w = x[N_params-8];
	mdata.midR_w = x[N_params-7];
	mdata.mtpR_w = x[N_params-6];
    mdata.hipL_w = x[N_params-5];
    mdata.kneeL_w = x[N_params-4];
    mdata.ankleL_w = x[N_params-3];
	mdata.midL_w = x[N_params-2];
	mdata.mtpL_w = x[N_params-1];
    
}
///////////////////////////////////////////////////////////////////////
/******************* end chang_init **********************/


