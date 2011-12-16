/*******************************************************************************

   READMUSCLES.C

   Author: Peter Loan

   Date of creation: 01-JUL-91

   Copyright (c) 1996-2000 MusculoGraphics, Inc.
   All rights reserved.

   Description: This file contains routines that read-in muscle
      descriptions from an input file.

   Routines:
      read_muscle_file        : main routine to read a muscle file
      read_muscle             : reads a muscle definition from a file
      read_muscle_parameter   : reads a [double] muscle parameter from a file
      read_muscle_function    : reads a [muscle] spline function from a file
      read_excitation         : reads a muscle excitation pattern from a file
      init_muscle             : initializes a muscle structure
      nullify_muscle          : sets muscle structure elements to NULL
      read_dynamic_parameters : reads dynamic (muscle model) parameters 
      setup_muscle_wrapping   : initializes the wrapping portions of the muscle

*******************************************************************************/

#include "universal.h"


/*************** DEFINES (for this file only) *********************************/

#define MUSCLE_ARRAY_INCREMENT 40
#define MP_ARRAY_INCREMENT 10


/*************** STATIC GLOBAL VARIABLES (for this file only) *****************/



/*************** GLOBAL VARIABLES for a few files only) ***********************/



/*************** EXTERNED VARIABLES (declared in another file) ****************/

extern char buffer[];
extern char errorbuffer[];


/*************** PROTOTYPES for STATIC FUNCTIONS (for this file only) *********/

static ReturnCode read_muscle(FILE **fp, SDModelStruct* sdm, MuscleStruct* ms,
			      MuscleStruct* dm);
static ReturnCode read_muscle_parameter(FILE** fp, MuscleStruct* ms,
					MuscleStruct* dm, double** param,
					double* def_param, char param_name[],
					double min_value, double max_value);
static ReturnCode read_muscle_function(FILE** fp, MuscleStruct* ms,
				       MuscleStruct* dm, SplineFunction** mfunc,
				       SplineFunction* default_mfunc,
				       char ending[], char description[]);
static ReturnCode read_excitation(FILE** fp, SDModelStruct* sdm, MuscleStruct* ms,
				  MuscleStruct* dm);
static ReturnCode init_muscle(SDModelStruct* sdm, MuscleStruct* ms, MuscleStruct* dm);
static void nullify_muscle(MuscleStruct* ms);
static ReturnCode read_dynamic_parameters(FILE** fp, SDModelStruct* sdm);
static ReturnCode init_dynamic_param_array(SDModelStruct* sdm, MuscleStruct* ms);
static void check_dynamic_param_array(FILE** fp, MuscleStruct* ms, char text_string[]);
static void setup_muscle_wrapping(MuscleStruct* ms);
static const char* get_wrap_algorithm_name (int i);



/* READ_MUSCLE_FILE: this is the main routine for reading a muscle input
 * file. It runs cpp on the muscle file, then reads it in. When it finds
 * the string "beginmuscle" it calls read_muscle() read in a muscle
 * definition.
 */

ReturnCode read_muscle_file(SDModelStruct* sdm, char filename[])
{

   int i, j, muscle_array_size;
   FILE *fp;
   ReturnCode rc;
   

   if ((fp = preprocess_file(filename,get_temp_file_name(".muscles"))) == NULL)

   {
      sprintf(errorbuffer,"Unable to open muscle input file %s", filename);
      error(none,errorbuffer);
      return (code_bad);
   }

   sdm->num_muscles = 0;
   muscle_array_size = MUSCLE_ARRAY_INCREMENT;
   sdm->muscles = (MuscleStruct*)simm_malloc(muscle_array_size*sizeof(MuscleStruct));

   while (1)
   {
      if (read_string(&fp,buffer) == EOF)
         break;

      if (buffer[0] == '#')
      {
	 read_nonempty_line(&fp,buffer);
	 continue;
      }

      if (STRINGS_ARE_EQUAL(buffer,"beginmuscle"))
      {
         read_string(&fp,buffer);
         if (STRINGS_ARE_EQUAL(buffer,"defaultmuscle"))
	 {
	    nullify_muscle(&sdm->default_muscle);
	    init_dynamic_param_array(sdm,&sdm->default_muscle);
	    rc = read_muscle(&fp,sdm,&sdm->default_muscle,&sdm->default_muscle);
	    if (rc == code_bad)
	       error(exit_program,"Could not read defaultmuscle.");
	 }
         else
         {
	    if (sdm->num_muscles == muscle_array_size)
	    {
	       muscle_array_size += MUSCLE_ARRAY_INCREMENT;
	       sdm->muscles = (MuscleStruct*)simm_realloc(sdm->muscles,
			muscle_array_size*sizeof(MuscleStruct),&rc);
	       if (rc == code_bad)
		  error(exit_program,"Could not read in all of the muscles.");
	    }
            rc = init_muscle(sdm,&sdm->muscles[sdm->num_muscles],&sdm->default_muscle);
	    if (rc == code_bad)
	       return (code_bad);
            mstrcpy(&sdm->muscles[sdm->num_muscles].name,buffer);
            rc = read_muscle(&fp,sdm,&sdm->muscles[sdm->num_muscles],
			     &sdm->default_muscle);
            if (rc == code_fine)
	    {
	       setup_muscle_wrapping(&sdm->muscles[sdm->num_muscles]);
               sdm->num_muscles++;
	    }
	    else
	       return (code_bad);
         }
      }
      else if (STRINGS_ARE_EQUAL(buffer,"begindynamicparameters"))
      {
	 if (read_dynamic_parameters(&fp,sdm) == code_bad)
	    return (code_bad);
      }
      else
      {
         sprintf(errorbuffer,"Unrecognized string \"%s\" found in muscle file.",
                 buffer);
         error(recover,errorbuffer);
      }
   }

   fclose(fp);
   (void)unlink(get_temp_file_name(".muscles"));


   sprintf(buffer,"Read muscle file %s", filename);
   message(buffer);

   return (code_fine);

}



