/*******************************************************************************

   PIPETOOLS.C

   Author: Peter Loan

   Date of creation: 08-AUG-91

   Copyright (c) 1996-2000 MusculoGraphics, Inc.
   All rights reserved.

   Description: This file contains routines that implement various
      mathematical functions and other utilities. Every one operates
      exclusively on the formal parameter list, and so does not change any
      external variables or other general model or plot structures.

   Routines:
      mstrcpy               : copies one string to another, mallocing first
      message               : prints a message to the screen
      convert_point         : converts a point from one frame to another
      error                 : prints a message based on an error status
      malloc_function       : mallocs space for a spline function
      realloc_function      : reallocs a spline function
      verify_assembly       : checks gencoord values in assembled model
      apply_external_forces : applies external forces and torques to segs
      prescribe_fixed_joints : prescribed Q,U,A of 'fixed' joints
      set_prescribed_motion : turns on/off prescribed motion for certain Qs
      set_data_frame        : fills in state vector with frame of inv dyn data
      write_data            : finds hinge torques needed for prescribed motion
      apply_joint_restraint_torques : applies torques to restrain joint motion
      prescribe_inverse_dynamics : prescribes motion according in inv dyn data
      get_muscle_param_value : returns current value of named parameter
      calc_system_energy     : calculates total system energy

*******************************************************************************/

#include "universal.h"


/*************** DEFINES (for this file only) *********************************/



/*************** STATIC GLOBAL VARIABLES (for this file only) *****************/



/**************** GLOBAL VARIABLES (used in only a few files) *****************/



/*************** EXTERNED VARIABLES (declared in another file) ****************/
extern char buffer[];
extern char errorbuffer[];


/*************** PROTOTYPES for STATIC FUNCTIONS (for this file only) *********/
static void make_quaternion(Quat q, const double axis[3], double angle);
static void rotate_matrix_by_quat(double m[][4], const Quat q);
static void quat_to_matrix (Quat q, double m[][4]);



/* MSTRCPY: this routine is like strcpy(), but it first mallocs space for
 * the copy of the string.
 */

ReturnCode mstrcpy(char* dest_str[], char original_str[])
{

   if ((*dest_str = (char*)simm_malloc(STRLEN(original_str)*sizeof(char))) == NULL)
      return (code_bad);

   (void)strcpy(*dest_str,original_str);

   return (code_fine);

}



/* MESSAGE: this routine prints a message to the screen. Someday it might be
 * more involved, like writing the messages to a log file as well as the
 * screen.
 */

void message(char message_str[])
{

   printf(message_str);
   printf("\n");

}


/* CONVERT_POINT: this function converts a point from one
 * reference frame to another.
 */
void convert_point(double pt[], int frame1, int frame2)
{

   int i;
   double vec[3], pt_g[3], origin_g[3], origin[] = {0.0,0.0,0.0};

   /* Convert pt to the ground frame */
   if (frame1 != GROUND)
   {
      sdpos(frame1,pt,pt_g);
   }
   else
   {
      for (i=0; i<3; i++)
	 pt_g[i] = pt[i];
   }

   /* Convert the origin of frame2 to the ground frame */
   if (frame2 != GROUND)
   {
      sdpos(frame2,origin,origin_g);
   }
   else
   {
      for (i=0; i<3; i++)
	 origin_g[i] = origin[i];
   }

   /* Form a vector from the origin of frame2 to pt,
    * expressed in the ground frame.
    */
   for (i=0; i<3; i++)
      vec[i] = pt_g[i] - origin_g[i];

   /* Convert vec from the ground frame to frame2,
    * storing the result in pt.
    */
   if (frame2 != GROUND)
      sdtrans(GROUND,vec,frame2,pt);
   else
   {
      for (i=0; i<3; i++)
	 pt[i] = vec[i];
   }

}


