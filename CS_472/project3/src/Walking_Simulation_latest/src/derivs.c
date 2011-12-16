/*******************************************************************************

   DERIVS.C

   Author: Peter Loan
           contributions from Chris Raasch and Lisa Schutte

   Date of creation: 09-JUL-91

   Copyright (c) 1996-2000 MusculoGraphics, Inc.
   All rights reserved.

   Description: This file contains 10 routines to calculate state derivatives
      for dimensionless muscle-tendon models. The routines are named
      muscle_deriv_func1() through muscle_deriv_func10(), and each one
      calculates all of a muscle's derivatives for a particular muscle-tendon
      model.

   Routines:
      muscle_deriv_func1  : 3 states (act, fiber_len, fiber_vel), no pennation
      muscle_deriv_func2  : 3-state model, pennation included
      muscle_deriv_func3  : like func2 plus 4th state for muscle power
      muscle_deriv_func4  : 2 states (act, fiber_len), non-zero passive force
      muscle_deriv_func5  : 1 state (act), infinitely stiff tendon
      muscle_deriv_func6  : the same as func4 except it is for regression
      muscle_deriv_func7  : unused
      muscle_deriv_func8  : unused
      muscle_deriv_func9  : unused
      muscle_deriv_func10 : unused

*******************************************************************************/

#include "universal.h"
#include "functions.h"

int get_muscle_param_index(MuscleStruct* ms, char param_name[]);


int muscle_deriv_func1(double time, MuscleStruct* ms, double state[],
		       double dstate[], double* muscle_force)
{

   int activation = 0, fiber_length = 1, fiber_velocity = 2;
   double exc, tendon_force, fiber_force, muscle_mass;
   double normstate[3], normdstate[3], norm_tendon_length;
   double norm_muscle_tendon_length;
   double c_tau, mass_param, c_act_1, c_act_2;

   c_tau = get_muscle_param_value(ms,"timescale");
   mass_param = get_muscle_param_value(ms,"mass");
   c_act_1 = get_muscle_param_value(ms,"activation1");
   c_act_2 = get_muscle_param_value(ms,"activation2");

   exc = ms->excitation_level;

   /* Normalize the muscle states */
  
   normstate[activation] = state[activation];
   normstate[fiber_length] = state[fiber_length] / (*(ms->optimal_fiber_length));
   normstate[fiber_velocity] = state[fiber_velocity] *
      (c_tau / (*(ms->optimal_fiber_length)));
  
   /* Compute normalized muscle state derivatives */

   normdstate[activation] = (exc - normstate[activation]) * (c_act_1 * exc + c_act_2);
   normdstate[fiber_length] = normstate[fiber_velocity];
   norm_muscle_tendon_length = calc_muscle_tendon_length(ms) /
      (*(ms->optimal_fiber_length));
   norm_tendon_length = norm_muscle_tendon_length - normstate[fiber_length];

   tendon_force = calc_tendon_force(ms,norm_tendon_length);
   fiber_force = calc_fiber_force(ms,normstate[activation],normstate[fiber_length],
				  normstate[fiber_velocity]);

   muscle_mass = mass_param * (*(ms->optimal_fiber_length)/c_tau) *
      (*(ms->optimal_fiber_length)/c_tau);
   normdstate[fiber_velocity] = (tendon_force - fiber_force)/muscle_mass;

   /* Un-normalize the muscle state derivatives */
  
   dstate[activation] = normdstate[activation] / c_tau;
   dstate[fiber_length] = normdstate[fiber_length] *
      (*(ms->optimal_fiber_length)) / c_tau;
   dstate[fiber_velocity] = normdstate[fiber_velocity] *
      (*(ms->optimal_fiber_length)) / (c_tau * c_tau);
  
   /* Store tendon force (same as muscle-tendon force) */
   /* in the muscle structure */  

   ms->force = tendon_force * (*(ms->max_isometric_force));

   *muscle_force = ms->force;

   return (3);			/* number of states for this muscle model. */

}



