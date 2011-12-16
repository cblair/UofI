/*******************************************************************************

   INITS.C

   Author: Peter Loan

   Date of creation: 09-JUL-91

   Copyright (c) 1996-2000 MusculoGraphics, Inc.
   All rights reserved.

   Description: This file contains 10 routines to initialize the states
      for dimensionless muscle-tendon models. The routines are named
      muscle_init_func1() through muscle_init_func10(), and each one
      initializes all of a muscle's states for a particular muscle-tendon
      model. The routine should be called at the beginning of your
      program, before entering the integration loop.

   Routines:
      muscle_init_func1  : 3 states (act, fiber_len, fiber_vel), no pennation
      muscle_init_func2  : 3-state model, pennation included
      muscle_init_func3  : like func2 plus 4th state for muscle power
      muscle_init_func4  : 2 states (act, fiber_len), non-zero passive force
      muscle_init_func5  : 1 state (act), infinitely stiff tendon
      muscle_init_func6  : unused
      muscle_init_func7  : unused
      muscle_init_func8  : unused
      muscle_init_func9  : unused
      muscle_init_func10 : unused

*******************************************************************************/

#include "universal.h"
#include "functions.h"


int muscle_init_func1(MuscleStruct *ms, double state[])
{

   int activation = 0, fiber_length = 1, fiber_velocity = 2;

   state[activation] = 0.0;
   state[fiber_length] = *(ms->optimal_fiber_length);
   state[fiber_velocity] = 0.0;

   return (3);       /* number of states for this muscle model */

}


int muscle_init_func2(MuscleStruct *ms, double state[])
{

   int activation = 0, fiber_length = 1, fiber_velocity = 2;

   /* Find static equilibrium for muscle. Use activation = 0.0 to
    * get initial fiber and tendon lengths for the muscle-tendon
    * actuator at the start of the simulation. This routine
    * includes pennation angle in the muscle model.
    */

   calc_muscle_tendon_force(ms,0.0);

   state[activation] = 0.0;
   state[fiber_length] = ms->fiber_length;
   state[fiber_velocity] = 0.0;

   return (3);       /* number of states for this muscle model */

}


int muscle_init_func3(MuscleStruct *ms, double state[])
{

   int activation = 0, fiber_length = 1, fiber_velocity = 2, energy = 3;

   /* Find static equilibrium for muscle. Use activation = 0.0 to
    * get initial fiber and tendon lengths for the muscle-tendon
    * actuator at the start of the simulation. This routine
    * includes pennation angle in the muscle model.
    */

   calc_muscle_tendon_force(ms,0.0);

   state[activation] = 0.0;
   state[fiber_length] = ms->fiber_length;
   state[fiber_velocity] = 0.0;
   state[energy] = 0.0;

   return (4);       /* number of states for this muscle model */

}


int muscle_init_func4(MuscleStruct *ms, double state[])
{

   int activation = 0, fiber_length = 1;

   double initial_act;  			/* Modified Code */

   /* Find static equilibrium for muscle. Use activation = 0.0 to
    * get initial fiber and tendon lengths for the muscle-tendon
    * actuator at the start of the simulation. This routine
    * includes pennation angle in the muscle model.
    */
   initial_act = ms->init_act;			/* Modified Code */

   calc_muscle_tendon_force(ms,initial_act);	/* Modified Code */

   state[activation] = initial_act;		/* Modified Code */

   state[fiber_length] = ms->fiber_length;

   return (2);       /* number of states for this muscle model */

}


int muscle_init_func5(MuscleStruct *ms, double state[])
{

   int activation = 0;

   state[activation] = 0.0;

   return (1);       /* number of states for this muscle model */

}


int muscle_init_func6(MuscleStruct *ms, double state[])
{

   return (3);       /* number of states for this muscle model */

}


int muscle_init_func7(MuscleStruct *ms, double state[])
{

   return (3);       /* number of states for this muscle model */

}


int muscle_init_func8(MuscleStruct *ms, double state[])
{

   return (3);       /* number of states for this muscle model */

}


int muscle_init_func9(MuscleStruct *ms, double state[])
{

   return (3);       /* number of states for this muscle model */

}


int muscle_init_func10(MuscleStruct *ms, double state[])
{

   return (3);       /* number of states for this muscle model */

}
