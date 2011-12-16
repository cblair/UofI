/*******************************************************************************

   READTOOLS.C

   Author: Peter Loan

   Date of creation: 01-JUL-91

   Copyright (c) 1996-2000 MusculoGraphics, Inc.
   All rights reserved.

   Description: This file contains tools for reading ascii text from
      input files, and for extracting numbers and strings from the text.

   Routines:
      preprocess_file           : runs acpp on a file, then opens it
      read_parameters_file      : reads the main simulation parameters file
      read_string               : reads a string from a file
      read_line                 : reads a possibly empty line from a file
      read_nonempty_line        : reads a non-empty line of chars from a file
      clean_string              : removes leading and trailing white space
      read_muscle_groups        : reads a set of muscle group names from a file
      read_muscle_attachment_points : reads muscle attachment points from a file
      read_double_array         : reads an array of x-y pairs from a file
      string_contains_closing_paren : checks to see if string has closing paren
      get_xy_pair_from_string   : reads x-y pair from a file, e.g. (0.00,0.00)
      parse_string              : extracts variable w/specified type from string
      enter_segment             : enters a segment name into a model's database
      enter_gencoord            : enters a gencoord name into a model's database
      load_kinetics_data        : reads a file of inverse dynamics data
      verify_kinetics_data      : checks kinetics data for completeness
      name_is_gencoord_deriv    : checks if a string is a gencoord or gc deriv
      name_is_body_segment      : checks is a string is a body segment name
      string_is_suffix          : compares one string to the end of another
      simm_malloc               : wrapper routine for malloc
      simm_calloc               : wrapper routine for calloc
      simm_realloc              : wrapper routine for realloc
      convert_string            : converts special chars in a string to '_'

*******************************************************************************/

#include "universal.h"


/*************** DEFINES (for this file only) *********************************/


/*************** STATIC GLOBAL VARIABLES (for this file only) *****************/
static char* velocity_suffix = "_vel";
static char* acceleration_suffix = "_acc";
static char* force_suffix = "_force";
static char* torque_suffix = "_torque";


/**************** GLOBAL VARIABLES (used in only a few files) *****************/
char buffer[500];
char errorbuffer[500];


/*************** EXTERNED VARIABLES (declared in another file) ****************/
extern SBoolean verbose;


/*************** PROTOTYPES for STATIC FUNCTIONS (for this file only) *********/
static ReturnCode verify_kinetics_data(SDModelStruct* sdm, MotionData* data,
				       SBoolean check_all_data);
static int name_is_gencoord_deriv(char name[], SDModelStruct* sdm, char suffix[]);
static int name_is_body_segment(SDModelStruct* sdm, char name[], char suffix[]);
static SBoolean string_is_suffix(char str1[], char suffix[]);
static SBoolean string_contains_closing_paren(char str_buffer[]);


ReturnCode read_parameters_file(char params_file[], SDModelStruct* sdm,
				char** muscle_file, char** kinetics_file,
				char** output_motion_file, char** output_kinetics_file)
{

   char* filename;
   FILE* fp;

   if ((fp = fopen(params_file,"r")) == NULL)
      return code_bad;
#ifdef SOLID
   init_object_info();
#endif
   while (1)
   {
      if (fscanf(fp,"%s", buffer) != 1)
	 break;
      if (STRINGS_ARE_EQUAL(buffer,"muscle_file") && muscle_file)
      {
	 read_line(&fp, buffer);
	 filename = clean_string(buffer);
	 if (filename == NULL || strlen(filename) < 1)
	 {
	    fprintf(stderr,"Error reading muscle_file name in %s\n", params_file);
	    exit(0);
	 }
	 else
	 {
	    mstrcpy(muscle_file,filename);
	 }
      }
      else if (STRINGS_ARE_EQUAL(buffer,"kinetics_file") && kinetics_file)
      {
	 read_line(&fp, buffer);
	 filename = clean_string(buffer);
	 if (filename == NULL || strlen(filename) < 1)
	 {
	    fprintf(stderr,"Error reading kinetics_file name in %s\n", params_file);
	    exit(0);
	 }
	 else
	 {
	    mstrcpy(kinetics_file,filename);
	 }
      }
      else if (STRINGS_ARE_EQUAL(buffer,"output_motion_file") && output_motion_file)
      {
	 read_line(&fp, buffer);
	 filename = clean_string(buffer);
	 if (filename == NULL || strlen(filename) < 1)
	 {
	    fprintf(stderr,"Error reading output_motion_file name in %s\n", params_file);
	    exit(0);
	 }
	 else
	 {
	    mstrcpy(output_motion_file,filename);
	 }
      }
      else if (STRINGS_ARE_EQUAL(buffer,"output_kinetics_file") && output_kinetics_file)
      {
	 read_line(&fp, buffer);
	 filename = clean_string(buffer);
	 if (filename == NULL || strlen(filename) < 1)
	 {
	    fprintf(stderr,"Error reading output_kinetics_file name in %s\n", params_file);
	    exit(0);
	 }
	 else
	 {
	    mstrcpy(output_kinetics_file,filename);
	 }
      }
#ifdef SOLID
      else if (STRINGS_ARE_EQUAL(buffer,"object"))
      {
	 read_object(&fp);
      }
      else if (STRINGS_ARE_EQUAL(buffer,"begin_group"))
      {
	 read_object_group(&fp);
      }
      else if (STRINGS_ARE_EQUAL(buffer,"collision_pair"))
      {
	 read_collision_pair(&fp);
      }
#endif
      else if (STRINGS_ARE_EQUAL(buffer,"verbose"))
      {
	 NULLIFY_STRING(buffer);
	 fscanf(fp,"%s", buffer);
	 if (STRINGS_ARE_EQUAL(buffer,"true"))
	    verbose = yes;
	 else if (STRINGS_ARE_EQUAL(buffer,"false"))
	    verbose = no;
	 else
	 {
	    fprintf(stderr,"Error reading verbose mode (true/false) in %s\n", params_file);
	    exit(0);
	 }
      }
      else
      {
	 fprintf(stderr,"Unknown string (%s) found in %s\n", buffer, params_file);
      }
   }

#ifdef SOLID
   check_object_info();
#endif

   return code_fine;

}


