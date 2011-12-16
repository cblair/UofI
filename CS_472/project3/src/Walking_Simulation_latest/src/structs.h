/*******************************************************************************

   STRUCTS.H

   Author: Peter Loan

   Date of creation: 01-JUL-91

   Copyright (c) 1996-2000 MusculoGraphics, Inc.
   All rights reserved.

   Description: This file contains definitions of all structures that are
      used by the Dynamics Pipeline.

*******************************************************************************/

#ifndef STRUCTS_H
#define STRUCTS_H

STRUCT
{
   int segment;                      /* body segment this point is fixed to */
   double coord[3];                  /* xyz coordinates of the point */
   double param[7];                  /* shoe model parameters */
   double grf[3];                    /* xyz components of the ground reaction force */
   double deformation;    	     /* element deformation */
   double force;    		     /* element force */
   double power;  //added 2-26-08
} ShoeStruct;   


STRUCT {
   double xyz[3];                    /* coordinates of a point in 3space */
} Coord3D;                           /* 3D point for passing to v3d() routine */


STRUCT {
   double a, b, c, d;
} PlaneStruct;


STRUCT
{
   int numpoints;                    /* number of points in the function */
   int coefficient_array_size;       /* current size of x,y,b,c,d arrays */
   int usernum;                      /* user-supplied number for this function */
   double* x;                        /* list of x coordinates */
   double* y;                        /* list of y coordinates */
   double* b;                        /* list of b coefficients for spline fit */
   double* c;                        /* list of c coefficients for spline fit */
   double* d;                        /* list of d coefficients for spline fit */
   SBoolean defined;                 /* is function defined in joint file? */
} SplineFunction;                    /* description of a cubic-spline function*/


STRUCT
{
   int genc;                         /* gencoord number */
   double start;                     /* starting value of range-of-motion */
   double end;                       /* ending value of range-of-motion */
} PointRange;                        /* muscle point range (wrapping pts) */


STRUCT
{
   int segment;                      /* body segment this point is fixed to */
   int state;                        /* state of point (on,off) */
   int refpt;                        /* corresponding point in saved muscle */
   SBoolean selected;                /* whether or not point has been selected */
   double point[3];                  /* xyz coordinates of the point */
   int numranges;                    /* number of ranges where point is active */
   PointRange* ranges;               /* array of ranges */
   SBoolean is_auto_wrap_point;      /* was this point calc-ed by auto wrapper? */
   double wrap_distance;             /* stores distance of wrap over object surface */
   double* wrap_pts;                 /* points wrapping over surface */
   int num_wrap_pts;                 /* number of points wrapping over surface */
} MusclePoint;                       /* properties of a muscle point */


ENUM
{
   spline_fit,                       /* interpolate control points with spline */
   step_function                     /* treat points as corners of step functions */
} ExcitationFormat;                  /* formats for muscle excitation functions */


STRUCT {
   int start;                        /* input:  wrap starting muscle point index */
   int end;                          /* input:  wrap ending muscle point index */
   double* wrap_pts;                 /* output: array of wrapping path points */
   int num_wrap_pts;                 /* output: size of 'wrap_pts' array */
   double wrap_path_length;          /* output: distance along curved r1->r2 path */
   double r1[4];                     /* output: wrap tangent point nearest to p1 */
   double r2[4];                     /* output: wrap tangent point nearest to p2 */
} WrapParams;


ENUM {
   wrap_sphere,
   wrap_cylinder,
   wrap_ellipsoid,
   wrap_none
} WrapObjectType;


STRUCT {
   char* name;                       /* name of wrap object */
   WrapObjectType wrap_type;         /* type of wrap object */
   int wrap_algorithm;               /* default wrap algorithm for wrap object */
   int segment;                      /* body segment object is fixed to */
   double radius[3];                 /* wrap object radius */
   double height;                    /* wrap object height (cylinder only) */
   int wrap_axis;                    /* which axis to wrap over X=0, Y=1, Z=2 */
   int  wrap_sign;                   /* which side of wrap axis to use (0 for both) */
   double from_local_xform[4][4];    /* wrapobj-to-parent transform matrix */
   double to_local_xform[4][4];      /* parent-to-wrapobj transform matrix */
} WrapObject;


