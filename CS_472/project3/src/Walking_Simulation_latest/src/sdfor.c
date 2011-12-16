/*******************************************************************************

   C:\Users\Ko\Artificial_Human\Gait\Optimization\COMPLETE_SA\SA_WALK100Toe\sdfor.c

   Created by SIMM, 2.0.2 (from joints file C:\Users\Ko\Artificial_Human\Gait\Optimization\COMPLETE_SA\SA_WALK100Toe\joints.jnt)

   Time of creation: 02/05/2004 08:27:34 am

   Description: This file contains the routines needed to perform a forward
      dynamics simulation of an SD/FAST model. The specific routines that it
      contains depend on the SIMM model from which this code was generated.

*******************************************************************************/

#include "universal.h"

/*************** DEFINES (for this file only) *********************************/
#define BAUMGARTE_STAB 20.0

/* Joints */
#define ground_pelvis 0
#define hip_r 1
#define knee_r 2
#define tib_pat_r 3
#define ankle_r 4
#define subtalar_r 5
#define midfoot_r 6
#define mtp_r 7
#define hip_l 8
#define knee_l 9
#define tib_pat_l 10
#define ankle_l 11
#define subtalar_l 12
#define midfoot_l 13
#define mtp_l 14

/* Q's */
#define               leg_tx  0   /* = sdindx(ground_pelvis,0) */
#define               leg_ty  1   /* = sdindx(ground_pelvis,1) */
#define      pelvis_rotation  2   /* = sdindx(ground_pelvis,2) */
#define          hip_angle_r  3   /* = sdindx(hip_r,0) */
#define            knee_r_tx  4   /* = sdindx(knee_r,0) */
#define            knee_r_ty  5   /* = sdindx(knee_r,1) */
#define         knee_angle_r  6   /* = sdindx(knee_r,2) */
#define         tib_pat_r_tx  7   /* = sdindx(tib_pat_r,0) */
#define         tib_pat_r_ty  8   /* = sdindx(tib_pat_r,1) */
#define         tib_pat_r_r3  9   /* = sdindx(tib_pat_r,2) */
#define        ankle_angle_r 10   /* = sdindx(ankle_r,0) */
#define         prescribed11 11   /* = sdindx(subtalar_r,0) */
#define      midfoot_angle_r 12   /* = sdindx(midfoot_r,0) */
#define          mtp_angle_r 13   /* = sdindx(mtp_r,0) */
#define          hip_angle_l 14   /* = sdindx(hip_l,0) */
#define            knee_l_tx 15   /* = sdindx(knee_l,0) */
#define            knee_l_ty 16   /* = sdindx(knee_l,1) */
#define         knee_angle_l 17   /* = sdindx(knee_l,2) */
#define         tib_pat_l_tx 18   /* = sdindx(tib_pat_l,0) */
#define         tib_pat_l_ty 19   /* = sdindx(tib_pat_l,1) */
#define         tib_pat_l_r3 20   /* = sdindx(tib_pat_l,2) */
#define        ankle_angle_l 21   /* = sdindx(ankle_l,0) */
#define         prescribed22 22   /* = sdindx(subtalar_l,0) */
#define      midfoot_angle_l 23   /* = sdindx(midfoot_l,0) */
#define          mtp_angle_l 24   /* = sdindx(mtp_l,0) */

/* Constrained Q's */
#define knee_r_tx_con 0
#define knee_r_ty_con 1
#define tib_pat_r_tx_con 2
#define tib_pat_r_ty_con 3
#define tib_pat_r_r3_con 4
#define knee_l_tx_con 5
#define knee_l_ty_con 6
#define tib_pat_l_tx_con 7
#define tib_pat_l_ty_con 8
#define tib_pat_l_r3_con 9

/* Body Segments */
#define ground -1
#define pelvis 0
#define femur_r 1
#define tibia_r 2
#define patella_r 3
#define talus_r 4
#define calcn_r 5
#define midfoot_r 6
#define toes_r 7
#define femur_l 8
#define tibia_l 9
#define patella_l 10
#define talus_l 11
#define calcn_l 12
#define midfoot_l 13
#define toes_l 14


/*************** STATIC GLOBAL VARIABLES (for this file only) *****************/

/* The following spline-function data points are copied directly from the
 * SIMM joints file.
 */

static double knee_r_tx_data[][2] =
{
{-120.0000000,-0.0032000},
{-100.0000000, 0.0017900},
{-80.0000000, 0.0041100},
{-60.0000000, 0.0041000},
{-40.0000000, 0.0021200},
{-20.0000000,-0.0010000},
{-10.0000000,-0.0031000},
{ 0.0000000,-0.0052500},
{ 5.0000000,-0.0052500}
};

static double knee_r_ty_data[][2] =
{
{-120.0000000,-0.4226000},
{-70.0000000,-0.4082000},
{-30.0000000,-0.3990000},
{-20.0000000,-0.3976000},
{-10.0000000,-0.3966000},
{ 0.0000000,-0.3960000},
{ 5.0000000,-0.3960000}
};

static double tib_pat_r_r3_data[][2] =
{
{-120.0000000,17.6500000},
{-114.5900000,17.6500000},
{-83.5100000,17.5500000},
{-30.1600000,15.4800000},
{ 1.6000000,-2.1200000},
{10.0000000,-16.0400000}
};

static double tib_pat_r_tx_data[][2] =
{
{-120.0000000, 0.0173000},
{-80.0000000, 0.0324000},
{-60.0000000, 0.0381000},
{-40.0000000, 0.0430000},
{-20.0000000, 0.0469000},
{-10.0000000, 0.0484000},
{ 0.0000000, 0.0496000},
{ 5.0000000, 0.0496000}
};

static double tib_pat_r_ty_data[][2] =
{
{-120.0000000,-0.0219000},
{-90.0000000,-0.0202000},
{-80.0000000,-0.0200000},
{-60.0000000,-0.0204000},
{-40.0000000,-0.0211000},
{-20.0000000,-0.0219000},
{-10.0000000,-0.0223000},
{ 0.3000000,-0.0227000},
{ 5.0000000,-0.0227000}
};

static double knee_l_tx_data[][2] =
{
{-120.0000000,-0.0032000},
{-100.0000000, 0.0017900},
{-80.0000000, 0.0041100},
{-60.0000000, 0.0041000},
{-40.0000000, 0.0021200},
{-20.0000000,-0.0010000},
{-10.0000000,-0.0031000},
{ 0.0000000,-0.0052500},
{ 5.0000000,-0.0052500}
};

static double knee_l_ty_data[][2] =
{
{-120.0000000,-0.4226000},
{-70.0000000,-0.4082000},
{-30.0000000,-0.3990000},
{-20.0000000,-0.3976000},
{-10.0000000,-0.3966000},
{ 0.0000000,-0.3960000},
{ 5.0000000,-0.3960000}
};

static double tib_pat_l_r3_data[][2] =
{
{-120.0000000,17.6500000},
{-114.5900000,17.6500000},
{-83.5100000,17.5500000},
{-30.1600000,15.4800000},
{ 1.6000000,-2.1200000},
{10.0000000,-16.0400000}
};

static double tib_pat_l_tx_data[][2] =
{
{-120.0000000, 0.0173000},
{-80.0000000, 0.0324000},
{-60.0000000, 0.0381000},
{-40.0000000, 0.0430000},
{-20.0000000, 0.0469000},
{-10.0000000, 0.0484000},
{ 0.0000000, 0.0496000},
{ 5.0000000, 0.0496000}
};

static double tib_pat_l_ty_data[][2] =
{
{-120.0000000,-0.0219000},
{-90.0000000,-0.0202000},
{-80.0000000,-0.0200000},
{-60.0000000,-0.0204000},
{-40.0000000,-0.0211000},
{-20.0000000,-0.0219000},
{-10.0000000,-0.0223000},
{ 0.3000000,-0.0227000},
{ 5.0000000,-0.0227000}
};