/* PREPROCESS_FILE: this routine runs the a C preprocessor on the specified
 * input file, putting the output in the specified output file. It then
 * opens the processed file and returns a pointer to a FILE structure for it.
 * NOTE: the original (Unix) version of preprocess_file() for the pipeline
 * called the ANSI system() routine to execute the C preprocessor.
 * Unfortunately, not all Win32 compilers support system(), nor do they all
 * provide command-line compilers.  So we avoid the problem by call a
 * C preprocessor linked directly into our executable (acpp.lib).
 */

static void acpp(char in_file[], const char out_file[], char option_string[])

{

   int argc = 0;
   char** argv;
   char* remaining_options = option_string;
   char option[CHARBUFFER];

   /* room for 50 arguments should be enough */
   argv = (char**) malloc(50 * sizeof(char*));
   mstrcpy(&argv[argc++], "acpp");

   while (1)
   {
      remaining_options = parse_string(remaining_options, type_string, option);

      if (option == NULL || STRING_IS_NULL(option))
	 break;

      mstrcpy(&argv[argc++], option);
   }

   mstrcpy(&argv[argc++], (char*) in_file);
   mstrcpy(&argv[argc++], (char*) out_file);

   remove(out_file); /* required to work-around CodeWarrior bug */

   if (acpp_main(argc,argv) != 0)
      fprintf(stderr, "Error running acpp on input file: %s.", in_file);

   while (argc > 0)
      free(argv[--argc]);

   free(argv);

}


FILE* preprocess_file(char infile[], char outfile[])
{

   FILE* fp = fopen(infile,"r");

   if (fp == NULL)
      return (NULL);

   fclose(fp);

   acpp(infile, outfile, "-P");

#ifndef WIN32
   chmod(outfile,438);
#endif

   return fopen(outfile, "r");

}



/* READ_STRING: this routine reads a character string from a file. It skips over
 * any white space at the beginning, and returns EOF if it hit the end of
 * the file before completing the string.
 */

int read_string(FILE** fp, char str_buffer[])
{

   int c;

   /* Scan thru white space until you find the first character in the string */

   while (1)
   {
      c = getc(*fp);
      if (c == EOF)
	 return (EOF);
      *str_buffer = c;
      if (CHAR_IS_NOT_WHITE_SPACE(*str_buffer))
	 break;
   }
   str_buffer++;

   /* Now read characters until you find white space or EOF */

   while (1)
   {
      c = getc(*fp);
      if (c == EOF)
	 return (EOF);
      *str_buffer = c;
      if (CHAR_IS_WHITE_SPACE(*str_buffer))
	 break;
      str_buffer++;
   }

   /* Null-terminate the string */

   *str_buffer = STRING_TERMINATOR;

   /* You found a valid string without hitting EOF, so return a value that you
    * know will never equal EOF, no matter how EOF is defined.
    */

   return (EOF+1);

}



/* READ_LINE: reads a line (possibly empty) from a file */