/* READ_MUSCLE: this routine reads a muscle definition from a file. It is
 * basically a big if-else block with one section for each muscle parameter.
 */

static ReturnCode read_muscle(FILE **fp, SDModelStruct* sdm, MuscleStruct* ms,
			      MuscleStruct* dm)
{

   int j, rc;

   while (1)
   {
      if (read_string(fp,buffer) == EOF)
         break;

      if (STRINGS_ARE_EQUAL(buffer,"beginpoints"))
      {
         ms->num_orig_points = (int*)simm_malloc(sizeof(int));
         ms->mp_orig = read_muscle_attachment_points(fp,sdm,ms->num_orig_points,
				  &ms->mp_array_size,&ms->has_wrapping_points);
         if (ms->mp_orig == NULL)
         {
            sprintf(errorbuffer,"error reading coordinates for muscle %s.\n",
                    ms->name);
            error(abort_action,errorbuffer);
            return (code_bad);
         }
      }
      else if (STRINGS_ARE_EQUAL(buffer,"begingroups"))
      {
         if (read_muscle_groups(fp) == code_bad)
	 {
            sprintf(errorbuffer,"EOF found while reading groups for muscle %s.\n",
                    ms->name);
            error(none,errorbuffer);
	    error(none,"Note: groups are ignored by the Pipeline.");
            return (code_bad);
         }
      }
      else if (STRINGS_ARE_EQUAL(buffer,"begintendonforcelengthcurve"))
      {
	 if (read_muscle_function(fp,ms,dm,&ms->tendon_force_len_curve,
		dm->tendon_force_len_curve,"endtendonforcelengthcurve",
		"tendon_force_length_curve") == code_bad)
	    return (code_bad);
      }
      else if (STRINGS_ARE_EQUAL(buffer,"beginactiveforcelengthcurve"))
      {
	 if (read_muscle_function(fp,ms,dm,&ms->active_force_len_curve,
		dm->active_force_len_curve,"endactiveforcelengthcurve",
		"active_force_length_curve") == code_bad)
	    return (code_bad);
      }
      else if (STRINGS_ARE_EQUAL(buffer,"beginpassiveforcelengthcurve"))
      {
	 if (read_muscle_function(fp,ms,dm,&ms->passive_force_len_curve,
		dm->passive_force_len_curve,"endpassiveforcelengthcurve",
		"passive_force_length_curve") == code_bad)
	    return (code_bad);
      }
      else if (STRINGS_ARE_EQUAL(buffer,"beginforcevelocitycurve"))
      {
	 if (read_muscle_function(fp,ms,dm,&ms->force_vel_curve,
		dm->force_vel_curve,"endforcevelocitycurve",
		"force_velocity_curve") == code_bad)
	    return (code_bad);
      }
      else if (STRINGS_ARE_EQUAL(buffer,"beginexcitation"))
      {
	 if (read_excitation(fp,sdm,ms,dm) == code_bad)
	    return (code_bad);
      }
      else if (STRINGS_ARE_EQUAL(buffer,"excitation_format"))
      {
         ms->excitation_format = (ExcitationFormat *)simm_malloc(sizeof(ExcitationFormat));
	 fscanf(*fp,"%s", buffer);
	 if (STRINGS_ARE_EQUAL(buffer,"spline_fit"))
	 {
	    *(ms->excitation_format) = spline_fit;
	 }
	 else if (STRINGS_ARE_EQUAL(buffer,"step_function"))
	 {
	    *(ms->excitation_format) = step_function;
	 }
	 else
	 {
            if (ms != dm && dm->excitation_format != NULL)
            {
               *(ms->excitation_format) = *(dm->excitation_format);
               sprintf(errorbuffer,
		       "Error reading excitation_format for muscle %s.\n",
		       ms->name);
               strcat(errorbuffer,"Using value from default muscle.");
               error(recover,errorbuffer);
            }
            else
            {
               sprintf(errorbuffer,
		       "Error reading excitation_format for muscle %s.",
                       ms->name);
               error(abort_action,errorbuffer);
               return (code_bad);
            }
         }
      }
      else if (STRINGS_ARE_EQUAL(buffer,"max_force"))
      {
	 if (read_muscle_parameter(fp,ms,dm,&ms->max_isometric_force,
				   dm->max_isometric_force,"max_force",
				   0.0,DONT_CHECK_DOUBLE) == code_bad)
	    return (code_bad);

      }
      else if (STRINGS_ARE_EQUAL(buffer,"tendon_slack_length"))
      {
	 if (read_muscle_parameter(fp,ms,dm,&ms->resting_tendon_length,
				   dm->resting_tendon_length,"tendon_slack_length",
				   0.0,DONT_CHECK_DOUBLE) == code_bad)
	    return (code_bad);
      }
      else if (STRINGS_ARE_EQUAL(buffer,"optimal_fiber_length"))
      {
	 if (read_muscle_parameter(fp,ms,dm,&ms->optimal_fiber_length,
				   dm->optimal_fiber_length,"optimal_fiber_length",
				   0.0,DONT_CHECK_DOUBLE) == code_bad)
	    return (code_bad);
      }
      else if (STRINGS_ARE_EQUAL(buffer,"pennation_angle"))
      {
	 if (read_muscle_parameter(fp,ms,dm,&ms->pennation_angle,
				   dm->pennation_angle,"pennation_angle",
				   0.0,90.0) == code_bad)
	    return (code_bad);
      }
      else if (STRINGS_ARE_EQUAL(buffer,"min_thickness"))
      {
	 if (read_muscle_parameter(fp,ms,dm,&ms->min_thickness,
				   dm->min_thickness,"min_thickness",
				   0.0,DONT_CHECK_DOUBLE) == code_bad)
	 {
	    error(none,"Note: This parameter is ignored by the Pipeline.");
	    return (code_bad);
	 }
      }
      else if (STRINGS_ARE_EQUAL(buffer,"max_thickness"))
      {
	 if (read_muscle_parameter(fp,ms,dm,&ms->max_thickness,
				   dm->max_thickness,"max_thickness",
				   0.0,DONT_CHECK_DOUBLE) == code_bad)
	 {
	    error(none,"Note: This parameter is ignored by the Pipeline.");
	    return (code_bad);
	 }
      }
      else if (STRINGS_ARE_EQUAL(buffer,"activation"))
      {
	 if (fscanf(*fp,"%lg", &ms->activation) != 1)
	 {
	    (void)sprintf(errorbuffer,"Error reading activation for muscle %s.",
			  ms->name);
	    error(none,errorbuffer);
	    error(none,"Note: This parameter is ignored by the Pipeline.");
	    return (code_bad);
	 }
      }
      else if (STRINGS_ARE_EQUAL(buffer,"max_contraction_velocity"))
      {
	 if (read_muscle_parameter(fp,ms,dm,&ms->max_contraction_vel,
				   dm->max_contraction_vel,"max_contraction_velocity",
				   0.0,DONT_CHECK_DOUBLE) == code_bad)
	    return (code_bad);
      }
      else if (STRINGS_ARE_EQUAL(buffer,"muscle_model"))
      {
         ms->muscle_model_index = (int *)simm_malloc(sizeof(int));
	 rc = fscanf(*fp,"%d", ms->muscle_model_index);
	 if (rc != 1 || *(ms->muscle_model_index) < 1 ||
	     *(ms->muscle_model_index) > 10)
         {
            if (ms != dm && dm->muscle_model_index != NULL)
            {
               *(ms->muscle_model_index) = *(dm->muscle_model_index);
               sprintf(errorbuffer,
		       "Error reading muscle_model for muscle %s.\n",
		       ms->name);
               strcat(errorbuffer,"Using value from default muscle.");
               error(recover,errorbuffer);
            }
            else
            {
               sprintf(errorbuffer,
		       "Error reading muscle_model for muscle %s.",
                       ms->name);
               error(abort_action,errorbuffer);
               return (code_bad);
            }
         }
	 else
	 {
	    (*ms->muscle_model_index)--;
	 }
      }
      else if (STRINGS_ARE_EQUAL(buffer,"wrapobject"))
      {
	 char* p;
	 int i;

	 read_line(fp,buffer);

	 p = strtok(buffer," \t");

	 if (p)
	 {
	    /* malloc the muscle wrap structure */
	    ms->muscwrap = (WrapStruct*)simm_malloc(sizeof(WrapStruct));
	    if (ms->muscwrap == NULL)
	       return (code_bad);
		/*
	    ms->muscwrap->wrap_object = get_wrap_object(p);
		*/
	    if (ms->muscwrap->wrap_object == NULL)
	    {
	       (void)sprintf(errorbuffer,"Unknown wrapobject specified for muscle %s.",
			     ms->name);
	       error(abort_action,errorbuffer);
	       free (ms->muscwrap);
	       return (code_bad);
	    }

	    ms->muscwrap->wrap_algorithm = ms->muscwrap->wrap_object->wrap_algorithm;
	    for (j=0; j<3; j++)
	    {
	       ms->muscwrap->c[j] = MINMDOUBLE;
	       ms->muscwrap->r1[j] = MINMDOUBLE;
	       ms->muscwrap->r2[j] = MINMDOUBLE;
	    }

	    /* if a wrap algorithm is specified, override the default */
	    p = strtok(NULL," \t");

	    if (p)
	    {
	       for (j=0; j<WE_NUM_WRAP_ALGORITHMS; j++)
	       {
		  if (STRINGS_ARE_EQUAL(p, get_wrap_algorithm_name(j)))
		  {
		     ms->muscwrap->wrap_algorithm = j;
		     break;
		  }
	       }
	    }
	 }
      }
      else if (STRINGS_ARE_EQUAL(buffer,"endmuscle"))
      {
         return (code_fine);
      }
      else
      {
	 check_dynamic_param_array(fp,ms,buffer);
      }
   }

   return code_fine;

}