static SplineFunction knee_r_tx_func;
static SplineFunction knee_r_ty_func;
static SplineFunction tib_pat_r_r3_func;
static SplineFunction tib_pat_r_tx_func;
static SplineFunction tib_pat_r_ty_func;
static SplineFunction knee_l_tx_func;
static SplineFunction knee_l_ty_func;
static SplineFunction tib_pat_l_r3_func;
static SplineFunction tib_pat_l_tx_func;
static SplineFunction tib_pat_l_ty_func;

static double q_restraint_func1_data[][2] =
{
{0.000000,0.000000},
{5.000000,300.000000},
{10.000000,600.000000},
{20.000000,1000.000000},
{100.000000,3000.000000}
};

static SplineFunction q_restraint_func[1];



/**************** GLOBAL VARIABLES (used in only a few files) *****************/
SDModelStruct sdm;


/*************** EXTERNED VARIABLES (declared in another file) ****************/



/*************** PROTOTYPES for STATIC FUNCTIONS (for this file only) *********/


/* INIT_QS: this routine initializes the array of structures
 * that hold information about the Qs (gencoords).
 */

void init_qs(void)
{

   int i;

   sdm.nq = 25;
   sdm.nu = 25;

   sdm.q = (QStruct*)simm_malloc(sdm.nq*sizeof(QStruct));
   sdm.num_closed_loops = 0;

   mstrcpy(&sdm.q[leg_tx].name,"leg_tx");
   sdm.q[leg_tx].type = unconstrained_q;
   sdm.q[leg_tx].joint = ground_pelvis;
   sdm.q[leg_tx].axis = 0;
   sdm.q[leg_tx].conversion = 1.00000;
   sdm.q[leg_tx].initial_value = 0.00000;
   sdm.q[leg_tx].range_start = -2.00000;
   sdm.q[leg_tx].range_end = 4.00000;
   sdm.q[leg_tx].restraint_func = NULL;
   sdm.q[leg_tx].min_restraint_func = NULL;
   sdm.q[leg_tx].max_restraint_func = NULL;

   mstrcpy(&sdm.q[leg_ty].name,"leg_ty");
   sdm.q[leg_ty].type = unconstrained_q;
   sdm.q[leg_ty].joint = ground_pelvis;
   sdm.q[leg_ty].axis = 1;
   sdm.q[leg_ty].conversion = 1.00000;
   sdm.q[leg_ty].initial_value = 0.91356;
   sdm.q[leg_ty].range_start = 0.00000;
   sdm.q[leg_ty].range_end = 2.00000;
   sdm.q[leg_ty].restraint_func = NULL;
   sdm.q[leg_ty].min_restraint_func = NULL;
   sdm.q[leg_ty].max_restraint_func = NULL;

   mstrcpy(&sdm.q[pelvis_rotation].name,"pelvis_rotation");
   sdm.q[pelvis_rotation].type = unconstrained_q;
   sdm.q[pelvis_rotation].joint = ground_pelvis;
   sdm.q[pelvis_rotation].axis = 2;
   sdm.q[pelvis_rotation].conversion = 57.29578;
   sdm.q[pelvis_rotation].initial_value = -0.72068;
   sdm.q[pelvis_rotation].range_start = -90.00000;
   sdm.q[pelvis_rotation].range_end = 90.00000;
   sdm.q[pelvis_rotation].restraint_func = NULL;
   sdm.q[pelvis_rotation].min_restraint_func = &q_restraint_func[0];
   sdm.q[pelvis_rotation].max_restraint_func = &q_restraint_func[0];

   mstrcpy(&sdm.q[hip_angle_r].name,"hip_angle_r");
   sdm.q[hip_angle_r].type = unconstrained_q;
   sdm.q[hip_angle_r].joint = hip_r;
   sdm.q[hip_angle_r].axis = 0;
   sdm.q[hip_angle_r].conversion = 57.29578;
   sdm.q[hip_angle_r].initial_value = 30.67604;
   sdm.q[hip_angle_r].range_start = -40.00000;
   sdm.q[hip_angle_r].range_end = 120.00000;
   sdm.q[hip_angle_r].restraint_func = NULL;
   sdm.q[hip_angle_r].min_restraint_func = &q_restraint_func[0];
   sdm.q[hip_angle_r].max_restraint_func = &q_restraint_func[0];

   mstrcpy(&sdm.q[knee_r_tx].name,"knee_r_tx");
   sdm.q[knee_r_tx].type = constrained_q;
   sdm.q[knee_r_tx].joint = knee_r;
   sdm.q[knee_r_tx].axis = 0;
   sdm.q[knee_r_tx].conversion = 1.00000;
   sdm.q[knee_r_tx].initial_value = -0.00453;
   sdm.q[knee_r_tx].range_start = -99999.9;
   sdm.q[knee_r_tx].range_end = 99999.9;
   sdm.q[knee_r_tx].restraint_func = NULL;
   sdm.q[knee_r_tx].min_restraint_func = NULL;
   sdm.q[knee_r_tx].max_restraint_func = NULL;

   mstrcpy(&sdm.q[knee_r_ty].name,"knee_r_ty");
   sdm.q[knee_r_ty].type = constrained_q;
   sdm.q[knee_r_ty].joint = knee_r;
   sdm.q[knee_r_ty].axis = 1;
   sdm.q[knee_r_ty].conversion = 1.00000;
   sdm.q[knee_r_ty].initial_value = -0.39618;
   sdm.q[knee_r_ty].range_start = -99999.9;
   sdm.q[knee_r_ty].range_end = 99999.9;
   sdm.q[knee_r_ty].restraint_func = NULL;
   sdm.q[knee_r_ty].min_restraint_func = NULL;
   sdm.q[knee_r_ty].max_restraint_func = NULL;

   mstrcpy(&sdm.q[knee_angle_r].name,"knee_angle_r");
   sdm.q[knee_angle_r].type = unconstrained_q;
   sdm.q[knee_angle_r].joint = knee_r;
   sdm.q[knee_angle_r].axis = 2;
   sdm.q[knee_angle_r].conversion = 57.29578;
   sdm.q[knee_angle_r].initial_value = -4.21151;
   sdm.q[knee_angle_r].range_start = -120.00000;
   sdm.q[knee_angle_r].range_end = 5.00000;
   sdm.q[knee_angle_r].restraint_func = NULL;
   sdm.q[knee_angle_r].min_restraint_func = &q_restraint_func[0];
   sdm.q[knee_angle_r].max_restraint_func = &q_restraint_func[0];

   mstrcpy(&sdm.q[tib_pat_r_tx].name,"tib_pat_r_tx");
   sdm.q[tib_pat_r_tx].type = constrained_q;
   sdm.q[tib_pat_r_tx].joint = tib_pat_r;
   sdm.q[tib_pat_r_tx].axis = 0;
   sdm.q[tib_pat_r_tx].conversion = 1.00000;
   sdm.q[tib_pat_r_tx].initial_value = 0.04921;
   sdm.q[tib_pat_r_tx].range_start = -99999.9;
   sdm.q[tib_pat_r_tx].range_end = 99999.9;
   sdm.q[tib_pat_r_tx].restraint_func = NULL;
   sdm.q[tib_pat_r_tx].min_restraint_func = NULL;
   sdm.q[tib_pat_r_tx].max_restraint_func = NULL;

   mstrcpy(&sdm.q[tib_pat_r_ty].name,"tib_pat_r_ty");
   sdm.q[tib_pat_r_ty].type = constrained_q;
   sdm.q[tib_pat_r_ty].joint = tib_pat_r;
   sdm.q[tib_pat_r_ty].axis = 1;
   sdm.q[tib_pat_r_ty].conversion = 1.00000;
   sdm.q[tib_pat_r_ty].initial_value = -0.02256;
   sdm.q[tib_pat_r_ty].range_start = -99999.9;
   sdm.q[tib_pat_r_ty].range_end = 99999.9;
   sdm.q[tib_pat_r_ty].restraint_func = NULL;
   sdm.q[tib_pat_r_ty].min_restraint_func = NULL;
   sdm.q[tib_pat_r_ty].max_restraint_func = NULL;

   mstrcpy(&sdm.q[tib_pat_r_r3].name,"tib_pat_r_r3");
   sdm.q[tib_pat_r_r3].type = constrained_q;
   sdm.q[tib_pat_r_r3].joint = tib_pat_r;
   sdm.q[tib_pat_r_r3].axis = 2;
   sdm.q[tib_pat_r_r3].conversion = 57.29578;
   sdm.q[tib_pat_r_r3].initial_value = 4.52253;
   sdm.q[tib_pat_r_r3].range_start = -99999.9;
   sdm.q[tib_pat_r_r3].range_end = 99999.9;
   sdm.q[tib_pat_r_r3].restraint_func = NULL;
   sdm.q[tib_pat_r_r3].min_restraint_func = NULL;
   sdm.q[tib_pat_r_r3].max_restraint_func = NULL;

   mstrcpy(&sdm.q[ankle_angle_r].name,"ankle_angle_r");
   sdm.q[ankle_angle_r].type = unconstrained_q;
   sdm.q[ankle_angle_r].joint = ankle_r;
   sdm.q[ankle_angle_r].axis = 0;
   sdm.q[ankle_angle_r].conversion = 57.29578;
   sdm.q[ankle_angle_r].initial_value = 1.73984;
   sdm.q[ankle_angle_r].range_start = -40.00000;
   sdm.q[ankle_angle_r].range_end = 40.00000;
   sdm.q[ankle_angle_r].restraint_func = NULL;
   sdm.q[ankle_angle_r].min_restraint_func = &q_restraint_func[0];
   sdm.q[ankle_angle_r].max_restraint_func = &q_restraint_func[0];

   mstrcpy(&sdm.q[prescribed11].name,"prescribed11");
   sdm.q[prescribed11].type = prescribed_q;
   sdm.q[prescribed11].joint = subtalar_r;
   sdm.q[prescribed11].axis = 0;
   sdm.q[prescribed11].conversion = 57.29578;
   sdm.q[prescribed11].initial_value = 0.00000;
   sdm.q[prescribed11].range_start = -99999.9;
   sdm.q[prescribed11].range_end = 99999.9;
   sdm.q[prescribed11].restraint_func = NULL;
   sdm.q[prescribed11].min_restraint_func = NULL;
   sdm.q[prescribed11].max_restraint_func = NULL;

   mstrcpy(&sdm.q[midfoot_angle_r].name,"midfoot_angle_r");
   sdm.q[midfoot_angle_r].type = unconstrained_q;
   sdm.q[midfoot_angle_r].joint = midfoot_r;
   sdm.q[midfoot_angle_r].axis = 0;
   sdm.q[midfoot_angle_r].conversion = 57.29578;
   sdm.q[midfoot_angle_r].initial_value = 0.00000;
   sdm.q[midfoot_angle_r].range_start = -60.00000;
   sdm.q[midfoot_angle_r].range_end = 60.00000;
   sdm.q[midfoot_angle_r].restraint_func = NULL;
   sdm.q[midfoot_angle_r].min_restraint_func = &q_restraint_func[0];
   sdm.q[midfoot_angle_r].max_restraint_func = &q_restraint_func[0];

   mstrcpy(&sdm.q[mtp_angle_r].name,"mtp_angle_r");
   sdm.q[mtp_angle_r].type = unconstrained_q;
   sdm.q[mtp_angle_r].joint = mtp_r;
   sdm.q[mtp_angle_r].axis = 0;
   sdm.q[mtp_angle_r].conversion = 57.29578;
   sdm.q[mtp_angle_r].initial_value = 0.00000;
   sdm.q[mtp_angle_r].range_start = -60.00000;
   sdm.q[mtp_angle_r].range_end = 60.00000;
   sdm.q[mtp_angle_r].restraint_func = NULL;
   sdm.q[mtp_angle_r].min_restraint_func = &q_restraint_func[0];
   sdm.q[mtp_angle_r].max_restraint_func = &q_restraint_func[0];

   mstrcpy(&sdm.q[hip_angle_l].name,"hip_angle_l");
   sdm.q[hip_angle_l].type = unconstrained_q;
   sdm.q[hip_angle_l].joint = hip_l;
   sdm.q[hip_angle_l].axis = 0;
   sdm.q[hip_angle_l].conversion = 57.29578;
   sdm.q[hip_angle_l].initial_value = -20.39872;
   sdm.q[hip_angle_l].range_start = -40.00000;
   sdm.q[hip_angle_l].range_end = 120.00000;
   sdm.q[hip_angle_l].restraint_func = NULL;
   sdm.q[hip_angle_l].min_restraint_func = &q_restraint_func[0];
   sdm.q[hip_angle_l].max_restraint_func = &q_restraint_func[0];

   mstrcpy(&sdm.q[knee_l_tx].name,"knee_l_tx");
   sdm.q[knee_l_tx].type = constrained_q;
   sdm.q[knee_l_tx].joint = knee_l;
   sdm.q[knee_l_tx].axis = 0;
   sdm.q[knee_l_tx].conversion = 1.00000;
   sdm.q[knee_l_tx].initial_value = -0.00260;
   sdm.q[knee_l_tx].range_start = -99999.9;
   sdm.q[knee_l_tx].range_end = 99999.9;
   sdm.q[knee_l_tx].restraint_func = NULL;
   sdm.q[knee_l_tx].min_restraint_func = NULL;
   sdm.q[knee_l_tx].max_restraint_func = NULL;

   mstrcpy(&sdm.q[knee_l_ty].name,"knee_l_ty");
   sdm.q[knee_l_ty].type = constrained_q;
   sdm.q[knee_l_ty].joint = knee_l;
   sdm.q[knee_l_ty].axis = 1;
   sdm.q[knee_l_ty].conversion = 1.00000;
   sdm.q[knee_l_ty].initial_value = -0.39678;
   sdm.q[knee_l_ty].range_start = -99999.9;
   sdm.q[knee_l_ty].range_end = 99999.9;
   sdm.q[knee_l_ty].restraint_func = NULL;
   sdm.q[knee_l_ty].min_restraint_func = NULL;
   sdm.q[knee_l_ty].max_restraint_func = NULL;

   mstrcpy(&sdm.q[knee_angle_l].name,"knee_angle_l");
   sdm.q[knee_angle_l].type = unconstrained_q;
   sdm.q[knee_angle_l].joint = knee_l;
   sdm.q[knee_angle_l].axis = 2;
   sdm.q[knee_angle_l].conversion = 57.29578;
   sdm.q[knee_angle_l].initial_value = -12.10386;
   sdm.q[knee_angle_l].range_start = -120.00000;
   sdm.q[knee_angle_l].range_end = 5.00000;
   sdm.q[knee_angle_l].restraint_func = NULL;
   sdm.q[knee_angle_l].min_restraint_func = &q_restraint_func[0];
   sdm.q[knee_angle_l].max_restraint_func = &q_restraint_func[0];

   mstrcpy(&sdm.q[tib_pat_l_tx].name,"tib_pat_l_tx");
   sdm.q[tib_pat_l_tx].type = constrained_q;
   sdm.q[tib_pat_l_tx].joint = tib_pat_l;
   sdm.q[tib_pat_l_tx].axis = 0;
   sdm.q[tib_pat_l_tx].conversion = 1.00000;
   sdm.q[tib_pat_l_tx].initial_value = 0.04809;
   sdm.q[tib_pat_l_tx].range_start = -99999.9;
   sdm.q[tib_pat_l_tx].range_end = 99999.9;
   sdm.q[tib_pat_l_tx].restraint_func = NULL;
   sdm.q[tib_pat_l_tx].min_restraint_func = NULL;
   sdm.q[tib_pat_l_tx].max_restraint_func = NULL;

   mstrcpy(&sdm.q[tib_pat_l_ty].name,"tib_pat_l_ty");
   sdm.q[tib_pat_l_ty].type = constrained_q;
   sdm.q[tib_pat_l_ty].joint = tib_pat_l;
   sdm.q[tib_pat_l_ty].axis = 1;
   sdm.q[tib_pat_l_ty].conversion = 1.00000;
   sdm.q[tib_pat_l_ty].initial_value = -0.02221;
   sdm.q[tib_pat_l_ty].range_start = -99999.9;
   sdm.q[tib_pat_l_ty].range_end = 99999.9;
   sdm.q[tib_pat_l_ty].restraint_func = NULL;
   sdm.q[tib_pat_l_ty].min_restraint_func = NULL;
   sdm.q[tib_pat_l_ty].max_restraint_func = NULL;

   mstrcpy(&sdm.q[tib_pat_l_r3].name,"tib_pat_l_r3");
   sdm.q[tib_pat_l_r3].type = constrained_q;
   sdm.q[tib_pat_l_r3].joint = tib_pat_l;
   sdm.q[tib_pat_l_r3].axis = 2;
   sdm.q[tib_pat_l_r3].conversion = 57.29578;
   sdm.q[tib_pat_l_r3].initial_value = 10.40390;
   sdm.q[tib_pat_l_r3].range_start = -99999.9;
   sdm.q[tib_pat_l_r3].range_end = 99999.9;
   sdm.q[tib_pat_l_r3].restraint_func = NULL;
   sdm.q[tib_pat_l_r3].min_restraint_func = NULL;
   sdm.q[tib_pat_l_r3].max_restraint_func = NULL;

   mstrcpy(&sdm.q[ankle_angle_l].name,"ankle_angle_l");
   sdm.q[ankle_angle_l].type = unconstrained_q;
   sdm.q[ankle_angle_l].joint = ankle_l;
   sdm.q[ankle_angle_l].axis = 0;
   sdm.q[ankle_angle_l].conversion = 57.29578;
   sdm.q[ankle_angle_l].initial_value = -0.90216;
   sdm.q[ankle_angle_l].range_start = -40.00000;
   sdm.q[ankle_angle_l].range_end = 40.00000;
   sdm.q[ankle_angle_l].restraint_func = NULL;
   sdm.q[ankle_angle_l].min_restraint_func = &q_restraint_func[0];
   sdm.q[ankle_angle_l].max_restraint_func = &q_restraint_func[0];

   mstrcpy(&sdm.q[prescribed22].name,"prescribed22");
   sdm.q[prescribed22].type = prescribed_q;
   sdm.q[prescribed22].joint = subtalar_l;
   sdm.q[prescribed22].axis = 0;
   sdm.q[prescribed22].conversion = 57.29578;
   sdm.q[prescribed22].initial_value = 0.00000;
   sdm.q[prescribed22].range_start = -99999.9;
   sdm.q[prescribed22].range_end = 99999.9;
   sdm.q[prescribed22].restraint_func = NULL;
   sdm.q[prescribed22].min_restraint_func = NULL;
   sdm.q[prescribed22].max_restraint_func = NULL;

   mstrcpy(&sdm.q[midfoot_angle_l].name,"midfoot_angle_l");
   sdm.q[midfoot_angle_l].type = unconstrained_q;
   sdm.q[midfoot_angle_l].joint = midfoot_l;
   sdm.q[midfoot_angle_l].axis = 0;
   sdm.q[midfoot_angle_l].conversion = 57.29578;
   sdm.q[midfoot_angle_l].initial_value = 0.00000;
   sdm.q[midfoot_angle_l].range_start = -60.00000;
   sdm.q[midfoot_angle_l].range_end = 60.00000;
   sdm.q[midfoot_angle_l].restraint_func = NULL;
   sdm.q[midfoot_angle_l].min_restraint_func = &q_restraint_func[0];
   sdm.q[midfoot_angle_l].max_restraint_func = &q_restraint_func[0];

   mstrcpy(&sdm.q[mtp_angle_l].name,"mtp_angle_l");
   sdm.q[mtp_angle_l].type = unconstrained_q;
   sdm.q[mtp_angle_l].joint = mtp_l;
   sdm.q[mtp_angle_l].axis = 0;
   sdm.q[mtp_angle_l].conversion = 57.29578;
   sdm.q[mtp_angle_l].initial_value = 30.00000;
   sdm.q[mtp_angle_l].range_start = -60.00000;
   sdm.q[mtp_angle_l].range_end = 60.00000;
   sdm.q[mtp_angle_l].restraint_func = NULL;
   sdm.q[mtp_angle_l].min_restraint_func = &q_restraint_func[0];
   sdm.q[mtp_angle_l].max_restraint_func = &q_restraint_func[0];

   for (i=0, sdm.num_gencoords=0; i<sdm.nq; i++)
      if (sdm.q[i].type == unconstrained_q)
         sdm.num_gencoords++;

}



