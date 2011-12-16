/********************* GaussPattern.c ********************************************/
/*********************************************************************************/
/** The functions within this file can be used to implement Gaussian excitation **/
/** patterns within the existing optimization framework created for 3D walking. **/
/** The functions and code have been adapted from the original code developed   **/
/** for optimization of wheelchair propulsion technique.                        **/
/*********************************************************************************/
/********** Created by Jeff Rankin: Mar2009 **************************************/
/*********************************************************************************/

#include "universal.h"
#include "SAstructs.h" //required to reference mdata

//external global variables (declared in other files)
extern SDModelStruct sdm; //declared in simann.c (main model structure)
extern OptStruct mdata; //declared in main.c (main optimization data structure)

//function prototypes for this file
int create_gauss_template(double shape_factor1, double shape_factor2, int exc_pattern);
double interpolate_spline_gauss(double abscissa, double* x_array, double* y_array, int points);

//Global variables for functions in this file
double MIN_EXC_VALUE = 0.01;


/************************* create_gauss_template **********************************/
/**********************************************************************************/
/** This function creates a gaussian excitation pattern based on the input       **/
/** parameters provided.  It returns an error code that can be used if desired.  **/
/**********************************************************************************/

//Parameter descriptions
//shape_factor1 -> The parameter defining the shape of the excitation burst related to the first excitation region
//
//shape_factor2 -> The parameter defining the shape of the excitation burst related to the second excitation region (middle burst).
//				   Only used with a type 3 pattern.
//exc_pattern -> This is the index indicating which excitation group's pattern is being modified
//exc_type -> this indicates the group's excitation pattern type (e.g. "type 1")

//A Note of caution: If nPoint_temp <=1, then this program will crash.  This value is calculated directly from the pattern cut times.  
//					 Make sure that space between any two consecutive cut times in >1 (e.g. not 99,100, but 98,100). This will prevent the crash.