int read_line(FILE** fp, char str_buffer[])
{

   int c;

   /* Read characters until you hit a carriage return or EOF */

   while (1)
   {
      c = getc(*fp);
      if (c == EOF)
	 return (EOF);
      *str_buffer = c;
      if (*str_buffer == CARRIAGE_RETURN)
	 break;
      str_buffer++;
   }

   /* Null-terminate the string */

   *str_buffer = STRING_TERMINATOR;

   /* You found a valid line without hitting EOF, so return a value that you
    * know will never equal EOF, no matter how EOF is defined.
    */

   return (EOF+1);

}



/* READ_NONEMPTY_LINE: Reads the first non-empty line from a file. */

int read_nonempty_line(FILE** fp, char str_buffer[])
{

   int c;

   /* Scan thru the white space until you find the first character */

   while (1)
   {
      c = getc(*fp);
      if (c == EOF)
	 return (EOF);
      *str_buffer = c;
      if (*str_buffer != SPACE && *str_buffer != TAB)
	 break;
   }
   str_buffer++;

   /* Now read characters until you find a carriage return or EOF */

   while (1)
   {
      c = getc(*fp);
      if (c == EOF)
	 return (EOF);
      *str_buffer = c;
      if (*str_buffer == CARRIAGE_RETURN)
	 break;
      str_buffer++;
   }

   *str_buffer = STRING_TERMINATOR;

   /* You found a valid line without hitting EOF, so return a value that you
    * know will never equal EOF, no matter how EOF is defined.
    */

   return (EOF+1);

}


/* CLEAN_STRING: clears leading and trailing white space from a string */

char* clean_string(char buf[])
{

   char* ptr;

   if (strlen(buf) < 1)
      return (NULL);

   /* Remove trailing white space */
   ptr = &buf[strlen(buf)-1];
   while (CHAR_IS_WHITE_SPACE(*ptr))
      ptr--;
   *(ptr+1) = STRING_TERMINATOR;

   /* Remove leading white space */
   ptr = buf;
   while (CHAR_IS_WHITE_SPACE(*ptr))
      ptr++;

   return (ptr);

}


/* READ_DOUBLE_ARRAY: this routine reads an array of pairs-of-doubles
 * (e.g. "(2.30, -0.05)"), as in the definition of a function. It keeps
 * reading until it encounters the ending string (e.g. "endfunction")
 * which is passed in. This routine is not trivial because it allows
 * spaces to be placed liberally in the string that it extracts the
 * doubles from (e.g. "( 2.30 , -0.05 )").
 */

ReturnCode read_double_array(FILE **fp, char ending[], char name[],
			     SplineFunction* func)
{

   int new_size;

   func->numpoints = 0;

   while (1)
   {
      if (read_string(fp,buffer) == EOF)
      {
         sprintf(errorbuffer,"Unexpected EOF reading function %s.", name);
         error(abort_action,errorbuffer);
         return (code_bad);
      }

      if (STRINGS_ARE_EQUAL(buffer,ending))
         break;

      /* If the string just read is not the ending string (e.g. "endfunction"),
       * then it must be part or all of an x-y pair "(x,y)". It cannot be more
       * than one x-y pair because there must be a space between each x-y pair.
       * Since there can be spaces within an x-y pair, you now want to continue
       * reading strings from the file until you encounter a closing parenthesis.
       */

      /* Before parsing this next string, first make sure that there is enough
       * space in the function structure for the next x-y pair.
       */

      if (func->numpoints == func->coefficient_array_size)
      {
	 new_size = func->coefficient_array_size + SPLINE_ARRAY_INCREMENT;
	 if (realloc_function(func,new_size) == code_bad)
	    return (code_bad);
      }

      while (1)
      {
	 if (string_contains_closing_paren(buffer) == yes)
	    break;

	 if (read_string(fp,&buffer[strlen(buffer)]) == EOF)
	 {
	    sprintf(errorbuffer,"Unexpected EOF reading function %s.", name);
	    error(abort_action,errorbuffer);
	    return (code_bad);
	 }
      }

      if (get_xypair_from_string(buffer,&func->x[func->numpoints],
			&func->y[func->numpoints]) == code_bad)
      {
	 sprintf(errorbuffer,"Error reading x-y pair in function %s.", name);
	 error(abort_action,errorbuffer);
	 return (code_bad);
      }
      
      (func->numpoints)++;
   }

   if (func->numpoints < 2)
   {
      sprintf(errorbuffer,"Function %s contains fewer than 2 points.", name);
      error(abort_action,errorbuffer);
      return (code_bad);
   }

   return (code_fine);

}



/* STRING_CONTAINS_CLOSING_PAREN: this routine scans a string to see if
 * it contains a closing parenthesis.
 */