STRUCT {
   WrapObject* wrap_object;          /* muscle is auto-wrapped over this object */
   int wrap_algorithm;               /* wrap algorithm for this muscle */
   double c[3];                      /* previous c point */
   double r1[3];                     /* previous r1 (tangent) point */
   double r2[3];                     /* previous r2 (tangent) point */
} WrapStruct;


STRUCT {
   char* name;                       /* name of muscle */
   SBoolean display;                 /* whether or not to display this muscle */
   SBoolean selected;                /* whether or not this muscle is selected */
   SBoolean has_wrapping_points;     /* does this muscle have wrapping pts? */
   SBoolean has_force_points;        /* does this muscle have force-dependent pts? */
   int first_on_point;               /* number of the first active muscle pt */
   int* num_orig_points;             /* number of muscle points */
   int num_points;                   /* number of muscle points */
   MusclePoint* mp_orig;             /* list of muscle points from muscle file */
   MusclePoint* mp_wrap;             /* list of points created by auto-wrapper */
   MusclePoint** mp;                 /* list of muscle points after auto wrapping */
   int mp_array_size;                /* current size of muscle point array */
   int numgroups;                    /* number of groups to which musc belongs*/
   int* group;                       /* list of muscle group numbers */
   double* max_isometric_force;      /* maximum isometric force */
   double* pennation_angle;          /* pennation angle of muscle fibers */
   double* optimal_fiber_length;     /* muscle fiber length */
   double* resting_tendon_length;    /* resting length of tendon */
   double* min_thickness;            /* minimum thickness of muscle line */
   double* max_thickness;            /* maximum thickness of muscle line */
   double activation;                /* activation level of muscle */
   double initial_activation;        /* default/initial activation level of muscle */
   double muscle_tendon_length;      /* musculotendon length */
   double fiber_length;              /* muscle fiber length */
   double tendon_length;             /* tendon length */
   double muscle_tendon_vel;         /* musculotendon velocity */
   double fiber_velocity;            /* muscle velocity */
   double tendon_velocity;           /* tendon velocity */
   double force;                     /* force in musculotendon unit */
   double applied_power;             /* power this muscle applies to body segments */
   double energy;                    /* for backwards compatibility */
   double heat_energy;               /* heat energy generated by this muscle */
   double mechanical_energy;         /* energy this muscle gives to body segments */
   int nummomentarms;                /* number of moment arms (= # of gencoords) */
   double* momentarms;               /* list of moment arm values */
   SplineFunction* tendon_force_len_curve; /* tendon force-length curve */
   SplineFunction* active_force_len_curve; /* muscle active force-length curve */
   SplineFunction* passive_force_len_curve; /* muscle passive force-length curve */
   double* max_contraction_vel;      /* maximum contraction velocity */
   SplineFunction* force_vel_curve;  /* muscle force-velocity curve */
   int num_dynamic_params;           /* size of dynamic_params array */
   char** dynamic_param_names;       /* list of dynamic parameter names */
   double** dynamic_params;          /* array of dynamic (muscle model) parameters */
   double dynamic_activation;        /* dynamic value of muscle activation */
   ExcitationFormat* excitation_format; /* format for excitation function */
   int excitation_abscissa;          /* excit. is func of this gencoord (-1 = time) */
   double excitation_level;          /* current level of excitation */
   int excitation_index;             /* excitation data point used last time */
   SplineFunction* excitation;       /* excitation (activation) sequence */
   int* muscle_model_index;          /* index for deriv, init, & assign func arrays */
   SBoolean wrap_calced;             /* has wrapping been calculated/updated? */
   WrapStruct* muscwrap;             /* holds information about wrap object */
   double init_act;		     /* initial muscle activation at time t=0.0 */
   double fiber_active_force;        /* active force in muscle fiber */

   int exc_group;					 // This keeps track of the excitaiton group that an inidividual
									 // muscle pertains to.  Added by Jeff R (Oct 2008)
  /////////////////////////////////////UMBERGER ENERGETICS/////////////////////////////////

   double percent_ft; //added 6-22-07
   double percent_st; //added 6-22-07
   double mus_mass; //added 6-22-07
   double STsat;
   double TestVar;						  /*put in for testing various variables*/
   double lce_opt;                    /*contractile element optimal length*/
   double Ham;						  /*Heat of activation & maintenance*/
   double Hsl;						  /*Heat of shortening and lengthaning*/
   double Hmw;                        /*Heat of muscle work*/
   double total_energy;				  /*total metabolic muscle energy*/
   double Fiso;                       /*iso force from calc active force*/




} MuscleStruct;                      /* properties of a musculotendon unit */