int create_gauss_template(double shape_factor1, double shape_factor2, int exc_pattern){

	int error_code = 0;			//for returning an error
	int j;						//indexing		

	double gauss_x[101] = {0};	//for creating the original temporary Gaussian pattern
	double gauss_y[101] = {0};
	int gauss_numpoints = 101;

	double *gauss_exc_y;		//for storing just the excitation portion of the temporary Gaussian pattern
	double *gauss_exc_x;
	int gauss_exc_numpoints = 0;
	int start_gauss_exc;		//for picking of the excitation region of the temporary gauss pattern

	int nPoint_temp = 0;		//used in updating the abscissa of the templates
	double new_abscissa;
	double ratio;

	double excite_threshold = MIN_EXC_VALUE;	//for determining a minimum excitation threshold for the pattern to be "on"

	//create the gaussian template shape based on the shape input and assume maximal excitation
	for (j=0;j<101;j++){
		gauss_x[j] = (double)j;
		gauss_y[j] = 1.0*(exp(-.5*pow(fabs(((double)j-50)/15),shape_factor1)));
	}
	
	//now go through and cut the template so that it removes all of the 0 portions (that way we only have the excitation values
	//this is necessary to do because of the shape parameter, which actually will shrink and expand the number of points that are
	//above some excitation threshhold (e.g. .05).
	
	//find the initial excitation time
	for (j=0;j<gauss_numpoints;j++){
		if (gauss_y[j] > excite_threshold){	
			start_gauss_exc = j;			
			break;
		}		
		if (j == gauss_numpoints-1){
			printf("Error, no excitation start value (Gauss) was found\n");
			printf("Pattern %i Type %i\n", exc_pattern,mdata.ExcPatterns[exc_pattern].pattern_type);
			getchar();
			exit(0);
		}
	}//end for loop
	
	//now that we have detected the onset time, we can use symmetry to determine the offset time and pull out the points we want	

	//if start_gauss_exc = 0, then we need to subtract the first and last point so that there is space in the "0 blocks".
	if (start_gauss_exc == 0){
		start_gauss_exc = 1;
	}

	gauss_exc_numpoints = 101-(2*start_gauss_exc);

	//allocate memory for the temporary excitation arrays
	gauss_exc_x = calloc(gauss_exc_numpoints,sizeof(double));
	gauss_exc_y = calloc(gauss_exc_numpoints,sizeof(double));

	
	//assign the new values to the excitation template curve
	for (j=0;j<gauss_exc_numpoints;j++){
		gauss_exc_x[j] = (double)j;		
			gauss_exc_y[j] = gauss_y[j+start_gauss_exc]-gauss_y[start_gauss_exc-1];			//this makes the initial point ~= 0		
	}	
	//this new curve starts at 0 and goes to (gauss_exc_numpoints-1)

	//save the Gaussian pattern template based on excitation type
	switch (mdata.ExcPatterns[exc_pattern].pattern_type){

		/////////////////////////Type 1 muscle/////////////////////////
		case 1:				

			//interpolate the excitation template so that it has the correct number of points

			//first determine the new abscissa increment
			nPoint_temp = mdata.ExcPatterns[exc_pattern].onBlock[0].nPoint;
			new_abscissa = (double)(gauss_exc_numpoints-1)/(double)(nPoint_temp-1);

			//assign the excitation points to the muscle excitation region		
			for (j=0;j<nPoint_temp;j++){
								
				//create the x-points for the excitation curve
				mdata.ExcPatterns[exc_pattern].onBlock[0].x[j] = (double)j*new_abscissa;
				
				//get the corresponding y points by interpolating the gauss_exc curves
				mdata.ExcPatterns[exc_pattern].onBlock[0].y[j] = interpolate_spline_gauss(mdata.ExcPatterns[exc_pattern].onBlock[0].x[j], gauss_exc_x, gauss_exc_y, gauss_exc_numpoints);
				
				//now that we have the appropriate y-values from the gauss pattern, we need to readjust the abscissa
				//so that it has the correct start and end times and the correct range value
				mdata.ExcPatterns[exc_pattern].onBlock[0].x[j] = (mdata.ExcPatterns[exc_pattern].onBlock[0].x[j]/new_abscissa)+(double)(mdata.ExcPatterns[exc_pattern].onBlock[0].cut1);												
			}
			break;

		////////////////////////Type 2 muscle//////////////////////////
		case 2:							
	
			//get the ratio between the timing in the two curves so that the gaussian pattern is properly assigned
			if (mdata.ExcPatterns[exc_pattern].onBlock[1].cut1 == 100 && mdata.ExcPatterns[exc_pattern].onBlock[0].cut2 == 0){
				ratio = 0.5;
			}
			else{
				ratio = mdata.ExcPatterns[exc_pattern].onBlock[0].cut2/((100-mdata.ExcPatterns[exc_pattern].onBlock[1].cut1) + mdata.ExcPatterns[exc_pattern].onBlock[0].cut2);
			}
			
			//create the new_abscissa increment based on the total number of points
			nPoint_temp = mdata.ExcPatterns[exc_pattern].onBlock[0].nPoint + mdata.ExcPatterns[exc_pattern].onBlock[1].nPoint;			
			new_abscissa = (double)(gauss_exc_numpoints-1)/(double)(nPoint_temp-1);			
			
			//assign the gaussian excitation pattern points to each of the excitation regions based on the number of points in each region

			/////////////////////////////////////
			//Region 1 -> second excitation burst
			/////////////////////////////////////
			nPoint_temp = mdata.ExcPatterns[exc_pattern].onBlock[1].nPoint;
			for (j=0;j<nPoint_temp;j++){
				
				//create the x-points for the excitation curve
				mdata.ExcPatterns[exc_pattern].onBlock[1].x[j] = (double)j*new_abscissa;

				//get the corresponding y points by interpolating the gauss_exc curves
				mdata.ExcPatterns[exc_pattern].onBlock[1].y[j] = interpolate_spline_gauss(mdata.ExcPatterns[exc_pattern].onBlock[1].x[j], gauss_exc_x, gauss_exc_y, gauss_exc_numpoints);
				
				//now that we have the appropriate y-values from the gauss pattern, we need to readjust the abscissa
				//so that it has the correct start and end times
				mdata.ExcPatterns[exc_pattern].onBlock[1].x[j] = (mdata.ExcPatterns[exc_pattern].onBlock[1].x[j]/new_abscissa) + (double)mdata.ExcPatterns[exc_pattern].onBlock[1].cut1;								
			}

			/////////////////////////////////////
			//Region 0 -> first excitation region
			/////////////////////////////////////
			nPoint_temp = mdata.ExcPatterns[exc_pattern].onBlock[0].nPoint;
			for (j=0;j<nPoint_temp;j++){

				//create the x-points for the excitation curve
				mdata.ExcPatterns[exc_pattern].onBlock[0].x[j] = (double)(j+mdata.ExcPatterns[exc_pattern].onBlock[1].nPoint)*new_abscissa;
				
				//get the corresponding y points by interpolation
				mdata.ExcPatterns[exc_pattern].onBlock[0].y[j] = interpolate_spline_gauss(mdata.ExcPatterns[exc_pattern].onBlock[0].x[j], gauss_exc_x, gauss_exc_y, gauss_exc_numpoints);

				//now that we have the appropriate y-values from the gauss pattern, we need to readjust the abscissa
				//so that it has the correct start and end times
				mdata.ExcPatterns[exc_pattern].onBlock[0].x[j] = (mdata.ExcPatterns[exc_pattern].onBlock[0].x[j])/new_abscissa - (double)mdata.ExcPatterns[exc_pattern].onBlock[1].nPoint;				
			}
			break;

		///////////////////////////Type 3 muscle///////////////////////////////
		case 3:

			//first set up excitation regions 0 and 2 (the regions on the end)
			if (mdata.ExcPatterns[exc_pattern].onBlock[2].cut1 == 100 && mdata.ExcPatterns[exc_pattern].onBlock[0].cut2 == 0){
				ratio = 0.5;
			}
			else{
				ratio = mdata.ExcPatterns[exc_pattern].onBlock[0].cut2/((100-mdata.ExcPatterns[exc_pattern].onBlock[2].cut1) + mdata.ExcPatterns[exc_pattern].onBlock[0].cut2);
			}

			//create the new_abscissa increment based on the total number of points
			nPoint_temp = mdata.ExcPatterns[exc_pattern].onBlock[0].nPoint + mdata.ExcPatterns[exc_pattern].onBlock[2].nPoint;
			new_abscissa = (double)(gauss_exc_numpoints-1)/(double)(nPoint_temp-1);

			//assign the gaussian excitation pattern points to excitation regions 0 and 2 based on the number of points in each region

			///////////////////////////////////
			//Region 2 -> last excitation burst
			///////////////////////////////////
			nPoint_temp = mdata.ExcPatterns[exc_pattern].onBlock[2].nPoint;
			for (j=0;j<nPoint_temp;j++){

				//create the x-points for the excitation curve
				mdata.ExcPatterns[exc_pattern].onBlock[2].x[j] = (double)j*new_abscissa;

				//get the corresponding y points by interpolation			
				mdata.ExcPatterns[exc_pattern].onBlock[2].y[j] = interpolate_spline_gauss(mdata.ExcPatterns[exc_pattern].onBlock[2].x[j], gauss_exc_x, gauss_exc_y, gauss_exc_numpoints);

				//now that we have the appropriate y-values from the gauss pattern, we need to readjust the abscissa
				//so that it has the correct start and end times
				mdata.ExcPatterns[exc_pattern].onBlock[2].x[j] = mdata.ExcPatterns[exc_pattern].onBlock[2].x[j]/new_abscissa + (double)mdata.ExcPatterns[exc_pattern].onBlock[2].cut1;											
			}

			////////////////////////////////////
			//Region 0 -> first excitation burst
			////////////////////////////////////
			nPoint_temp = mdata.ExcPatterns[exc_pattern].onBlock[0].nPoint;
			for (j=0;j<nPoint_temp;j++){
				
				//create the x-points for the excitation curve
				mdata.ExcPatterns[exc_pattern].onBlock[0].x[j] = (double)(j+mdata.ExcPatterns[exc_pattern].onBlock[2].nPoint)*new_abscissa;

				//get the corresponding y points by interpolation
				mdata.ExcPatterns[exc_pattern].onBlock[0].y[j] = interpolate_spline_gauss(mdata.ExcPatterns[exc_pattern].onBlock[0].x[j], gauss_exc_x, gauss_exc_y, gauss_exc_numpoints);

				//now that we have the appropriate y-values from the gauss pattern, we need to readjust the abscissa
				//so that it has the correct start and end times
				mdata.ExcPatterns[exc_pattern].onBlock[0].x[j] = mdata.ExcPatterns[exc_pattern].onBlock[0].x[j]/new_abscissa - (double)mdata.ExcPatterns[exc_pattern].onBlock[2].nPoint;				
			}

			/////////////////////////////////////
			//Region 1 -> middle excitation burst
			/////////////////////////////////////
			//in order to do this burst the gauss template must be redone with the second set of excitation magnitude and shape parameters			

			for (j=0;j<101;j++){
				gauss_x[j] = (double)j;
				gauss_y[j] = 1.0*(exp(-.5*pow(fabs(((double)j-50)/15),shape_factor2)));
			}			
			//likewise the "excitation only" template must be redone			
			//find the initial excitation time
			for (j=0;j<gauss_numpoints;j++){
				if (gauss_y[j] > excite_threshold){//then the curve starts exciting here so get the point before as the start point
					start_gauss_exc = j;
					break;
				}
				if (j == gauss_numpoints-1){
					printf("Error, no excitation start value (Gauss) was found\n");
					getchar();
					exit(0);
				}
			}//end for loop	
			
			//if start_gauss_exc = 0, then we need to subtract the first and last point so that there is space in the "0 blocks".
			if (start_gauss_exc == 0){
				start_gauss_exc = 1;
			}

			//now that we have detected the onset time, we can use symmetry to determine the offset time and pull out the points we want
			//reallocate the excitation only gaussian template to have the right amount of space
			gauss_exc_numpoints = 101-(2*start_gauss_exc);
			gauss_exc_y = realloc(gauss_exc_y,gauss_exc_numpoints*sizeof(double));
			gauss_exc_x = realloc(gauss_exc_x,gauss_exc_numpoints*sizeof(double));

			//assign the new values to the excitation curve
			for (j=0;j<gauss_exc_numpoints;j++){
				gauss_exc_y[j] = gauss_y[j+start_gauss_exc]-gauss_y[start_gauss_exc-1];			
			}
			//first determine the new abscissa increment
			nPoint_temp = mdata.ExcPatterns[exc_pattern].onBlock[1].nPoint;
			new_abscissa = (double)(gauss_exc_numpoints-1)/(double)(nPoint_temp-1);

			//assign the excitation points to the muscle excitation region		
			for (j=0;j<nPoint_temp;j++){
				
				//create the x-points for pulling off of the excitation curve
				mdata.ExcPatterns[exc_pattern].onBlock[1].x[j] = (double)j*new_abscissa;
				
				//get the corresponding y points by interpolation
				mdata.ExcPatterns[exc_pattern].onBlock[1].y[j] = interpolate_spline_gauss(mdata.ExcPatterns[exc_pattern].onBlock[1].x[j], gauss_exc_x, gauss_exc_y, gauss_exc_numpoints);
				
				//now that we have the appropriate y-values from the gauss pattern, we need to readjust the abscissa
				//so that it has the correct start and end times
				mdata.ExcPatterns[exc_pattern].onBlock[1].x[j] = mdata.ExcPatterns[exc_pattern].onBlock[1].x[j]/new_abscissa+(double)(mdata.ExcPatterns[exc_pattern].onBlock[1].cut1);												
			}
			break;
		default:
			printf("Error creating gaussian excitation patterns (illegal muscle type)\n");
			error_code = 5;
			break;					
	}//end switch (excitation pattern type)

	//free memory
	free(gauss_exc_y);
	free(gauss_exc_x);

	return error_code;
}//end create_gauss_patterns