static SBoolean string_contains_closing_paren(char str_buffer[])
{

   while (*str_buffer != STRING_TERMINATOR)
   {
      if (*(str_buffer++) == ')')
	 return (yes);
   }

   return (no);

}



/* GET_XYPAIR_FROM_STRING: this routine parses a string to extract a pair
 * of doubles from it. The string should be of the form: "(x, y)".
 */

ReturnCode get_xypair_from_string(char str_buffer[], double* x, double* y)
{

   char junk;
   char* buffer_ptr;

   *x = *y = ERROR_DOUBLE;

   buffer_ptr = parse_string(str_buffer,type_char,(void*)&junk); /* open paren */
   buffer_ptr = parse_string(buffer_ptr,type_double,(void*)x);   /* x coord */
   buffer_ptr = parse_string(buffer_ptr,type_char,(void*)&junk); /* comma */
   buffer_ptr = parse_string(buffer_ptr,type_double,(void*)y);   /* y coord */
   buffer_ptr = parse_string(buffer_ptr,type_char,(void*)&junk); /* close paren*/

   if (*x == ERROR_DOUBLE || *y == ERROR_DOUBLE)
      return (code_bad);

   return (code_fine);

}



/* PARSE_STRING: this routine scans a string and extracts a variable from
 * it. The type of variable that it extracts is specified in one of the
 * arguments. It returns the unused portion of the string so that more
 * variables can be extracted from it.
 */

char* parse_string(char str_buffer[], VariableType var_type,
		   void* dest_var)
{

   char tmp_buffer[CHARBUFFER], *buffer_ptr;

   if (str_buffer == NULL)
      return (NULL);

   buffer_ptr = tmp_buffer;

   while (CHAR_IS_WHITE_SPACE(*str_buffer))
      str_buffer++;

   if (var_type == type_char)
   {
      *((char*)dest_var) = *str_buffer;
      return (str_buffer+1);
   }

   if (var_type == type_string)
   {
      if (STRING_IS_NULL(str_buffer))
         *((char*)dest_var) = STRING_TERMINATOR;
      else
         sscanf(str_buffer,"%s", (char*)dest_var);
      return (str_buffer + strlen((char*)dest_var));
   }

   if (var_type == type_double)
   {
      *((double*)dest_var) = ERROR_DOUBLE;
      if (STRING_IS_NOT_NULL(str_buffer))
      {
         while (*str_buffer == '-' || *str_buffer == '.' ||
		(*str_buffer >= '0' && *str_buffer <= '9'))
            *(buffer_ptr++) = *(str_buffer++);
         *buffer_ptr = STRING_TERMINATOR;
         if (sscanf(tmp_buffer,"%lf",(double*)dest_var) != 1)
            *((double*)dest_var) = ERROR_DOUBLE;
      }
      return (str_buffer);
   }

   if (var_type == type_int)
   {
      *((int*)dest_var) = ERROR_INT;
      if (STRING_IS_NOT_NULL(str_buffer))
      {
         while (*str_buffer == '-' || (*str_buffer >= '0' && *str_buffer <= '9'))
            *(buffer_ptr++) = *(str_buffer++);
         *buffer_ptr = STRING_TERMINATOR;
         sscanf(tmp_buffer,"%d",(int*)dest_var);
      }
      return (str_buffer);
   }

   sprintf(errorbuffer,"Unknown variable type (%d) passed to parse_string().",
	   (int)var_type);
   error(none,errorbuffer);

   return str_buffer;

}



/* ENTER_SEGMENT: this routine takes the name of a body segment and enters it
 * into the model's array of segment names. It returns the index of the array
 * entry where the name is stored (minus 1 since SD/FAST body numbers start at -1).
 * It is used when reading muscle attachment points. When a segment name is read
 * from a muscle file, you want to find out which SD/FAST body segment number the
 * name corresponds to. If the name is not in the SDModel structure, it is an error
 * since all of the body segments have already been defined. Before checking the
 * name, remove any special characters from it since this has been done to the
 * array of valid segment names (because they have to compile as #defines).
 */

int enter_segment(SDModelStruct* sdm, char segment_name[])
{

   int i;

   /* Because body segment numbers in SD/FAST start at -1 (ground),
    * this routine returns segment array indices minus one, so that
    * it also numbers segments starting at -1.
    */

   convert_string(segment_name);

   for (i=0; i<sdm->num_body_segments; i++)
      if (STRINGS_ARE_EQUAL(segment_name,sdm->body_segment[i].name))
         return (i-1);

   return (-2);

}