/* READ_MUSCLE_PARAMETER: this routine reads a single muscle parameter from
 * a file. The parameter must be stored as a pointer to a double in the
 * muscle structure. If there is an error when reading the parameter, the
 * pointer will be set to the corresponding parameter in the default muscle.
 */

static ReturnCode read_muscle_parameter(FILE** fp, MuscleStruct* ms,
					MuscleStruct* dm, double** param,
					double* def_param, char param_name[],
					double min_value, double max_value)
{

   *param = (double *)simm_malloc(sizeof(double));
   if (*param == NULL)
      return (code_bad);

   if (fscanf(*fp,"%lf", *param) != 1)
   {
      if (ms != dm && def_param != NULL)
      {
	 free(*param);
	 *param = def_param;
	 sprintf(errorbuffer,"Error reading %s for muscle %s.\n",
		 param_name, ms->name);
	 strcat(errorbuffer,"Using value from default muscle.");
	 error(recover,errorbuffer);
	 return (code_fine);
      }
      else
      {
	 free(*param);
	 sprintf(errorbuffer,"Error reading %s for muscle %s.",
		 param_name, ms->name);
	 error(abort_action,errorbuffer);
	 return (code_bad);
      }
   }
   else
   {
      if (min_value != DONT_CHECK_DOUBLE)
      {
	 if (**param < min_value)
	 {
	    sprintf(errorbuffer,"Error reading %s for muscle %s:\n",
		    param_name, ms->name);
	    error(none,errorbuffer);
	    sprintf(errorbuffer,"   value of %lf is below min allowable value (%lf).",
		    **param, min_value);
	    error(abort_action,errorbuffer);
	    free(*param);
	    return (code_bad);
	 }
      }

      if (max_value != DONT_CHECK_DOUBLE)
      {
	 if (**param > max_value)
	 {
	    sprintf(errorbuffer,"Error reading %s for muscle %s:\n",
		    param_name, ms->name);
	    error(none,errorbuffer);
	    sprintf(errorbuffer,"   value of %lf is above max allowable value (%lf).",
		    **param, max_value);
	    error(abort_action,errorbuffer);
	    free(*param);
	    return (code_bad);
	 }
      }
   }

   return (code_fine);

}



