/*******************************************************************************

   FUNCTIONS.H

   Author: Peter Loan

   Date of creation: 01-JUL-91

   Copyright (c) 1996-2000 MusculoGraphics, Inc.
   All rights reserved.

   Description: This file contains function prototypes for all functions which
      are callable by the GMC or SD/FAST user.

*******************************************************************************/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/* functions added to default SIMM functions */

void calc_joint_torques(double t,double y[],double ydot[],double joint_torque[]);
void calc_muscle_torques(double t,double y[],double ydot[],int muscle,double muscle_torque[]);
void apply_one_muscle_force(MuscleStruct* ms);
void apply_one_unit_force(MuscleStruct* ms);
void apply_unit_forces(SDModelStruct* sdm);
void calc_joint_powers(double joint_torque[],double joint_power[]);
void segment_power(double segpow[],double inertia[],double mass[],
                   double vel[][2], double angvel[]);
void induced_accel(double indacc[]);
void apply_ground_contact_forces(SDModelStruct* sdm);
void apply_pandy_grfs(SDModelStruct* sdm);
void apply_passive_joint_torques(SDModelStruct* sdm, double q[], double u[]);
void calc_passive_joint_torques(SDModelStruct* sdm, double q[], double u[], double passive_trq[]);
void calc_visceral_mass_forces(SDModelStruct* sdm, double force[]);
void apply_visceral_mass_forces(SDModelStruct* sdm);
void calc_net_grfs(SDModelStruct* sdm, double y[],double dy[], double netgrf[], double cop[]);
void segment_me(double segme[], double inertia[], double mass[],
                double vel[][3], double angvel[], double pos[]);


int    acpp_main(int argc, char* argv[]);
void   apply_external_forces(MotionData* inv_data);
void   apply_joint_restraint_torques(SDModelStruct* sdm, double state[]);
void   apply_muscle_forces(SDModelStruct* sdm);
void   assign_muscle_states(SDModelStruct* sdm, double time, double state[]);
double calc_active_force(MuscleStruct* ms, double norm_fiber_length);
void   calc_collisions(void);
void   calc_derivatives(double time, double state[], double dstate[],
		      double param[], int* status);
double calc_distance_between_points(double p1[], int n1, double p2[], int n2);
double calc_excitation(MuscleStruct* ms, double time, double state[]);
double calc_fiber_force(MuscleStruct* ms, double activation,
			double norm_fiber_length, double norm_fiber_velocity);
double calc_fiber_velocity(MuscleStruct* ms, double activation,
			   double active_force, double velocity_dependent_force);
double calc_force_velocity(MuscleStruct* ms, double norm_fiber_velocity);
int calc_line_intersect_cylinder (
	double		p11[3],		/* input:  muscle point 1		*/
	double		p22[3],		/* input:  muscle point 2		*/
        double		p0[3],          /* input:  point on cylinder axis       */
	double		dn[3],          /* input:  direction vector of cyl axis */
	double		r,              /* input:  cylinder radius              */
	double*		rlen,		/* output: length of surface wrap	*/
	double		r1[],		/* output: wrap tangent point 1		*/
	double		r2[],		/* output: wrap tangent point 2		*/
	double**	wrap_pts,	/* output: intermediate surface wrap pts*/
	int*		num_wrap_pts,	/* output: number of intermediate pts	*/
	int		wrap_axis,	/* input:  constraint axis		*/
	int		wrap_sign,	/* input:  constraint direction		*/
	WrapStruct*     mwrap,		/* in/out: muscle wrap structure	*/
	int*		p_flag,		/* output: did wrapping occur?		*/
	WrapObject*	wo);