/********************* interpolate_spline_gauss *************************/
/************************************************************************/
/** This function is loosely based on the simm_pipeline function       **/
/** "interpolate_spline".  It will take a set of x,y coordinates and   **/
/** the desired x value and determine the desired y value by linear-   **/
/** izing between the two points adjacent to the desired value.        **/
/************************************************************************/
/********** Created by: Jeff Rankin Mar2009 *****************************/
//////////////////////////////////////////////////////////////////////////
double interpolate_spline_gauss(double abscissa, double* x_array, double* y_array, int points)
{

   int i;
   double dx,y;

  for (i = points-1; i >= 0; i--)
  {	  
	  if (abscissa >= x_array[i]){
		  //get the corresponding y value by linear interpolation between the two points
		  if (i == points-1){
			  //just return the last point
			  return y_array[i];
		  }
		  else if (i == 0){
			  //then just return the first point
			  return y_array[i];
		  }
		  else{
			  //linear interpolation between two points
			  dx = (abscissa-x_array[i])/(x_array[i+1] - x_array[i]);
			  y = y_array[i] + dx*(y_array[i+1] - y_array[i]);
			  return y;
		  }
	  }
  }

  /* If the abscissa is less than x[0], return y[0]. */
  return y_array[0];
}//end interpolate_spline_gauss