/* READ_MUSCLE_FUNCTION: this routine reads a spline function for a muscle
 * (e.g. active force-length curve). If there is an error when reading from
 * the file, it sets the function equal to the corresponding function in the
 * default muscle.
 */

static ReturnCode read_muscle_function(FILE** fp, MuscleStruct* ms,
				       MuscleStruct* dm, SplineFunction** mfunc,
				       SplineFunction* default_mfunc,
				       char ending[], char description[])
{

   *mfunc = (SplineFunction*)simm_malloc(sizeof(SplineFunction));

   malloc_function(*mfunc,SPLINE_ARRAY_INCREMENT);

   if (read_double_array(fp,ending,description,*mfunc) == code_bad)
   {
      free((*mfunc)->x);
      free((*mfunc)->y);
      free((*mfunc)->b);
      free((*mfunc)->c);
      free((*mfunc)->d);
      free(*mfunc);

      if (ms != dm && default_mfunc != NULL)
      {
	 *mfunc = default_mfunc;
	 (void)sprintf(errorbuffer,
		       "Error reading %s for muscle %s.\n", description, ms->name);
	 (void)strcat(errorbuffer,"Using function from default muscle.");
	 error(recover,errorbuffer);
	 return (code_fine);
      }
      else
      {
	 (void)sprintf(errorbuffer,
		       "Error reading %s for muscle %s.", description, ms->name);
	 error(abort_action,errorbuffer);
	 return (code_bad);
      }
   }

   calc_spline_coefficients(*mfunc);

   return (code_fine);

}