/* ENTER_GENCOORD: this routine takes the name of a generalized coordinate
 * and enters it into the model's array of gencoord (q) names. It returns the
 * index of the array entry where the name is stored. It is used when reading
 * muscles which have wrapping points that are functions of a generalized
 * coordinate. If the gencoord name found in the muscle file (and passed into
 * this routine) is not in the SDModel structure, it is an error since all of
 * the model's gencoords have already been defined. Before checking the name,
 * remove any special characters from it since this has been done to the array
 * of valid gencoord names (because they have to compile as #defines).
 */

int enter_gencoord(SDModelStruct* sdm, char gencoord_name[])
{

   int i;

   convert_string(gencoord_name);

   for (i=0; i<sdm->nq; i++)
      if (STRINGS_ARE_EQUAL(gencoord_name,sdm->q[i].name))
         return (i);

   return (-1);

}



/* LOAD_KINETICS_DATA: this routine reads a file of kinetics data. This file can
 * contain positions, velocities, and accelerations of Qs (gencoords), as well as
 * external forces and torques applied to the body segments. Thus it can be used
 * in a forward or inverse simulation.
 */

ReturnCode load_kinetics_data(SDModelStruct* sdm, MotionData* data, char filename[],
			      SBoolean check_all_data)
{

   int i, j, num_read, num_columns;
   FILE* fp;
   SBoolean read_frames = no, read_elements = no, read_otherdata = no;

   if ((fp = preprocess_file(filename,get_temp_file_name(".kinetics"))) == NULL)

   {
      sprintf(errorbuffer,"Unable to open motion file %s", filename);
      error(none,errorbuffer);
      return (code_bad);
   }

   data->num_frames = data->current_frame = 0;
   data->num_elements = data->num_otherdata = 0;
   data->num_forces = data->num_torques = 0;

   while (1)
   {

      if (read_string(&fp,buffer) == EOF)
      {
	 error(abort_action,"Premature EOF found in motion file.");
	 return (code_bad);
      }

      if (buffer[0] == '#')
      {
	 read_nonempty_line(&fp,buffer);
	 continue;
      }

      if (STRINGS_ARE_EQUAL(buffer,"frames"))
      {
	 if (fscanf(fp,"%d", &data->num_frames) != 1)
	 {
	    error(abort_action,"Error reading number of frames from motion file.");
	    return (code_bad);
	 }
	 else
	    read_frames = yes;
      }
      else if (STRINGS_ARE_EQUAL(buffer,"elements"))
      {
	 if (fscanf(fp,"%d", &data->num_elements) != 1)
	 {
	    error(abort_action,"Error reading number of elements from motion file.");
	    return (code_bad);
	 }
	 else
	    read_elements = yes;
      }
      else if (STRINGS_ARE_EQUAL(buffer,"otherdata"))
      {
	 if (fscanf(fp,"%d", &data->num_otherdata) != 1)
	 {
	    error(abort_action,"Error reading number of otherdata from motion file.");
	    return (code_bad);
	 }
	 else
	    read_otherdata = yes;
      }
      else
      {
         sprintf(errorbuffer,"Unrecognized string \"%s\" found in motion file.",
                 buffer);
         error(recover,errorbuffer);
      }

      if (read_frames == yes && read_elements == yes && read_otherdata == yes)
	 break;
   }

   data->elementnames = (char**)simm_malloc(data->num_elements*sizeof(char*));
   if (data->elementnames == NULL)
      return (code_bad);

   for (num_read=0, i=0; i<data->num_elements; i++)
   {
      num_read += fscanf(fp,"%s", buffer);
      mstrcpy(&data->elementnames[i],buffer);
   }

   if (num_read < data->num_elements)
   {
      (void)sprintf(buffer,"Error reading names of data elements. Only %d of %d read.",
		    num_read, data->num_elements);
      for (i=0; i<num_read; i++)
	 free(data->elementnames[i]);
      free(data->elementnames);
      return (code_bad);
   }

   for (num_columns = 0, i=0; i<data->num_elements; i++)
   {
      if (string_is_suffix(data->elementnames[i],"_force") == yes)
	 num_columns += 6;
      else if (string_is_suffix(data->elementnames[i],"_torque") == yes)
	 num_columns += 3;
      else
	 num_columns++;
   }

   data->motiondata = (double**)simm_malloc(num_columns*sizeof(double*));
   if (data->motiondata == NULL)
   {
      for (i=0; i<data->num_elements; i++)
	 free(data->elementnames[i]);
      free(data->elementnames);
      return (code_bad);
   }

   for (i=0; i<num_columns; i++)
   {
      data->motiondata[i] = (double*)simm_malloc(data->num_frames*sizeof(double));
      if (data->motiondata[i] == NULL)
      {
	 for (j=0; j<data->num_elements; j++)
	    free(data->elementnames[j]);
	 free(data->elementnames);
	 for (j=0; j<i; j++)
	    free(data->motiondata[j]);
	 return (code_bad);
      }
   }

   for (i=0; i<data->num_frames; i++)
   {
      num_read = 0;
      for (j=0; j<num_columns; j++)
         num_read += fscanf(fp,"%lg", &data->motiondata[j][i]);
      if (num_read < num_columns)
      {
	 data->num_frames = i;
	 (void)sprintf(buffer,"Error reading motion file %s:\n  Only %d frames found.",
		       filename, data->num_frames);
	 error(recover,buffer);
	 break;
      }
   }

   (void)fclose(fp);
   (void)unlink(get_temp_file_name(".kinetics"));


   sprintf(buffer,"Read %d frames of data from kinetics file %s", data->num_frames, filename);
   message(buffer);

   return (verify_kinetics_data(sdm,data,check_all_data));

}