/************* Code to be added in different places - be sure to uncomment them ****************/



/*************** Code Set 1 ********************/

/*else if (mdata.ExcPatterns[m].pattern_source == 3){					
					for(i=0;i<N_emg_step;i++){
						mdata.emg_y[m][i] = 1.0;
					}					
				}
*/


/**************** Code Set 2 ************************/

/*//functions in GaussPattern.c
//Gaussian Pattern Functions - Mar2009
extern "C" int create_gauss_template(double shape_factor1, double shape_factor2, int exc_pattern);
*/

/******************* Code Set 3 ********************/

/*	//set the number of parameter values that are to be optimized based on the pattern type and source and allocate the necessary space
	for(i=0;i<N_musG;i++){
		if((mdata.ExcPatterns[i].pattern_type == 1) || (mdata.ExcPatterns[i].pattern_type == 2)){
			if (mdata.ExcPatterns[i].pattern_source == 3){
				mdata.ExcPatterns[i].num_excite_params = 4; //on, off, magnitude, shape
			}
			else{			
				mdata.ExcPatterns[i].num_excite_params = 3; //on, off, and magnitude
			}
		}
		else if(mdata.ExcPatterns[i].pattern_type == 3){
			if (mdata.ExcPatterns[i].pattern_source == 3){			
				mdata.ExcPatterns[i].num_excite_params = 8; //on, off, magnitude, shape for first region & on, off, mag, shape for second region
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
*/