/* func2 is just like func1 except that the fiber velocity equation accounts
 * for the pennation angle of the fibers.
 */

int muscle_deriv_func2(double time, MuscleStruct* ms, double state[],
		       double dstate[], double* muscle_force)
{

   int activation = 0, fiber_length = 1, fiber_velocity = 2;
   double exc, tendon_force, fiber_force, muscle_mass;
   double normstate[3], normdstate[3], norm_tendon_length, ca, ta;
   double norm_muscle_tendon_length, mass_term, vel_term, pennation_angle;
   double c_tau, mass_param, c_act_1, c_act_2;

   c_tau = get_muscle_param_value(ms,"timescale");
   mass_param = get_muscle_param_value(ms,"mass");
   c_act_1 = get_muscle_param_value(ms,"activation1");
   c_act_2 = get_muscle_param_value(ms,"activation2");

   exc = calc_excitation(ms,time,state);

   /* Normalize the muscle states */
  
   normstate[activation] = state[activation];
   normstate[fiber_length] = state[fiber_length]/(*(ms->optimal_fiber_length));
   normstate[fiber_velocity] = state[fiber_velocity] *
      (c_tau / (*(ms->optimal_fiber_length)));
  
   /* Compute normalized muscle state derivatives */

   normdstate[activation] = (exc - normstate[activation]) * (c_act_1 * exc + c_act_2);
   normdstate[fiber_length] = normstate[fiber_velocity];
   pennation_angle = calc_pennation(normstate[fiber_length],1.0,
				    (*(ms->pennation_angle)));
   ca = cos(DEG_TO_RAD*pennation_angle);
   ta = tan(DEG_TO_RAD*pennation_angle);
   norm_muscle_tendon_length = calc_muscle_tendon_length(ms) /
      (*(ms->optimal_fiber_length));
   norm_tendon_length = norm_muscle_tendon_length - normstate[fiber_length]*ca;
   tendon_force = calc_tendon_force(ms,norm_tendon_length);
   fiber_force = calc_fiber_force(ms,normstate[activation],normstate[fiber_length],
				  normstate[fiber_velocity]);

   muscle_mass = mass_param * (*(ms->optimal_fiber_length)/c_tau) *
      (*(ms->optimal_fiber_length)/c_tau);
   mass_term = (tendon_force*ca - fiber_force*ca*ca) / muscle_mass;
   vel_term = normstate[fiber_velocity]*normstate[fiber_velocity]*ta*ta /
      normstate[fiber_length];
   normdstate[fiber_velocity] = mass_term + vel_term;

   /* Un-normalize the muscle state derivatives */

   dstate[activation] = normdstate[activation] / c_tau;
   dstate[fiber_length] = normdstate[fiber_length] *
      (*(ms->optimal_fiber_length)) / c_tau;
   dstate[fiber_velocity] = normdstate[fiber_velocity] *
      (*(ms->optimal_fiber_length)) / (c_tau * c_tau);
  
   /* Store tendon force (same as muscle-tendon force) */
   /* in the muscle structure */  

   ms->force = tendon_force * (*(ms->max_isometric_force));

   *muscle_force = ms->force;

   return (3);			/* number of states for this muscle model. */

}



/* func3 is just like func2 but with a fourth state for muscle energy,
 * which is the integral of the power applied to the body segments.
 * NOTE: this muscle model has a known bug concerning the integration of
 * the muscle power state. Do not use this model.
 */