/* VERIFY_KINETICS_DATA: this routine checks the validity of inverse dynamics data.
 * It first checks the column names in the file to make sure they match the model's
 * gencoords (for positions, velocities, and accelerations) and body segments (for
 * forces and torques). Then, if there are no closed loops in the model, it checks
 * to see if data for all Qs (gencoords) have been specified. If there are closed
 * loops, it doesn't check because it can't determine the minimum number of Qs that
 * have to be specified (and the user wouldn't specify all Qs).
 */

static ReturnCode verify_kinetics_data(SDModelStruct* sdm, MotionData* data,
				       SBoolean check_all_data)
{

   int i, gc, seg, column, num_otherdata;
   ForceStruct* fs;
   TorqueStruct* ts;

   data->q_data = (double**)simm_malloc(sdm->nq*sizeof(double*));
   data->u_data = (double**)simm_malloc(sdm->nq*sizeof(double*));
   data->udot_data = (double**)simm_malloc(sdm->nq*sizeof(double*));
   for (i=0; i<sdm->nq; i++)
   {
      data->q_data[i] = NULL;
      data->u_data[i] = NULL;
      data->udot_data[i] = NULL;
   }

   /* Malloc plenty of space for the force and torque arrays. */

   data->forces = (ForceStruct**)simm_malloc(data->num_elements*sizeof(ForceStruct*));
   data->torques = (TorqueStruct**)simm_malloc(data->num_elements*sizeof(TorqueStruct*));

   for (column=0, num_otherdata = 0, i=0; i<data->num_elements; i++)
   {
      if ((gc = name_is_gencoord_deriv(data->elementnames[i],sdm,NULL)) != -1)
	 data->q_data[gc] = data->motiondata[column++];
      else if ((gc = name_is_gencoord_deriv(data->elementnames[i],sdm,velocity_suffix)) != -1)
	 data->u_data[gc] = data->motiondata[column++];
      else if ((gc = name_is_gencoord_deriv(data->elementnames[i],sdm,acceleration_suffix)) != -1)
	 data->udot_data[gc] = data->motiondata[column++];
      else if ((seg = name_is_body_segment(sdm,data->elementnames[i],force_suffix)) != -2)
      {
	 data->forces[data->num_forces] = (ForceStruct*)simm_malloc(sizeof(ForceStruct));
	 fs = data->forces[data->num_forces++];
	 fs->segment = seg;
	 fs->appl_point[XX] = data->motiondata[column++];
	 fs->appl_point[YY] = data->motiondata[column++];
	 fs->appl_point[ZZ] = data->motiondata[column++];
	 fs->force_vec[XX] = data->motiondata[column++];
	 fs->force_vec[YY] = data->motiondata[column++];
	 fs->force_vec[ZZ] = data->motiondata[column++];
      }
      else if ((seg = name_is_body_segment(sdm,data->elementnames[i],torque_suffix)) != -2)
      {
	 data->torques[data->num_torques] = (TorqueStruct*)simm_malloc(sizeof(TorqueStruct));
	 ts = data->torques[data->num_torques++];
	 ts->segment = seg;
	 ts->torque_vec[XX] = data->motiondata[column++];
	 ts->torque_vec[YY] = data->motiondata[column++];
	 ts->torque_vec[ZZ] = data->motiondata[column++];
      }
      else if (num_otherdata >= data->num_otherdata)
      {
	 sprintf(buffer,"Too many unknown data elements (%s)", data->elementnames[i]);
	 error(none,buffer);
	 return (code_bad);
      }
      else
      {
	 data->num_otherdata++;
      }
   }

   if (check_all_data == no)
      return (code_fine);

   /* If there are no closed loops in the model, then for inverse dynamics all Qs must
    * be specified in the data file. If there are closed loops, don't check anything
    * since you can't determine the minimum number of Qs that the user needs to
    * completely specify the motion.
    * If there are closed loops in the model, all you want to check is that there are no
    * gencoords for which velocity and/or acceleration data are specified, but no position
    * data. This is because the code in sdinv.c checks to see if q_data[] is NULL when
    * deciding whether or not to prescibe the motion of that gencoord. So if q_data[i] is
    * NULL but u_data[i] and/or udot_data[i] are not NULL, that non-NULL information will
    * be ignored.
    */

   if (sdm->num_closed_loops == 0)
   {
      for (i=0; i<sdm->nq; i++)
      {
	 if (sdm->q[i].type != unconstrained_q)
	    continue;
	 if (data->q_data[i] == NULL)
	 {
	    sprintf(buffer,"Position data for %s not specified in file.", sdm->q[i].name);
	    error(none,buffer);
	    return (code_bad);
	 }
	 if (data->u_data[i] == NULL)
	 {
	    sprintf(buffer,"Velocity data for %s not specified in file.", sdm->q[i].name);
	    error(none,buffer);
	    error(none,"Using default value of 0.000\n");
	 }
	 if (data->udot_data[i] == NULL)
	 {
	    sprintf(buffer,"Acceleration data for %s not specified in file.", sdm->q[i].name);
	    error(none,buffer);
	    error(none,"Using default value of 0.000\n");
	 }
      }
   }
   else
   {
      for (i=0; i<sdm->nq; i++)
      {
	 if (sdm->q[i].type != unconstrained_q)
	    continue;
	 if (data->q_data[i] == NULL && (data->u_data[i] != NULL || data->udot_data[i] != NULL))
	 {
	    sprintf(buffer,"Position data for %s not specified in file.", sdm->q[i].name);
	    error(none,buffer);
	    error(none,"You cannot specify velocity or acceleration data without also");
	    error(none,"specifying position data.");
	    return (code_bad);
	 }
      }
   }

   message("Verified kinetics file.");

   return (code_fine);

}