/* INIT_SEGMENTS: this routine should be called before
 * read_muscles() because it does two things that need to
 * be done before you read in the muscles. First, it assigns
 * numbers to the body segments in your model. These body
 * numbers should match the numbers in the "_info" file
 * that SD/FAST makes. Thus when you read in the muscle
 * attachment points, the segment name listed for each point
 * is converted into an SD/FAST body segment number.
 * Second, this routine records the positions of the mass
 * centers of each body segment. These are the coordinates of
 * the mass centers with respect to the origins of the SIMM
 * bone file. When you read in a muscle attachment point, the
 * mass center coordinates are subtracted from it, so that the
 * muscle point is now w.r.t. the mass center of the body
 * segment, as SD/FAST expects.
 * Note that you cannot use the #defined segment numbers as indices
 * into the sdm.body_segment[] array because they start at -1.
 */

void init_segments(void)
{

   sdm.num_body_segments = 16;

   sdm.body_segment = (BodyStruct*)simm_malloc(sdm.num_body_segments*sizeof(BodyStruct));

   mstrcpy(&sdm.body_segment[0].name,"ground");
   sdm.body_segment[0].mass = 0.000000;
   sdm.body_segment[0].inertia[0][0] = 0.000000;
   sdm.body_segment[0].inertia[0][1] = 0.000000;
   sdm.body_segment[0].inertia[0][2] = 0.000000;
   sdm.body_segment[0].inertia[1][0] = 0.000000;
   sdm.body_segment[0].inertia[1][1] = 0.000000;
   sdm.body_segment[0].inertia[1][2] = 0.000000;
   sdm.body_segment[0].inertia[2][0] = 0.000000;
   sdm.body_segment[0].inertia[2][1] = 0.000000;
   sdm.body_segment[0].inertia[2][2] = 0.000000;
   sdm.body_segment[0].mass_center[0] = 0.000000;
   sdm.body_segment[0].mass_center[1] = 0.000000;
   sdm.body_segment[0].mass_center[2] = 0.000000;

   mstrcpy(&sdm.body_segment[1].name,"pelvis");
   sdm.body_segment[1].mass = 51.100000;
   sdm.body_segment[1].inertia[0][0] = 1.396000;
   sdm.body_segment[1].inertia[0][1] = 0.000000;
   sdm.body_segment[1].inertia[0][2] = 0.000000;
   sdm.body_segment[1].inertia[1][0] = 0.000000;
   sdm.body_segment[1].inertia[1][1] = 0.715300;
   sdm.body_segment[1].inertia[1][2] = 0.000000;
   sdm.body_segment[1].inertia[2][0] = 0.000000;
   sdm.body_segment[1].inertia[2][1] = 0.000000;
   sdm.body_segment[1].inertia[2][2] = 2.580000;
   sdm.body_segment[1].mass_center[0] = -0.070700;
   sdm.body_segment[1].mass_center[1] = 0.263900;
   sdm.body_segment[1].mass_center[2] = 0.000000;

   mstrcpy(&sdm.body_segment[2].name,"femur_r");
   sdm.body_segment[2].mass = 7.760000;
   sdm.body_segment[2].inertia[0][0] = 0.137000;
   sdm.body_segment[2].inertia[0][1] = 0.000000;
   sdm.body_segment[2].inertia[0][2] = 0.000000;
   sdm.body_segment[2].inertia[1][0] = 0.000000;
   sdm.body_segment[2].inertia[1][1] = 0.031600;
   sdm.body_segment[2].inertia[1][2] = 0.000000;
   sdm.body_segment[2].inertia[2][0] = 0.000000;
   sdm.body_segment[2].inertia[2][1] = 0.000000;
   sdm.body_segment[2].inertia[2][2] = 0.137000;
   sdm.body_segment[2].mass_center[0] = 0.000000;
   sdm.body_segment[2].mass_center[1] = -0.170000;
   sdm.body_segment[2].mass_center[2] = 0.000000;

   mstrcpy(&sdm.body_segment[3].name,"tibia_r");
   sdm.body_segment[3].mass = 3.030000;
   sdm.body_segment[3].inertia[0][0] = 0.044400;
   sdm.body_segment[3].inertia[0][1] = 0.000000;
   sdm.body_segment[3].inertia[0][2] = 0.000000;
   sdm.body_segment[3].inertia[1][0] = 0.000000;
   sdm.body_segment[3].inertia[1][1] = 0.003830;
   sdm.body_segment[3].inertia[1][2] = 0.000000;
   sdm.body_segment[3].inertia[2][0] = 0.000000;
   sdm.body_segment[3].inertia[2][1] = 0.000000;
   sdm.body_segment[3].inertia[2][2] = 0.044500;
   sdm.body_segment[3].mass_center[0] = 0.000000;
   sdm.body_segment[3].mass_center[1] = -0.186700;
   sdm.body_segment[3].mass_center[2] = 0.000000;

   mstrcpy(&sdm.body_segment[4].name,"patella_r");
   sdm.body_segment[4].mass = 0.100000;
   sdm.body_segment[4].inertia[0][0] = 0.001000;
   sdm.body_segment[4].inertia[0][1] = 0.000000;
   sdm.body_segment[4].inertia[0][2] = 0.000000;
   sdm.body_segment[4].inertia[1][0] = 0.000000;
   sdm.body_segment[4].inertia[1][1] = 0.001000;
   sdm.body_segment[4].inertia[1][2] = 0.000000;
   sdm.body_segment[4].inertia[2][0] = 0.000000;
   sdm.body_segment[4].inertia[2][1] = 0.000000;
   sdm.body_segment[4].inertia[2][2] = 0.001000;
   sdm.body_segment[4].mass_center[0] = 0.000000;
   sdm.body_segment[4].mass_center[1] = 0.000000;
   sdm.body_segment[4].mass_center[2] = 0.000000;

   mstrcpy(&sdm.body_segment[5].name,"talus_r");
   sdm.body_segment[5].mass = 0.100000;
   sdm.body_segment[5].inertia[0][0] = 0.001000;
   sdm.body_segment[5].inertia[0][1] = 0.000000;
   sdm.body_segment[5].inertia[0][2] = 0.000000;
   sdm.body_segment[5].inertia[1][0] = 0.000000;
   sdm.body_segment[5].inertia[1][1] = 0.001000;
   sdm.body_segment[5].inertia[1][2] = 0.000000;
   sdm.body_segment[5].inertia[2][0] = 0.000000;
   sdm.body_segment[5].inertia[2][1] = 0.000000;
   sdm.body_segment[5].inertia[2][2] = 0.001000;
   sdm.body_segment[5].mass_center[0] = 0.000000;
   sdm.body_segment[5].mass_center[1] = 0.000000;
   sdm.body_segment[5].mass_center[2] = 0.000000;

   mstrcpy(&sdm.body_segment[6].name,"calcn_r");
   sdm.body_segment[6].mass = 0.418300;
   sdm.body_segment[6].inertia[0][0] = 0.001000;
   sdm.body_segment[6].inertia[0][1] = 0.000000;
   sdm.body_segment[6].inertia[0][2] = 0.000000;
   sdm.body_segment[6].inertia[1][0] = 0.000000;
   sdm.body_segment[6].inertia[1][1] = 0.001000;
   sdm.body_segment[6].inertia[1][2] = 0.000000;
   sdm.body_segment[6].inertia[2][0] = 0.000000;
   sdm.body_segment[6].inertia[2][1] = 0.000000;
   sdm.body_segment[6].inertia[2][2] = 0.001000;
   sdm.body_segment[6].mass_center[0] = 0.025000;
   sdm.body_segment[6].mass_center[1] = 0.015000;
   sdm.body_segment[6].mass_center[2] = 0.000000;

   mstrcpy(&sdm.body_segment[7].name,"midfoot_r");
   sdm.body_segment[7].mass = 0.400000;
   sdm.body_segment[7].inertia[0][0] = 0.001000;
   sdm.body_segment[7].inertia[0][1] = 0.000000;
   sdm.body_segment[7].inertia[0][2] = 0.000000;
   sdm.body_segment[7].inertia[1][0] = 0.000000;
   sdm.body_segment[7].inertia[1][1] = 0.001000;
   sdm.body_segment[7].inertia[1][2] = 0.000000;
   sdm.body_segment[7].inertia[2][0] = 0.000000;
   sdm.body_segment[7].inertia[2][1] = 0.000000;
   sdm.body_segment[7].inertia[2][2] = 0.001000;
   sdm.body_segment[7].mass_center[0] = 0.025000;
   sdm.body_segment[7].mass_center[1] = 0.015000;
   sdm.body_segment[7].mass_center[2] = 0.000000;

   mstrcpy(&sdm.body_segment[8].name,"toes_r");
   sdm.body_segment[8].mass = 0.205800;
   sdm.body_segment[8].inertia[0][0] = 0.001000;
   sdm.body_segment[8].inertia[0][1] = 0.000000;
   sdm.body_segment[8].inertia[0][2] = 0.000000;
   sdm.body_segment[8].inertia[1][0] = 0.000000;
   sdm.body_segment[8].inertia[1][1] = 0.001000;
   sdm.body_segment[8].inertia[1][2] = 0.000000;
   sdm.body_segment[8].inertia[2][0] = 0.000000;
   sdm.body_segment[8].inertia[2][1] = 0.000000;
   sdm.body_segment[8].inertia[2][2] = 0.001000;
   sdm.body_segment[8].mass_center[0] = 0.034600;
   sdm.body_segment[8].mass_center[1] = 0.006000;
   sdm.body_segment[8].mass_center[2] = -0.017500;

   mstrcpy(&sdm.body_segment[9].name,"femur_l");
   sdm.body_segment[9].mass = 7.760000;
   sdm.body_segment[9].inertia[0][0] = 0.137000;
   sdm.body_segment[9].inertia[0][1] = 0.000000;
   sdm.body_segment[9].inertia[0][2] = 0.000000;
   sdm.body_segment[9].inertia[1][0] = 0.000000;
   sdm.body_segment[9].inertia[1][1] = 0.031600;
   sdm.body_segment[9].inertia[1][2] = 0.000000;
   sdm.body_segment[9].inertia[2][0] = 0.000000;
   sdm.body_segment[9].inertia[2][1] = 0.000000;
   sdm.body_segment[9].inertia[2][2] = 0.137000;
   sdm.body_segment[9].mass_center[0] = 0.000000;
   sdm.body_segment[9].mass_center[1] = -0.170000;
   sdm.body_segment[9].mass_center[2] = 0.000000;

   mstrcpy(&sdm.body_segment[10].name,"tibia_l");
   sdm.body_segment[10].mass = 3.030000;
   sdm.body_segment[10].inertia[0][0] = 0.044400;
   sdm.body_segment[10].inertia[0][1] = 0.000000;
   sdm.body_segment[10].inertia[0][2] = 0.000000;
   sdm.body_segment[10].inertia[1][0] = 0.000000;
   sdm.body_segment[10].inertia[1][1] = 0.003830;
   sdm.body_segment[10].inertia[1][2] = 0.000000;
   sdm.body_segment[10].inertia[2][0] = 0.000000;
   sdm.body_segment[10].inertia[2][1] = 0.000000;
   sdm.body_segment[10].inertia[2][2] = 0.044500;
   sdm.body_segment[10].mass_center[0] = 0.000000;
   sdm.body_segment[10].mass_center[1] = -0.186700;
   sdm.body_segment[10].mass_center[2] = 0.000000;

   mstrcpy(&sdm.body_segment[11].name,"patella_l");
   sdm.body_segment[11].mass = 0.100000;
   sdm.body_segment[11].inertia[0][0] = 0.001000;
   sdm.body_segment[11].inertia[0][1] = 0.000000;
   sdm.body_segment[11].inertia[0][2] = 0.000000;
   sdm.body_segment[11].inertia[1][0] = 0.000000;
   sdm.body_segment[11].inertia[1][1] = 0.001000;
   sdm.body_segment[11].inertia[1][2] = 0.000000;
   sdm.body_segment[11].inertia[2][0] = 0.000000;
   sdm.body_segment[11].inertia[2][1] = 0.000000;
   sdm.body_segment[11].inertia[2][2] = 0.001000;
   sdm.body_segment[11].mass_center[0] = 0.000000;
   sdm.body_segment[11].mass_center[1] = 0.000000;
   sdm.body_segment[11].mass_center[2] = 0.000000;

   mstrcpy(&sdm.body_segment[12].name,"talus_l");
   sdm.body_segment[12].mass = 0.100000;
   sdm.body_segment[12].inertia[0][0] = 0.001000;
   sdm.body_segment[12].inertia[0][1] = 0.000000;
   sdm.body_segment[12].inertia[0][2] = 0.000000;
   sdm.body_segment[12].inertia[1][0] = 0.000000;
   sdm.body_segment[12].inertia[1][1] = 0.001000;
   sdm.body_segment[12].inertia[1][2] = 0.000000;
   sdm.body_segment[12].inertia[2][0] = 0.000000;
   sdm.body_segment[12].inertia[2][1] = 0.000000;
   sdm.body_segment[12].inertia[2][2] = 0.001000;
   sdm.body_segment[12].mass_center[0] = 0.000000;
   sdm.body_segment[12].mass_center[1] = 0.000000;
   sdm.body_segment[12].mass_center[2] = 0.000000;

   mstrcpy(&sdm.body_segment[13].name,"calcn_l");
   sdm.body_segment[13].mass = 0.418300;
   sdm.body_segment[13].inertia[0][0] = 0.001000;
   sdm.body_segment[13].inertia[0][1] = 0.000000;
   sdm.body_segment[13].inertia[0][2] = 0.000000;
   sdm.body_segment[13].inertia[1][0] = 0.000000;
   sdm.body_segment[13].inertia[1][1] = 0.001000;
   sdm.body_segment[13].inertia[1][2] = 0.000000;
   sdm.body_segment[13].inertia[2][0] = 0.000000;
   sdm.body_segment[13].inertia[2][1] = 0.000000;
   sdm.body_segment[13].inertia[2][2] = 0.001000;
   sdm.body_segment[13].mass_center[0] = 0.025000;
   sdm.body_segment[13].mass_center[1] = 0.015000;
   sdm.body_segment[13].mass_center[2] = 0.000000;

   mstrcpy(&sdm.body_segment[14].name,"midfoot_l");
   sdm.body_segment[14].mass = 0.400000;
   sdm.body_segment[14].inertia[0][0] = 0.001000;
   sdm.body_segment[14].inertia[0][1] = 0.000000;
   sdm.body_segment[14].inertia[0][2] = 0.000000;
   sdm.body_segment[14].inertia[1][0] = 0.000000;
   sdm.body_segment[14].inertia[1][1] = 0.001000;
   sdm.body_segment[14].inertia[1][2] = 0.000000;
   sdm.body_segment[14].inertia[2][0] = 0.000000;
   sdm.body_segment[14].inertia[2][1] = 0.000000;
   sdm.body_segment[14].inertia[2][2] = 0.001000;
   sdm.body_segment[14].mass_center[0] = 0.025000;
   sdm.body_segment[14].mass_center[1] = 0.015000;
   sdm.body_segment[14].mass_center[2] = 0.000000;

   mstrcpy(&sdm.body_segment[15].name,"toes_l");
   sdm.body_segment[15].mass = 0.205800;
   sdm.body_segment[15].inertia[0][0] = 0.001000;
   sdm.body_segment[15].inertia[0][1] = 0.000000;
   sdm.body_segment[15].inertia[0][2] = 0.000000;
   sdm.body_segment[15].inertia[1][0] = 0.000000;
   sdm.body_segment[15].inertia[1][1] = 0.001000;
   sdm.body_segment[15].inertia[1][2] = 0.000000;
   sdm.body_segment[15].inertia[2][0] = 0.000000;
   sdm.body_segment[15].inertia[2][1] = 0.000000;
   sdm.body_segment[15].inertia[2][2] = 0.001000;
   sdm.body_segment[15].mass_center[0] = 0.034600;
   sdm.body_segment[15].mass_center[1] = 0.006000;
   sdm.body_segment[15].mass_center[2] = 0.017500;

}