STRUCT {
   double x1;
   double x2;
   double y1;
   double y2;
   double z1;
   double z2;
} BoundingCube;


STRUCT {
   int num_vertices;
   int *vertex_index;
   double normal[3];
   double d;
} PolygonStruct;


STRUCT {
   double coord[3];
   double normal[3];
   int polygon_count;
   int* polygons;
} VertexStruct;


STRUCT {
   char* name;
   int body_segment;
   BoundingCube bc;
   VertexStruct* vertex;
   int num_vertices;
   PolygonStruct* polygon;
   int num_polygons;
} PolyhedronStruct;


ENUM {
   unconstrained_q,
   constrained_q,
   prescribed_q
} QType;


STRUCT {
   char* name;
   QType type;
   int joint;
   int axis;
   double initial_value;
   double conversion;
   double range_start;
   double range_end;
   SplineFunction* restraint_func;
   SplineFunction* min_restraint_func;
   SplineFunction* max_restraint_func;
} QStruct;


STRUCT {
   char* name;
   double mass;
   double inertia[3][3];
   double mass_center[3];
   int num_objects;
   PolyhedronStruct** object;
} BodyStruct;


STRUCT
{
   int nq;
   int nu;
   int neq;
   int num_muscles;
   int num_body_segments;
   int num_gencoords;
   int num_closed_loops;
   int num_dynamic_params;           /* size of dynamic_param_names array */
   char** dynamic_param_names;       /* names of dynamic muscle parameters */
   QStruct* q;
   BodyStruct* body_segment;
   MuscleStruct* muscles;
   MuscleStruct default_muscle;
   int num_shoe_elements;	     /* number of shoe elements */
   ShoeStruct* shoe_element;
} SDModelStruct;	             /* Structure externed to GMC */


STRUCT {
   int segment;                      /* body segment to which force is applied */
   double* appl_point[3];            /* pnt of application of force (local coords) */
   double* force_vec[3];             /* force vector */
} ForceStruct;                       /* contains one external force */


STRUCT {
   int segment;                      /* body segment to which torque is applied */
   double* torque_vec[3];            /* torque vector (in coords of body segment) */
} TorqueStruct;                      /* contains one external torque */


STRUCT {
   int num_frames;                   /* number of [time] frames of data */
   int num_otherdata;                /* number of "other" data values per frame */
   int num_elements;                 /* number of data values per frame */
   int num_forces;                   /* number of external forces */
   int num_torques;                  /* number of external torques */
   int current_frame;                /* current data frame in simulation */
   char** elementnames;              /* names of the data elements */
   double** motiondata;              /* the actual motion data */
   double** q_data;                  /* pointers to the q (gencoord) data */
   double** u_data;                  /* pointers to the u (gencoord vels) data */
   double** udot_data;               /* pointers to the udot (gencoord accels) */
   ForceStruct** forces;             /* external forces (e.g. ground-reaction) */
   TorqueStruct** torques;           /* external torques (e.g. ground-reaction) */
} MotionData;                        /* holds motion data for inverse dynamics */

#endif /* STRUCTS_H */