/* NAME_IS_GENCOORD_DERIV: this routine checks to see if a string is the name
 * of one of the model's gencoords, or a derivative of same (e.g. knee_angle,
 * knee_angle_vel, knee_angle_acc).
 */

static int name_is_gencoord_deriv(char name[], SDModelStruct* sdm, char suffix[])
{

   int i, len, suffix_len;

   if (suffix == NULL)
   {
      for (i=0; i<sdm->nq; i++)
	 if (STRINGS_ARE_EQUAL(name,sdm->q[i].name))
	    return (i);

      return (-1);
   }

   len = strlen(name);
   suffix_len = strlen(suffix);

   if (len <= suffix_len)
      return (-1);

   if (string_is_suffix(name,suffix) == no)
      return (-1);

   strncpy(buffer,name,len-suffix_len);
   buffer[len-suffix_len] = STRING_TERMINATOR;

   for (i=0; i<sdm->nq; i++)
      if (STRINGS_ARE_EQUAL(buffer,sdm->q[i].name))
	 return (i);

   return (-1);

}



/* NAME_IS_BODY_SEGMENT: this routine checks to see if a string is the name
 * of one of the model's body segments. Since the C version of SD/FAST uses -1
 * as the number of the ground segment, -2 means there was no match.
 */

static int name_is_body_segment(SDModelStruct* sdm, char name[], char suffix[])
{

   int i, len, suffix_len;

   if (suffix == NULL)
   {
      for (i=0; i<sdm->num_body_segments; i++)
	 if (STRINGS_ARE_EQUAL(buffer,sdm->body_segment[i].name))
	    return (i-1);

      return (-2);
   }

   len = strlen(name);
   suffix_len = strlen(suffix);

   if (len <= suffix_len)
      return (-2);

   if (string_is_suffix(name,suffix) == no)
      return (-2);

   strncpy(buffer,name,len-suffix_len);
   buffer[len-suffix_len] = STRING_TERMINATOR;

   for (i=0; i<sdm->num_body_segments; i++)
      if (STRINGS_ARE_EQUAL(buffer,sdm->body_segment[i].name))
	 return (i-1);

   return (-2);

}




/* STRING_IS_SUFFIX: this routine compares one string to the end of another. */