/* READ_EXCITATION: this routine reads an excitation pattern for a muscle. It consists
 * of a sequence of number pairs, the first being the time (in seconds), the second
 * being the excitation level (from 0.0 to 1.0). These data points can then be fit
 * with a spline function, or be used as corners of a step function.
 */

static ReturnCode read_excitation(FILE** fp, SDModelStruct* sdm, MuscleStruct* ms,
				  MuscleStruct* dm)
{

   /* First read the name of the abscissa of the function. It should be either
    * the word "time" or the name of a generalized coordinate, of which this
    * excitation pattern will be a function.
    */

   if (fscanf(*fp,"%s", buffer) != 1)
   {
      (void)sprintf(errorbuffer,"Error reading name of abscissa in excitation function for muscle %s", ms->name);
      error(abort_action,errorbuffer);
      return (code_bad);
   }

   if (STRINGS_ARE_EQUAL(buffer,"time"))
   {
      ms->excitation_abscissa = TIME;
   }
   else
   {
      ms->excitation_abscissa = enter_gencoord(sdm,buffer);
      if (ms->excitation_abscissa == -1)
      {
	 (void)sprintf(errorbuffer,"Error reading abscissa of excitation function for muscle %s", ms->name);
	 error(none,errorbuffer);
	 (void)sprintf(errorbuffer,"%s is neither \"time\" nor the name of a generalized coordinate", buffer);
	 error(abort_action,errorbuffer);
	 return (code_bad);
      }
   }

   ms->excitation = (SplineFunction*)simm_malloc(sizeof(SplineFunction));

   ms->excitation->numpoints = 0;
   if (malloc_function(ms->excitation,500) == code_bad)
      return (code_bad);

   if (read_double_array(fp,"endexcitation","excitation-sequence",
			ms->excitation) == code_bad)
   {
      free(ms->excitation->x);
      free(ms->excitation->y);

      if (ms != dm && dm->excitation != NULL)
      {
	 ms->excitation = dm->excitation;
	 (void)sprintf(errorbuffer,
		       "Error reading excitation sequence for muscle %s.\n",
		       ms->name);
	 (void)strcat(errorbuffer,"Using same sequence as for default muscle.");
	 error(recover,errorbuffer);
	 return (code_fine);
      }
      else
      {
	 (void)sprintf(errorbuffer,
		       "Error reading excitation sequence for muscle %s.",
		       ms->name);
	 error(abort_action,errorbuffer);
	 return (code_bad);
      }
   }

   if (realloc_function(ms->excitation,ms->excitation->numpoints) == code_bad)
      return (code_bad);

   calc_spline_coefficients(ms->excitation);

   return (code_fine);

}


/* READ_MUSCLE_GROUPS: This routine reads names of muscle groups from a file
 * until the string "endgroups" is found. Muscle groups are used in SIMM,
 * but have no use in an SD/FAST simulation. So that a SIMM muscle file can
 * be read in by the GMC without changes, however, this routine needs to exist.
 * It just reads strings from the file until "endgroups" is found.
 */

ReturnCode read_muscle_groups(FILE** fp)
{

   while (1)
   {
      if (fscanf(*fp,"%s",buffer) != 1)
         return (code_bad);

      if (STRINGS_ARE_EQUAL(buffer,"endgroups"))
         return (code_fine);
   }

}


/* READ_MUSCLE_ATTACHMENT_POINTS: this routine reads muscle attachment points
 * from a file. It keeps reading from the file until the string "endpoints"
 * is encountered. It initially mallocs space for some points, and if it
 * fills up that space and finds more points, it reallocs the muscle point
 * array so that it can add the additional points.
 */