void copy_4x4matrix(double from[][4], double to[][4])
{

   to[0][0] = from[0][0];
   to[0][1] = from[0][1];
   to[0][2] = from[0][2];
   to[0][3] = from[0][3];
   to[1][0] = from[1][0];
   to[1][1] = from[1][1];
   to[1][2] = from[1][2];
   to[1][3] = from[1][3];
   to[2][0] = from[2][0];
   to[2][1] = from[2][1];
   to[2][2] = from[2][2];
   to[2][3] = from[2][3];
   to[3][0] = from[3][0];
   to[3][1] = from[3][1];
   to[3][2] = from[3][2];
   to[3][3] = from[3][3];

}


/* ERROR: this routine prints an error message depending on the string
 * and error status that is passed in.
 */

void error(ErrorAction action, char str_buffer[])
{

   (void)fprintf(stderr,"%s\n", str_buffer);

   if (action == recover)
      (void)fprintf(stderr,"Attempting to recover.\n");
   else if (action == abort_action)
      (void)fprintf(stderr,"Aborting...\n");
   else if (action == exit_program)
   {
      (void)fprintf(stderr,"Program terminated.\n");
      exit (0);
   }

}



/* MALLOC_FUNCTION: this routine mallocs space for a spline function.
 */

ReturnCode malloc_function(SplineFunction* func, int numpoints)
{

   func->coefficient_array_size = 0;

   if ((func->x = (double*)simm_malloc(numpoints*sizeof(double))) == NULL)
      return (code_bad);
   if ((func->y = (double*)simm_malloc(numpoints*sizeof(double))) == NULL)
      return (code_bad);
   if ((func->b = (double*)simm_malloc(numpoints*sizeof(double))) == NULL)
      return (code_bad);
   if ((func->c = (double*)simm_malloc(numpoints*sizeof(double))) == NULL)
      return (code_bad);
   if ((func->d = (double*)simm_malloc(numpoints*sizeof(double))) == NULL)
      return (code_bad);

   func->coefficient_array_size = numpoints;

   return (code_fine);

}



/* REALLOC_FUNCTION: this routine reallocs the arrays in a spline structure. */

ReturnCode realloc_function(SplineFunction* func, int size)
{

   ReturnCode rc1, rc2, rc3, rc4, rc5;

   func->coefficient_array_size = size;

   func->x = (double*)simm_realloc(func->x,size*sizeof(double),&rc1);
   func->y = (double*)simm_realloc(func->y,size*sizeof(double),&rc2);
   func->b = (double*)simm_realloc(func->b,size*sizeof(double),&rc3);
   func->c = (double*)simm_realloc(func->c,size*sizeof(double),&rc4);
   func->d = (double*)simm_realloc(func->d,size*sizeof(double),&rc5);

   if (rc1 == code_bad || rc2 == code_bad || rc3 == code_bad || rc4 == code_bad ||
       rc5 == code_bad)
      return (code_bad);

   return (code_fine);

}



/* VERIFY_ASSEMBLY: this routine checks to see if the assembled SD/FAST model has
 * any Qs (gencoords) that violate their allowable ranges of motion. A gencoord
 * is allowed to violate its range of motion if a restraint function is defined
 * for it (the assumption being that the restraint torque will eventually pull
 * the gencoord back into range.
 */

ReturnCode verify_assembly(SDModelStruct* sdm, double state[])
{

   int i;
   SBoolean valid = yes;

   for (i=0; i<sdm->nq; i++)
   {
      if (sdm->q[i].type != unconstrained_q)
	 continue;
      if ((sdm->q[i].restraint_func == NULL) &&
	  ((state[i]*sdm->q[i].conversion < sdm->q[i].range_start && sdm->q[i].min_restraint_func == NULL) ||
	  (state[i]*sdm->q[i].conversion > sdm->q[i].range_end && sdm->q[i].max_restraint_func == NULL)))
      {
	 sprintf(errorbuffer,"%s out of range: value = %lf, range = %lf to %lf\n",
	       sdm->q[i].name, state[i]*sdm->q[i].conversion, sdm->q[i].range_start, sdm->q[i].range_end);
	 error(none,errorbuffer);
	 valid = no;
      }
   }

   if (valid == no)
   {
      error(none,"Assembly invalid.\n");
      return (code_bad);
   }

   return (code_fine);

}