int muscle_deriv_func3(double time, MuscleStruct* ms, double state[],
		       double dstate[], double* muscle_force)
{

   int activation = 0, fiber_length = 1, fiber_velocity = 2, energy = 3;
   double exc, ca, ta, tendon_force, fiber_force, muscle_mass;
   double normstate[3], normdstate[3], norm_tendon_length;
   double pennation_angle, mass_term, vel_term, norm_muscle_tendon_length;
   double c_tau, mass_param, c_act_1, c_act_2;

   c_tau = get_muscle_param_value(ms,"timescale");
   mass_param = get_muscle_param_value(ms,"mass");
   c_act_1 = get_muscle_param_value(ms,"activation1");
   c_act_2 = get_muscle_param_value(ms,"activation2");

   exc = calc_excitation(ms,time,state);

   /* Normalize the muscle states */
  
   normstate[activation] = state[activation];
   normstate[fiber_length] = state[fiber_length]/(*(ms->optimal_fiber_length));
   normstate[fiber_velocity] = state[fiber_velocity] *
      (c_tau / (*(ms->optimal_fiber_length)));
   normstate[energy] = state[energy];
  
   /* Compute normalized muscle state derivatives */

   normdstate[activation] = (exc - normstate[activation]) * (c_act_1 * exc + c_act_2);
   normdstate[fiber_length] = normstate[fiber_velocity];
   pennation_angle = calc_pennation(normstate[fiber_length],1.0,
				    (*(ms->pennation_angle)));
   ca = cos(DEG_TO_RAD*pennation_angle);
   ta = tan(DEG_TO_RAD*pennation_angle);
   norm_muscle_tendon_length = calc_muscle_tendon_length(ms) /
      (*(ms->optimal_fiber_length));
   norm_tendon_length = norm_muscle_tendon_length - normstate[fiber_length]*ca;
   tendon_force = calc_tendon_force(ms,norm_tendon_length);
   fiber_force = calc_fiber_force(ms,normstate[activation],normstate[fiber_length],
				  normstate[fiber_velocity]);

   muscle_mass = mass_param * (*(ms->optimal_fiber_length)/c_tau) *
      (*(ms->optimal_fiber_length)/c_tau);
   mass_term = (tendon_force*ca - fiber_force*ca*ca) / muscle_mass;
   vel_term = normstate[fiber_velocity]*normstate[fiber_velocity]*ta*ta /
      normstate[fiber_length];
   normdstate[fiber_velocity] = mass_term + vel_term;
   normdstate[energy] = ms->applied_power;

   /* Un-normalize the muscle state derivatives */

   dstate[activation] = normdstate[activation] / get_muscle_param_value(ms,"timescale");
   dstate[fiber_length] = normdstate[fiber_length] *
      (*(ms->optimal_fiber_length)) / c_tau;
   dstate[fiber_velocity] = normdstate[fiber_velocity] *
      (*(ms->optimal_fiber_length)) / (c_tau * c_tau);
   dstate[energy] = normdstate[energy];
  
   /* Store tendon force (same as muscle-tendon force) */
   /* in the muscle structure */  

   ms->force = tendon_force * (*(ms->max_isometric_force));

   *muscle_force = ms->force;

   return (4);			/* number of states for this muscle model . */

}



/* func4 is Lisa Schutte's 2-state model which was designed to improve
 * performance and accuracy from the 3-state model (func2). It does not
 * include muscle mass, but uses an inverse force-velocity relation which
 * includes damping.
 */