MusclePoint* read_muscle_attachment_points(FILE** fp, SDModelStruct* sdm,
					   int* numpoints, int* mp_array_size,
					   SBoolean* has_wrapping_points)
{

   int i;

   MusclePoint* mp;
   char com[CHARBUFFER], segment_name[CHARBUFFER], gencoord_name[CHARBUFFER];
   char* line;
   ReturnCode rc;

   *numpoints = 0;
   *mp_array_size = MP_ARRAY_INCREMENT;

   mp = (MusclePoint *)simm_malloc((*mp_array_size)*sizeof(MusclePoint));

   while (1)
   {
      fscanf(*fp,"%s",buffer);

      if (STRINGS_ARE_EQUAL(buffer,"endpoints"))
         return (mp);

      /* Check to see if you need to increase the size of the muscle-point
       * array before reading any more points.
       */

      if ((*numpoints) >= (*mp_array_size))
      {
	 (*mp_array_size) += MP_ARRAY_INCREMENT;
	 mp = (MusclePoint*)simm_realloc(mp,(*mp_array_size)*sizeof(MusclePoint),&rc);
	 if (rc == code_bad)
	    return (NULL);
      }

      /* If the string you just read was not "endpoints" then it must
       * be the start of a new point (so it's the x-coordinate).
       */

      if (sscanf(buffer,"%lf", &mp[*numpoints].point[XX]) != 1)
         return (NULL);
      read_nonempty_line(fp,buffer);

      line = parse_string(buffer,type_double,(void*)&mp[*numpoints].point[YY]);
      if (mp[*numpoints].point[YY] == ERROR_DOUBLE)
         return (NULL);

      line = parse_string(line,type_double,(void*)&mp[*numpoints].point[ZZ]);
      if (mp[*numpoints].point[ZZ] == ERROR_DOUBLE)
         return (NULL);

      /* read the keyword "segment" */

      line = parse_string(line,type_string,(void*)buffer);

      /* read the segment name */

      line = parse_string(line,type_string,(void*)segment_name);
      if (segment_name == NULL)
         return (NULL);

      mp[*numpoints].refpt = *numpoints;
      mp[*numpoints].segment = enter_segment(sdm,segment_name);
      if (mp[*numpoints].segment == -2)
	 return (NULL);

      /* In SD/FAST, the origin of a body's reference frame is located at the
       * center of mass. So subtract the coordinates of the center of mass
       * (w.r.t the SIMM reference frame) to get the coords of the muscle
       * point w.r.t the center of mass. The mass center coords for segment i
       * are located in the body segment array at element i+1 (because ground
       * is -1, but is the 0th element in the array).
       */

      for (i=0; i<3; i++)
	 mp[*numpoints].point[i] -=
	    sdm->body_segment[mp[*numpoints].segment+1].mass_center[i];

      /* read a string from the leftover part of the line. For most points,
       * the leftover will be NULL. For wrapping points, the first string
       * in the leftover should be "ranges".
       */

      line = parse_string(line,type_string,(void*)com);

      mp[*numpoints].numranges = 0;
      mp[*numpoints].ranges = NULL;

      if (STRINGS_ARE_EQUAL(com,"ranges"))
      {
         *has_wrapping_points = yes;
         line = parse_string(line,type_int,(void*)&mp[*numpoints].numranges);
	 if (mp[*numpoints].numranges <= 0)
	 {
	    error(none,"The number of ranges must be greater than 0.");
	    return (NULL);
	 }
	 mp[*numpoints].ranges = (PointRange*)simm_malloc(mp[*numpoints].numranges*
						     sizeof(PointRange));
         for (i=0; i<mp[*numpoints].numranges; i++)
         {
	    line = parse_string(line,type_string,(void*)gencoord_name);
	    if (gencoord_name == NULL)
	    {
	       free(mp[*numpoints].ranges);
               return (NULL);
	    }

	    if (get_xypair_from_string(line,&mp[*numpoints].ranges[i].start,
		&mp[*numpoints].ranges[i].end) == code_bad)
	    {
	       free(mp[*numpoints].ranges);
	       return (NULL);
	    }

            mp[*numpoints].ranges[i].genc = enter_gencoord(sdm,gencoord_name);
	    if (mp[*numpoints].ranges[i].genc == -1)
	       return (NULL);
         }
      }
/*TODO: convert degrees to radians for check_wrapping_points() ????????? */
      /* Each muscle point starts out 'on' (active) and unselected. */

      mp[*numpoints].state = on;
      mp[*numpoints].selected = no;
      mp[*numpoints].is_auto_wrap_point = no;
      mp[*numpoints].wrap_distance = 0.0;
      mp[*numpoints].num_wrap_pts = 0;
      mp[*numpoints].wrap_pts = NULL;

      (*numpoints)++;
   }
}


