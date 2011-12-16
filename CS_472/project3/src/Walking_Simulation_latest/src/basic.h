/*******************************************************************************

   BASIC.H

   Author: Peter Loan

   Date of creation: 13-APR-89

   Copyright (c) 1996-2000 MusculoGraphics, Inc.
   All rights reserved.

   Description: This file contains #defines and enums that are used by most
      of the GMC source files.

*******************************************************************************/

#ifndef BASIC_H
#define BASIC_H

#define STRUCT typedef struct
#define UNION typedef union
#define ENUM typedef enum

#define GROUND -1
#define TRUE 1
#define FALSE 0

#define XX 0
#define YY 1
#define ZZ 2
#define WW 3

#define SPACE ' '
#define TAB '\t'
#define CARRIAGE_RETURN '\n'
#define STRING_TERMINATOR '\0'

#define TIME -1

#define END_OF_ARRAY -1
#define TINY_NUMBER 0.0000001

#define MAX_USER_FUNCTIONS 10
#define SPLINE_ARRAY_INCREMENT 20

#define MUSCLEBUFFER 96
#define SEGMENTBUFFER 32
#define GENBUFFER 64
#define GROUPBUFFER 96
#define CHARBUFFER 500

#define MAXMDOUBLE 99999.99
#define MINMDOUBLE -99999.99
#define ERROR_DOUBLE -999999.3
#define DONT_CHECK_DOUBLE -999999.5
#define ERROR_INT -32760
#define ROUNDOFF_ERROR 0.0000000000002
#define MINIMUM_DIVISOR 0.0000001
#define DEG_TO_RAD 0.017453292519943
#define RAD_TO_DEG 57.295779513082323
#define DTOR DEG_TO_RAD
#define RTOD RAD_TO_DEG
#define GRAV_CONSTANT 9.80665
#define MAX_MATRIX_SIZE 20

#ifndef M_PI
#define M_PI   3.1415926535897932384626433832795
#define M_PI_2 (M_PI / 2.0)
#endif

#define ON_RAY 0
#define ABOVE_RAY 1
#define BELOW_RAY 2

#ifdef WIN32
  #define DIR_SEP_CHAR   '\\'
  #define DIR_SEP_STRING "\\"
  
  #define COPY_COMMAND   "copy"
#else
  #define DIR_SEP_CHAR   '/'
  #define DIR_SEP_STRING "/"
  
  #define COPY_COMMAND   "cp"
#endif

/* wrapping algorithms
 */
#define WE_HYBRID_ALGORITHM    0   /* Frans + fan algorithm */
#define WE_MIDPOINT_ALGORITHM  1   /* midpoint algorithm    */
#define WE_AXIAL_ALGORITHM     2   /* Frans only algorithm  */
#define WE_NUM_WRAP_ALGORITHMS 3

#define WE_FAN_ALGORITHM       4   /* fan only algorithm    */

typedef double Quat[4];
typedef double DMatrix[4][4];

ENUM
{
   code_fine,                        /* fine, no error was encountered */
   code_bad                          /* bad, an error was encountered */
} ReturnCode;                        /* error condition values */


ENUM
{
   defining_element,                 /* defining an element's properties */
   declaring_element,                /* using the element name, not defining it */
   just_checking_element             /* just checking to see if already defined */
} EnterMode;                         /* modes when entering/defining model elements */


ENUM
{
   recover,                          /* recover from error */
   abort_action,                     /* abort action; critical error */
   exit_program,                     /* exit program; fatal error */
   none                              /* no action; no error */
} ErrorAction;                       /* error recovery actions */


ENUM
{
   zeroth,                           /* zeroth derivative */
   first,                            /* first derivative */
   second                            /* second derivative */
} Derivative;                        /* function derivative values */


ENUM
{
   no,                               /* no */
   yes                               /* yes */
} SBoolean;                          /* conventional boolean */


ENUM
{
   off,                              /* off */
   on                                /* on */
} OnOffSwitch;                       /* conventional on/off switch */


ENUM
{
   type_int,                         /* integer */
   type_double,                      /* double */
   type_char,                        /* single character */
   type_string                       /* string */
} VariableType;                      /* variable types that can be read from string */


ENUM
{
   old_ascii,
   new_ascii,
   binary,
   unknown,
   file_not_found
} FileType;

#endif /* BASIC_H */