int calc_line_intersect_ellipsoid (
	double		p1[3],		/* input:  muscle point 1		*/
	double		p2[3],		/* input:  muscle point 2		*/
	double		m[3],		/* input:  ellipsoid origin (0,0,0)	*/
	double		a[3],		/* input:  ellipsoid xyz radii		*/
	double*		rlen,		/* output: length of surface wrap	*/
	double		r1[],		/* output: wrap tangent point 1		*/
	double		r2[],		/* output: wrap tangent point 2		*/
	double**	wrap_pts,	/* output: intermediate surface wrap pts*/
	int*		num_wrap_pts,	/* output: number of intermediate pts	*/
	int		wrap_axis,	/* input:  constraint axis		*/
	int		wrap_sign,	/* input:  constraint direction		*/
	WrapStruct*     mwrap,		/* in/out: muscle wrap structure	*/
	int*		p_flag,		/* output: did wrapping occur?		*/
	WrapObject*	wo);
int calc_line_intersect_sphere(double p1[], double p2[], double m[],
			       double r, double *rlen, double r1[],
			       double r2[],
			       double** wrap_pts, int *num_wrap_pts,
			       int wrap_axis, int wrap_sign, WrapStruct* mwrap,
			       int *p_flag,
			       WrapObject* wo);
void   calc_muscle_power(MuscleStruct* ms);
void   calc_muscle_tendon_force(MuscleStruct* ms, double activation);
double calc_muscle_tendon_length(MuscleStruct* ms);
double calc_muscle_tendon_velocity(MuscleStruct* ms);
double calc_nonzero_passive_force(MuscleStruct* ms, double norm_fiber_length,
				  double norm_fiber_velocity);
double calc_passive_force(MuscleStruct* ms, double norm_fiber_length,
			  double norm_fiber_velocity);
double calc_pennation(double fiber_length, double optimal_fiber_length,
		      double initial_pennation_angle);
double calc_system_energy(void);
double calc_tendon_force(MuscleStruct* ms, double norm_tendon_length);
void   check_object_info(void);
void   check_wrapping_points(MuscleStruct* ms, double state[]);
char*  clean_string(char buf[]);
void   convert_point(double pt[], int frame1, int frame2);
void   convert_string(char str[]);
int    count_muscle_states(SDModelStruct* sdm);
int    enter_gencoord(SDModelStruct* sdm, char gencoord_name[]);
int    enter_segment(SDModelStruct* sdm, char segment_name[]);
void   error(ErrorAction action, char str_buffer[]);
double get_muscle_param_value(MuscleStruct* ms, char param_name[]);
ReturnCode get_string_pair(char str_buffer[], char str1[], char str2[]);
char*  get_temp_file_name(const char* baseFileName);
WrapObject* get_wrap_object(char name[]);
ReturnCode get_xypair_from_string(char str_buffer[], double* x, double* y);
void   init_joint_functions(void);
void   init_muscle_states(SDModelStruct* sdm, double state[]);
void   init_object_info(void);
void   init_q_restraint_functions(void);
void   init_qs(void);
void   init_segments(void);
void   invert_3x3matrix(double matrix[][3], double inverse[][3]);
void   invert_4x4matrix(double matrix[][4], double inverse[][4]);
void   invert_4x4transform(double matrix[][4], double inverse[][4]);
void   invert_matrix(double* matrix[], double* inverse[], int size);
ReturnCode load_kinetics_data(SDModelStruct* sdm, MotionData* data, char filename[],
			      SBoolean verify_data);
ReturnCode malloc_function(SplineFunction* func, int numpoints);
void   message(char message_str[]);
ReturnCode mstrcpy(char* dest_str[], char original_str[]);
char*  parse_string(char str_buffer[], VariableType var_type,
		    void* dest_var);
FILE*  preprocess_file(char infile[], char outfile[]);
void   prescribe_inverse_dynamics(SDModelStruct* sdm, MotionData* inv_data, int frame);
void   prescribe_fixed_joints(SDModelStruct* sdm);
void   read_collision_pair(FILE** fp);
ReturnCode read_double_array(FILE **fp, char ending[], char name[],
			     SplineFunction* func);
int    read_line(FILE** fp, char str_buffer[]);
MusclePoint* read_muscle_attachment_points(FILE** fp, SDModelStruct* sdm,
					   int* numpoints, int* mp_array_size,
					   SBoolean* has_wrapping_points);