/* INIT_MUSCLE: this routine initializes much of the muscle structure. Before
 * a given muscle is read-in from an input file, each of the pointers to its
 * muscle-specific parameters is set to point into the default-muscle structure.
 * Thus if a certain parameter is not specified for a muscle (e.g. force-length
 * curve), the default-muscle's parameter is used. This cuts down significantly
 * on the amount of information that must be supplied in a muscle file since
 * most muscles share the same activeflcurve, tendonflcurve, passiveflcurve,
 * and fvcurve, as well as some tendon characteristics.
 */

static ReturnCode init_muscle(SDModelStruct* sdm, MuscleStruct* ms, MuscleStruct* dm)
{

   int i;

   ms->name = dm->name;
   ms->num_orig_points = dm->num_orig_points;
   ms->mp_orig = dm->mp_orig;
   ms->max_isometric_force = dm->max_isometric_force;
   ms->pennation_angle = dm->pennation_angle;
   ms->max_contraction_vel = dm->max_contraction_vel;
   ms->optimal_fiber_length = dm->optimal_fiber_length;
   ms->resting_tendon_length = dm->resting_tendon_length;
   ms->excitation_format = dm->excitation_format;
   ms->excitation = dm->excitation;
   ms->excitation_abscissa = dm->excitation_abscissa;
   ms->tendon_force_len_curve = dm->tendon_force_len_curve;
   ms->active_force_len_curve = dm->active_force_len_curve;
   ms->passive_force_len_curve = dm->passive_force_len_curve;
   ms->force_vel_curve = dm->force_vel_curve;
   ms->muscle_model_index = dm->muscle_model_index;

   ms->excitation_index = 0;
   ms->has_wrapping_points = no;
   ms->has_force_points = no;
   ms->first_on_point = 0;
   ms->num_points = 0;
   ms->mp = NULL;
   ms->mp_wrap = NULL;

   ms->muscle_tendon_length = 0.0;
   ms->fiber_length = 0.0;
   ms->tendon_length = 0.0;
   ms->muscle_tendon_vel = 0.0;
   ms->fiber_velocity = 0.0;
   ms->tendon_velocity = 0.0;
   ms->force = 0.0;
   ms->applied_power = 0.0;
   ms->energy = 0.0;
   ms->heat_energy = 0.0;
   ms->mechanical_energy = 0.0;
   ms->dynamic_activation = 0.0;

   ms->wrap_calced = no;
   ms->muscwrap = NULL;

   if (init_dynamic_param_array(sdm,ms) == code_bad)
      return (code_bad);

   for (i=0; i<ms->num_dynamic_params; i++)
      ms->dynamic_params[i] = dm->dynamic_params[i];

   /* The following elements are not used in the Pipeline. They are used in
    * SIMM, so for compatibility they are also included in the Pipeline's
    * muscle structure. They are initialized to arbitrary values.
    */

   ms->display = yes;
   ms->selected = no;
   ms->numgroups = 0;
   ms->group = NULL;
   ms->activation = 1.0;
   ms->initial_activation = 1.0;
   ms->nummomentarms = 0;
   ms->momentarms = NULL;
   ms->min_thickness = NULL;
   ms->max_thickness = NULL;

   return (code_fine);

}



/* NULLIFY_MUSCLE: this routine sets all of the pointers in a muscle structure
 * to NULL, and also sets some other elements to default values.
 */

static void nullify_muscle(MuscleStruct* ms)
{

   int i;

   ms->name = NULL;
   ms->has_wrapping_points = no;
   ms->has_force_points = no;
   ms->first_on_point = 0;
   ms->num_points = NULL;
   ms->mp = NULL;
   ms->max_isometric_force = NULL;
   ms->pennation_angle = NULL;
   ms->max_contraction_vel = NULL;
   ms->optimal_fiber_length = NULL;
   ms->resting_tendon_length = NULL;
   ms->excitation_format = NULL;
   ms->excitation = NULL;
   ms->excitation_abscissa = -1;
   ms->excitation_index = 0;
   ms->tendon_force_len_curve = NULL;
   ms->active_force_len_curve = NULL;
   ms->passive_force_len_curve = NULL;
   ms->force_vel_curve = NULL;
   ms->muscle_model_index = NULL;

   /* The following elements are not used in the Pipeline. They are used in
    * SIMM, so for compatibility they are also included in the Pipeline's
    * muscle structure. They are initialized to arbitrary values.
    */

   ms->display = no;
   ms->numgroups = 0;
   ms->group = NULL;
   ms->activation = 1.0;
   ms->initial_activation = 1.0;
   ms->nummomentarms = 0;
   ms->momentarms = NULL;
   ms->min_thickness = NULL;
   ms->max_thickness = NULL;

}