/* INIT_Q_RESTRAINT_FUNCTIONS: this routine initializes the restraint
 * functions which are used to keep the Qs from exceeding their ranges of motion.
 */

void init_q_restraint_functions(void)
{

   int i, numpts;

   numpts = sizeof(q_restraint_func1_data)/(sizeof(double)*2);
   (void)malloc_function(&q_restraint_func[0],numpts);
   q_restraint_func[0].numpoints = numpts;
   for (i=0; i<numpts; i++)
   {
      q_restraint_func[0].x[i] = q_restraint_func1_data[i][0];
      q_restraint_func[0].y[i] = q_restraint_func1_data[i][1];
   }
   calc_spline_coefficients(&q_restraint_func[0]);

}


/* INIT_JOINT_FUNCTIONS: this routine initializes the constraint functions
 * for the joints which have user-defined constrained.
 */

void init_joint_functions(void)
{

   int i, numpts;

   numpts = sizeof(knee_r_tx_data)/(sizeof(double)*2);
   (void)malloc_function(&knee_r_tx_func,numpts);
   knee_r_tx_func.numpoints = numpts;
   for (i=0; i<numpts; i++)
   {
      knee_r_tx_func.x[i] = knee_r_tx_data[i][0];
      knee_r_tx_func.y[i] = knee_r_tx_data[i][1];
   }
   calc_spline_coefficients(&knee_r_tx_func);

   numpts = sizeof(knee_r_ty_data)/(sizeof(double)*2);
   (void)malloc_function(&knee_r_ty_func,numpts);
   knee_r_ty_func.numpoints = numpts;
   for (i=0; i<numpts; i++)
   {
      knee_r_ty_func.x[i] = knee_r_ty_data[i][0];
      knee_r_ty_func.y[i] = knee_r_ty_data[i][1];
   }
   calc_spline_coefficients(&knee_r_ty_func);

   numpts = sizeof(tib_pat_r_r3_data)/(sizeof(double)*2);
   (void)malloc_function(&tib_pat_r_r3_func,numpts);
   tib_pat_r_r3_func.numpoints = numpts;
   for (i=0; i<numpts; i++)
   {
      tib_pat_r_r3_func.x[i] = tib_pat_r_r3_data[i][0];
      tib_pat_r_r3_func.y[i] = tib_pat_r_r3_data[i][1];
   }
   calc_spline_coefficients(&tib_pat_r_r3_func);

   numpts = sizeof(tib_pat_r_tx_data)/(sizeof(double)*2);
   (void)malloc_function(&tib_pat_r_tx_func,numpts);
   tib_pat_r_tx_func.numpoints = numpts;
   for (i=0; i<numpts; i++)
   {
      tib_pat_r_tx_func.x[i] = tib_pat_r_tx_data[i][0];
      tib_pat_r_tx_func.y[i] = tib_pat_r_tx_data[i][1];
   }
   calc_spline_coefficients(&tib_pat_r_tx_func);

   numpts = sizeof(tib_pat_r_ty_data)/(sizeof(double)*2);
   (void)malloc_function(&tib_pat_r_ty_func,numpts);
   tib_pat_r_ty_func.numpoints = numpts;
   for (i=0; i<numpts; i++)
   {
      tib_pat_r_ty_func.x[i] = tib_pat_r_ty_data[i][0];
      tib_pat_r_ty_func.y[i] = tib_pat_r_ty_data[i][1];
   }
   calc_spline_coefficients(&tib_pat_r_ty_func);

   numpts = sizeof(knee_l_tx_data)/(sizeof(double)*2);
   (void)malloc_function(&knee_l_tx_func,numpts);
   knee_l_tx_func.numpoints = numpts;
   for (i=0; i<numpts; i++)
   {
      knee_l_tx_func.x[i] = knee_l_tx_data[i][0];
      knee_l_tx_func.y[i] = knee_l_tx_data[i][1];
   }
   calc_spline_coefficients(&knee_l_tx_func);

   numpts = sizeof(knee_l_ty_data)/(sizeof(double)*2);
   (void)malloc_function(&knee_l_ty_func,numpts);
   knee_l_ty_func.numpoints = numpts;
   for (i=0; i<numpts; i++)
   {
      knee_l_ty_func.x[i] = knee_l_ty_data[i][0];
      knee_l_ty_func.y[i] = knee_l_ty_data[i][1];
   }
   calc_spline_coefficients(&knee_l_ty_func);

   numpts = sizeof(tib_pat_l_r3_data)/(sizeof(double)*2);
   (void)malloc_function(&tib_pat_l_r3_func,numpts);
   tib_pat_l_r3_func.numpoints = numpts;
   for (i=0; i<numpts; i++)
   {
      tib_pat_l_r3_func.x[i] = tib_pat_l_r3_data[i][0];
      tib_pat_l_r3_func.y[i] = tib_pat_l_r3_data[i][1];
   }
   calc_spline_coefficients(&tib_pat_l_r3_func);

   numpts = sizeof(tib_pat_l_tx_data)/(sizeof(double)*2);
   (void)malloc_function(&tib_pat_l_tx_func,numpts);
   tib_pat_l_tx_func.numpoints = numpts;
   for (i=0; i<numpts; i++)
   {
      tib_pat_l_tx_func.x[i] = tib_pat_l_tx_data[i][0];
      tib_pat_l_tx_func.y[i] = tib_pat_l_tx_data[i][1];
   }
   calc_spline_coefficients(&tib_pat_l_tx_func);

   numpts = sizeof(tib_pat_l_ty_data)/(sizeof(double)*2);
   (void)malloc_function(&tib_pat_l_ty_func,numpts);
   tib_pat_l_ty_func.numpoints = numpts;
   for (i=0; i<numpts; i++)
   {
      tib_pat_l_ty_func.x[i] = tib_pat_l_ty_data[i][0];
      tib_pat_l_ty_func.y[i] = tib_pat_l_ty_data[i][1];
   }
   calc_spline_coefficients(&tib_pat_l_ty_func);

}