/* APPLY_EXTERNAL_FORCES: This routine applies external, user-defined forces
 * and torques. A typical example of an external force is a ground-reaction.
 * force. The forces and torques are specified in a data file and are loaded
 * into an inv_data structure.
 */

void apply_external_forces(MotionData* inv_data)
{

   int i;
   double appl_point[3], force_vec[3], torque_vec[3];

   for (i=0; i<inv_data->num_forces; i++)
   {
      appl_point[XX] = inv_data->forces[i]->appl_point[XX][inv_data->current_frame];
      appl_point[YY] = inv_data->forces[i]->appl_point[YY][inv_data->current_frame];
      appl_point[ZZ] = inv_data->forces[i]->appl_point[ZZ][inv_data->current_frame];
      force_vec[XX] = inv_data->forces[i]->force_vec[XX][inv_data->current_frame];
      force_vec[YY] = inv_data->forces[i]->force_vec[YY][inv_data->current_frame];
      force_vec[ZZ] = inv_data->forces[i]->force_vec[ZZ][inv_data->current_frame];
      sdpointf(inv_data->forces[i]->segment,appl_point,force_vec);
   }

   for (i=0; i<inv_data->num_torques; i++)
   {
      torque_vec[XX] = inv_data->torques[i]->torque_vec[XX][inv_data->current_frame];
      torque_vec[YY] = inv_data->torques[i]->torque_vec[YY][inv_data->current_frame];
      torque_vec[ZZ] = inv_data->torques[i]->torque_vec[ZZ][inv_data->current_frame];
      sdbodyt(inv_data->torques[i]->segment,torque_vec);
   }

}



/* PRESCRIBE_FIXED_JOINTS: This routine prescribes the position, velocity, and
 * acceleration of the fixed joints in the model. The fixed joints are the ones
 * that should be constant, but could not be implemented that way in SD/FAST.
 * So they are implemented as pin rotations that are prescribed to be constant.
 */

void prescribe_fixed_joints(SDModelStruct* sdm)
{

   int i;

   for (i=0; i<sdm->nq; i++)
   {
      if (sdm->q[i].type == prescribed_q)
      {
	 sdprespos(sdm->q[i].joint,sdm->q[i].axis,sdm->q[i].initial_value/
	    sdm->q[i].conversion);
	 sdpresvel(sdm->q[i].joint,sdm->q[i].axis,0.0);
	 sdpresacc(sdm->q[i].joint,sdm->q[i].axis,0.0);
      }
   }

}



/* SET_PRESCRIBED_MOTION: This routine turns on/off prescribed motion for all the
 * Qs of the specified type.
 */

void set_prescribed_motion(SDModelStruct* sdm, QType type, int value)
{

   int i;

   for (i=0; i<sdm->nq; i++)
      if (sdm->q[i].type == type)
	 sdpres(sdm->q[i].joint,sdm->q[i].axis,value);

}



/* SET_DATA_FRAME: This routine sets the Qs to their values in the specified frame of
 * the inverse dynamics data.
 */

void set_data_frame(SDModelStruct* sdm, MotionData* inv_data, double q[], double u[], int frame)
{

   int i;

   /* First set all the Qs to their values recorded by SIMM */

   if (frame == 0)
   {
      for (i=0; i<sdm->nq; i++)
	 q[i] = sdm->q[i].initial_value/sdm->q[i].conversion;
   }

   /* Now, for those Qs with data in the inverse dynamics data file, set them
    * to their values in the appropriate frame of data. For the [tree] weld joints,
    * which are implemented as prescribed pins, set the corresponding Q to 0.0.
    */

   for (i=0; i<sdm->nq; i++)
   {
      if (inv_data->q_data[i] != NULL)
	 q[i] = inv_data->q_data[i][frame]/sdm->q[i].conversion;
      else
	 q[i] = sdm->q[i].initial_value/sdm->q[i].conversion;

      if (inv_data->u_data[i] != NULL)
	 u[i] = inv_data->u_data[i][frame]/sdm->q[i].conversion;
      else
	 u[i] = 0.0;
   }

}