static ReturnCode read_dynamic_parameters(FILE** fp, SDModelStruct* sdm)
{

   int i, array_size = 10;
   ReturnCode rc;

   sdm->num_dynamic_params = 0;
   sdm->dynamic_param_names = (char**)simm_malloc(array_size*sizeof(char*));
   if (sdm->dynamic_param_names == NULL)
      return (code_bad);

   while (1)
   {
      if (fscanf(*fp,"%s",buffer) != 1)
      {
	 error(none,"EOF while reading dynamic parameters");
	 return (code_bad);
      }

      if (STRINGS_ARE_EQUAL(buffer,"enddynamicparameters"))
         break;

      /* Check to see if you need to increase the size of the array before
       * reading any more parameter names.
       */

      if (sdm->num_dynamic_params >= array_size)
      {
	 array_size += 10;
	 sdm->dynamic_param_names = (char**)simm_realloc(sdm->dynamic_param_names,
				    sdm->num_dynamic_params*sizeof(char*),&rc);
	 if (rc == code_bad)
	    return (code_bad);
      }

      mstrcpy(&sdm->dynamic_param_names[sdm->num_dynamic_params],buffer);

      sdm->num_dynamic_params++;
   }

   sdm->dynamic_param_names = (char**)simm_realloc(sdm->dynamic_param_names,
			      sdm->num_dynamic_params*sizeof(char*),&rc);
   return (rc);

}



static ReturnCode init_dynamic_param_array(SDModelStruct* sdm, MuscleStruct* ms)
{

   int i;

   ms->num_dynamic_params = sdm->num_dynamic_params;

   ms->dynamic_param_names = sdm->dynamic_param_names;

   ms->dynamic_params = (double**)simm_malloc(ms->num_dynamic_params*sizeof(double*));

   if (ms->dynamic_params == NULL)
      return (code_bad);

   for (i=0; i<ms->num_dynamic_params; i++)
      ms->dynamic_params[i] = NULL;

   return (code_fine);

}



static void check_dynamic_param_array(FILE** fp, MuscleStruct* ms, char text_string[])
{

   int i;

   for (i=0; i<ms->num_dynamic_params; i++)
   {
      if (STRINGS_ARE_EQUAL(text_string,ms->dynamic_param_names[i]))
      {
	 ms->dynamic_params[i] = (double*)simm_malloc(sizeof(double));
	 fscanf(*fp,"%lg", ms->dynamic_params[i]);
	 return;
      }
   }

   sprintf(errorbuffer,"Unrecognized string \"%s\" found while reading muscle %s.",
	   text_string, ms->name);
   error(recover,errorbuffer);

}


static void setup_muscle_wrapping(MuscleStruct* ms)
{

   int i;

   /* For now, the muscle can only wrap over one wrap object.
    * So you need to malloc space for only two new muscle points,
    * which go in mp_wrap. In check_wrapping_points, the mp[] array
    * will be set to points in mp_orig[] and mp_wrap[].
    */

   if (*(ms->num_orig_points) < 1)
      return;

   if (ms->muscwrap != NULL)
   {
      ms->mp_wrap = (MusclePoint*)simm_malloc(sizeof(MusclePoint)*2);
      if (ms->mp_wrap == NULL)
	 return;
      ms->mp_wrap[0].is_auto_wrap_point = yes;
      ms->mp_wrap[0].wrap_distance = 0.0;
      ms->mp_wrap[0].num_wrap_pts = 0;
      ms->mp_wrap[0].wrap_pts = NULL;
      ms->mp_wrap[1].is_auto_wrap_point = yes;
      ms->mp_wrap[1].wrap_distance = 0.0;
      ms->mp_wrap[1].num_wrap_pts = 0;
      ms->mp_wrap[1].wrap_pts = NULL;
      ms->mp = (MusclePoint**)simm_malloc(sizeof(MusclePoint*) *
					     (*(ms->num_orig_points)+2));
   }
   else
   {
      ms->mp = (MusclePoint**)simm_malloc(sizeof(MusclePoint*) *
					     (*(ms->num_orig_points)));
   }

   ms->num_points = *(ms->num_orig_points);

   for (i=0; i<ms->num_points; i++)
      ms->mp[i] = &ms->mp_orig[i];

}

static const char* get_wrap_algorithm_name (int i)
{
   static char* names[WE_NUM_WRAP_ALGORITHMS] = { "hybrid", "midpoint", "axial" };
   
   if (i >= 0 && i < WE_NUM_WRAP_ALGORITHMS)
      return names[i];
   
   return NULL;
}
