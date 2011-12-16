/*******************************************************************************

   ASSIGNS.C

   Author: Peter Loan

   Date of creation: 11-SEP-91

   Copyright (c) 1996-2000 MusculoGraphics, Inc.
   All rights reserved.

   Description:  This file contains 10 routines to store muscle state values
      after the state variables have been integrated for a given time step.
      For example, in a muscle model where activation is a state, you would
      want to store the activation-state-variable value in the muscle structure
      after each time step so that you can print it out or write it to a file
      without having to figure out which element in the state vector it is
      every time you want to do something with it. The routines are named
      muscle_assign_func1() through muscle_assign_func10(), and each one
      stores all of a muscle's states wherever the user wants.


   Routines:
      muscle_assign_func1  : 3 states (act, fiber_len, fiber_vel), no pennation
      muscle_assign_func2  : 3-state model, pennation included
      muscle_assign_func3  : like func2 plus 4th state for muscle power
      muscle_assign_func4  : 2 states (act, fiber_len), non-zero passive force
      muscle_assign_func5  : 1 state (act), infinitely stiff tendon
      muscle_assign_func6  : unused
      muscle_assign_func7  : unused
      muscle_assign_func8  : unused
      muscle_assign_func9  : unused
      muscle_assign_func10 : unused

*******************************************************************************/

#include "universal.h"
#include "functions.h"


int muscle_assign_func1(double time, MuscleStruct *ms, double state[])
{

   int activation = 0, fiber_length = 1, fiber_velocity = 2;

   ms->dynamic_activation = state[activation];
   ms->fiber_length = state[fiber_length];
   ms->fiber_velocity = state[fiber_velocity];

   return (3);       /* number of states for this muscle model */

}


int muscle_assign_func2(double time, MuscleStruct *ms, double state[])
{

   int activation = 0, fiber_length = 1, fiber_velocity = 2;

   ms->dynamic_activation = state[activation];
   ms->fiber_length = state[fiber_length];
   ms->fiber_velocity = state[fiber_velocity];

   return (3);       /* number of states for this muscle model */

}


int muscle_assign_func3(double time, MuscleStruct *ms, double state[])
{

   int activation = 0, fiber_length = 1, fiber_velocity = 2, energy = 3;

   ms->dynamic_activation = state[activation];
   ms->fiber_length = state[fiber_length];
   ms->fiber_velocity = state[fiber_velocity];
   ms->energy = state[energy];

   return (4);       /* number of states for this muscle model */

}


int muscle_assign_func4(double time, MuscleStruct *ms, double state[])
{

   int activation = 0, fiber_length = 1;

   ms->dynamic_activation = state[activation];
   ms->fiber_length = state[fiber_length];

   return (2);       /* number of states for this muscle model */

}


int muscle_assign_func5(double time, MuscleStruct *ms, double state[])
{

   int activation = 0;

   ms->dynamic_activation = state[activation];

   return (1);       /* number of states for this muscle model */

}


int muscle_assign_func6(double time, MuscleStruct *ms, double state[])
{

   return (3);       /* number of states for this muscle model */

}


int muscle_assign_func7(double time, MuscleStruct *ms, double state[])
{

   return (3);       /* number of states for this muscle model */

}


int muscle_assign_func8(double time, MuscleStruct *ms, double state[])
{

   return (3);       /* number of states for this muscle model */

}


int muscle_assign_func9(double time, MuscleStruct *ms, double state[])
{

   return (3);       /* number of states for this muscle model */

}


int muscle_assign_func10(double time, MuscleStruct *ms, double state[])
{

   return (3);       /* number of states for this muscle model */

}