void sduperr(double t, double q[], double errors[])
{

   double q_ind, q_dep, u_ind = 0.0, a_ind = 0.0;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   q_dep = q[knee_r_tx];
   errors[knee_r_tx_con] = q_dep - interpolate_spline(q_ind,&knee_r_tx_func,zeroth,u_ind,a_ind)/sdm.q[knee_r_tx].conversion;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   q_dep = q[knee_r_ty];
   errors[knee_r_ty_con] = q_dep - interpolate_spline(q_ind,&knee_r_ty_func,zeroth,u_ind,a_ind)/sdm.q[knee_r_ty].conversion;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   q_dep = q[tib_pat_r_tx];
   errors[tib_pat_r_tx_con] = q_dep - interpolate_spline(q_ind,&tib_pat_r_tx_func,zeroth,u_ind,a_ind)/sdm.q[tib_pat_r_tx].conversion;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   q_dep = q[tib_pat_r_ty];
   errors[tib_pat_r_ty_con] = q_dep - interpolate_spline(q_ind,&tib_pat_r_ty_func,zeroth,u_ind,a_ind)/sdm.q[tib_pat_r_ty].conversion;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   q_dep = q[tib_pat_r_r3];
   errors[tib_pat_r_r3_con] = q_dep - interpolate_spline(q_ind,&tib_pat_r_r3_func,zeroth,u_ind,a_ind)/sdm.q[tib_pat_r_r3].conversion;

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   q_dep = q[knee_l_tx];
   errors[knee_l_tx_con] = q_dep - interpolate_spline(q_ind,&knee_l_tx_func,zeroth,u_ind,a_ind)/sdm.q[knee_l_tx].conversion;

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   q_dep = q[knee_l_ty];
   errors[knee_l_ty_con] = q_dep - interpolate_spline(q_ind,&knee_l_ty_func,zeroth,u_ind,a_ind)/sdm.q[knee_l_ty].conversion;

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   q_dep = q[tib_pat_l_tx];
   errors[tib_pat_l_tx_con] = q_dep - interpolate_spline(q_ind,&tib_pat_l_tx_func,zeroth,u_ind,a_ind)/sdm.q[tib_pat_l_tx].conversion;

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   q_dep = q[tib_pat_l_ty];
   errors[tib_pat_l_ty_con] = q_dep - interpolate_spline(q_ind,&tib_pat_l_ty_func,zeroth,u_ind,a_ind)/sdm.q[tib_pat_l_ty].conversion;

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   q_dep = q[tib_pat_l_r3];
   errors[tib_pat_l_r3_con] = q_dep - interpolate_spline(q_ind,&tib_pat_l_r3_func,zeroth,u_ind,a_ind)/sdm.q[tib_pat_l_r3].conversion;

}


