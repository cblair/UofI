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

/* Muscles */

/*These are now in ExcGroups.h
#define sol_r 0
#define mgas_r 1
#define sm_r 2
#define bfsh_r 3
#define psoas_r 4
#define ta_r 5
#define rf_r 6
#define vas1_r 7
#define gmax_r 8
#define gmeda_r 9
#define perlng_r 10
#define tp_r 11
#define flxdig_r 12
#define flxhal_r 13
#define lgas_r 14
#define bflh_r 15
#define iliac_r 16
#define pertert_r 17
#define extdig_r 18
#define exthal_r 19
#define vas2_r 20
#define vas3_r 21
#define addmag_r 22
#define gmedp_r 23
#define perbrev_r 24

#define sol_l 25
#define mgas_l 26
#define sm_l 27
#define bfsh_l 28
#define psoas_l 29
#define ta_l 30
#define rf_l 31
#define vas1_l 32
#define gmax_l 33
#define gmeda_l 34
#define perlng_l 35
#define tp_l 36
#define flxdig_l 37
#define flxhal_l 38
#define lgas_l 39
#define bflh_l 40
#define iliac_l 41
#define pertert_l 42
#define extdig_l 43
#define exthal_l 44
#define vas2_l 45
#define vas3_l 46
#define addmag_l 47
#define gmedp_l 48
#define perbrev_l 49
*/
//muscles per leg
#define sol 0
#define mgas 1
#define sm 2
#define bfsh 3
#define psoas 4
#define ta 5
#define rf 6
#define vas1 7
#define gmax 8
#define gmeda 9
#define perlng 10
#define tp 11
#define flxdig 12
#define flxhal 13
#define lgas 14
#define bflh 15
#define iliac 16
#define pertert 17
#define extdig 18
#define exthal 19
#define vas2 20
#define vas3 21
#define addmag 22
#define gmedp 23
#define perbrev 24

/*placed in ExcGroups.h
//muscle group
#define solG 0
#define gasG 1
#define smG 2
#define bfshG 3
#define psoasG 4
#define taG 5
#define rfG 6
#define vas1G 7
#define vas2G 8
#define vas3G 9
#define gmaxG 10
#define gmedG 11
#define perlngG 12
#define flxdigG 13
#define extdigG 14
*/

// leg joint
#define hipJ_r 0
#define kneeJ_r 1
#define ankleJ_r 2
#define midfootJ_r 3
#define mtpJ_r 4
#define hipJ_l 5
#define kneeJ_l 6
#define ankleJ_l 7
#define midfootJ_l 8
#define mtpJ_l 9