ReturnCode read_muscle_file(SDModelStruct* sdm, char filename[]);
ReturnCode read_muscle_groups(FILE** fp);
int    read_nonempty_line(FILE** fp, char str_buffer[]);
void   read_object(FILE** fp);
void   read_object_group(FILE** fp);
ReturnCode read_parameters_file(char params_file[], SDModelStruct* sdm,
				char** muscle_file, char** kinetics_file,
				char** output_motion_file, char** output_kinetics_file);
int    read_string(FILE** fp, char str_buffer[]);
ReturnCode realloc_function(SplineFunction* func, int size);
int    sduforce(double t, double q[], double u[]);
int    sdumotion(double t, double q[], double u[]);
void   set_data_frame(SDModelStruct* sdm, MotionData* inv_data, double q[],
		      double u[], int frame);
void   set_prescribed_motion(SDModelStruct* sdm, QType type, int value);
FILE*  simm_fopen (const char* name, const char* mode);
void*  simm_malloc(unsigned mem_size);
void*  simm_calloc(unsigned num_elements, unsigned elem_size);
void*  simm_realloc(void* ptr, unsigned mem_size, ReturnCode* rc);
ReturnCode verify_assembly(SDModelStruct* sdm, double state[]);
void write_data(SDModelStruct* sdm, MotionData* inv_data, FILE** fp);

/* Function Prototypes for the user-created muscle-state functions */

int muscle_init_func1(MuscleStruct *ms, double state[]);
int muscle_init_func2(MuscleStruct *ms, double state[]);
int muscle_init_func3(MuscleStruct *ms, double state[]);
int muscle_init_func4(MuscleStruct *ms, double state[]);
int muscle_init_func5(MuscleStruct *ms, double state[]);
int muscle_init_func6(MuscleStruct *ms, double state[]);
int muscle_init_func7(MuscleStruct *ms, double state[]);
int muscle_init_func8(MuscleStruct *ms, double state[]);
int muscle_init_func9(MuscleStruct *ms, double state[]);
int muscle_init_func10(MuscleStruct *ms, double state[]);

int muscle_deriv_func1(double time, MuscleStruct *ms, double state[],
		       double dstate[], double *muscle_force);
int muscle_deriv_func2(double time, MuscleStruct *ms, double state[],
		       double dstate[], double *muscle_force);
int muscle_deriv_func3(double time, MuscleStruct *ms, double state[],
		       double dstate[], double *muscle_force);
int muscle_deriv_func4(double time, MuscleStruct *ms, double state[],
		       double dstate[], double *muscle_force);
int muscle_deriv_func5(double time, MuscleStruct *ms, double state[],
		       double dstate[], double *muscle_force);
int muscle_deriv_func6(double time, MuscleStruct *ms, double state[],
		       double dstate[], double *muscle_force);
int muscle_deriv_func7(double time, MuscleStruct *ms, double state[],
		       double dstate[], double *muscle_force);
int muscle_deriv_func8(double time, MuscleStruct *ms, double state[],
		       double dstate[], double *muscle_force);
int muscle_deriv_func9(double time, MuscleStruct *ms, double state[],
		       double dstate[], double *muscle_force);
int muscle_deriv_func10(double time, MuscleStruct *ms, double state[],
			double dstate[], double *muscle_force);

int muscle_assign_func1(double time, MuscleStruct *ms, double state[]);
int muscle_assign_func2(double time, MuscleStruct *ms, double state[]);
int muscle_assign_func3(double time, MuscleStruct *ms, double state[]);
int muscle_assign_func4(double time, MuscleStruct *ms, double state[]);
int muscle_assign_func5(double time, MuscleStruct *ms, double state[]);
int muscle_assign_func6(double time, MuscleStruct *ms, double state[]);
int muscle_assign_func7(double time, MuscleStruct *ms, double state[]);
int muscle_assign_func8(double time, MuscleStruct *ms, double state[]);
int muscle_assign_func9(double time, MuscleStruct *ms, double state[]);
int muscle_assign_func10(double time, MuscleStruct *ms, double state[]);

#endif /* FUNCTIONS_H */