void sduverr(double t, double q[], double u[], double errors[])
{

   double q_ind, u_ind, u_dep, a_ind = 0.0;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_ind = u[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_dep = u[knee_r_tx];
   errors[knee_r_tx_con] = u_dep - interpolate_spline(q_ind,&knee_r_tx_func,first,u_ind,a_ind)/sdm.q[knee_r_tx].conversion;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_ind = u[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_dep = u[knee_r_ty];
   errors[knee_r_ty_con] = u_dep - interpolate_spline(q_ind,&knee_r_ty_func,first,u_ind,a_ind)/sdm.q[knee_r_ty].conversion;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_ind = u[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_dep = u[tib_pat_r_tx];
   errors[tib_pat_r_tx_con] = u_dep - interpolate_spline(q_ind,&tib_pat_r_tx_func,first,u_ind,a_ind)/sdm.q[tib_pat_r_tx].conversion;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_ind = u[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_dep = u[tib_pat_r_ty];
   errors[tib_pat_r_ty_con] = u_dep - interpolate_spline(q_ind,&tib_pat_r_ty_func,first,u_ind,a_ind)/sdm.q[tib_pat_r_ty].conversion;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_ind = u[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_dep = u[tib_pat_r_r3];
   errors[tib_pat_r_r3_con] = u_dep - interpolate_spline(q_ind,&tib_pat_r_r3_func,first,u_ind,a_ind)/sdm.q[tib_pat_r_r3].conversion;

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_ind = u[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_dep = u[knee_l_tx];
   errors[knee_l_tx_con] = u_dep - interpolate_spline(q_ind,&knee_l_tx_func,first,u_ind,a_ind)/sdm.q[knee_l_tx].conversion;

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_ind = u[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_dep = u[knee_l_ty];
   errors[knee_l_ty_con] = u_dep - interpolate_spline(q_ind,&knee_l_ty_func,first,u_ind,a_ind)/sdm.q[knee_l_ty].conversion;

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_ind = u[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_dep = u[tib_pat_l_tx];
   errors[tib_pat_l_tx_con] = u_dep - interpolate_spline(q_ind,&tib_pat_l_tx_func,first,u_ind,a_ind)/sdm.q[tib_pat_l_tx].conversion;

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_ind = u[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_dep = u[tib_pat_l_ty];
   errors[tib_pat_l_ty_con] = u_dep - interpolate_spline(q_ind,&tib_pat_l_ty_func,first,u_ind,a_ind)/sdm.q[tib_pat_l_ty].conversion;

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_ind = u[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_dep = u[tib_pat_l_r3];
   errors[tib_pat_l_r3_con] = u_dep - interpolate_spline(q_ind,&tib_pat_l_r3_func,first,u_ind,a_ind)/sdm.q[tib_pat_l_r3].conversion;

}


void sduaerr(double t, double q[], double u[], double udot[], double errors[])
{

   double q_ind, u_ind, a_ind, a_dep;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_ind = u[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   a_ind = udot[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   a_dep = udot[knee_r_tx];
   errors[knee_r_tx_con] = a_dep - interpolate_spline(q_ind,&knee_r_tx_func,second,u_ind,a_ind)/sdm.q[knee_r_tx].conversion;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_ind = u[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   a_ind = udot[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   a_dep = udot[knee_r_ty];
   errors[knee_r_ty_con] = a_dep - interpolate_spline(q_ind,&knee_r_ty_func,second,u_ind,a_ind)/sdm.q[knee_r_ty].conversion;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_ind = u[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   a_ind = udot[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   a_dep = udot[tib_pat_r_tx];
   errors[tib_pat_r_tx_con] = a_dep - interpolate_spline(q_ind,&tib_pat_r_tx_func,second,u_ind,a_ind)/sdm.q[tib_pat_r_tx].conversion;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_ind = u[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   a_ind = udot[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   a_dep = udot[tib_pat_r_ty];
   errors[tib_pat_r_ty_con] = a_dep - interpolate_spline(q_ind,&tib_pat_r_ty_func,second,u_ind,a_ind)/sdm.q[tib_pat_r_ty].conversion;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_ind = u[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   a_ind = udot[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   a_dep = udot[tib_pat_r_r3];
   errors[tib_pat_r_r3_con] = a_dep - interpolate_spline(q_ind,&tib_pat_r_r3_func,second,u_ind,a_ind)/sdm.q[tib_pat_r_r3].conversion;

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_ind = u[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   a_ind = udot[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   a_dep = udot[knee_l_tx];
   errors[knee_l_tx_con] = a_dep - interpolate_spline(q_ind,&knee_l_tx_func,second,u_ind,a_ind)/sdm.q[knee_l_tx].conversion;

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_ind = u[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   a_ind = udot[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   a_dep = udot[knee_l_ty];
   errors[knee_l_ty_con] = a_dep - interpolate_spline(q_ind,&knee_l_ty_func,second,u_ind,a_ind)/sdm.q[knee_l_ty].conversion;

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_ind = u[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   a_ind = udot[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   a_dep = udot[tib_pat_l_tx];
   errors[tib_pat_l_tx_con] = a_dep - interpolate_spline(q_ind,&tib_pat_l_tx_func,second,u_ind,a_ind)/sdm.q[tib_pat_l_tx].conversion;

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_ind = u[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   a_ind = udot[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   a_dep = udot[tib_pat_l_ty];
   errors[tib_pat_l_ty_con] = a_dep - interpolate_spline(q_ind,&tib_pat_l_ty_func,second,u_ind,a_ind)/sdm.q[tib_pat_l_ty].conversion;

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_ind = u[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   a_ind = udot[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   a_dep = udot[tib_pat_l_r3];
   errors[tib_pat_l_r3_con] = a_dep - interpolate_spline(q_ind,&tib_pat_l_r3_func,second,u_ind,a_ind)/sdm.q[tib_pat_l_r3].conversion;

}


void sduconsfrc(double t, double q[], double u[], double mults[])
{

   double q_ind, torque, u_ind = RAD_TO_DEG, a_ind = 0.0;

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_ind = sdm.q[knee_angle_r].conversion;
   sdhinget(knee_r,0,mults[knee_r_tx_con]);
   torque = -mults[knee_r_tx_con] * interpolate_spline(q_ind,&knee_r_tx_func,first,u_ind,a_ind)/sdm.q[knee_r_tx].conversion;
   sdhinget(knee_r,2,torque);

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_ind = sdm.q[knee_angle_r].conversion;
   sdhinget(knee_r,1,mults[knee_r_ty_con]);
   torque = -mults[knee_r_ty_con] * interpolate_spline(q_ind,&knee_r_ty_func,first,u_ind,a_ind)/sdm.q[knee_r_ty].conversion;
   sdhinget(knee_r,2,torque);

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_ind = sdm.q[knee_angle_r].conversion;
   sdhinget(tib_pat_r,0,mults[tib_pat_r_tx_con]);
   torque = -mults[tib_pat_r_tx_con] * interpolate_spline(q_ind,&tib_pat_r_tx_func,first,u_ind,a_ind)/sdm.q[tib_pat_r_tx].conversion;
   sdhinget(knee_r,2,torque);

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_ind = sdm.q[knee_angle_r].conversion;
   sdhinget(tib_pat_r,1,mults[tib_pat_r_ty_con]);
   torque = -mults[tib_pat_r_ty_con] * interpolate_spline(q_ind,&tib_pat_r_ty_func,first,u_ind,a_ind)/sdm.q[tib_pat_r_ty].conversion;
   sdhinget(knee_r,2,torque);

   q_ind = q[knee_angle_r]*sdm.q[knee_angle_r].conversion;
   u_ind = sdm.q[knee_angle_r].conversion;
   sdhinget(tib_pat_r,2,mults[tib_pat_r_r3_con]);
   torque = -mults[tib_pat_r_r3_con] * interpolate_spline(q_ind,&tib_pat_r_r3_func,first,u_ind,a_ind)/sdm.q[tib_pat_r_r3].conversion;
   sdhinget(knee_r,2,torque);

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_ind = sdm.q[knee_angle_l].conversion;
   sdhinget(knee_l,0,mults[knee_l_tx_con]);
   torque = -mults[knee_l_tx_con] * interpolate_spline(q_ind,&knee_l_tx_func,first,u_ind,a_ind)/sdm.q[knee_l_tx].conversion;
   sdhinget(knee_l,2,torque);

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_ind = sdm.q[knee_angle_l].conversion;
   sdhinget(knee_l,1,mults[knee_l_ty_con]);
   torque = -mults[knee_l_ty_con] * interpolate_spline(q_ind,&knee_l_ty_func,first,u_ind,a_ind)/sdm.q[knee_l_ty].conversion;
   sdhinget(knee_l,2,torque);

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_ind = sdm.q[knee_angle_l].conversion;
   sdhinget(tib_pat_l,0,mults[tib_pat_l_tx_con]);
   torque = -mults[tib_pat_l_tx_con] * interpolate_spline(q_ind,&tib_pat_l_tx_func,first,u_ind,a_ind)/sdm.q[tib_pat_l_tx].conversion;
   sdhinget(knee_l,2,torque);

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_ind = sdm.q[knee_angle_l].conversion;
   sdhinget(tib_pat_l,1,mults[tib_pat_l_ty_con]);
   torque = -mults[tib_pat_l_ty_con] * interpolate_spline(q_ind,&tib_pat_l_ty_func,first,u_ind,a_ind)/sdm.q[tib_pat_l_ty].conversion;
   sdhinget(knee_l,2,torque);

   q_ind = q[knee_angle_l]*sdm.q[knee_angle_l].conversion;
   u_ind = sdm.q[knee_angle_l].conversion;
   sdhinget(tib_pat_l,2,mults[tib_pat_l_r3_con]);
   torque = -mults[tib_pat_l_r3_con] * interpolate_spline(q_ind,&tib_pat_l_r3_func,first,u_ind,a_ind)/sdm.q[tib_pat_l_r3].conversion;
   sdhinget(knee_l,2,torque);

}


WrapObject* get_wrap_object(char name[])
{

   /* There are no wrap objects in this model. */
   return NULL;

}