/* WRITE_DATA: This routine finds the hinge torques required to generate the prescribed
 * motion specified in the inverse dynamics data file, then write the torques to a file.
 * It is called once for each frame of inverse dynamics data, after sdderiv() has
 * calculated the torques.
 */

void write_data(SDModelStruct* sdm, MotionData* inv_data, FILE** fp)
{

   int i;
   double torque;

   for (i=0; i<sdm->nq; i++)
   {
      if (sdm->q[i].type == unconstrained_q && inv_data->q_data[i] != NULL)
      {
	 sdgetht(sdm->q[i].joint,sdm->q[i].axis,&torque);
	 fprintf(*fp,"%10.5lf ", torque);
      }
   }

   fprintf(*fp,"\n");

}



/* APPLY_JOINT_RESTRAINT_TORQUES: This routine uses the user-defined restraint torque
 * functions to apply torques to the degrees of freedom in order to keep them from
 * going out of the desirable ranges of motion. The new method of specifying restraint
 * torques uses one function (restraint_func) which covers the complete range of motion
 * as well as off both ends. If this function is defined, use it, otherwise use the
 * old method (min_restraint_func and max_restraint_func).
 */

void apply_joint_restraint_torques(SDModelStruct* sdm, double state[])
{

   int i;
   double state_value, difference, torque;

   for (i=0; i<sdm->nq; i++)
   {
      if (sdm->q[i].type != unconstrained_q)
	 continue;
      state_value = state[i]*sdm->q[i].conversion;
      if (sdm->q[i].restraint_func != NULL)
      {
	 torque = interpolate_spline(state_value,sdm->q[i].restraint_func,zeroth,1.0,1.0);
	 sdhinget(sdm->q[i].joint,sdm->q[i].axis,torque);
      }
      else if (state_value < sdm->q[i].range_start && sdm->q[i].min_restraint_func != NULL)
      {
	 difference = sdm->q[i].range_start - state_value;
	 torque = interpolate_spline(difference,sdm->q[i].min_restraint_func,
				     zeroth,1.0,1.0);
/*	 printf("%s too low, torque = %lf\n", sdm->q[i].name, torque);*/
	 sdhinget(sdm->q[i].joint,sdm->q[i].axis,torque);
      }
      else if (state_value > sdm->q[i].range_end && sdm->q[i].max_restraint_func != NULL)
      {
	 difference = state_value - sdm->q[i].range_end;
	 torque = -interpolate_spline(difference,sdm->q[i].max_restraint_func,
				      zeroth,1.0,1.0);
/*	 printf("%s too high, torque = %lf\n", sdm->q[i].name, torque);*/
	 sdhinget(sdm->q[i].joint,sdm->q[i].axis,torque);
      }
   }

}



/* PRESCRIBE_INVERSE_DYNAMICS: This routine calls the sdpresXXX() routines to prescribe
 * the position, velocity, and acceleration of the gencoords for which the user has
 * supplied data in the inverse dynamics data file.
 */

void prescribe_inverse_dynamics(SDModelStruct* sdm, MotionData* inv_data, int frame)
{

   int i;

   for (i=0; i<sdm->nq; i++)
   {
      if (sdm->q[i].type != unconstrained_q || inv_data->q_data[i] == NULL)
	 continue;
      sdprespos(sdm->q[i].joint,sdm->q[i].axis,inv_data->q_data[i][frame]/sdm->q[i].conversion);
      if (inv_data->u_data[i] == NULL)
	 sdpresvel(sdm->q[i].joint,sdm->q[i].axis,0.0);
      else
	 sdpresvel(sdm->q[i].joint,sdm->q[i].axis,inv_data->u_data[i][frame]/sdm->q[i].conversion);
      if (inv_data->udot_data[i] == NULL)
	 sdpresacc(sdm->q[i].joint,sdm->q[i].axis,0.0);
      else
	 sdpresacc(sdm->q[i].joint,sdm->q[i].axis,inv_data->udot_data[i][frame]/sdm->q[i].conversion);
   }

}