int muscle_deriv_func4(double time, MuscleStruct* ms, double state[],
		       double dstate[], double* muscle_force)
{
	static int first_time = 1;
   int activation = 0, fiber_length = 1;
   double exc, tendon_force, active_force;
   double passive_force, velocity_dependent_force;
   double normstate[2], normdstate[2], norm_tendon_length, ca;
   double norm_muscle_tendon_length, pennation_angle;
   double h, w, new_fiber_length, new_pennation_angle, new_ca;
   double c_tau, c_act_1, c_act_2;
	static int c_tau_index = 0, c_act_1_index = 0, c_act_2_index = 0;

	if (first_time)
	{
		c_tau_index = get_muscle_param_index(ms,"timescale");
		c_act_1_index = get_muscle_param_index(ms,"activation1");
		c_act_2_index = get_muscle_param_index(ms,"activation2");
		first_time = 0;
	}

	c_tau = *(ms->dynamic_params[c_tau_index]);
	c_act_1 = *(ms->dynamic_params[c_act_1_index]);
	c_act_2 = *(ms->dynamic_params[c_act_2_index]);

   exc = ms->excitation_level;

   /* Normalize the muscle states */
  
   normstate[activation] = state[activation];
   normstate[fiber_length] = state[fiber_length] / (*(ms->optimal_fiber_length));
  
   /* Compute normalized muscle state derivatives */

   if (exc >= normstate[activation])
      normdstate[activation] = (exc - normstate[activation]) *
	 (c_act_1 * exc + c_act_2);
   else
      normdstate[activation] = (exc - normstate[activation]) * c_act_2;

   pennation_angle = calc_pennation(normstate[fiber_length],1.0,
				    (*(ms->pennation_angle)));
   ca = cos(DEG_TO_RAD*pennation_angle);
   norm_muscle_tendon_length = calc_muscle_tendon_length(ms) /
      (*(ms->optimal_fiber_length));
   norm_tendon_length = norm_muscle_tendon_length - normstate[fiber_length]*ca; 
   tendon_force = calc_tendon_force(ms,norm_tendon_length);
   passive_force = calc_nonzero_passive_force(ms,normstate[fiber_length],0.0);
   active_force = calc_active_force(ms,normstate[fiber_length]);

   /* If pennation equals 90 degrees, fiber length equals muscle width and fiber
    * velocity goes to zero.  Pennation will stay at 90 until tendon starts to
    * pull, then "stiff tendon" approximation is used to calculate approximate
    * fiber velocity (a kluge, I know)
    */

   if (EQUAL_WITHIN_ERROR(ca,0.0))
   {
      if (EQUAL_WITHIN_ERROR(tendon_force,0.0))
      {
	 normdstate[fiber_length] = 0.0;
	 ms->fiber_velocity = 0.0;
      }
      else
      {
	 h = norm_muscle_tendon_length - (*(ms->resting_tendon_length));
	 w = (*(ms->optimal_fiber_length)) * sin((*(ms->pennation_angle))*DEG_TO_RAD);
	 new_fiber_length = sqrt(h*h + w*w) / (*(ms->optimal_fiber_length));
	 new_pennation_angle = calc_pennation(new_fiber_length,1.0,
					      (*(ms->pennation_angle)));
	 new_ca = cos(new_pennation_angle);
	 normdstate[fiber_length] = calc_muscle_tendon_velocity(ms) *
	    c_tau / (*(ms->optimal_fiber_length))  * new_ca ;
	 ms->fiber_velocity = normdstate[fiber_length];
      }
   }
   else
   {
      velocity_dependent_force = tendon_force/ca - passive_force;
      normdstate[fiber_length] = calc_fiber_velocity(ms,normstate[activation],
				          active_force,velocity_dependent_force);
   }

   /* Un-normalize the muscle state derivatives */
  
   dstate[activation] = normdstate[activation] / c_tau;
   dstate[fiber_length] = normdstate[fiber_length] *
      (*(ms->optimal_fiber_length)) / c_tau;

   /* Store tendon force (same as muscle-tendon force) */
   /* in the muscle structure */  

   ms->force = tendon_force * (*(ms->max_isometric_force));

   *muscle_force = ms->force;

   return (2); /* return the number of states in the model */

}



/* func5 is a 1-state model proposed by Felix in the hopes that it would be
 * significantly faster than any other model. It uses an infinitely stiff
 * tendon to remove the fiber_len state, leaving only activation dynamics.
 */