static SBoolean string_is_suffix(char str1[], char suffix[])
{

   int i, j, len1, len2;

   len1 = strlen(str1);
   len2 = strlen(suffix);

   if (len2 > len1)
      return (no);

   for (i=len1-1, j=len2-1; j>= 0; i--, j--)
   {
      if (str1[i] != suffix[j])
	 return (no);
   }

   return (yes);

}



/* SIMM_MALLOC: this is a wrapper routine for malloc. It checks the size
 * you want to malloc, then calls malloc. If there is an error, it prints
 * an error message then returns NULL.
 */

void* simm_malloc(unsigned mem_size)
{

   void* ptr;

/* Temporary hack to make sure you don't try to malloc 0 bytes (which is a
 * problem because malloc returns NULL when you try it). The long term
 * solution is to check every place that calls simm_malloc() and don't call
 * it when mem_size is 0.
 */

   if (mem_size <= 0)
      mem_size = sizeof(int);

   ptr = malloc(mem_size);

   if (ptr == NULL)
   {
      sprintf(errorbuffer,"Ran out of memory. Unable to malloc %d bytes.",
	      (int)mem_size);
      error(none,errorbuffer);
   }

   return (ptr);

}



/* SIMM_CALLOC: this is a wrapper routine for calloc. It checks the size
 * you want to calloc, then calls calloc. If there is an error, it prints
 * an error message then returns NULL.
 */

void* simm_calloc(unsigned num_elements, unsigned elem_size)
{

   void* ptr;

/* Temporary hack to make sure you don't try to calloc 0 bytes (which is a
 * problem because calloc returns NULL when you try it). The long term
 * solution is to check every place that calls simm_calloc() and don't call
 * it when num_elements and/or elem_size is 0.
 */

   if (num_elements*elem_size <= 0)
   {
      num_elements = 1;
      elem_size = sizeof(int);
   }

   ptr = calloc(num_elements,elem_size);

   if (ptr == NULL)
   {
      sprintf(errorbuffer,"Ran out of memory. Unable to calloc %d bytes.",
	      (int)(num_elements*elem_size));
      error(none,errorbuffer);
   }

   return (ptr);

}



/* SIMM_REALLOC: this is a wrapper routine for realloc. It checks the size
 * you want to realloc to, then calls realloc. If there is an error, it prints
 * an error message then returns NULL.
 */

void* simm_realloc(void* ptr, unsigned mem_size, ReturnCode* rc)
{

   void* new_ptr;

   /* Temporary hack to make sure you don't try to realloc 0 bytes (which is a
    * problem because realloc returns NULL when you try it). The long term
    * solution is to check every place that calls simm_realloc() and don't call
    * it when mem_size is 0.
    */

   if (mem_size <= 0)
      mem_size = sizeof(int);

   new_ptr = realloc(ptr,mem_size);

   if (new_ptr == NULL)
   {
      sprintf(errorbuffer,"Ran out of memory. Unable to realloc %d bytes.",
	      (int)mem_size);
      *rc = code_bad;
      return (ptr);
   }

   *rc = code_fine;
   return (new_ptr);

}



/* CONVERT_STRING: this routine scans a string and converts all special
 * characters into underscores. A special character is any character
 * other than a letter or number. The resulting string is one token,
 * and can therefore be used as a variable name in SIMM-written C code.
 */

void convert_string(char str[])
{

   int i, len;

   len = strlen(str);

   for (i=0; i<len; i++)
   {
      if (str[i] >= 97 && str[i] <= 122)  /* lowercase letters */
	 continue;
      if (str[i] >= 65 && str[i] <= 90)   /* uppercase letters */
	 continue;
      if (str[i] >= 48 && str[i] <= 57)   /* numbers */
	 continue;
      str[i] = '_';
   }

}


FILE* simm_fopen(const char* name, const char* mode)
{
#ifdef WIN32
   if (mode && mode[0] == 'w')
      remove(name);
#endif
   
   return fopen(name, mode);
}


#ifdef WIN32
   #include "windows.h"
#endif

char* get_temp_file_name(const char* baseFileName)
{

   static char sTempFileNameBuffer[512];

#ifdef WIN32
   static char sTempFilePath[256];
   static int  sTempFilePathInited = 0;

   if ( ! sTempFilePathInited)
   {
      GetTempPath(sizeof(sTempFilePath), sTempFilePath);
      sTempFilePathInited = 1;
   }

   if (baseFileName && baseFileName[0] == '.' && baseFileName[1] != '\0')
      baseFileName++;

#else
   static char sTempFilePath[] = "/tmp/";
#endif

   strcpy(sTempFileNameBuffer, sTempFilePath);

   if (baseFileName)
      strcat(sTempFileNameBuffer, baseFileName);

   return sTempFileNameBuffer;

}