/* GET_MUSCLE_PARAM_VALUE: This routine gives the Pipeline programmer access to the
 * muscle parameters. Since the parameters used in the dynamics code are defined
 * at runtime, the programmer who writes the muscle model code in derivs.c cannot
 * just access the parameters by looking into the muscle structure. Instead, one
 * must pass the name of the parameter into this routine, which returns the value
 * for that parameter. For completeness, this routine also gives access to the
 * SIMM/static parameters (e.g. max_isometric_force), although this routine is not
 * required for this access.
 */

double get_muscle_param_value(MuscleStruct* ms, char param_name[])
{

   int i;

   for (i=0; i<ms->num_dynamic_params; i++)
   {
      if (STRINGS_ARE_EQUAL(param_name,ms->dynamic_param_names[i]))
      {
	 if (ms->dynamic_params[i] == NULL)
	 {
	    sprintf(buffer,"Error: %s was not specified for muscle %s",
		    param_name, ms->name);
	    error(none,buffer);
	    return (0.0);
	 }
	 else
	 {
	    return (*ms->dynamic_params[i]);
	 }
      }
   }

   if (STRINGS_ARE_EQUAL(param_name,"max_force"))
   {
      if (ms->max_isometric_force == NULL)
      {
	 sprintf(buffer,"Error: %s was not specified for muscle %s",
		 param_name, ms->name);
	 error(none,buffer);
	 return (0.0);
      }
      else
      {
	 return (*ms->max_isometric_force);
      }
   }

   if (STRINGS_ARE_EQUAL(param_name,"pennation_angle"))
   {
      if (ms->pennation_angle == NULL)
      {
	 sprintf(buffer,"Error: %s was not specified for muscle %s",
		 param_name, ms->name);
	 error(none,buffer);
	 return (0.0);
      }
      else
      {
	 return (*ms->pennation_angle);
      }
   }

   if (STRINGS_ARE_EQUAL(param_name,"optimal_fiber_length"))
   {
      if (ms->optimal_fiber_length == NULL)
      {
	 sprintf(buffer,"Error: %s was not specified for muscle %s",
		 param_name, ms->name);
	 error(none,buffer);
	 return (0.0);
      }
      else
      {
	 return (*ms->optimal_fiber_length);
      }
   }

   if (STRINGS_ARE_EQUAL(param_name,"tendon_slack_length"))
   {
      if (ms->resting_tendon_length == NULL)
      {
	 sprintf(buffer,"Error: %s was not specified for muscle %s",
		 param_name, ms->name);
	 error(none,buffer);
	 return (0.0);
      }
      else
      {
	 return (*ms->resting_tendon_length);
      }
   }

   if (STRINGS_ARE_EQUAL(param_name,"max_contraction_velocity"))
   {
      if (ms->max_contraction_vel == NULL)
      {
	 sprintf(buffer,"Error: %s was not specified for muscle %s",
		 param_name, ms->name);
	 error(none,buffer);
	 return (0.0);
      }
      else
      {
	 return (*ms->max_contraction_vel);
      }
   }

   sprintf(buffer,"Error: attempted to access undefined muscle parameter (%s)",
	   param_name);
   error(none,buffer);
   return (0.0);

}



/* CALC_SYSTEM_ENERGY: this routine calculates the total energy of the body
 * segments in the system. It does not keep track of the energy that the
 * muscles add/remove from the system, so if any muscles in the model generate
 * force, system energy will not be conserved.
 */

double calc_system_energy(void)
{

   double linear_momentum[3], angular_momentum[3], grav[3], kinetic_energy;
   double system_mass, mass_center[3], system_inertia[3][3], potential_energy;

   sdmom(linear_momentum,angular_momentum,&kinetic_energy);
   sdsys(&system_mass,mass_center,system_inertia);
   sdgetgrav(grav);

   potential_energy = -system_mass * (mass_center[XX]*grav[XX] +
		      mass_center[YY]*grav[YY] + mass_center[ZZ]*grav[ZZ]);

   return (kinetic_energy + potential_energy);

}