int muscle_deriv_func5(double time, MuscleStruct* ms, double state[],
		       double dstate[], double* muscle_force)
{

   int activation = 0, fiber_length = 1;
   double exc, ca, h, w, fiber_force, tendon_force;
   double norm_fiber_length, norm_fiber_velocity;
   double normstate[1], normdstate[1], norm_tendon_length;
   double norm_muscle_tendon_length, pennation_angle;
   double c_tau, c_act_1, c_act_2;

   c_tau = get_muscle_param_value(ms,"timescale");
   c_act_1 = get_muscle_param_value(ms,"activation1");
   c_act_2 = get_muscle_param_value(ms,"activation2");

   exc = ms->excitation_level;

   /* Normalize the muscle states */
  
   normstate[activation] = state[activation];
  
   /* Compute normalized muscle state derivatives */

   if (exc - normstate[activation] >= 0.0)
      normdstate[activation] = (exc - normstate[activation]) *
	 (c_act_1 * exc + c_act_2);
   else
      normdstate[activation] = (exc - normstate[activation]) * c_act_2;

/* Without pennation:

   norm_muscle_tendon_length = calc_muscle_tendon_length(ms) /
      (*(ms->optimal_fiber_length));
   norm_tendon_length = (*(ms->resting_tendon_length)) / (*(ms->optimal_fiber_length));
   norm_fiber_length = (norm_muscle_tendon_length - norm_tendon_length);
   norm_fiber_velocity = calc_muscle_tendon_velocity(ms) * c_tau /
      (*(ms->optimal_fiber_length));
   tendon_force = calc_fiber_force(ms,normstate[activation],norm_fiber_length,
                                   norm_fiber_velocity);
*/

/* With pennation: */
   norm_muscle_tendon_length = calc_muscle_tendon_length(ms);
   norm_tendon_length = (*(ms->resting_tendon_length));
   h = norm_muscle_tendon_length - norm_tendon_length;
   w = (*(ms->optimal_fiber_length)) * sin((*(ms->pennation_angle))*DEG_TO_RAD);

   if (EQUAL_WITHIN_ERROR(h,0.0) || h < 0.0)
   {
      norm_fiber_length = w / (*(ms->optimal_fiber_length));
      norm_fiber_velocity = 0.0;
      tendon_force = 0.0;
   }
   else
   {
      norm_fiber_length = sqrt(h*h + w*w) / (*(ms->optimal_fiber_length));
      pennation_angle = calc_pennation(norm_fiber_length,1.0,(*(ms->pennation_angle)));
      ca = cos(DEG_TO_RAD*pennation_angle);
      norm_fiber_velocity = calc_muscle_tendon_velocity(ms) *
	 get_muscle_param_value(ms,"timescale") / (*(ms->optimal_fiber_length))  * ca ;

      fiber_force = calc_fiber_force(ms,normstate[activation],norm_fiber_length,
				     norm_fiber_velocity);
      tendon_force = fiber_force*ca;
   }

   /* Un-normalize the muscle state derivatives */
  
   dstate[activation] = normdstate[activation] / c_tau;
  
   /* Store force, length, and velocity in the muscle structure */  

   ms->force = tendon_force * (*(ms->max_isometric_force));
   ms->fiber_length = norm_fiber_length * (*(ms->optimal_fiber_length));
   ms->fiber_velocity = norm_fiber_velocity * (*(ms->optimal_fiber_length)) /
      c_tau;

   *muscle_force = ms->force;

   return (1);

}


int muscle_deriv_func6(double time, MuscleStruct* ms, double state[],
		       double dstate[], double q[], double* muscle_force)
{

   int activation = 0, fiber_length = 1;
   double exc, tendon_force, active_force;
   double passive_force, velocity_dependent_force;
   double normstate[2], normdstate[2], norm_tendon_length, ca;
   double norm_muscle_tendon_length, pennation_angle;
   double h, w, new_fiber_length, new_pennation_angle, new_ca;
   double c_tau, c_act_1, c_act_2;

   c_tau = get_muscle_param_value(ms,"timescale");
   c_act_1 = get_muscle_param_value(ms,"activation1");
   c_act_2 = get_muscle_param_value(ms,"activation2");

   exc = ms->excitation_level;

   /* Normalize the muscle states */
  
   normstate[activation] = state[activation];
   normstate[fiber_length] = state[fiber_length] / (*(ms->optimal_fiber_length));
  
   /* Compute normalized muscle state derivatives */

   if (exc >= normstate[activation])
      normdstate[activation] = (exc - normstate[activation]) *
	 (c_act_1 * exc + c_act_2);
   else
      normdstate[activation] = (exc - normstate[activation]) * c_act_2;

   pennation_angle = calc_pennation(normstate[fiber_length],1.0,
				    (*(ms->pennation_angle)));
   ca = cos(DEG_TO_RAD*pennation_angle);
   norm_muscle_tendon_length = calc_muscle_tendon_length(ms) /
      (*(ms->optimal_fiber_length));
   

   norm_tendon_length = norm_muscle_tendon_length - normstate[fiber_length]*ca;
   tendon_force = calc_tendon_force(ms,norm_tendon_length);
   passive_force = calc_nonzero_passive_force(ms,normstate[fiber_length],0.0);
   active_force = calc_active_force(ms,normstate[fiber_length]);

   /* If pennation equals 90 degrees, fiber length equals muscle width and fiber
    * velocity goes to zero.  Pennation will stay at 90 until tendon starts to
    * pull, then "stiff tendon" approximation is used to calculate approximate
    * fiber velocity (a kluge, I know)
    */

   if (EQUAL_WITHIN_ERROR(ca,0.0))
   {
      if (EQUAL_WITHIN_ERROR(tendon_force,0.0))
      {
	 normdstate[fiber_length] = 0.0;
	 ms->fiber_velocity = 0.0;
      }
      else
      {
	 h = norm_muscle_tendon_length - (*(ms->resting_tendon_length));
	 w = (*(ms->optimal_fiber_length)) * sin((*(ms->pennation_angle))*DEG_TO_RAD);
	 new_fiber_length = sqrt(h*h + w*w) / (*(ms->optimal_fiber_length));
	 new_pennation_angle = calc_pennation(new_fiber_length,1.0,
					      (*(ms->pennation_angle)));
	 new_ca = cos(new_pennation_angle);
	 normdstate[fiber_length] = calc_muscle_tendon_velocity(ms) *
	    c_tau / (*(ms->optimal_fiber_length))  * new_ca ;
	 ms->fiber_velocity = normdstate[fiber_length];
      }
   }
   else
   {
      velocity_dependent_force = tendon_force/ca - passive_force;
      normdstate[fiber_length] = calc_fiber_velocity(ms,normstate[activation],
				          active_force,velocity_dependent_force);
   }

   /* Un-normalize the muscle state derivatives */
  
   dstate[activation] = normdstate[activation] / c_tau;
   dstate[fiber_length] = normdstate[fiber_length] *
      (*(ms->optimal_fiber_length)) / c_tau;

   /* Store tendon force (same as muscle-tendon force) */
   /* in the muscle structure */  

   ms->force = tendon_force * (*(ms->max_isometric_force));

   *muscle_force = ms->force;

   return (2); /* return the number of states in the model */


}


int muscle_deriv_func7(double time, MuscleStruct* ms, double state[],
		       double dstate[], double* muscle_force)
{

   /* Make sure to set *muscle_force in this routine. */

   return (3); /* return the number of states in the model */

}


int muscle_deriv_func8(double time, MuscleStruct* ms, double state[],
		       double dstate[], double* muscle_force)
{

   /* Make sure to set *muscle_force in this routine. */

   return (3); /* return the number of states in the model */

}


int muscle_deriv_func9(double time, MuscleStruct* ms, double state[],
		       double dstate[], double* muscle_force)
{

   /* Make sure to set *muscle_force in this routine. */

   return (3); /* return the number of states in the model */

}


int muscle_deriv_func10(double time, MuscleStruct* ms, double state[],
			double dstate[], double* muscle_force)
{

   /* Make sure to set *muscle_force in this routine. */

   return (3); /* return the number of states in the model */

}

int get_muscle_param_index(MuscleStruct* ms, char param_name[])
{
   int i;

   for (i = 0; i < ms->num_dynamic_params; i++)
   {
      if (STRINGS_ARE_EQUAL(param_name, ms->dynamic_param_names[i]))
      {
			return i;
      }
   }

	printf("Error: %s was not specified for muscle %s.", param_name, ms->name);
	return 0; // should probably return -1, but code is not ready to handle it
}
