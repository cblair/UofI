/*
Generated 22-Jun-2007 09:35:40 by SD/FAST, Kane's formulation
(sdfast B.2.8 #30123) on machine ID unknown
Copyright (c) 1990-1997 Symbolic Dynamics, Inc.
Copyright (c) 1990-1997 Parametric Technology Corp.
RESTRICTED RIGHTS LEGEND: Use, duplication, or disclosure by the U.S.
Government is subject to restrictions as set forth in subparagraph
(c)(1)(ii) of the Rights in Technical Data and Computer Software
clause at DFARS 52.227-7013 and similar clauses in the FAR and NASA
FAR Supplement.  Symbolic Dynamics, Inc., Mountain View, CA 94041


ROADMAP (model.sd)

Bodies        Inb
No  Name      body Joint type  Coords q         Multipliers
--- --------- ---- ----------- ---------------- -----------------------
 -1 $ground                                    |
  0 pelvis     -1  Planar        0   1   2     |
  1 femur_r     0  Pin           3             |
  2 tibia_r     1  Planar        4   5   6     |
  3 patella_r   2  Planar        7   8   9     |
  4 talus_r     2  Pin          10             |
  5 calcn_r     4  Pin          11p            |  0p
  6 midfoot_r   5  Pin          12             |
  7 toes_r      6  Pin          13             |
  8 femur_l     0  Pin          14             |
  9 tibia_l     8  Planar       15  16  17     |
 10 patella_l   9  Planar       18  19  20     |
 11 talus_l     9  Pin          21             |
 12 calcn_l    11  Pin          22p            |  1p
 13 midfoot_l  12  Pin          23             |
 14 toes_l     13  Pin          24             |

User Constraints

  0 user_0                                     |  2
  1 user_1                                     |  3
  2 user_2                                     |  4
  3 user_3                                     |  5
  4 user_4                                     |  6
  5 user_5                                     |  7
  6 user_6                                     |  8
  7 user_7                                     |  9
  8 user_8                                     | 10
  9 user_9                                     | 11

*/
#include <math.h>
#include <stdio.h>

typedef struct {
    int ground_,nbod_,ndof_,ncons_,nloop_,nldof_,nloopc_,nball_,nlball_,npres_,
      nuser_;
    int jtype_[15],inb_[15],outb_[15],njntdof_[15],njntc_[15],njntp_[15],firstq_
      [15],ballq_[15],firstm_[15],firstp_[15];
    int trans_[25],firstu_[10];
} sdgtopo_t;
#define ground (sdgtopo.ground_)
#define nbod (sdgtopo.nbod_)
#define ndof (sdgtopo.ndof_)
#define ncons (sdgtopo.ncons_)
#define nloop (sdgtopo.nloop_)
#define nldof (sdgtopo.nldof_)
#define nloopc (sdgtopo.nloopc_)
#define nball (sdgtopo.nball_)
#define nlball (sdgtopo.nlball_)
#define npres (sdgtopo.npres_)
#define nuser (sdgtopo.nuser_)
#define jtype (sdgtopo.jtype_)
#define inb (sdgtopo.inb_)
#define outb (sdgtopo.outb_)
#define njntdof (sdgtopo.njntdof_)
#define njntc (sdgtopo.njntc_)
#define njntp (sdgtopo.njntp_)
#define firstq (sdgtopo.firstq_)
#define ballq (sdgtopo.ballq_)
#define firstm (sdgtopo.firstm_)
#define firstp (sdgtopo.firstp_)
#define trans (sdgtopo.trans_)
#define firstu (sdgtopo.firstu_)

typedef struct {
    double grav_[3],mk_[15],ik_[15][3][3],pin_[25][3];
    double rk_[15][3],ri_[15][3],pres_[25],stabvel_,stabpos_;
    int mfrcflg_,roustate_,vpkflg_,inerflg_,mmflg_,mmlduflg_,wwflg_,ltauflg_,
      fs0flg_,ii_,mmap_[25];
    int gravq_[3],mkq_[15],ikq_[15][3][3],pinq_[25][3],rkq_[15][3],riq_[15][3],
      presq_[25],stabvelq_,stabposq_;
    double mtot_,psmkg_,rhead_[15][3],rcom_[15][3],mkrcomt_[15][3][3],psikg_[3][
      3],psrcomg_[3],psrkg_[3],psrig_[3],psmk_[25],psik_[25][3][3],psrcom_[25][3
      ],psrk_[25][3],psri_[25][3];
} sdginput_t;
#define grav (sdginput.grav_)
#define mk (sdginput.mk_)
#define ik (sdginput.ik_)
#define pin (sdginput.pin_)
#define rk (sdginput.rk_)
#define ri (sdginput.ri_)
#define pres (sdginput.pres_)
#define stabvel (sdginput.stabvel_)
#define stabpos (sdginput.stabpos_)
#define rhead (sdginput.rhead_)
#define rcom (sdginput.rcom_)
#define psrcomg (sdginput.psrcomg_)
#define psrcom (sdginput.psrcom_)
#define mkrcomt (sdginput.mkrcomt_)
#define psmk (sdginput.psmk_)
#define psik (sdginput.psik_)
#define psrk (sdginput.psrk_)
#define psri (sdginput.psri_)
#define psmkg (sdginput.psmkg_)
#define psikg (sdginput.psikg_)
#define psrkg (sdginput.psrkg_)
#define psrig (sdginput.psrig_)
#define mtot (sdginput.mtot_)
#define mfrcflg (sdginput.mfrcflg_)
#define roustate (sdginput.roustate_)
#define vpkflg (sdginput.vpkflg_)
#define inerflg (sdginput.inerflg_)
#define mmflg (sdginput.mmflg_)
#define mmlduflg (sdginput.mmlduflg_)
#define wwflg (sdginput.wwflg_)
#define ltauflg (sdginput.ltauflg_)
#define fs0flg (sdginput.fs0flg_)
#define ii (sdginput.ii_)
#define mmap (sdginput.mmap_)
#define gravq (sdginput.gravq_)
#define mkq (sdginput.mkq_)
#define ikq (sdginput.ikq_)
#define pinq (sdginput.pinq_)
#define rkq (sdginput.rkq_)
#define riq (sdginput.riq_)
#define presq (sdginput.presq_)
#define stabvelq (sdginput.stabvelq_)
#define stabposq (sdginput.stabposq_)

typedef struct {
    double curtim_,q_[25],qn_[25],u_[25],cnk_[25][3][3],cnb_[15][3][3];
    double rnk_[25][3],vnk_[25][3],wk_[25][3],rnb_[15][3],vnb_[15][3],wb_[15][3]
      ,wbrcom_[15][3],com_[3],rnkg_[3];
    double Cik_[25][3][3],rikt_[25][3][3],Iko_[25][3][3],mkrk_[25][3][3],Cib_[15
      ][3][3];
    double Wkk_[25][3],Vkk_[25][3],dik_[25][3],rpp_[25][3],rpk_[25][3],rik_[25][
      3],rik2_[25][3];
    double rpri_[25][3],Wik_[25][3],Vik_[25][3],Wirk_[25][3],rkWkk_[25][3],
      Wkrpk_[25][3],VikWkr_[25][3];
    double perr_[12],verr_[12],aerr_[12],mult_[12],ufk_[15][3],utk_[15][3],mfk_[
      15][3],mtk_[15][3];
    double utau_[25],mtau_[25],uacc_[25],uvel_[25],upos_[25];
    double s2_,c2_,s3_,c3_,s6_,c6_,s9_,c9_,s10_,c10_,s11_,c11_,s12_,c12_,s13_,
      c13_,s14_,c14_,s17_,c17_,s20_,c20_,s21_,c21_,s22_,c22_,s23_,c23_,s24_,c24_
      ;
} sdgstate_t;
#define curtim (sdgstate.curtim_)
#define q (sdgstate.q_)
#define qn (sdgstate.qn_)
#define u (sdgstate.u_)
#define cnk (sdgstate.cnk_)
#define cnb (sdgstate.cnb_)
#define rnkg (sdgstate.rnkg_)
#define rnk (sdgstate.rnk_)
#define rnb (sdgstate.rnb_)
#define vnk (sdgstate.vnk_)
#define vnb (sdgstate.vnb_)
#define wk (sdgstate.wk_)
#define wb (sdgstate.wb_)
#define com (sdgstate.com_)
#define Cik (sdgstate.Cik_)
#define Cib (sdgstate.Cib_)
#define rikt (sdgstate.rikt_)
#define Iko (sdgstate.Iko_)
#define mkrk (sdgstate.mkrk_)
#define Wkk (sdgstate.Wkk_)
#define Vkk (sdgstate.Vkk_)
#define dik (sdgstate.dik_)
#define rpp (sdgstate.rpp_)
#define rpk (sdgstate.rpk_)
#define rik (sdgstate.rik_)
#define rik2 (sdgstate.rik2_)
#define rpri (sdgstate.rpri_)
#define Wik (sdgstate.Wik_)
#define Vik (sdgstate.Vik_)
#define Wirk (sdgstate.Wirk_)
#define rkWkk (sdgstate.rkWkk_)
#define Wkrpk (sdgstate.Wkrpk_)
#define VikWkr (sdgstate.VikWkr_)
#define wbrcom (sdgstate.wbrcom_)
#define perr (sdgstate.perr_)
#define verr (sdgstate.verr_)
#define aerr (sdgstate.aerr_)
#define mult (sdgstate.mult_)
#define ufk (sdgstate.ufk_)
#define utk (sdgstate.utk_)
#define utau (sdgstate.utau_)
#define mfk (sdgstate.mfk_)
#define mtk (sdgstate.mtk_)
#define mtau (sdgstate.mtau_)
#define uacc (sdgstate.uacc_)
#define uvel (sdgstate.uvel_)
#define upos (sdgstate.upos_)
#define s2 (sdgstate.s2_)
#define c2 (sdgstate.c2_)
#define s3 (sdgstate.s3_)
#define c3 (sdgstate.c3_)
#define s6 (sdgstate.s6_)
#define c6 (sdgstate.c6_)
#define s9 (sdgstate.s9_)
#define c9 (sdgstate.c9_)
#define s10 (sdgstate.s10_)
#define c10 (sdgstate.c10_)
#define s11 (sdgstate.s11_)
#define c11 (sdgstate.c11_)
#define s12 (sdgstate.s12_)
#define c12 (sdgstate.c12_)
#define s13 (sdgstate.s13_)
#define c13 (sdgstate.c13_)
#define s14 (sdgstate.s14_)
#define c14 (sdgstate.c14_)
#define s17 (sdgstate.s17_)
#define c17 (sdgstate.c17_)
#define s20 (sdgstate.s20_)
#define c20 (sdgstate.c20_)
#define s21 (sdgstate.s21_)
#define c21 (sdgstate.c21_)
#define s22 (sdgstate.s22_)
#define c22 (sdgstate.c22_)
#define s23 (sdgstate.s23_)
#define c23 (sdgstate.c23_)
#define s24 (sdgstate.s24_)
#define c24 (sdgstate.c24_)

typedef struct {
    double fs0_[25],qdot_[25],Otk_[25][3],Atk_[25][3],AiOiWi_[25][3],Fstar_[25][
      3];
    double Tstar_[25][3],Fstark_[25][3],Tstark_[25][3],IkWk_[25][3],WkIkWk_[25][
      3],gk_[25][3],IkbWk_[15][3],WkIkbWk_[15][3];
    double w0w0_[15],w1w1_[15],w2w2_[15],w0w1_[15],w0w2_[15],w1w2_[15];
    double w00w11_[15],w00w22_[15],w11w22_[15],ww_[12][12],qraux_[12];
    double mm_[25][25],mlo_[25][25],mdi_[25],IkWpk_[25][25][3],works_[25],
      workss_[25][25];
    double Wpk_[25][25][3],Vpk_[25][25][3],VWri_[25][25][3];
    int wmap_[12],multmap_[12],jpvt_[12],wsiz_,wrank_;
} sdglhs_t;
#define qdot (sdglhs.qdot_)
#define Otk (sdglhs.Otk_)
#define Atk (sdglhs.Atk_)
#define AiOiWi (sdglhs.AiOiWi_)
#define Fstar (sdglhs.Fstar_)
#define Tstar (sdglhs.Tstar_)
#define fs0 (sdglhs.fs0_)
#define Fstark (sdglhs.Fstark_)
#define Tstark (sdglhs.Tstark_)
#define IkWk (sdglhs.IkWk_)
#define IkbWk (sdglhs.IkbWk_)
#define WkIkWk (sdglhs.WkIkWk_)
#define WkIkbWk (sdglhs.WkIkbWk_)
#define gk (sdglhs.gk_)
#define w0w0 (sdglhs.w0w0_)
#define w1w1 (sdglhs.w1w1_)
#define w2w2 (sdglhs.w2w2_)
#define w0w1 (sdglhs.w0w1_)
#define w0w2 (sdglhs.w0w2_)
#define w1w2 (sdglhs.w1w2_)
#define w00w11 (sdglhs.w00w11_)
#define w00w22 (sdglhs.w00w22_)
#define w11w22 (sdglhs.w11w22_)
#define ww (sdglhs.ww_)
#define qraux (sdglhs.qraux_)
#define mm (sdglhs.mm_)
#define mlo (sdglhs.mlo_)
#define mdi (sdglhs.mdi_)
#define IkWpk (sdglhs.IkWpk_)
#define works (sdglhs.works_)
#define workss (sdglhs.workss_)
#define Wpk (sdglhs.Wpk_)
#define Vpk (sdglhs.Vpk_)
#define VWri (sdglhs.VWri_)
#define wmap (sdglhs.wmap_)
#define multmap (sdglhs.multmap_)
#define jpvt (sdglhs.jpvt_)
#define wsiz (sdglhs.wsiz_)
#define wrank (sdglhs.wrank_)

typedef struct {
    double fs_[25],udot_[25],tauc_[25],dyad_[15][3][3],fc_[25][3],tc_[25][3];
    double ank_[25][3],onk_[25][3],Onkb_[25][3],AOnkri_[25][3],Ankb_[25][3],
      AnkAtk_[25][3],anb_[15][3],onb_[15][3],dyrcom_[15][3];
    double ffk_[25][3],ttk_[25][3],fccikt_[25][3],ffkb_[15][3],ttkb_[15][3];
} sdgrhs_t;
#define fs (sdgrhs.fs_)
#define udot (sdgrhs.udot_)
#define ank (sdgrhs.ank_)
#define anb (sdgrhs.anb_)
#define onk (sdgrhs.onk_)
#define onb (sdgrhs.onb_)
#define Onkb (sdgrhs.Onkb_)
#define AOnkri (sdgrhs.AOnkri_)
#define Ankb (sdgrhs.Ankb_)
#define AnkAtk (sdgrhs.AnkAtk_)
#define dyrcom (sdgrhs.dyrcom_)
#define ffk (sdgrhs.ffk_)
#define ttk (sdgrhs.ttk_)
#define fccikt (sdgrhs.fccikt_)
#define ffkb (sdgrhs.ffkb_)
#define ttkb (sdgrhs.ttkb_)
#define dyad (sdgrhs.dyad_)
#define fc (sdgrhs.fc_)
#define tc (sdgrhs.tc_)
#define tauc (sdgrhs.tauc_)

typedef struct {
    double temp_[3000],tmat1_[3][3],tmat2_[3][3],tvec1_[3],tvec2_[3],tvec3_[3],
      tvec4_[3],tvec5_[3];
    double tsc1_,tsc2_,tsc3_;
} sdgtemp_t;
#define temp (sdgtemp.temp_)
#define tmat1 (sdgtemp.tmat1_)
#define tmat2 (sdgtemp.tmat2_)
#define tvec1 (sdgtemp.tvec1_)
#define tvec2 (sdgtemp.tvec2_)
#define tvec3 (sdgtemp.tvec3_)
#define tvec4 (sdgtemp.tvec4_)
#define tvec5 (sdgtemp.tvec5_)
#define tsc1 (sdgtemp.tsc1_)
#define tsc2 (sdgtemp.tsc2_)
#define tsc3 (sdgtemp.tsc3_)

sdgtopo_t sdgtopo = {
/*  Topological information
*/
    /* ground */ 1,
    /* nbod */ 15,
    /* ndof */ 25,
    /* ncons */ 12,
    /* nloop */ 0,
    /* nldof */ 0,
    /* nloopc */ 0,
    /* nball */ 0,
    /* nlball */ 0,
    /* npres */ 2,
    /* nuser */ 10,
    /* jtype[0] */ 8,
    /* jtype[1] */ 1,
    /* jtype[2] */ 8,
    /* jtype[3] */ 8,
    /* jtype[4] */ 1,
    /* jtype[5] */ 1,
    /* jtype[6] */ 1,
    /* jtype[7] */ 1,
    /* jtype[8] */ 1,
    /* jtype[9] */ 8,
    /* jtype[10] */ 8,
    /* jtype[11] */ 1,
    /* jtype[12] */ 1,
    /* jtype[13] */ 1,
    /* jtype[14] */ 1,
    /* inb[0] */ -1,
    /* inb[1] */ 0,
    /* inb[2] */ 1,
    /* inb[3] */ 2,
    /* inb[4] */ 2,
    /* inb[5] */ 4,
    /* inb[6] */ 5,
    /* inb[7] */ 6,
    /* inb[8] */ 0,
    /* inb[9] */ 8,
    /* inb[10] */ 9,
    /* inb[11] */ 9,
    /* inb[12] */ 11,
    /* inb[13] */ 12,
    /* inb[14] */ 13,
    /* outb[0] */ 0,
    /* outb[1] */ 1,
    /* outb[2] */ 2,
    /* outb[3] */ 3,
    /* outb[4] */ 4,
    /* outb[5] */ 5,
    /* outb[6] */ 6,
    /* outb[7] */ 7,
    /* outb[8] */ 8,
    /* outb[9] */ 9,
    /* outb[10] */ 10,
    /* outb[11] */ 11,
    /* outb[12] */ 12,
    /* outb[13] */ 13,
    /* outb[14] */ 14,
    /* njntdof[0] */ 3,
    /* njntdof[1] */ 1,
    /* njntdof[2] */ 3,
    /* njntdof[3] */ 3,
    /* njntdof[4] */ 1,
    /* njntdof[5] */ 1,
    /* njntdof[6] */ 1,
    /* njntdof[7] */ 1,
    /* njntdof[8] */ 1,
    /* njntdof[9] */ 3,
    /* njntdof[10] */ 3,
    /* njntdof[11] */ 1,
    /* njntdof[12] */ 1,
    /* njntdof[13] */ 1,
    /* njntdof[14] */ 1,
    /* njntc[0] */ 0,
    /* njntc[1] */ 0,
    /* njntc[2] */ 0,
    /* njntc[3] */ 0,
    /* njntc[4] */ 0,
    /* njntc[5] */ 0,
    /* njntc[6] */ 0,
    /* njntc[7] */ 0,
    /* njntc[8] */ 0,
    /* njntc[9] */ 0,
    /* njntc[10] */ 0,
    /* njntc[11] */ 0,
    /* njntc[12] */ 0,
    /* njntc[13] */ 0,
    /* njntc[14] */ 0,
    /* njntp[0] */ 0,
    /* njntp[1] */ 0,
    /* njntp[2] */ 0,
    /* njntp[3] */ 0,
    /* njntp[4] */ 0,
    /* njntp[5] */ 1,
    /* njntp[6] */ 0,
    /* njntp[7] */ 0,
    /* njntp[8] */ 0,
    /* njntp[9] */ 0,
    /* njntp[10] */ 0,
    /* njntp[11] */ 0,
    /* njntp[12] */ 1,
    /* njntp[13] */ 0,
    /* njntp[14] */ 0,
    /* firstq[0] */ 0,
    /* firstq[1] */ 3,
    /* firstq[2] */ 4,
    /* firstq[3] */ 7,
    /* firstq[4] */ 10,
    /* firstq[5] */ 11,
    /* firstq[6] */ 12,
    /* firstq[7] */ 13,
    /* firstq[8] */ 14,
    /* firstq[9] */ 15,
    /* firstq[10] */ 18,
    /* firstq[11] */ 21,
    /* firstq[12] */ 22,
    /* firstq[13] */ 23,
    /* firstq[14] */ 24,
    /* ballq[0] */ -104,
    /* ballq[1] */ -104,
    /* ballq[2] */ -104,
    /* ballq[3] */ -104,
    /* ballq[4] */ -104,
    /* ballq[5] */ -104,
    /* ballq[6] */ -104,
    /* ballq[7] */ -104,
    /* ballq[8] */ -104,
    /* ballq[9] */ -104,
    /* ballq[10] */ -104,
    /* ballq[11] */ -104,
    /* ballq[12] */ -104,
    /* ballq[13] */ -104,
    /* ballq[14] */ -104,
    /* firstm[0] */ -1,
    /* firstm[1] */ -1,
    /* firstm[2] */ -1,
    /* firstm[3] */ -1,
    /* firstm[4] */ -1,
    /* firstm[5] */ -1,
    /* firstm[6] */ -1,
    /* firstm[7] */ -1,
    /* firstm[8] */ -1,
    /* firstm[9] */ -1,
    /* firstm[10] */ -1,
    /* firstm[11] */ -1,
    /* firstm[12] */ -1,
    /* firstm[13] */ -1,
    /* firstm[14] */ -1,
    /* firstp[0] */ -1,
    /* firstp[1] */ -1,
    /* firstp[2] */ -1,
    /* firstp[3] */ -1,
    /* firstp[4] */ -1,
    /* firstp[5] */ 0,
    /* firstp[6] */ -1,
    /* firstp[7] */ -1,
    /* firstp[8] */ -1,
    /* firstp[9] */ -1,
    /* firstp[10] */ -1,
    /* firstp[11] */ -1,
    /* firstp[12] */ 1,
    /* firstp[13] */ -1,
    /* firstp[14] */ -1,
    /* trans[0] */ 1,
    /* trans[1] */ 1,
    /* trans[2] */ 0,
    /* trans[3] */ 0,
    /* trans[4] */ 1,
    /* trans[5] */ 1,
    /* trans[6] */ 0,
    /* trans[7] */ 1,
    /* trans[8] */ 1,
    /* trans[9] */ 0,
    /* trans[10] */ 0,
    /* trans[11] */ 0,
    /* trans[12] */ 0,
    /* trans[13] */ 0,
    /* trans[14] */ 0,
    /* trans[15] */ 1,
    /* trans[16] */ 1,
    /* trans[17] */ 0,
    /* trans[18] */ 1,
    /* trans[19] */ 1,
    /* trans[20] */ 0,
    /* trans[21] */ 0,
    /* trans[22] */ 0,
    /* trans[23] */ 0,
    /* trans[24] */ 0,
    /* firstu[0] */ 2,
    /* firstu[1] */ 3,
    /* firstu[2] */ 4,
    /* firstu[3] */ 5,
    /* firstu[4] */ 6,
    /* firstu[5] */ 7,
    /* firstu[6] */ 8,
    /* firstu[7] */ 9,
    /* firstu[8] */ 10,
    /* firstu[9] */ 11,
};
sdginput_t sdginput = {
/* Model parameters from the input file */

/* gravity */
    /* grav[0] */ 0.,
    /* grav[1] */ -9.80665,
    /* grav[2] */ 0.,

/* mass */
    /* mk[0] */ 51.1,
    /* mk[1] */ 7.76,
    /* mk[2] */ 3.03,
    /* mk[3] */ .1,
    /* mk[4] */ .1,
    /* mk[5] */ .4183,
    /* mk[6] */ .4,
    /* mk[7] */ .2058,
    /* mk[8] */ 7.76,
    /* mk[9] */ 3.03,
    /* mk[10] */ .1,
    /* mk[11] */ .1,
    /* mk[12] */ .4183,
    /* mk[13] */ .4,
    /* mk[14] */ .2058,

/* inertia */
    /* ik[0][0][0] */ 1.396,
    /* ik[0][0][1] */ 0.,
    /* ik[0][0][2] */ 0.,
    /* ik[0][1][0] */ 0.,
    /* ik[0][1][1] */ .7153,
    /* ik[0][1][2] */ 0.,
    /* ik[0][2][0] */ 0.,
    /* ik[0][2][1] */ 0.,
    /* ik[0][2][2] */ 2.58,
    /* ik[1][0][0] */ .137,
    /* ik[1][0][1] */ 0.,
    /* ik[1][0][2] */ 0.,
    /* ik[1][1][0] */ 0.,
    /* ik[1][1][1] */ .0316,
    /* ik[1][1][2] */ 0.,
    /* ik[1][2][0] */ 0.,
    /* ik[1][2][1] */ 0.,
    /* ik[1][2][2] */ .137,
    /* ik[2][0][0] */ .0444,
    /* ik[2][0][1] */ 0.,
    /* ik[2][0][2] */ 0.,
    /* ik[2][1][0] */ 0.,
    /* ik[2][1][1] */ .00383,
    /* ik[2][1][2] */ 0.,
    /* ik[2][2][0] */ 0.,
    /* ik[2][2][1] */ 0.,
    /* ik[2][2][2] */ .0445,
    /* ik[3][0][0] */ .001,
    /* ik[3][0][1] */ 0.,
    /* ik[3][0][2] */ 0.,
    /* ik[3][1][0] */ 0.,
    /* ik[3][1][1] */ .001,
    /* ik[3][1][2] */ 0.,
    /* ik[3][2][0] */ 0.,
    /* ik[3][2][1] */ 0.,
    /* ik[3][2][2] */ .001,
    /* ik[4][0][0] */ .001,
    /* ik[4][0][1] */ 0.,
    /* ik[4][0][2] */ 0.,
    /* ik[4][1][0] */ 0.,
    /* ik[4][1][1] */ .001,
    /* ik[4][1][2] */ 0.,
    /* ik[4][2][0] */ 0.,
    /* ik[4][2][1] */ 0.,
    /* ik[4][2][2] */ .001,
    /* ik[5][0][0] */ .001,
    /* ik[5][0][1] */ 0.,
    /* ik[5][0][2] */ 0.,
    /* ik[5][1][0] */ 0.,
    /* ik[5][1][1] */ .001,
    /* ik[5][1][2] */ 0.,
    /* ik[5][2][0] */ 0.,
    /* ik[5][2][1] */ 0.,
    /* ik[5][2][2] */ .001,
    /* ik[6][0][0] */ .001,
    /* ik[6][0][1] */ 0.,
    /* ik[6][0][2] */ 0.,
    /* ik[6][1][0] */ 0.,
    /* ik[6][1][1] */ .001,
    /* ik[6][1][2] */ 0.,
    /* ik[6][2][0] */ 0.,
    /* ik[6][2][1] */ 0.,
    /* ik[6][2][2] */ .001,
    /* ik[7][0][0] */ .001,
    /* ik[7][0][1] */ 0.,
    /* ik[7][0][2] */ 0.,
    /* ik[7][1][0] */ 0.,
    /* ik[7][1][1] */ .001,
    /* ik[7][1][2] */ 0.,
    /* ik[7][2][0] */ 0.,
    /* ik[7][2][1] */ 0.,
    /* ik[7][2][2] */ .001,
    /* ik[8][0][0] */ .137,
    /* ik[8][0][1] */ 0.,
    /* ik[8][0][2] */ 0.,
    /* ik[8][1][0] */ 0.,
    /* ik[8][1][1] */ .0316,
    /* ik[8][1][2] */ 0.,
    /* ik[8][2][0] */ 0.,
    /* ik[8][2][1] */ 0.,
    /* ik[8][2][2] */ .137,
    /* ik[9][0][0] */ .0444,
    /* ik[9][0][1] */ 0.,
    /* ik[9][0][2] */ 0.,
    /* ik[9][1][0] */ 0.,
    /* ik[9][1][1] */ .00383,
    /* ik[9][1][2] */ 0.,
    /* ik[9][2][0] */ 0.,
    /* ik[9][2][1] */ 0.,
    /* ik[9][2][2] */ .0445,
    /* ik[10][0][0] */ .001,
    /* ik[10][0][1] */ 0.,
    /* ik[10][0][2] */ 0.,
    /* ik[10][1][0] */ 0.,
    /* ik[10][1][1] */ .001,
    /* ik[10][1][2] */ 0.,
    /* ik[10][2][0] */ 0.,
    /* ik[10][2][1] */ 0.,
    /* ik[10][2][2] */ .001,
    /* ik[11][0][0] */ .001,
    /* ik[11][0][1] */ 0.,
    /* ik[11][0][2] */ 0.,
    /* ik[11][1][0] */ 0.,
    /* ik[11][1][1] */ .001,
    /* ik[11][1][2] */ 0.,
    /* ik[11][2][0] */ 0.,
    /* ik[11][2][1] */ 0.,
    /* ik[11][2][2] */ .001,
    /* ik[12][0][0] */ .001,
    /* ik[12][0][1] */ 0.,
    /* ik[12][0][2] */ 0.,
    /* ik[12][1][0] */ 0.,
    /* ik[12][1][1] */ .001,
    /* ik[12][1][2] */ 0.,
    /* ik[12][2][0] */ 0.,
    /* ik[12][2][1] */ 0.,
    /* ik[12][2][2] */ .001,
    /* ik[13][0][0] */ .001,
    /* ik[13][0][1] */ 0.,
    /* ik[13][0][2] */ 0.,
    /* ik[13][1][0] */ 0.,
    /* ik[13][1][1] */ .001,
    /* ik[13][1][2] */ 0.,
    /* ik[13][2][0] */ 0.,
    /* ik[13][2][1] */ 0.,
    /* ik[13][2][2] */ .001,
    /* ik[14][0][0] */ .001,
    /* ik[14][0][1] */ 0.,
    /* ik[14][0][2] */ 0.,
    /* ik[14][1][0] */ 0.,
    /* ik[14][1][1] */ .001,
    /* ik[14][1][2] */ 0.,
    /* ik[14][2][0] */ 0.,
    /* ik[14][2][1] */ 0.,
    /* ik[14][2][2] */ .001,

/* tree hinge axis vectors */
    /* pin[0][0] */ 1.,
    /* pin[0][1] */ 0.,
    /* pin[0][2] */ 0.,
    /* pin[1][0] */ 0.,
    /* pin[1][1] */ 1.,
    /* pin[1][2] */ 0.,
    /* pin[2][0] */ 0.,
    /* pin[2][1] */ 0.,
    /* pin[2][2] */ 1.,
    /* pin[3][0] */ 0.,
    /* pin[3][1] */ 0.,
    /* pin[3][2] */ 1.,
    /* pin[4][0] */ 1.,
    /* pin[4][1] */ 0.,
    /* pin[4][2] */ 0.,
    /* pin[5][0] */ 0.,
    /* pin[5][1] */ 1.,
    /* pin[5][2] */ 0.,
    /* pin[6][0] */ 0.,
    /* pin[6][1] */ 0.,
    /* pin[6][2] */ 1.,
    /* pin[7][0] */ 1.,
    /* pin[7][1] */ 0.,
    /* pin[7][2] */ 0.,
    /* pin[8][0] */ 0.,
    /* pin[8][1] */ 1.,
    /* pin[8][2] */ 0.,
    /* pin[9][0] */ 0.,
    /* pin[9][1] */ 0.,
    /* pin[9][2] */ 1.,
    /* pin[10][0] */ 0.,
    /* pin[10][1] */ 0.,
    /* pin[10][2] */ 1.,
    /* pin[11][0] */ 1.,
    /* pin[11][1] */ 0.,
    /* pin[11][2] */ 0.,
    /* pin[12][0] */ 0.,
    /* pin[12][1] */ 0.,
    /* pin[12][2] */ 1.,
    /* pin[13][0] */ 0.,
    /* pin[13][1] */ 0.,
    /* pin[13][2] */ 1.,
    /* pin[14][0] */ 0.,
    /* pin[14][1] */ 0.,
    /* pin[14][2] */ 1.,
    /* pin[15][0] */ 1.,
    /* pin[15][1] */ 0.,
    /* pin[15][2] */ 0.,
    /* pin[16][0] */ 0.,
    /* pin[16][1] */ 1.,
    /* pin[16][2] */ 0.,
    /* pin[17][0] */ 0.,
    /* pin[17][1] */ 0.,
    /* pin[17][2] */ 1.,
    /* pin[18][0] */ 1.,
    /* pin[18][1] */ 0.,
    /* pin[18][2] */ 0.,
    /* pin[19][0] */ 0.,
    /* pin[19][1] */ 1.,
    /* pin[19][2] */ 0.,
    /* pin[20][0] */ 0.,
    /* pin[20][1] */ 0.,
    /* pin[20][2] */ 1.,
    /* pin[21][0] */ 0.,
    /* pin[21][1] */ 0.,
    /* pin[21][2] */ 1.,
    /* pin[22][0] */ 1.,
    /* pin[22][1] */ 0.,
    /* pin[22][2] */ 0.,
    /* pin[23][0] */ 0.,
    /* pin[23][1] */ 0.,
    /* pin[23][2] */ 1.,
    /* pin[24][0] */ 0.,
    /* pin[24][1] */ 0.,
    /* pin[24][2] */ 1.,

/* tree bodytojoint vectors */
    /* rk[0][0] */ .0707,
    /* rk[0][1] */ -.2639,
    /* rk[0][2] */ 0.,
    /* rk[1][0] */ 0.,
    /* rk[1][1] */ .17,
    /* rk[1][2] */ 0.,
    /* rk[2][0] */ 0.,
    /* rk[2][1] */ .1867,
    /* rk[2][2] */ 0.,
    /* rk[3][0] */ 0.,
    /* rk[3][1] */ 0.,
    /* rk[3][2] */ 0.,
    /* rk[4][0] */ 0.,
    /* rk[4][1] */ 0.,
    /* rk[4][2] */ 0.,
    /* rk[5][0] */ -.025,
    /* rk[5][1] */ -.015,
    /* rk[5][2] */ 0.,
    /* rk[6][0] */ -.025,
    /* rk[6][1] */ -.015,
    /* rk[6][2] */ 0.,
    /* rk[7][0] */ -.0346,
    /* rk[7][1] */ -.006,
    /* rk[7][2] */ .0175,
    /* rk[8][0] */ 0.,
    /* rk[8][1] */ .17,
    /* rk[8][2] */ 0.,
    /* rk[9][0] */ 0.,
    /* rk[9][1] */ .1867,
    /* rk[9][2] */ 0.,
    /* rk[10][0] */ 0.,
    /* rk[10][1] */ 0.,
    /* rk[10][2] */ 0.,
    /* rk[11][0] */ 0.,
    /* rk[11][1] */ 0.,
    /* rk[11][2] */ 0.,
    /* rk[12][0] */ -.025,
    /* rk[12][1] */ -.015,
    /* rk[12][2] */ 0.,
    /* rk[13][0] */ -.025,
    /* rk[13][1] */ -.015,
    /* rk[13][2] */ 0.,
    /* rk[14][0] */ -.0346,
    /* rk[14][1] */ -.006,
    /* rk[14][2] */ -.0175,

/* tree inbtojoint vectors */
    /* ri[0][0] */ 0.,
    /* ri[0][1] */ 0.,
    /* ri[0][2] */ 0.,
    /* ri[1][0] */ 0.,
    /* ri[1][1] */ -.33,
    /* ri[1][2] */ .0835,
    /* ri[2][0] */ 0.,
    /* ri[2][1] */ .17,
    /* ri[2][2] */ 0.,
    /* ri[3][0] */ 0.,
    /* ri[3][1] */ .1867,
    /* ri[3][2] */ .0024,
    /* ri[4][0] */ 0.,
    /* ri[4][1] */ -.2433,
    /* ri[4][2] */ 0.,
    /* ri[5][0] */ -.04877,
    /* ri[5][1] */ -.04195,
    /* ri[5][2] */ .00792,
    /* ri[6][0] */ .0942,
    /* ri[6][1] */ -.015,
    /* ri[6][2] */ 0.,
    /* ri[7][0] */ .0346,
    /* ri[7][1] */ -.017,
    /* ri[7][2] */ .00108,
    /* ri[8][0] */ 0.,
    /* ri[8][1] */ -.33,
    /* ri[8][2] */ -.0835,
    /* ri[9][0] */ 0.,
    /* ri[9][1] */ .17,
    /* ri[9][2] */ 0.,
    /* ri[10][0] */ 0.,
    /* ri[10][1] */ .1867,
    /* ri[10][2] */ -.0024,
    /* ri[11][0] */ 0.,
    /* ri[11][1] */ -.2433,
    /* ri[11][2] */ 0.,
    /* ri[12][0] */ -.04877,
    /* ri[12][1] */ -.04195,
    /* ri[12][2] */ -.00792,
    /* ri[13][0] */ .0942,
    /* ri[13][1] */ -.015,
    /* ri[13][2] */ 0.,
    /* ri[14][0] */ .0346,
    /* ri[14][1] */ -.017,
    /* ri[14][2] */ -.00108,

/* tree prescribed motion */
    /* pres[0] */ 0.,
    /* pres[1] */ 0.,
    /* pres[2] */ 0.,
    /* pres[3] */ 0.,
    /* pres[4] */ 0.,
    /* pres[5] */ 0.,
    /* pres[6] */ 0.,
    /* pres[7] */ 0.,
    /* pres[8] */ 0.,
    /* pres[9] */ 0.,
    /* pres[10] */ 0.,
    /* pres[11] */ 1.,
    /* pres[12] */ 0.,
    /* pres[13] */ 0.,
    /* pres[14] */ 0.,
    /* pres[15] */ 0.,
    /* pres[16] */ 0.,
    /* pres[17] */ 0.,
    /* pres[18] */ 0.,
    /* pres[19] */ 0.,
    /* pres[20] */ 0.,
    /* pres[21] */ 0.,
    /* pres[22] */ 1.,
    /* pres[23] */ 0.,
    /* pres[24] */ 0.,

/* stabilization parameters */
    /* stabvel */ 0.,
    /* stabpos */ 0.,

/* miscellaneous */
    /* mfrcflg */ 0,
    /* roustate */ 0,
    /* vpkflg */ 0,
    /* inerflg */ 0,
    /* mmflg */ 0,
    /* mmlduflg */ 0,
    /* wwflg */ 0,
    /* ltauflg */ 0,
    /* fs0flg */ 0,
    /* ii */ 0,
    /* mmap[0] */ 0,
    /* mmap[1] */ 1,
    /* mmap[2] */ 2,
    /* mmap[3] */ 3,
    /* mmap[4] */ 4,
    /* mmap[5] */ 5,
    /* mmap[6] */ 6,
    /* mmap[7] */ 7,
    /* mmap[8] */ 8,
    /* mmap[9] */ 9,
    /* mmap[10] */ 10,
    /* mmap[11] */ 11,
    /* mmap[12] */ 12,
    /* mmap[13] */ 13,
    /* mmap[14] */ 14,
    /* mmap[15] */ 15,
    /* mmap[16] */ 16,
    /* mmap[17] */ 17,
    /* mmap[18] */ 18,
    /* mmap[19] */ 19,
    /* mmap[20] */ 20,
    /* mmap[21] */ 21,
    /* mmap[22] */ 22,
    /* mmap[23] */ 23,
    /* mmap[24] */ 24,

/* Which parameters were "?" (1) or "<nominal>?" (3) */
    /* gravq[0] */ 0,
    /* gravq[1] */ 0,
    /* gravq[2] */ 0,
    /* mkq[0] */ 0,
    /* mkq[1] */ 0,
    /* mkq[2] */ 0,
    /* mkq[3] */ 0,
    /* mkq[4] */ 0,
    /* mkq[5] */ 0,
    /* mkq[6] */ 0,
    /* mkq[7] */ 0,
    /* mkq[8] */ 0,
    /* mkq[9] */ 0,
    /* mkq[10] */ 0,
    /* mkq[11] */ 0,
    /* mkq[12] */ 0,
    /* mkq[13] */ 0,
    /* mkq[14] */ 0,
    /* ikq[0][0][0] */ 0,
    /* ikq[0][0][1] */ 0,
    /* ikq[0][0][2] */ 0,
    /* ikq[0][1][0] */ 0,
    /* ikq[0][1][1] */ 0,
    /* ikq[0][1][2] */ 0,
    /* ikq[0][2][0] */ 0,
    /* ikq[0][2][1] */ 0,
    /* ikq[0][2][2] */ 0,
    /* ikq[1][0][0] */ 0,
    /* ikq[1][0][1] */ 0,
    /* ikq[1][0][2] */ 0,
    /* ikq[1][1][0] */ 0,
    /* ikq[1][1][1] */ 0,
    /* ikq[1][1][2] */ 0,
    /* ikq[1][2][0] */ 0,
    /* ikq[1][2][1] */ 0,
    /* ikq[1][2][2] */ 0,
    /* ikq[2][0][0] */ 0,
    /* ikq[2][0][1] */ 0,
    /* ikq[2][0][2] */ 0,
    /* ikq[2][1][0] */ 0,
    /* ikq[2][1][1] */ 0,
    /* ikq[2][1][2] */ 0,
    /* ikq[2][2][0] */ 0,
    /* ikq[2][2][1] */ 0,
    /* ikq[2][2][2] */ 0,
    /* ikq[3][0][0] */ 0,
    /* ikq[3][0][1] */ 0,
    /* ikq[3][0][2] */ 0,
    /* ikq[3][1][0] */ 0,
    /* ikq[3][1][1] */ 0,
    /* ikq[3][1][2] */ 0,
    /* ikq[3][2][0] */ 0,
    /* ikq[3][2][1] */ 0,
    /* ikq[3][2][2] */ 0,
    /* ikq[4][0][0] */ 0,
    /* ikq[4][0][1] */ 0,
    /* ikq[4][0][2] */ 0,
    /* ikq[4][1][0] */ 0,
    /* ikq[4][1][1] */ 0,
    /* ikq[4][1][2] */ 0,
    /* ikq[4][2][0] */ 0,
    /* ikq[4][2][1] */ 0,
    /* ikq[4][2][2] */ 0,
    /* ikq[5][0][0] */ 0,
    /* ikq[5][0][1] */ 0,
    /* ikq[5][0][2] */ 0,
    /* ikq[5][1][0] */ 0,
    /* ikq[5][1][1] */ 0,
    /* ikq[5][1][2] */ 0,
    /* ikq[5][2][0] */ 0,
    /* ikq[5][2][1] */ 0,
    /* ikq[5][2][2] */ 0,
    /* ikq[6][0][0] */ 0,
    /* ikq[6][0][1] */ 0,
    /* ikq[6][0][2] */ 0,
    /* ikq[6][1][0] */ 0,
    /* ikq[6][1][1] */ 0,
    /* ikq[6][1][2] */ 0,
    /* ikq[6][2][0] */ 0,
    /* ikq[6][2][1] */ 0,
    /* ikq[6][2][2] */ 0,
    /* ikq[7][0][0] */ 0,
    /* ikq[7][0][1] */ 0,
    /* ikq[7][0][2] */ 0,
    /* ikq[7][1][0] */ 0,
    /* ikq[7][1][1] */ 0,
    /* ikq[7][1][2] */ 0,
    /* ikq[7][2][0] */ 0,
    /* ikq[7][2][1] */ 0,
    /* ikq[7][2][2] */ 0,
    /* ikq[8][0][0] */ 0,
    /* ikq[8][0][1] */ 0,
    /* ikq[8][0][2] */ 0,
    /* ikq[8][1][0] */ 0,
    /* ikq[8][1][1] */ 0,
    /* ikq[8][1][2] */ 0,
    /* ikq[8][2][0] */ 0,
    /* ikq[8][2][1] */ 0,
    /* ikq[8][2][2] */ 0,
    /* ikq[9][0][0] */ 0,
    /* ikq[9][0][1] */ 0,
    /* ikq[9][0][2] */ 0,
    /* ikq[9][1][0] */ 0,
    /* ikq[9][1][1] */ 0,
    /* ikq[9][1][2] */ 0,
    /* ikq[9][2][0] */ 0,
    /* ikq[9][2][1] */ 0,
    /* ikq[9][2][2] */ 0,
    /* ikq[10][0][0] */ 0,
    /* ikq[10][0][1] */ 0,
    /* ikq[10][0][2] */ 0,
    /* ikq[10][1][0] */ 0,
    /* ikq[10][1][1] */ 0,
    /* ikq[10][1][2] */ 0,
    /* ikq[10][2][0] */ 0,
    /* ikq[10][2][1] */ 0,
    /* ikq[10][2][2] */ 0,
    /* ikq[11][0][0] */ 0,
    /* ikq[11][0][1] */ 0,
    /* ikq[11][0][2] */ 0,
    /* ikq[11][1][0] */ 0,
    /* ikq[11][1][1] */ 0,
    /* ikq[11][1][2] */ 0,
    /* ikq[11][2][0] */ 0,
    /* ikq[11][2][1] */ 0,
    /* ikq[11][2][2] */ 0,
    /* ikq[12][0][0] */ 0,
    /* ikq[12][0][1] */ 0,
    /* ikq[12][0][2] */ 0,
    /* ikq[12][1][0] */ 0,
    /* ikq[12][1][1] */ 0,
    /* ikq[12][1][2] */ 0,
    /* ikq[12][2][0] */ 0,
    /* ikq[12][2][1] */ 0,
    /* ikq[12][2][2] */ 0,
    /* ikq[13][0][0] */ 0,
    /* ikq[13][0][1] */ 0,
    /* ikq[13][0][2] */ 0,
    /* ikq[13][1][0] */ 0,
    /* ikq[13][1][1] */ 0,
    /* ikq[13][1][2] */ 0,
    /* ikq[13][2][0] */ 0,
    /* ikq[13][2][1] */ 0,
    /* ikq[13][2][2] */ 0,
    /* ikq[14][0][0] */ 0,
    /* ikq[14][0][1] */ 0,
    /* ikq[14][0][2] */ 0,
    /* ikq[14][1][0] */ 0,
    /* ikq[14][1][1] */ 0,
    /* ikq[14][1][2] */ 0,
    /* ikq[14][2][0] */ 0,
    /* ikq[14][2][1] */ 0,
    /* ikq[14][2][2] */ 0,
    /* pinq[0][0] */ 0,
    /* pinq[0][1] */ 0,
    /* pinq[0][2] */ 0,
    /* pinq[1][0] */ 0,
    /* pinq[1][1] */ 0,
    /* pinq[1][2] */ 0,
    /* pinq[2][0] */ 0,
    /* pinq[2][1] */ 0,
    /* pinq[2][2] */ 0,
    /* pinq[3][0] */ 0,
    /* pinq[3][1] */ 0,
    /* pinq[3][2] */ 0,
    /* pinq[4][0] */ 0,
    /* pinq[4][1] */ 0,
    /* pinq[4][2] */ 0,
    /* pinq[5][0] */ 0,
    /* pinq[5][1] */ 0,
    /* pinq[5][2] */ 0,
    /* pinq[6][0] */ 0,
    /* pinq[6][1] */ 0,
    /* pinq[6][2] */ 0,
    /* pinq[7][0] */ 0,
    /* pinq[7][1] */ 0,
    /* pinq[7][2] */ 0,
    /* pinq[8][0] */ 0,
    /* pinq[8][1] */ 0,
    /* pinq[8][2] */ 0,
    /* pinq[9][0] */ 0,
    /* pinq[9][1] */ 0,
    /* pinq[9][2] */ 0,
    /* pinq[10][0] */ 0,
    /* pinq[10][1] */ 0,
    /* pinq[10][2] */ 0,
    /* pinq[11][0] */ 0,
    /* pinq[11][1] */ 0,
    /* pinq[11][2] */ 0,
    /* pinq[12][0] */ 0,
    /* pinq[12][1] */ 0,
    /* pinq[12][2] */ 0,
    /* pinq[13][0] */ 0,
    /* pinq[13][1] */ 0,
    /* pinq[13][2] */ 0,
    /* pinq[14][0] */ 0,
    /* pinq[14][1] */ 0,
    /* pinq[14][2] */ 0,
    /* pinq[15][0] */ 0,
    /* pinq[15][1] */ 0,
    /* pinq[15][2] */ 0,
    /* pinq[16][0] */ 0,
    /* pinq[16][1] */ 0,
    /* pinq[16][2] */ 0,
    /* pinq[17][0] */ 0,
    /* pinq[17][1] */ 0,
    /* pinq[17][2] */ 0,
    /* pinq[18][0] */ 0,
    /* pinq[18][1] */ 0,
    /* pinq[18][2] */ 0,
    /* pinq[19][0] */ 0,
    /* pinq[19][1] */ 0,
    /* pinq[19][2] */ 0,
    /* pinq[20][0] */ 0,
    /* pinq[20][1] */ 0,
    /* pinq[20][2] */ 0,
    /* pinq[21][0] */ 0,
    /* pinq[21][1] */ 0,
    /* pinq[21][2] */ 0,
    /* pinq[22][0] */ 0,
    /* pinq[22][1] */ 0,
    /* pinq[22][2] */ 0,
    /* pinq[23][0] */ 0,
    /* pinq[23][1] */ 0,
    /* pinq[23][2] */ 0,
    /* pinq[24][0] */ 0,
    /* pinq[24][1] */ 0,
    /* pinq[24][2] */ 0,
    /* rkq[0][0] */ 0,
    /* rkq[0][1] */ 0,
    /* rkq[0][2] */ 0,
    /* rkq[1][0] */ 0,
    /* rkq[1][1] */ 0,
    /* rkq[1][2] */ 0,
    /* rkq[2][0] */ 0,
    /* rkq[2][1] */ 0,
    /* rkq[2][2] */ 0,
    /* rkq[3][0] */ 0,
    /* rkq[3][1] */ 0,
    /* rkq[3][2] */ 0,
    /* rkq[4][0] */ 0,
    /* rkq[4][1] */ 0,
    /* rkq[4][2] */ 0,
    /* rkq[5][0] */ 0,
    /* rkq[5][1] */ 0,
    /* rkq[5][2] */ 0,
    /* rkq[6][0] */ 0,
    /* rkq[6][1] */ 0,
    /* rkq[6][2] */ 0,
    /* rkq[7][0] */ 0,
    /* rkq[7][1] */ 0,
    /* rkq[7][2] */ 0,
    /* rkq[8][0] */ 0,
    /* rkq[8][1] */ 0,
    /* rkq[8][2] */ 0,
    /* rkq[9][0] */ 0,
    /* rkq[9][1] */ 0,
    /* rkq[9][2] */ 0,
    /* rkq[10][0] */ 0,
    /* rkq[10][1] */ 0,
    /* rkq[10][2] */ 0,
    /* rkq[11][0] */ 0,
    /* rkq[11][1] */ 0,
    /* rkq[11][2] */ 0,
    /* rkq[12][0] */ 0,
    /* rkq[12][1] */ 0,
    /* rkq[12][2] */ 0,
    /* rkq[13][0] */ 0,
    /* rkq[13][1] */ 0,
    /* rkq[13][2] */ 0,
    /* rkq[14][0] */ 0,
    /* rkq[14][1] */ 0,
    /* rkq[14][2] */ 0,
    /* riq[0][0] */ 0,
    /* riq[0][1] */ 0,
    /* riq[0][2] */ 0,
    /* riq[1][0] */ 0,
    /* riq[1][1] */ 0,
    /* riq[1][2] */ 0,
    /* riq[2][0] */ 0,
    /* riq[2][1] */ 0,
    /* riq[2][2] */ 0,
    /* riq[3][0] */ 0,
    /* riq[3][1] */ 0,
    /* riq[3][2] */ 0,
    /* riq[4][0] */ 0,
    /* riq[4][1] */ 0,
    /* riq[4][2] */ 0,
    /* riq[5][0] */ 0,
    /* riq[5][1] */ 0,
    /* riq[5][2] */ 0,
    /* riq[6][0] */ 0,
    /* riq[6][1] */ 0,
    /* riq[6][2] */ 0,
    /* riq[7][0] */ 0,
    /* riq[7][1] */ 0,
    /* riq[7][2] */ 0,
    /* riq[8][0] */ 0,
    /* riq[8][1] */ 0,
    /* riq[8][2] */ 0,
    /* riq[9][0] */ 0,
    /* riq[9][1] */ 0,
    /* riq[9][2] */ 0,
    /* riq[10][0] */ 0,
    /* riq[10][1] */ 0,
    /* riq[10][2] */ 0,
    /* riq[11][0] */ 0,
    /* riq[11][1] */ 0,
    /* riq[11][2] */ 0,
    /* riq[12][0] */ 0,
    /* riq[12][1] */ 0,
    /* riq[12][2] */ 0,
    /* riq[13][0] */ 0,
    /* riq[13][1] */ 0,
    /* riq[13][2] */ 0,
    /* riq[14][0] */ 0,
    /* riq[14][1] */ 0,
    /* riq[14][2] */ 0,
    /* presq[0] */ 0,
    /* presq[1] */ 0,
    /* presq[2] */ 0,
    /* presq[3] */ 0,
    /* presq[4] */ 0,
    /* presq[5] */ 0,
    /* presq[6] */ 0,
    /* presq[7] */ 0,
    /* presq[8] */ 0,
    /* presq[9] */ 0,
    /* presq[10] */ 0,
    /* presq[11] */ 0,
    /* presq[12] */ 0,
    /* presq[13] */ 0,
    /* presq[14] */ 0,
    /* presq[15] */ 0,
    /* presq[16] */ 0,
    /* presq[17] */ 0,
    /* presq[18] */ 0,
    /* presq[19] */ 0,
    /* presq[20] */ 0,
    /* presq[21] */ 0,
    /* presq[22] */ 0,
    /* presq[23] */ 0,
    /* presq[24] */ 0,
    /* stabvelq */ 3,
    /* stabposq */ 3,

/* End of values from input file */

};
sdgstate_t sdgstate;
sdglhs_t sdglhs;
sdgrhs_t sdgrhs;
sdgtemp_t sdgtemp;


void sdinit(void)
{
/*
Initialization routine


 This routine must be called before the first call to sdstate(), after
 supplying values for any `?' parameters in the input.
*/
    double sumsq,norminv;
    int i,j,k;


/* Check that all `?' parameters have been assigned values */

    for (k = 0; k < 3; k++) {
        if (gravq[k] == 1) {
            sdseterr(7,25);
        }
    }
    for (k = 0; k < 15; k++) {
        if (mkq[k] == 1) {
            sdseterr(7,26);
        }
        for (i = 0; i < 3; i++) {
            if (rkq[k][i] == 1) {
                sdseterr(7,29);
            }
            if (riq[k][i] == 1) {
                sdseterr(7,30);
            }
            for (j = 0; j < 3; j++) {
                if (ikq[k][i][j] == 1) {
                    sdseterr(7,27);
                }
            }
        }
    }
    for (k = 0; k < 25; k++) {
        for (i = 0; i < 3; i++) {
            if (pinq[k][i] == 1) {
                sdseterr(7,28);
            }
        }
    }

/* Normalize pin vectors if necessary */


/* Zero out Vpk and Wpk */

    for (i = 0; i < 25; i++) {
        for (j = i; j <= 24; j++) {
            for (k = 0; k < 3; k++) {
                Vpk[i][j][k] = 0.;
                Wpk[i][j][k] = 0.;
            }
        }
    }

/* Compute pseudobody-related constants */

    rcom[0][0] = 0.;
    rcom[0][1] = 0.;
    rcom[0][2] = 0.;
    rcom[1][0] = 0.;
    rcom[1][1] = 0.;
    rcom[1][2] = 0.;
    rcom[2][0] = 0.;
    rcom[2][1] = 0.;
    rcom[2][2] = 0.;
    rcom[3][0] = 0.;
    rcom[3][1] = 0.;
    rcom[3][2] = 0.;
    rcom[4][0] = 0.;
    rcom[4][1] = 0.;
    rcom[4][2] = 0.;
    rcom[5][0] = 0.;
    rcom[5][1] = 0.;
    rcom[5][2] = 0.;
    rcom[6][0] = 0.;
    rcom[6][1] = 0.;
    rcom[6][2] = 0.;
    rcom[7][0] = 0.;
    rcom[7][1] = 0.;
    rcom[7][2] = 0.;
    rcom[8][0] = 0.;
    rcom[8][1] = 0.;
    rcom[8][2] = 0.;
    rcom[9][0] = 0.;
    rcom[9][1] = 0.;
    rcom[9][2] = 0.;
    rcom[10][0] = 0.;
    rcom[10][1] = 0.;
    rcom[10][2] = 0.;
    rcom[11][0] = 0.;
    rcom[11][1] = 0.;
    rcom[11][2] = 0.;
    rcom[12][0] = 0.;
    rcom[12][1] = 0.;
    rcom[12][2] = 0.;
    rcom[13][0] = 0.;
    rcom[13][1] = 0.;
    rcom[13][2] = 0.;
    rcom[14][0] = 0.;
    rcom[14][1] = 0.;
    rcom[14][2] = 0.;

/* Compute mass properties-related constants */

    mtot = 75.1282;
    sdserialno(&i);
    if (i != 30123) {
        sdseterr(7,41);
    }
    roustate = 1;
}

/* Convert state to form using 1-2-3 Euler angles for ball joints. */

void sdst2ang(double st[25],
    double stang[25])
{
    int i;

    for (i = 0; i < 25; i++) {
        stang[i] = st[i];
    }
}

/* Convert 1-2-3 form of state back to Euler parameters for ball joints. */

void sdang2st(double stang[25],
    double st[25])
{
    int i;

    for (i = 0; i < 25; i++) {
        st[i] = stang[i];
    }
}

/* Normalize Euler parameters in state. */

void sdnrmsterr(double st[25],
    double normst[25],
    int routine)
{
    int i;

    for (i = 0; i < 25; i++) {
        normst[i] = st[i];
    }
}

void sdnormst(double st[25],
    double normst[25])
{

    sdnrmsterr(st,normst,0);
}

void sdstate(double timein,
    double qin[25],
    double uin[25])
{
/*
Compute kinematic information and store it in sdgstate.

Generated 22-Jun-2007 09:35:40 by SD/FAST, Kane's formulation
(sdfast B.2.8 #30123) on machine ID unknown
Copyright (c) 1990-1997 Symbolic Dynamics, Inc.
Copyright (c) 1990-1997 Parametric Technology Corp.
RESTRICTED RIGHTS LEGEND: Use, duplication, or disclosure by the U.S.
Government is subject to restrictions as set forth in subparagraph
(c)(1)(ii) of the Rights in Technical Data and Computer Software
clause at DFARS 52.227-7013 and similar clauses in the FAR and NASA
FAR Supplement.  Symbolic Dynamics, Inc., Mountain View, CA 94041
*/
    int i,j,qchg,uchg;

    if ((roustate != 1) && (roustate != 2) && (roustate != 3)) {
        sdseterr(8,22);
        return;
    }
    if (roustate == 1) {
        for (i = 0; i < 25; i++) {
            if (presq[i] == 1) {
                sdseterr(8,31);
            }
        }
    }
/*
See if time or any qs have changed since last call
*/
    if ((roustate != 1) && (timein == curtim)) {
        qchg = 0;
        for (i = 0; i < 25; i++) {
            if (qin[i] != q[i]) {
                qchg = 1;
                break;
            }
        }
    } else {
        qchg = 1;
    }
/*
If time and qs are unchanged, check us
*/
    if (qchg == 0) {
        uchg = 0;
        for (i = 0; i < 25; i++) {
            if (uin[i] != u[i]) {
                uchg = 1;
                break;
            }
        }
    } else {
        uchg = 1;
    }
    curtim = timein;
    roustate = 2;
    if (qchg == 0) {
        goto skipqs;
    }
/*
Position-related variables need to be computed
*/
    vpkflg = 0;
    mmflg = 0;
    mmlduflg = 0;
    wwflg = 0;
    for (i = 0; i < 25; i++) {
        q[i] = qin[i];
    }
/*
Compute sines and cosines of q
*/
    s2 = sin(q[2]);
    c2 = cos(q[2]);
    s3 = sin(q[3]);
    c3 = cos(q[3]);
    s6 = sin(q[6]);
    c6 = cos(q[6]);
    s9 = sin(q[9]);
    c9 = cos(q[9]);
    s10 = sin(q[10]);
    c10 = cos(q[10]);
    s11 = sin(q[11]);
    c11 = cos(q[11]);
    s12 = sin(q[12]);
    c12 = cos(q[12]);
    s13 = sin(q[13]);
    c13 = cos(q[13]);
    s14 = sin(q[14]);
    c14 = cos(q[14]);
    s17 = sin(q[17]);
    c17 = cos(q[17]);
    s20 = sin(q[20]);
    c20 = cos(q[20]);
    s21 = sin(q[21]);
    c21 = cos(q[21]);
    s22 = sin(q[22]);
    c22 = cos(q[22]);
    s23 = sin(q[23]);
    c23 = cos(q[23]);
    s24 = sin(q[24]);
    c24 = cos(q[24]);
/*
Compute across-axis direction cosines Cik
*/
/*
Compute across-joint direction cosines Cib
*/
/*
Compute gravity
*/
    gk[3][0] = -(9.80665*((s2*c3)+(s3*c2)));
    gk[3][1] = (9.80665*((s2*s3)-(c2*c3)));
    gk[6][0] = ((gk[3][0]*c6)+(gk[3][1]*s6));
    gk[6][1] = ((gk[3][1]*c6)-(gk[3][0]*s6));
    gk[9][0] = ((gk[6][0]*c9)+(gk[6][1]*s9));
    gk[9][1] = ((gk[6][1]*c9)-(gk[6][0]*s9));
    gk[10][0] = ((gk[6][0]*c10)+(gk[6][1]*s10));
    gk[10][1] = ((gk[6][1]*c10)-(gk[6][0]*s10));
    gk[11][1] = (gk[10][1]*c11);
    gk[11][2] = -(gk[10][1]*s11);
    gk[12][0] = ((gk[10][0]*c12)+(gk[11][1]*s12));
    gk[12][1] = ((gk[11][1]*c12)-(gk[10][0]*s12));
    gk[13][0] = ((gk[12][0]*c13)+(gk[12][1]*s13));
    gk[13][1] = ((gk[12][1]*c13)-(gk[12][0]*s13));
    gk[14][0] = -(9.80665*((s2*c14)+(s14*c2)));
    gk[14][1] = (9.80665*((s2*s14)-(c2*c14)));
    gk[17][0] = ((gk[14][0]*c17)+(gk[14][1]*s17));
    gk[17][1] = ((gk[14][1]*c17)-(gk[14][0]*s17));
    gk[20][0] = ((gk[17][0]*c20)+(gk[17][1]*s20));
    gk[20][1] = ((gk[17][1]*c20)-(gk[17][0]*s20));
    gk[21][0] = ((gk[17][0]*c21)+(gk[17][1]*s21));
    gk[21][1] = ((gk[17][1]*c21)-(gk[17][0]*s21));
    gk[22][1] = (gk[21][1]*c22);
    gk[22][2] = -(gk[21][1]*s22);
    gk[23][0] = ((gk[21][0]*c23)+(gk[22][1]*s23));
    gk[23][1] = ((gk[22][1]*c23)-(gk[21][0]*s23));
    gk[24][0] = ((gk[23][0]*c24)+(gk[23][1]*s24));
    gk[24][1] = ((gk[23][1]*c24)-(gk[23][0]*s24));
/*
Compute cnk & cnb (direction cosines in N)
*/
    cnk[3][0][0] = ((c2*c3)-(s2*s3));
    cnk[3][0][1] = -((s2*c3)+(s3*c2));
    cnk[3][1][0] = ((s2*c3)+(s3*c2));
    cnk[3][1][1] = ((c2*c3)-(s2*s3));
    cnk[6][0][0] = ((cnk[3][0][0]*c6)+(cnk[3][0][1]*s6));
    cnk[6][0][1] = ((cnk[3][0][1]*c6)-(cnk[3][0][0]*s6));
    cnk[6][1][0] = ((cnk[3][1][0]*c6)+(cnk[3][1][1]*s6));
    cnk[6][1][1] = ((cnk[3][1][1]*c6)-(cnk[3][1][0]*s6));
    cnk[9][0][0] = ((cnk[6][0][0]*c9)+(cnk[6][0][1]*s9));
    cnk[9][0][1] = ((cnk[6][0][1]*c9)-(cnk[6][0][0]*s9));
    cnk[9][1][0] = ((cnk[6][1][0]*c9)+(cnk[6][1][1]*s9));
    cnk[9][1][1] = ((cnk[6][1][1]*c9)-(cnk[6][1][0]*s9));
    cnk[10][0][0] = ((cnk[6][0][0]*c10)+(cnk[6][0][1]*s10));
    cnk[10][0][1] = ((cnk[6][0][1]*c10)-(cnk[6][0][0]*s10));
    cnk[10][1][0] = ((cnk[6][1][0]*c10)+(cnk[6][1][1]*s10));
    cnk[10][1][1] = ((cnk[6][1][1]*c10)-(cnk[6][1][0]*s10));
    cnk[11][0][1] = (cnk[10][0][1]*c11);
    cnk[11][0][2] = -(cnk[10][0][1]*s11);
    cnk[11][1][1] = (cnk[10][1][1]*c11);
    cnk[11][1][2] = -(cnk[10][1][1]*s11);
    cnk[12][0][0] = ((cnk[10][0][0]*c12)+(cnk[11][0][1]*s12));
    cnk[12][0][1] = ((cnk[11][0][1]*c12)-(cnk[10][0][0]*s12));
    cnk[12][1][0] = ((cnk[10][1][0]*c12)+(cnk[11][1][1]*s12));
    cnk[12][1][1] = ((cnk[11][1][1]*c12)-(cnk[10][1][0]*s12));
    cnk[12][2][0] = (s11*s12);
    cnk[12][2][1] = (s11*c12);
    cnk[13][0][0] = ((cnk[12][0][0]*c13)+(cnk[12][0][1]*s13));
    cnk[13][0][1] = ((cnk[12][0][1]*c13)-(cnk[12][0][0]*s13));
    cnk[13][1][0] = ((cnk[12][1][0]*c13)+(cnk[12][1][1]*s13));
    cnk[13][1][1] = ((cnk[12][1][1]*c13)-(cnk[12][1][0]*s13));
    cnk[13][2][0] = ((cnk[12][2][0]*c13)+(cnk[12][2][1]*s13));
    cnk[13][2][1] = ((cnk[12][2][1]*c13)-(cnk[12][2][0]*s13));
    cnk[14][0][0] = ((c2*c14)-(s2*s14));
    cnk[14][0][1] = -((s2*c14)+(s14*c2));
    cnk[14][1][0] = ((s2*c14)+(s14*c2));
    cnk[14][1][1] = ((c2*c14)-(s2*s14));
    cnk[17][0][0] = ((cnk[14][0][0]*c17)+(cnk[14][0][1]*s17));
    cnk[17][0][1] = ((cnk[14][0][1]*c17)-(cnk[14][0][0]*s17));
    cnk[17][1][0] = ((cnk[14][1][0]*c17)+(cnk[14][1][1]*s17));
    cnk[17][1][1] = ((cnk[14][1][1]*c17)-(cnk[14][1][0]*s17));
    cnk[20][0][0] = ((cnk[17][0][0]*c20)+(cnk[17][0][1]*s20));
    cnk[20][0][1] = ((cnk[17][0][1]*c20)-(cnk[17][0][0]*s20));
    cnk[20][1][0] = ((cnk[17][1][0]*c20)+(cnk[17][1][1]*s20));
    cnk[20][1][1] = ((cnk[17][1][1]*c20)-(cnk[17][1][0]*s20));
    cnk[21][0][0] = ((cnk[17][0][0]*c21)+(cnk[17][0][1]*s21));
    cnk[21][0][1] = ((cnk[17][0][1]*c21)-(cnk[17][0][0]*s21));
    cnk[21][1][0] = ((cnk[17][1][0]*c21)+(cnk[17][1][1]*s21));
    cnk[21][1][1] = ((cnk[17][1][1]*c21)-(cnk[17][1][0]*s21));
    cnk[22][0][1] = (cnk[21][0][1]*c22);
    cnk[22][0][2] = -(cnk[21][0][1]*s22);
    cnk[22][1][1] = (cnk[21][1][1]*c22);
    cnk[22][1][2] = -(cnk[21][1][1]*s22);
    cnk[23][0][0] = ((cnk[21][0][0]*c23)+(cnk[22][0][1]*s23));
    cnk[23][0][1] = ((cnk[22][0][1]*c23)-(cnk[21][0][0]*s23));
    cnk[23][1][0] = ((cnk[21][1][0]*c23)+(cnk[22][1][1]*s23));
    cnk[23][1][1] = ((cnk[22][1][1]*c23)-(cnk[21][1][0]*s23));
    cnk[23][2][0] = (s22*s23);
    cnk[23][2][1] = (s22*c23);
    cnk[24][0][0] = ((cnk[23][0][0]*c24)+(cnk[23][0][1]*s24));
    cnk[24][0][1] = ((cnk[23][0][1]*c24)-(cnk[23][0][0]*s24));
    cnk[24][1][0] = ((cnk[23][1][0]*c24)+(cnk[23][1][1]*s24));
    cnk[24][1][1] = ((cnk[23][1][1]*c24)-(cnk[23][1][0]*s24));
    cnk[24][2][0] = ((cnk[23][2][0]*c24)+(cnk[23][2][1]*s24));
    cnk[24][2][1] = ((cnk[23][2][1]*c24)-(cnk[23][2][0]*s24));
    cnb[0][0][0] = c2;
    cnb[0][0][1] = -s2;
    cnb[0][0][2] = 0.;
    cnb[0][1][0] = s2;
    cnb[0][1][1] = c2;
    cnb[0][1][2] = 0.;
    cnb[0][2][0] = 0.;
    cnb[0][2][1] = 0.;
    cnb[0][2][2] = 1.;
    cnb[1][0][0] = cnk[3][0][0];
    cnb[1][0][1] = cnk[3][0][1];
    cnb[1][0][2] = 0.;
    cnb[1][1][0] = cnk[3][1][0];
    cnb[1][1][1] = cnk[3][1][1];
    cnb[1][1][2] = 0.;
    cnb[1][2][0] = 0.;
    cnb[1][2][1] = 0.;
    cnb[1][2][2] = 1.;
    cnb[2][0][0] = cnk[6][0][0];
    cnb[2][0][1] = cnk[6][0][1];
    cnb[2][0][2] = 0.;
    cnb[2][1][0] = cnk[6][1][0];
    cnb[2][1][1] = cnk[6][1][1];
    cnb[2][1][2] = 0.;
    cnb[2][2][0] = 0.;
    cnb[2][2][1] = 0.;
    cnb[2][2][2] = 1.;
    cnb[3][0][0] = cnk[9][0][0];
    cnb[3][0][1] = cnk[9][0][1];
    cnb[3][0][2] = 0.;
    cnb[3][1][0] = cnk[9][1][0];
    cnb[3][1][1] = cnk[9][1][1];
    cnb[3][1][2] = 0.;
    cnb[3][2][0] = 0.;
    cnb[3][2][1] = 0.;
    cnb[3][2][2] = 1.;
    cnb[4][0][0] = cnk[10][0][0];
    cnb[4][0][1] = cnk[10][0][1];
    cnb[4][0][2] = 0.;
    cnb[4][1][0] = cnk[10][1][0];
    cnb[4][1][1] = cnk[10][1][1];
    cnb[4][1][2] = 0.;
    cnb[4][2][0] = 0.;
    cnb[4][2][1] = 0.;
    cnb[4][2][2] = 1.;
    cnb[5][0][0] = cnk[10][0][0];
    cnb[5][0][1] = cnk[11][0][1];
    cnb[5][0][2] = cnk[11][0][2];
    cnb[5][1][0] = cnk[10][1][0];
    cnb[5][1][1] = cnk[11][1][1];
    cnb[5][1][2] = cnk[11][1][2];
    cnb[5][2][0] = 0.;
    cnb[5][2][1] = s11;
    cnb[5][2][2] = c11;
    cnb[6][0][0] = cnk[12][0][0];
    cnb[6][0][1] = cnk[12][0][1];
    cnb[6][0][2] = cnk[11][0][2];
    cnb[6][1][0] = cnk[12][1][0];
    cnb[6][1][1] = cnk[12][1][1];
    cnb[6][1][2] = cnk[11][1][2];
    cnb[6][2][0] = cnk[12][2][0];
    cnb[6][2][1] = cnk[12][2][1];
    cnb[6][2][2] = c11;
    cnb[7][0][0] = cnk[13][0][0];
    cnb[7][0][1] = cnk[13][0][1];
    cnb[7][0][2] = cnk[11][0][2];
    cnb[7][1][0] = cnk[13][1][0];
    cnb[7][1][1] = cnk[13][1][1];
    cnb[7][1][2] = cnk[11][1][2];
    cnb[7][2][0] = cnk[13][2][0];
    cnb[7][2][1] = cnk[13][2][1];
    cnb[7][2][2] = c11;
    cnb[8][0][0] = cnk[14][0][0];
    cnb[8][0][1] = cnk[14][0][1];
    cnb[8][0][2] = 0.;
    cnb[8][1][0] = cnk[14][1][0];
    cnb[8][1][1] = cnk[14][1][1];
    cnb[8][1][2] = 0.;
    cnb[8][2][0] = 0.;
    cnb[8][2][1] = 0.;
    cnb[8][2][2] = 1.;
    cnb[9][0][0] = cnk[17][0][0];
    cnb[9][0][1] = cnk[17][0][1];
    cnb[9][0][2] = 0.;
    cnb[9][1][0] = cnk[17][1][0];
    cnb[9][1][1] = cnk[17][1][1];
    cnb[9][1][2] = 0.;
    cnb[9][2][0] = 0.;
    cnb[9][2][1] = 0.;
    cnb[9][2][2] = 1.;
    cnb[10][0][0] = cnk[20][0][0];
    cnb[10][0][1] = cnk[20][0][1];
    cnb[10][0][2] = 0.;
    cnb[10][1][0] = cnk[20][1][0];
    cnb[10][1][1] = cnk[20][1][1];
    cnb[10][1][2] = 0.;
    cnb[10][2][0] = 0.;
    cnb[10][2][1] = 0.;
    cnb[10][2][2] = 1.;
    cnb[11][0][0] = cnk[21][0][0];
    cnb[11][0][1] = cnk[21][0][1];
    cnb[11][0][2] = 0.;
    cnb[11][1][0] = cnk[21][1][0];
    cnb[11][1][1] = cnk[21][1][1];
    cnb[11][1][2] = 0.;
    cnb[11][2][0] = 0.;
    cnb[11][2][1] = 0.;
    cnb[11][2][2] = 1.;
    cnb[12][0][0] = cnk[21][0][0];
    cnb[12][0][1] = cnk[22][0][1];
    cnb[12][0][2] = cnk[22][0][2];
    cnb[12][1][0] = cnk[21][1][0];
    cnb[12][1][1] = cnk[22][1][1];
    cnb[12][1][2] = cnk[22][1][2];
    cnb[12][2][0] = 0.;
    cnb[12][2][1] = s22;
    cnb[12][2][2] = c22;
    cnb[13][0][0] = cnk[23][0][0];
    cnb[13][0][1] = cnk[23][0][1];
    cnb[13][0][2] = cnk[22][0][2];
    cnb[13][1][0] = cnk[23][1][0];
    cnb[13][1][1] = cnk[23][1][1];
    cnb[13][1][2] = cnk[22][1][2];
    cnb[13][2][0] = cnk[23][2][0];
    cnb[13][2][1] = cnk[23][2][1];
    cnb[13][2][2] = c22;
    cnb[14][0][0] = cnk[24][0][0];
    cnb[14][0][1] = cnk[24][0][1];
    cnb[14][0][2] = cnk[22][0][2];
    cnb[14][1][0] = cnk[24][1][0];
    cnb[14][1][1] = cnk[24][1][1];
    cnb[14][1][2] = cnk[22][1][2];
    cnb[14][2][0] = cnk[24][2][0];
    cnb[14][2][1] = cnk[24][2][1];
    cnb[14][2][2] = c22;
/*
Compute q-related auxiliary variables
*/
    rik[3][1] = -(.17+(.33*c3));
    rik[11][1] = (.015+((.00792*s11)-(.04195*c11)));
    rik[11][2] = ((.00792*c11)+(.04195*s11));
    rik[12][0] = (.025+((.0942*c12)-(.015*s12)));
    rik[12][1] = (.015-((.015*c12)+(.0942*s12)));
    rik[13][0] = (.0346+((.0346*c13)-(.017*s13)));
    rik[13][1] = (.006-((.017*c13)+(.0346*s13)));
    rik[14][1] = -(.17+(.33*c14));
    rik[22][1] = (.015-((.00792*s22)+(.04195*c22)));
    rik[22][2] = ((.04195*s22)-(.00792*c22));
    rik[23][0] = (.025+((.0942*c23)-(.015*s23)));
    rik[23][1] = (.015-((.015*c23)+(.0942*s23)));
    rik[24][0] = (.0346+((.0346*c24)-(.017*s24)));
    rik[24][1] = (.006-((.017*c24)+(.0346*s24)));
/*
Compute rnk & rnb (mass center locations in N)
*/
    rnk[2][0] = (q[0]-((.0707*c2)+(.2639*s2)));
    rnk[2][1] = (q[1]+((.2639*c2)-(.0707*s2)));
    rnk[3][0] = ((rnk[2][0]+(.33*s2))-(.17*cnk[3][0][1]));
    rnk[3][1] = ((rnk[2][1]-(.33*c2))-(.17*cnk[3][1][1]));
    rnk[4][0] = ((cnk[3][0][0]*q[4])+(rnk[3][0]+(.17*cnk[3][0][1])));
    rnk[4][1] = ((cnk[3][1][0]*q[4])+(rnk[3][1]+(.17*cnk[3][1][1])));
    rnk[5][0] = (rnk[4][0]+(cnk[3][0][1]*q[5]));
    rnk[5][1] = (rnk[4][1]+(cnk[3][1][1]*q[5]));
    rnk[6][0] = (rnk[5][0]-(.1867*cnk[6][0][1]));
    rnk[6][1] = (rnk[5][1]-(.1867*cnk[6][1][1]));
    rnk[7][0] = ((cnk[6][0][0]*q[7])+(rnk[6][0]+(.1867*cnk[6][0][1])));
    rnk[7][1] = ((cnk[6][1][0]*q[7])+(rnk[6][1]+(.1867*cnk[6][1][1])));
    rnk[8][0] = (rnk[7][0]+(cnk[6][0][1]*q[8]));
    rnk[8][1] = (rnk[7][1]+(cnk[6][1][1]*q[8]));
    rnk[10][0] = (rnk[6][0]-(.2433*cnk[6][0][1]));
    rnk[10][1] = (rnk[6][1]-(.2433*cnk[6][1][1]));
    rnk[11][0] = ((rnk[10][0]-((.04195*cnk[10][0][1])+(.04877*cnk[10][0][0])))+(
      (.015*cnk[11][0][1])+(.025*cnk[10][0][0])));
    rnk[11][1] = ((rnk[10][1]-((.04195*cnk[10][1][1])+(.04877*cnk[10][1][0])))+(
      (.015*cnk[11][1][1])+(.025*cnk[10][1][0])));
    rnk[11][2] = (.09142+(.015*s11));
    rnk[12][0] = ((rnk[11][0]+((.0942*cnk[10][0][0])-(.015*cnk[11][0][1])))+((
      .015*cnk[12][0][1])+(.025*cnk[12][0][0])));
    rnk[12][1] = ((rnk[11][1]+((.0942*cnk[10][1][0])-(.015*cnk[11][1][1])))+((
      .015*cnk[12][1][1])+(.025*cnk[12][1][0])));
    rnk[12][2] = ((rnk[11][2]-(.015*s11))+((.015*cnk[12][2][1])+(.025*
      cnk[12][2][0])));
    rnk[13][0] = ((rnk[12][0]+((.00108*cnk[11][0][2])+((.0346*cnk[12][0][0])-(
      .017*cnk[12][0][1]))))+(((.006*cnk[13][0][1])+(.0346*cnk[13][0][0]))-(
      .0175*cnk[11][0][2])));
    rnk[13][1] = ((rnk[12][1]+((.00108*cnk[11][1][2])+((.0346*cnk[12][1][0])-(
      .017*cnk[12][1][1]))))+(((.006*cnk[13][1][1])+(.0346*cnk[13][1][0]))-(
      .0175*cnk[11][1][2])));
    rnk[13][2] = ((rnk[12][2]+((.00108*c11)+((.0346*cnk[12][2][0])-(.017*
      cnk[12][2][1]))))+(((.006*cnk[13][2][1])+(.0346*cnk[13][2][0]))-(.0175*c11
      )));
    rnk[14][0] = ((rnk[2][0]+(.33*s2))-(.17*cnk[14][0][1]));
    rnk[14][1] = ((rnk[2][1]-(.33*c2))-(.17*cnk[14][1][1]));
    rnk[15][0] = ((cnk[14][0][0]*q[15])+(rnk[14][0]+(.17*cnk[14][0][1])));
    rnk[15][1] = ((cnk[14][1][0]*q[15])+(rnk[14][1]+(.17*cnk[14][1][1])));
    rnk[16][0] = (rnk[15][0]+(cnk[14][0][1]*q[16]));
    rnk[16][1] = (rnk[15][1]+(cnk[14][1][1]*q[16]));
    rnk[17][0] = (rnk[16][0]-(.1867*cnk[17][0][1]));
    rnk[17][1] = (rnk[16][1]-(.1867*cnk[17][1][1]));
    rnk[18][0] = ((cnk[17][0][0]*q[18])+(rnk[17][0]+(.1867*cnk[17][0][1])));
    rnk[18][1] = ((cnk[17][1][0]*q[18])+(rnk[17][1]+(.1867*cnk[17][1][1])));
    rnk[19][0] = (rnk[18][0]+(cnk[17][0][1]*q[19]));
    rnk[19][1] = (rnk[18][1]+(cnk[17][1][1]*q[19]));
    rnk[21][0] = (rnk[17][0]-(.2433*cnk[17][0][1]));
    rnk[21][1] = (rnk[17][1]-(.2433*cnk[17][1][1]));
    rnk[22][0] = ((rnk[21][0]-((.04195*cnk[21][0][1])+(.04877*cnk[21][0][0])))+(
      (.015*cnk[22][0][1])+(.025*cnk[21][0][0])));
    rnk[22][1] = ((rnk[21][1]-((.04195*cnk[21][1][1])+(.04877*cnk[21][1][0])))+(
      (.015*cnk[22][1][1])+(.025*cnk[21][1][0])));
    rnk[22][2] = ((.015*s22)-.09142);
    rnk[23][0] = ((rnk[22][0]+((.0942*cnk[21][0][0])-(.015*cnk[22][0][1])))+((
      .015*cnk[23][0][1])+(.025*cnk[23][0][0])));
    rnk[23][1] = ((rnk[22][1]+((.0942*cnk[21][1][0])-(.015*cnk[22][1][1])))+((
      .015*cnk[23][1][1])+(.025*cnk[23][1][0])));
    rnk[23][2] = ((rnk[22][2]-(.015*s22))+((.015*cnk[23][2][1])+(.025*
      cnk[23][2][0])));
    rnk[24][0] = ((rnk[23][0]+(((.0346*cnk[23][0][0])-(.017*cnk[23][0][1]))-(
      .00108*cnk[22][0][2])))+((.0175*cnk[22][0][2])+((.006*cnk[24][0][1])+(
      .0346*cnk[24][0][0]))));
    rnk[24][1] = ((rnk[23][1]+(((.0346*cnk[23][1][0])-(.017*cnk[23][1][1]))-(
      .00108*cnk[22][1][2])))+((.0175*cnk[22][1][2])+((.006*cnk[24][1][1])+(
      .0346*cnk[24][1][0]))));
    rnk[24][2] = ((rnk[23][2]+(((.0346*cnk[23][2][0])-(.017*cnk[23][2][1]))-(
      .00108*c22)))+((.0175*c22)+((.006*cnk[24][2][1])+(.0346*cnk[24][2][0]))));
    rnb[0][0] = rnk[2][0];
    rnb[0][1] = rnk[2][1];
    rnb[0][2] = 0.;
    rnb[1][0] = rnk[3][0];
    rnb[1][1] = rnk[3][1];
    rnb[1][2] = .0835;
    rnb[2][0] = rnk[6][0];
    rnb[2][1] = rnk[6][1];
    rnb[2][2] = .0835;
    rnb[3][0] = rnk[8][0];
    rnb[3][1] = rnk[8][1];
    rnb[3][2] = .0859;
    rnb[4][0] = rnk[10][0];
    rnb[4][1] = rnk[10][1];
    rnb[4][2] = .0835;
    rnb[5][0] = rnk[11][0];
    rnb[5][1] = rnk[11][1];
    rnb[5][2] = rnk[11][2];
    rnb[6][0] = rnk[12][0];
    rnb[6][1] = rnk[12][1];
    rnb[6][2] = rnk[12][2];
    rnb[7][0] = rnk[13][0];
    rnb[7][1] = rnk[13][1];
    rnb[7][2] = rnk[13][2];
    rnb[8][0] = rnk[14][0];
    rnb[8][1] = rnk[14][1];
    rnb[8][2] = -.0835;
    rnb[9][0] = rnk[17][0];
    rnb[9][1] = rnk[17][1];
    rnb[9][2] = -.0835;
    rnb[10][0] = rnk[19][0];
    rnb[10][1] = rnk[19][1];
    rnb[10][2] = -.0859;
    rnb[11][0] = rnk[21][0];
    rnb[11][1] = rnk[21][1];
    rnb[11][2] = -.0835;
    rnb[12][0] = rnk[22][0];
    rnb[12][1] = rnk[22][1];
    rnb[12][2] = rnk[22][2];
    rnb[13][0] = rnk[23][0];
    rnb[13][1] = rnk[23][1];
    rnb[13][2] = rnk[23][2];
    rnb[14][0] = rnk[24][0];
    rnb[14][1] = rnk[24][1];
    rnb[14][2] = rnk[24][2];
/*
Compute com (system mass center location in N)
*/
    com[0] = (.0133105811133502*((.2058*rnk[24][0])+((.4*rnk[23][0])+((.4183*
      rnk[22][0])+((.1*rnk[21][0])+((.1*rnk[19][0])+((3.03*rnk[17][0])+((7.76*
      rnk[14][0])+((.2058*rnk[13][0])+((.4*rnk[12][0])+((.4183*rnk[11][0])+((.1*
      rnk[10][0])+((.1*rnk[8][0])+((3.03*rnk[6][0])+((7.76*rnk[3][0])+(51.1*
      rnk[2][0]))))))))))))))));
    com[1] = (.0133105811133502*((.2058*rnk[24][1])+((.4*rnk[23][1])+((.4183*
      rnk[22][1])+((.1*rnk[21][1])+((.1*rnk[19][1])+((3.03*rnk[17][1])+((7.76*
      rnk[14][1])+((.2058*rnk[13][1])+((.4*rnk[12][1])+((.4183*rnk[11][1])+((.1*
      rnk[10][1])+((.1*rnk[8][1])+((3.03*rnk[6][1])+((7.76*rnk[3][1])+(51.1*
      rnk[2][1]))))))))))))))));
    com[2] = (.0133105811133502*((.2058*rnk[24][2])+((.4*rnk[23][2])+((.4183*
      rnk[22][2])+((((((.2058*rnk[13][2])+((.4*rnk[12][2])+(.917905+(.4183*
      rnk[11][2]))))-.64796)-.253005)-.00859)-.00835)))));
/*
Compute constraint position errors
*/
    skipqs: ;
    sduperr(curtim,q,&perr[2]);
    if (uchg == 0) {
        goto skipus;
    }
/*
Velocity-related variables need to be computed
*/
    inerflg = 0;
    for (i = 0; i < 25; i++) {
        u[i] = uin[i];
    }
/*
Compute u-related auxiliary variables
*/
/*
Compute wk & wb (angular velocities)
*/
    wk[3][2] = (u[2]+u[3]);
    wk[6][2] = (u[6]+wk[3][2]);
    wk[9][2] = (u[9]+wk[6][2]);
    wk[10][2] = (u[10]+wk[6][2]);
    wk[11][1] = (wk[10][2]*s11);
    wk[11][2] = (wk[10][2]*c11);
    wk[12][0] = ((u[11]*c12)+(wk[11][1]*s12));
    wk[12][1] = ((wk[11][1]*c12)-(u[11]*s12));
    wk[12][2] = (u[12]+wk[11][2]);
    wk[13][0] = ((wk[12][0]*c13)+(wk[12][1]*s13));
    wk[13][1] = ((wk[12][1]*c13)-(wk[12][0]*s13));
    wk[13][2] = (u[13]+wk[12][2]);
    wk[14][2] = (u[2]+u[14]);
    wk[17][2] = (u[17]+wk[14][2]);
    wk[20][2] = (u[20]+wk[17][2]);
    wk[21][2] = (u[21]+wk[17][2]);
    wk[22][1] = (wk[21][2]*s22);
    wk[22][2] = (wk[21][2]*c22);
    wk[23][0] = ((u[22]*c23)+(wk[22][1]*s23));
    wk[23][1] = ((wk[22][1]*c23)-(u[22]*s23));
    wk[23][2] = (u[23]+wk[22][2]);
    wk[24][0] = ((wk[23][0]*c24)+(wk[23][1]*s24));
    wk[24][1] = ((wk[23][1]*c24)-(wk[23][0]*s24));
    wk[24][2] = (u[24]+wk[23][2]);
    wb[0][0] = 0.;
    wb[0][1] = 0.;
    wb[0][2] = u[2];
    wb[1][0] = 0.;
    wb[1][1] = 0.;
    wb[1][2] = wk[3][2];
    wb[2][0] = 0.;
    wb[2][1] = 0.;
    wb[2][2] = wk[6][2];
    wb[3][0] = 0.;
    wb[3][1] = 0.;
    wb[3][2] = wk[9][2];
    wb[4][0] = 0.;
    wb[4][1] = 0.;
    wb[4][2] = wk[10][2];
    wb[5][0] = u[11];
    wb[5][1] = wk[11][1];
    wb[5][2] = wk[11][2];
    wb[6][0] = wk[12][0];
    wb[6][1] = wk[12][1];
    wb[6][2] = wk[12][2];
    wb[7][0] = wk[13][0];
    wb[7][1] = wk[13][1];
    wb[7][2] = wk[13][2];
    wb[8][0] = 0.;
    wb[8][1] = 0.;
    wb[8][2] = wk[14][2];
    wb[9][0] = 0.;
    wb[9][1] = 0.;
    wb[9][2] = wk[17][2];
    wb[10][0] = 0.;
    wb[10][1] = 0.;
    wb[10][2] = wk[20][2];
    wb[11][0] = 0.;
    wb[11][1] = 0.;
    wb[11][2] = wk[21][2];
    wb[12][0] = u[22];
    wb[12][1] = wk[22][1];
    wb[12][2] = wk[22][2];
    wb[13][0] = wk[23][0];
    wb[13][1] = wk[23][1];
    wb[13][2] = wk[23][2];
    wb[14][0] = wk[24][0];
    wb[14][1] = wk[24][1];
    wb[14][2] = wk[24][2];
/*
Compute auxiliary variables involving wk
*/
    Wirk[12][2] = -((.015*u[11])+(.0942*wk[11][1]));
    Wirk[13][0] = ((.00108*wk[12][1])+(.017*wk[12][2]));
    Wirk[13][1] = ((.0346*wk[12][2])-(.00108*wk[12][0]));
    Wirk[13][2] = -((.017*wk[12][0])+(.0346*wk[12][1]));
    Wirk[23][2] = -((.015*u[22])+(.0942*wk[22][1]));
    Wirk[24][0] = ((.017*wk[23][2])-(.00108*wk[23][1]));
    Wirk[24][1] = ((.00108*wk[23][0])+(.0346*wk[23][2]));
    Wirk[24][2] = -((.017*wk[23][0])+(.0346*wk[23][1]));
    Wkrpk[4][1] = (q[4]*wk[3][2]);
    Wkrpk[5][0] = -(q[5]*wk[3][2]);
    Wkrpk[7][1] = (q[7]*wk[6][2]);
    Wkrpk[8][0] = -(q[8]*wk[6][2]);
    Wkrpk[11][2] = ((.015*u[11])-(.025*wk[11][1]));
    Wkrpk[12][2] = ((.015*wk[12][0])-(.025*wk[12][1]));
    Wkrpk[13][0] = -((.006*wk[13][2])+(.0175*wk[13][1]));
    Wkrpk[13][1] = ((.0175*wk[13][0])+(.0346*wk[13][2]));
    Wkrpk[13][2] = ((.006*wk[13][0])-(.0346*wk[13][1]));
    Wkrpk[15][1] = (q[15]*wk[14][2]);
    Wkrpk[16][0] = -(q[16]*wk[14][2]);
    Wkrpk[18][1] = (q[18]*wk[17][2]);
    Wkrpk[19][0] = -(q[19]*wk[17][2]);
    Wkrpk[22][2] = ((.015*u[22])-(.025*wk[22][1]));
    Wkrpk[23][2] = ((.015*wk[23][0])-(.025*wk[23][1]));
    Wkrpk[24][0] = ((.0175*wk[24][1])-(.006*wk[24][2]));
    Wkrpk[24][1] = ((.0346*wk[24][2])-(.0175*wk[24][0]));
    Wkrpk[24][2] = ((.006*wk[24][0])-(.0346*wk[24][1]));
/*
Compute temporaries for use in SDRHS
*/
    w0w0[5] = (u[11]*u[11]);
    w0w0[6] = (wk[12][0]*wk[12][0]);
    w0w0[7] = (wk[13][0]*wk[13][0]);
    w0w0[12] = (u[22]*u[22]);
    w0w0[13] = (wk[23][0]*wk[23][0]);
    w0w0[14] = (wk[24][0]*wk[24][0]);
    w1w1[5] = (wk[11][1]*wk[11][1]);
    w1w1[6] = (wk[12][1]*wk[12][1]);
    w1w1[7] = (wk[13][1]*wk[13][1]);
    w1w1[12] = (wk[22][1]*wk[22][1]);
    w1w1[13] = (wk[23][1]*wk[23][1]);
    w1w1[14] = (wk[24][1]*wk[24][1]);
    w2w2[0] = (u[2]*u[2]);
    w2w2[1] = (wk[3][2]*wk[3][2]);
    w2w2[2] = (wk[6][2]*wk[6][2]);
    w2w2[3] = (wk[9][2]*wk[9][2]);
    w2w2[4] = (wk[10][2]*wk[10][2]);
    w2w2[5] = (wk[11][2]*wk[11][2]);
    w2w2[6] = (wk[12][2]*wk[12][2]);
    w2w2[7] = (wk[13][2]*wk[13][2]);
    w2w2[8] = (wk[14][2]*wk[14][2]);
    w2w2[9] = (wk[17][2]*wk[17][2]);
    w2w2[10] = (wk[20][2]*wk[20][2]);
    w2w2[11] = (wk[21][2]*wk[21][2]);
    w2w2[12] = (wk[22][2]*wk[22][2]);
    w2w2[13] = (wk[23][2]*wk[23][2]);
    w2w2[14] = (wk[24][2]*wk[24][2]);
    w0w1[5] = (u[11]*wk[11][1]);
    w0w1[6] = (wk[12][0]*wk[12][1]);
    w0w1[7] = (wk[13][0]*wk[13][1]);
    w0w1[12] = (u[22]*wk[22][1]);
    w0w1[13] = (wk[23][0]*wk[23][1]);
    w0w1[14] = (wk[24][0]*wk[24][1]);
    w0w2[5] = (u[11]*wk[11][2]);
    w0w2[6] = (wk[12][0]*wk[12][2]);
    w0w2[7] = (wk[13][0]*wk[13][2]);
    w0w2[12] = (u[22]*wk[22][2]);
    w0w2[13] = (wk[23][0]*wk[23][2]);
    w0w2[14] = (wk[24][0]*wk[24][2]);
    w1w2[5] = (wk[11][1]*wk[11][2]);
    w1w2[6] = (wk[12][1]*wk[12][2]);
    w1w2[7] = (wk[13][1]*wk[13][2]);
    w1w2[12] = (wk[22][1]*wk[22][2]);
    w1w2[13] = (wk[23][1]*wk[23][2]);
    w1w2[14] = (wk[24][1]*wk[24][2]);
    w00w11[5] = -(w0w0[5]+w1w1[5]);
    w00w11[6] = -(w0w0[6]+w1w1[6]);
    w00w11[7] = -(w0w0[7]+w1w1[7]);
    w00w11[12] = -(w0w0[12]+w1w1[12]);
    w00w11[13] = -(w0w0[13]+w1w1[13]);
    w00w11[14] = -(w0w0[14]+w1w1[14]);
    w00w22[5] = -(w0w0[5]+w2w2[5]);
    w00w22[6] = -(w0w0[6]+w2w2[6]);
    w00w22[7] = -(w0w0[7]+w2w2[7]);
    w00w22[12] = -(w0w0[12]+w2w2[12]);
    w00w22[13] = -(w0w0[13]+w2w2[13]);
    w00w22[14] = -(w0w0[14]+w2w2[14]);
    w11w22[5] = -(w1w1[5]+w2w2[5]);
    w11w22[6] = -(w1w1[6]+w2w2[6]);
    w11w22[7] = -(w1w1[7]+w2w2[7]);
    w11w22[12] = -(w1w1[12]+w2w2[12]);
    w11w22[13] = -(w1w1[13]+w2w2[13]);
    w11w22[14] = -(w1w1[14]+w2w2[14]);
/*
Compute vnk & vnb (mass center linear velocities in N)
*/
    vnk[2][0] = (u[0]+((.0707*(u[2]*s2))-(.2639*(u[2]*c2))));
    vnk[2][1] = (u[1]-((.0707*(u[2]*c2))+(.2639*(u[2]*s2))));
    vnk[3][0] = ((.17*(cnk[3][0][0]*wk[3][2]))+(vnk[2][0]+(.33*(u[2]*c2))));
    vnk[3][1] = ((.17*(cnk[3][1][0]*wk[3][2]))+(vnk[2][1]+(.33*(u[2]*s2))));
    vnk[4][0] = ((vnk[3][0]-(.17*(cnk[3][0][0]*wk[3][2])))+((cnk[3][0][0]*u[4])+
      (cnk[3][0][1]*Wkrpk[4][1])));
    vnk[4][1] = ((vnk[3][1]-(.17*(cnk[3][1][0]*wk[3][2])))+((cnk[3][1][0]*u[4])+
      (cnk[3][1][1]*Wkrpk[4][1])));
    vnk[5][0] = (vnk[4][0]+((cnk[3][0][0]*Wkrpk[5][0])+(cnk[3][0][1]*u[5])));
    vnk[5][1] = (vnk[4][1]+((cnk[3][1][0]*Wkrpk[5][0])+(cnk[3][1][1]*u[5])));
    vnk[6][0] = (vnk[5][0]+(.1867*(cnk[6][0][0]*wk[6][2])));
    vnk[6][1] = (vnk[5][1]+(.1867*(cnk[6][1][0]*wk[6][2])));
    vnk[7][0] = ((vnk[6][0]-(.1867*(cnk[6][0][0]*wk[6][2])))+((cnk[6][0][0]*u[7]
      )+(cnk[6][0][1]*Wkrpk[7][1])));
    vnk[7][1] = ((vnk[6][1]-(.1867*(cnk[6][1][0]*wk[6][2])))+((cnk[6][1][0]*u[7]
      )+(cnk[6][1][1]*Wkrpk[7][1])));
    vnk[8][0] = (vnk[7][0]+((cnk[6][0][0]*Wkrpk[8][0])+(cnk[6][0][1]*u[8])));
    vnk[8][1] = (vnk[7][1]+((cnk[6][1][0]*Wkrpk[8][0])+(cnk[6][1][1]*u[8])));
    vnk[10][0] = (vnk[6][0]+(.2433*(cnk[6][0][0]*wk[6][2])));
    vnk[10][1] = (vnk[6][1]+(.2433*(cnk[6][1][0]*wk[6][2])));
    vnk[11][0] = ((vnk[10][0]+((.04195*(cnk[10][0][0]*wk[10][2]))-(.04877*(
      cnk[10][0][1]*wk[10][2]))))+((cnk[11][0][2]*Wkrpk[11][2])+((.025*(
      cnk[11][0][1]*wk[11][2]))-(.015*(cnk[10][0][0]*wk[11][2])))));
    vnk[11][1] = ((vnk[10][1]+((.04195*(cnk[10][1][0]*wk[10][2]))-(.04877*(
      cnk[10][1][1]*wk[10][2]))))+((cnk[11][1][2]*Wkrpk[11][2])+((.025*(
      cnk[11][1][1]*wk[11][2]))-(.015*(cnk[10][1][0]*wk[11][2])))));
    vnk[11][2] = ((.025*(wk[11][2]*s11))+(Wkrpk[11][2]*c11));
    vnk[12][0] = ((vnk[11][0]+((cnk[11][0][2]*Wirk[12][2])+((.015*(cnk[10][0][0]
      *wk[11][2]))+(.0942*(cnk[11][0][1]*wk[11][2])))))+((cnk[11][0][2]*
      Wkrpk[12][2])+((.025*(cnk[12][0][1]*wk[12][2]))-(.015*(cnk[12][0][0]*
      wk[12][2])))));
    vnk[12][1] = ((vnk[11][1]+((cnk[11][1][2]*Wirk[12][2])+((.015*(cnk[10][1][0]
      *wk[11][2]))+(.0942*(cnk[11][1][1]*wk[11][2])))))+((cnk[11][1][2]*
      Wkrpk[12][2])+((.025*(cnk[12][1][1]*wk[12][2]))-(.015*(cnk[12][1][0]*
      wk[12][2])))));
    vnk[12][2] = ((vnk[11][2]+((.0942*(wk[11][2]*s11))+(Wirk[12][2]*c11)))+((
      Wkrpk[12][2]*c11)+((.025*(cnk[12][2][1]*wk[12][2]))-(.015*(cnk[12][2][0]*
      wk[12][2])))));
    vnk[13][0] = ((vnk[12][0]+((cnk[11][0][2]*Wirk[13][2])+((cnk[12][0][0]*
      Wirk[13][0])+(cnk[12][0][1]*Wirk[13][1]))))+((cnk[11][0][2]*Wkrpk[13][2])+
      ((cnk[13][0][0]*Wkrpk[13][0])+(cnk[13][0][1]*Wkrpk[13][1]))));
    vnk[13][1] = ((vnk[12][1]+((cnk[11][1][2]*Wirk[13][2])+((cnk[12][1][0]*
      Wirk[13][0])+(cnk[12][1][1]*Wirk[13][1]))))+((cnk[11][1][2]*Wkrpk[13][2])+
      ((cnk[13][1][0]*Wkrpk[13][0])+(cnk[13][1][1]*Wkrpk[13][1]))));
    vnk[13][2] = ((vnk[12][2]+((Wirk[13][2]*c11)+((cnk[12][2][0]*Wirk[13][0])+(
      cnk[12][2][1]*Wirk[13][1]))))+((Wkrpk[13][2]*c11)+((cnk[13][2][0]*
      Wkrpk[13][0])+(cnk[13][2][1]*Wkrpk[13][1]))));
    vnk[14][0] = ((.17*(cnk[14][0][0]*wk[14][2]))+(vnk[2][0]+(.33*(u[2]*c2))));
    vnk[14][1] = ((.17*(cnk[14][1][0]*wk[14][2]))+(vnk[2][1]+(.33*(u[2]*s2))));
    vnk[15][0] = ((vnk[14][0]-(.17*(cnk[14][0][0]*wk[14][2])))+((cnk[14][0][0]*
      u[15])+(cnk[14][0][1]*Wkrpk[15][1])));
    vnk[15][1] = ((vnk[14][1]-(.17*(cnk[14][1][0]*wk[14][2])))+((cnk[14][1][0]*
      u[15])+(cnk[14][1][1]*Wkrpk[15][1])));
    vnk[16][0] = (vnk[15][0]+((cnk[14][0][0]*Wkrpk[16][0])+(cnk[14][0][1]*u[16])
      ));
    vnk[16][1] = (vnk[15][1]+((cnk[14][1][0]*Wkrpk[16][0])+(cnk[14][1][1]*u[16])
      ));
    vnk[17][0] = (vnk[16][0]+(.1867*(cnk[17][0][0]*wk[17][2])));
    vnk[17][1] = (vnk[16][1]+(.1867*(cnk[17][1][0]*wk[17][2])));
    vnk[18][0] = ((vnk[17][0]-(.1867*(cnk[17][0][0]*wk[17][2])))+((cnk[17][0][0]
      *u[18])+(cnk[17][0][1]*Wkrpk[18][1])));
    vnk[18][1] = ((vnk[17][1]-(.1867*(cnk[17][1][0]*wk[17][2])))+((cnk[17][1][0]
      *u[18])+(cnk[17][1][1]*Wkrpk[18][1])));
    vnk[19][0] = (vnk[18][0]+((cnk[17][0][0]*Wkrpk[19][0])+(cnk[17][0][1]*u[19])
      ));
    vnk[19][1] = (vnk[18][1]+((cnk[17][1][0]*Wkrpk[19][0])+(cnk[17][1][1]*u[19])
      ));
    vnk[21][0] = (vnk[17][0]+(.2433*(cnk[17][0][0]*wk[17][2])));
    vnk[21][1] = (vnk[17][1]+(.2433*(cnk[17][1][0]*wk[17][2])));
    vnk[22][0] = ((vnk[21][0]+((.04195*(cnk[21][0][0]*wk[21][2]))-(.04877*(
      cnk[21][0][1]*wk[21][2]))))+((cnk[22][0][2]*Wkrpk[22][2])+((.025*(
      cnk[22][0][1]*wk[22][2]))-(.015*(cnk[21][0][0]*wk[22][2])))));
    vnk[22][1] = ((vnk[21][1]+((.04195*(cnk[21][1][0]*wk[21][2]))-(.04877*(
      cnk[21][1][1]*wk[21][2]))))+((cnk[22][1][2]*Wkrpk[22][2])+((.025*(
      cnk[22][1][1]*wk[22][2]))-(.015*(cnk[21][1][0]*wk[22][2])))));
    vnk[22][2] = ((.025*(wk[22][2]*s22))+(Wkrpk[22][2]*c22));
    vnk[23][0] = ((vnk[22][0]+((cnk[22][0][2]*Wirk[23][2])+((.015*(cnk[21][0][0]
      *wk[22][2]))+(.0942*(cnk[22][0][1]*wk[22][2])))))+((cnk[22][0][2]*
      Wkrpk[23][2])+((.025*(cnk[23][0][1]*wk[23][2]))-(.015*(cnk[23][0][0]*
      wk[23][2])))));
    vnk[23][1] = ((vnk[22][1]+((cnk[22][1][2]*Wirk[23][2])+((.015*(cnk[21][1][0]
      *wk[22][2]))+(.0942*(cnk[22][1][1]*wk[22][2])))))+((cnk[22][1][2]*
      Wkrpk[23][2])+((.025*(cnk[23][1][1]*wk[23][2]))-(.015*(cnk[23][1][0]*
      wk[23][2])))));
    vnk[23][2] = ((vnk[22][2]+((.0942*(wk[22][2]*s22))+(Wirk[23][2]*c22)))+((
      Wkrpk[23][2]*c22)+((.025*(cnk[23][2][1]*wk[23][2]))-(.015*(cnk[23][2][0]*
      wk[23][2])))));
    vnk[24][0] = ((vnk[23][0]+((cnk[22][0][2]*Wirk[24][2])+((cnk[23][0][0]*
      Wirk[24][0])+(cnk[23][0][1]*Wirk[24][1]))))+((cnk[22][0][2]*Wkrpk[24][2])+
      ((cnk[24][0][0]*Wkrpk[24][0])+(cnk[24][0][1]*Wkrpk[24][1]))));
    vnk[24][1] = ((vnk[23][1]+((cnk[22][1][2]*Wirk[24][2])+((cnk[23][1][0]*
      Wirk[24][0])+(cnk[23][1][1]*Wirk[24][1]))))+((cnk[22][1][2]*Wkrpk[24][2])+
      ((cnk[24][1][0]*Wkrpk[24][0])+(cnk[24][1][1]*Wkrpk[24][1]))));
    vnk[24][2] = ((vnk[23][2]+((Wirk[24][2]*c22)+((cnk[23][2][0]*Wirk[24][0])+(
      cnk[23][2][1]*Wirk[24][1]))))+((Wkrpk[24][2]*c22)+((cnk[24][2][0]*
      Wkrpk[24][0])+(cnk[24][2][1]*Wkrpk[24][1]))));
    vnb[0][0] = vnk[2][0];
    vnb[0][1] = vnk[2][1];
    vnb[0][2] = 0.;
    vnb[1][0] = vnk[3][0];
    vnb[1][1] = vnk[3][1];
    vnb[1][2] = 0.;
    vnb[2][0] = vnk[6][0];
    vnb[2][1] = vnk[6][1];
    vnb[2][2] = 0.;
    vnb[3][0] = vnk[8][0];
    vnb[3][1] = vnk[8][1];
    vnb[3][2] = 0.;
    vnb[4][0] = vnk[10][0];
    vnb[4][1] = vnk[10][1];
    vnb[4][2] = 0.;
    vnb[5][0] = vnk[11][0];
    vnb[5][1] = vnk[11][1];
    vnb[5][2] = vnk[11][2];
    vnb[6][0] = vnk[12][0];
    vnb[6][1] = vnk[12][1];
    vnb[6][2] = vnk[12][2];
    vnb[7][0] = vnk[13][0];
    vnb[7][1] = vnk[13][1];
    vnb[7][2] = vnk[13][2];
    vnb[8][0] = vnk[14][0];
    vnb[8][1] = vnk[14][1];
    vnb[8][2] = 0.;
    vnb[9][0] = vnk[17][0];
    vnb[9][1] = vnk[17][1];
    vnb[9][2] = 0.;
    vnb[10][0] = vnk[19][0];
    vnb[10][1] = vnk[19][1];
    vnb[10][2] = 0.;
    vnb[11][0] = vnk[21][0];
    vnb[11][1] = vnk[21][1];
    vnb[11][2] = 0.;
    vnb[12][0] = vnk[22][0];
    vnb[12][1] = vnk[22][1];
    vnb[12][2] = vnk[22][2];
    vnb[13][0] = vnk[23][0];
    vnb[13][1] = vnk[23][1];
    vnb[13][2] = vnk[23][2];
    vnb[14][0] = vnk[24][0];
    vnb[14][1] = vnk[24][1];
    vnb[14][2] = vnk[24][2];
/*
Compute qdot (kinematical equations)
*/
    qdot[0] = u[0];
    qdot[1] = u[1];
    qdot[2] = u[2];
    qdot[3] = u[3];
    qdot[4] = u[4];
    qdot[5] = u[5];
    qdot[6] = u[6];
    qdot[7] = u[7];
    qdot[8] = u[8];
    qdot[9] = u[9];
    qdot[10] = u[10];
    qdot[11] = u[11];
    qdot[12] = u[12];
    qdot[13] = u[13];
    qdot[14] = u[14];
    qdot[15] = u[15];
    qdot[16] = u[16];
    qdot[17] = u[17];
    qdot[18] = u[18];
    qdot[19] = u[19];
    qdot[20] = u[20];
    qdot[21] = u[21];
    qdot[22] = u[22];
    qdot[23] = u[23];
    qdot[24] = u[24];
/*
Compute constraint velocity errors
*/
    skipus: ;
    sduverr(curtim,q,u,&verr[2]);
/*
Initialize applied forces and torques to zero
*/
    for (i = 0; i < 15; i++) {
        for (j = 0; j < 3; j++) {
            ufk[i][j] = 0.;
            utk[i][j] = 0.;
        }
    }
    for (i = 0; i < 25; i++) {
        utau[i] = 0.;
    }
    ltauflg = 0;
    fs0flg = 0;
/*
Initialize prescribed motions
*/
    uacc[11] = 0.;
    uvel[11] = u[11];
    upos[11] = q[11];
    uacc[22] = 0.;
    uvel[22] = u[22];
    upos[22] = q[22];
/*
 Used 0.03 seconds CPU time,
 0 additional bytes of memory.
 Equations contain  506 adds/subtracts/negates
                    688 multiplies
                      0 divides
                    784 assignments
*/
}

void sdqdot(double oqdot[25])
{
/*
Return position coordinate derivatives for tree joints.
*/
    int i;

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(63,23);
        return;
    }
    for (i = 0; i <= 24; i++) {
        oqdot[i] = qdot[i];
    }
}

void sdu2qdot(double uin[25],
    double oqdot[25])
{
/*
Convert velocities to qdots.
*/
    int i;

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(64,23);
        return;
    }
    for (i = 0; i <= 24; i++) {
        oqdot[i] = uin[i];
    }
/*
 Used 0.00 seconds CPU time,
 0 additional bytes of memory.
 Equations contain    0 adds/subtracts/negates
                      0 multiplies
                      0 divides
                     25 assignments
*/
}

void sdpsstate(double lqin[1])
{

    if (roustate != 2) {
        sdseterr(9,23);
        return;
    }
}

void sddovpk(void)
{

    if (vpkflg == 0) {
/*
Compute Wpk (partial angular velocities)
*/
        Wpk[2][2][2] = 1.;
        Wpk[2][3][2] = 1.;
        Wpk[2][4][2] = 1.;
        Wpk[2][5][2] = 1.;
        Wpk[2][6][2] = 1.;
        Wpk[2][7][2] = 1.;
        Wpk[2][8][2] = 1.;
        Wpk[2][9][2] = 1.;
        Wpk[2][10][2] = 1.;
        Wpk[2][11][1] = s11;
        Wpk[2][11][2] = c11;
        Wpk[2][12][0] = (s11*s12);
        Wpk[2][12][1] = (s11*c12);
        Wpk[2][12][2] = c11;
        Wpk[2][13][0] = ((Wpk[2][12][0]*c13)+(Wpk[2][12][1]*s13));
        Wpk[2][13][1] = ((Wpk[2][12][1]*c13)-(Wpk[2][12][0]*s13));
        Wpk[2][13][2] = c11;
        Wpk[2][14][2] = 1.;
        Wpk[2][15][2] = 1.;
        Wpk[2][16][2] = 1.;
        Wpk[2][17][2] = 1.;
        Wpk[2][18][2] = 1.;
        Wpk[2][19][2] = 1.;
        Wpk[2][20][2] = 1.;
        Wpk[2][21][2] = 1.;
        Wpk[2][22][1] = s22;
        Wpk[2][22][2] = c22;
        Wpk[2][23][0] = (s22*s23);
        Wpk[2][23][1] = (s22*c23);
        Wpk[2][23][2] = c22;
        Wpk[2][24][0] = ((Wpk[2][23][0]*c24)+(Wpk[2][23][1]*s24));
        Wpk[2][24][1] = ((Wpk[2][23][1]*c24)-(Wpk[2][23][0]*s24));
        Wpk[2][24][2] = c22;
        Wpk[3][3][2] = 1.;
        Wpk[3][4][2] = 1.;
        Wpk[3][5][2] = 1.;
        Wpk[3][6][2] = 1.;
        Wpk[3][7][2] = 1.;
        Wpk[3][8][2] = 1.;
        Wpk[3][9][2] = 1.;
        Wpk[3][10][2] = 1.;
        Wpk[3][11][1] = s11;
        Wpk[3][11][2] = c11;
        Wpk[3][12][0] = (s11*s12);
        Wpk[3][12][1] = (s11*c12);
        Wpk[3][12][2] = c11;
        Wpk[3][13][0] = ((Wpk[3][12][0]*c13)+(Wpk[3][12][1]*s13));
        Wpk[3][13][1] = ((Wpk[3][12][1]*c13)-(Wpk[3][12][0]*s13));
        Wpk[3][13][2] = c11;
        Wpk[6][6][2] = 1.;
        Wpk[6][7][2] = 1.;
        Wpk[6][8][2] = 1.;
        Wpk[6][9][2] = 1.;
        Wpk[6][10][2] = 1.;
        Wpk[6][11][1] = s11;
        Wpk[6][11][2] = c11;
        Wpk[6][12][0] = (s11*s12);
        Wpk[6][12][1] = (s11*c12);
        Wpk[6][12][2] = c11;
        Wpk[6][13][0] = ((Wpk[6][12][0]*c13)+(Wpk[6][12][1]*s13));
        Wpk[6][13][1] = ((Wpk[6][12][1]*c13)-(Wpk[6][12][0]*s13));
        Wpk[6][13][2] = c11;
        Wpk[9][9][2] = 1.;
        Wpk[10][10][2] = 1.;
        Wpk[10][11][1] = s11;
        Wpk[10][11][2] = c11;
        Wpk[10][12][0] = (s11*s12);
        Wpk[10][12][1] = (s11*c12);
        Wpk[10][12][2] = c11;
        Wpk[10][13][0] = ((Wpk[10][12][0]*c13)+(Wpk[10][12][1]*s13));
        Wpk[10][13][1] = ((Wpk[10][12][1]*c13)-(Wpk[10][12][0]*s13));
        Wpk[10][13][2] = c11;
        Wpk[11][11][0] = 1.;
        Wpk[11][12][0] = c12;
        Wpk[11][12][1] = -s12;
        Wpk[11][13][0] = ((c12*c13)-(s12*s13));
        Wpk[11][13][1] = -((s12*c13)+(s13*c12));
        Wpk[12][12][2] = 1.;
        Wpk[12][13][2] = 1.;
        Wpk[13][13][2] = 1.;
        Wpk[14][14][2] = 1.;
        Wpk[14][15][2] = 1.;
        Wpk[14][16][2] = 1.;
        Wpk[14][17][2] = 1.;
        Wpk[14][18][2] = 1.;
        Wpk[14][19][2] = 1.;
        Wpk[14][20][2] = 1.;
        Wpk[14][21][2] = 1.;
        Wpk[14][22][1] = s22;
        Wpk[14][22][2] = c22;
        Wpk[14][23][0] = (s22*s23);
        Wpk[14][23][1] = (s22*c23);
        Wpk[14][23][2] = c22;
        Wpk[14][24][0] = ((Wpk[14][23][0]*c24)+(Wpk[14][23][1]*s24));
        Wpk[14][24][1] = ((Wpk[14][23][1]*c24)-(Wpk[14][23][0]*s24));
        Wpk[14][24][2] = c22;
        Wpk[17][17][2] = 1.;
        Wpk[17][18][2] = 1.;
        Wpk[17][19][2] = 1.;
        Wpk[17][20][2] = 1.;
        Wpk[17][21][2] = 1.;
        Wpk[17][22][1] = s22;
        Wpk[17][22][2] = c22;
        Wpk[17][23][0] = (s22*s23);
        Wpk[17][23][1] = (s22*c23);
        Wpk[17][23][2] = c22;
        Wpk[17][24][0] = ((Wpk[17][23][0]*c24)+(Wpk[17][23][1]*s24));
        Wpk[17][24][1] = ((Wpk[17][23][1]*c24)-(Wpk[17][23][0]*s24));
        Wpk[17][24][2] = c22;
        Wpk[20][20][2] = 1.;
        Wpk[21][21][2] = 1.;
        Wpk[21][22][1] = s22;
        Wpk[21][22][2] = c22;
        Wpk[21][23][0] = (s22*s23);
        Wpk[21][23][1] = (s22*c23);
        Wpk[21][23][2] = c22;
        Wpk[21][24][0] = ((Wpk[21][23][0]*c24)+(Wpk[21][23][1]*s24));
        Wpk[21][24][1] = ((Wpk[21][23][1]*c24)-(Wpk[21][23][0]*s24));
        Wpk[21][24][2] = c22;
        Wpk[22][22][0] = 1.;
        Wpk[22][23][0] = c23;
        Wpk[22][23][1] = -s23;
        Wpk[22][24][0] = ((c23*c24)-(s23*s24));
        Wpk[22][24][1] = -((s23*c24)+(s24*c23));
        Wpk[23][23][2] = 1.;
        Wpk[23][24][2] = 1.;
        Wpk[24][24][2] = 1.;
/*
Compute Vpk (partial velocities)
*/
        Vpk[0][0][0] = 1.;
        Vpk[0][1][0] = 1.;
        Vpk[0][2][0] = c2;
        Vpk[0][2][1] = -s2;
        Vpk[0][3][0] = ((c2*c3)-(s2*s3));
        Vpk[0][3][1] = -((s2*c3)+(s3*c2));
        Vpk[0][4][0] = Vpk[0][3][0];
        Vpk[0][4][1] = Vpk[0][3][1];
        Vpk[0][5][0] = Vpk[0][3][0];
        Vpk[0][5][1] = Vpk[0][3][1];
        Vpk[0][6][0] = ((Vpk[0][3][0]*c6)+(Vpk[0][3][1]*s6));
        Vpk[0][6][1] = ((Vpk[0][3][1]*c6)-(Vpk[0][3][0]*s6));
        Vpk[0][7][0] = Vpk[0][6][0];
        Vpk[0][7][1] = Vpk[0][6][1];
        Vpk[0][8][0] = Vpk[0][6][0];
        Vpk[0][8][1] = Vpk[0][6][1];
        Vpk[0][9][0] = ((Vpk[0][6][0]*c9)+(Vpk[0][6][1]*s9));
        Vpk[0][9][1] = ((Vpk[0][6][1]*c9)-(Vpk[0][6][0]*s9));
        Vpk[0][10][0] = ((Vpk[0][6][0]*c10)+(Vpk[0][6][1]*s10));
        Vpk[0][10][1] = ((Vpk[0][6][1]*c10)-(Vpk[0][6][0]*s10));
        Vpk[0][11][0] = Vpk[0][10][0];
        Vpk[0][11][1] = (Vpk[0][10][1]*c11);
        Vpk[0][11][2] = -(Vpk[0][10][1]*s11);
        Vpk[0][12][0] = ((Vpk[0][10][0]*c12)+(Vpk[0][11][1]*s12));
        Vpk[0][12][1] = ((Vpk[0][11][1]*c12)-(Vpk[0][10][0]*s12));
        Vpk[0][12][2] = Vpk[0][11][2];
        Vpk[0][13][0] = ((Vpk[0][12][0]*c13)+(Vpk[0][12][1]*s13));
        Vpk[0][13][1] = ((Vpk[0][12][1]*c13)-(Vpk[0][12][0]*s13));
        Vpk[0][13][2] = Vpk[0][11][2];
        Vpk[0][14][0] = ((c2*c14)-(s2*s14));
        Vpk[0][14][1] = -((s2*c14)+(s14*c2));
        Vpk[0][15][0] = Vpk[0][14][0];
        Vpk[0][15][1] = Vpk[0][14][1];
        Vpk[0][16][0] = Vpk[0][14][0];
        Vpk[0][16][1] = Vpk[0][14][1];
        Vpk[0][17][0] = ((Vpk[0][14][0]*c17)+(Vpk[0][14][1]*s17));
        Vpk[0][17][1] = ((Vpk[0][14][1]*c17)-(Vpk[0][14][0]*s17));
        Vpk[0][18][0] = Vpk[0][17][0];
        Vpk[0][18][1] = Vpk[0][17][1];
        Vpk[0][19][0] = Vpk[0][17][0];
        Vpk[0][19][1] = Vpk[0][17][1];
        Vpk[0][20][0] = ((Vpk[0][17][0]*c20)+(Vpk[0][17][1]*s20));
        Vpk[0][20][1] = ((Vpk[0][17][1]*c20)-(Vpk[0][17][0]*s20));
        Vpk[0][21][0] = ((Vpk[0][17][0]*c21)+(Vpk[0][17][1]*s21));
        Vpk[0][21][1] = ((Vpk[0][17][1]*c21)-(Vpk[0][17][0]*s21));
        Vpk[0][22][0] = Vpk[0][21][0];
        Vpk[0][22][1] = (Vpk[0][21][1]*c22);
        Vpk[0][22][2] = -(Vpk[0][21][1]*s22);
        Vpk[0][23][0] = ((Vpk[0][21][0]*c23)+(Vpk[0][22][1]*s23));
        Vpk[0][23][1] = ((Vpk[0][22][1]*c23)-(Vpk[0][21][0]*s23));
        Vpk[0][23][2] = Vpk[0][22][2];
        Vpk[0][24][0] = ((Vpk[0][23][0]*c24)+(Vpk[0][23][1]*s24));
        Vpk[0][24][1] = ((Vpk[0][23][1]*c24)-(Vpk[0][23][0]*s24));
        Vpk[0][24][2] = Vpk[0][22][2];
        Vpk[1][1][1] = 1.;
        Vpk[1][2][0] = s2;
        Vpk[1][2][1] = c2;
        Vpk[1][3][0] = ((s2*c3)+(s3*c2));
        Vpk[1][3][1] = ((c2*c3)-(s2*s3));
        Vpk[1][4][0] = Vpk[1][3][0];
        Vpk[1][4][1] = Vpk[1][3][1];
        Vpk[1][5][0] = Vpk[1][3][0];
        Vpk[1][5][1] = Vpk[1][3][1];
        Vpk[1][6][0] = ((Vpk[1][3][0]*c6)+(Vpk[1][3][1]*s6));
        Vpk[1][6][1] = ((Vpk[1][3][1]*c6)-(Vpk[1][3][0]*s6));
        Vpk[1][7][0] = Vpk[1][6][0];
        Vpk[1][7][1] = Vpk[1][6][1];
        Vpk[1][8][0] = Vpk[1][6][0];
        Vpk[1][8][1] = Vpk[1][6][1];
        Vpk[1][9][0] = ((Vpk[1][6][0]*c9)+(Vpk[1][6][1]*s9));
        Vpk[1][9][1] = ((Vpk[1][6][1]*c9)-(Vpk[1][6][0]*s9));
        Vpk[1][10][0] = ((Vpk[1][6][0]*c10)+(Vpk[1][6][1]*s10));
        Vpk[1][10][1] = ((Vpk[1][6][1]*c10)-(Vpk[1][6][0]*s10));
        Vpk[1][11][0] = Vpk[1][10][0];
        Vpk[1][11][1] = (Vpk[1][10][1]*c11);
        Vpk[1][11][2] = -(Vpk[1][10][1]*s11);
        Vpk[1][12][0] = ((Vpk[1][10][0]*c12)+(Vpk[1][11][1]*s12));
        Vpk[1][12][1] = ((Vpk[1][11][1]*c12)-(Vpk[1][10][0]*s12));
        Vpk[1][12][2] = Vpk[1][11][2];
        Vpk[1][13][0] = ((Vpk[1][12][0]*c13)+(Vpk[1][12][1]*s13));
        Vpk[1][13][1] = ((Vpk[1][12][1]*c13)-(Vpk[1][12][0]*s13));
        Vpk[1][13][2] = Vpk[1][11][2];
        Vpk[1][14][0] = ((s2*c14)+(s14*c2));
        Vpk[1][14][1] = ((c2*c14)-(s2*s14));
        Vpk[1][15][0] = Vpk[1][14][0];
        Vpk[1][15][1] = Vpk[1][14][1];
        Vpk[1][16][0] = Vpk[1][14][0];
        Vpk[1][16][1] = Vpk[1][14][1];
        Vpk[1][17][0] = ((Vpk[1][14][0]*c17)+(Vpk[1][14][1]*s17));
        Vpk[1][17][1] = ((Vpk[1][14][1]*c17)-(Vpk[1][14][0]*s17));
        Vpk[1][18][0] = Vpk[1][17][0];
        Vpk[1][18][1] = Vpk[1][17][1];
        Vpk[1][19][0] = Vpk[1][17][0];
        Vpk[1][19][1] = Vpk[1][17][1];
        Vpk[1][20][0] = ((Vpk[1][17][0]*c20)+(Vpk[1][17][1]*s20));
        Vpk[1][20][1] = ((Vpk[1][17][1]*c20)-(Vpk[1][17][0]*s20));
        Vpk[1][21][0] = ((Vpk[1][17][0]*c21)+(Vpk[1][17][1]*s21));
        Vpk[1][21][1] = ((Vpk[1][17][1]*c21)-(Vpk[1][17][0]*s21));
        Vpk[1][22][0] = Vpk[1][21][0];
        Vpk[1][22][1] = (Vpk[1][21][1]*c22);
        Vpk[1][22][2] = -(Vpk[1][21][1]*s22);
        Vpk[1][23][0] = ((Vpk[1][21][0]*c23)+(Vpk[1][22][1]*s23));
        Vpk[1][23][1] = ((Vpk[1][22][1]*c23)-(Vpk[1][21][0]*s23));
        Vpk[1][23][2] = Vpk[1][22][2];
        Vpk[1][24][0] = ((Vpk[1][23][0]*c24)+(Vpk[1][23][1]*s24));
        Vpk[1][24][1] = ((Vpk[1][23][1]*c24)-(Vpk[1][23][0]*s24));
        Vpk[1][24][2] = Vpk[1][22][2];
        Vpk[2][2][0] = -.2639;
        Vpk[2][2][1] = -.0707;
        Vpk[2][3][0] = (.17+((.0661*c3)-(.0707*s3)));
        Vpk[2][3][1] = -((.0661*s3)+(.0707*c3));
        VWri[2][4][0] = (Vpk[2][3][0]-.17);
        Vpk[2][4][0] = VWri[2][4][0];
        Vpk[2][4][1] = (q[4]+Vpk[2][3][1]);
        Vpk[2][5][0] = (VWri[2][4][0]-q[5]);
        Vpk[2][5][1] = Vpk[2][4][1];
        Vpk[2][6][0] = (.1867+((Vpk[2][4][1]*s6)+(Vpk[2][5][0]*c6)));
        Vpk[2][6][1] = ((Vpk[2][4][1]*c6)-(Vpk[2][5][0]*s6));
        VWri[2][7][0] = (Vpk[2][6][0]-.1867);
        Vpk[2][7][0] = VWri[2][7][0];
        Vpk[2][7][1] = (q[7]+Vpk[2][6][1]);
        Vpk[2][8][0] = (VWri[2][7][0]-q[8]);
        Vpk[2][8][1] = Vpk[2][7][1];
        Vpk[2][9][0] = ((Vpk[2][7][1]*s9)+(Vpk[2][8][0]*c9));
        Vpk[2][9][1] = ((Vpk[2][7][1]*c9)-(Vpk[2][8][0]*s9));
        VWri[2][10][0] = (.2433+Vpk[2][6][0]);
        Vpk[2][10][0] = ((Vpk[2][6][1]*s10)+(VWri[2][10][0]*c10));
        Vpk[2][10][1] = ((Vpk[2][6][1]*c10)-(VWri[2][10][0]*s10));
        VWri[2][11][0] = (.04195+Vpk[2][10][0]);
        VWri[2][11][1] = (Vpk[2][10][1]-.04877);
        Vpk[2][11][0] = (VWri[2][11][0]-(.015*c11));
        Vpk[2][11][1] = ((.025*c11)+(VWri[2][11][1]*c11));
        Vpk[2][11][2] = -((.025*s11)+(VWri[2][11][1]*s11));
        VWri[2][12][0] = (Vpk[2][11][0]+(.015*c11));
        VWri[2][12][1] = (Vpk[2][11][1]+(.0942*c11));
        VWri[2][12][2] = (Vpk[2][11][2]-(.0942*s11));
        Vpk[2][12][0] = (((VWri[2][12][0]*c12)+(VWri[2][12][1]*s12))-(.015*c11))
          ;
        Vpk[2][12][1] = ((.025*c11)+((VWri[2][12][1]*c12)-(VWri[2][12][0]*s12)))
          ;
        Vpk[2][12][2] = (VWri[2][12][2]+((.015*Wpk[2][12][0])-(.025*
          Wpk[2][12][1])));
        VWri[2][13][0] = (Vpk[2][12][0]+((.00108*Wpk[2][12][1])+(.017*c11)));
        VWri[2][13][1] = (Vpk[2][12][1]+((.0346*c11)-(.00108*Wpk[2][12][0])));
        VWri[2][13][2] = (Vpk[2][12][2]-((.017*Wpk[2][12][0])+(.0346*
          Wpk[2][12][1])));
        Vpk[2][13][0] = (((VWri[2][13][0]*c13)+(VWri[2][13][1]*s13))-((.006*c11)
          +(.0175*Wpk[2][13][1])));
        Vpk[2][13][1] = (((.0175*Wpk[2][13][0])+(.0346*c11))+((VWri[2][13][1]*
          c13)-(VWri[2][13][0]*s13)));
        Vpk[2][13][2] = (VWri[2][13][2]+((.006*Wpk[2][13][0])-(.0346*
          Wpk[2][13][1])));
        Vpk[2][14][0] = (.17+((.0661*c14)-(.0707*s14)));
        Vpk[2][14][1] = -((.0661*s14)+(.0707*c14));
        VWri[2][15][0] = (Vpk[2][14][0]-.17);
        Vpk[2][15][0] = VWri[2][15][0];
        Vpk[2][15][1] = (q[15]+Vpk[2][14][1]);
        Vpk[2][16][0] = (VWri[2][15][0]-q[16]);
        Vpk[2][16][1] = Vpk[2][15][1];
        Vpk[2][17][0] = (.1867+((Vpk[2][15][1]*s17)+(Vpk[2][16][0]*c17)));
        Vpk[2][17][1] = ((Vpk[2][15][1]*c17)-(Vpk[2][16][0]*s17));
        VWri[2][18][0] = (Vpk[2][17][0]-.1867);
        Vpk[2][18][0] = VWri[2][18][0];
        Vpk[2][18][1] = (q[18]+Vpk[2][17][1]);
        Vpk[2][19][0] = (VWri[2][18][0]-q[19]);
        Vpk[2][19][1] = Vpk[2][18][1];
        Vpk[2][20][0] = ((Vpk[2][18][1]*s20)+(Vpk[2][19][0]*c20));
        Vpk[2][20][1] = ((Vpk[2][18][1]*c20)-(Vpk[2][19][0]*s20));
        VWri[2][21][0] = (.2433+Vpk[2][17][0]);
        Vpk[2][21][0] = ((Vpk[2][17][1]*s21)+(VWri[2][21][0]*c21));
        Vpk[2][21][1] = ((Vpk[2][17][1]*c21)-(VWri[2][21][0]*s21));
        VWri[2][22][0] = (.04195+Vpk[2][21][0]);
        VWri[2][22][1] = (Vpk[2][21][1]-.04877);
        Vpk[2][22][0] = (VWri[2][22][0]-(.015*c22));
        Vpk[2][22][1] = ((.025*c22)+(VWri[2][22][1]*c22));
        Vpk[2][22][2] = -((.025*s22)+(VWri[2][22][1]*s22));
        VWri[2][23][0] = (Vpk[2][22][0]+(.015*c22));
        VWri[2][23][1] = (Vpk[2][22][1]+(.0942*c22));
        VWri[2][23][2] = (Vpk[2][22][2]-(.0942*s22));
        Vpk[2][23][0] = (((VWri[2][23][0]*c23)+(VWri[2][23][1]*s23))-(.015*c22))
          ;
        Vpk[2][23][1] = ((.025*c22)+((VWri[2][23][1]*c23)-(VWri[2][23][0]*s23)))
          ;
        Vpk[2][23][2] = (VWri[2][23][2]+((.015*Wpk[2][23][0])-(.025*
          Wpk[2][23][1])));
        VWri[2][24][0] = (Vpk[2][23][0]+((.017*c22)-(.00108*Wpk[2][23][1])));
        VWri[2][24][1] = (Vpk[2][23][1]+((.00108*Wpk[2][23][0])+(.0346*c22)));
        VWri[2][24][2] = (Vpk[2][23][2]-((.017*Wpk[2][23][0])+(.0346*
          Wpk[2][23][1])));
        Vpk[2][24][0] = (((.0175*Wpk[2][24][1])-(.006*c22))+((VWri[2][24][0]*c24
          )+(VWri[2][24][1]*s24)));
        Vpk[2][24][1] = (((.0346*c22)-(.0175*Wpk[2][24][0]))+((VWri[2][24][1]*
          c24)-(VWri[2][24][0]*s24)));
        Vpk[2][24][2] = (VWri[2][24][2]+((.006*Wpk[2][24][0])-(.0346*
          Wpk[2][24][1])));
        Vpk[3][3][0] = .17;
        Vpk[3][4][1] = q[4];
        Vpk[3][5][0] = -q[5];
        Vpk[3][5][1] = q[4];
        Vpk[3][6][0] = (.1867+((q[4]*s6)-(q[5]*c6)));
        Vpk[3][6][1] = ((q[4]*c6)+(q[5]*s6));
        VWri[3][7][0] = (Vpk[3][6][0]-.1867);
        Vpk[3][7][0] = VWri[3][7][0];
        Vpk[3][7][1] = (q[7]+Vpk[3][6][1]);
        Vpk[3][8][0] = (VWri[3][7][0]-q[8]);
        Vpk[3][8][1] = Vpk[3][7][1];
        Vpk[3][9][0] = ((Vpk[3][7][1]*s9)+(Vpk[3][8][0]*c9));
        Vpk[3][9][1] = ((Vpk[3][7][1]*c9)-(Vpk[3][8][0]*s9));
        VWri[3][10][0] = (.2433+Vpk[3][6][0]);
        Vpk[3][10][0] = ((Vpk[3][6][1]*s10)+(VWri[3][10][0]*c10));
        Vpk[3][10][1] = ((Vpk[3][6][1]*c10)-(VWri[3][10][0]*s10));
        VWri[3][11][0] = (.04195+Vpk[3][10][0]);
        VWri[3][11][1] = (Vpk[3][10][1]-.04877);
        Vpk[3][11][0] = (VWri[3][11][0]-(.015*c11));
        Vpk[3][11][1] = ((.025*c11)+(VWri[3][11][1]*c11));
        Vpk[3][11][2] = -((.025*s11)+(VWri[3][11][1]*s11));
        VWri[3][12][0] = (Vpk[3][11][0]+(.015*c11));
        VWri[3][12][1] = (Vpk[3][11][1]+(.0942*c11));
        VWri[3][12][2] = (Vpk[3][11][2]-(.0942*s11));
        Vpk[3][12][0] = (((VWri[3][12][0]*c12)+(VWri[3][12][1]*s12))-(.015*c11))
          ;
        Vpk[3][12][1] = ((.025*c11)+((VWri[3][12][1]*c12)-(VWri[3][12][0]*s12)))
          ;
        Vpk[3][12][2] = (VWri[3][12][2]+((.015*Wpk[3][12][0])-(.025*
          Wpk[3][12][1])));
        VWri[3][13][0] = (Vpk[3][12][0]+((.00108*Wpk[3][12][1])+(.017*c11)));
        VWri[3][13][1] = (Vpk[3][12][1]+((.0346*c11)-(.00108*Wpk[3][12][0])));
        VWri[3][13][2] = (Vpk[3][12][2]-((.017*Wpk[3][12][0])+(.0346*
          Wpk[3][12][1])));
        Vpk[3][13][0] = (((VWri[3][13][0]*c13)+(VWri[3][13][1]*s13))-((.006*c11)
          +(.0175*Wpk[3][13][1])));
        Vpk[3][13][1] = (((.0175*Wpk[3][13][0])+(.0346*c11))+((VWri[3][13][1]*
          c13)-(VWri[3][13][0]*s13)));
        Vpk[3][13][2] = (VWri[3][13][2]+((.006*Wpk[3][13][0])-(.0346*
          Wpk[3][13][1])));
        Vpk[4][4][0] = 1.;
        Vpk[4][5][0] = 1.;
        Vpk[4][6][0] = c6;
        Vpk[4][6][1] = -s6;
        Vpk[4][7][0] = c6;
        Vpk[4][7][1] = -s6;
        Vpk[4][8][0] = c6;
        Vpk[4][8][1] = -s6;
        Vpk[4][9][0] = ((c6*c9)-(s6*s9));
        Vpk[4][9][1] = -((s6*c9)+(s9*c6));
        Vpk[4][10][0] = ((c6*c10)-(s6*s10));
        Vpk[4][10][1] = -((s6*c10)+(s10*c6));
        Vpk[4][11][0] = Vpk[4][10][0];
        Vpk[4][11][1] = (Vpk[4][10][1]*c11);
        Vpk[4][11][2] = -(Vpk[4][10][1]*s11);
        Vpk[4][12][0] = ((Vpk[4][10][0]*c12)+(Vpk[4][11][1]*s12));
        Vpk[4][12][1] = ((Vpk[4][11][1]*c12)-(Vpk[4][10][0]*s12));
        Vpk[4][12][2] = Vpk[4][11][2];
        Vpk[4][13][0] = ((Vpk[4][12][0]*c13)+(Vpk[4][12][1]*s13));
        Vpk[4][13][1] = ((Vpk[4][12][1]*c13)-(Vpk[4][12][0]*s13));
        Vpk[4][13][2] = Vpk[4][11][2];
        Vpk[5][5][1] = 1.;
        Vpk[5][6][0] = s6;
        Vpk[5][6][1] = c6;
        Vpk[5][7][0] = s6;
        Vpk[5][7][1] = c6;
        Vpk[5][8][0] = s6;
        Vpk[5][8][1] = c6;
        Vpk[5][9][0] = ((s6*c9)+(s9*c6));
        Vpk[5][9][1] = ((c6*c9)-(s6*s9));
        Vpk[5][10][0] = ((s6*c10)+(s10*c6));
        Vpk[5][10][1] = ((c6*c10)-(s6*s10));
        Vpk[5][11][0] = Vpk[5][10][0];
        Vpk[5][11][1] = (Vpk[5][10][1]*c11);
        Vpk[5][11][2] = -(Vpk[5][10][1]*s11);
        Vpk[5][12][0] = ((Vpk[5][10][0]*c12)+(Vpk[5][11][1]*s12));
        Vpk[5][12][1] = ((Vpk[5][11][1]*c12)-(Vpk[5][10][0]*s12));
        Vpk[5][12][2] = Vpk[5][11][2];
        Vpk[5][13][0] = ((Vpk[5][12][0]*c13)+(Vpk[5][12][1]*s13));
        Vpk[5][13][1] = ((Vpk[5][12][1]*c13)-(Vpk[5][12][0]*s13));
        Vpk[5][13][2] = Vpk[5][11][2];
        Vpk[6][6][0] = .1867;
        Vpk[6][7][1] = q[7];
        Vpk[6][8][0] = -q[8];
        Vpk[6][8][1] = q[7];
        Vpk[6][9][0] = ((q[7]*s9)-(q[8]*c9));
        Vpk[6][9][1] = ((q[7]*c9)+(q[8]*s9));
        Vpk[6][10][0] = (.43*c10);
        Vpk[6][10][1] = -(.43*s10);
        VWri[6][11][0] = (.04195+(.43*c10));
        VWri[6][11][1] = -(.04877+(.43*s10));
        Vpk[6][11][0] = (VWri[6][11][0]-(.015*c11));
        Vpk[6][11][1] = ((.025*c11)+(VWri[6][11][1]*c11));
        Vpk[6][11][2] = -((.025*s11)+(VWri[6][11][1]*s11));
        VWri[6][12][0] = (Vpk[6][11][0]+(.015*c11));
        VWri[6][12][1] = (Vpk[6][11][1]+(.0942*c11));
        VWri[6][12][2] = (Vpk[6][11][2]-(.0942*s11));
        Vpk[6][12][0] = (((VWri[6][12][0]*c12)+(VWri[6][12][1]*s12))-(.015*c11))
          ;
        Vpk[6][12][1] = ((.025*c11)+((VWri[6][12][1]*c12)-(VWri[6][12][0]*s12)))
          ;
        Vpk[6][12][2] = (VWri[6][12][2]+((.015*Wpk[6][12][0])-(.025*
          Wpk[6][12][1])));
        VWri[6][13][0] = (Vpk[6][12][0]+((.00108*Wpk[6][12][1])+(.017*c11)));
        VWri[6][13][1] = (Vpk[6][12][1]+((.0346*c11)-(.00108*Wpk[6][12][0])));
        VWri[6][13][2] = (Vpk[6][12][2]-((.017*Wpk[6][12][0])+(.0346*
          Wpk[6][12][1])));
        Vpk[6][13][0] = (((VWri[6][13][0]*c13)+(VWri[6][13][1]*s13))-((.006*c11)
          +(.0175*Wpk[6][13][1])));
        Vpk[6][13][1] = (((.0175*Wpk[6][13][0])+(.0346*c11))+((VWri[6][13][1]*
          c13)-(VWri[6][13][0]*s13)));
        Vpk[6][13][2] = (VWri[6][13][2]+((.006*Wpk[6][13][0])-(.0346*
          Wpk[6][13][1])));
        Vpk[7][7][0] = 1.;
        Vpk[7][8][0] = 1.;
        Vpk[7][9][0] = c9;
        Vpk[7][9][1] = -s9;
        Vpk[8][8][1] = 1.;
        Vpk[8][9][0] = s9;
        Vpk[8][9][1] = c9;
        Vpk[10][11][0] = (.04195-(.015*c11));
        Vpk[10][11][1] = -(.02377*c11);
        Vpk[10][11][2] = (.02377*s11);
        VWri[10][12][0] = (Vpk[10][11][0]+(.015*c11));
        Vpk[10][12][0] = (((.07043*(s12*c11))+(VWri[10][12][0]*c12))-(.015*c11))
          ;
        Vpk[10][12][1] = ((.025*c11)+((.07043*(c11*c12))-(VWri[10][12][0]*s12)))
          ;
        Vpk[10][12][2] = (((.015*Wpk[10][12][0])-(.025*Wpk[10][12][1]))-(.07043*
          s11));
        VWri[10][13][0] = (Vpk[10][12][0]+((.00108*Wpk[10][12][1])+(.017*c11)));
        VWri[10][13][1] = (Vpk[10][12][1]+((.0346*c11)-(.00108*Wpk[10][12][0])))
          ;
        VWri[10][13][2] = (Vpk[10][12][2]-((.017*Wpk[10][12][0])+(.0346*
          Wpk[10][12][1])));
        Vpk[10][13][0] = (((VWri[10][13][0]*c13)+(VWri[10][13][1]*s13))-((.006*
          c11)+(.0175*Wpk[10][13][1])));
        Vpk[10][13][1] = (((.0175*Wpk[10][13][0])+(.0346*c11))+((VWri[10][13][1]
          *c13)-(VWri[10][13][0]*s13)));
        Vpk[10][13][2] = (VWri[10][13][2]+((.006*Wpk[10][13][0])-(.0346*
          Wpk[10][13][1])));
        Vpk[11][11][2] = .015;
        Vpk[11][12][2] = ((.015*c12)+(.025*s12));
        VWri[11][13][2] = (Vpk[11][12][2]+((.0346*s12)-(.017*c12)));
        Vpk[11][13][0] = -((.00108*((s12*c13)+(s13*c12)))+(.0175*Wpk[11][13][1])
          );
        Vpk[11][13][1] = ((.00108*((s12*s13)-(c12*c13)))+(.0175*Wpk[11][13][0]))
          ;
        Vpk[11][13][2] = (VWri[11][13][2]+((.006*Wpk[11][13][0])-(.0346*
          Wpk[11][13][1])));
        Vpk[12][12][0] = -.015;
        Vpk[12][12][1] = .025;
        Vpk[12][13][0] = (((.002*c13)+(.0596*s13))-.006);
        Vpk[12][13][1] = (.0346+((.0596*c13)-(.002*s13)));
        Vpk[13][13][0] = -.006;
        Vpk[13][13][1] = .0346;
        Vpk[14][14][0] = .17;
        Vpk[14][15][1] = q[15];
        Vpk[14][16][0] = -q[16];
        Vpk[14][16][1] = q[15];
        Vpk[14][17][0] = (.1867+((q[15]*s17)-(q[16]*c17)));
        Vpk[14][17][1] = ((q[15]*c17)+(q[16]*s17));
        VWri[14][18][0] = (Vpk[14][17][0]-.1867);
        Vpk[14][18][0] = VWri[14][18][0];
        Vpk[14][18][1] = (q[18]+Vpk[14][17][1]);
        Vpk[14][19][0] = (VWri[14][18][0]-q[19]);
        Vpk[14][19][1] = Vpk[14][18][1];
        Vpk[14][20][0] = ((Vpk[14][18][1]*s20)+(Vpk[14][19][0]*c20));
        Vpk[14][20][1] = ((Vpk[14][18][1]*c20)-(Vpk[14][19][0]*s20));
        VWri[14][21][0] = (.2433+Vpk[14][17][0]);
        Vpk[14][21][0] = ((Vpk[14][17][1]*s21)+(VWri[14][21][0]*c21));
        Vpk[14][21][1] = ((Vpk[14][17][1]*c21)-(VWri[14][21][0]*s21));
        VWri[14][22][0] = (.04195+Vpk[14][21][0]);
        VWri[14][22][1] = (Vpk[14][21][1]-.04877);
        Vpk[14][22][0] = (VWri[14][22][0]-(.015*c22));
        Vpk[14][22][1] = ((.025*c22)+(VWri[14][22][1]*c22));
        Vpk[14][22][2] = -((.025*s22)+(VWri[14][22][1]*s22));
        VWri[14][23][0] = (Vpk[14][22][0]+(.015*c22));
        VWri[14][23][1] = (Vpk[14][22][1]+(.0942*c22));
        VWri[14][23][2] = (Vpk[14][22][2]-(.0942*s22));
        Vpk[14][23][0] = (((VWri[14][23][0]*c23)+(VWri[14][23][1]*s23))-(.015*
          c22));
        Vpk[14][23][1] = ((.025*c22)+((VWri[14][23][1]*c23)-(VWri[14][23][0]*s23
          )));
        Vpk[14][23][2] = (VWri[14][23][2]+((.015*Wpk[14][23][0])-(.025*
          Wpk[14][23][1])));
        VWri[14][24][0] = (Vpk[14][23][0]+((.017*c22)-(.00108*Wpk[14][23][1])));
        VWri[14][24][1] = (Vpk[14][23][1]+((.00108*Wpk[14][23][0])+(.0346*c22)))
          ;
        VWri[14][24][2] = (Vpk[14][23][2]-((.017*Wpk[14][23][0])+(.0346*
          Wpk[14][23][1])));
        Vpk[14][24][0] = (((.0175*Wpk[14][24][1])-(.006*c22))+((VWri[14][24][0]*
          c24)+(VWri[14][24][1]*s24)));
        Vpk[14][24][1] = (((.0346*c22)-(.0175*Wpk[14][24][0]))+((VWri[14][24][1]
          *c24)-(VWri[14][24][0]*s24)));
        Vpk[14][24][2] = (VWri[14][24][2]+((.006*Wpk[14][24][0])-(.0346*
          Wpk[14][24][1])));
        Vpk[15][15][0] = 1.;
        Vpk[15][16][0] = 1.;
        Vpk[15][17][0] = c17;
        Vpk[15][17][1] = -s17;
        Vpk[15][18][0] = c17;
        Vpk[15][18][1] = -s17;
        Vpk[15][19][0] = c17;
        Vpk[15][19][1] = -s17;
        Vpk[15][20][0] = ((c17*c20)-(s17*s20));
        Vpk[15][20][1] = -((s17*c20)+(s20*c17));
        Vpk[15][21][0] = ((c17*c21)-(s17*s21));
        Vpk[15][21][1] = -((s17*c21)+(s21*c17));
        Vpk[15][22][0] = Vpk[15][21][0];
        Vpk[15][22][1] = (Vpk[15][21][1]*c22);
        Vpk[15][22][2] = -(Vpk[15][21][1]*s22);
        Vpk[15][23][0] = ((Vpk[15][21][0]*c23)+(Vpk[15][22][1]*s23));
        Vpk[15][23][1] = ((Vpk[15][22][1]*c23)-(Vpk[15][21][0]*s23));
        Vpk[15][23][2] = Vpk[15][22][2];
        Vpk[15][24][0] = ((Vpk[15][23][0]*c24)+(Vpk[15][23][1]*s24));
        Vpk[15][24][1] = ((Vpk[15][23][1]*c24)-(Vpk[15][23][0]*s24));
        Vpk[15][24][2] = Vpk[15][22][2];
        Vpk[16][16][1] = 1.;
        Vpk[16][17][0] = s17;
        Vpk[16][17][1] = c17;
        Vpk[16][18][0] = s17;
        Vpk[16][18][1] = c17;
        Vpk[16][19][0] = s17;
        Vpk[16][19][1] = c17;
        Vpk[16][20][0] = ((s17*c20)+(s20*c17));
        Vpk[16][20][1] = ((c17*c20)-(s17*s20));
        Vpk[16][21][0] = ((s17*c21)+(s21*c17));
        Vpk[16][21][1] = ((c17*c21)-(s17*s21));
        Vpk[16][22][0] = Vpk[16][21][0];
        Vpk[16][22][1] = (Vpk[16][21][1]*c22);
        Vpk[16][22][2] = -(Vpk[16][21][1]*s22);
        Vpk[16][23][0] = ((Vpk[16][21][0]*c23)+(Vpk[16][22][1]*s23));
        Vpk[16][23][1] = ((Vpk[16][22][1]*c23)-(Vpk[16][21][0]*s23));
        Vpk[16][23][2] = Vpk[16][22][2];
        Vpk[16][24][0] = ((Vpk[16][23][0]*c24)+(Vpk[16][23][1]*s24));
        Vpk[16][24][1] = ((Vpk[16][23][1]*c24)-(Vpk[16][23][0]*s24));
        Vpk[16][24][2] = Vpk[16][22][2];
        Vpk[17][17][0] = .1867;
        Vpk[17][18][1] = q[18];
        Vpk[17][19][0] = -q[19];
        Vpk[17][19][1] = q[18];
        Vpk[17][20][0] = ((q[18]*s20)-(q[19]*c20));
        Vpk[17][20][1] = ((q[18]*c20)+(q[19]*s20));
        Vpk[17][21][0] = (.43*c21);
        Vpk[17][21][1] = -(.43*s21);
        VWri[17][22][0] = (.04195+(.43*c21));
        VWri[17][22][1] = -(.04877+(.43*s21));
        Vpk[17][22][0] = (VWri[17][22][0]-(.015*c22));
        Vpk[17][22][1] = ((.025*c22)+(VWri[17][22][1]*c22));
        Vpk[17][22][2] = -((.025*s22)+(VWri[17][22][1]*s22));
        VWri[17][23][0] = (Vpk[17][22][0]+(.015*c22));
        VWri[17][23][1] = (Vpk[17][22][1]+(.0942*c22));
        VWri[17][23][2] = (Vpk[17][22][2]-(.0942*s22));
        Vpk[17][23][0] = (((VWri[17][23][0]*c23)+(VWri[17][23][1]*s23))-(.015*
          c22));
        Vpk[17][23][1] = ((.025*c22)+((VWri[17][23][1]*c23)-(VWri[17][23][0]*s23
          )));
        Vpk[17][23][2] = (VWri[17][23][2]+((.015*Wpk[17][23][0])-(.025*
          Wpk[17][23][1])));
        VWri[17][24][0] = (Vpk[17][23][0]+((.017*c22)-(.00108*Wpk[17][23][1])));
        VWri[17][24][1] = (Vpk[17][23][1]+((.00108*Wpk[17][23][0])+(.0346*c22)))
          ;
        VWri[17][24][2] = (Vpk[17][23][2]-((.017*Wpk[17][23][0])+(.0346*
          Wpk[17][23][1])));
        Vpk[17][24][0] = (((.0175*Wpk[17][24][1])-(.006*c22))+((VWri[17][24][0]*
          c24)+(VWri[17][24][1]*s24)));
        Vpk[17][24][1] = (((.0346*c22)-(.0175*Wpk[17][24][0]))+((VWri[17][24][1]
          *c24)-(VWri[17][24][0]*s24)));
        Vpk[17][24][2] = (VWri[17][24][2]+((.006*Wpk[17][24][0])-(.0346*
          Wpk[17][24][1])));
        Vpk[18][18][0] = 1.;
        Vpk[18][19][0] = 1.;
        Vpk[18][20][0] = c20;
        Vpk[18][20][1] = -s20;
        Vpk[19][19][1] = 1.;
        Vpk[19][20][0] = s20;
        Vpk[19][20][1] = c20;
        Vpk[21][22][0] = (.04195-(.015*c22));
        Vpk[21][22][1] = -(.02377*c22);
        Vpk[21][22][2] = (.02377*s22);
        VWri[21][23][0] = (Vpk[21][22][0]+(.015*c22));
        Vpk[21][23][0] = (((.07043*(s23*c22))+(VWri[21][23][0]*c23))-(.015*c22))
          ;
        Vpk[21][23][1] = ((.025*c22)+((.07043*(c22*c23))-(VWri[21][23][0]*s23)))
          ;
        Vpk[21][23][2] = (((.015*Wpk[21][23][0])-(.025*Wpk[21][23][1]))-(.07043*
          s22));
        VWri[21][24][0] = (Vpk[21][23][0]+((.017*c22)-(.00108*Wpk[21][23][1])));
        VWri[21][24][1] = (Vpk[21][23][1]+((.00108*Wpk[21][23][0])+(.0346*c22)))
          ;
        VWri[21][24][2] = (Vpk[21][23][2]-((.017*Wpk[21][23][0])+(.0346*
          Wpk[21][23][1])));
        Vpk[21][24][0] = (((.0175*Wpk[21][24][1])-(.006*c22))+((VWri[21][24][0]*
          c24)+(VWri[21][24][1]*s24)));
        Vpk[21][24][1] = (((.0346*c22)-(.0175*Wpk[21][24][0]))+((VWri[21][24][1]
          *c24)-(VWri[21][24][0]*s24)));
        Vpk[21][24][2] = (VWri[21][24][2]+((.006*Wpk[21][24][0])-(.0346*
          Wpk[21][24][1])));
        Vpk[22][22][2] = .015;
        Vpk[22][23][2] = ((.015*c23)+(.025*s23));
        VWri[22][24][2] = (Vpk[22][23][2]+((.0346*s23)-(.017*c23)));
        Vpk[22][24][0] = ((.00108*((s23*c24)+(s24*c23)))+(.0175*Wpk[22][24][1]))
          ;
        Vpk[22][24][1] = ((.00108*((c23*c24)-(s23*s24)))-(.0175*Wpk[22][24][0]))
          ;
        Vpk[22][24][2] = (VWri[22][24][2]+((.006*Wpk[22][24][0])-(.0346*
          Wpk[22][24][1])));
        Vpk[23][23][0] = -.015;
        Vpk[23][23][1] = .025;
        Vpk[23][24][0] = (((.002*c24)+(.0596*s24))-.006);
        Vpk[23][24][1] = (.0346+((.0596*c24)-(.002*s24)));
        Vpk[24][24][0] = -.006;
        Vpk[24][24][1] = .0346;
        vpkflg = 1;
    }
/*
 Used 0.05 seconds CPU time,
 0 additional bytes of memory.
 Equations contain  444 adds/subtracts/negates
                    594 multiplies
                      0 divides
                    570 assignments
*/
}

void sddoltau(void)
{

/*
Compute effect of loop hinge torques
*/
/*
 Used 0.00 seconds CPU time,
 0 additional bytes of memory.
 Equations contain    0 adds/subtracts/negates
                      0 multiplies
                      0 divides
                      0 assignments
*/
}

void sddoiner(void)
{

/*
Compute inertial accelerations and related temps
*/
    if (inerflg == 0) {
/*
Compute Otk (inertial angular acceleration)
*/
        Otk[11][1] = (u[11]*wk[11][2]);
        Otk[11][2] = -(u[11]*wk[11][1]);
        Otk[12][0] = ((Otk[11][1]*s12)+(u[12]*wk[12][1]));
        Otk[12][1] = ((Otk[11][1]*c12)-(u[12]*wk[12][0]));
        Otk[13][0] = ((u[13]*wk[13][1])+((Otk[12][0]*c13)+(Otk[12][1]*s13)));
        Otk[13][1] = (((Otk[12][1]*c13)-(Otk[12][0]*s13))-(u[13]*wk[13][0]));
        Otk[22][1] = (u[22]*wk[22][2]);
        Otk[22][2] = -(u[22]*wk[22][1]);
        Otk[23][0] = ((Otk[22][1]*s23)+(u[23]*wk[23][1]));
        Otk[23][1] = ((Otk[22][1]*c23)-(u[23]*wk[23][0]));
        Otk[24][0] = ((u[24]*wk[24][1])+((Otk[23][0]*c24)+(Otk[23][1]*s24)));
        Otk[24][1] = (((Otk[23][1]*c24)-(Otk[23][0]*s24))-(u[24]*wk[24][0]));
/*
Compute Atk (inertial linear acceleration)
*/
        Atk[2][0] = (.0707*(u[2]*u[2]));
        Atk[2][1] = -(.2639*(u[2]*u[2]));
        AiOiWi[3][1] = (Atk[2][1]+(.33*(u[2]*u[2])));
        Atk[3][0] = ((AiOiWi[3][1]*s3)+(Atk[2][0]*c3));
        Atk[3][1] = ((.17*(wk[3][2]*wk[3][2]))+((AiOiWi[3][1]*c3)-(Atk[2][0]*s3)
          ));
        AiOiWi[4][1] = (Atk[3][1]-(.17*(wk[3][2]*wk[3][2])));
        Atk[4][0] = (Atk[3][0]-(wk[3][2]*Wkrpk[4][1]));
        Atk[4][1] = (AiOiWi[4][1]+(2.*(u[4]*wk[3][2])));
        Atk[5][0] = (Atk[4][0]-(2.*(u[5]*wk[3][2])));
        Atk[5][1] = (Atk[4][1]+(wk[3][2]*Wkrpk[5][0]));
        Atk[6][0] = ((Atk[5][0]*c6)+(Atk[5][1]*s6));
        Atk[6][1] = ((.1867*(wk[6][2]*wk[6][2]))+((Atk[5][1]*c6)-(Atk[5][0]*s6))
          );
        AiOiWi[7][1] = (Atk[6][1]-(.1867*(wk[6][2]*wk[6][2])));
        Atk[7][0] = (Atk[6][0]-(wk[6][2]*Wkrpk[7][1]));
        Atk[7][1] = (AiOiWi[7][1]+(2.*(u[7]*wk[6][2])));
        Atk[8][0] = (Atk[7][0]-(2.*(u[8]*wk[6][2])));
        Atk[8][1] = (Atk[7][1]+(wk[6][2]*Wkrpk[8][0]));
        Atk[9][0] = ((Atk[8][0]*c9)+(Atk[8][1]*s9));
        Atk[9][1] = ((Atk[8][1]*c9)-(Atk[8][0]*s9));
        AiOiWi[10][1] = (Atk[6][1]+(.2433*(wk[6][2]*wk[6][2])));
        Atk[10][0] = ((AiOiWi[10][1]*s10)+(Atk[6][0]*c10));
        Atk[10][1] = ((AiOiWi[10][1]*c10)-(Atk[6][0]*s10));
        AiOiWi[11][0] = (Atk[10][0]+(.04877*(wk[10][2]*wk[10][2])));
        AiOiWi[11][1] = (Atk[10][1]+(.04195*(wk[10][2]*wk[10][2])));
        Atk[11][0] = (AiOiWi[11][0]+(((wk[11][1]*Wkrpk[11][2])-(.025*(wk[11][2]*
          wk[11][2])))-(.015*Otk[11][2])));
        Atk[11][1] = ((AiOiWi[11][1]*c11)+((.025*Otk[11][2])-((.015*(wk[11][2]*
          wk[11][2]))+(u[11]*Wkrpk[11][2]))));
        Atk[11][2] = ((((.015*(wk[11][1]*wk[11][2]))+(.025*(u[11]*wk[11][2])))-(
          .025*Otk[11][1]))-(AiOiWi[11][1]*s11));
        AiOiWi[12][0] = (Atk[11][0]+((.015*Otk[11][2])+((Wirk[12][2]*wk[11][1])-
          (.0942*(wk[11][2]*wk[11][2])))));
        AiOiWi[12][1] = (Atk[11][1]+((.0942*Otk[11][2])+((.015*(wk[11][2]*
          wk[11][2]))-(u[11]*Wirk[12][2]))));
        AiOiWi[12][2] = (Atk[11][2]+(((.0942*(u[11]*wk[11][2]))-(.015*(wk[11][1]
          *wk[11][2])))-(.0942*Otk[11][1])));
        Atk[12][0] = (((AiOiWi[12][0]*c12)+(AiOiWi[12][1]*s12))+(((wk[12][1]*
          Wkrpk[12][2])-(.025*(wk[12][2]*wk[12][2])))-(.015*Otk[11][2])));
        Atk[12][1] = (((.025*Otk[11][2])-((.015*(wk[12][2]*wk[12][2]))+(
          wk[12][0]*Wkrpk[12][2])))+((AiOiWi[12][1]*c12)-(AiOiWi[12][0]*s12)));
        Atk[12][2] = (AiOiWi[12][2]+(((.015*Otk[12][0])-(.025*Otk[12][1]))+((
          .015*(wk[12][1]*wk[12][2]))+(.025*(wk[12][0]*wk[12][2])))));
        AiOiWi[13][0] = (Atk[12][0]+(((.00108*Otk[12][1])+(.017*Otk[11][2]))+((
          Wirk[13][2]*wk[12][1])-(Wirk[13][1]*wk[12][2]))));
        AiOiWi[13][1] = (Atk[12][1]+(((.0346*Otk[11][2])-(.00108*Otk[12][0]))+((
          Wirk[13][0]*wk[12][2])-(Wirk[13][2]*wk[12][0]))));
        AiOiWi[13][2] = (Atk[12][2]+(((Wirk[13][1]*wk[12][0])-(Wirk[13][0]*
          wk[12][1]))-((.017*Otk[12][0])+(.0346*Otk[12][1]))));
        Atk[13][0] = (((AiOiWi[13][0]*c13)+(AiOiWi[13][1]*s13))+(((wk[13][1]*
          Wkrpk[13][2])-(wk[13][2]*Wkrpk[13][1]))-((.006*Otk[11][2])+(.0175*
          Otk[13][1]))));
        Atk[13][1] = (((AiOiWi[13][1]*c13)-(AiOiWi[13][0]*s13))+(((.0175*
          Otk[13][0])+(.0346*Otk[11][2]))+((wk[13][2]*Wkrpk[13][0])-(wk[13][0]*
          Wkrpk[13][2]))));
        Atk[13][2] = (AiOiWi[13][2]+(((.006*Otk[13][0])-(.0346*Otk[13][1]))+((
          wk[13][0]*Wkrpk[13][1])-(wk[13][1]*Wkrpk[13][0]))));
        AiOiWi[14][1] = (Atk[2][1]+(.33*(u[2]*u[2])));
        Atk[14][0] = ((AiOiWi[14][1]*s14)+(Atk[2][0]*c14));
        Atk[14][1] = ((.17*(wk[14][2]*wk[14][2]))+((AiOiWi[14][1]*c14)-(
          Atk[2][0]*s14)));
        AiOiWi[15][1] = (Atk[14][1]-(.17*(wk[14][2]*wk[14][2])));
        Atk[15][0] = (Atk[14][0]-(wk[14][2]*Wkrpk[15][1]));
        Atk[15][1] = (AiOiWi[15][1]+(2.*(u[15]*wk[14][2])));
        Atk[16][0] = (Atk[15][0]-(2.*(u[16]*wk[14][2])));
        Atk[16][1] = (Atk[15][1]+(wk[14][2]*Wkrpk[16][0]));
        Atk[17][0] = ((Atk[16][0]*c17)+(Atk[16][1]*s17));
        Atk[17][1] = ((.1867*(wk[17][2]*wk[17][2]))+((Atk[16][1]*c17)-(
          Atk[16][0]*s17)));
        AiOiWi[18][1] = (Atk[17][1]-(.1867*(wk[17][2]*wk[17][2])));
        Atk[18][0] = (Atk[17][0]-(wk[17][2]*Wkrpk[18][1]));
        Atk[18][1] = (AiOiWi[18][1]+(2.*(u[18]*wk[17][2])));
        Atk[19][0] = (Atk[18][0]-(2.*(u[19]*wk[17][2])));
        Atk[19][1] = (Atk[18][1]+(wk[17][2]*Wkrpk[19][0]));
        Atk[20][0] = ((Atk[19][0]*c20)+(Atk[19][1]*s20));
        Atk[20][1] = ((Atk[19][1]*c20)-(Atk[19][0]*s20));
        AiOiWi[21][1] = (Atk[17][1]+(.2433*(wk[17][2]*wk[17][2])));
        Atk[21][0] = ((AiOiWi[21][1]*s21)+(Atk[17][0]*c21));
        Atk[21][1] = ((AiOiWi[21][1]*c21)-(Atk[17][0]*s21));
        AiOiWi[22][0] = (Atk[21][0]+(.04877*(wk[21][2]*wk[21][2])));
        AiOiWi[22][1] = (Atk[21][1]+(.04195*(wk[21][2]*wk[21][2])));
        Atk[22][0] = (AiOiWi[22][0]+(((wk[22][1]*Wkrpk[22][2])-(.025*(wk[22][2]*
          wk[22][2])))-(.015*Otk[22][2])));
        Atk[22][1] = ((AiOiWi[22][1]*c22)+((.025*Otk[22][2])-((.015*(wk[22][2]*
          wk[22][2]))+(u[22]*Wkrpk[22][2]))));
        Atk[22][2] = ((((.015*(wk[22][1]*wk[22][2]))+(.025*(u[22]*wk[22][2])))-(
          .025*Otk[22][1]))-(AiOiWi[22][1]*s22));
        AiOiWi[23][0] = (Atk[22][0]+((.015*Otk[22][2])+((Wirk[23][2]*wk[22][1])-
          (.0942*(wk[22][2]*wk[22][2])))));
        AiOiWi[23][1] = (Atk[22][1]+((.0942*Otk[22][2])+((.015*(wk[22][2]*
          wk[22][2]))-(u[22]*Wirk[23][2]))));
        AiOiWi[23][2] = (Atk[22][2]+(((.0942*(u[22]*wk[22][2]))-(.015*(wk[22][1]
          *wk[22][2])))-(.0942*Otk[22][1])));
        Atk[23][0] = (((AiOiWi[23][0]*c23)+(AiOiWi[23][1]*s23))+(((wk[23][1]*
          Wkrpk[23][2])-(.025*(wk[23][2]*wk[23][2])))-(.015*Otk[22][2])));
        Atk[23][1] = (((.025*Otk[22][2])-((.015*(wk[23][2]*wk[23][2]))+(
          wk[23][0]*Wkrpk[23][2])))+((AiOiWi[23][1]*c23)-(AiOiWi[23][0]*s23)));
        Atk[23][2] = (AiOiWi[23][2]+(((.015*Otk[23][0])-(.025*Otk[23][1]))+((
          .015*(wk[23][1]*wk[23][2]))+(.025*(wk[23][0]*wk[23][2])))));
        AiOiWi[24][0] = (Atk[23][0]+(((.017*Otk[22][2])-(.00108*Otk[23][1]))+((
          Wirk[24][2]*wk[23][1])-(Wirk[24][1]*wk[23][2]))));
        AiOiWi[24][1] = (Atk[23][1]+(((.00108*Otk[23][0])+(.0346*Otk[22][2]))+((
          Wirk[24][0]*wk[23][2])-(Wirk[24][2]*wk[23][0]))));
        AiOiWi[24][2] = (Atk[23][2]+(((Wirk[24][1]*wk[23][0])-(Wirk[24][0]*
          wk[23][1]))-((.017*Otk[23][0])+(.0346*Otk[23][1]))));
        Atk[24][0] = (((AiOiWi[24][0]*c24)+(AiOiWi[24][1]*s24))+(((.0175*
          Otk[24][1])-(.006*Otk[22][2]))+((wk[24][1]*Wkrpk[24][2])-(wk[24][2]*
          Wkrpk[24][1]))));
        Atk[24][1] = (((AiOiWi[24][1]*c24)-(AiOiWi[24][0]*s24))+(((.0346*
          Otk[22][2])-(.0175*Otk[24][0]))+((wk[24][2]*Wkrpk[24][0])-(wk[24][0]*
          Wkrpk[24][2]))));
        Atk[24][2] = (AiOiWi[24][2]+(((.006*Otk[24][0])-(.0346*Otk[24][1]))+((
          wk[24][0]*Wkrpk[24][1])-(wk[24][1]*Wkrpk[24][0]))));
        inerflg = 1;
    }
/*
 Used 0.02 seconds CPU time,
 0 additional bytes of memory.
 Equations contain  175 adds/subtracts/negates
                    264 multiplies
                      0 divides
                     88 assignments
*/
}

void sddofs0(void)
{

/*
Compute effect of all applied loads
*/
    if (fs0flg == 0) {
        sddoltau();
        sddoiner();
/*
Compute Fstar (forces)
*/
        Fstar[2][0] = ((51.1*(Atk[2][0]+(9.80665*s2)))-ufk[0][0]);
        Fstar[2][1] = ((51.1*(Atk[2][1]+(9.80665*c2)))-ufk[0][1]);
        Fstar[3][0] = ((7.76*(Atk[3][0]-gk[3][0]))-ufk[1][0]);
        Fstar[3][1] = ((7.76*(Atk[3][1]-gk[3][1]))-ufk[1][1]);
        Fstar[6][0] = ((3.03*(Atk[6][0]-gk[6][0]))-ufk[2][0]);
        Fstar[6][1] = ((3.03*(Atk[6][1]-gk[6][1]))-ufk[2][1]);
        Fstar[9][0] = ((.1*(Atk[9][0]-gk[9][0]))-ufk[3][0]);
        Fstar[9][1] = ((.1*(Atk[9][1]-gk[9][1]))-ufk[3][1]);
        Fstar[10][0] = ((.1*(Atk[10][0]-gk[10][0]))-ufk[4][0]);
        Fstar[10][1] = ((.1*(Atk[10][1]-gk[10][1]))-ufk[4][1]);
        Fstar[11][0] = ((.4183*(Atk[11][0]-gk[10][0]))-ufk[5][0]);
        Fstar[11][1] = ((.4183*(Atk[11][1]-gk[11][1]))-ufk[5][1]);
        Fstar[11][2] = ((.4183*(Atk[11][2]-gk[11][2]))-ufk[5][2]);
        Fstar[12][0] = ((.4*(Atk[12][0]-gk[12][0]))-ufk[6][0]);
        Fstar[12][1] = ((.4*(Atk[12][1]-gk[12][1]))-ufk[6][1]);
        Fstar[12][2] = ((.4*(Atk[12][2]-gk[11][2]))-ufk[6][2]);
        Fstar[13][0] = ((.2058*(Atk[13][0]-gk[13][0]))-ufk[7][0]);
        Fstar[13][1] = ((.2058*(Atk[13][1]-gk[13][1]))-ufk[7][1]);
        Fstar[13][2] = ((.2058*(Atk[13][2]-gk[11][2]))-ufk[7][2]);
        Fstar[14][0] = ((7.76*(Atk[14][0]-gk[14][0]))-ufk[8][0]);
        Fstar[14][1] = ((7.76*(Atk[14][1]-gk[14][1]))-ufk[8][1]);
        Fstar[17][0] = ((3.03*(Atk[17][0]-gk[17][0]))-ufk[9][0]);
        Fstar[17][1] = ((3.03*(Atk[17][1]-gk[17][1]))-ufk[9][1]);
        Fstar[20][0] = ((.1*(Atk[20][0]-gk[20][0]))-ufk[10][0]);
        Fstar[20][1] = ((.1*(Atk[20][1]-gk[20][1]))-ufk[10][1]);
        Fstar[21][0] = ((.1*(Atk[21][0]-gk[21][0]))-ufk[11][0]);
        Fstar[21][1] = ((.1*(Atk[21][1]-gk[21][1]))-ufk[11][1]);
        Fstar[22][0] = ((.4183*(Atk[22][0]-gk[21][0]))-ufk[12][0]);
        Fstar[22][1] = ((.4183*(Atk[22][1]-gk[22][1]))-ufk[12][1]);
        Fstar[22][2] = ((.4183*(Atk[22][2]-gk[22][2]))-ufk[12][2]);
        Fstar[23][0] = ((.4*(Atk[23][0]-gk[23][0]))-ufk[13][0]);
        Fstar[23][1] = ((.4*(Atk[23][1]-gk[23][1]))-ufk[13][1]);
        Fstar[23][2] = ((.4*(Atk[23][2]-gk[22][2]))-ufk[13][2]);
        Fstar[24][0] = ((.2058*(Atk[24][0]-gk[24][0]))-ufk[14][0]);
        Fstar[24][1] = ((.2058*(Atk[24][1]-gk[24][1]))-ufk[14][1]);
        Fstar[24][2] = ((.2058*(Atk[24][2]-gk[22][2]))-ufk[14][2]);
/*
Compute Tstar (torques)
*/
        Tstar[11][1] = ((.001*Otk[11][1])-utk[5][1]);
        Tstar[11][2] = ((.001*Otk[11][2])-utk[5][2]);
        Tstar[12][0] = ((.001*Otk[12][0])-utk[6][0]);
        Tstar[12][1] = ((.001*Otk[12][1])-utk[6][1]);
        Tstar[12][2] = ((.001*Otk[11][2])-utk[6][2]);
        Tstar[13][0] = ((.001*Otk[13][0])-utk[7][0]);
        Tstar[13][1] = ((.001*Otk[13][1])-utk[7][1]);
        Tstar[13][2] = ((.001*Otk[11][2])-utk[7][2]);
        Tstar[22][1] = ((.001*Otk[22][1])-utk[12][1]);
        Tstar[22][2] = ((.001*Otk[22][2])-utk[12][2]);
        Tstar[23][0] = ((.001*Otk[23][0])-utk[13][0]);
        Tstar[23][1] = ((.001*Otk[23][1])-utk[13][1]);
        Tstar[23][2] = ((.001*Otk[22][2])-utk[13][2]);
        Tstar[24][0] = ((.001*Otk[24][0])-utk[14][0]);
        Tstar[24][1] = ((.001*Otk[24][1])-utk[14][1]);
        Tstar[24][2] = ((.001*Otk[22][2])-utk[14][2]);
/*
Compute fs0 (RHS ignoring constraints)
*/
        sddovpk();
        temp[0] = (((Fstar[11][2]*Vpk[0][11][2])+((Fstar[11][0]*Vpk[0][10][0])+(
          Fstar[11][1]*Vpk[0][11][1])))+(((Fstar[10][0]*Vpk[0][10][0])+(
          Fstar[10][1]*Vpk[0][10][1]))+(((Fstar[9][0]*Vpk[0][9][0])+(Fstar[9][1]
          *Vpk[0][9][1]))+(((Fstar[6][0]*Vpk[0][6][0])+(Fstar[6][1]*Vpk[0][6][1]
          ))+(((Fstar[2][0]*c2)-(Fstar[2][1]*s2))+((Fstar[3][0]*Vpk[0][3][0])+(
          Fstar[3][1]*Vpk[0][3][1])))))));
        temp[1] = (((Fstar[21][0]*Vpk[0][21][0])+(Fstar[21][1]*Vpk[0][21][1]))+(
          ((Fstar[20][0]*Vpk[0][20][0])+(Fstar[20][1]*Vpk[0][20][1]))+(((
          Fstar[17][0]*Vpk[0][17][0])+(Fstar[17][1]*Vpk[0][17][1]))+(((
          Fstar[14][0]*Vpk[0][14][0])+(Fstar[14][1]*Vpk[0][14][1]))+(((
          Fstar[13][2]*Vpk[0][11][2])+((Fstar[13][0]*Vpk[0][13][0])+(
          Fstar[13][1]*Vpk[0][13][1])))+(((Fstar[12][2]*Vpk[0][11][2])+((
          Fstar[12][0]*Vpk[0][12][0])+(Fstar[12][1]*Vpk[0][12][1])))+temp[0]))))
          ));
        fs0[0] = (utau[0]-(((Fstar[24][2]*Vpk[0][22][2])+((Fstar[24][0]*
          Vpk[0][24][0])+(Fstar[24][1]*Vpk[0][24][1])))+(((Fstar[23][2]*
          Vpk[0][22][2])+((Fstar[23][0]*Vpk[0][23][0])+(Fstar[23][1]*
          Vpk[0][23][1])))+(((Fstar[22][2]*Vpk[0][22][2])+((Fstar[22][0]*
          Vpk[0][21][0])+(Fstar[22][1]*Vpk[0][22][1])))+temp[1]))));
        temp[0] = (((Fstar[11][2]*Vpk[1][11][2])+((Fstar[11][0]*Vpk[1][10][0])+(
          Fstar[11][1]*Vpk[1][11][1])))+(((Fstar[10][0]*Vpk[1][10][0])+(
          Fstar[10][1]*Vpk[1][10][1]))+(((Fstar[9][0]*Vpk[1][9][0])+(Fstar[9][1]
          *Vpk[1][9][1]))+(((Fstar[6][0]*Vpk[1][6][0])+(Fstar[6][1]*Vpk[1][6][1]
          ))+(((Fstar[2][0]*s2)+(Fstar[2][1]*c2))+((Fstar[3][0]*Vpk[1][3][0])+(
          Fstar[3][1]*Vpk[1][3][1])))))));
        temp[1] = (((Fstar[21][0]*Vpk[1][21][0])+(Fstar[21][1]*Vpk[1][21][1]))+(
          ((Fstar[20][0]*Vpk[1][20][0])+(Fstar[20][1]*Vpk[1][20][1]))+(((
          Fstar[17][0]*Vpk[1][17][0])+(Fstar[17][1]*Vpk[1][17][1]))+(((
          Fstar[14][0]*Vpk[1][14][0])+(Fstar[14][1]*Vpk[1][14][1]))+(((
          Fstar[13][2]*Vpk[1][11][2])+((Fstar[13][0]*Vpk[1][13][0])+(
          Fstar[13][1]*Vpk[1][13][1])))+(((Fstar[12][2]*Vpk[1][11][2])+((
          Fstar[12][0]*Vpk[1][12][0])+(Fstar[12][1]*Vpk[1][12][1])))+temp[0]))))
          ));
        fs0[1] = (utau[1]-(((Fstar[24][2]*Vpk[1][22][2])+((Fstar[24][0]*
          Vpk[1][24][0])+(Fstar[24][1]*Vpk[1][24][1])))+(((Fstar[23][2]*
          Vpk[1][22][2])+((Fstar[23][0]*Vpk[1][23][0])+(Fstar[23][1]*
          Vpk[1][23][1])))+(((Fstar[22][2]*Vpk[1][22][2])+((Fstar[22][0]*
          Vpk[1][21][0])+(Fstar[22][1]*Vpk[1][22][1])))+temp[1]))));
        temp[0] = ((((Fstar[10][0]*Vpk[2][10][0])+(Fstar[10][1]*Vpk[2][10][1]))-
          utk[4][2])+((((Fstar[9][0]*Vpk[2][9][0])+(Fstar[9][1]*Vpk[2][9][1]))-
          utk[3][2])+((((Fstar[6][0]*Vpk[2][6][0])+(Fstar[6][1]*Vpk[2][6][1]))-
          utk[2][2])+((((Fstar[3][0]*Vpk[2][3][0])+(Fstar[3][1]*Vpk[2][3][1]))-
          utk[1][2])-(utk[0][2]+((.0707*Fstar[2][1])+(.2639*Fstar[2][0])))))));
        temp[1] = ((((Fstar[12][2]*Vpk[2][12][2])+((Fstar[12][0]*Vpk[2][12][0])+
          (Fstar[12][1]*Vpk[2][12][1])))+((Tstar[12][2]*c11)+((Tstar[12][0]*
          Wpk[2][12][0])+(Tstar[12][1]*Wpk[2][12][1]))))+((((Fstar[11][2]*
          Vpk[2][11][2])+((Fstar[11][0]*Vpk[2][11][0])+(Fstar[11][1]*
          Vpk[2][11][1])))+((Tstar[11][1]*s11)+(Tstar[11][2]*c11)))+temp[0]));
        temp[2] = ((((Fstar[20][0]*Vpk[2][20][0])+(Fstar[20][1]*Vpk[2][20][1]))-
          utk[10][2])+((((Fstar[17][0]*Vpk[2][17][0])+(Fstar[17][1]*
          Vpk[2][17][1]))-utk[9][2])+((((Fstar[14][0]*Vpk[2][14][0])+(
          Fstar[14][1]*Vpk[2][14][1]))-utk[8][2])+((((Fstar[13][2]*Vpk[2][13][2]
          )+((Fstar[13][0]*Vpk[2][13][0])+(Fstar[13][1]*Vpk[2][13][1])))+((
          Tstar[13][2]*c11)+((Tstar[13][0]*Wpk[2][13][0])+(Tstar[13][1]*
          Wpk[2][13][1]))))+temp[1]))));
        temp[3] = ((((Fstar[23][2]*Vpk[2][23][2])+((Fstar[23][0]*Vpk[2][23][0])+
          (Fstar[23][1]*Vpk[2][23][1])))+((Tstar[23][2]*c22)+((Tstar[23][0]*
          Wpk[2][23][0])+(Tstar[23][1]*Wpk[2][23][1]))))+((((Fstar[22][2]*
          Vpk[2][22][2])+((Fstar[22][0]*Vpk[2][22][0])+(Fstar[22][1]*
          Vpk[2][22][1])))+((Tstar[22][1]*s22)+(Tstar[22][2]*c22)))+((((
          Fstar[21][0]*Vpk[2][21][0])+(Fstar[21][1]*Vpk[2][21][1]))-utk[11][2])+
          temp[2])));
        fs0[2] = (utau[2]-((((Fstar[24][2]*Vpk[2][24][2])+((Fstar[24][0]*
          Vpk[2][24][0])+(Fstar[24][1]*Vpk[2][24][1])))+((Tstar[24][2]*c22)+((
          Tstar[24][0]*Wpk[2][24][0])+(Tstar[24][1]*Wpk[2][24][1]))))+temp[3]));
        temp[0] = ((((Fstar[11][2]*Vpk[3][11][2])+((Fstar[11][0]*Vpk[3][11][0])+
          (Fstar[11][1]*Vpk[3][11][1])))+((Tstar[11][1]*s11)+(Tstar[11][2]*c11))
          )+((((Fstar[10][0]*Vpk[3][10][0])+(Fstar[10][1]*Vpk[3][10][1]))-
          utk[4][2])+((((.17*Fstar[3][0])-utk[1][2])+(((Fstar[6][0]*Vpk[3][6][0]
          )+(Fstar[6][1]*Vpk[3][6][1]))-utk[2][2]))+(((Fstar[9][0]*Vpk[3][9][0])
          +(Fstar[9][1]*Vpk[3][9][1]))-utk[3][2]))));
        fs0[3] = (utau[3]-((((Fstar[13][2]*Vpk[3][13][2])+((Fstar[13][0]*
          Vpk[3][13][0])+(Fstar[13][1]*Vpk[3][13][1])))+((Tstar[13][2]*c11)+((
          Tstar[13][0]*Wpk[3][13][0])+(Tstar[13][1]*Wpk[3][13][1]))))+((((
          Fstar[12][2]*Vpk[3][12][2])+((Fstar[12][0]*Vpk[3][12][0])+(
          Fstar[12][1]*Vpk[3][12][1])))+((Tstar[12][2]*c11)+((Tstar[12][0]*
          Wpk[3][12][0])+(Tstar[12][1]*Wpk[3][12][1]))))+temp[0])));
        temp[0] = (((Fstar[13][2]*Vpk[4][11][2])+((Fstar[13][0]*Vpk[4][13][0])+(
          Fstar[13][1]*Vpk[4][13][1])))+(((Fstar[12][2]*Vpk[4][11][2])+((
          Fstar[12][0]*Vpk[4][12][0])+(Fstar[12][1]*Vpk[4][12][1])))+(((
          Fstar[11][2]*Vpk[4][11][2])+((Fstar[11][0]*Vpk[4][10][0])+(
          Fstar[11][1]*Vpk[4][11][1])))+(((Fstar[10][0]*Vpk[4][10][0])+(
          Fstar[10][1]*Vpk[4][10][1]))+(((Fstar[6][0]*c6)-(Fstar[6][1]*s6))+((
          Fstar[9][0]*Vpk[4][9][0])+(Fstar[9][1]*Vpk[4][9][1])))))));
        fs0[4] = (utau[4]-temp[0]);
        temp[0] = (((Fstar[13][2]*Vpk[5][11][2])+((Fstar[13][0]*Vpk[5][13][0])+(
          Fstar[13][1]*Vpk[5][13][1])))+(((Fstar[12][2]*Vpk[5][11][2])+((
          Fstar[12][0]*Vpk[5][12][0])+(Fstar[12][1]*Vpk[5][12][1])))+(((
          Fstar[11][2]*Vpk[5][11][2])+((Fstar[11][0]*Vpk[5][10][0])+(
          Fstar[11][1]*Vpk[5][11][1])))+(((Fstar[10][0]*Vpk[5][10][0])+(
          Fstar[10][1]*Vpk[5][10][1]))+(((Fstar[6][0]*s6)+(Fstar[6][1]*c6))+((
          Fstar[9][0]*Vpk[5][9][0])+(Fstar[9][1]*Vpk[5][9][1])))))));
        fs0[5] = (utau[5]-temp[0]);
        temp[0] = ((((.43*((Fstar[10][0]*c10)-(Fstar[10][1]*s10)))-utk[4][2])+((
          (.1867*Fstar[6][0])-utk[2][2])+(((Fstar[9][0]*Vpk[6][9][0])+(
          Fstar[9][1]*Vpk[6][9][1]))-utk[3][2])))+(((Fstar[11][2]*Vpk[6][11][2])
          +((Fstar[11][0]*Vpk[6][11][0])+(Fstar[11][1]*Vpk[6][11][1])))+((
          Tstar[11][1]*s11)+(Tstar[11][2]*c11))));
        fs0[6] = (utau[6]-((((Fstar[13][2]*Vpk[6][13][2])+((Fstar[13][0]*
          Vpk[6][13][0])+(Fstar[13][1]*Vpk[6][13][1])))+((Tstar[13][2]*c11)+((
          Tstar[13][0]*Wpk[6][13][0])+(Tstar[13][1]*Wpk[6][13][1]))))+((((
          Fstar[12][2]*Vpk[6][12][2])+((Fstar[12][0]*Vpk[6][12][0])+(
          Fstar[12][1]*Vpk[6][12][1])))+((Tstar[12][2]*c11)+((Tstar[12][0]*
          Wpk[6][12][0])+(Tstar[12][1]*Wpk[6][12][1]))))+temp[0])));
        fs0[7] = (utau[7]-((Fstar[9][0]*c9)-(Fstar[9][1]*s9)));
        fs0[8] = (utau[8]-((Fstar[9][0]*s9)+(Fstar[9][1]*c9)));
        fs0[9] = (utau[9]+utk[3][2]);
        temp[0] = ((((Fstar[12][2]*Vpk[10][12][2])+((Fstar[12][0]*Vpk[10][12][0]
          )+(Fstar[12][1]*Vpk[10][12][1])))+((Tstar[12][2]*c11)+((Tstar[12][0]*
          Wpk[10][12][0])+(Tstar[12][1]*Wpk[10][12][1]))))+((((.02377*(
          Fstar[11][2]*s11))+((Fstar[11][0]*Vpk[10][11][0])-(.02377*(
          Fstar[11][1]*c11))))+((Tstar[11][1]*s11)+(Tstar[11][2]*c11)))-
          utk[4][2]));
        fs0[10] = (utau[10]-((((Fstar[13][2]*Vpk[10][13][2])+((Fstar[13][0]*
          Vpk[10][13][0])+(Fstar[13][1]*Vpk[10][13][1])))+((Tstar[13][2]*c11)+((
          Tstar[13][0]*Wpk[10][13][0])+(Tstar[13][1]*Wpk[10][13][1]))))+temp[0])
          );
        fs0[11] = (utau[11]-((((.015*Fstar[11][2])-utk[5][0])+((Fstar[12][2]*
          Vpk[11][12][2])+((Tstar[12][0]*c12)-(Tstar[12][1]*s12))))+(((
          Fstar[13][2]*Vpk[11][13][2])+((Fstar[13][0]*Vpk[11][13][0])+(
          Fstar[13][1]*Vpk[11][13][1])))+((Tstar[13][0]*Wpk[11][13][0])+(
          Tstar[13][1]*Wpk[11][13][1])))));
        fs0[12] = (utau[12]-((Tstar[12][2]+((.025*Fstar[12][1])-(.015*
          Fstar[12][0])))+(Tstar[13][2]+((Fstar[13][0]*Vpk[12][13][0])+(
          Fstar[13][1]*Vpk[12][13][1])))));
        fs0[13] = (utau[13]-(Tstar[13][2]+((.0346*Fstar[13][1])-(.006*
          Fstar[13][0]))));
        temp[0] = ((((Fstar[22][2]*Vpk[14][22][2])+((Fstar[22][0]*Vpk[14][22][0]
          )+(Fstar[22][1]*Vpk[14][22][1])))+((Tstar[22][1]*s22)+(Tstar[22][2]*
          c22)))+((((Fstar[21][0]*Vpk[14][21][0])+(Fstar[21][1]*Vpk[14][21][1]))
          -utk[11][2])+((((.17*Fstar[14][0])-utk[8][2])+(((Fstar[17][0]*
          Vpk[14][17][0])+(Fstar[17][1]*Vpk[14][17][1]))-utk[9][2]))+(((
          Fstar[20][0]*Vpk[14][20][0])+(Fstar[20][1]*Vpk[14][20][1]))-utk[10][2]
          ))));
        fs0[14] = (utau[14]-((((Fstar[24][2]*Vpk[14][24][2])+((Fstar[24][0]*
          Vpk[14][24][0])+(Fstar[24][1]*Vpk[14][24][1])))+((Tstar[24][2]*c22)+((
          Tstar[24][0]*Wpk[14][24][0])+(Tstar[24][1]*Wpk[14][24][1]))))+((((
          Fstar[23][2]*Vpk[14][23][2])+((Fstar[23][0]*Vpk[14][23][0])+(
          Fstar[23][1]*Vpk[14][23][1])))+((Tstar[23][2]*c22)+((Tstar[23][0]*
          Wpk[14][23][0])+(Tstar[23][1]*Wpk[14][23][1]))))+temp[0])));
        temp[0] = (((Fstar[24][2]*Vpk[15][22][2])+((Fstar[24][0]*Vpk[15][24][0])
          +(Fstar[24][1]*Vpk[15][24][1])))+(((Fstar[23][2]*Vpk[15][22][2])+((
          Fstar[23][0]*Vpk[15][23][0])+(Fstar[23][1]*Vpk[15][23][1])))+(((
          Fstar[22][2]*Vpk[15][22][2])+((Fstar[22][0]*Vpk[15][21][0])+(
          Fstar[22][1]*Vpk[15][22][1])))+(((Fstar[21][0]*Vpk[15][21][0])+(
          Fstar[21][1]*Vpk[15][21][1]))+(((Fstar[17][0]*c17)-(Fstar[17][1]*s17))
          +((Fstar[20][0]*Vpk[15][20][0])+(Fstar[20][1]*Vpk[15][20][1])))))));
        fs0[15] = (utau[15]-temp[0]);
        temp[0] = (((Fstar[24][2]*Vpk[16][22][2])+((Fstar[24][0]*Vpk[16][24][0])
          +(Fstar[24][1]*Vpk[16][24][1])))+(((Fstar[23][2]*Vpk[16][22][2])+((
          Fstar[23][0]*Vpk[16][23][0])+(Fstar[23][1]*Vpk[16][23][1])))+(((
          Fstar[22][2]*Vpk[16][22][2])+((Fstar[22][0]*Vpk[16][21][0])+(
          Fstar[22][1]*Vpk[16][22][1])))+(((Fstar[21][0]*Vpk[16][21][0])+(
          Fstar[21][1]*Vpk[16][21][1]))+(((Fstar[17][0]*s17)+(Fstar[17][1]*c17))
          +((Fstar[20][0]*Vpk[16][20][0])+(Fstar[20][1]*Vpk[16][20][1])))))));
        fs0[16] = (utau[16]-temp[0]);
        temp[0] = ((((.43*((Fstar[21][0]*c21)-(Fstar[21][1]*s21)))-utk[11][2])+(
          ((.1867*Fstar[17][0])-utk[9][2])+(((Fstar[20][0]*Vpk[17][20][0])+(
          Fstar[20][1]*Vpk[17][20][1]))-utk[10][2])))+(((Fstar[22][2]*
          Vpk[17][22][2])+((Fstar[22][0]*Vpk[17][22][0])+(Fstar[22][1]*
          Vpk[17][22][1])))+((Tstar[22][1]*s22)+(Tstar[22][2]*c22))));
        fs0[17] = (utau[17]-((((Fstar[24][2]*Vpk[17][24][2])+((Fstar[24][0]*
          Vpk[17][24][0])+(Fstar[24][1]*Vpk[17][24][1])))+((Tstar[24][2]*c22)+((
          Tstar[24][0]*Wpk[17][24][0])+(Tstar[24][1]*Wpk[17][24][1]))))+((((
          Fstar[23][2]*Vpk[17][23][2])+((Fstar[23][0]*Vpk[17][23][0])+(
          Fstar[23][1]*Vpk[17][23][1])))+((Tstar[23][2]*c22)+((Tstar[23][0]*
          Wpk[17][23][0])+(Tstar[23][1]*Wpk[17][23][1]))))+temp[0])));
        fs0[18] = (utau[18]-((Fstar[20][0]*c20)-(Fstar[20][1]*s20)));
        fs0[19] = (utau[19]-((Fstar[20][0]*s20)+(Fstar[20][1]*c20)));
        fs0[20] = (utau[20]+utk[10][2]);
        temp[0] = ((((Fstar[23][2]*Vpk[21][23][2])+((Fstar[23][0]*Vpk[21][23][0]
          )+(Fstar[23][1]*Vpk[21][23][1])))+((Tstar[23][2]*c22)+((Tstar[23][0]*
          Wpk[21][23][0])+(Tstar[23][1]*Wpk[21][23][1]))))+((((.02377*(
          Fstar[22][2]*s22))+((Fstar[22][0]*Vpk[21][22][0])-(.02377*(
          Fstar[22][1]*c22))))+((Tstar[22][1]*s22)+(Tstar[22][2]*c22)))-
          utk[11][2]));
        fs0[21] = (utau[21]-((((Fstar[24][2]*Vpk[21][24][2])+((Fstar[24][0]*
          Vpk[21][24][0])+(Fstar[24][1]*Vpk[21][24][1])))+((Tstar[24][2]*c22)+((
          Tstar[24][0]*Wpk[21][24][0])+(Tstar[24][1]*Wpk[21][24][1]))))+temp[0])
          );
        fs0[22] = (utau[22]-((((.015*Fstar[22][2])-utk[12][0])+((Fstar[23][2]*
          Vpk[22][23][2])+((Tstar[23][0]*c23)-(Tstar[23][1]*s23))))+(((
          Fstar[24][2]*Vpk[22][24][2])+((Fstar[24][0]*Vpk[22][24][0])+(
          Fstar[24][1]*Vpk[22][24][1])))+((Tstar[24][0]*Wpk[22][24][0])+(
          Tstar[24][1]*Wpk[22][24][1])))));
        fs0[23] = (utau[23]-((Tstar[23][2]+((.025*Fstar[23][1])-(.015*
          Fstar[23][0])))+(Tstar[24][2]+((Fstar[24][0]*Vpk[23][24][0])+(
          Fstar[24][1]*Vpk[23][24][1])))));
        fs0[24] = (utau[24]-(Tstar[24][2]+((.0346*Fstar[24][1])-(.006*
          Fstar[24][0]))));
        fs0flg = 1;
    }
/*
 Used 0.02 seconds CPU time,
 0 additional bytes of memory.
 Equations contain  471 adds/subtracts/negates
                    408 multiplies
                      0 divides
                     95 assignments
*/
}

void sddomm(int routine)
{
    int dumroutine,errnum;
    int i;

    if (mmflg == 0) {
/*
Compute mass matrix (MM)
*/
        sddovpk();
        temp[0] = ((.4183*((Vpk[0][11][2]*Vpk[0][11][2])+((Vpk[0][10][0]*
          Vpk[0][10][0])+(Vpk[0][11][1]*Vpk[0][11][1]))))+((.1*((Vpk[0][10][0]*
          Vpk[0][10][0])+(Vpk[0][10][1]*Vpk[0][10][1])))+((.1*((Vpk[0][9][0]*
          Vpk[0][9][0])+(Vpk[0][9][1]*Vpk[0][9][1])))+((3.03*((Vpk[0][6][0]*
          Vpk[0][6][0])+(Vpk[0][6][1]*Vpk[0][6][1])))+(51.1+(7.76*((Vpk[0][3][0]
          *Vpk[0][3][0])+(Vpk[0][3][1]*Vpk[0][3][1]))))))));
        temp[1] = ((3.03*((Vpk[0][17][0]*Vpk[0][17][0])+(Vpk[0][17][1]*
          Vpk[0][17][1])))+((7.76*((Vpk[0][14][0]*Vpk[0][14][0])+(Vpk[0][14][1]*
          Vpk[0][14][1])))+((.2058*((Vpk[0][11][2]*Vpk[0][11][2])+((
          Vpk[0][13][0]*Vpk[0][13][0])+(Vpk[0][13][1]*Vpk[0][13][1]))))+((.4*((
          Vpk[0][11][2]*Vpk[0][11][2])+((Vpk[0][12][0]*Vpk[0][12][0])+(
          Vpk[0][12][1]*Vpk[0][12][1]))))+temp[0]))));
        temp[2] = ((.4*((Vpk[0][22][2]*Vpk[0][22][2])+((Vpk[0][23][0]*
          Vpk[0][23][0])+(Vpk[0][23][1]*Vpk[0][23][1]))))+((.4183*((
          Vpk[0][22][2]*Vpk[0][22][2])+((Vpk[0][21][0]*Vpk[0][21][0])+(
          Vpk[0][22][1]*Vpk[0][22][1]))))+((.1*((Vpk[0][21][0]*Vpk[0][21][0])+(
          Vpk[0][21][1]*Vpk[0][21][1])))+((.1*((Vpk[0][20][0]*Vpk[0][20][0])+(
          Vpk[0][20][1]*Vpk[0][20][1])))+temp[1]))));
        mm[0][0] = ((.2058*((Vpk[0][22][2]*Vpk[0][22][2])+((Vpk[0][24][0]*
          Vpk[0][24][0])+(Vpk[0][24][1]*Vpk[0][24][1]))))+temp[2]);
        temp[0] = ((.4183*((Vpk[0][11][2]*Vpk[1][11][2])+((Vpk[0][10][0]*
          Vpk[1][10][0])+(Vpk[0][11][1]*Vpk[1][11][1]))))+((.1*((Vpk[0][10][0]*
          Vpk[1][10][0])+(Vpk[0][10][1]*Vpk[1][10][1])))+((.1*((Vpk[0][9][0]*
          Vpk[1][9][0])+(Vpk[0][9][1]*Vpk[1][9][1])))+((3.03*((Vpk[0][6][0]*
          Vpk[1][6][0])+(Vpk[0][6][1]*Vpk[1][6][1])))+(7.76*((Vpk[0][3][0]*
          Vpk[1][3][0])+(Vpk[0][3][1]*Vpk[1][3][1])))))));
        temp[1] = ((3.03*((Vpk[0][17][0]*Vpk[1][17][0])+(Vpk[0][17][1]*
          Vpk[1][17][1])))+((7.76*((Vpk[0][14][0]*Vpk[1][14][0])+(Vpk[0][14][1]*
          Vpk[1][14][1])))+((.2058*((Vpk[0][11][2]*Vpk[1][11][2])+((
          Vpk[0][13][0]*Vpk[1][13][0])+(Vpk[0][13][1]*Vpk[1][13][1]))))+((.4*((
          Vpk[0][11][2]*Vpk[1][11][2])+((Vpk[0][12][0]*Vpk[1][12][0])+(
          Vpk[0][12][1]*Vpk[1][12][1]))))+temp[0]))));
        temp[2] = ((.4*((Vpk[0][22][2]*Vpk[1][22][2])+((Vpk[0][23][0]*
          Vpk[1][23][0])+(Vpk[0][23][1]*Vpk[1][23][1]))))+((.4183*((
          Vpk[0][22][2]*Vpk[1][22][2])+((Vpk[0][21][0]*Vpk[1][21][0])+(
          Vpk[0][22][1]*Vpk[1][22][1]))))+((.1*((Vpk[0][21][0]*Vpk[1][21][0])+(
          Vpk[0][21][1]*Vpk[1][21][1])))+((.1*((Vpk[0][20][0]*Vpk[1][20][0])+(
          Vpk[0][20][1]*Vpk[1][20][1])))+temp[1]))));
        mm[0][1] = ((.2058*((Vpk[0][22][2]*Vpk[1][22][2])+((Vpk[0][24][0]*
          Vpk[1][24][0])+(Vpk[0][24][1]*Vpk[1][24][1]))))+temp[2]);
        temp[0] = ((.1*((Vpk[0][10][0]*Vpk[2][10][0])+(Vpk[0][10][1]*
          Vpk[2][10][1])))+((.1*((Vpk[0][9][0]*Vpk[2][9][0])+(Vpk[0][9][1]*
          Vpk[2][9][1])))+((3.03*((Vpk[0][6][0]*Vpk[2][6][0])+(Vpk[0][6][1]*
          Vpk[2][6][1])))+((7.76*((Vpk[0][3][0]*Vpk[2][3][0])+(Vpk[0][3][1]*
          Vpk[2][3][1])))+(51.1*((.0707*s2)-(.2639*c2)))))));
        temp[1] = ((7.76*((Vpk[0][14][0]*Vpk[2][14][0])+(Vpk[0][14][1]*
          Vpk[2][14][1])))+((.2058*((Vpk[0][11][2]*Vpk[2][13][2])+((
          Vpk[0][13][0]*Vpk[2][13][0])+(Vpk[0][13][1]*Vpk[2][13][1]))))+((.4*((
          Vpk[0][11][2]*Vpk[2][12][2])+((Vpk[0][12][0]*Vpk[2][12][0])+(
          Vpk[0][12][1]*Vpk[2][12][1]))))+((.4183*((Vpk[0][11][2]*Vpk[2][11][2])
          +((Vpk[0][10][0]*Vpk[2][11][0])+(Vpk[0][11][1]*Vpk[2][11][1]))))+
          temp[0]))));
        temp[2] = ((.4183*((Vpk[0][22][2]*Vpk[2][22][2])+((Vpk[0][21][0]*
          Vpk[2][22][0])+(Vpk[0][22][1]*Vpk[2][22][1]))))+((.1*((Vpk[0][21][0]*
          Vpk[2][21][0])+(Vpk[0][21][1]*Vpk[2][21][1])))+((.1*((Vpk[0][20][0]*
          Vpk[2][20][0])+(Vpk[0][20][1]*Vpk[2][20][1])))+((3.03*((Vpk[0][17][0]*
          Vpk[2][17][0])+(Vpk[0][17][1]*Vpk[2][17][1])))+temp[1]))));
        mm[0][2] = ((.2058*((Vpk[0][22][2]*Vpk[2][24][2])+((Vpk[0][24][0]*
          Vpk[2][24][0])+(Vpk[0][24][1]*Vpk[2][24][1]))))+((.4*((Vpk[0][22][2]*
          Vpk[2][23][2])+((Vpk[0][23][0]*Vpk[2][23][0])+(Vpk[0][23][1]*
          Vpk[2][23][1]))))+temp[2]));
        temp[0] = ((.4183*((Vpk[0][11][2]*Vpk[3][11][2])+((Vpk[0][10][0]*
          Vpk[3][11][0])+(Vpk[0][11][1]*Vpk[3][11][1]))))+((.1*((Vpk[0][10][0]*
          Vpk[3][10][0])+(Vpk[0][10][1]*Vpk[3][10][1])))+((.1*((Vpk[0][9][0]*
          Vpk[3][9][0])+(Vpk[0][9][1]*Vpk[3][9][1])))+((1.3192*Vpk[0][3][0])+(
          3.03*((Vpk[0][6][0]*Vpk[3][6][0])+(Vpk[0][6][1]*Vpk[3][6][1])))))));
        mm[0][3] = ((.2058*((Vpk[0][11][2]*Vpk[3][13][2])+((Vpk[0][13][0]*
          Vpk[3][13][0])+(Vpk[0][13][1]*Vpk[3][13][1]))))+((.4*((Vpk[0][11][2]*
          Vpk[3][12][2])+((Vpk[0][12][0]*Vpk[3][12][0])+(Vpk[0][12][1]*
          Vpk[3][12][1]))))+temp[0]));
        temp[0] = ((.4*((Vpk[0][11][2]*Vpk[4][11][2])+((Vpk[0][12][0]*
          Vpk[4][12][0])+(Vpk[0][12][1]*Vpk[4][12][1]))))+((.4183*((
          Vpk[0][11][2]*Vpk[4][11][2])+((Vpk[0][10][0]*Vpk[4][10][0])+(
          Vpk[0][11][1]*Vpk[4][11][1]))))+((.1*((Vpk[0][10][0]*Vpk[4][10][0])+(
          Vpk[0][10][1]*Vpk[4][10][1])))+((.1*((Vpk[0][9][0]*Vpk[4][9][0])+(
          Vpk[0][9][1]*Vpk[4][9][1])))+(3.03*((Vpk[0][6][0]*c6)-(Vpk[0][6][1]*s6
          )))))));
        mm[0][4] = ((.2058*((Vpk[0][11][2]*Vpk[4][11][2])+((Vpk[0][13][0]*
          Vpk[4][13][0])+(Vpk[0][13][1]*Vpk[4][13][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[0][11][2]*Vpk[5][11][2])+((Vpk[0][12][0]*
          Vpk[5][12][0])+(Vpk[0][12][1]*Vpk[5][12][1]))))+((.4183*((
          Vpk[0][11][2]*Vpk[5][11][2])+((Vpk[0][10][0]*Vpk[5][10][0])+(
          Vpk[0][11][1]*Vpk[5][11][1]))))+((.1*((Vpk[0][10][0]*Vpk[5][10][0])+(
          Vpk[0][10][1]*Vpk[5][10][1])))+((.1*((Vpk[0][9][0]*Vpk[5][9][0])+(
          Vpk[0][9][1]*Vpk[5][9][1])))+(3.03*((Vpk[0][6][0]*s6)+(Vpk[0][6][1]*c6
          )))))));
        mm[0][5] = ((.2058*((Vpk[0][11][2]*Vpk[5][11][2])+((Vpk[0][13][0]*
          Vpk[5][13][0])+(Vpk[0][13][1]*Vpk[5][13][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[0][11][2]*Vpk[6][12][2])+((Vpk[0][12][0]*
          Vpk[6][12][0])+(Vpk[0][12][1]*Vpk[6][12][1]))))+((.4183*((
          Vpk[0][11][2]*Vpk[6][11][2])+((Vpk[0][10][0]*Vpk[6][11][0])+(
          Vpk[0][11][1]*Vpk[6][11][1]))))+((.043*((Vpk[0][10][0]*c10)-(
          Vpk[0][10][1]*s10)))+((.1*((Vpk[0][9][0]*Vpk[6][9][0])+(Vpk[0][9][1]*
          Vpk[6][9][1])))+(.565701*Vpk[0][6][0])))));
        mm[0][6] = ((.2058*((Vpk[0][11][2]*Vpk[6][13][2])+((Vpk[0][13][0]*
          Vpk[6][13][0])+(Vpk[0][13][1]*Vpk[6][13][1]))))+temp[0]);
        mm[0][7] = (.1*((Vpk[0][9][0]*c9)-(Vpk[0][9][1]*s9)));
        mm[0][8] = (.1*((Vpk[0][9][0]*s9)+(Vpk[0][9][1]*c9)));
        mm[0][9] = 0.;
        mm[0][10] = ((.2058*((Vpk[0][11][2]*Vpk[10][13][2])+((Vpk[0][13][0]*
          Vpk[10][13][0])+(Vpk[0][13][1]*Vpk[10][13][1]))))+((.4*((Vpk[0][11][2]
          *Vpk[10][12][2])+((Vpk[0][12][0]*Vpk[10][12][0])+(Vpk[0][12][1]*
          Vpk[10][12][1]))))+(.4183*((.02377*(Vpk[0][11][2]*s11))+((
          Vpk[0][10][0]*Vpk[10][11][0])-(.02377*(Vpk[0][11][1]*c11)))))));
        mm[0][11] = ((.2058*((Vpk[0][11][2]*Vpk[11][13][2])+((Vpk[0][13][0]*
          Vpk[11][13][0])+(Vpk[0][13][1]*Vpk[11][13][1]))))+((.0062745*
          Vpk[0][11][2])+(.4*(Vpk[0][11][2]*Vpk[11][12][2]))));
        mm[0][12] = ((.2058*((Vpk[0][13][0]*Vpk[12][13][0])+(Vpk[0][13][1]*
          Vpk[12][13][1])))+(.4*((.025*Vpk[0][12][1])-(.015*Vpk[0][12][0]))));
        mm[0][13] = (.2058*((.0346*Vpk[0][13][1])-(.006*Vpk[0][13][0])));
        temp[0] = ((.4183*((Vpk[0][22][2]*Vpk[14][22][2])+((Vpk[0][21][0]*
          Vpk[14][22][0])+(Vpk[0][22][1]*Vpk[14][22][1]))))+((.1*((Vpk[0][21][0]
          *Vpk[14][21][0])+(Vpk[0][21][1]*Vpk[14][21][1])))+((.1*((Vpk[0][20][0]
          *Vpk[14][20][0])+(Vpk[0][20][1]*Vpk[14][20][1])))+((1.3192*
          Vpk[0][14][0])+(3.03*((Vpk[0][17][0]*Vpk[14][17][0])+(Vpk[0][17][1]*
          Vpk[14][17][1])))))));
        mm[0][14] = ((.2058*((Vpk[0][22][2]*Vpk[14][24][2])+((Vpk[0][24][0]*
          Vpk[14][24][0])+(Vpk[0][24][1]*Vpk[14][24][1]))))+((.4*((Vpk[0][22][2]
          *Vpk[14][23][2])+((Vpk[0][23][0]*Vpk[14][23][0])+(Vpk[0][23][1]*
          Vpk[14][23][1]))))+temp[0]));
        temp[0] = ((.4*((Vpk[0][22][2]*Vpk[15][22][2])+((Vpk[0][23][0]*
          Vpk[15][23][0])+(Vpk[0][23][1]*Vpk[15][23][1]))))+((.4183*((
          Vpk[0][22][2]*Vpk[15][22][2])+((Vpk[0][21][0]*Vpk[15][21][0])+(
          Vpk[0][22][1]*Vpk[15][22][1]))))+((.1*((Vpk[0][21][0]*Vpk[15][21][0])+
          (Vpk[0][21][1]*Vpk[15][21][1])))+((.1*((Vpk[0][20][0]*Vpk[15][20][0])+
          (Vpk[0][20][1]*Vpk[15][20][1])))+(3.03*((Vpk[0][17][0]*c17)-(
          Vpk[0][17][1]*s17)))))));
        mm[0][15] = ((.2058*((Vpk[0][22][2]*Vpk[15][22][2])+((Vpk[0][24][0]*
          Vpk[15][24][0])+(Vpk[0][24][1]*Vpk[15][24][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[0][22][2]*Vpk[16][22][2])+((Vpk[0][23][0]*
          Vpk[16][23][0])+(Vpk[0][23][1]*Vpk[16][23][1]))))+((.4183*((
          Vpk[0][22][2]*Vpk[16][22][2])+((Vpk[0][21][0]*Vpk[16][21][0])+(
          Vpk[0][22][1]*Vpk[16][22][1]))))+((.1*((Vpk[0][21][0]*Vpk[16][21][0])+
          (Vpk[0][21][1]*Vpk[16][21][1])))+((.1*((Vpk[0][20][0]*Vpk[16][20][0])+
          (Vpk[0][20][1]*Vpk[16][20][1])))+(3.03*((Vpk[0][17][0]*s17)+(
          Vpk[0][17][1]*c17)))))));
        mm[0][16] = ((.2058*((Vpk[0][22][2]*Vpk[16][22][2])+((Vpk[0][24][0]*
          Vpk[16][24][0])+(Vpk[0][24][1]*Vpk[16][24][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[0][22][2]*Vpk[17][23][2])+((Vpk[0][23][0]*
          Vpk[17][23][0])+(Vpk[0][23][1]*Vpk[17][23][1]))))+((.4183*((
          Vpk[0][22][2]*Vpk[17][22][2])+((Vpk[0][21][0]*Vpk[17][22][0])+(
          Vpk[0][22][1]*Vpk[17][22][1]))))+((.043*((Vpk[0][21][0]*c21)-(
          Vpk[0][21][1]*s21)))+((.1*((Vpk[0][20][0]*Vpk[17][20][0])+(
          Vpk[0][20][1]*Vpk[17][20][1])))+(.565701*Vpk[0][17][0])))));
        mm[0][17] = ((.2058*((Vpk[0][22][2]*Vpk[17][24][2])+((Vpk[0][24][0]*
          Vpk[17][24][0])+(Vpk[0][24][1]*Vpk[17][24][1]))))+temp[0]);
        mm[0][18] = (.1*((Vpk[0][20][0]*c20)-(Vpk[0][20][1]*s20)));
        mm[0][19] = (.1*((Vpk[0][20][0]*s20)+(Vpk[0][20][1]*c20)));
        mm[0][20] = 0.;
        mm[0][21] = ((.2058*((Vpk[0][22][2]*Vpk[21][24][2])+((Vpk[0][24][0]*
          Vpk[21][24][0])+(Vpk[0][24][1]*Vpk[21][24][1]))))+((.4*((Vpk[0][22][2]
          *Vpk[21][23][2])+((Vpk[0][23][0]*Vpk[21][23][0])+(Vpk[0][23][1]*
          Vpk[21][23][1]))))+(.4183*((.02377*(Vpk[0][22][2]*s22))+((
          Vpk[0][21][0]*Vpk[21][22][0])-(.02377*(Vpk[0][22][1]*c22)))))));
        mm[0][22] = ((.2058*((Vpk[0][22][2]*Vpk[22][24][2])+((Vpk[0][24][0]*
          Vpk[22][24][0])+(Vpk[0][24][1]*Vpk[22][24][1]))))+((.0062745*
          Vpk[0][22][2])+(.4*(Vpk[0][22][2]*Vpk[22][23][2]))));
        mm[0][23] = ((.2058*((Vpk[0][24][0]*Vpk[23][24][0])+(Vpk[0][24][1]*
          Vpk[23][24][1])))+(.4*((.025*Vpk[0][23][1])-(.015*Vpk[0][23][0]))));
        mm[0][24] = (.2058*((.0346*Vpk[0][24][1])-(.006*Vpk[0][24][0])));
        temp[0] = ((.4183*((Vpk[1][11][2]*Vpk[1][11][2])+((Vpk[1][10][0]*
          Vpk[1][10][0])+(Vpk[1][11][1]*Vpk[1][11][1]))))+((.1*((Vpk[1][10][0]*
          Vpk[1][10][0])+(Vpk[1][10][1]*Vpk[1][10][1])))+((.1*((Vpk[1][9][0]*
          Vpk[1][9][0])+(Vpk[1][9][1]*Vpk[1][9][1])))+((3.03*((Vpk[1][6][0]*
          Vpk[1][6][0])+(Vpk[1][6][1]*Vpk[1][6][1])))+(51.1+(7.76*((Vpk[1][3][0]
          *Vpk[1][3][0])+(Vpk[1][3][1]*Vpk[1][3][1]))))))));
        temp[1] = ((3.03*((Vpk[1][17][0]*Vpk[1][17][0])+(Vpk[1][17][1]*
          Vpk[1][17][1])))+((7.76*((Vpk[1][14][0]*Vpk[1][14][0])+(Vpk[1][14][1]*
          Vpk[1][14][1])))+((.2058*((Vpk[1][11][2]*Vpk[1][11][2])+((
          Vpk[1][13][0]*Vpk[1][13][0])+(Vpk[1][13][1]*Vpk[1][13][1]))))+((.4*((
          Vpk[1][11][2]*Vpk[1][11][2])+((Vpk[1][12][0]*Vpk[1][12][0])+(
          Vpk[1][12][1]*Vpk[1][12][1]))))+temp[0]))));
        temp[2] = ((.4*((Vpk[1][22][2]*Vpk[1][22][2])+((Vpk[1][23][0]*
          Vpk[1][23][0])+(Vpk[1][23][1]*Vpk[1][23][1]))))+((.4183*((
          Vpk[1][22][2]*Vpk[1][22][2])+((Vpk[1][21][0]*Vpk[1][21][0])+(
          Vpk[1][22][1]*Vpk[1][22][1]))))+((.1*((Vpk[1][21][0]*Vpk[1][21][0])+(
          Vpk[1][21][1]*Vpk[1][21][1])))+((.1*((Vpk[1][20][0]*Vpk[1][20][0])+(
          Vpk[1][20][1]*Vpk[1][20][1])))+temp[1]))));
        mm[1][1] = ((.2058*((Vpk[1][22][2]*Vpk[1][22][2])+((Vpk[1][24][0]*
          Vpk[1][24][0])+(Vpk[1][24][1]*Vpk[1][24][1]))))+temp[2]);
        temp[0] = ((.1*((Vpk[1][10][0]*Vpk[2][10][0])+(Vpk[1][10][1]*
          Vpk[2][10][1])))+((.1*((Vpk[1][9][0]*Vpk[2][9][0])+(Vpk[1][9][1]*
          Vpk[2][9][1])))+((3.03*((Vpk[1][6][0]*Vpk[2][6][0])+(Vpk[1][6][1]*
          Vpk[2][6][1])))+((7.76*((Vpk[1][3][0]*Vpk[2][3][0])+(Vpk[1][3][1]*
          Vpk[2][3][1])))-(51.1*((.0707*c2)+(.2639*s2)))))));
        temp[1] = ((7.76*((Vpk[1][14][0]*Vpk[2][14][0])+(Vpk[1][14][1]*
          Vpk[2][14][1])))+((.2058*((Vpk[1][11][2]*Vpk[2][13][2])+((
          Vpk[1][13][0]*Vpk[2][13][0])+(Vpk[1][13][1]*Vpk[2][13][1]))))+((.4*((
          Vpk[1][11][2]*Vpk[2][12][2])+((Vpk[1][12][0]*Vpk[2][12][0])+(
          Vpk[1][12][1]*Vpk[2][12][1]))))+((.4183*((Vpk[1][11][2]*Vpk[2][11][2])
          +((Vpk[1][10][0]*Vpk[2][11][0])+(Vpk[1][11][1]*Vpk[2][11][1]))))+
          temp[0]))));
        temp[2] = ((.4183*((Vpk[1][22][2]*Vpk[2][22][2])+((Vpk[1][21][0]*
          Vpk[2][22][0])+(Vpk[1][22][1]*Vpk[2][22][1]))))+((.1*((Vpk[1][21][0]*
          Vpk[2][21][0])+(Vpk[1][21][1]*Vpk[2][21][1])))+((.1*((Vpk[1][20][0]*
          Vpk[2][20][0])+(Vpk[1][20][1]*Vpk[2][20][1])))+((3.03*((Vpk[1][17][0]*
          Vpk[2][17][0])+(Vpk[1][17][1]*Vpk[2][17][1])))+temp[1]))));
        mm[1][2] = ((.2058*((Vpk[1][22][2]*Vpk[2][24][2])+((Vpk[1][24][0]*
          Vpk[2][24][0])+(Vpk[1][24][1]*Vpk[2][24][1]))))+((.4*((Vpk[1][22][2]*
          Vpk[2][23][2])+((Vpk[1][23][0]*Vpk[2][23][0])+(Vpk[1][23][1]*
          Vpk[2][23][1]))))+temp[2]));
        temp[0] = ((.4183*((Vpk[1][11][2]*Vpk[3][11][2])+((Vpk[1][10][0]*
          Vpk[3][11][0])+(Vpk[1][11][1]*Vpk[3][11][1]))))+((.1*((Vpk[1][10][0]*
          Vpk[3][10][0])+(Vpk[1][10][1]*Vpk[3][10][1])))+((.1*((Vpk[1][9][0]*
          Vpk[3][9][0])+(Vpk[1][9][1]*Vpk[3][9][1])))+((1.3192*Vpk[1][3][0])+(
          3.03*((Vpk[1][6][0]*Vpk[3][6][0])+(Vpk[1][6][1]*Vpk[3][6][1])))))));
        mm[1][3] = ((.2058*((Vpk[1][11][2]*Vpk[3][13][2])+((Vpk[1][13][0]*
          Vpk[3][13][0])+(Vpk[1][13][1]*Vpk[3][13][1]))))+((.4*((Vpk[1][11][2]*
          Vpk[3][12][2])+((Vpk[1][12][0]*Vpk[3][12][0])+(Vpk[1][12][1]*
          Vpk[3][12][1]))))+temp[0]));
        temp[0] = ((.4*((Vpk[1][11][2]*Vpk[4][11][2])+((Vpk[1][12][0]*
          Vpk[4][12][0])+(Vpk[1][12][1]*Vpk[4][12][1]))))+((.4183*((
          Vpk[1][11][2]*Vpk[4][11][2])+((Vpk[1][10][0]*Vpk[4][10][0])+(
          Vpk[1][11][1]*Vpk[4][11][1]))))+((.1*((Vpk[1][10][0]*Vpk[4][10][0])+(
          Vpk[1][10][1]*Vpk[4][10][1])))+((.1*((Vpk[1][9][0]*Vpk[4][9][0])+(
          Vpk[1][9][1]*Vpk[4][9][1])))+(3.03*((Vpk[1][6][0]*c6)-(Vpk[1][6][1]*s6
          )))))));
        mm[1][4] = ((.2058*((Vpk[1][11][2]*Vpk[4][11][2])+((Vpk[1][13][0]*
          Vpk[4][13][0])+(Vpk[1][13][1]*Vpk[4][13][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[1][11][2]*Vpk[5][11][2])+((Vpk[1][12][0]*
          Vpk[5][12][0])+(Vpk[1][12][1]*Vpk[5][12][1]))))+((.4183*((
          Vpk[1][11][2]*Vpk[5][11][2])+((Vpk[1][10][0]*Vpk[5][10][0])+(
          Vpk[1][11][1]*Vpk[5][11][1]))))+((.1*((Vpk[1][10][0]*Vpk[5][10][0])+(
          Vpk[1][10][1]*Vpk[5][10][1])))+((.1*((Vpk[1][9][0]*Vpk[5][9][0])+(
          Vpk[1][9][1]*Vpk[5][9][1])))+(3.03*((Vpk[1][6][0]*s6)+(Vpk[1][6][1]*c6
          )))))));
        mm[1][5] = ((.2058*((Vpk[1][11][2]*Vpk[5][11][2])+((Vpk[1][13][0]*
          Vpk[5][13][0])+(Vpk[1][13][1]*Vpk[5][13][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[1][11][2]*Vpk[6][12][2])+((Vpk[1][12][0]*
          Vpk[6][12][0])+(Vpk[1][12][1]*Vpk[6][12][1]))))+((.4183*((
          Vpk[1][11][2]*Vpk[6][11][2])+((Vpk[1][10][0]*Vpk[6][11][0])+(
          Vpk[1][11][1]*Vpk[6][11][1]))))+((.043*((Vpk[1][10][0]*c10)-(
          Vpk[1][10][1]*s10)))+((.1*((Vpk[1][9][0]*Vpk[6][9][0])+(Vpk[1][9][1]*
          Vpk[6][9][1])))+(.565701*Vpk[1][6][0])))));
        mm[1][6] = ((.2058*((Vpk[1][11][2]*Vpk[6][13][2])+((Vpk[1][13][0]*
          Vpk[6][13][0])+(Vpk[1][13][1]*Vpk[6][13][1]))))+temp[0]);
        mm[1][7] = (.1*((Vpk[1][9][0]*c9)-(Vpk[1][9][1]*s9)));
        mm[1][8] = (.1*((Vpk[1][9][0]*s9)+(Vpk[1][9][1]*c9)));
        mm[1][9] = 0.;
        mm[1][10] = ((.2058*((Vpk[1][11][2]*Vpk[10][13][2])+((Vpk[1][13][0]*
          Vpk[10][13][0])+(Vpk[1][13][1]*Vpk[10][13][1]))))+((.4*((Vpk[1][11][2]
          *Vpk[10][12][2])+((Vpk[1][12][0]*Vpk[10][12][0])+(Vpk[1][12][1]*
          Vpk[10][12][1]))))+(.4183*((.02377*(Vpk[1][11][2]*s11))+((
          Vpk[1][10][0]*Vpk[10][11][0])-(.02377*(Vpk[1][11][1]*c11)))))));
        mm[1][11] = ((.2058*((Vpk[1][11][2]*Vpk[11][13][2])+((Vpk[1][13][0]*
          Vpk[11][13][0])+(Vpk[1][13][1]*Vpk[11][13][1]))))+((.0062745*
          Vpk[1][11][2])+(.4*(Vpk[1][11][2]*Vpk[11][12][2]))));
        mm[1][12] = ((.2058*((Vpk[1][13][0]*Vpk[12][13][0])+(Vpk[1][13][1]*
          Vpk[12][13][1])))+(.4*((.025*Vpk[1][12][1])-(.015*Vpk[1][12][0]))));
        mm[1][13] = (.2058*((.0346*Vpk[1][13][1])-(.006*Vpk[1][13][0])));
        temp[0] = ((.4183*((Vpk[1][22][2]*Vpk[14][22][2])+((Vpk[1][21][0]*
          Vpk[14][22][0])+(Vpk[1][22][1]*Vpk[14][22][1]))))+((.1*((Vpk[1][21][0]
          *Vpk[14][21][0])+(Vpk[1][21][1]*Vpk[14][21][1])))+((.1*((Vpk[1][20][0]
          *Vpk[14][20][0])+(Vpk[1][20][1]*Vpk[14][20][1])))+((1.3192*
          Vpk[1][14][0])+(3.03*((Vpk[1][17][0]*Vpk[14][17][0])+(Vpk[1][17][1]*
          Vpk[14][17][1])))))));
        mm[1][14] = ((.2058*((Vpk[1][22][2]*Vpk[14][24][2])+((Vpk[1][24][0]*
          Vpk[14][24][0])+(Vpk[1][24][1]*Vpk[14][24][1]))))+((.4*((Vpk[1][22][2]
          *Vpk[14][23][2])+((Vpk[1][23][0]*Vpk[14][23][0])+(Vpk[1][23][1]*
          Vpk[14][23][1]))))+temp[0]));
        temp[0] = ((.4*((Vpk[1][22][2]*Vpk[15][22][2])+((Vpk[1][23][0]*
          Vpk[15][23][0])+(Vpk[1][23][1]*Vpk[15][23][1]))))+((.4183*((
          Vpk[1][22][2]*Vpk[15][22][2])+((Vpk[1][21][0]*Vpk[15][21][0])+(
          Vpk[1][22][1]*Vpk[15][22][1]))))+((.1*((Vpk[1][21][0]*Vpk[15][21][0])+
          (Vpk[1][21][1]*Vpk[15][21][1])))+((.1*((Vpk[1][20][0]*Vpk[15][20][0])+
          (Vpk[1][20][1]*Vpk[15][20][1])))+(3.03*((Vpk[1][17][0]*c17)-(
          Vpk[1][17][1]*s17)))))));
        mm[1][15] = ((.2058*((Vpk[1][22][2]*Vpk[15][22][2])+((Vpk[1][24][0]*
          Vpk[15][24][0])+(Vpk[1][24][1]*Vpk[15][24][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[1][22][2]*Vpk[16][22][2])+((Vpk[1][23][0]*
          Vpk[16][23][0])+(Vpk[1][23][1]*Vpk[16][23][1]))))+((.4183*((
          Vpk[1][22][2]*Vpk[16][22][2])+((Vpk[1][21][0]*Vpk[16][21][0])+(
          Vpk[1][22][1]*Vpk[16][22][1]))))+((.1*((Vpk[1][21][0]*Vpk[16][21][0])+
          (Vpk[1][21][1]*Vpk[16][21][1])))+((.1*((Vpk[1][20][0]*Vpk[16][20][0])+
          (Vpk[1][20][1]*Vpk[16][20][1])))+(3.03*((Vpk[1][17][0]*s17)+(
          Vpk[1][17][1]*c17)))))));
        mm[1][16] = ((.2058*((Vpk[1][22][2]*Vpk[16][22][2])+((Vpk[1][24][0]*
          Vpk[16][24][0])+(Vpk[1][24][1]*Vpk[16][24][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[1][22][2]*Vpk[17][23][2])+((Vpk[1][23][0]*
          Vpk[17][23][0])+(Vpk[1][23][1]*Vpk[17][23][1]))))+((.4183*((
          Vpk[1][22][2]*Vpk[17][22][2])+((Vpk[1][21][0]*Vpk[17][22][0])+(
          Vpk[1][22][1]*Vpk[17][22][1]))))+((.043*((Vpk[1][21][0]*c21)-(
          Vpk[1][21][1]*s21)))+((.1*((Vpk[1][20][0]*Vpk[17][20][0])+(
          Vpk[1][20][1]*Vpk[17][20][1])))+(.565701*Vpk[1][17][0])))));
        mm[1][17] = ((.2058*((Vpk[1][22][2]*Vpk[17][24][2])+((Vpk[1][24][0]*
          Vpk[17][24][0])+(Vpk[1][24][1]*Vpk[17][24][1]))))+temp[0]);
        mm[1][18] = (.1*((Vpk[1][20][0]*c20)-(Vpk[1][20][1]*s20)));
        mm[1][19] = (.1*((Vpk[1][20][0]*s20)+(Vpk[1][20][1]*c20)));
        mm[1][20] = 0.;
        mm[1][21] = ((.2058*((Vpk[1][22][2]*Vpk[21][24][2])+((Vpk[1][24][0]*
          Vpk[21][24][0])+(Vpk[1][24][1]*Vpk[21][24][1]))))+((.4*((Vpk[1][22][2]
          *Vpk[21][23][2])+((Vpk[1][23][0]*Vpk[21][23][0])+(Vpk[1][23][1]*
          Vpk[21][23][1]))))+(.4183*((.02377*(Vpk[1][22][2]*s22))+((
          Vpk[1][21][0]*Vpk[21][22][0])-(.02377*(Vpk[1][22][1]*c22)))))));
        mm[1][22] = ((.2058*((Vpk[1][22][2]*Vpk[22][24][2])+((Vpk[1][24][0]*
          Vpk[22][24][0])+(Vpk[1][24][1]*Vpk[22][24][1]))))+((.0062745*
          Vpk[1][22][2])+(.4*(Vpk[1][22][2]*Vpk[22][23][2]))));
        mm[1][23] = ((.2058*((Vpk[1][24][0]*Vpk[23][24][0])+(Vpk[1][24][1]*
          Vpk[23][24][1])))+(.4*((.025*Vpk[1][23][1])-(.015*Vpk[1][23][0]))));
        mm[1][24] = (.2058*((.0346*Vpk[1][24][1])-(.006*Vpk[1][24][0])));
        temp[0] = ((.4183*((Vpk[2][11][2]*Vpk[2][11][2])+((Vpk[2][11][0]*
          Vpk[2][11][0])+(Vpk[2][11][1]*Vpk[2][11][1]))))+((.1*((Vpk[2][10][0]*
          Vpk[2][10][0])+(Vpk[2][10][1]*Vpk[2][10][1])))+((.1*((Vpk[2][9][0]*
          Vpk[2][9][0])+(Vpk[2][9][1]*Vpk[2][9][1])))+((3.03*((Vpk[2][6][0]*
          Vpk[2][6][0])+(Vpk[2][6][1]*Vpk[2][6][1])))+(7.76*((Vpk[2][3][0]*
          Vpk[2][3][0])+(Vpk[2][3][1]*Vpk[2][3][1])))))));
        temp[1] = (((.001*((c11*c11)+((Wpk[2][13][0]*Wpk[2][13][0])+(
          Wpk[2][13][1]*Wpk[2][13][1]))))+(.2058*((Vpk[2][13][2]*Vpk[2][13][2])+
          ((Vpk[2][13][0]*Vpk[2][13][0])+(Vpk[2][13][1]*Vpk[2][13][1])))))+(((
          .001*((c11*c11)+((Wpk[2][12][0]*Wpk[2][12][0])+(Wpk[2][12][1]*
          Wpk[2][12][1]))))+(.4*((Vpk[2][12][2]*Vpk[2][12][2])+((Vpk[2][12][0]*
          Vpk[2][12][0])+(Vpk[2][12][1]*Vpk[2][12][1])))))+temp[0]));
        temp[2] = ((.4183*((Vpk[2][22][2]*Vpk[2][22][2])+((Vpk[2][22][0]*
          Vpk[2][22][0])+(Vpk[2][22][1]*Vpk[2][22][1]))))+((.1*((Vpk[2][21][0]*
          Vpk[2][21][0])+(Vpk[2][21][1]*Vpk[2][21][1])))+((.1*((Vpk[2][20][0]*
          Vpk[2][20][0])+(Vpk[2][20][1]*Vpk[2][20][1])))+((3.03*((Vpk[2][17][0]*
          Vpk[2][17][0])+(Vpk[2][17][1]*Vpk[2][17][1])))+((7.76*((Vpk[2][14][0]*
          Vpk[2][14][0])+(Vpk[2][14][1]*Vpk[2][14][1])))+temp[1])))));
        mm[2][2] = (6.76319087+(((.001*((c22*c22)+((Wpk[2][24][0]*Wpk[2][24][0])
          +(Wpk[2][24][1]*Wpk[2][24][1]))))+(.2058*((Vpk[2][24][2]*Vpk[2][24][2]
          )+((Vpk[2][24][0]*Vpk[2][24][0])+(Vpk[2][24][1]*Vpk[2][24][1])))))+(((
          .001*((c22*c22)+((Wpk[2][23][0]*Wpk[2][23][0])+(Wpk[2][23][1]*
          Wpk[2][23][1]))))+(.4*((Vpk[2][23][2]*Vpk[2][23][2])+((Vpk[2][23][0]*
          Vpk[2][23][0])+(Vpk[2][23][1]*Vpk[2][23][1])))))+temp[2])));
        temp[0] = ((.4183*((Vpk[2][11][2]*Vpk[3][11][2])+((Vpk[2][11][0]*
          Vpk[3][11][0])+(Vpk[2][11][1]*Vpk[3][11][1]))))+((.1*((Vpk[2][10][0]*
          Vpk[3][10][0])+(Vpk[2][10][1]*Vpk[3][10][1])))+((.1*((Vpk[2][9][0]*
          Vpk[3][9][0])+(Vpk[2][9][1]*Vpk[3][9][1])))+((1.3192*Vpk[2][3][0])+(
          3.03*((Vpk[2][6][0]*Vpk[3][6][0])+(Vpk[2][6][1]*Vpk[3][6][1])))))));
        mm[2][3] = (.1845+(((.001*((c11*c11)+((Wpk[2][13][0]*Wpk[3][13][0])+(
          Wpk[2][13][1]*Wpk[3][13][1]))))+(.2058*((Vpk[2][13][2]*Vpk[3][13][2])+
          ((Vpk[2][13][0]*Vpk[3][13][0])+(Vpk[2][13][1]*Vpk[3][13][1])))))+(((
          .001*((c11*c11)+((Wpk[2][12][0]*Wpk[3][12][0])+(Wpk[2][12][1]*
          Wpk[3][12][1]))))+(.4*((Vpk[2][12][2]*Vpk[3][12][2])+((Vpk[2][12][0]*
          Vpk[3][12][0])+(Vpk[2][12][1]*Vpk[3][12][1])))))+temp[0])));
        temp[0] = ((.4*((Vpk[2][12][2]*Vpk[4][11][2])+((Vpk[2][12][0]*
          Vpk[4][12][0])+(Vpk[2][12][1]*Vpk[4][12][1]))))+((.4183*((
          Vpk[2][11][2]*Vpk[4][11][2])+((Vpk[2][11][0]*Vpk[4][10][0])+(
          Vpk[2][11][1]*Vpk[4][11][1]))))+((.1*((Vpk[2][10][0]*Vpk[4][10][0])+(
          Vpk[2][10][1]*Vpk[4][10][1])))+((.1*((Vpk[2][9][0]*Vpk[4][9][0])+(
          Vpk[2][9][1]*Vpk[4][9][1])))+(3.03*((Vpk[2][6][0]*c6)-(Vpk[2][6][1]*s6
          )))))));
        mm[2][4] = ((.2058*((Vpk[2][13][2]*Vpk[4][11][2])+((Vpk[2][13][0]*
          Vpk[4][13][0])+(Vpk[2][13][1]*Vpk[4][13][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[2][12][2]*Vpk[5][11][2])+((Vpk[2][12][0]*
          Vpk[5][12][0])+(Vpk[2][12][1]*Vpk[5][12][1]))))+((.4183*((
          Vpk[2][11][2]*Vpk[5][11][2])+((Vpk[2][11][0]*Vpk[5][10][0])+(
          Vpk[2][11][1]*Vpk[5][11][1]))))+((.1*((Vpk[2][10][0]*Vpk[5][10][0])+(
          Vpk[2][10][1]*Vpk[5][10][1])))+((.1*((Vpk[2][9][0]*Vpk[5][9][0])+(
          Vpk[2][9][1]*Vpk[5][9][1])))+(3.03*((Vpk[2][6][0]*s6)+(Vpk[2][6][1]*c6
          )))))));
        mm[2][5] = ((.2058*((Vpk[2][13][2]*Vpk[5][11][2])+((Vpk[2][13][0]*
          Vpk[5][13][0])+(Vpk[2][13][1]*Vpk[5][13][1]))))+temp[0]);
        temp[0] = ((.4183*((Vpk[2][11][2]*Vpk[6][11][2])+((Vpk[2][11][0]*
          Vpk[6][11][0])+(Vpk[2][11][1]*Vpk[6][11][1]))))+((.043*((Vpk[2][10][0]
          *c10)-(Vpk[2][10][1]*s10)))+((.1*((Vpk[2][9][0]*Vpk[6][9][0])+(
          Vpk[2][9][1]*Vpk[6][9][1])))+(.565701*Vpk[2][6][0]))));
        mm[2][6] = (.0475+(((.001*((c11*c11)+((Wpk[2][13][0]*Wpk[6][13][0])+(
          Wpk[2][13][1]*Wpk[6][13][1]))))+(.2058*((Vpk[2][13][2]*Vpk[6][13][2])+
          ((Vpk[2][13][0]*Vpk[6][13][0])+(Vpk[2][13][1]*Vpk[6][13][1])))))+(((
          .001*((c11*c11)+((Wpk[2][12][0]*Wpk[6][12][0])+(Wpk[2][12][1]*
          Wpk[6][12][1]))))+(.4*((Vpk[2][12][2]*Vpk[6][12][2])+((Vpk[2][12][0]*
          Vpk[6][12][0])+(Vpk[2][12][1]*Vpk[6][12][1])))))+temp[0])));
        mm[2][7] = (.1*((Vpk[2][9][0]*c9)-(Vpk[2][9][1]*s9)));
        mm[2][8] = (.1*((Vpk[2][9][0]*s9)+(Vpk[2][9][1]*c9)));
        mm[2][9] = .001;
        temp[0] = ((.4183*((.02377*(Vpk[2][11][2]*s11))+((Vpk[2][11][0]*
          Vpk[10][11][0])-(.02377*(Vpk[2][11][1]*c11)))))+((.001*((c11*c11)+((
          Wpk[2][12][0]*Wpk[10][12][0])+(Wpk[2][12][1]*Wpk[10][12][1]))))+(.4*((
          Vpk[2][12][2]*Vpk[10][12][2])+((Vpk[2][12][0]*Vpk[10][12][0])+(
          Vpk[2][12][1]*Vpk[10][12][1]))))));
        mm[2][10] = (.002+(((.001*((c11*c11)+((Wpk[2][13][0]*Wpk[10][13][0])+(
          Wpk[2][13][1]*Wpk[10][13][1]))))+(.2058*((Vpk[2][13][2]*Vpk[10][13][2]
          )+((Vpk[2][13][0]*Vpk[10][13][0])+(Vpk[2][13][1]*Vpk[10][13][1])))))+
          temp[0]));
        mm[2][11] = (((.001*((Wpk[2][13][0]*Wpk[11][13][0])+(Wpk[2][13][1]*
          Wpk[11][13][1])))+(.2058*((Vpk[2][13][2]*Vpk[11][13][2])+((
          Vpk[2][13][0]*Vpk[11][13][0])+(Vpk[2][13][1]*Vpk[11][13][1])))))+((
          .0062745*Vpk[2][11][2])+((.001*((Wpk[2][12][0]*c12)-(Wpk[2][12][1]*s12
          )))+(.4*(Vpk[2][12][2]*Vpk[11][12][2])))));
        mm[2][12] = (((.001*c11)+(.2058*((Vpk[2][13][0]*Vpk[12][13][0])+(
          Vpk[2][13][1]*Vpk[12][13][1]))))+((.001*c11)+(.4*((.025*Vpk[2][12][1])
          -(.015*Vpk[2][12][0])))));
        mm[2][13] = ((.001*c11)+(.2058*((.0346*Vpk[2][13][1])-(.006*
          Vpk[2][13][0]))));
        temp[0] = ((.4183*((Vpk[2][22][2]*Vpk[14][22][2])+((Vpk[2][22][0]*
          Vpk[14][22][0])+(Vpk[2][22][1]*Vpk[14][22][1]))))+((.1*((Vpk[2][21][0]
          *Vpk[14][21][0])+(Vpk[2][21][1]*Vpk[14][21][1])))+((.1*((Vpk[2][20][0]
          *Vpk[14][20][0])+(Vpk[2][20][1]*Vpk[14][20][1])))+((1.3192*
          Vpk[2][14][0])+(3.03*((Vpk[2][17][0]*Vpk[14][17][0])+(Vpk[2][17][1]*
          Vpk[14][17][1])))))));
        mm[2][14] = (.1845+(((.001*((c22*c22)+((Wpk[2][24][0]*Wpk[14][24][0])+(
          Wpk[2][24][1]*Wpk[14][24][1]))))+(.2058*((Vpk[2][24][2]*Vpk[14][24][2]
          )+((Vpk[2][24][0]*Vpk[14][24][0])+(Vpk[2][24][1]*Vpk[14][24][1])))))+(
          ((.001*((c22*c22)+((Wpk[2][23][0]*Wpk[14][23][0])+(Wpk[2][23][1]*
          Wpk[14][23][1]))))+(.4*((Vpk[2][23][2]*Vpk[14][23][2])+((Vpk[2][23][0]
          *Vpk[14][23][0])+(Vpk[2][23][1]*Vpk[14][23][1])))))+temp[0])));
        temp[0] = ((.4*((Vpk[2][23][2]*Vpk[15][22][2])+((Vpk[2][23][0]*
          Vpk[15][23][0])+(Vpk[2][23][1]*Vpk[15][23][1]))))+((.4183*((
          Vpk[2][22][2]*Vpk[15][22][2])+((Vpk[2][22][0]*Vpk[15][21][0])+(
          Vpk[2][22][1]*Vpk[15][22][1]))))+((.1*((Vpk[2][21][0]*Vpk[15][21][0])+
          (Vpk[2][21][1]*Vpk[15][21][1])))+((.1*((Vpk[2][20][0]*Vpk[15][20][0])+
          (Vpk[2][20][1]*Vpk[15][20][1])))+(3.03*((Vpk[2][17][0]*c17)-(
          Vpk[2][17][1]*s17)))))));
        mm[2][15] = ((.2058*((Vpk[2][24][2]*Vpk[15][22][2])+((Vpk[2][24][0]*
          Vpk[15][24][0])+(Vpk[2][24][1]*Vpk[15][24][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[2][23][2]*Vpk[16][22][2])+((Vpk[2][23][0]*
          Vpk[16][23][0])+(Vpk[2][23][1]*Vpk[16][23][1]))))+((.4183*((
          Vpk[2][22][2]*Vpk[16][22][2])+((Vpk[2][22][0]*Vpk[16][21][0])+(
          Vpk[2][22][1]*Vpk[16][22][1]))))+((.1*((Vpk[2][21][0]*Vpk[16][21][0])+
          (Vpk[2][21][1]*Vpk[16][21][1])))+((.1*((Vpk[2][20][0]*Vpk[16][20][0])+
          (Vpk[2][20][1]*Vpk[16][20][1])))+(3.03*((Vpk[2][17][0]*s17)+(
          Vpk[2][17][1]*c17)))))));
        mm[2][16] = ((.2058*((Vpk[2][24][2]*Vpk[16][22][2])+((Vpk[2][24][0]*
          Vpk[16][24][0])+(Vpk[2][24][1]*Vpk[16][24][1]))))+temp[0]);
        temp[0] = ((.4183*((Vpk[2][22][2]*Vpk[17][22][2])+((Vpk[2][22][0]*
          Vpk[17][22][0])+(Vpk[2][22][1]*Vpk[17][22][1]))))+((.043*((
          Vpk[2][21][0]*c21)-(Vpk[2][21][1]*s21)))+((.1*((Vpk[2][20][0]*
          Vpk[17][20][0])+(Vpk[2][20][1]*Vpk[17][20][1])))+(.565701*
          Vpk[2][17][0]))));
        mm[2][17] = (.0475+(((.001*((c22*c22)+((Wpk[2][24][0]*Wpk[17][24][0])+(
          Wpk[2][24][1]*Wpk[17][24][1]))))+(.2058*((Vpk[2][24][2]*Vpk[17][24][2]
          )+((Vpk[2][24][0]*Vpk[17][24][0])+(Vpk[2][24][1]*Vpk[17][24][1])))))+(
          ((.001*((c22*c22)+((Wpk[2][23][0]*Wpk[17][23][0])+(Wpk[2][23][1]*
          Wpk[17][23][1]))))+(.4*((Vpk[2][23][2]*Vpk[17][23][2])+((Vpk[2][23][0]
          *Vpk[17][23][0])+(Vpk[2][23][1]*Vpk[17][23][1])))))+temp[0])));
        mm[2][18] = (.1*((Vpk[2][20][0]*c20)-(Vpk[2][20][1]*s20)));
        mm[2][19] = (.1*((Vpk[2][20][0]*s20)+(Vpk[2][20][1]*c20)));
        mm[2][20] = .001;
        temp[0] = ((.4183*((.02377*(Vpk[2][22][2]*s22))+((Vpk[2][22][0]*
          Vpk[21][22][0])-(.02377*(Vpk[2][22][1]*c22)))))+((.001*((c22*c22)+((
          Wpk[2][23][0]*Wpk[21][23][0])+(Wpk[2][23][1]*Wpk[21][23][1]))))+(.4*((
          Vpk[2][23][2]*Vpk[21][23][2])+((Vpk[2][23][0]*Vpk[21][23][0])+(
          Vpk[2][23][1]*Vpk[21][23][1]))))));
        mm[2][21] = (.002+(((.001*((c22*c22)+((Wpk[2][24][0]*Wpk[21][24][0])+(
          Wpk[2][24][1]*Wpk[21][24][1]))))+(.2058*((Vpk[2][24][2]*Vpk[21][24][2]
          )+((Vpk[2][24][0]*Vpk[21][24][0])+(Vpk[2][24][1]*Vpk[21][24][1])))))+
          temp[0]));
        mm[2][22] = (((.001*((Wpk[2][24][0]*Wpk[22][24][0])+(Wpk[2][24][1]*
          Wpk[22][24][1])))+(.2058*((Vpk[2][24][2]*Vpk[22][24][2])+((
          Vpk[2][24][0]*Vpk[22][24][0])+(Vpk[2][24][1]*Vpk[22][24][1])))))+((
          .0062745*Vpk[2][22][2])+((.001*((Wpk[2][23][0]*c23)-(Wpk[2][23][1]*s23
          )))+(.4*(Vpk[2][23][2]*Vpk[22][23][2])))));
        mm[2][23] = (((.001*c22)+(.2058*((Vpk[2][24][0]*Vpk[23][24][0])+(
          Vpk[2][24][1]*Vpk[23][24][1]))))+((.001*c22)+(.4*((.025*Vpk[2][23][1])
          -(.015*Vpk[2][23][0])))));
        mm[2][24] = ((.001*c22)+(.2058*((.0346*Vpk[2][24][1])-(.006*
          Vpk[2][24][0]))));
        temp[0] = ((.4183*((Vpk[3][11][2]*Vpk[3][11][2])+((Vpk[3][11][0]*
          Vpk[3][11][0])+(Vpk[3][11][1]*Vpk[3][11][1]))))+((.1*((Vpk[3][10][0]*
          Vpk[3][10][0])+(Vpk[3][10][1]*Vpk[3][10][1])))+((.1*((Vpk[3][9][0]*
          Vpk[3][9][0])+(Vpk[3][9][1]*Vpk[3][9][1])))+(3.03*((Vpk[3][6][0]*
          Vpk[3][6][0])+(Vpk[3][6][1]*Vpk[3][6][1]))))));
        mm[3][3] = (.408764+(((.001*((c11*c11)+((Wpk[3][13][0]*Wpk[3][13][0])+(
          Wpk[3][13][1]*Wpk[3][13][1]))))+(.2058*((Vpk[3][13][2]*Vpk[3][13][2])+
          ((Vpk[3][13][0]*Vpk[3][13][0])+(Vpk[3][13][1]*Vpk[3][13][1])))))+(((
          .001*((c11*c11)+((Wpk[3][12][0]*Wpk[3][12][0])+(Wpk[3][12][1]*
          Wpk[3][12][1]))))+(.4*((Vpk[3][12][2]*Vpk[3][12][2])+((Vpk[3][12][0]*
          Vpk[3][12][0])+(Vpk[3][12][1]*Vpk[3][12][1])))))+temp[0])));
        temp[0] = ((.4*((Vpk[3][12][2]*Vpk[4][11][2])+((Vpk[3][12][0]*
          Vpk[4][12][0])+(Vpk[3][12][1]*Vpk[4][12][1]))))+((.4183*((
          Vpk[3][11][2]*Vpk[4][11][2])+((Vpk[3][11][0]*Vpk[4][10][0])+(
          Vpk[3][11][1]*Vpk[4][11][1]))))+((.1*((Vpk[3][10][0]*Vpk[4][10][0])+(
          Vpk[3][10][1]*Vpk[4][10][1])))+((.1*((Vpk[3][9][0]*Vpk[4][9][0])+(
          Vpk[3][9][1]*Vpk[4][9][1])))+(3.03*((Vpk[3][6][0]*c6)-(Vpk[3][6][1]*s6
          )))))));
        mm[3][4] = ((.2058*((Vpk[3][13][2]*Vpk[4][11][2])+((Vpk[3][13][0]*
          Vpk[4][13][0])+(Vpk[3][13][1]*Vpk[4][13][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[3][12][2]*Vpk[5][11][2])+((Vpk[3][12][0]*
          Vpk[5][12][0])+(Vpk[3][12][1]*Vpk[5][12][1]))))+((.4183*((
          Vpk[3][11][2]*Vpk[5][11][2])+((Vpk[3][11][0]*Vpk[5][10][0])+(
          Vpk[3][11][1]*Vpk[5][11][1]))))+((.1*((Vpk[3][10][0]*Vpk[5][10][0])+(
          Vpk[3][10][1]*Vpk[5][10][1])))+((.1*((Vpk[3][9][0]*Vpk[5][9][0])+(
          Vpk[3][9][1]*Vpk[5][9][1])))+(3.03*((Vpk[3][6][0]*s6)+(Vpk[3][6][1]*c6
          )))))));
        mm[3][5] = ((.2058*((Vpk[3][13][2]*Vpk[5][11][2])+((Vpk[3][13][0]*
          Vpk[5][13][0])+(Vpk[3][13][1]*Vpk[5][13][1]))))+temp[0]);
        temp[0] = ((.4183*((Vpk[3][11][2]*Vpk[6][11][2])+((Vpk[3][11][0]*
          Vpk[6][11][0])+(Vpk[3][11][1]*Vpk[6][11][1]))))+((.043*((Vpk[3][10][0]
          *c10)-(Vpk[3][10][1]*s10)))+((.1*((Vpk[3][9][0]*Vpk[6][9][0])+(
          Vpk[3][9][1]*Vpk[6][9][1])))+(.565701*Vpk[3][6][0]))));
        mm[3][6] = (.0475+(((.001*((c11*c11)+((Wpk[3][13][0]*Wpk[6][13][0])+(
          Wpk[3][13][1]*Wpk[6][13][1]))))+(.2058*((Vpk[3][13][2]*Vpk[6][13][2])+
          ((Vpk[3][13][0]*Vpk[6][13][0])+(Vpk[3][13][1]*Vpk[6][13][1])))))+(((
          .001*((c11*c11)+((Wpk[3][12][0]*Wpk[6][12][0])+(Wpk[3][12][1]*
          Wpk[6][12][1]))))+(.4*((Vpk[3][12][2]*Vpk[6][12][2])+((Vpk[3][12][0]*
          Vpk[6][12][0])+(Vpk[3][12][1]*Vpk[6][12][1])))))+temp[0])));
        mm[3][7] = (.1*((Vpk[3][9][0]*c9)-(Vpk[3][9][1]*s9)));
        mm[3][8] = (.1*((Vpk[3][9][0]*s9)+(Vpk[3][9][1]*c9)));
        mm[3][9] = .001;
        temp[0] = ((.4183*((.02377*(Vpk[3][11][2]*s11))+((Vpk[3][11][0]*
          Vpk[10][11][0])-(.02377*(Vpk[3][11][1]*c11)))))+((.001*((c11*c11)+((
          Wpk[3][12][0]*Wpk[10][12][0])+(Wpk[3][12][1]*Wpk[10][12][1]))))+(.4*((
          Vpk[3][12][2]*Vpk[10][12][2])+((Vpk[3][12][0]*Vpk[10][12][0])+(
          Vpk[3][12][1]*Vpk[10][12][1]))))));
        mm[3][10] = (.002+(((.001*((c11*c11)+((Wpk[3][13][0]*Wpk[10][13][0])+(
          Wpk[3][13][1]*Wpk[10][13][1]))))+(.2058*((Vpk[3][13][2]*Vpk[10][13][2]
          )+((Vpk[3][13][0]*Vpk[10][13][0])+(Vpk[3][13][1]*Vpk[10][13][1])))))+
          temp[0]));
        mm[3][11] = (((.001*((Wpk[3][13][0]*Wpk[11][13][0])+(Wpk[3][13][1]*
          Wpk[11][13][1])))+(.2058*((Vpk[3][13][2]*Vpk[11][13][2])+((
          Vpk[3][13][0]*Vpk[11][13][0])+(Vpk[3][13][1]*Vpk[11][13][1])))))+((
          .0062745*Vpk[3][11][2])+((.001*((Wpk[3][12][0]*c12)-(Wpk[3][12][1]*s12
          )))+(.4*(Vpk[3][12][2]*Vpk[11][12][2])))));
        mm[3][12] = (((.001*c11)+(.2058*((Vpk[3][13][0]*Vpk[12][13][0])+(
          Vpk[3][13][1]*Vpk[12][13][1]))))+((.001*c11)+(.4*((.025*Vpk[3][12][1])
          -(.015*Vpk[3][12][0])))));
        mm[3][13] = ((.001*c11)+(.2058*((.0346*Vpk[3][13][1])-(.006*
          Vpk[3][13][0]))));
        mm[3][14] = 0.;
        mm[3][15] = 0.;
        mm[3][16] = 0.;
        mm[3][17] = 0.;
        mm[3][18] = 0.;
        mm[3][19] = 0.;
        mm[3][20] = 0.;
        mm[3][21] = 0.;
        mm[3][22] = 0.;
        mm[3][23] = 0.;
        mm[3][24] = 0.;
        temp[0] = ((.4*((Vpk[4][11][2]*Vpk[4][11][2])+((Vpk[4][12][0]*
          Vpk[4][12][0])+(Vpk[4][12][1]*Vpk[4][12][1]))))+((.4183*((
          Vpk[4][11][2]*Vpk[4][11][2])+((Vpk[4][10][0]*Vpk[4][10][0])+(
          Vpk[4][11][1]*Vpk[4][11][1]))))+((.1*((Vpk[4][10][0]*Vpk[4][10][0])+(
          Vpk[4][10][1]*Vpk[4][10][1])))+(3.03+(.1*((Vpk[4][9][0]*Vpk[4][9][0])+
          (Vpk[4][9][1]*Vpk[4][9][1])))))));
        mm[4][4] = ((.2058*((Vpk[4][11][2]*Vpk[4][11][2])+((Vpk[4][13][0]*
          Vpk[4][13][0])+(Vpk[4][13][1]*Vpk[4][13][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[4][11][2]*Vpk[5][11][2])+((Vpk[4][12][0]*
          Vpk[5][12][0])+(Vpk[4][12][1]*Vpk[5][12][1]))))+((.1*(((Vpk[4][9][0]*
          Vpk[5][9][0])+(Vpk[4][9][1]*Vpk[5][9][1]))+((Vpk[4][10][0]*
          Vpk[5][10][0])+(Vpk[4][10][1]*Vpk[5][10][1]))))+(.4183*((Vpk[4][11][2]
          *Vpk[5][11][2])+((Vpk[4][10][0]*Vpk[5][10][0])+(Vpk[4][11][1]*
          Vpk[5][11][1]))))));
        mm[4][5] = ((.2058*((Vpk[4][11][2]*Vpk[5][11][2])+((Vpk[4][13][0]*
          Vpk[5][13][0])+(Vpk[4][13][1]*Vpk[5][13][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[4][11][2]*Vpk[6][12][2])+((Vpk[4][12][0]*
          Vpk[6][12][0])+(Vpk[4][12][1]*Vpk[6][12][1]))))+((.4183*((
          Vpk[4][11][2]*Vpk[6][11][2])+((Vpk[4][10][0]*Vpk[6][11][0])+(
          Vpk[4][11][1]*Vpk[6][11][1]))))+((.043*((Vpk[4][10][0]*c10)-(
          Vpk[4][10][1]*s10)))+((.1*((Vpk[4][9][0]*Vpk[6][9][0])+(Vpk[4][9][1]*
          Vpk[6][9][1])))+(.565701*c6)))));
        mm[4][6] = ((.2058*((Vpk[4][11][2]*Vpk[6][13][2])+((Vpk[4][13][0]*
          Vpk[6][13][0])+(Vpk[4][13][1]*Vpk[6][13][1]))))+temp[0]);
        mm[4][7] = (.1*((Vpk[4][9][0]*c9)-(Vpk[4][9][1]*s9)));
        mm[4][8] = (.1*((Vpk[4][9][0]*s9)+(Vpk[4][9][1]*c9)));
        mm[4][9] = 0.;
        mm[4][10] = ((.2058*((Vpk[4][11][2]*Vpk[10][13][2])+((Vpk[4][13][0]*
          Vpk[10][13][0])+(Vpk[4][13][1]*Vpk[10][13][1]))))+((.4*((Vpk[4][11][2]
          *Vpk[10][12][2])+((Vpk[4][12][0]*Vpk[10][12][0])+(Vpk[4][12][1]*
          Vpk[10][12][1]))))+(.4183*((.02377*(Vpk[4][11][2]*s11))+((
          Vpk[4][10][0]*Vpk[10][11][0])-(.02377*(Vpk[4][11][1]*c11)))))));
        mm[4][11] = ((.2058*((Vpk[4][11][2]*Vpk[11][13][2])+((Vpk[4][13][0]*
          Vpk[11][13][0])+(Vpk[4][13][1]*Vpk[11][13][1]))))+((.0062745*
          Vpk[4][11][2])+(.4*(Vpk[4][11][2]*Vpk[11][12][2]))));
        mm[4][12] = ((.2058*((Vpk[4][13][0]*Vpk[12][13][0])+(Vpk[4][13][1]*
          Vpk[12][13][1])))+(.4*((.025*Vpk[4][12][1])-(.015*Vpk[4][12][0]))));
        mm[4][13] = (.2058*((.0346*Vpk[4][13][1])-(.006*Vpk[4][13][0])));
        mm[4][14] = 0.;
        mm[4][15] = 0.;
        mm[4][16] = 0.;
        mm[4][17] = 0.;
        mm[4][18] = 0.;
        mm[4][19] = 0.;
        mm[4][20] = 0.;
        mm[4][21] = 0.;
        mm[4][22] = 0.;
        mm[4][23] = 0.;
        mm[4][24] = 0.;
        temp[0] = ((.4*((Vpk[5][11][2]*Vpk[5][11][2])+((Vpk[5][12][0]*
          Vpk[5][12][0])+(Vpk[5][12][1]*Vpk[5][12][1]))))+((.4183*((
          Vpk[5][11][2]*Vpk[5][11][2])+((Vpk[5][10][0]*Vpk[5][10][0])+(
          Vpk[5][11][1]*Vpk[5][11][1]))))+((.1*((Vpk[5][10][0]*Vpk[5][10][0])+(
          Vpk[5][10][1]*Vpk[5][10][1])))+(3.03+(.1*((Vpk[5][9][0]*Vpk[5][9][0])+
          (Vpk[5][9][1]*Vpk[5][9][1])))))));
        mm[5][5] = ((.2058*((Vpk[5][11][2]*Vpk[5][11][2])+((Vpk[5][13][0]*
          Vpk[5][13][0])+(Vpk[5][13][1]*Vpk[5][13][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[5][11][2]*Vpk[6][12][2])+((Vpk[5][12][0]*
          Vpk[6][12][0])+(Vpk[5][12][1]*Vpk[6][12][1]))))+((.4183*((
          Vpk[5][11][2]*Vpk[6][11][2])+((Vpk[5][10][0]*Vpk[6][11][0])+(
          Vpk[5][11][1]*Vpk[6][11][1]))))+((.043*((Vpk[5][10][0]*c10)-(
          Vpk[5][10][1]*s10)))+((.1*((Vpk[5][9][0]*Vpk[6][9][0])+(Vpk[5][9][1]*
          Vpk[6][9][1])))+(.565701*s6)))));
        mm[5][6] = ((.2058*((Vpk[5][11][2]*Vpk[6][13][2])+((Vpk[5][13][0]*
          Vpk[6][13][0])+(Vpk[5][13][1]*Vpk[6][13][1]))))+temp[0]);
        mm[5][7] = (.1*((Vpk[5][9][0]*c9)-(Vpk[5][9][1]*s9)));
        mm[5][8] = (.1*((Vpk[5][9][0]*s9)+(Vpk[5][9][1]*c9)));
        mm[5][9] = 0.;
        mm[5][10] = ((.2058*((Vpk[5][11][2]*Vpk[10][13][2])+((Vpk[5][13][0]*
          Vpk[10][13][0])+(Vpk[5][13][1]*Vpk[10][13][1]))))+((.4*((Vpk[5][11][2]
          *Vpk[10][12][2])+((Vpk[5][12][0]*Vpk[10][12][0])+(Vpk[5][12][1]*
          Vpk[10][12][1]))))+(.4183*((.02377*(Vpk[5][11][2]*s11))+((
          Vpk[5][10][0]*Vpk[10][11][0])-(.02377*(Vpk[5][11][1]*c11)))))));
        mm[5][11] = ((.2058*((Vpk[5][11][2]*Vpk[11][13][2])+((Vpk[5][13][0]*
          Vpk[11][13][0])+(Vpk[5][13][1]*Vpk[11][13][1]))))+((.0062745*
          Vpk[5][11][2])+(.4*(Vpk[5][11][2]*Vpk[11][12][2]))));
        mm[5][12] = ((.2058*((Vpk[5][13][0]*Vpk[12][13][0])+(Vpk[5][13][1]*
          Vpk[12][13][1])))+(.4*((.025*Vpk[5][12][1])-(.015*Vpk[5][12][0]))));
        mm[5][13] = (.2058*((.0346*Vpk[5][13][1])-(.006*Vpk[5][13][0])));
        mm[5][14] = 0.;
        mm[5][15] = 0.;
        mm[5][16] = 0.;
        mm[5][17] = 0.;
        mm[5][18] = 0.;
        mm[5][19] = 0.;
        mm[5][20] = 0.;
        mm[5][21] = 0.;
        mm[5][22] = 0.;
        mm[5][23] = 0.;
        mm[5][24] = 0.;
        temp[0] = (((.001*((c11*c11)+((Wpk[6][12][0]*Wpk[6][12][0])+(
          Wpk[6][12][1]*Wpk[6][12][1]))))+(.4*((Vpk[6][12][2]*Vpk[6][12][2])+((
          Vpk[6][12][0]*Vpk[6][12][0])+(Vpk[6][12][1]*Vpk[6][12][1])))))+((.1*((
          Vpk[6][9][0]*Vpk[6][9][0])+(Vpk[6][9][1]*Vpk[6][9][1])))+(.4183*((
          Vpk[6][11][2]*Vpk[6][11][2])+((Vpk[6][11][0]*Vpk[6][11][0])+(
          Vpk[6][11][1]*Vpk[6][11][1]))))));
        mm[6][6] = (.1716063767+(((.001*((c11*c11)+((Wpk[6][13][0]*Wpk[6][13][0]
          )+(Wpk[6][13][1]*Wpk[6][13][1]))))+(.2058*((Vpk[6][13][2]*
          Vpk[6][13][2])+((Vpk[6][13][0]*Vpk[6][13][0])+(Vpk[6][13][1]*
          Vpk[6][13][1])))))+temp[0]));
        mm[6][7] = (.1*((Vpk[6][9][0]*c9)-(Vpk[6][9][1]*s9)));
        mm[6][8] = (.1*((Vpk[6][9][0]*s9)+(Vpk[6][9][1]*c9)));
        mm[6][9] = .001;
        temp[0] = ((.4183*((.02377*(Vpk[6][11][2]*s11))+((Vpk[6][11][0]*
          Vpk[10][11][0])-(.02377*(Vpk[6][11][1]*c11)))))+((.001*((c11*c11)+((
          Wpk[6][12][0]*Wpk[10][12][0])+(Wpk[6][12][1]*Wpk[10][12][1]))))+(.4*((
          Vpk[6][12][2]*Vpk[10][12][2])+((Vpk[6][12][0]*Vpk[10][12][0])+(
          Vpk[6][12][1]*Vpk[10][12][1]))))));
        mm[6][10] = (.002+(((.001*((c11*c11)+((Wpk[6][13][0]*Wpk[10][13][0])+(
          Wpk[6][13][1]*Wpk[10][13][1]))))+(.2058*((Vpk[6][13][2]*Vpk[10][13][2]
          )+((Vpk[6][13][0]*Vpk[10][13][0])+(Vpk[6][13][1]*Vpk[10][13][1])))))+
          temp[0]));
        mm[6][11] = (((.001*((Wpk[6][13][0]*Wpk[11][13][0])+(Wpk[6][13][1]*
          Wpk[11][13][1])))+(.2058*((Vpk[6][13][2]*Vpk[11][13][2])+((
          Vpk[6][13][0]*Vpk[11][13][0])+(Vpk[6][13][1]*Vpk[11][13][1])))))+((
          .0062745*Vpk[6][11][2])+((.001*((Wpk[6][12][0]*c12)-(Wpk[6][12][1]*s12
          )))+(.4*(Vpk[6][12][2]*Vpk[11][12][2])))));
        mm[6][12] = (((.001*c11)+(.2058*((Vpk[6][13][0]*Vpk[12][13][0])+(
          Vpk[6][13][1]*Vpk[12][13][1]))))+((.001*c11)+(.4*((.025*Vpk[6][12][1])
          -(.015*Vpk[6][12][0])))));
        mm[6][13] = ((.001*c11)+(.2058*((.0346*Vpk[6][13][1])-(.006*
          Vpk[6][13][0]))));
        mm[6][14] = 0.;
        mm[6][15] = 0.;
        mm[6][16] = 0.;
        mm[6][17] = 0.;
        mm[6][18] = 0.;
        mm[6][19] = 0.;
        mm[6][20] = 0.;
        mm[6][21] = 0.;
        mm[6][22] = 0.;
        mm[6][23] = 0.;
        mm[6][24] = 0.;
        mm[7][7] = .1;
        mm[7][8] = 0.;
        mm[7][9] = 0.;
        mm[7][10] = 0.;
        mm[7][11] = 0.;
        mm[7][12] = 0.;
        mm[7][13] = 0.;
        mm[7][14] = 0.;
        mm[7][15] = 0.;
        mm[7][16] = 0.;
        mm[7][17] = 0.;
        mm[7][18] = 0.;
        mm[7][19] = 0.;
        mm[7][20] = 0.;
        mm[7][21] = 0.;
        mm[7][22] = 0.;
        mm[7][23] = 0.;
        mm[7][24] = 0.;
        mm[8][8] = .1;
        mm[8][9] = 0.;
        mm[8][10] = 0.;
        mm[8][11] = 0.;
        mm[8][12] = 0.;
        mm[8][13] = 0.;
        mm[8][14] = 0.;
        mm[8][15] = 0.;
        mm[8][16] = 0.;
        mm[8][17] = 0.;
        mm[8][18] = 0.;
        mm[8][19] = 0.;
        mm[8][20] = 0.;
        mm[8][21] = 0.;
        mm[8][22] = 0.;
        mm[8][23] = 0.;
        mm[8][24] = 0.;
        mm[9][9] = .001;
        mm[9][10] = 0.;
        mm[9][11] = 0.;
        mm[9][12] = 0.;
        mm[9][13] = 0.;
        mm[9][14] = 0.;
        mm[9][15] = 0.;
        mm[9][16] = 0.;
        mm[9][17] = 0.;
        mm[9][18] = 0.;
        mm[9][19] = 0.;
        mm[9][20] = 0.;
        mm[9][21] = 0.;
        mm[9][22] = 0.;
        mm[9][23] = 0.;
        mm[9][24] = 0.;
        temp[0] = ((.4183*((.0005650129*(s11*s11))+((.0005650129*(c11*c11))+(
          Vpk[10][11][0]*Vpk[10][11][0]))))+((.001*((c11*c11)+((Wpk[10][12][0]*
          Wpk[10][12][0])+(Wpk[10][12][1]*Wpk[10][12][1]))))+(.4*((
          Vpk[10][12][2]*Vpk[10][12][2])+((Vpk[10][12][0]*Vpk[10][12][0])+(
          Vpk[10][12][1]*Vpk[10][12][1]))))));
        mm[10][10] = (.002+(((.001*((c11*c11)+((Wpk[10][13][0]*Wpk[10][13][0])+(
          Wpk[10][13][1]*Wpk[10][13][1]))))+(.2058*((Vpk[10][13][2]*
          Vpk[10][13][2])+((Vpk[10][13][0]*Vpk[10][13][0])+(Vpk[10][13][1]*
          Vpk[10][13][1])))))+temp[0]));
        mm[10][11] = (((.000149144865*s11)+((.001*((Wpk[10][12][0]*c12)-(
          Wpk[10][12][1]*s12)))+(.4*(Vpk[10][12][2]*Vpk[11][12][2]))))+((.001*((
          Wpk[10][13][0]*Wpk[11][13][0])+(Wpk[10][13][1]*Wpk[11][13][1])))+(
          .2058*((Vpk[10][13][2]*Vpk[11][13][2])+((Vpk[10][13][0]*Vpk[11][13][0]
          )+(Vpk[10][13][1]*Vpk[11][13][1]))))));
        mm[10][12] = (((.001*c11)+(.2058*((Vpk[10][13][0]*Vpk[12][13][0])+(
          Vpk[10][13][1]*Vpk[12][13][1]))))+((.001*c11)+(.4*((.025*
          Vpk[10][12][1])-(.015*Vpk[10][12][0])))));
        mm[10][13] = ((.001*c11)+(.2058*((.0346*Vpk[10][13][1])-(.006*
          Vpk[10][13][0]))));
        mm[10][14] = 0.;
        mm[10][15] = 0.;
        mm[10][16] = 0.;
        mm[10][17] = 0.;
        mm[10][18] = 0.;
        mm[10][19] = 0.;
        mm[10][20] = 0.;
        mm[10][21] = 0.;
        mm[10][22] = 0.;
        mm[10][23] = 0.;
        mm[10][24] = 0.;
        mm[11][11] = (.0020941175+((.4*(Vpk[11][12][2]*Vpk[11][12][2]))+((.001*(
          (Wpk[11][13][0]*Wpk[11][13][0])+(Wpk[11][13][1]*Wpk[11][13][1])))+(
          .2058*((Vpk[11][13][2]*Vpk[11][13][2])+((Vpk[11][13][0]*Vpk[11][13][0]
          )+(Vpk[11][13][1]*Vpk[11][13][1])))))));
        mm[11][12] = (.2058*((Vpk[11][13][0]*Vpk[12][13][0])+(Vpk[11][13][1]*
          Vpk[12][13][1])));
        mm[11][13] = (.2058*((.0346*Vpk[11][13][1])-(.006*Vpk[11][13][0])));
        mm[11][14] = 0.;
        mm[11][15] = 0.;
        mm[11][16] = 0.;
        mm[11][17] = 0.;
        mm[11][18] = 0.;
        mm[11][19] = 0.;
        mm[11][20] = 0.;
        mm[11][21] = 0.;
        mm[11][22] = 0.;
        mm[11][23] = 0.;
        mm[11][24] = 0.;
        mm[12][12] = (.00234+(.2058*((Vpk[12][13][0]*Vpk[12][13][0])+(
          Vpk[12][13][1]*Vpk[12][13][1]))));
        mm[12][13] = (.001+(.2058*((.0346*Vpk[12][13][1])-(.006*Vpk[12][13][0]))
          ));
        mm[12][14] = 0.;
        mm[12][15] = 0.;
        mm[12][16] = 0.;
        mm[12][17] = 0.;
        mm[12][18] = 0.;
        mm[12][19] = 0.;
        mm[12][20] = 0.;
        mm[12][21] = 0.;
        mm[12][22] = 0.;
        mm[12][23] = 0.;
        mm[12][24] = 0.;
        mm[13][13] = .001253784328;
        mm[13][14] = 0.;
        mm[13][15] = 0.;
        mm[13][16] = 0.;
        mm[13][17] = 0.;
        mm[13][18] = 0.;
        mm[13][19] = 0.;
        mm[13][20] = 0.;
        mm[13][21] = 0.;
        mm[13][22] = 0.;
        mm[13][23] = 0.;
        mm[13][24] = 0.;
        temp[0] = ((.4183*((Vpk[14][22][2]*Vpk[14][22][2])+((Vpk[14][22][0]*
          Vpk[14][22][0])+(Vpk[14][22][1]*Vpk[14][22][1]))))+((.1*((
          Vpk[14][21][0]*Vpk[14][21][0])+(Vpk[14][21][1]*Vpk[14][21][1])))+((.1*
          ((Vpk[14][20][0]*Vpk[14][20][0])+(Vpk[14][20][1]*Vpk[14][20][1])))+(
          3.03*((Vpk[14][17][0]*Vpk[14][17][0])+(Vpk[14][17][1]*Vpk[14][17][1]))
          ))));
        mm[14][14] = (.408764+(((.001*((c22*c22)+((Wpk[14][24][0]*Wpk[14][24][0]
          )+(Wpk[14][24][1]*Wpk[14][24][1]))))+(.2058*((Vpk[14][24][2]*
          Vpk[14][24][2])+((Vpk[14][24][0]*Vpk[14][24][0])+(Vpk[14][24][1]*
          Vpk[14][24][1])))))+(((.001*((c22*c22)+((Wpk[14][23][0]*Wpk[14][23][0]
          )+(Wpk[14][23][1]*Wpk[14][23][1]))))+(.4*((Vpk[14][23][2]*
          Vpk[14][23][2])+((Vpk[14][23][0]*Vpk[14][23][0])+(Vpk[14][23][1]*
          Vpk[14][23][1])))))+temp[0])));
        temp[0] = ((.4*((Vpk[14][23][2]*Vpk[15][22][2])+((Vpk[14][23][0]*
          Vpk[15][23][0])+(Vpk[14][23][1]*Vpk[15][23][1]))))+((.4183*((
          Vpk[14][22][2]*Vpk[15][22][2])+((Vpk[14][22][0]*Vpk[15][21][0])+(
          Vpk[14][22][1]*Vpk[15][22][1]))))+((.1*((Vpk[14][21][0]*Vpk[15][21][0]
          )+(Vpk[14][21][1]*Vpk[15][21][1])))+((.1*((Vpk[14][20][0]*
          Vpk[15][20][0])+(Vpk[14][20][1]*Vpk[15][20][1])))+(3.03*((
          Vpk[14][17][0]*c17)-(Vpk[14][17][1]*s17)))))));
        mm[14][15] = ((.2058*((Vpk[14][24][2]*Vpk[15][22][2])+((Vpk[14][24][0]*
          Vpk[15][24][0])+(Vpk[14][24][1]*Vpk[15][24][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[14][23][2]*Vpk[16][22][2])+((Vpk[14][23][0]*
          Vpk[16][23][0])+(Vpk[14][23][1]*Vpk[16][23][1]))))+((.4183*((
          Vpk[14][22][2]*Vpk[16][22][2])+((Vpk[14][22][0]*Vpk[16][21][0])+(
          Vpk[14][22][1]*Vpk[16][22][1]))))+((.1*((Vpk[14][21][0]*Vpk[16][21][0]
          )+(Vpk[14][21][1]*Vpk[16][21][1])))+((.1*((Vpk[14][20][0]*
          Vpk[16][20][0])+(Vpk[14][20][1]*Vpk[16][20][1])))+(3.03*((
          Vpk[14][17][0]*s17)+(Vpk[14][17][1]*c17)))))));
        mm[14][16] = ((.2058*((Vpk[14][24][2]*Vpk[16][22][2])+((Vpk[14][24][0]*
          Vpk[16][24][0])+(Vpk[14][24][1]*Vpk[16][24][1]))))+temp[0]);
        temp[0] = ((.4183*((Vpk[14][22][2]*Vpk[17][22][2])+((Vpk[14][22][0]*
          Vpk[17][22][0])+(Vpk[14][22][1]*Vpk[17][22][1]))))+((.043*((
          Vpk[14][21][0]*c21)-(Vpk[14][21][1]*s21)))+((.1*((Vpk[14][20][0]*
          Vpk[17][20][0])+(Vpk[14][20][1]*Vpk[17][20][1])))+(.565701*
          Vpk[14][17][0]))));
        mm[14][17] = (.0475+(((.001*((c22*c22)+((Wpk[14][24][0]*Wpk[17][24][0])+
          (Wpk[14][24][1]*Wpk[17][24][1]))))+(.2058*((Vpk[14][24][2]*
          Vpk[17][24][2])+((Vpk[14][24][0]*Vpk[17][24][0])+(Vpk[14][24][1]*
          Vpk[17][24][1])))))+(((.001*((c22*c22)+((Wpk[14][23][0]*Wpk[17][23][0]
          )+(Wpk[14][23][1]*Wpk[17][23][1]))))+(.4*((Vpk[14][23][2]*
          Vpk[17][23][2])+((Vpk[14][23][0]*Vpk[17][23][0])+(Vpk[14][23][1]*
          Vpk[17][23][1])))))+temp[0])));
        mm[14][18] = (.1*((Vpk[14][20][0]*c20)-(Vpk[14][20][1]*s20)));
        mm[14][19] = (.1*((Vpk[14][20][0]*s20)+(Vpk[14][20][1]*c20)));
        mm[14][20] = .001;
        temp[0] = ((.4183*((.02377*(Vpk[14][22][2]*s22))+((Vpk[14][22][0]*
          Vpk[21][22][0])-(.02377*(Vpk[14][22][1]*c22)))))+((.001*((c22*c22)+((
          Wpk[14][23][0]*Wpk[21][23][0])+(Wpk[14][23][1]*Wpk[21][23][1]))))+(.4*
          ((Vpk[14][23][2]*Vpk[21][23][2])+((Vpk[14][23][0]*Vpk[21][23][0])+(
          Vpk[14][23][1]*Vpk[21][23][1]))))));
        mm[14][21] = (.002+(((.001*((c22*c22)+((Wpk[14][24][0]*Wpk[21][24][0])+(
          Wpk[14][24][1]*Wpk[21][24][1]))))+(.2058*((Vpk[14][24][2]*
          Vpk[21][24][2])+((Vpk[14][24][0]*Vpk[21][24][0])+(Vpk[14][24][1]*
          Vpk[21][24][1])))))+temp[0]));
        mm[14][22] = (((.001*((Wpk[14][24][0]*Wpk[22][24][0])+(Wpk[14][24][1]*
          Wpk[22][24][1])))+(.2058*((Vpk[14][24][2]*Vpk[22][24][2])+((
          Vpk[14][24][0]*Vpk[22][24][0])+(Vpk[14][24][1]*Vpk[22][24][1])))))+((
          .0062745*Vpk[14][22][2])+((.001*((Wpk[14][23][0]*c23)-(Wpk[14][23][1]*
          s23)))+(.4*(Vpk[14][23][2]*Vpk[22][23][2])))));
        mm[14][23] = (((.001*c22)+(.2058*((Vpk[14][24][0]*Vpk[23][24][0])+(
          Vpk[14][24][1]*Vpk[23][24][1]))))+((.001*c22)+(.4*((.025*
          Vpk[14][23][1])-(.015*Vpk[14][23][0])))));
        mm[14][24] = ((.001*c22)+(.2058*((.0346*Vpk[14][24][1])-(.006*
          Vpk[14][24][0]))));
        temp[0] = ((.4*((Vpk[15][22][2]*Vpk[15][22][2])+((Vpk[15][23][0]*
          Vpk[15][23][0])+(Vpk[15][23][1]*Vpk[15][23][1]))))+((.4183*((
          Vpk[15][22][2]*Vpk[15][22][2])+((Vpk[15][21][0]*Vpk[15][21][0])+(
          Vpk[15][22][1]*Vpk[15][22][1]))))+((.1*((Vpk[15][21][0]*Vpk[15][21][0]
          )+(Vpk[15][21][1]*Vpk[15][21][1])))+(3.03+(.1*((Vpk[15][20][0]*
          Vpk[15][20][0])+(Vpk[15][20][1]*Vpk[15][20][1])))))));
        mm[15][15] = ((.2058*((Vpk[15][22][2]*Vpk[15][22][2])+((Vpk[15][24][0]*
          Vpk[15][24][0])+(Vpk[15][24][1]*Vpk[15][24][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[15][22][2]*Vpk[16][22][2])+((Vpk[15][23][0]*
          Vpk[16][23][0])+(Vpk[15][23][1]*Vpk[16][23][1]))))+((.1*(((
          Vpk[15][20][0]*Vpk[16][20][0])+(Vpk[15][20][1]*Vpk[16][20][1]))+((
          Vpk[15][21][0]*Vpk[16][21][0])+(Vpk[15][21][1]*Vpk[16][21][1]))))+(
          .4183*((Vpk[15][22][2]*Vpk[16][22][2])+((Vpk[15][21][0]*Vpk[16][21][0]
          )+(Vpk[15][22][1]*Vpk[16][22][1]))))));
        mm[15][16] = ((.2058*((Vpk[15][22][2]*Vpk[16][22][2])+((Vpk[15][24][0]*
          Vpk[16][24][0])+(Vpk[15][24][1]*Vpk[16][24][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[15][22][2]*Vpk[17][23][2])+((Vpk[15][23][0]*
          Vpk[17][23][0])+(Vpk[15][23][1]*Vpk[17][23][1]))))+((.4183*((
          Vpk[15][22][2]*Vpk[17][22][2])+((Vpk[15][21][0]*Vpk[17][22][0])+(
          Vpk[15][22][1]*Vpk[17][22][1]))))+((.043*((Vpk[15][21][0]*c21)-(
          Vpk[15][21][1]*s21)))+((.1*((Vpk[15][20][0]*Vpk[17][20][0])+(
          Vpk[15][20][1]*Vpk[17][20][1])))+(.565701*c17)))));
        mm[15][17] = ((.2058*((Vpk[15][22][2]*Vpk[17][24][2])+((Vpk[15][24][0]*
          Vpk[17][24][0])+(Vpk[15][24][1]*Vpk[17][24][1]))))+temp[0]);
        mm[15][18] = (.1*((Vpk[15][20][0]*c20)-(Vpk[15][20][1]*s20)));
        mm[15][19] = (.1*((Vpk[15][20][0]*s20)+(Vpk[15][20][1]*c20)));
        mm[15][20] = 0.;
        mm[15][21] = ((.2058*((Vpk[15][22][2]*Vpk[21][24][2])+((Vpk[15][24][0]*
          Vpk[21][24][0])+(Vpk[15][24][1]*Vpk[21][24][1]))))+((.4*((
          Vpk[15][22][2]*Vpk[21][23][2])+((Vpk[15][23][0]*Vpk[21][23][0])+(
          Vpk[15][23][1]*Vpk[21][23][1]))))+(.4183*((.02377*(Vpk[15][22][2]*s22)
          )+((Vpk[15][21][0]*Vpk[21][22][0])-(.02377*(Vpk[15][22][1]*c22)))))));
        mm[15][22] = ((.2058*((Vpk[15][22][2]*Vpk[22][24][2])+((Vpk[15][24][0]*
          Vpk[22][24][0])+(Vpk[15][24][1]*Vpk[22][24][1]))))+((.0062745*
          Vpk[15][22][2])+(.4*(Vpk[15][22][2]*Vpk[22][23][2]))));
        mm[15][23] = ((.2058*((Vpk[15][24][0]*Vpk[23][24][0])+(Vpk[15][24][1]*
          Vpk[23][24][1])))+(.4*((.025*Vpk[15][23][1])-(.015*Vpk[15][23][0]))));
        mm[15][24] = (.2058*((.0346*Vpk[15][24][1])-(.006*Vpk[15][24][0])));
        temp[0] = ((.4*((Vpk[16][22][2]*Vpk[16][22][2])+((Vpk[16][23][0]*
          Vpk[16][23][0])+(Vpk[16][23][1]*Vpk[16][23][1]))))+((.4183*((
          Vpk[16][22][2]*Vpk[16][22][2])+((Vpk[16][21][0]*Vpk[16][21][0])+(
          Vpk[16][22][1]*Vpk[16][22][1]))))+((.1*((Vpk[16][21][0]*Vpk[16][21][0]
          )+(Vpk[16][21][1]*Vpk[16][21][1])))+(3.03+(.1*((Vpk[16][20][0]*
          Vpk[16][20][0])+(Vpk[16][20][1]*Vpk[16][20][1])))))));
        mm[16][16] = ((.2058*((Vpk[16][22][2]*Vpk[16][22][2])+((Vpk[16][24][0]*
          Vpk[16][24][0])+(Vpk[16][24][1]*Vpk[16][24][1]))))+temp[0]);
        temp[0] = ((.4*((Vpk[16][22][2]*Vpk[17][23][2])+((Vpk[16][23][0]*
          Vpk[17][23][0])+(Vpk[16][23][1]*Vpk[17][23][1]))))+((.4183*((
          Vpk[16][22][2]*Vpk[17][22][2])+((Vpk[16][21][0]*Vpk[17][22][0])+(
          Vpk[16][22][1]*Vpk[17][22][1]))))+((.043*((Vpk[16][21][0]*c21)-(
          Vpk[16][21][1]*s21)))+((.1*((Vpk[16][20][0]*Vpk[17][20][0])+(
          Vpk[16][20][1]*Vpk[17][20][1])))+(.565701*s17)))));
        mm[16][17] = ((.2058*((Vpk[16][22][2]*Vpk[17][24][2])+((Vpk[16][24][0]*
          Vpk[17][24][0])+(Vpk[16][24][1]*Vpk[17][24][1]))))+temp[0]);
        mm[16][18] = (.1*((Vpk[16][20][0]*c20)-(Vpk[16][20][1]*s20)));
        mm[16][19] = (.1*((Vpk[16][20][0]*s20)+(Vpk[16][20][1]*c20)));
        mm[16][20] = 0.;
        mm[16][21] = ((.2058*((Vpk[16][22][2]*Vpk[21][24][2])+((Vpk[16][24][0]*
          Vpk[21][24][0])+(Vpk[16][24][1]*Vpk[21][24][1]))))+((.4*((
          Vpk[16][22][2]*Vpk[21][23][2])+((Vpk[16][23][0]*Vpk[21][23][0])+(
          Vpk[16][23][1]*Vpk[21][23][1]))))+(.4183*((.02377*(Vpk[16][22][2]*s22)
          )+((Vpk[16][21][0]*Vpk[21][22][0])-(.02377*(Vpk[16][22][1]*c22)))))));
        mm[16][22] = ((.2058*((Vpk[16][22][2]*Vpk[22][24][2])+((Vpk[16][24][0]*
          Vpk[22][24][0])+(Vpk[16][24][1]*Vpk[22][24][1]))))+((.0062745*
          Vpk[16][22][2])+(.4*(Vpk[16][22][2]*Vpk[22][23][2]))));
        mm[16][23] = ((.2058*((Vpk[16][24][0]*Vpk[23][24][0])+(Vpk[16][24][1]*
          Vpk[23][24][1])))+(.4*((.025*Vpk[16][23][1])-(.015*Vpk[16][23][0]))));
        mm[16][24] = (.2058*((.0346*Vpk[16][24][1])-(.006*Vpk[16][24][0])));
        temp[0] = (((.001*((c22*c22)+((Wpk[17][23][0]*Wpk[17][23][0])+(
          Wpk[17][23][1]*Wpk[17][23][1]))))+(.4*((Vpk[17][23][2]*Vpk[17][23][2])
          +((Vpk[17][23][0]*Vpk[17][23][0])+(Vpk[17][23][1]*Vpk[17][23][1])))))+
          ((.1*((Vpk[17][20][0]*Vpk[17][20][0])+(Vpk[17][20][1]*Vpk[17][20][1]))
          )+(.4183*((Vpk[17][22][2]*Vpk[17][22][2])+((Vpk[17][22][0]*
          Vpk[17][22][0])+(Vpk[17][22][1]*Vpk[17][22][1]))))));
        mm[17][17] = (.1716063767+(((.001*((c22*c22)+((Wpk[17][24][0]*
          Wpk[17][24][0])+(Wpk[17][24][1]*Wpk[17][24][1]))))+(.2058*((
          Vpk[17][24][2]*Vpk[17][24][2])+((Vpk[17][24][0]*Vpk[17][24][0])+(
          Vpk[17][24][1]*Vpk[17][24][1])))))+temp[0]));
        mm[17][18] = (.1*((Vpk[17][20][0]*c20)-(Vpk[17][20][1]*s20)));
        mm[17][19] = (.1*((Vpk[17][20][0]*s20)+(Vpk[17][20][1]*c20)));
        mm[17][20] = .001;
        temp[0] = ((.4183*((.02377*(Vpk[17][22][2]*s22))+((Vpk[17][22][0]*
          Vpk[21][22][0])-(.02377*(Vpk[17][22][1]*c22)))))+((.001*((c22*c22)+((
          Wpk[17][23][0]*Wpk[21][23][0])+(Wpk[17][23][1]*Wpk[21][23][1]))))+(.4*
          ((Vpk[17][23][2]*Vpk[21][23][2])+((Vpk[17][23][0]*Vpk[21][23][0])+(
          Vpk[17][23][1]*Vpk[21][23][1]))))));
        mm[17][21] = (.002+(((.001*((c22*c22)+((Wpk[17][24][0]*Wpk[21][24][0])+(
          Wpk[17][24][1]*Wpk[21][24][1]))))+(.2058*((Vpk[17][24][2]*
          Vpk[21][24][2])+((Vpk[17][24][0]*Vpk[21][24][0])+(Vpk[17][24][1]*
          Vpk[21][24][1])))))+temp[0]));
        mm[17][22] = (((.001*((Wpk[17][24][0]*Wpk[22][24][0])+(Wpk[17][24][1]*
          Wpk[22][24][1])))+(.2058*((Vpk[17][24][2]*Vpk[22][24][2])+((
          Vpk[17][24][0]*Vpk[22][24][0])+(Vpk[17][24][1]*Vpk[22][24][1])))))+((
          .0062745*Vpk[17][22][2])+((.001*((Wpk[17][23][0]*c23)-(Wpk[17][23][1]*
          s23)))+(.4*(Vpk[17][23][2]*Vpk[22][23][2])))));
        mm[17][23] = (((.001*c22)+(.2058*((Vpk[17][24][0]*Vpk[23][24][0])+(
          Vpk[17][24][1]*Vpk[23][24][1]))))+((.001*c22)+(.4*((.025*
          Vpk[17][23][1])-(.015*Vpk[17][23][0])))));
        mm[17][24] = ((.001*c22)+(.2058*((.0346*Vpk[17][24][1])-(.006*
          Vpk[17][24][0]))));
        mm[18][18] = .1;
        mm[18][19] = 0.;
        mm[18][20] = 0.;
        mm[18][21] = 0.;
        mm[18][22] = 0.;
        mm[18][23] = 0.;
        mm[18][24] = 0.;
        mm[19][19] = .1;
        mm[19][20] = 0.;
        mm[19][21] = 0.;
        mm[19][22] = 0.;
        mm[19][23] = 0.;
        mm[19][24] = 0.;
        mm[20][20] = .001;
        mm[20][21] = 0.;
        mm[20][22] = 0.;
        mm[20][23] = 0.;
        mm[20][24] = 0.;
        temp[0] = ((.4183*((.0005650129*(s22*s22))+((.0005650129*(c22*c22))+(
          Vpk[21][22][0]*Vpk[21][22][0]))))+((.001*((c22*c22)+((Wpk[21][23][0]*
          Wpk[21][23][0])+(Wpk[21][23][1]*Wpk[21][23][1]))))+(.4*((
          Vpk[21][23][2]*Vpk[21][23][2])+((Vpk[21][23][0]*Vpk[21][23][0])+(
          Vpk[21][23][1]*Vpk[21][23][1]))))));
        mm[21][21] = (.002+(((.001*((c22*c22)+((Wpk[21][24][0]*Wpk[21][24][0])+(
          Wpk[21][24][1]*Wpk[21][24][1]))))+(.2058*((Vpk[21][24][2]*
          Vpk[21][24][2])+((Vpk[21][24][0]*Vpk[21][24][0])+(Vpk[21][24][1]*
          Vpk[21][24][1])))))+temp[0]));
        mm[21][22] = (((.000149144865*s22)+((.001*((Wpk[21][23][0]*c23)-(
          Wpk[21][23][1]*s23)))+(.4*(Vpk[21][23][2]*Vpk[22][23][2]))))+((.001*((
          Wpk[21][24][0]*Wpk[22][24][0])+(Wpk[21][24][1]*Wpk[22][24][1])))+(
          .2058*((Vpk[21][24][2]*Vpk[22][24][2])+((Vpk[21][24][0]*Vpk[22][24][0]
          )+(Vpk[21][24][1]*Vpk[22][24][1]))))));
        mm[21][23] = (((.001*c22)+(.2058*((Vpk[21][24][0]*Vpk[23][24][0])+(
          Vpk[21][24][1]*Vpk[23][24][1]))))+((.001*c22)+(.4*((.025*
          Vpk[21][23][1])-(.015*Vpk[21][23][0])))));
        mm[21][24] = ((.001*c22)+(.2058*((.0346*Vpk[21][24][1])-(.006*
          Vpk[21][24][0]))));
        mm[22][22] = (.0020941175+((.4*(Vpk[22][23][2]*Vpk[22][23][2]))+((.001*(
          (Wpk[22][24][0]*Wpk[22][24][0])+(Wpk[22][24][1]*Wpk[22][24][1])))+(
          .2058*((Vpk[22][24][2]*Vpk[22][24][2])+((Vpk[22][24][0]*Vpk[22][24][0]
          )+(Vpk[22][24][1]*Vpk[22][24][1])))))));
        mm[22][23] = (.2058*((Vpk[22][24][0]*Vpk[23][24][0])+(Vpk[22][24][1]*
          Vpk[23][24][1])));
        mm[22][24] = (.2058*((.0346*Vpk[22][24][1])-(.006*Vpk[22][24][0])));
        mm[23][23] = (.00234+(.2058*((Vpk[23][24][0]*Vpk[23][24][0])+(
          Vpk[23][24][1]*Vpk[23][24][1]))));
        mm[23][24] = (.001+(.2058*((.0346*Vpk[23][24][1])-(.006*Vpk[23][24][0]))
          ));
        mm[24][24] = .001253784328;
/*
Check for singular mass matrix
*/
        for (i = 0; i < 25; i++) {
            if (mm[i][i] < 1e-13) {
                sdseterr(routine,47);
            }
        }
        sderror(&dumroutine,&errnum);
        if (errnum == 0) {
            mmflg = 1;
        }
    }
/*
 Used 0.08 seconds CPU time,
 0 additional bytes of memory.
 Equations contain 1301 adds/subtracts/negates
                   2006 multiplies
                      0 divides
                    395 assignments
*/
}

void sddommldu(int routine)
{
    int i;
    int dumroutine,errnum;

    if (mmlduflg == 0) {
        sddomm(routine);
/*
Numerically decompose the mass matrix
*/
        sdldudcomp(25,25,mmap,1e-13,workss,works,mm,mlo,mdi);
/*
Check for singular mass matrix
*/
        for (i = 0; i < 25; i++) {
            if (mdi[i] <= 1e-13) {
                sdseterr(routine,47);
            }
        }
        sderror(&dumroutine,&errnum);
        if (errnum == 0) {
            mmlduflg = 1;
        }
    }
}

void sdlhs(int routine)
{
/* Compute all remaining state- and force-dependent quantities
*/

    roustate = 2;
    sddommldu(routine);
    sddofs0();
}

void sdmfrc(double imult[12])
{
/*
Calculate forces due to constraint multipliers.

*/
    int i,j;
    double umult[10];

/*
Initialize all multiplier forces to zero.
*/
    for (i = 0; i <= 14; i++) {
        for (j = 0; j <= 2; j++) {
            mfk[i][j] = 0.;
            mtk[i][j] = 0.;
        }
    }
    for (i = 0; i <= 24; i++) {
        mtau[i] = 0.;
    }
/*
Compute user-generated multiplier forces
*/
    umult[0] = imult[2];
    umult[1] = imult[3];
    umult[2] = imult[4];
    umult[3] = imult[5];
    umult[4] = imult[6];
    umult[5] = imult[7];
    umult[6] = imult[8];
    umult[7] = imult[9];
    umult[8] = imult[10];
    umult[9] = imult[11];
    mfrcflg = 1;
    sduconsfrc(curtim,q,u,umult);
    mfrcflg = 0;
    mtau[11] = (imult[0]+mtau[11]);
    mtau[22] = (imult[1]+mtau[22]);
/*
 Used 0.00 seconds CPU time,
 0 additional bytes of memory.
 Equations contain    2 adds/subtracts/negates
                      0 multiplies
                      0 divides
                    127 assignments
*/
}

void sdequivht(double tau[25])
{
/* Compute tree hinge torques to match effect of applied loads
*/
    double fstareq[25][3],tstareq[25][3];

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(56,23);
        return;
    }
/*
Compute fstareq (forces)
*/
    fstareq[2][0] = ((501.119815*s2)-ufk[0][0]);
    fstareq[2][1] = ((501.119815*c2)-ufk[0][1]);
    fstareq[3][0] = -(ufk[1][0]+(7.76*gk[3][0]));
    fstareq[3][1] = -(ufk[1][1]+(7.76*gk[3][1]));
    fstareq[6][0] = -(ufk[2][0]+(3.03*gk[6][0]));
    fstareq[6][1] = -(ufk[2][1]+(3.03*gk[6][1]));
    fstareq[9][0] = -(ufk[3][0]+(.1*gk[9][0]));
    fstareq[9][1] = -(ufk[3][1]+(.1*gk[9][1]));
    fstareq[10][0] = -(ufk[4][0]+(.1*gk[10][0]));
    fstareq[10][1] = -(ufk[4][1]+(.1*gk[10][1]));
    fstareq[11][0] = -(ufk[5][0]+(.4183*gk[10][0]));
    fstareq[11][1] = -(ufk[5][1]+(.4183*gk[11][1]));
    fstareq[11][2] = -(ufk[5][2]+(.4183*gk[11][2]));
    fstareq[12][0] = -(ufk[6][0]+(.4*gk[12][0]));
    fstareq[12][1] = -(ufk[6][1]+(.4*gk[12][1]));
    fstareq[12][2] = -(ufk[6][2]+(.4*gk[11][2]));
    fstareq[13][0] = -(ufk[7][0]+(.2058*gk[13][0]));
    fstareq[13][1] = -(ufk[7][1]+(.2058*gk[13][1]));
    fstareq[13][2] = -(ufk[7][2]+(.2058*gk[11][2]));
    fstareq[14][0] = -(ufk[8][0]+(7.76*gk[14][0]));
    fstareq[14][1] = -(ufk[8][1]+(7.76*gk[14][1]));
    fstareq[17][0] = -(ufk[9][0]+(3.03*gk[17][0]));
    fstareq[17][1] = -(ufk[9][1]+(3.03*gk[17][1]));
    fstareq[20][0] = -(ufk[10][0]+(.1*gk[20][0]));
    fstareq[20][1] = -(ufk[10][1]+(.1*gk[20][1]));
    fstareq[21][0] = -(ufk[11][0]+(.1*gk[21][0]));
    fstareq[21][1] = -(ufk[11][1]+(.1*gk[21][1]));
    fstareq[22][0] = -(ufk[12][0]+(.4183*gk[21][0]));
    fstareq[22][1] = -(ufk[12][1]+(.4183*gk[22][1]));
    fstareq[22][2] = -(ufk[12][2]+(.4183*gk[22][2]));
    fstareq[23][0] = -(ufk[13][0]+(.4*gk[23][0]));
    fstareq[23][1] = -(ufk[13][1]+(.4*gk[23][1]));
    fstareq[23][2] = -(ufk[13][2]+(.4*gk[22][2]));
    fstareq[24][0] = -(ufk[14][0]+(.2058*gk[24][0]));
    fstareq[24][1] = -(ufk[14][1]+(.2058*gk[24][1]));
    fstareq[24][2] = -(ufk[14][2]+(.2058*gk[22][2]));
/*
Compute tstareq (torques)
*/
/*
Compute taus (RHS ignoring constraints and inertial forces)
*/
    sddovpk();
    temp[0] = (((fstareq[11][2]*Vpk[0][11][2])+((fstareq[11][0]*Vpk[0][10][0])+(
      fstareq[11][1]*Vpk[0][11][1])))+(((fstareq[10][0]*Vpk[0][10][0])+(
      fstareq[10][1]*Vpk[0][10][1]))+(((fstareq[9][0]*Vpk[0][9][0])+(
      fstareq[9][1]*Vpk[0][9][1]))+(((fstareq[6][0]*Vpk[0][6][0])+(fstareq[6][1]
      *Vpk[0][6][1]))+(((fstareq[2][0]*c2)-(fstareq[2][1]*s2))+((fstareq[3][0]*
      Vpk[0][3][0])+(fstareq[3][1]*Vpk[0][3][1])))))));
    temp[1] = (((fstareq[21][0]*Vpk[0][21][0])+(fstareq[21][1]*Vpk[0][21][1]))+(
      ((fstareq[20][0]*Vpk[0][20][0])+(fstareq[20][1]*Vpk[0][20][1]))+(((
      fstareq[17][0]*Vpk[0][17][0])+(fstareq[17][1]*Vpk[0][17][1]))+(((
      fstareq[14][0]*Vpk[0][14][0])+(fstareq[14][1]*Vpk[0][14][1]))+(((
      fstareq[13][2]*Vpk[0][11][2])+((fstareq[13][0]*Vpk[0][13][0])+(
      fstareq[13][1]*Vpk[0][13][1])))+(((fstareq[12][2]*Vpk[0][11][2])+((
      fstareq[12][0]*Vpk[0][12][0])+(fstareq[12][1]*Vpk[0][12][1])))+temp[0]))))
      ));
    tau[0] = (utau[0]-(((fstareq[24][2]*Vpk[0][22][2])+((fstareq[24][0]*
      Vpk[0][24][0])+(fstareq[24][1]*Vpk[0][24][1])))+(((fstareq[23][2]*
      Vpk[0][22][2])+((fstareq[23][0]*Vpk[0][23][0])+(fstareq[23][1]*
      Vpk[0][23][1])))+(((fstareq[22][2]*Vpk[0][22][2])+((fstareq[22][0]*
      Vpk[0][21][0])+(fstareq[22][1]*Vpk[0][22][1])))+temp[1]))));
    temp[0] = (((fstareq[11][2]*Vpk[1][11][2])+((fstareq[11][0]*Vpk[1][10][0])+(
      fstareq[11][1]*Vpk[1][11][1])))+(((fstareq[10][0]*Vpk[1][10][0])+(
      fstareq[10][1]*Vpk[1][10][1]))+(((fstareq[9][0]*Vpk[1][9][0])+(
      fstareq[9][1]*Vpk[1][9][1]))+(((fstareq[6][0]*Vpk[1][6][0])+(fstareq[6][1]
      *Vpk[1][6][1]))+(((fstareq[2][0]*s2)+(fstareq[2][1]*c2))+((fstareq[3][0]*
      Vpk[1][3][0])+(fstareq[3][1]*Vpk[1][3][1])))))));
    temp[1] = (((fstareq[21][0]*Vpk[1][21][0])+(fstareq[21][1]*Vpk[1][21][1]))+(
      ((fstareq[20][0]*Vpk[1][20][0])+(fstareq[20][1]*Vpk[1][20][1]))+(((
      fstareq[17][0]*Vpk[1][17][0])+(fstareq[17][1]*Vpk[1][17][1]))+(((
      fstareq[14][0]*Vpk[1][14][0])+(fstareq[14][1]*Vpk[1][14][1]))+(((
      fstareq[13][2]*Vpk[1][11][2])+((fstareq[13][0]*Vpk[1][13][0])+(
      fstareq[13][1]*Vpk[1][13][1])))+(((fstareq[12][2]*Vpk[1][11][2])+((
      fstareq[12][0]*Vpk[1][12][0])+(fstareq[12][1]*Vpk[1][12][1])))+temp[0]))))
      ));
    tau[1] = (utau[1]-(((fstareq[24][2]*Vpk[1][22][2])+((fstareq[24][0]*
      Vpk[1][24][0])+(fstareq[24][1]*Vpk[1][24][1])))+(((fstareq[23][2]*
      Vpk[1][22][2])+((fstareq[23][0]*Vpk[1][23][0])+(fstareq[23][1]*
      Vpk[1][23][1])))+(((fstareq[22][2]*Vpk[1][22][2])+((fstareq[22][0]*
      Vpk[1][21][0])+(fstareq[22][1]*Vpk[1][22][1])))+temp[1]))));
    temp[0] = ((((fstareq[10][0]*Vpk[2][10][0])+(fstareq[10][1]*Vpk[2][10][1]))-
      utk[4][2])+((((fstareq[9][0]*Vpk[2][9][0])+(fstareq[9][1]*Vpk[2][9][1]))-
      utk[3][2])+((((fstareq[6][0]*Vpk[2][6][0])+(fstareq[6][1]*Vpk[2][6][1]))-
      utk[2][2])+((((fstareq[3][0]*Vpk[2][3][0])+(fstareq[3][1]*Vpk[2][3][1]))-
      utk[1][2])-(utk[0][2]+((.0707*fstareq[2][1])+(.2639*fstareq[2][0])))))));
    temp[1] = ((((fstareq[12][2]*Vpk[2][12][2])+((fstareq[12][0]*Vpk[2][12][0])+
      (fstareq[12][1]*Vpk[2][12][1])))-((utk[6][2]*c11)+((utk[6][0]*
      Wpk[2][12][0])+(utk[6][1]*Wpk[2][12][1]))))+((((fstareq[11][2]*
      Vpk[2][11][2])+((fstareq[11][0]*Vpk[2][11][0])+(fstareq[11][1]*
      Vpk[2][11][1])))-((utk[5][1]*s11)+(utk[5][2]*c11)))+temp[0]));
    temp[2] = ((((fstareq[20][0]*Vpk[2][20][0])+(fstareq[20][1]*Vpk[2][20][1]))-
      utk[10][2])+((((fstareq[17][0]*Vpk[2][17][0])+(fstareq[17][1]*
      Vpk[2][17][1]))-utk[9][2])+((((fstareq[14][0]*Vpk[2][14][0])+(
      fstareq[14][1]*Vpk[2][14][1]))-utk[8][2])+((((fstareq[13][2]*Vpk[2][13][2]
      )+((fstareq[13][0]*Vpk[2][13][0])+(fstareq[13][1]*Vpk[2][13][1])))-((
      utk[7][2]*c11)+((utk[7][0]*Wpk[2][13][0])+(utk[7][1]*Wpk[2][13][1]))))+
      temp[1]))));
    temp[3] = ((((fstareq[23][2]*Vpk[2][23][2])+((fstareq[23][0]*Vpk[2][23][0])+
      (fstareq[23][1]*Vpk[2][23][1])))-((utk[13][2]*c22)+((utk[13][0]*
      Wpk[2][23][0])+(utk[13][1]*Wpk[2][23][1]))))+((((fstareq[22][2]*
      Vpk[2][22][2])+((fstareq[22][0]*Vpk[2][22][0])+(fstareq[22][1]*
      Vpk[2][22][1])))-((utk[12][1]*s22)+(utk[12][2]*c22)))+((((fstareq[21][0]*
      Vpk[2][21][0])+(fstareq[21][1]*Vpk[2][21][1]))-utk[11][2])+temp[2])));
    tau[2] = (utau[2]-((((fstareq[24][2]*Vpk[2][24][2])+((fstareq[24][0]*
      Vpk[2][24][0])+(fstareq[24][1]*Vpk[2][24][1])))-((utk[14][2]*c22)+((
      utk[14][0]*Wpk[2][24][0])+(utk[14][1]*Wpk[2][24][1]))))+temp[3]));
    temp[0] = ((((fstareq[11][2]*Vpk[3][11][2])+((fstareq[11][0]*Vpk[3][11][0])+
      (fstareq[11][1]*Vpk[3][11][1])))-((utk[5][1]*s11)+(utk[5][2]*c11)))+((((
      fstareq[10][0]*Vpk[3][10][0])+(fstareq[10][1]*Vpk[3][10][1]))-utk[4][2])+(
      (((.17*fstareq[3][0])-utk[1][2])+(((fstareq[6][0]*Vpk[3][6][0])+(
      fstareq[6][1]*Vpk[3][6][1]))-utk[2][2]))+(((fstareq[9][0]*Vpk[3][9][0])+(
      fstareq[9][1]*Vpk[3][9][1]))-utk[3][2]))));
    tau[3] = (utau[3]-((((fstareq[13][2]*Vpk[3][13][2])+((fstareq[13][0]*
      Vpk[3][13][0])+(fstareq[13][1]*Vpk[3][13][1])))-((utk[7][2]*c11)+((
      utk[7][0]*Wpk[3][13][0])+(utk[7][1]*Wpk[3][13][1]))))+((((fstareq[12][2]*
      Vpk[3][12][2])+((fstareq[12][0]*Vpk[3][12][0])+(fstareq[12][1]*
      Vpk[3][12][1])))-((utk[6][2]*c11)+((utk[6][0]*Wpk[3][12][0])+(utk[6][1]*
      Wpk[3][12][1]))))+temp[0])));
    temp[0] = (((fstareq[13][2]*Vpk[4][11][2])+((fstareq[13][0]*Vpk[4][13][0])+(
      fstareq[13][1]*Vpk[4][13][1])))+(((fstareq[12][2]*Vpk[4][11][2])+((
      fstareq[12][0]*Vpk[4][12][0])+(fstareq[12][1]*Vpk[4][12][1])))+(((
      fstareq[11][2]*Vpk[4][11][2])+((fstareq[11][0]*Vpk[4][10][0])+(
      fstareq[11][1]*Vpk[4][11][1])))+(((fstareq[10][0]*Vpk[4][10][0])+(
      fstareq[10][1]*Vpk[4][10][1]))+(((fstareq[6][0]*c6)-(fstareq[6][1]*s6))+((
      fstareq[9][0]*Vpk[4][9][0])+(fstareq[9][1]*Vpk[4][9][1])))))));
    tau[4] = (utau[4]-temp[0]);
    temp[0] = (((fstareq[13][2]*Vpk[5][11][2])+((fstareq[13][0]*Vpk[5][13][0])+(
      fstareq[13][1]*Vpk[5][13][1])))+(((fstareq[12][2]*Vpk[5][11][2])+((
      fstareq[12][0]*Vpk[5][12][0])+(fstareq[12][1]*Vpk[5][12][1])))+(((
      fstareq[11][2]*Vpk[5][11][2])+((fstareq[11][0]*Vpk[5][10][0])+(
      fstareq[11][1]*Vpk[5][11][1])))+(((fstareq[10][0]*Vpk[5][10][0])+(
      fstareq[10][1]*Vpk[5][10][1]))+(((fstareq[6][0]*s6)+(fstareq[6][1]*c6))+((
      fstareq[9][0]*Vpk[5][9][0])+(fstareq[9][1]*Vpk[5][9][1])))))));
    tau[5] = (utau[5]-temp[0]);
    temp[0] = ((((.43*((fstareq[10][0]*c10)-(fstareq[10][1]*s10)))-utk[4][2])+((
      (.1867*fstareq[6][0])-utk[2][2])+(((fstareq[9][0]*Vpk[6][9][0])+(
      fstareq[9][1]*Vpk[6][9][1]))-utk[3][2])))+(((fstareq[11][2]*Vpk[6][11][2])
      +((fstareq[11][0]*Vpk[6][11][0])+(fstareq[11][1]*Vpk[6][11][1])))-((
      utk[5][1]*s11)+(utk[5][2]*c11))));
    tau[6] = (utau[6]-((((fstareq[13][2]*Vpk[6][13][2])+((fstareq[13][0]*
      Vpk[6][13][0])+(fstareq[13][1]*Vpk[6][13][1])))-((utk[7][2]*c11)+((
      utk[7][0]*Wpk[6][13][0])+(utk[7][1]*Wpk[6][13][1]))))+((((fstareq[12][2]*
      Vpk[6][12][2])+((fstareq[12][0]*Vpk[6][12][0])+(fstareq[12][1]*
      Vpk[6][12][1])))-((utk[6][2]*c11)+((utk[6][0]*Wpk[6][12][0])+(utk[6][1]*
      Wpk[6][12][1]))))+temp[0])));
    tau[7] = (utau[7]-((fstareq[9][0]*c9)-(fstareq[9][1]*s9)));
    tau[8] = (utau[8]-((fstareq[9][0]*s9)+(fstareq[9][1]*c9)));
    tau[9] = (utau[9]+utk[3][2]);
    temp[0] = ((((fstareq[12][2]*Vpk[10][12][2])+((fstareq[12][0]*Vpk[10][12][0]
      )+(fstareq[12][1]*Vpk[10][12][1])))-((utk[6][2]*c11)+((utk[6][0]*
      Wpk[10][12][0])+(utk[6][1]*Wpk[10][12][1]))))+((((.02377*(fstareq[11][2]*
      s11))+((fstareq[11][0]*Vpk[10][11][0])-(.02377*(fstareq[11][1]*c11))))-((
      utk[5][1]*s11)+(utk[5][2]*c11)))-utk[4][2]));
    tau[10] = (utau[10]-((((fstareq[13][2]*Vpk[10][13][2])+((fstareq[13][0]*
      Vpk[10][13][0])+(fstareq[13][1]*Vpk[10][13][1])))-((utk[7][2]*c11)+((
      utk[7][0]*Wpk[10][13][0])+(utk[7][1]*Wpk[10][13][1]))))+temp[0]));
    tau[11] = (utau[11]-((((.015*fstareq[11][2])-utk[5][0])+((fstareq[12][2]*
      Vpk[11][12][2])+((utk[6][1]*s12)-(utk[6][0]*c12))))+(((fstareq[13][2]*
      Vpk[11][13][2])+((fstareq[13][0]*Vpk[11][13][0])+(fstareq[13][1]*
      Vpk[11][13][1])))-((utk[7][0]*Wpk[11][13][0])+(utk[7][1]*Wpk[11][13][1])))
      ));
    tau[12] = (utau[12]-((((.025*fstareq[12][1])-(.015*fstareq[12][0]))-
      utk[6][2])+(((fstareq[13][0]*Vpk[12][13][0])+(fstareq[13][1]*
      Vpk[12][13][1]))-utk[7][2])));
    tau[13] = (utau[13]-(((.0346*fstareq[13][1])-(.006*fstareq[13][0]))-
      utk[7][2]));
    temp[0] = ((((fstareq[22][2]*Vpk[14][22][2])+((fstareq[22][0]*Vpk[14][22][0]
      )+(fstareq[22][1]*Vpk[14][22][1])))-((utk[12][1]*s22)+(utk[12][2]*c22)))+(
      (((fstareq[21][0]*Vpk[14][21][0])+(fstareq[21][1]*Vpk[14][21][1]))-
      utk[11][2])+((((.17*fstareq[14][0])-utk[8][2])+(((fstareq[17][0]*
      Vpk[14][17][0])+(fstareq[17][1]*Vpk[14][17][1]))-utk[9][2]))+(((
      fstareq[20][0]*Vpk[14][20][0])+(fstareq[20][1]*Vpk[14][20][1]))-utk[10][2]
      ))));
    tau[14] = (utau[14]-((((fstareq[24][2]*Vpk[14][24][2])+((fstareq[24][0]*
      Vpk[14][24][0])+(fstareq[24][1]*Vpk[14][24][1])))-((utk[14][2]*c22)+((
      utk[14][0]*Wpk[14][24][0])+(utk[14][1]*Wpk[14][24][1]))))+((((
      fstareq[23][2]*Vpk[14][23][2])+((fstareq[23][0]*Vpk[14][23][0])+(
      fstareq[23][1]*Vpk[14][23][1])))-((utk[13][2]*c22)+((utk[13][0]*
      Wpk[14][23][0])+(utk[13][1]*Wpk[14][23][1]))))+temp[0])));
    temp[0] = (((fstareq[24][2]*Vpk[15][22][2])+((fstareq[24][0]*Vpk[15][24][0])
      +(fstareq[24][1]*Vpk[15][24][1])))+(((fstareq[23][2]*Vpk[15][22][2])+((
      fstareq[23][0]*Vpk[15][23][0])+(fstareq[23][1]*Vpk[15][23][1])))+(((
      fstareq[22][2]*Vpk[15][22][2])+((fstareq[22][0]*Vpk[15][21][0])+(
      fstareq[22][1]*Vpk[15][22][1])))+(((fstareq[21][0]*Vpk[15][21][0])+(
      fstareq[21][1]*Vpk[15][21][1]))+(((fstareq[17][0]*c17)-(fstareq[17][1]*s17
      ))+((fstareq[20][0]*Vpk[15][20][0])+(fstareq[20][1]*Vpk[15][20][1])))))));
    tau[15] = (utau[15]-temp[0]);
    temp[0] = (((fstareq[24][2]*Vpk[16][22][2])+((fstareq[24][0]*Vpk[16][24][0])
      +(fstareq[24][1]*Vpk[16][24][1])))+(((fstareq[23][2]*Vpk[16][22][2])+((
      fstareq[23][0]*Vpk[16][23][0])+(fstareq[23][1]*Vpk[16][23][1])))+(((
      fstareq[22][2]*Vpk[16][22][2])+((fstareq[22][0]*Vpk[16][21][0])+(
      fstareq[22][1]*Vpk[16][22][1])))+(((fstareq[21][0]*Vpk[16][21][0])+(
      fstareq[21][1]*Vpk[16][21][1]))+(((fstareq[17][0]*s17)+(fstareq[17][1]*c17
      ))+((fstareq[20][0]*Vpk[16][20][0])+(fstareq[20][1]*Vpk[16][20][1])))))));
    tau[16] = (utau[16]-temp[0]);
    temp[0] = ((((.43*((fstareq[21][0]*c21)-(fstareq[21][1]*s21)))-utk[11][2])+(
      ((.1867*fstareq[17][0])-utk[9][2])+(((fstareq[20][0]*Vpk[17][20][0])+(
      fstareq[20][1]*Vpk[17][20][1]))-utk[10][2])))+(((fstareq[22][2]*
      Vpk[17][22][2])+((fstareq[22][0]*Vpk[17][22][0])+(fstareq[22][1]*
      Vpk[17][22][1])))-((utk[12][1]*s22)+(utk[12][2]*c22))));
    tau[17] = (utau[17]-((((fstareq[24][2]*Vpk[17][24][2])+((fstareq[24][0]*
      Vpk[17][24][0])+(fstareq[24][1]*Vpk[17][24][1])))-((utk[14][2]*c22)+((
      utk[14][0]*Wpk[17][24][0])+(utk[14][1]*Wpk[17][24][1]))))+((((
      fstareq[23][2]*Vpk[17][23][2])+((fstareq[23][0]*Vpk[17][23][0])+(
      fstareq[23][1]*Vpk[17][23][1])))-((utk[13][2]*c22)+((utk[13][0]*
      Wpk[17][23][0])+(utk[13][1]*Wpk[17][23][1]))))+temp[0])));
    tau[18] = (utau[18]-((fstareq[20][0]*c20)-(fstareq[20][1]*s20)));
    tau[19] = (utau[19]-((fstareq[20][0]*s20)+(fstareq[20][1]*c20)));
    tau[20] = (utau[20]+utk[10][2]);
    temp[0] = ((((fstareq[23][2]*Vpk[21][23][2])+((fstareq[23][0]*Vpk[21][23][0]
      )+(fstareq[23][1]*Vpk[21][23][1])))-((utk[13][2]*c22)+((utk[13][0]*
      Wpk[21][23][0])+(utk[13][1]*Wpk[21][23][1]))))+((((.02377*(fstareq[22][2]*
      s22))+((fstareq[22][0]*Vpk[21][22][0])-(.02377*(fstareq[22][1]*c22))))-((
      utk[12][1]*s22)+(utk[12][2]*c22)))-utk[11][2]));
    tau[21] = (utau[21]-((((fstareq[24][2]*Vpk[21][24][2])+((fstareq[24][0]*
      Vpk[21][24][0])+(fstareq[24][1]*Vpk[21][24][1])))-((utk[14][2]*c22)+((
      utk[14][0]*Wpk[21][24][0])+(utk[14][1]*Wpk[21][24][1]))))+temp[0]));
    tau[22] = (utau[22]-((((.015*fstareq[22][2])-utk[12][0])+((fstareq[23][2]*
      Vpk[22][23][2])+((utk[13][1]*s23)-(utk[13][0]*c23))))+(((fstareq[24][2]*
      Vpk[22][24][2])+((fstareq[24][0]*Vpk[22][24][0])+(fstareq[24][1]*
      Vpk[22][24][1])))-((utk[14][0]*Wpk[22][24][0])+(utk[14][1]*Wpk[22][24][1])
      ))));
    tau[23] = (utau[23]-((((.025*fstareq[23][1])-(.015*fstareq[23][0]))-
      utk[13][2])+(((fstareq[24][0]*Vpk[23][24][0])+(fstareq[24][1]*
      Vpk[23][24][1]))-utk[14][2])));
    tau[24] = (utau[24]-(((.0346*fstareq[24][1])-(.006*fstareq[24][0]))-
      utk[14][2]));
/*
Op counts below do not include called subroutines
*/
/*
 Used 0.02 seconds CPU time,
 0 additional bytes of memory.
 Equations contain  453 adds/subtracts/negates
                    390 multiplies
                      0 divides
                     79 assignments
*/
}

void sdfs0(void)
{

/*
Compute Fs (ignoring multiplier forces)
*/
    fs[0] = fs0[0];
    fs[1] = fs0[1];
    fs[2] = fs0[2];
    fs[3] = fs0[3];
    fs[4] = fs0[4];
    fs[5] = fs0[5];
    fs[6] = fs0[6];
    fs[7] = fs0[7];
    fs[8] = fs0[8];
    fs[9] = fs0[9];
    fs[10] = fs0[10];
    fs[11] = fs0[11];
    fs[12] = fs0[12];
    fs[13] = fs0[13];
    fs[14] = fs0[14];
    fs[15] = fs0[15];
    fs[16] = fs0[16];
    fs[17] = fs0[17];
    fs[18] = fs0[18];
    fs[19] = fs0[19];
    fs[20] = fs0[20];
    fs[21] = fs0[21];
    fs[22] = fs0[22];
    fs[23] = fs0[23];
    fs[24] = fs0[24];
/*
 Used 0.00 seconds CPU time,
 0 additional bytes of memory.
 Equations contain    0 adds/subtracts/negates
                      0 multiplies
                      0 divides
                     25 assignments
*/
}

void sdfsmult(void)
{

/*
Compute Fs (multiplier-generated forces only)
*/
    sddovpk();
    temp[0] = (((((((mfk[0][1]*s2)-(mfk[0][0]*c2))-((mfk[1][0]*Vpk[0][3][0])+(
      mfk[1][1]*Vpk[0][3][1])))-((mfk[2][0]*Vpk[0][6][0])+(mfk[2][1]*
      Vpk[0][6][1])))-((mfk[3][0]*Vpk[0][9][0])+(mfk[3][1]*Vpk[0][9][1])))-((
      mfk[4][0]*Vpk[0][10][0])+(mfk[4][1]*Vpk[0][10][1])))-((mfk[5][2]*
      Vpk[0][11][2])+((mfk[5][0]*Vpk[0][10][0])+(mfk[5][1]*Vpk[0][11][1]))));
    temp[1] = ((((((temp[0]-((mfk[6][2]*Vpk[0][11][2])+((mfk[6][0]*Vpk[0][12][0]
      )+(mfk[6][1]*Vpk[0][12][1]))))-((mfk[7][2]*Vpk[0][11][2])+((mfk[7][0]*
      Vpk[0][13][0])+(mfk[7][1]*Vpk[0][13][1]))))-((mfk[8][0]*Vpk[0][14][0])+(
      mfk[8][1]*Vpk[0][14][1])))-((mfk[9][0]*Vpk[0][17][0])+(mfk[9][1]*
      Vpk[0][17][1])))-((mfk[10][0]*Vpk[0][20][0])+(mfk[10][1]*Vpk[0][20][1])))-
      ((mfk[11][0]*Vpk[0][21][0])+(mfk[11][1]*Vpk[0][21][1])));
    fs[0] = (mtau[0]-(((temp[1]-((mfk[12][2]*Vpk[0][22][2])+((mfk[12][0]*
      Vpk[0][21][0])+(mfk[12][1]*Vpk[0][22][1]))))-((mfk[13][2]*Vpk[0][22][2])+(
      (mfk[13][0]*Vpk[0][23][0])+(mfk[13][1]*Vpk[0][23][1]))))-((mfk[14][2]*
      Vpk[0][22][2])+((mfk[14][0]*Vpk[0][24][0])+(mfk[14][1]*Vpk[0][24][1])))));
    temp[0] = (((mfk[5][2]*Vpk[1][11][2])+((mfk[5][0]*Vpk[1][10][0])+(mfk[5][1]*
      Vpk[1][11][1])))+(((mfk[4][0]*Vpk[1][10][0])+(mfk[4][1]*Vpk[1][10][1]))+((
      (mfk[3][0]*Vpk[1][9][0])+(mfk[3][1]*Vpk[1][9][1]))+(((mfk[2][0]*
      Vpk[1][6][0])+(mfk[2][1]*Vpk[1][6][1]))+(((mfk[0][0]*s2)+(mfk[0][1]*c2))+(
      (mfk[1][0]*Vpk[1][3][0])+(mfk[1][1]*Vpk[1][3][1])))))));
    temp[1] = (((mfk[11][0]*Vpk[1][21][0])+(mfk[11][1]*Vpk[1][21][1]))+(((
      mfk[10][0]*Vpk[1][20][0])+(mfk[10][1]*Vpk[1][20][1]))+(((mfk[9][0]*
      Vpk[1][17][0])+(mfk[9][1]*Vpk[1][17][1]))+(((mfk[8][0]*Vpk[1][14][0])+(
      mfk[8][1]*Vpk[1][14][1]))+(((mfk[7][2]*Vpk[1][11][2])+((mfk[7][0]*
      Vpk[1][13][0])+(mfk[7][1]*Vpk[1][13][1])))+(((mfk[6][2]*Vpk[1][11][2])+((
      mfk[6][0]*Vpk[1][12][0])+(mfk[6][1]*Vpk[1][12][1])))+temp[0]))))));
    fs[1] = (mtau[1]+(((mfk[14][2]*Vpk[1][22][2])+((mfk[14][0]*Vpk[1][24][0])+(
      mfk[14][1]*Vpk[1][24][1])))+(((mfk[13][2]*Vpk[1][22][2])+((mfk[13][0]*
      Vpk[1][23][0])+(mfk[13][1]*Vpk[1][23][1])))+(((mfk[12][2]*Vpk[1][22][2])+(
      (mfk[12][0]*Vpk[1][21][0])+(mfk[12][1]*Vpk[1][22][1])))+temp[1]))));
    temp[0] = (((((((.0707*mfk[0][1])+(.2639*mfk[0][0]))-mtk[0][2])-(mtk[1][2]+(
      (mfk[1][0]*Vpk[2][3][0])+(mfk[1][1]*Vpk[2][3][1]))))-(mtk[2][2]+((
      mfk[2][0]*Vpk[2][6][0])+(mfk[2][1]*Vpk[2][6][1]))))-(mtk[3][2]+((mfk[3][0]
      *Vpk[2][9][0])+(mfk[3][1]*Vpk[2][9][1]))))-(mtk[4][2]+((mfk[4][0]*
      Vpk[2][10][0])+(mfk[4][1]*Vpk[2][10][1]))));
    temp[1] = ((temp[0]-(((mfk[5][2]*Vpk[2][11][2])+((mfk[5][0]*Vpk[2][11][0])+(
      mfk[5][1]*Vpk[2][11][1])))+((mtk[5][1]*s11)+(mtk[5][2]*c11))))-(((
      mfk[6][2]*Vpk[2][12][2])+((mfk[6][0]*Vpk[2][12][0])+(mfk[6][1]*
      Vpk[2][12][1])))+((mtk[6][2]*c11)+((mtk[6][0]*Wpk[2][12][0])+(mtk[6][1]*
      Wpk[2][12][1])))));
    temp[2] = ((((temp[1]-(((mfk[7][2]*Vpk[2][13][2])+((mfk[7][0]*Vpk[2][13][0])
      +(mfk[7][1]*Vpk[2][13][1])))+((mtk[7][2]*c11)+((mtk[7][0]*Wpk[2][13][0])+(
      mtk[7][1]*Wpk[2][13][1])))))-(mtk[8][2]+((mfk[8][0]*Vpk[2][14][0])+(
      mfk[8][1]*Vpk[2][14][1]))))-(mtk[9][2]+((mfk[9][0]*Vpk[2][17][0])+(
      mfk[9][1]*Vpk[2][17][1]))))-(mtk[10][2]+((mfk[10][0]*Vpk[2][20][0])+(
      mfk[10][1]*Vpk[2][20][1]))));
    temp[3] = (((temp[2]-(mtk[11][2]+((mfk[11][0]*Vpk[2][21][0])+(mfk[11][1]*
      Vpk[2][21][1]))))-(((mfk[12][2]*Vpk[2][22][2])+((mfk[12][0]*Vpk[2][22][0])
      +(mfk[12][1]*Vpk[2][22][1])))+((mtk[12][1]*s22)+(mtk[12][2]*c22))))-(((
      mfk[13][2]*Vpk[2][23][2])+((mfk[13][0]*Vpk[2][23][0])+(mfk[13][1]*
      Vpk[2][23][1])))+((mtk[13][2]*c22)+((mtk[13][0]*Wpk[2][23][0])+(mtk[13][1]
      *Wpk[2][23][1])))));
    fs[2] = (mtau[2]-(temp[3]-(((mfk[14][2]*Vpk[2][24][2])+((mfk[14][0]*
      Vpk[2][24][0])+(mfk[14][1]*Vpk[2][24][1])))+((mtk[14][2]*c22)+((mtk[14][0]
      *Wpk[2][24][0])+(mtk[14][1]*Wpk[2][24][1]))))));
    temp[0] = (((mtk[4][2]+((mfk[4][0]*Vpk[3][10][0])+(mfk[4][1]*Vpk[3][10][1]))
      )+((mtk[3][2]+((mfk[3][0]*Vpk[3][9][0])+(mfk[3][1]*Vpk[3][9][1])))+((
      mtk[1][2]+(.17*mfk[1][0]))+(mtk[2][2]+((mfk[2][0]*Vpk[3][6][0])+(mfk[2][1]
      *Vpk[3][6][1]))))))+(((mfk[5][2]*Vpk[3][11][2])+((mfk[5][0]*Vpk[3][11][0])
      +(mfk[5][1]*Vpk[3][11][1])))+((mtk[5][1]*s11)+(mtk[5][2]*c11))));
    fs[3] = (mtau[3]+((((mfk[7][2]*Vpk[3][13][2])+((mfk[7][0]*Vpk[3][13][0])+(
      mfk[7][1]*Vpk[3][13][1])))+((mtk[7][2]*c11)+((mtk[7][0]*Wpk[3][13][0])+(
      mtk[7][1]*Wpk[3][13][1]))))+((((mfk[6][2]*Vpk[3][12][2])+((mfk[6][0]*
      Vpk[3][12][0])+(mfk[6][1]*Vpk[3][12][1])))+((mtk[6][2]*c11)+((mtk[6][0]*
      Wpk[3][12][0])+(mtk[6][1]*Wpk[3][12][1]))))+temp[0])));
    temp[0] = (((((((mfk[2][1]*s6)-(mfk[2][0]*c6))-((mfk[3][0]*Vpk[4][9][0])+(
      mfk[3][1]*Vpk[4][9][1])))-((mfk[4][0]*Vpk[4][10][0])+(mfk[4][1]*
      Vpk[4][10][1])))-((mfk[5][2]*Vpk[4][11][2])+((mfk[5][0]*Vpk[4][10][0])+(
      mfk[5][1]*Vpk[4][11][1]))))-((mfk[6][2]*Vpk[4][11][2])+((mfk[6][0]*
      Vpk[4][12][0])+(mfk[6][1]*Vpk[4][12][1]))))-((mfk[7][2]*Vpk[4][11][2])+((
      mfk[7][0]*Vpk[4][13][0])+(mfk[7][1]*Vpk[4][13][1]))));
    fs[4] = (mtau[4]-temp[0]);
    temp[0] = (((mfk[7][2]*Vpk[5][11][2])+((mfk[7][0]*Vpk[5][13][0])+(mfk[7][1]*
      Vpk[5][13][1])))+(((mfk[6][2]*Vpk[5][11][2])+((mfk[6][0]*Vpk[5][12][0])+(
      mfk[6][1]*Vpk[5][12][1])))+(((mfk[5][2]*Vpk[5][11][2])+((mfk[5][0]*
      Vpk[5][10][0])+(mfk[5][1]*Vpk[5][11][1])))+(((mfk[4][0]*Vpk[5][10][0])+(
      mfk[4][1]*Vpk[5][10][1]))+(((mfk[2][0]*s6)+(mfk[2][1]*c6))+((mfk[3][0]*
      Vpk[5][9][0])+(mfk[3][1]*Vpk[5][9][1])))))));
    fs[5] = (mtau[5]+temp[0]);
    temp[0] = ((((.43*((mfk[4][1]*s10)-(mfk[4][0]*c10)))-mtk[4][2])-((mtk[2][2]+
      (.1867*mfk[2][0]))+(mtk[3][2]+((mfk[3][0]*Vpk[6][9][0])+(mfk[3][1]*
      Vpk[6][9][1])))))-(((mfk[5][2]*Vpk[6][11][2])+((mfk[5][0]*Vpk[6][11][0])+(
      mfk[5][1]*Vpk[6][11][1])))+((mtk[5][1]*s11)+(mtk[5][2]*c11))));
    fs[6] = (mtau[6]-((temp[0]-(((mfk[6][2]*Vpk[6][12][2])+((mfk[6][0]*
      Vpk[6][12][0])+(mfk[6][1]*Vpk[6][12][1])))+((mtk[6][2]*c11)+((mtk[6][0]*
      Wpk[6][12][0])+(mtk[6][1]*Wpk[6][12][1])))))-(((mfk[7][2]*Vpk[6][13][2])+(
      (mfk[7][0]*Vpk[6][13][0])+(mfk[7][1]*Vpk[6][13][1])))+((mtk[7][2]*c11)+((
      mtk[7][0]*Wpk[6][13][0])+(mtk[7][1]*Wpk[6][13][1]))))));
    fs[7] = (mtau[7]-((mfk[3][1]*s9)-(mfk[3][0]*c9)));
    fs[8] = (mtau[8]+((mfk[3][0]*s9)+(mfk[3][1]*c9)));
    fs[9] = (mtau[9]+mtk[3][2]);
    temp[0] = ((((((.02377*(mfk[5][1]*c11))-(mfk[5][0]*Vpk[10][11][0]))-(.02377*
      (mfk[5][2]*s11)))-((mtk[5][1]*s11)+(mtk[5][2]*c11)))-mtk[4][2])-(((
      mfk[6][2]*Vpk[10][12][2])+((mfk[6][0]*Vpk[10][12][0])+(mfk[6][1]*
      Vpk[10][12][1])))+((mtk[6][2]*c11)+((mtk[6][0]*Wpk[10][12][0])+(mtk[6][1]*
      Wpk[10][12][1])))));
    fs[10] = (mtau[10]-(temp[0]-(((mfk[7][2]*Vpk[10][13][2])+((mfk[7][0]*
      Vpk[10][13][0])+(mfk[7][1]*Vpk[10][13][1])))+((mtk[7][2]*c11)+((mtk[7][0]*
      Wpk[10][13][0])+(mtk[7][1]*Wpk[10][13][1]))))));
    fs[11] = (mtau[11]-(((((mtk[6][1]*s12)-(mtk[6][0]*c12))-(mfk[6][2]*
      Vpk[11][12][2]))-(mtk[5][0]+(.015*mfk[5][2])))-(((mfk[7][2]*Vpk[11][13][2]
      )+((mfk[7][0]*Vpk[11][13][0])+(mfk[7][1]*Vpk[11][13][1])))+((mtk[7][0]*
      Wpk[11][13][0])+(mtk[7][1]*Wpk[11][13][1])))));
    fs[12] = (mtau[12]-((((.015*mfk[6][0])-(.025*mfk[6][1]))-mtk[6][2])-(
      mtk[7][2]+((mfk[7][0]*Vpk[12][13][0])+(mfk[7][1]*Vpk[12][13][1])))));
    fs[13] = (mtau[13]-(((.006*mfk[7][0])-(.0346*mfk[7][1]))-mtk[7][2]));
    temp[0] = (((mtk[11][2]+((mfk[11][0]*Vpk[14][21][0])+(mfk[11][1]*
      Vpk[14][21][1])))+((mtk[10][2]+((mfk[10][0]*Vpk[14][20][0])+(mfk[10][1]*
      Vpk[14][20][1])))+((mtk[8][2]+(.17*mfk[8][0]))+(mtk[9][2]+((mfk[9][0]*
      Vpk[14][17][0])+(mfk[9][1]*Vpk[14][17][1]))))))+(((mfk[12][2]*
      Vpk[14][22][2])+((mfk[12][0]*Vpk[14][22][0])+(mfk[12][1]*Vpk[14][22][1])))
      +((mtk[12][1]*s22)+(mtk[12][2]*c22))));
    fs[14] = (mtau[14]+((((mfk[14][2]*Vpk[14][24][2])+((mfk[14][0]*
      Vpk[14][24][0])+(mfk[14][1]*Vpk[14][24][1])))+((mtk[14][2]*c22)+((
      mtk[14][0]*Wpk[14][24][0])+(mtk[14][1]*Wpk[14][24][1]))))+((((mfk[13][2]*
      Vpk[14][23][2])+((mfk[13][0]*Vpk[14][23][0])+(mfk[13][1]*Vpk[14][23][1])))
      +((mtk[13][2]*c22)+((mtk[13][0]*Wpk[14][23][0])+(mtk[13][1]*Wpk[14][23][1]
      ))))+temp[0])));
    temp[0] = (((((((mfk[9][1]*s17)-(mfk[9][0]*c17))-((mfk[10][0]*Vpk[15][20][0]
      )+(mfk[10][1]*Vpk[15][20][1])))-((mfk[11][0]*Vpk[15][21][0])+(mfk[11][1]*
      Vpk[15][21][1])))-((mfk[12][2]*Vpk[15][22][2])+((mfk[12][0]*Vpk[15][21][0]
      )+(mfk[12][1]*Vpk[15][22][1]))))-((mfk[13][2]*Vpk[15][22][2])+((mfk[13][0]
      *Vpk[15][23][0])+(mfk[13][1]*Vpk[15][23][1]))))-((mfk[14][2]*
      Vpk[15][22][2])+((mfk[14][0]*Vpk[15][24][0])+(mfk[14][1]*Vpk[15][24][1])))
      );
    fs[15] = (mtau[15]-temp[0]);
    temp[0] = (((mfk[14][2]*Vpk[16][22][2])+((mfk[14][0]*Vpk[16][24][0])+(
      mfk[14][1]*Vpk[16][24][1])))+(((mfk[13][2]*Vpk[16][22][2])+((mfk[13][0]*
      Vpk[16][23][0])+(mfk[13][1]*Vpk[16][23][1])))+(((mfk[12][2]*Vpk[16][22][2]
      )+((mfk[12][0]*Vpk[16][21][0])+(mfk[12][1]*Vpk[16][22][1])))+(((mfk[11][0]
      *Vpk[16][21][0])+(mfk[11][1]*Vpk[16][21][1]))+(((mfk[9][0]*s17)+(mfk[9][1]
      *c17))+((mfk[10][0]*Vpk[16][20][0])+(mfk[10][1]*Vpk[16][20][1])))))));
    fs[16] = (mtau[16]+temp[0]);
    temp[0] = ((((.43*((mfk[11][1]*s21)-(mfk[11][0]*c21)))-mtk[11][2])-((
      mtk[9][2]+(.1867*mfk[9][0]))+(mtk[10][2]+((mfk[10][0]*Vpk[17][20][0])+(
      mfk[10][1]*Vpk[17][20][1])))))-(((mfk[12][2]*Vpk[17][22][2])+((mfk[12][0]*
      Vpk[17][22][0])+(mfk[12][1]*Vpk[17][22][1])))+((mtk[12][1]*s22)+(
      mtk[12][2]*c22))));
    fs[17] = (mtau[17]-((temp[0]-(((mfk[13][2]*Vpk[17][23][2])+((mfk[13][0]*
      Vpk[17][23][0])+(mfk[13][1]*Vpk[17][23][1])))+((mtk[13][2]*c22)+((
      mtk[13][0]*Wpk[17][23][0])+(mtk[13][1]*Wpk[17][23][1])))))-(((mfk[14][2]*
      Vpk[17][24][2])+((mfk[14][0]*Vpk[17][24][0])+(mfk[14][1]*Vpk[17][24][1])))
      +((mtk[14][2]*c22)+((mtk[14][0]*Wpk[17][24][0])+(mtk[14][1]*Wpk[17][24][1]
      ))))));
    fs[18] = (mtau[18]-((mfk[10][1]*s20)-(mfk[10][0]*c20)));
    fs[19] = (mtau[19]+((mfk[10][0]*s20)+(mfk[10][1]*c20)));
    fs[20] = (mtau[20]+mtk[10][2]);
    temp[0] = ((((((.02377*(mfk[12][1]*c22))-(mfk[12][0]*Vpk[21][22][0]))-(
      .02377*(mfk[12][2]*s22)))-((mtk[12][1]*s22)+(mtk[12][2]*c22)))-mtk[11][2])
      -(((mfk[13][2]*Vpk[21][23][2])+((mfk[13][0]*Vpk[21][23][0])+(mfk[13][1]*
      Vpk[21][23][1])))+((mtk[13][2]*c22)+((mtk[13][0]*Wpk[21][23][0])+(
      mtk[13][1]*Wpk[21][23][1])))));
    fs[21] = (mtau[21]-(temp[0]-(((mfk[14][2]*Vpk[21][24][2])+((mfk[14][0]*
      Vpk[21][24][0])+(mfk[14][1]*Vpk[21][24][1])))+((mtk[14][2]*c22)+((
      mtk[14][0]*Wpk[21][24][0])+(mtk[14][1]*Wpk[21][24][1]))))));
    fs[22] = (mtau[22]-(((((mtk[13][1]*s23)-(mtk[13][0]*c23))-(mfk[13][2]*
      Vpk[22][23][2]))-(mtk[12][0]+(.015*mfk[12][2])))-(((mfk[14][2]*
      Vpk[22][24][2])+((mfk[14][0]*Vpk[22][24][0])+(mfk[14][1]*Vpk[22][24][1])))
      +((mtk[14][0]*Wpk[22][24][0])+(mtk[14][1]*Wpk[22][24][1])))));
    fs[23] = (mtau[23]-((((.015*mfk[13][0])-(.025*mfk[13][1]))-mtk[13][2])-(
      mtk[14][2]+((mfk[14][0]*Vpk[23][24][0])+(mfk[14][1]*Vpk[23][24][1])))));
    fs[24] = (mtau[24]-(((.006*mfk[14][0])-(.0346*mfk[14][1]))-mtk[14][2]));
/*
 Used 0.02 seconds CPU time,
 0 additional bytes of memory.
 Equations contain  383 adds/subtracts/negates
                    354 multiplies
                      0 divides
                     43 assignments
*/
}

void sdfsfull(void)
{

/*
Compute Fs (including all forces)
*/
    sdfsmult();
    fs[0] = (fs[0]+fs0[0]);
    fs[1] = (fs[1]+fs0[1]);
    fs[2] = (fs[2]+fs0[2]);
    fs[3] = (fs[3]+fs0[3]);
    fs[4] = (fs[4]+fs0[4]);
    fs[5] = (fs[5]+fs0[5]);
    fs[6] = (fs[6]+fs0[6]);
    fs[7] = (fs[7]+fs0[7]);
    fs[8] = (fs[8]+fs0[8]);
    fs[9] = (fs[9]+fs0[9]);
    fs[10] = (fs[10]+fs0[10]);
    fs[11] = (fs[11]+fs0[11]);
    fs[12] = (fs[12]+fs0[12]);
    fs[13] = (fs[13]+fs0[13]);
    fs[14] = (fs[14]+fs0[14]);
    fs[15] = (fs[15]+fs0[15]);
    fs[16] = (fs[16]+fs0[16]);
    fs[17] = (fs[17]+fs0[17]);
    fs[18] = (fs[18]+fs0[18]);
    fs[19] = (fs[19]+fs0[19]);
    fs[20] = (fs[20]+fs0[20]);
    fs[21] = (fs[21]+fs0[21]);
    fs[22] = (fs[22]+fs0[22]);
    fs[23] = (fs[23]+fs0[23]);
    fs[24] = (fs[24]+fs0[24]);
/*
 Used 0.00 seconds CPU time,
 0 additional bytes of memory.
 Equations contain   25 adds/subtracts/negates
                      0 multiplies
                      0 divides
                     25 assignments
*/
}

void sdfsgenmult(void)
{

/*
Compute Fs (generic multiplier-generated forces)
*/
    sddovpk();
    temp[0] = (((((((mfk[0][1]*s2)-(mfk[0][0]*c2))-((mfk[1][0]*Vpk[0][3][0])+(
      mfk[1][1]*Vpk[0][3][1])))-((mfk[2][0]*Vpk[0][6][0])+(mfk[2][1]*
      Vpk[0][6][1])))-((mfk[3][0]*Vpk[0][9][0])+(mfk[3][1]*Vpk[0][9][1])))-((
      mfk[4][0]*Vpk[0][10][0])+(mfk[4][1]*Vpk[0][10][1])))-((mfk[5][2]*
      Vpk[0][11][2])+((mfk[5][0]*Vpk[0][10][0])+(mfk[5][1]*Vpk[0][11][1]))));
    temp[1] = ((((((temp[0]-((mfk[6][2]*Vpk[0][11][2])+((mfk[6][0]*Vpk[0][12][0]
      )+(mfk[6][1]*Vpk[0][12][1]))))-((mfk[7][2]*Vpk[0][11][2])+((mfk[7][0]*
      Vpk[0][13][0])+(mfk[7][1]*Vpk[0][13][1]))))-((mfk[8][0]*Vpk[0][14][0])+(
      mfk[8][1]*Vpk[0][14][1])))-((mfk[9][0]*Vpk[0][17][0])+(mfk[9][1]*
      Vpk[0][17][1])))-((mfk[10][0]*Vpk[0][20][0])+(mfk[10][1]*Vpk[0][20][1])))-
      ((mfk[11][0]*Vpk[0][21][0])+(mfk[11][1]*Vpk[0][21][1])));
    fs[0] = (mtau[0]-(((temp[1]-((mfk[12][2]*Vpk[0][22][2])+((mfk[12][0]*
      Vpk[0][21][0])+(mfk[12][1]*Vpk[0][22][1]))))-((mfk[13][2]*Vpk[0][22][2])+(
      (mfk[13][0]*Vpk[0][23][0])+(mfk[13][1]*Vpk[0][23][1]))))-((mfk[14][2]*
      Vpk[0][22][2])+((mfk[14][0]*Vpk[0][24][0])+(mfk[14][1]*Vpk[0][24][1])))));
    temp[0] = (((mfk[5][2]*Vpk[1][11][2])+((mfk[5][0]*Vpk[1][10][0])+(mfk[5][1]*
      Vpk[1][11][1])))+(((mfk[4][0]*Vpk[1][10][0])+(mfk[4][1]*Vpk[1][10][1]))+((
      (mfk[3][0]*Vpk[1][9][0])+(mfk[3][1]*Vpk[1][9][1]))+(((mfk[2][0]*
      Vpk[1][6][0])+(mfk[2][1]*Vpk[1][6][1]))+(((mfk[0][0]*s2)+(mfk[0][1]*c2))+(
      (mfk[1][0]*Vpk[1][3][0])+(mfk[1][1]*Vpk[1][3][1])))))));
    temp[1] = (((mfk[11][0]*Vpk[1][21][0])+(mfk[11][1]*Vpk[1][21][1]))+(((
      mfk[10][0]*Vpk[1][20][0])+(mfk[10][1]*Vpk[1][20][1]))+(((mfk[9][0]*
      Vpk[1][17][0])+(mfk[9][1]*Vpk[1][17][1]))+(((mfk[8][0]*Vpk[1][14][0])+(
      mfk[8][1]*Vpk[1][14][1]))+(((mfk[7][2]*Vpk[1][11][2])+((mfk[7][0]*
      Vpk[1][13][0])+(mfk[7][1]*Vpk[1][13][1])))+(((mfk[6][2]*Vpk[1][11][2])+((
      mfk[6][0]*Vpk[1][12][0])+(mfk[6][1]*Vpk[1][12][1])))+temp[0]))))));
    fs[1] = (mtau[1]+(((mfk[14][2]*Vpk[1][22][2])+((mfk[14][0]*Vpk[1][24][0])+(
      mfk[14][1]*Vpk[1][24][1])))+(((mfk[13][2]*Vpk[1][22][2])+((mfk[13][0]*
      Vpk[1][23][0])+(mfk[13][1]*Vpk[1][23][1])))+(((mfk[12][2]*Vpk[1][22][2])+(
      (mfk[12][0]*Vpk[1][21][0])+(mfk[12][1]*Vpk[1][22][1])))+temp[1]))));
    temp[0] = (((((((.0707*mfk[0][1])+(.2639*mfk[0][0]))-mtk[0][2])-(mtk[1][2]+(
      (mfk[1][0]*Vpk[2][3][0])+(mfk[1][1]*Vpk[2][3][1]))))-(mtk[2][2]+((
      mfk[2][0]*Vpk[2][6][0])+(mfk[2][1]*Vpk[2][6][1]))))-(mtk[3][2]+((mfk[3][0]
      *Vpk[2][9][0])+(mfk[3][1]*Vpk[2][9][1]))))-(mtk[4][2]+((mfk[4][0]*
      Vpk[2][10][0])+(mfk[4][1]*Vpk[2][10][1]))));
    temp[1] = ((temp[0]-(((mfk[5][2]*Vpk[2][11][2])+((mfk[5][0]*Vpk[2][11][0])+(
      mfk[5][1]*Vpk[2][11][1])))+((mtk[5][1]*s11)+(mtk[5][2]*c11))))-(((
      mfk[6][2]*Vpk[2][12][2])+((mfk[6][0]*Vpk[2][12][0])+(mfk[6][1]*
      Vpk[2][12][1])))+((mtk[6][2]*c11)+((mtk[6][0]*Wpk[2][12][0])+(mtk[6][1]*
      Wpk[2][12][1])))));
    temp[2] = ((((temp[1]-(((mfk[7][2]*Vpk[2][13][2])+((mfk[7][0]*Vpk[2][13][0])
      +(mfk[7][1]*Vpk[2][13][1])))+((mtk[7][2]*c11)+((mtk[7][0]*Wpk[2][13][0])+(
      mtk[7][1]*Wpk[2][13][1])))))-(mtk[8][2]+((mfk[8][0]*Vpk[2][14][0])+(
      mfk[8][1]*Vpk[2][14][1]))))-(mtk[9][2]+((mfk[9][0]*Vpk[2][17][0])+(
      mfk[9][1]*Vpk[2][17][1]))))-(mtk[10][2]+((mfk[10][0]*Vpk[2][20][0])+(
      mfk[10][1]*Vpk[2][20][1]))));
    temp[3] = (((temp[2]-(mtk[11][2]+((mfk[11][0]*Vpk[2][21][0])+(mfk[11][1]*
      Vpk[2][21][1]))))-(((mfk[12][2]*Vpk[2][22][2])+((mfk[12][0]*Vpk[2][22][0])
      +(mfk[12][1]*Vpk[2][22][1])))+((mtk[12][1]*s22)+(mtk[12][2]*c22))))-(((
      mfk[13][2]*Vpk[2][23][2])+((mfk[13][0]*Vpk[2][23][0])+(mfk[13][1]*
      Vpk[2][23][1])))+((mtk[13][2]*c22)+((mtk[13][0]*Wpk[2][23][0])+(mtk[13][1]
      *Wpk[2][23][1])))));
    fs[2] = (mtau[2]-(temp[3]-(((mfk[14][2]*Vpk[2][24][2])+((mfk[14][0]*
      Vpk[2][24][0])+(mfk[14][1]*Vpk[2][24][1])))+((mtk[14][2]*c22)+((mtk[14][0]
      *Wpk[2][24][0])+(mtk[14][1]*Wpk[2][24][1]))))));
    temp[0] = (((mtk[4][2]+((mfk[4][0]*Vpk[3][10][0])+(mfk[4][1]*Vpk[3][10][1]))
      )+((mtk[3][2]+((mfk[3][0]*Vpk[3][9][0])+(mfk[3][1]*Vpk[3][9][1])))+((
      mtk[1][2]+(.17*mfk[1][0]))+(mtk[2][2]+((mfk[2][0]*Vpk[3][6][0])+(mfk[2][1]
      *Vpk[3][6][1]))))))+(((mfk[5][2]*Vpk[3][11][2])+((mfk[5][0]*Vpk[3][11][0])
      +(mfk[5][1]*Vpk[3][11][1])))+((mtk[5][1]*s11)+(mtk[5][2]*c11))));
    fs[3] = (mtau[3]+((((mfk[7][2]*Vpk[3][13][2])+((mfk[7][0]*Vpk[3][13][0])+(
      mfk[7][1]*Vpk[3][13][1])))+((mtk[7][2]*c11)+((mtk[7][0]*Wpk[3][13][0])+(
      mtk[7][1]*Wpk[3][13][1]))))+((((mfk[6][2]*Vpk[3][12][2])+((mfk[6][0]*
      Vpk[3][12][0])+(mfk[6][1]*Vpk[3][12][1])))+((mtk[6][2]*c11)+((mtk[6][0]*
      Wpk[3][12][0])+(mtk[6][1]*Wpk[3][12][1]))))+temp[0])));
    temp[0] = (((((((mfk[2][1]*s6)-(mfk[2][0]*c6))-((mfk[3][0]*Vpk[4][9][0])+(
      mfk[3][1]*Vpk[4][9][1])))-((mfk[4][0]*Vpk[4][10][0])+(mfk[4][1]*
      Vpk[4][10][1])))-((mfk[5][2]*Vpk[4][11][2])+((mfk[5][0]*Vpk[4][10][0])+(
      mfk[5][1]*Vpk[4][11][1]))))-((mfk[6][2]*Vpk[4][11][2])+((mfk[6][0]*
      Vpk[4][12][0])+(mfk[6][1]*Vpk[4][12][1]))))-((mfk[7][2]*Vpk[4][11][2])+((
      mfk[7][0]*Vpk[4][13][0])+(mfk[7][1]*Vpk[4][13][1]))));
    fs[4] = (mtau[4]-temp[0]);
    temp[0] = (((mfk[7][2]*Vpk[5][11][2])+((mfk[7][0]*Vpk[5][13][0])+(mfk[7][1]*
      Vpk[5][13][1])))+(((mfk[6][2]*Vpk[5][11][2])+((mfk[6][0]*Vpk[5][12][0])+(
      mfk[6][1]*Vpk[5][12][1])))+(((mfk[5][2]*Vpk[5][11][2])+((mfk[5][0]*
      Vpk[5][10][0])+(mfk[5][1]*Vpk[5][11][1])))+(((mfk[4][0]*Vpk[5][10][0])+(
      mfk[4][1]*Vpk[5][10][1]))+(((mfk[2][0]*s6)+(mfk[2][1]*c6))+((mfk[3][0]*
      Vpk[5][9][0])+(mfk[3][1]*Vpk[5][9][1])))))));
    fs[5] = (mtau[5]+temp[0]);
    temp[0] = ((((.43*((mfk[4][1]*s10)-(mfk[4][0]*c10)))-mtk[4][2])-((mtk[2][2]+
      (.1867*mfk[2][0]))+(mtk[3][2]+((mfk[3][0]*Vpk[6][9][0])+(mfk[3][1]*
      Vpk[6][9][1])))))-(((mfk[5][2]*Vpk[6][11][2])+((mfk[5][0]*Vpk[6][11][0])+(
      mfk[5][1]*Vpk[6][11][1])))+((mtk[5][1]*s11)+(mtk[5][2]*c11))));
    fs[6] = (mtau[6]-((temp[0]-(((mfk[6][2]*Vpk[6][12][2])+((mfk[6][0]*
      Vpk[6][12][0])+(mfk[6][1]*Vpk[6][12][1])))+((mtk[6][2]*c11)+((mtk[6][0]*
      Wpk[6][12][0])+(mtk[6][1]*Wpk[6][12][1])))))-(((mfk[7][2]*Vpk[6][13][2])+(
      (mfk[7][0]*Vpk[6][13][0])+(mfk[7][1]*Vpk[6][13][1])))+((mtk[7][2]*c11)+((
      mtk[7][0]*Wpk[6][13][0])+(mtk[7][1]*Wpk[6][13][1]))))));
    fs[7] = (mtau[7]-((mfk[3][1]*s9)-(mfk[3][0]*c9)));
    fs[8] = (mtau[8]+((mfk[3][0]*s9)+(mfk[3][1]*c9)));
    fs[9] = (mtau[9]+mtk[3][2]);
    temp[0] = ((((((.02377*(mfk[5][1]*c11))-(mfk[5][0]*Vpk[10][11][0]))-(.02377*
      (mfk[5][2]*s11)))-((mtk[5][1]*s11)+(mtk[5][2]*c11)))-mtk[4][2])-(((
      mfk[6][2]*Vpk[10][12][2])+((mfk[6][0]*Vpk[10][12][0])+(mfk[6][1]*
      Vpk[10][12][1])))+((mtk[6][2]*c11)+((mtk[6][0]*Wpk[10][12][0])+(mtk[6][1]*
      Wpk[10][12][1])))));
    fs[10] = (mtau[10]-(temp[0]-(((mfk[7][2]*Vpk[10][13][2])+((mfk[7][0]*
      Vpk[10][13][0])+(mfk[7][1]*Vpk[10][13][1])))+((mtk[7][2]*c11)+((mtk[7][0]*
      Wpk[10][13][0])+(mtk[7][1]*Wpk[10][13][1]))))));
    fs[11] = (mtau[11]-(((((mtk[6][1]*s12)-(mtk[6][0]*c12))-(mfk[6][2]*
      Vpk[11][12][2]))-(mtk[5][0]+(.015*mfk[5][2])))-(((mfk[7][2]*Vpk[11][13][2]
      )+((mfk[7][0]*Vpk[11][13][0])+(mfk[7][1]*Vpk[11][13][1])))+((mtk[7][0]*
      Wpk[11][13][0])+(mtk[7][1]*Wpk[11][13][1])))));
    fs[12] = (mtau[12]-((((.015*mfk[6][0])-(.025*mfk[6][1]))-mtk[6][2])-(
      mtk[7][2]+((mfk[7][0]*Vpk[12][13][0])+(mfk[7][1]*Vpk[12][13][1])))));
    fs[13] = (mtau[13]-(((.006*mfk[7][0])-(.0346*mfk[7][1]))-mtk[7][2]));
    temp[0] = (((mtk[11][2]+((mfk[11][0]*Vpk[14][21][0])+(mfk[11][1]*
      Vpk[14][21][1])))+((mtk[10][2]+((mfk[10][0]*Vpk[14][20][0])+(mfk[10][1]*
      Vpk[14][20][1])))+((mtk[8][2]+(.17*mfk[8][0]))+(mtk[9][2]+((mfk[9][0]*
      Vpk[14][17][0])+(mfk[9][1]*Vpk[14][17][1]))))))+(((mfk[12][2]*
      Vpk[14][22][2])+((mfk[12][0]*Vpk[14][22][0])+(mfk[12][1]*Vpk[14][22][1])))
      +((mtk[12][1]*s22)+(mtk[12][2]*c22))));
    fs[14] = (mtau[14]+((((mfk[14][2]*Vpk[14][24][2])+((mfk[14][0]*
      Vpk[14][24][0])+(mfk[14][1]*Vpk[14][24][1])))+((mtk[14][2]*c22)+((
      mtk[14][0]*Wpk[14][24][0])+(mtk[14][1]*Wpk[14][24][1]))))+((((mfk[13][2]*
      Vpk[14][23][2])+((mfk[13][0]*Vpk[14][23][0])+(mfk[13][1]*Vpk[14][23][1])))
      +((mtk[13][2]*c22)+((mtk[13][0]*Wpk[14][23][0])+(mtk[13][1]*Wpk[14][23][1]
      ))))+temp[0])));
    temp[0] = (((((((mfk[9][1]*s17)-(mfk[9][0]*c17))-((mfk[10][0]*Vpk[15][20][0]
      )+(mfk[10][1]*Vpk[15][20][1])))-((mfk[11][0]*Vpk[15][21][0])+(mfk[11][1]*
      Vpk[15][21][1])))-((mfk[12][2]*Vpk[15][22][2])+((mfk[12][0]*Vpk[15][21][0]
      )+(mfk[12][1]*Vpk[15][22][1]))))-((mfk[13][2]*Vpk[15][22][2])+((mfk[13][0]
      *Vpk[15][23][0])+(mfk[13][1]*Vpk[15][23][1]))))-((mfk[14][2]*
      Vpk[15][22][2])+((mfk[14][0]*Vpk[15][24][0])+(mfk[14][1]*Vpk[15][24][1])))
      );
    fs[15] = (mtau[15]-temp[0]);
    temp[0] = (((mfk[14][2]*Vpk[16][22][2])+((mfk[14][0]*Vpk[16][24][0])+(
      mfk[14][1]*Vpk[16][24][1])))+(((mfk[13][2]*Vpk[16][22][2])+((mfk[13][0]*
      Vpk[16][23][0])+(mfk[13][1]*Vpk[16][23][1])))+(((mfk[12][2]*Vpk[16][22][2]
      )+((mfk[12][0]*Vpk[16][21][0])+(mfk[12][1]*Vpk[16][22][1])))+(((mfk[11][0]
      *Vpk[16][21][0])+(mfk[11][1]*Vpk[16][21][1]))+(((mfk[9][0]*s17)+(mfk[9][1]
      *c17))+((mfk[10][0]*Vpk[16][20][0])+(mfk[10][1]*Vpk[16][20][1])))))));
    fs[16] = (mtau[16]+temp[0]);
    temp[0] = ((((.43*((mfk[11][1]*s21)-(mfk[11][0]*c21)))-mtk[11][2])-((
      mtk[9][2]+(.1867*mfk[9][0]))+(mtk[10][2]+((mfk[10][0]*Vpk[17][20][0])+(
      mfk[10][1]*Vpk[17][20][1])))))-(((mfk[12][2]*Vpk[17][22][2])+((mfk[12][0]*
      Vpk[17][22][0])+(mfk[12][1]*Vpk[17][22][1])))+((mtk[12][1]*s22)+(
      mtk[12][2]*c22))));
    fs[17] = (mtau[17]-((temp[0]-(((mfk[13][2]*Vpk[17][23][2])+((mfk[13][0]*
      Vpk[17][23][0])+(mfk[13][1]*Vpk[17][23][1])))+((mtk[13][2]*c22)+((
      mtk[13][0]*Wpk[17][23][0])+(mtk[13][1]*Wpk[17][23][1])))))-(((mfk[14][2]*
      Vpk[17][24][2])+((mfk[14][0]*Vpk[17][24][0])+(mfk[14][1]*Vpk[17][24][1])))
      +((mtk[14][2]*c22)+((mtk[14][0]*Wpk[17][24][0])+(mtk[14][1]*Wpk[17][24][1]
      ))))));
    fs[18] = (mtau[18]-((mfk[10][1]*s20)-(mfk[10][0]*c20)));
    fs[19] = (mtau[19]+((mfk[10][0]*s20)+(mfk[10][1]*c20)));
    fs[20] = (mtau[20]+mtk[10][2]);
    temp[0] = ((((((.02377*(mfk[12][1]*c22))-(mfk[12][0]*Vpk[21][22][0]))-(
      .02377*(mfk[12][2]*s22)))-((mtk[12][1]*s22)+(mtk[12][2]*c22)))-mtk[11][2])
      -(((mfk[13][2]*Vpk[21][23][2])+((mfk[13][0]*Vpk[21][23][0])+(mfk[13][1]*
      Vpk[21][23][1])))+((mtk[13][2]*c22)+((mtk[13][0]*Wpk[21][23][0])+(
      mtk[13][1]*Wpk[21][23][1])))));
    fs[21] = (mtau[21]-(temp[0]-(((mfk[14][2]*Vpk[21][24][2])+((mfk[14][0]*
      Vpk[21][24][0])+(mfk[14][1]*Vpk[21][24][1])))+((mtk[14][2]*c22)+((
      mtk[14][0]*Wpk[21][24][0])+(mtk[14][1]*Wpk[21][24][1]))))));
    fs[22] = (mtau[22]-(((((mtk[13][1]*s23)-(mtk[13][0]*c23))-(mfk[13][2]*
      Vpk[22][23][2]))-(mtk[12][0]+(.015*mfk[12][2])))-(((mfk[14][2]*
      Vpk[22][24][2])+((mfk[14][0]*Vpk[22][24][0])+(mfk[14][1]*Vpk[22][24][1])))
      +((mtk[14][0]*Wpk[22][24][0])+(mtk[14][1]*Wpk[22][24][1])))));
    fs[23] = (mtau[23]-((((.015*mfk[13][0])-(.025*mfk[13][1]))-mtk[13][2])-(
      mtk[14][2]+((mfk[14][0]*Vpk[23][24][0])+(mfk[14][1]*Vpk[23][24][1])))));
    fs[24] = (mtau[24]-(((.006*mfk[14][0])-(.0346*mfk[14][1]))-mtk[14][2]));
/*
 Used 0.02 seconds CPU time,
 0 additional bytes of memory.
 Equations contain  383 adds/subtracts/negates
                    354 multiplies
                      0 divides
                     43 assignments
*/
}

void sdfsgenfull(void)
{

/*
Compute Fs (incl generic mult & other forces)
*/
    sdfsgenmult();
    fs[0] = (fs[0]+fs0[0]);
    fs[1] = (fs[1]+fs0[1]);
    fs[2] = (fs[2]+fs0[2]);
    fs[3] = (fs[3]+fs0[3]);
    fs[4] = (fs[4]+fs0[4]);
    fs[5] = (fs[5]+fs0[5]);
    fs[6] = (fs[6]+fs0[6]);
    fs[7] = (fs[7]+fs0[7]);
    fs[8] = (fs[8]+fs0[8]);
    fs[9] = (fs[9]+fs0[9]);
    fs[10] = (fs[10]+fs0[10]);
    fs[11] = (fs[11]+fs0[11]);
    fs[12] = (fs[12]+fs0[12]);
    fs[13] = (fs[13]+fs0[13]);
    fs[14] = (fs[14]+fs0[14]);
    fs[15] = (fs[15]+fs0[15]);
    fs[16] = (fs[16]+fs0[16]);
    fs[17] = (fs[17]+fs0[17]);
    fs[18] = (fs[18]+fs0[18]);
    fs[19] = (fs[19]+fs0[19]);
    fs[20] = (fs[20]+fs0[20]);
    fs[21] = (fs[21]+fs0[21]);
    fs[22] = (fs[22]+fs0[22]);
    fs[23] = (fs[23]+fs0[23]);
    fs[24] = (fs[24]+fs0[24]);
/*
 Used -0.00 seconds CPU time,
 0 additional bytes of memory.
 Equations contain   25 adds/subtracts/negates
                      0 multiplies
                      0 divides
                     25 assignments
*/
}

void sdfulltrq(double udotin[25],
    double multin[12],
    double trqout[25])
{
/* Compute hinge torques which would produce indicated udots
*/
    double fstarr[25][3],tstarr[25][3],Otkr[25][3],Atir[25][3],Atkr[25][3];

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(61,23);
        return;
    }
/*
Compute multiplier-generated forces
*/
    sdmfrc(multin);
/*
Account for inertial accelerations and supplied udots
*/
    Otkr[3][2] = (udotin[2]+udotin[3]);
    Otkr[6][2] = (Otkr[3][2]+udotin[6]);
    Otkr[9][2] = (Otkr[6][2]+udotin[9]);
    Otkr[10][2] = (Otkr[6][2]+udotin[10]);
    Otkr[11][1] = ((Otkr[10][2]*s11)+(u[11]*wk[11][2]));
    Otkr[11][2] = ((Otkr[10][2]*c11)-(u[11]*wk[11][1]));
    Otkr[12][0] = ((u[12]*wk[12][1])+((Otkr[11][1]*s12)+(udotin[11]*c12)));
    Otkr[12][1] = (((Otkr[11][1]*c12)-(udotin[11]*s12))-(u[12]*wk[12][0]));
    Otkr[12][2] = (Otkr[11][2]+udotin[12]);
    Otkr[13][0] = ((u[13]*wk[13][1])+((Otkr[12][0]*c13)+(Otkr[12][1]*s13)));
    Otkr[13][1] = (((Otkr[12][1]*c13)-(Otkr[12][0]*s13))-(u[13]*wk[13][0]));
    Otkr[13][2] = (Otkr[12][2]+udotin[13]);
    Otkr[14][2] = (udotin[2]+udotin[14]);
    Otkr[17][2] = (Otkr[14][2]+udotin[17]);
    Otkr[20][2] = (Otkr[17][2]+udotin[20]);
    Otkr[21][2] = (Otkr[17][2]+udotin[21]);
    Otkr[22][1] = ((Otkr[21][2]*s22)+(u[22]*wk[22][2]));
    Otkr[22][2] = ((Otkr[21][2]*c22)-(u[22]*wk[22][1]));
    Otkr[23][0] = ((u[23]*wk[23][1])+((Otkr[22][1]*s23)+(udotin[22]*c23)));
    Otkr[23][1] = (((Otkr[22][1]*c23)-(udotin[22]*s23))-(u[23]*wk[23][0]));
    Otkr[23][2] = (Otkr[22][2]+udotin[23]);
    Otkr[24][0] = ((u[24]*wk[24][1])+((Otkr[23][0]*c24)+(Otkr[23][1]*s24)));
    Otkr[24][1] = (((Otkr[23][1]*c24)-(Otkr[23][0]*s24))-(u[24]*wk[24][0]));
    Otkr[24][2] = (Otkr[23][2]+udotin[24]);
    Atkr[2][0] = (((.0707*(u[2]*u[2]))-(.2639*udotin[2]))+((udotin[0]*c2)+(
      udotin[1]*s2)));
    Atkr[2][1] = (((udotin[1]*c2)-(udotin[0]*s2))-((.0707*udotin[2])+(.2639*(
      u[2]*u[2]))));
    Atir[3][0] = (Atkr[2][0]+(.33*udotin[2]));
    Atir[3][1] = (Atkr[2][1]+(.33*(u[2]*u[2])));
    Atkr[3][0] = ((.17*Otkr[3][2])+((Atir[3][0]*c3)+(Atir[3][1]*s3)));
    Atkr[3][1] = ((.17*(wk[3][2]*wk[3][2]))+((Atir[3][1]*c3)-(Atir[3][0]*s3)));
    Atir[4][0] = (Atkr[3][0]-(.17*Otkr[3][2]));
    Atir[4][1] = (Atkr[3][1]-(.17*(wk[3][2]*wk[3][2])));
    Atkr[4][0] = (udotin[4]+(Atir[4][0]-(wk[3][2]*Wkrpk[4][1])));
    Atkr[4][1] = (Atir[4][1]+((2.*(u[4]*wk[3][2]))+(Otkr[3][2]*q[4])));
    Atkr[5][0] = (Atkr[4][0]-((2.*(u[5]*wk[3][2]))+(Otkr[3][2]*q[5])));
    Atkr[5][1] = (udotin[5]+(Atkr[4][1]+(wk[3][2]*Wkrpk[5][0])));
    Atkr[6][0] = ((.1867*Otkr[6][2])+((Atkr[5][0]*c6)+(Atkr[5][1]*s6)));
    Atkr[6][1] = ((.1867*(wk[6][2]*wk[6][2]))+((Atkr[5][1]*c6)-(Atkr[5][0]*s6)))
      ;
    Atir[7][0] = (Atkr[6][0]-(.1867*Otkr[6][2]));
    Atir[7][1] = (Atkr[6][1]-(.1867*(wk[6][2]*wk[6][2])));
    Atkr[7][0] = (udotin[7]+(Atir[7][0]-(wk[6][2]*Wkrpk[7][1])));
    Atkr[7][1] = (Atir[7][1]+((2.*(u[7]*wk[6][2]))+(Otkr[6][2]*q[7])));
    Atkr[8][0] = (Atkr[7][0]-((2.*(u[8]*wk[6][2]))+(Otkr[6][2]*q[8])));
    Atkr[8][1] = (udotin[8]+(Atkr[7][1]+(wk[6][2]*Wkrpk[8][0])));
    Atkr[9][0] = ((Atkr[8][0]*c9)+(Atkr[8][1]*s9));
    Atkr[9][1] = ((Atkr[8][1]*c9)-(Atkr[8][0]*s9));
    Atir[10][0] = (Atkr[6][0]+(.2433*Otkr[6][2]));
    Atir[10][1] = (Atkr[6][1]+(.2433*(wk[6][2]*wk[6][2])));
    Atkr[10][0] = ((Atir[10][0]*c10)+(Atir[10][1]*s10));
    Atkr[10][1] = ((Atir[10][1]*c10)-(Atir[10][0]*s10));
    Atir[11][0] = (Atkr[10][0]+((.04195*Otkr[10][2])+(.04877*(wk[10][2]*
      wk[10][2]))));
    Atir[11][1] = (Atkr[10][1]+((.04195*(wk[10][2]*wk[10][2]))-(.04877*
      Otkr[10][2])));
    Atkr[11][0] = (Atir[11][0]+(((wk[11][1]*Wkrpk[11][2])-(.025*(wk[11][2]*
      wk[11][2])))-(.015*Otkr[11][2])));
    Atkr[11][1] = ((Atir[11][1]*c11)+((.025*Otkr[11][2])-((.015*(wk[11][2]*
      wk[11][2]))+(u[11]*Wkrpk[11][2]))));
    Atkr[11][2] = ((((.015*udotin[11])-(.025*Otkr[11][1]))+((.015*(wk[11][1]*
      wk[11][2]))+(.025*(u[11]*wk[11][2]))))-(Atir[11][1]*s11));
    Atir[12][0] = (Atkr[11][0]+((.015*Otkr[11][2])+((Wirk[12][2]*wk[11][1])-(
      .0942*(wk[11][2]*wk[11][2])))));
    Atir[12][1] = (Atkr[11][1]+((.0942*Otkr[11][2])+((.015*(wk[11][2]*wk[11][2])
      )-(u[11]*Wirk[12][2]))));
    Atir[12][2] = (Atkr[11][2]+(((.0942*(u[11]*wk[11][2]))-(.015*(wk[11][1]*
      wk[11][2])))-((.015*udotin[11])+(.0942*Otkr[11][1]))));
    Atkr[12][0] = (((Atir[12][0]*c12)+(Atir[12][1]*s12))+(((wk[12][1]*
      Wkrpk[12][2])-(.025*(wk[12][2]*wk[12][2])))-(.015*Otkr[12][2])));
    Atkr[12][1] = (((.025*Otkr[12][2])-((.015*(wk[12][2]*wk[12][2]))+(wk[12][0]*
      Wkrpk[12][2])))+((Atir[12][1]*c12)-(Atir[12][0]*s12)));
    Atkr[12][2] = (Atir[12][2]+(((.015*Otkr[12][0])-(.025*Otkr[12][1]))+((.015*(
      wk[12][1]*wk[12][2]))+(.025*(wk[12][0]*wk[12][2])))));
    Atir[13][0] = (Atkr[12][0]+(((.00108*Otkr[12][1])+(.017*Otkr[12][2]))+((
      Wirk[13][2]*wk[12][1])-(Wirk[13][1]*wk[12][2]))));
    Atir[13][1] = (Atkr[12][1]+(((.0346*Otkr[12][2])-(.00108*Otkr[12][0]))+((
      Wirk[13][0]*wk[12][2])-(Wirk[13][2]*wk[12][0]))));
    Atir[13][2] = (Atkr[12][2]+(((Wirk[13][1]*wk[12][0])-(Wirk[13][0]*wk[12][1])
      )-((.017*Otkr[12][0])+(.0346*Otkr[12][1]))));
    Atkr[13][0] = (((Atir[13][0]*c13)+(Atir[13][1]*s13))+(((wk[13][1]*
      Wkrpk[13][2])-(wk[13][2]*Wkrpk[13][1]))-((.006*Otkr[13][2])+(.0175*
      Otkr[13][1]))));
    Atkr[13][1] = (((Atir[13][1]*c13)-(Atir[13][0]*s13))+(((.0175*Otkr[13][0])+(
      .0346*Otkr[13][2]))+((wk[13][2]*Wkrpk[13][0])-(wk[13][0]*Wkrpk[13][2]))));
    Atkr[13][2] = (Atir[13][2]+(((.006*Otkr[13][0])-(.0346*Otkr[13][1]))+((
      wk[13][0]*Wkrpk[13][1])-(wk[13][1]*Wkrpk[13][0]))));
    Atir[14][0] = (Atkr[2][0]+(.33*udotin[2]));
    Atir[14][1] = (Atkr[2][1]+(.33*(u[2]*u[2])));
    Atkr[14][0] = ((.17*Otkr[14][2])+((Atir[14][0]*c14)+(Atir[14][1]*s14)));
    Atkr[14][1] = ((.17*(wk[14][2]*wk[14][2]))+((Atir[14][1]*c14)-(Atir[14][0]*
      s14)));
    Atir[15][0] = (Atkr[14][0]-(.17*Otkr[14][2]));
    Atir[15][1] = (Atkr[14][1]-(.17*(wk[14][2]*wk[14][2])));
    Atkr[15][0] = (udotin[15]+(Atir[15][0]-(wk[14][2]*Wkrpk[15][1])));
    Atkr[15][1] = (Atir[15][1]+((2.*(u[15]*wk[14][2]))+(Otkr[14][2]*q[15])));
    Atkr[16][0] = (Atkr[15][0]-((2.*(u[16]*wk[14][2]))+(Otkr[14][2]*q[16])));
    Atkr[16][1] = (udotin[16]+(Atkr[15][1]+(wk[14][2]*Wkrpk[16][0])));
    Atkr[17][0] = ((.1867*Otkr[17][2])+((Atkr[16][0]*c17)+(Atkr[16][1]*s17)));
    Atkr[17][1] = ((.1867*(wk[17][2]*wk[17][2]))+((Atkr[16][1]*c17)-(Atkr[16][0]
      *s17)));
    Atir[18][0] = (Atkr[17][0]-(.1867*Otkr[17][2]));
    Atir[18][1] = (Atkr[17][1]-(.1867*(wk[17][2]*wk[17][2])));
    Atkr[18][0] = (udotin[18]+(Atir[18][0]-(wk[17][2]*Wkrpk[18][1])));
    Atkr[18][1] = (Atir[18][1]+((2.*(u[18]*wk[17][2]))+(Otkr[17][2]*q[18])));
    Atkr[19][0] = (Atkr[18][0]-((2.*(u[19]*wk[17][2]))+(Otkr[17][2]*q[19])));
    Atkr[19][1] = (udotin[19]+(Atkr[18][1]+(wk[17][2]*Wkrpk[19][0])));
    Atkr[20][0] = ((Atkr[19][0]*c20)+(Atkr[19][1]*s20));
    Atkr[20][1] = ((Atkr[19][1]*c20)-(Atkr[19][0]*s20));
    Atir[21][0] = (Atkr[17][0]+(.2433*Otkr[17][2]));
    Atir[21][1] = (Atkr[17][1]+(.2433*(wk[17][2]*wk[17][2])));
    Atkr[21][0] = ((Atir[21][0]*c21)+(Atir[21][1]*s21));
    Atkr[21][1] = ((Atir[21][1]*c21)-(Atir[21][0]*s21));
    Atir[22][0] = (Atkr[21][0]+((.04195*Otkr[21][2])+(.04877*(wk[21][2]*
      wk[21][2]))));
    Atir[22][1] = (Atkr[21][1]+((.04195*(wk[21][2]*wk[21][2]))-(.04877*
      Otkr[21][2])));
    Atkr[22][0] = (Atir[22][0]+(((wk[22][1]*Wkrpk[22][2])-(.025*(wk[22][2]*
      wk[22][2])))-(.015*Otkr[22][2])));
    Atkr[22][1] = ((Atir[22][1]*c22)+((.025*Otkr[22][2])-((.015*(wk[22][2]*
      wk[22][2]))+(u[22]*Wkrpk[22][2]))));
    Atkr[22][2] = ((((.015*udotin[22])-(.025*Otkr[22][1]))+((.015*(wk[22][1]*
      wk[22][2]))+(.025*(u[22]*wk[22][2]))))-(Atir[22][1]*s22));
    Atir[23][0] = (Atkr[22][0]+((.015*Otkr[22][2])+((Wirk[23][2]*wk[22][1])-(
      .0942*(wk[22][2]*wk[22][2])))));
    Atir[23][1] = (Atkr[22][1]+((.0942*Otkr[22][2])+((.015*(wk[22][2]*wk[22][2])
      )-(u[22]*Wirk[23][2]))));
    Atir[23][2] = (Atkr[22][2]+(((.0942*(u[22]*wk[22][2]))-(.015*(wk[22][1]*
      wk[22][2])))-((.015*udotin[22])+(.0942*Otkr[22][1]))));
    Atkr[23][0] = (((Atir[23][0]*c23)+(Atir[23][1]*s23))+(((wk[23][1]*
      Wkrpk[23][2])-(.025*(wk[23][2]*wk[23][2])))-(.015*Otkr[23][2])));
    Atkr[23][1] = (((.025*Otkr[23][2])-((.015*(wk[23][2]*wk[23][2]))+(wk[23][0]*
      Wkrpk[23][2])))+((Atir[23][1]*c23)-(Atir[23][0]*s23)));
    Atkr[23][2] = (Atir[23][2]+(((.015*Otkr[23][0])-(.025*Otkr[23][1]))+((.015*(
      wk[23][1]*wk[23][2]))+(.025*(wk[23][0]*wk[23][2])))));
    Atir[24][0] = (Atkr[23][0]+(((.017*Otkr[23][2])-(.00108*Otkr[23][1]))+((
      Wirk[24][2]*wk[23][1])-(Wirk[24][1]*wk[23][2]))));
    Atir[24][1] = (Atkr[23][1]+(((.00108*Otkr[23][0])+(.0346*Otkr[23][2]))+((
      Wirk[24][0]*wk[23][2])-(Wirk[24][2]*wk[23][0]))));
    Atir[24][2] = (Atkr[23][2]+(((Wirk[24][1]*wk[23][0])-(Wirk[24][0]*wk[23][1])
      )-((.017*Otkr[23][0])+(.0346*Otkr[23][1]))));
    Atkr[24][0] = (((Atir[24][0]*c24)+(Atir[24][1]*s24))+(((.0175*Otkr[24][1])-(
      .006*Otkr[24][2]))+((wk[24][1]*Wkrpk[24][2])-(wk[24][2]*Wkrpk[24][1]))));
    Atkr[24][1] = (((Atir[24][1]*c24)-(Atir[24][0]*s24))+(((.0346*Otkr[24][2])-(
      .0175*Otkr[24][0]))+((wk[24][2]*Wkrpk[24][0])-(wk[24][0]*Wkrpk[24][2]))));
    Atkr[24][2] = (Atir[24][2]+(((.006*Otkr[24][0])-(.0346*Otkr[24][1]))+((
      wk[24][0]*Wkrpk[24][1])-(wk[24][1]*Wkrpk[24][0]))));
/*
Accumulate all forces and torques
*/
    fstarr[2][0] = ((mfk[0][0]+ufk[0][0])-(51.1*(Atkr[2][0]+(9.80665*s2))));
    fstarr[2][1] = ((mfk[0][1]+ufk[0][1])-(51.1*(Atkr[2][1]+(9.80665*c2))));
    fstarr[2][2] = (mfk[0][2]+ufk[0][2]);
    fstarr[3][0] = ((7.76*(gk[3][0]-Atkr[3][0]))+(mfk[1][0]+ufk[1][0]));
    fstarr[3][1] = ((7.76*(gk[3][1]-Atkr[3][1]))+(mfk[1][1]+ufk[1][1]));
    fstarr[3][2] = (mfk[1][2]+ufk[1][2]);
    fstarr[6][0] = ((3.03*(gk[6][0]-Atkr[6][0]))+(mfk[2][0]+ufk[2][0]));
    fstarr[6][1] = ((3.03*(gk[6][1]-Atkr[6][1]))+(mfk[2][1]+ufk[2][1]));
    fstarr[6][2] = (mfk[2][2]+ufk[2][2]);
    fstarr[9][0] = ((.1*(gk[9][0]-Atkr[9][0]))+(mfk[3][0]+ufk[3][0]));
    fstarr[9][1] = ((.1*(gk[9][1]-Atkr[9][1]))+(mfk[3][1]+ufk[3][1]));
    fstarr[9][2] = (mfk[3][2]+ufk[3][2]);
    fstarr[10][0] = ((.1*(gk[10][0]-Atkr[10][0]))+(mfk[4][0]+ufk[4][0]));
    fstarr[10][1] = ((.1*(gk[10][1]-Atkr[10][1]))+(mfk[4][1]+ufk[4][1]));
    fstarr[10][2] = (mfk[4][2]+ufk[4][2]);
    fstarr[11][0] = ((.4183*(gk[10][0]-Atkr[11][0]))+(mfk[5][0]+ufk[5][0]));
    fstarr[11][1] = ((.4183*(gk[11][1]-Atkr[11][1]))+(mfk[5][1]+ufk[5][1]));
    fstarr[11][2] = ((.4183*(gk[11][2]-Atkr[11][2]))+(mfk[5][2]+ufk[5][2]));
    fstarr[12][0] = ((.4*(gk[12][0]-Atkr[12][0]))+(mfk[6][0]+ufk[6][0]));
    fstarr[12][1] = ((.4*(gk[12][1]-Atkr[12][1]))+(mfk[6][1]+ufk[6][1]));
    fstarr[12][2] = ((.4*(gk[11][2]-Atkr[12][2]))+(mfk[6][2]+ufk[6][2]));
    fstarr[13][0] = ((.2058*(gk[13][0]-Atkr[13][0]))+(mfk[7][0]+ufk[7][0]));
    fstarr[13][1] = ((.2058*(gk[13][1]-Atkr[13][1]))+(mfk[7][1]+ufk[7][1]));
    fstarr[13][2] = ((.2058*(gk[11][2]-Atkr[13][2]))+(mfk[7][2]+ufk[7][2]));
    fstarr[14][0] = ((7.76*(gk[14][0]-Atkr[14][0]))+(mfk[8][0]+ufk[8][0]));
    fstarr[14][1] = ((7.76*(gk[14][1]-Atkr[14][1]))+(mfk[8][1]+ufk[8][1]));
    fstarr[14][2] = (mfk[8][2]+ufk[8][2]);
    fstarr[17][0] = ((3.03*(gk[17][0]-Atkr[17][0]))+(mfk[9][0]+ufk[9][0]));
    fstarr[17][1] = ((3.03*(gk[17][1]-Atkr[17][1]))+(mfk[9][1]+ufk[9][1]));
    fstarr[17][2] = (mfk[9][2]+ufk[9][2]);
    fstarr[20][0] = ((.1*(gk[20][0]-Atkr[20][0]))+(mfk[10][0]+ufk[10][0]));
    fstarr[20][1] = ((.1*(gk[20][1]-Atkr[20][1]))+(mfk[10][1]+ufk[10][1]));
    fstarr[20][2] = (mfk[10][2]+ufk[10][2]);
    fstarr[21][0] = ((.1*(gk[21][0]-Atkr[21][0]))+(mfk[11][0]+ufk[11][0]));
    fstarr[21][1] = ((.1*(gk[21][1]-Atkr[21][1]))+(mfk[11][1]+ufk[11][1]));
    fstarr[21][2] = (mfk[11][2]+ufk[11][2]);
    fstarr[22][0] = ((.4183*(gk[21][0]-Atkr[22][0]))+(mfk[12][0]+ufk[12][0]));
    fstarr[22][1] = ((.4183*(gk[22][1]-Atkr[22][1]))+(mfk[12][1]+ufk[12][1]));
    fstarr[22][2] = ((.4183*(gk[22][2]-Atkr[22][2]))+(mfk[12][2]+ufk[12][2]));
    fstarr[23][0] = ((.4*(gk[23][0]-Atkr[23][0]))+(mfk[13][0]+ufk[13][0]));
    fstarr[23][1] = ((.4*(gk[23][1]-Atkr[23][1]))+(mfk[13][1]+ufk[13][1]));
    fstarr[23][2] = ((.4*(gk[22][2]-Atkr[23][2]))+(mfk[13][2]+ufk[13][2]));
    fstarr[24][0] = ((.2058*(gk[24][0]-Atkr[24][0]))+(mfk[14][0]+ufk[14][0]));
    fstarr[24][1] = ((.2058*(gk[24][1]-Atkr[24][1]))+(mfk[14][1]+ufk[14][1]));
    fstarr[24][2] = ((.2058*(gk[22][2]-Atkr[24][2]))+(mfk[14][2]+ufk[14][2]));
    tstarr[2][0] = (mtk[0][0]+utk[0][0]);
    tstarr[2][1] = (mtk[0][1]+utk[0][1]);
    tstarr[2][2] = ((mtk[0][2]+utk[0][2])-(2.58*udotin[2]));
    tstarr[3][0] = (mtk[1][0]+utk[1][0]);
    tstarr[3][1] = (mtk[1][1]+utk[1][1]);
    tstarr[3][2] = ((mtk[1][2]+utk[1][2])-(.137*Otkr[3][2]));
    tstarr[6][0] = (mtk[2][0]+utk[2][0]);
    tstarr[6][1] = (mtk[2][1]+utk[2][1]);
    tstarr[6][2] = ((mtk[2][2]+utk[2][2])-(.0445*Otkr[6][2]));
    tstarr[9][0] = (mtk[3][0]+utk[3][0]);
    tstarr[9][1] = (mtk[3][1]+utk[3][1]);
    tstarr[9][2] = ((mtk[3][2]+utk[3][2])-(.001*Otkr[9][2]));
    tstarr[10][0] = (mtk[4][0]+utk[4][0]);
    tstarr[10][1] = (mtk[4][1]+utk[4][1]);
    tstarr[10][2] = ((mtk[4][2]+utk[4][2])-(.001*Otkr[10][2]));
    tstarr[11][0] = ((mtk[5][0]+utk[5][0])-(.001*udotin[11]));
    tstarr[11][1] = ((mtk[5][1]+utk[5][1])-(.001*Otkr[11][1]));
    tstarr[11][2] = ((mtk[5][2]+utk[5][2])-(.001*Otkr[11][2]));
    tstarr[12][0] = ((mtk[6][0]+utk[6][0])-(.001*Otkr[12][0]));
    tstarr[12][1] = ((mtk[6][1]+utk[6][1])-(.001*Otkr[12][1]));
    tstarr[12][2] = ((mtk[6][2]+utk[6][2])-(.001*Otkr[12][2]));
    tstarr[13][0] = ((mtk[7][0]+utk[7][0])-(.001*Otkr[13][0]));
    tstarr[13][1] = ((mtk[7][1]+utk[7][1])-(.001*Otkr[13][1]));
    tstarr[13][2] = ((mtk[7][2]+utk[7][2])-(.001*Otkr[13][2]));
    tstarr[14][0] = (mtk[8][0]+utk[8][0]);
    tstarr[14][1] = (mtk[8][1]+utk[8][1]);
    tstarr[14][2] = ((mtk[8][2]+utk[8][2])-(.137*Otkr[14][2]));
    tstarr[17][0] = (mtk[9][0]+utk[9][0]);
    tstarr[17][1] = (mtk[9][1]+utk[9][1]);
    tstarr[17][2] = ((mtk[9][2]+utk[9][2])-(.0445*Otkr[17][2]));
    tstarr[20][0] = (mtk[10][0]+utk[10][0]);
    tstarr[20][1] = (mtk[10][1]+utk[10][1]);
    tstarr[20][2] = ((mtk[10][2]+utk[10][2])-(.001*Otkr[20][2]));
    tstarr[21][0] = (mtk[11][0]+utk[11][0]);
    tstarr[21][1] = (mtk[11][1]+utk[11][1]);
    tstarr[21][2] = ((mtk[11][2]+utk[11][2])-(.001*Otkr[21][2]));
    tstarr[22][0] = ((mtk[12][0]+utk[12][0])-(.001*udotin[22]));
    tstarr[22][1] = ((mtk[12][1]+utk[12][1])-(.001*Otkr[22][1]));
    tstarr[22][2] = ((mtk[12][2]+utk[12][2])-(.001*Otkr[22][2]));
    tstarr[23][0] = ((mtk[13][0]+utk[13][0])-(.001*Otkr[23][0]));
    tstarr[23][1] = ((mtk[13][1]+utk[13][1])-(.001*Otkr[23][1]));
    tstarr[23][2] = ((mtk[13][2]+utk[13][2])-(.001*Otkr[23][2]));
    tstarr[24][0] = ((mtk[14][0]+utk[14][0])-(.001*Otkr[24][0]));
    tstarr[24][1] = ((mtk[14][1]+utk[14][1])-(.001*Otkr[24][1]));
    tstarr[24][2] = ((mtk[14][2]+utk[14][2])-(.001*Otkr[24][2]));
/*
Now calculate the torques
*/
    sddovpk();
    temp[0] = (((fstarr[11][2]*Vpk[0][11][2])+((fstarr[11][0]*Vpk[0][10][0])+(
      fstarr[11][1]*Vpk[0][11][1])))+(((fstarr[10][0]*Vpk[0][10][0])+(
      fstarr[10][1]*Vpk[0][10][1]))+(((fstarr[9][0]*Vpk[0][9][0])+(fstarr[9][1]*
      Vpk[0][9][1]))+(((fstarr[6][0]*Vpk[0][6][0])+(fstarr[6][1]*Vpk[0][6][1]))+
      (((fstarr[2][0]*c2)-(fstarr[2][1]*s2))+((fstarr[3][0]*Vpk[0][3][0])+(
      fstarr[3][1]*Vpk[0][3][1])))))));
    temp[1] = (((fstarr[21][0]*Vpk[0][21][0])+(fstarr[21][1]*Vpk[0][21][1]))+(((
      fstarr[20][0]*Vpk[0][20][0])+(fstarr[20][1]*Vpk[0][20][1]))+(((
      fstarr[17][0]*Vpk[0][17][0])+(fstarr[17][1]*Vpk[0][17][1]))+(((
      fstarr[14][0]*Vpk[0][14][0])+(fstarr[14][1]*Vpk[0][14][1]))+(((
      fstarr[13][2]*Vpk[0][11][2])+((fstarr[13][0]*Vpk[0][13][0])+(fstarr[13][1]
      *Vpk[0][13][1])))+(((fstarr[12][2]*Vpk[0][11][2])+((fstarr[12][0]*
      Vpk[0][12][0])+(fstarr[12][1]*Vpk[0][12][1])))+temp[0]))))));
    trqout[0] = -((mtau[0]+utau[0])+(((fstarr[24][2]*Vpk[0][22][2])+((
      fstarr[24][0]*Vpk[0][24][0])+(fstarr[24][1]*Vpk[0][24][1])))+(((
      fstarr[23][2]*Vpk[0][22][2])+((fstarr[23][0]*Vpk[0][23][0])+(fstarr[23][1]
      *Vpk[0][23][1])))+(((fstarr[22][2]*Vpk[0][22][2])+((fstarr[22][0]*
      Vpk[0][21][0])+(fstarr[22][1]*Vpk[0][22][1])))+temp[1]))));
    temp[0] = (((fstarr[11][2]*Vpk[1][11][2])+((fstarr[11][0]*Vpk[1][10][0])+(
      fstarr[11][1]*Vpk[1][11][1])))+(((fstarr[10][0]*Vpk[1][10][0])+(
      fstarr[10][1]*Vpk[1][10][1]))+(((fstarr[9][0]*Vpk[1][9][0])+(fstarr[9][1]*
      Vpk[1][9][1]))+(((fstarr[6][0]*Vpk[1][6][0])+(fstarr[6][1]*Vpk[1][6][1]))+
      (((fstarr[2][0]*s2)+(fstarr[2][1]*c2))+((fstarr[3][0]*Vpk[1][3][0])+(
      fstarr[3][1]*Vpk[1][3][1])))))));
    temp[1] = (((fstarr[21][0]*Vpk[1][21][0])+(fstarr[21][1]*Vpk[1][21][1]))+(((
      fstarr[20][0]*Vpk[1][20][0])+(fstarr[20][1]*Vpk[1][20][1]))+(((
      fstarr[17][0]*Vpk[1][17][0])+(fstarr[17][1]*Vpk[1][17][1]))+(((
      fstarr[14][0]*Vpk[1][14][0])+(fstarr[14][1]*Vpk[1][14][1]))+(((
      fstarr[13][2]*Vpk[1][11][2])+((fstarr[13][0]*Vpk[1][13][0])+(fstarr[13][1]
      *Vpk[1][13][1])))+(((fstarr[12][2]*Vpk[1][11][2])+((fstarr[12][0]*
      Vpk[1][12][0])+(fstarr[12][1]*Vpk[1][12][1])))+temp[0]))))));
    trqout[1] = -((mtau[1]+utau[1])+(((fstarr[24][2]*Vpk[1][22][2])+((
      fstarr[24][0]*Vpk[1][24][0])+(fstarr[24][1]*Vpk[1][24][1])))+(((
      fstarr[23][2]*Vpk[1][22][2])+((fstarr[23][0]*Vpk[1][23][0])+(fstarr[23][1]
      *Vpk[1][23][1])))+(((fstarr[22][2]*Vpk[1][22][2])+((fstarr[22][0]*
      Vpk[1][21][0])+(fstarr[22][1]*Vpk[1][22][1])))+temp[1]))));
    temp[0] = ((tstarr[10][2]+((fstarr[10][0]*Vpk[2][10][0])+(fstarr[10][1]*
      Vpk[2][10][1])))+((tstarr[9][2]+((fstarr[9][0]*Vpk[2][9][0])+(fstarr[9][1]
      *Vpk[2][9][1])))+((tstarr[6][2]+((fstarr[6][0]*Vpk[2][6][0])+(fstarr[6][1]
      *Vpk[2][6][1])))+((tstarr[2][2]-((.0707*fstarr[2][1])+(.2639*fstarr[2][0])
      ))+(tstarr[3][2]+((fstarr[3][0]*Vpk[2][3][0])+(fstarr[3][1]*Vpk[2][3][1]))
      )))));
    temp[1] = ((((fstarr[12][2]*Vpk[2][12][2])+((fstarr[12][0]*Vpk[2][12][0])+(
      fstarr[12][1]*Vpk[2][12][1])))+((tstarr[12][2]*c11)+((tstarr[12][0]*
      Wpk[2][12][0])+(tstarr[12][1]*Wpk[2][12][1]))))+(temp[0]+(((fstarr[11][2]*
      Vpk[2][11][2])+((fstarr[11][0]*Vpk[2][11][0])+(fstarr[11][1]*Vpk[2][11][1]
      )))+((tstarr[11][1]*s11)+(tstarr[11][2]*c11)))));
    temp[2] = ((tstarr[20][2]+((fstarr[20][0]*Vpk[2][20][0])+(fstarr[20][1]*
      Vpk[2][20][1])))+((tstarr[17][2]+((fstarr[17][0]*Vpk[2][17][0])+(
      fstarr[17][1]*Vpk[2][17][1])))+((tstarr[14][2]+((fstarr[14][0]*
      Vpk[2][14][0])+(fstarr[14][1]*Vpk[2][14][1])))+((((fstarr[13][2]*
      Vpk[2][13][2])+((fstarr[13][0]*Vpk[2][13][0])+(fstarr[13][1]*Vpk[2][13][1]
      )))+((tstarr[13][2]*c11)+((tstarr[13][0]*Wpk[2][13][0])+(tstarr[13][1]*
      Wpk[2][13][1]))))+temp[1]))));
    temp[3] = ((((fstarr[23][2]*Vpk[2][23][2])+((fstarr[23][0]*Vpk[2][23][0])+(
      fstarr[23][1]*Vpk[2][23][1])))+((tstarr[23][2]*c22)+((tstarr[23][0]*
      Wpk[2][23][0])+(tstarr[23][1]*Wpk[2][23][1]))))+(((tstarr[21][2]+((
      fstarr[21][0]*Vpk[2][21][0])+(fstarr[21][1]*Vpk[2][21][1])))+temp[2])+(((
      fstarr[22][2]*Vpk[2][22][2])+((fstarr[22][0]*Vpk[2][22][0])+(fstarr[22][1]
      *Vpk[2][22][1])))+((tstarr[22][1]*s22)+(tstarr[22][2]*c22)))));
    trqout[2] = -((mtau[2]+utau[2])+((((fstarr[24][2]*Vpk[2][24][2])+((
      fstarr[24][0]*Vpk[2][24][0])+(fstarr[24][1]*Vpk[2][24][1])))+((
      tstarr[24][2]*c22)+((tstarr[24][0]*Wpk[2][24][0])+(tstarr[24][1]*
      Wpk[2][24][1]))))+temp[3]));
    temp[0] = (((tstarr[10][2]+((fstarr[10][0]*Vpk[3][10][0])+(fstarr[10][1]*
      Vpk[3][10][1])))+((tstarr[9][2]+((fstarr[9][0]*Vpk[3][9][0])+(fstarr[9][1]
      *Vpk[3][9][1])))+((tstarr[3][2]+(.17*fstarr[3][0]))+(tstarr[6][2]+((
      fstarr[6][0]*Vpk[3][6][0])+(fstarr[6][1]*Vpk[3][6][1]))))))+(((
      fstarr[11][2]*Vpk[3][11][2])+((fstarr[11][0]*Vpk[3][11][0])+(fstarr[11][1]
      *Vpk[3][11][1])))+((tstarr[11][1]*s11)+(tstarr[11][2]*c11))));
    trqout[3] = -((mtau[3]+utau[3])+((((fstarr[13][2]*Vpk[3][13][2])+((
      fstarr[13][0]*Vpk[3][13][0])+(fstarr[13][1]*Vpk[3][13][1])))+((
      tstarr[13][2]*c11)+((tstarr[13][0]*Wpk[3][13][0])+(tstarr[13][1]*
      Wpk[3][13][1]))))+((((fstarr[12][2]*Vpk[3][12][2])+((fstarr[12][0]*
      Vpk[3][12][0])+(fstarr[12][1]*Vpk[3][12][1])))+((tstarr[12][2]*c11)+((
      tstarr[12][0]*Wpk[3][12][0])+(tstarr[12][1]*Wpk[3][12][1]))))+temp[0])));
    temp[0] = (((fstarr[13][2]*Vpk[4][11][2])+((fstarr[13][0]*Vpk[4][13][0])+(
      fstarr[13][1]*Vpk[4][13][1])))+(((fstarr[12][2]*Vpk[4][11][2])+((
      fstarr[12][0]*Vpk[4][12][0])+(fstarr[12][1]*Vpk[4][12][1])))+(((
      fstarr[11][2]*Vpk[4][11][2])+((fstarr[11][0]*Vpk[4][10][0])+(fstarr[11][1]
      *Vpk[4][11][1])))+(((fstarr[10][0]*Vpk[4][10][0])+(fstarr[10][1]*
      Vpk[4][10][1]))+(((fstarr[6][0]*c6)-(fstarr[6][1]*s6))+((fstarr[9][0]*
      Vpk[4][9][0])+(fstarr[9][1]*Vpk[4][9][1])))))));
    trqout[4] = -((mtau[4]+utau[4])+temp[0]);
    temp[0] = (((fstarr[13][2]*Vpk[5][11][2])+((fstarr[13][0]*Vpk[5][13][0])+(
      fstarr[13][1]*Vpk[5][13][1])))+(((fstarr[12][2]*Vpk[5][11][2])+((
      fstarr[12][0]*Vpk[5][12][0])+(fstarr[12][1]*Vpk[5][12][1])))+(((
      fstarr[11][2]*Vpk[5][11][2])+((fstarr[11][0]*Vpk[5][10][0])+(fstarr[11][1]
      *Vpk[5][11][1])))+(((fstarr[10][0]*Vpk[5][10][0])+(fstarr[10][1]*
      Vpk[5][10][1]))+(((fstarr[6][0]*s6)+(fstarr[6][1]*c6))+((fstarr[9][0]*
      Vpk[5][9][0])+(fstarr[9][1]*Vpk[5][9][1])))))));
    trqout[5] = -((mtau[5]+utau[5])+temp[0]);
    temp[0] = (((tstarr[10][2]+(.43*((fstarr[10][0]*c10)-(fstarr[10][1]*s10))))+
      ((tstarr[6][2]+(.1867*fstarr[6][0]))+(tstarr[9][2]+((fstarr[9][0]*
      Vpk[6][9][0])+(fstarr[9][1]*Vpk[6][9][1])))))+(((fstarr[11][2]*
      Vpk[6][11][2])+((fstarr[11][0]*Vpk[6][11][0])+(fstarr[11][1]*Vpk[6][11][1]
      )))+((tstarr[11][1]*s11)+(tstarr[11][2]*c11))));
    trqout[6] = -((mtau[6]+utau[6])+((((fstarr[13][2]*Vpk[6][13][2])+((
      fstarr[13][0]*Vpk[6][13][0])+(fstarr[13][1]*Vpk[6][13][1])))+((
      tstarr[13][2]*c11)+((tstarr[13][0]*Wpk[6][13][0])+(tstarr[13][1]*
      Wpk[6][13][1]))))+((((fstarr[12][2]*Vpk[6][12][2])+((fstarr[12][0]*
      Vpk[6][12][0])+(fstarr[12][1]*Vpk[6][12][1])))+((tstarr[12][2]*c11)+((
      tstarr[12][0]*Wpk[6][12][0])+(tstarr[12][1]*Wpk[6][12][1]))))+temp[0])));
    trqout[7] = -((mtau[7]+utau[7])+((fstarr[9][0]*c9)-(fstarr[9][1]*s9)));
    trqout[8] = -((mtau[8]+utau[8])+((fstarr[9][0]*s9)+(fstarr[9][1]*c9)));
    trqout[9] = -(tstarr[9][2]+(mtau[9]+utau[9]));
    temp[0] = ((tstarr[10][2]+(((.02377*(fstarr[11][2]*s11))+((fstarr[11][0]*
      Vpk[10][11][0])-(.02377*(fstarr[11][1]*c11))))+((tstarr[11][1]*s11)+(
      tstarr[11][2]*c11))))+(((fstarr[12][2]*Vpk[10][12][2])+((fstarr[12][0]*
      Vpk[10][12][0])+(fstarr[12][1]*Vpk[10][12][1])))+((tstarr[12][2]*c11)+((
      tstarr[12][0]*Wpk[10][12][0])+(tstarr[12][1]*Wpk[10][12][1])))));
    trqout[10] = -((mtau[10]+utau[10])+(temp[0]+(((fstarr[13][2]*Vpk[10][13][2])
      +((fstarr[13][0]*Vpk[10][13][0])+(fstarr[13][1]*Vpk[10][13][1])))+((
      tstarr[13][2]*c11)+((tstarr[13][0]*Wpk[10][13][0])+(tstarr[13][1]*
      Wpk[10][13][1]))))));
    trqout[11] = -((mtau[11]+utau[11])+(((tstarr[11][0]+(.015*fstarr[11][2]))+((
      fstarr[12][2]*Vpk[11][12][2])+((tstarr[12][0]*c12)-(tstarr[12][1]*s12))))+
      (((fstarr[13][2]*Vpk[11][13][2])+((fstarr[13][0]*Vpk[11][13][0])+(
      fstarr[13][1]*Vpk[11][13][1])))+((tstarr[13][0]*Wpk[11][13][0])+(
      tstarr[13][1]*Wpk[11][13][1])))));
    trqout[12] = -((mtau[12]+utau[12])+((tstarr[12][2]+((.025*fstarr[12][1])-(
      .015*fstarr[12][0])))+(tstarr[13][2]+((fstarr[13][0]*Vpk[12][13][0])+(
      fstarr[13][1]*Vpk[12][13][1])))));
    trqout[13] = -((mtau[13]+utau[13])+(tstarr[13][2]+((.0346*fstarr[13][1])-(
      .006*fstarr[13][0]))));
    temp[0] = (((tstarr[21][2]+((fstarr[21][0]*Vpk[14][21][0])+(fstarr[21][1]*
      Vpk[14][21][1])))+((tstarr[20][2]+((fstarr[20][0]*Vpk[14][20][0])+(
      fstarr[20][1]*Vpk[14][20][1])))+((tstarr[14][2]+(.17*fstarr[14][0]))+(
      tstarr[17][2]+((fstarr[17][0]*Vpk[14][17][0])+(fstarr[17][1]*
      Vpk[14][17][1]))))))+(((fstarr[22][2]*Vpk[14][22][2])+((fstarr[22][0]*
      Vpk[14][22][0])+(fstarr[22][1]*Vpk[14][22][1])))+((tstarr[22][1]*s22)+(
      tstarr[22][2]*c22))));
    trqout[14] = -((mtau[14]+utau[14])+((((fstarr[24][2]*Vpk[14][24][2])+((
      fstarr[24][0]*Vpk[14][24][0])+(fstarr[24][1]*Vpk[14][24][1])))+((
      tstarr[24][2]*c22)+((tstarr[24][0]*Wpk[14][24][0])+(tstarr[24][1]*
      Wpk[14][24][1]))))+((((fstarr[23][2]*Vpk[14][23][2])+((fstarr[23][0]*
      Vpk[14][23][0])+(fstarr[23][1]*Vpk[14][23][1])))+((tstarr[23][2]*c22)+((
      tstarr[23][0]*Wpk[14][23][0])+(tstarr[23][1]*Wpk[14][23][1]))))+temp[0])))
      ;
    temp[0] = (((fstarr[24][2]*Vpk[15][22][2])+((fstarr[24][0]*Vpk[15][24][0])+(
      fstarr[24][1]*Vpk[15][24][1])))+(((fstarr[23][2]*Vpk[15][22][2])+((
      fstarr[23][0]*Vpk[15][23][0])+(fstarr[23][1]*Vpk[15][23][1])))+(((
      fstarr[22][2]*Vpk[15][22][2])+((fstarr[22][0]*Vpk[15][21][0])+(
      fstarr[22][1]*Vpk[15][22][1])))+(((fstarr[21][0]*Vpk[15][21][0])+(
      fstarr[21][1]*Vpk[15][21][1]))+(((fstarr[17][0]*c17)-(fstarr[17][1]*s17))+
      ((fstarr[20][0]*Vpk[15][20][0])+(fstarr[20][1]*Vpk[15][20][1])))))));
    trqout[15] = -((mtau[15]+utau[15])+temp[0]);
    temp[0] = (((fstarr[24][2]*Vpk[16][22][2])+((fstarr[24][0]*Vpk[16][24][0])+(
      fstarr[24][1]*Vpk[16][24][1])))+(((fstarr[23][2]*Vpk[16][22][2])+((
      fstarr[23][0]*Vpk[16][23][0])+(fstarr[23][1]*Vpk[16][23][1])))+(((
      fstarr[22][2]*Vpk[16][22][2])+((fstarr[22][0]*Vpk[16][21][0])+(
      fstarr[22][1]*Vpk[16][22][1])))+(((fstarr[21][0]*Vpk[16][21][0])+(
      fstarr[21][1]*Vpk[16][21][1]))+(((fstarr[17][0]*s17)+(fstarr[17][1]*c17))+
      ((fstarr[20][0]*Vpk[16][20][0])+(fstarr[20][1]*Vpk[16][20][1])))))));
    trqout[16] = -((mtau[16]+utau[16])+temp[0]);
    temp[0] = (((tstarr[21][2]+(.43*((fstarr[21][0]*c21)-(fstarr[21][1]*s21))))+
      ((tstarr[17][2]+(.1867*fstarr[17][0]))+(tstarr[20][2]+((fstarr[20][0]*
      Vpk[17][20][0])+(fstarr[20][1]*Vpk[17][20][1])))))+(((fstarr[22][2]*
      Vpk[17][22][2])+((fstarr[22][0]*Vpk[17][22][0])+(fstarr[22][1]*
      Vpk[17][22][1])))+((tstarr[22][1]*s22)+(tstarr[22][2]*c22))));
    trqout[17] = -((mtau[17]+utau[17])+((((fstarr[24][2]*Vpk[17][24][2])+((
      fstarr[24][0]*Vpk[17][24][0])+(fstarr[24][1]*Vpk[17][24][1])))+((
      tstarr[24][2]*c22)+((tstarr[24][0]*Wpk[17][24][0])+(tstarr[24][1]*
      Wpk[17][24][1]))))+((((fstarr[23][2]*Vpk[17][23][2])+((fstarr[23][0]*
      Vpk[17][23][0])+(fstarr[23][1]*Vpk[17][23][1])))+((tstarr[23][2]*c22)+((
      tstarr[23][0]*Wpk[17][23][0])+(tstarr[23][1]*Wpk[17][23][1]))))+temp[0])))
      ;
    trqout[18] = -((mtau[18]+utau[18])+((fstarr[20][0]*c20)-(fstarr[20][1]*s20))
      );
    trqout[19] = -((mtau[19]+utau[19])+((fstarr[20][0]*s20)+(fstarr[20][1]*c20))
      );
    trqout[20] = -(tstarr[20][2]+(mtau[20]+utau[20]));
    temp[0] = ((tstarr[21][2]+(((.02377*(fstarr[22][2]*s22))+((fstarr[22][0]*
      Vpk[21][22][0])-(.02377*(fstarr[22][1]*c22))))+((tstarr[22][1]*s22)+(
      tstarr[22][2]*c22))))+(((fstarr[23][2]*Vpk[21][23][2])+((fstarr[23][0]*
      Vpk[21][23][0])+(fstarr[23][1]*Vpk[21][23][1])))+((tstarr[23][2]*c22)+((
      tstarr[23][0]*Wpk[21][23][0])+(tstarr[23][1]*Wpk[21][23][1])))));
    trqout[21] = -((mtau[21]+utau[21])+(temp[0]+(((fstarr[24][2]*Vpk[21][24][2])
      +((fstarr[24][0]*Vpk[21][24][0])+(fstarr[24][1]*Vpk[21][24][1])))+((
      tstarr[24][2]*c22)+((tstarr[24][0]*Wpk[21][24][0])+(tstarr[24][1]*
      Wpk[21][24][1]))))));
    trqout[22] = -((mtau[22]+utau[22])+(((tstarr[22][0]+(.015*fstarr[22][2]))+((
      fstarr[23][2]*Vpk[22][23][2])+((tstarr[23][0]*c23)-(tstarr[23][1]*s23))))+
      (((fstarr[24][2]*Vpk[22][24][2])+((fstarr[24][0]*Vpk[22][24][0])+(
      fstarr[24][1]*Vpk[22][24][1])))+((tstarr[24][0]*Wpk[22][24][0])+(
      tstarr[24][1]*Wpk[22][24][1])))));
    trqout[23] = -((mtau[23]+utau[23])+((tstarr[23][2]+((.025*fstarr[23][1])-(
      .015*fstarr[23][0])))+(tstarr[24][2]+((fstarr[24][0]*Vpk[23][24][0])+(
      fstarr[24][1]*Vpk[23][24][1])))));
    trqout[24] = -((mtau[24]+utau[24])+(tstarr[24][2]+((.0346*fstarr[24][1])-(
      .006*fstarr[24][0]))));
/*
Op counts below do not include called subroutines
*/
/*
 Used 0.03 seconds CPU time,
 0 additional bytes of memory.
 Equations contain  856 adds/subtracts/negates
                    725 multiplies
                      0 divides
                    241 assignments
*/
}

void sdcomptrq(double udotin[25],
    double trqout[25])
{
/* Compute hinge torques to produce these udots, ignoring constraints
*/
    int i;
    double multin[12];

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(60,23);
        return;
    }
    for (i = 0; i < 12; i++) {
        multin[i] = 0.;
    }
    sdfulltrq(udotin,multin,trqout);
}

void sdmulttrq(double multin[12],
    double trqout[25])
{
/* Compute hinge trqs which would be produced by these mults.
*/
    int i;

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(65,23);
        return;
    }
    sdmfrc(multin);
    sdfsmult();
    for (i = 0; i < 25; i++) {
        trqout[i] = fs[i];
    }
}

void sdrhs(void)
{
/*
Generated 22-Jun-2007 09:35:41 by SD/FAST, Kane's formulation
(sdfast B.2.8 #30123) on machine ID unknown
Copyright (c) 1990-1997 Symbolic Dynamics, Inc.
Copyright (c) 1990-1997 Parametric Technology Corp.
RESTRICTED RIGHTS LEGEND: Use, duplication, or disclosure by the U.S.
Government is subject to restrictions as set forth in subparagraph
(c)(1)(ii) of the Rights in Technical Data and Computer Software
clause at DFARS 52.227-7013 and similar clauses in the FAR and NASA
FAR Supplement.  Symbolic Dynamics, Inc., Mountain View, CA 94041
*/

/*
Compute hinge torques for tree hinges
*/
    tauc[0] = (mtau[0]+utau[0]);
    tauc[1] = (mtau[1]+utau[1]);
    tauc[2] = (mtau[2]+utau[2]);
    tauc[3] = (mtau[3]+utau[3]);
    tauc[4] = (mtau[4]+utau[4]);
    tauc[5] = (mtau[5]+utau[5]);
    tauc[6] = (mtau[6]+utau[6]);
    tauc[7] = (mtau[7]+utau[7]);
    tauc[8] = (mtau[8]+utau[8]);
    tauc[9] = (mtau[9]+utau[9]);
    tauc[10] = (mtau[10]+utau[10]);
    tauc[11] = (mtau[11]+utau[11]);
    tauc[12] = (mtau[12]+utau[12]);
    tauc[13] = (mtau[13]+utau[13]);
    tauc[14] = (mtau[14]+utau[14]);
    tauc[15] = (mtau[15]+utau[15]);
    tauc[16] = (mtau[16]+utau[16]);
    tauc[17] = (mtau[17]+utau[17]);
    tauc[18] = (mtau[18]+utau[18]);
    tauc[19] = (mtau[19]+utau[19]);
    tauc[20] = (mtau[20]+utau[20]);
    tauc[21] = (mtau[21]+utau[21]);
    tauc[22] = (mtau[22]+utau[22]);
    tauc[23] = (mtau[23]+utau[23]);
    tauc[24] = (mtau[24]+utau[24]);
    sddoiner();
/*
Compute onk & onb (angular accels in N)
*/
    Onkb[3][2] = (udot[2]+udot[3]);
    Onkb[6][2] = (Onkb[3][2]+udot[6]);
    Onkb[9][2] = (Onkb[6][2]+udot[9]);
    Onkb[10][2] = (Onkb[6][2]+udot[10]);
    Onkb[11][1] = (Onkb[10][2]*s11);
    Onkb[11][2] = (Onkb[10][2]*c11);
    Onkb[12][0] = ((Onkb[11][1]*s12)+(udot[11]*c12));
    Onkb[12][1] = ((Onkb[11][1]*c12)-(udot[11]*s12));
    Onkb[12][2] = (Onkb[11][2]+udot[12]);
    Onkb[13][0] = ((Onkb[12][0]*c13)+(Onkb[12][1]*s13));
    Onkb[13][1] = ((Onkb[12][1]*c13)-(Onkb[12][0]*s13));
    Onkb[13][2] = (Onkb[12][2]+udot[13]);
    Onkb[14][2] = (udot[2]+udot[14]);
    Onkb[17][2] = (Onkb[14][2]+udot[17]);
    Onkb[20][2] = (Onkb[17][2]+udot[20]);
    Onkb[21][2] = (Onkb[17][2]+udot[21]);
    Onkb[22][1] = (Onkb[21][2]*s22);
    Onkb[22][2] = (Onkb[21][2]*c22);
    Onkb[23][0] = ((Onkb[22][1]*s23)+(udot[22]*c23));
    Onkb[23][1] = ((Onkb[22][1]*c23)-(udot[22]*s23));
    Onkb[23][2] = (Onkb[22][2]+udot[23]);
    Onkb[24][0] = ((Onkb[23][0]*c24)+(Onkb[23][1]*s24));
    Onkb[24][1] = ((Onkb[23][1]*c24)-(Onkb[23][0]*s24));
    Onkb[24][2] = (Onkb[23][2]+udot[24]);
    onk[11][1] = (Onkb[11][1]+Otk[11][1]);
    onk[11][2] = (Onkb[11][2]+Otk[11][2]);
    onk[12][0] = (Onkb[12][0]+Otk[12][0]);
    onk[12][1] = (Onkb[12][1]+Otk[12][1]);
    onk[12][2] = (Onkb[12][2]+Otk[11][2]);
    onk[13][0] = (Onkb[13][0]+Otk[13][0]);
    onk[13][1] = (Onkb[13][1]+Otk[13][1]);
    onk[13][2] = (Onkb[13][2]+Otk[11][2]);
    onk[22][1] = (Onkb[22][1]+Otk[22][1]);
    onk[22][2] = (Onkb[22][2]+Otk[22][2]);
    onk[23][0] = (Onkb[23][0]+Otk[23][0]);
    onk[23][1] = (Onkb[23][1]+Otk[23][1]);
    onk[23][2] = (Onkb[23][2]+Otk[22][2]);
    onk[24][0] = (Onkb[24][0]+Otk[24][0]);
    onk[24][1] = (Onkb[24][1]+Otk[24][1]);
    onk[24][2] = (Onkb[24][2]+Otk[22][2]);
    onb[0][0] = 0.;
    onb[0][1] = 0.;
    onb[0][2] = udot[2];
    onb[1][0] = 0.;
    onb[1][1] = 0.;
    onb[1][2] = Onkb[3][2];
    onb[2][0] = 0.;
    onb[2][1] = 0.;
    onb[2][2] = Onkb[6][2];
    onb[3][0] = 0.;
    onb[3][1] = 0.;
    onb[3][2] = Onkb[9][2];
    onb[4][0] = 0.;
    onb[4][1] = 0.;
    onb[4][2] = Onkb[10][2];
    onb[5][0] = udot[11];
    onb[5][1] = onk[11][1];
    onb[5][2] = onk[11][2];
    onb[6][0] = onk[12][0];
    onb[6][1] = onk[12][1];
    onb[6][2] = onk[12][2];
    onb[7][0] = onk[13][0];
    onb[7][1] = onk[13][1];
    onb[7][2] = onk[13][2];
    onb[8][0] = 0.;
    onb[8][1] = 0.;
    onb[8][2] = Onkb[14][2];
    onb[9][0] = 0.;
    onb[9][1] = 0.;
    onb[9][2] = Onkb[17][2];
    onb[10][0] = 0.;
    onb[10][1] = 0.;
    onb[10][2] = Onkb[20][2];
    onb[11][0] = 0.;
    onb[11][1] = 0.;
    onb[11][2] = Onkb[21][2];
    onb[12][0] = udot[22];
    onb[12][1] = onk[22][1];
    onb[12][2] = onk[22][2];
    onb[13][0] = onk[23][0];
    onb[13][1] = onk[23][1];
    onb[13][2] = onk[23][2];
    onb[14][0] = onk[24][0];
    onb[14][1] = onk[24][1];
    onb[14][2] = onk[24][2];
/*
Compute acceleration dyadics
*/
    dyad[0][0][0] = -w2w2[0];
    dyad[0][0][1] = -udot[2];
    dyad[0][0][2] = 0.;
    dyad[0][1][0] = udot[2];
    dyad[0][1][1] = -w2w2[0];
    dyad[0][1][2] = 0.;
    dyad[0][2][0] = 0.;
    dyad[0][2][1] = 0.;
    dyad[0][2][2] = 0.;
    dyad[1][0][0] = -w2w2[1];
    dyad[1][0][1] = -Onkb[3][2];
    dyad[1][0][2] = 0.;
    dyad[1][1][0] = Onkb[3][2];
    dyad[1][1][1] = -w2w2[1];
    dyad[1][1][2] = 0.;
    dyad[1][2][0] = 0.;
    dyad[1][2][1] = 0.;
    dyad[1][2][2] = 0.;
    dyad[2][0][0] = -w2w2[2];
    dyad[2][0][1] = -Onkb[6][2];
    dyad[2][0][2] = 0.;
    dyad[2][1][0] = Onkb[6][2];
    dyad[2][1][1] = -w2w2[2];
    dyad[2][1][2] = 0.;
    dyad[2][2][0] = 0.;
    dyad[2][2][1] = 0.;
    dyad[2][2][2] = 0.;
    dyad[3][0][0] = -w2w2[3];
    dyad[3][0][1] = -Onkb[9][2];
    dyad[3][0][2] = 0.;
    dyad[3][1][0] = Onkb[9][2];
    dyad[3][1][1] = -w2w2[3];
    dyad[3][1][2] = 0.;
    dyad[3][2][0] = 0.;
    dyad[3][2][1] = 0.;
    dyad[3][2][2] = 0.;
    dyad[4][0][0] = -w2w2[4];
    dyad[4][0][1] = -Onkb[10][2];
    dyad[4][0][2] = 0.;
    dyad[4][1][0] = Onkb[10][2];
    dyad[4][1][1] = -w2w2[4];
    dyad[4][1][2] = 0.;
    dyad[4][2][0] = 0.;
    dyad[4][2][1] = 0.;
    dyad[4][2][2] = 0.;
    dyad[5][0][0] = w11w22[5];
    dyad[5][0][1] = (w0w1[5]-onk[11][2]);
    dyad[5][0][2] = (onk[11][1]+w0w2[5]);
    dyad[5][1][0] = (onk[11][2]+w0w1[5]);
    dyad[5][1][1] = w00w22[5];
    dyad[5][1][2] = (w1w2[5]-udot[11]);
    dyad[5][2][0] = (w0w2[5]-onk[11][1]);
    dyad[5][2][1] = (udot[11]+w1w2[5]);
    dyad[5][2][2] = w00w11[5];
    dyad[6][0][0] = w11w22[6];
    dyad[6][0][1] = (w0w1[6]-onk[12][2]);
    dyad[6][0][2] = (onk[12][1]+w0w2[6]);
    dyad[6][1][0] = (onk[12][2]+w0w1[6]);
    dyad[6][1][1] = w00w22[6];
    dyad[6][1][2] = (w1w2[6]-onk[12][0]);
    dyad[6][2][0] = (w0w2[6]-onk[12][1]);
    dyad[6][2][1] = (onk[12][0]+w1w2[6]);
    dyad[6][2][2] = w00w11[6];
    dyad[7][0][0] = w11w22[7];
    dyad[7][0][1] = (w0w1[7]-onk[13][2]);
    dyad[7][0][2] = (onk[13][1]+w0w2[7]);
    dyad[7][1][0] = (onk[13][2]+w0w1[7]);
    dyad[7][1][1] = w00w22[7];
    dyad[7][1][2] = (w1w2[7]-onk[13][0]);
    dyad[7][2][0] = (w0w2[7]-onk[13][1]);
    dyad[7][2][1] = (onk[13][0]+w1w2[7]);
    dyad[7][2][2] = w00w11[7];
    dyad[8][0][0] = -w2w2[8];
    dyad[8][0][1] = -Onkb[14][2];
    dyad[8][0][2] = 0.;
    dyad[8][1][0] = Onkb[14][2];
    dyad[8][1][1] = -w2w2[8];
    dyad[8][1][2] = 0.;
    dyad[8][2][0] = 0.;
    dyad[8][2][1] = 0.;
    dyad[8][2][2] = 0.;
    dyad[9][0][0] = -w2w2[9];
    dyad[9][0][1] = -Onkb[17][2];
    dyad[9][0][2] = 0.;
    dyad[9][1][0] = Onkb[17][2];
    dyad[9][1][1] = -w2w2[9];
    dyad[9][1][2] = 0.;
    dyad[9][2][0] = 0.;
    dyad[9][2][1] = 0.;
    dyad[9][2][2] = 0.;
    dyad[10][0][0] = -w2w2[10];
    dyad[10][0][1] = -Onkb[20][2];
    dyad[10][0][2] = 0.;
    dyad[10][1][0] = Onkb[20][2];
    dyad[10][1][1] = -w2w2[10];
    dyad[10][1][2] = 0.;
    dyad[10][2][0] = 0.;
    dyad[10][2][1] = 0.;
    dyad[10][2][2] = 0.;
    dyad[11][0][0] = -w2w2[11];
    dyad[11][0][1] = -Onkb[21][2];
    dyad[11][0][2] = 0.;
    dyad[11][1][0] = Onkb[21][2];
    dyad[11][1][1] = -w2w2[11];
    dyad[11][1][2] = 0.;
    dyad[11][2][0] = 0.;
    dyad[11][2][1] = 0.;
    dyad[11][2][2] = 0.;
    dyad[12][0][0] = w11w22[12];
    dyad[12][0][1] = (w0w1[12]-onk[22][2]);
    dyad[12][0][2] = (onk[22][1]+w0w2[12]);
    dyad[12][1][0] = (onk[22][2]+w0w1[12]);
    dyad[12][1][1] = w00w22[12];
    dyad[12][1][2] = (w1w2[12]-udot[22]);
    dyad[12][2][0] = (w0w2[12]-onk[22][1]);
    dyad[12][2][1] = (udot[22]+w1w2[12]);
    dyad[12][2][2] = w00w11[12];
    dyad[13][0][0] = w11w22[13];
    dyad[13][0][1] = (w0w1[13]-onk[23][2]);
    dyad[13][0][2] = (onk[23][1]+w0w2[13]);
    dyad[13][1][0] = (onk[23][2]+w0w1[13]);
    dyad[13][1][1] = w00w22[13];
    dyad[13][1][2] = (w1w2[13]-onk[23][0]);
    dyad[13][2][0] = (w0w2[13]-onk[23][1]);
    dyad[13][2][1] = (onk[23][0]+w1w2[13]);
    dyad[13][2][2] = w00w11[13];
    dyad[14][0][0] = w11w22[14];
    dyad[14][0][1] = (w0w1[14]-onk[24][2]);
    dyad[14][0][2] = (onk[24][1]+w0w2[14]);
    dyad[14][1][0] = (onk[24][2]+w0w1[14]);
    dyad[14][1][1] = w00w22[14];
    dyad[14][1][2] = (w1w2[14]-onk[24][0]);
    dyad[14][2][0] = (w0w2[14]-onk[24][1]);
    dyad[14][2][1] = (onk[24][0]+w1w2[14]);
    dyad[14][2][2] = w00w11[14];
/*
Compute ank & anb (mass center linear accels in N)
*/
    Ankb[2][0] = (((udot[0]*c2)+(udot[1]*s2))-(.2639*udot[2]));
    Ankb[2][1] = (((udot[1]*c2)-(udot[0]*s2))-(.0707*udot[2]));
    AOnkri[3][0] = (Ankb[2][0]+(.33*udot[2]));
    Ankb[3][0] = ((.17*Onkb[3][2])+((Ankb[2][1]*s3)+(AOnkri[3][0]*c3)));
    Ankb[3][1] = ((Ankb[2][1]*c3)-(AOnkri[3][0]*s3));
    AOnkri[4][0] = (Ankb[3][0]-(.17*Onkb[3][2]));
    Ankb[4][0] = (AOnkri[4][0]+udot[4]);
    Ankb[4][1] = (Ankb[3][1]+(Onkb[3][2]*q[4]));
    Ankb[5][0] = (Ankb[4][0]-(Onkb[3][2]*q[5]));
    Ankb[5][1] = (Ankb[4][1]+udot[5]);
    Ankb[6][0] = ((.1867*Onkb[6][2])+((Ankb[5][0]*c6)+(Ankb[5][1]*s6)));
    Ankb[6][1] = ((Ankb[5][1]*c6)-(Ankb[5][0]*s6));
    AOnkri[7][0] = (Ankb[6][0]-(.1867*Onkb[6][2]));
    Ankb[7][0] = (AOnkri[7][0]+udot[7]);
    Ankb[7][1] = (Ankb[6][1]+(Onkb[6][2]*q[7]));
    Ankb[8][0] = (Ankb[7][0]-(Onkb[6][2]*q[8]));
    Ankb[8][1] = (Ankb[7][1]+udot[8]);
    Ankb[9][0] = ((Ankb[8][0]*c9)+(Ankb[8][1]*s9));
    Ankb[9][1] = ((Ankb[8][1]*c9)-(Ankb[8][0]*s9));
    AOnkri[10][0] = (Ankb[6][0]+(.2433*Onkb[6][2]));
    Ankb[10][0] = ((Ankb[6][1]*s10)+(AOnkri[10][0]*c10));
    Ankb[10][1] = ((Ankb[6][1]*c10)-(AOnkri[10][0]*s10));
    AOnkri[11][0] = (Ankb[10][0]+(.04195*Onkb[10][2]));
    AOnkri[11][1] = (Ankb[10][1]-(.04877*Onkb[10][2]));
    Ankb[11][0] = (AOnkri[11][0]-(.015*Onkb[11][2]));
    Ankb[11][1] = ((.025*Onkb[11][2])+(AOnkri[11][1]*c11));
    Ankb[11][2] = (((.015*udot[11])-(.025*Onkb[11][1]))-(AOnkri[11][1]*s11));
    AOnkri[12][0] = (Ankb[11][0]+(.015*Onkb[11][2]));
    AOnkri[12][1] = (Ankb[11][1]+(.0942*Onkb[11][2]));
    AOnkri[12][2] = (Ankb[11][2]-((.015*udot[11])+(.0942*Onkb[11][1])));
    Ankb[12][0] = (((AOnkri[12][0]*c12)+(AOnkri[12][1]*s12))-(.015*Onkb[12][2]))
      ;
    Ankb[12][1] = ((.025*Onkb[12][2])+((AOnkri[12][1]*c12)-(AOnkri[12][0]*s12)))
      ;
    Ankb[12][2] = (AOnkri[12][2]+((.015*Onkb[12][0])-(.025*Onkb[12][1])));
    AOnkri[13][0] = (Ankb[12][0]+((.00108*Onkb[12][1])+(.017*Onkb[12][2])));
    AOnkri[13][1] = (Ankb[12][1]+((.0346*Onkb[12][2])-(.00108*Onkb[12][0])));
    AOnkri[13][2] = (Ankb[12][2]-((.017*Onkb[12][0])+(.0346*Onkb[12][1])));
    Ankb[13][0] = (((AOnkri[13][0]*c13)+(AOnkri[13][1]*s13))-((.006*Onkb[13][2])
      +(.0175*Onkb[13][1])));
    Ankb[13][1] = (((.0175*Onkb[13][0])+(.0346*Onkb[13][2]))+((AOnkri[13][1]*c13
      )-(AOnkri[13][0]*s13)));
    Ankb[13][2] = (AOnkri[13][2]+((.006*Onkb[13][0])-(.0346*Onkb[13][1])));
    AOnkri[14][0] = (Ankb[2][0]+(.33*udot[2]));
    Ankb[14][0] = ((.17*Onkb[14][2])+((Ankb[2][1]*s14)+(AOnkri[14][0]*c14)));
    Ankb[14][1] = ((Ankb[2][1]*c14)-(AOnkri[14][0]*s14));
    AOnkri[15][0] = (Ankb[14][0]-(.17*Onkb[14][2]));
    Ankb[15][0] = (AOnkri[15][0]+udot[15]);
    Ankb[15][1] = (Ankb[14][1]+(Onkb[14][2]*q[15]));
    Ankb[16][0] = (Ankb[15][0]-(Onkb[14][2]*q[16]));
    Ankb[16][1] = (Ankb[15][1]+udot[16]);
    Ankb[17][0] = ((.1867*Onkb[17][2])+((Ankb[16][0]*c17)+(Ankb[16][1]*s17)));
    Ankb[17][1] = ((Ankb[16][1]*c17)-(Ankb[16][0]*s17));
    AOnkri[18][0] = (Ankb[17][0]-(.1867*Onkb[17][2]));
    Ankb[18][0] = (AOnkri[18][0]+udot[18]);
    Ankb[18][1] = (Ankb[17][1]+(Onkb[17][2]*q[18]));
    Ankb[19][0] = (Ankb[18][0]-(Onkb[17][2]*q[19]));
    Ankb[19][1] = (Ankb[18][1]+udot[19]);
    Ankb[20][0] = ((Ankb[19][0]*c20)+(Ankb[19][1]*s20));
    Ankb[20][1] = ((Ankb[19][1]*c20)-(Ankb[19][0]*s20));
    AOnkri[21][0] = (Ankb[17][0]+(.2433*Onkb[17][2]));
    Ankb[21][0] = ((Ankb[17][1]*s21)+(AOnkri[21][0]*c21));
    Ankb[21][1] = ((Ankb[17][1]*c21)-(AOnkri[21][0]*s21));
    AOnkri[22][0] = (Ankb[21][0]+(.04195*Onkb[21][2]));
    AOnkri[22][1] = (Ankb[21][1]-(.04877*Onkb[21][2]));
    Ankb[22][0] = (AOnkri[22][0]-(.015*Onkb[22][2]));
    Ankb[22][1] = ((.025*Onkb[22][2])+(AOnkri[22][1]*c22));
    Ankb[22][2] = (((.015*udot[22])-(.025*Onkb[22][1]))-(AOnkri[22][1]*s22));
    AOnkri[23][0] = (Ankb[22][0]+(.015*Onkb[22][2]));
    AOnkri[23][1] = (Ankb[22][1]+(.0942*Onkb[22][2]));
    AOnkri[23][2] = (Ankb[22][2]-((.015*udot[22])+(.0942*Onkb[22][1])));
    Ankb[23][0] = (((AOnkri[23][0]*c23)+(AOnkri[23][1]*s23))-(.015*Onkb[23][2]))
      ;
    Ankb[23][1] = ((.025*Onkb[23][2])+((AOnkri[23][1]*c23)-(AOnkri[23][0]*s23)))
      ;
    Ankb[23][2] = (AOnkri[23][2]+((.015*Onkb[23][0])-(.025*Onkb[23][1])));
    AOnkri[24][0] = (Ankb[23][0]+((.017*Onkb[23][2])-(.00108*Onkb[23][1])));
    AOnkri[24][1] = (Ankb[23][1]+((.00108*Onkb[23][0])+(.0346*Onkb[23][2])));
    AOnkri[24][2] = (Ankb[23][2]-((.017*Onkb[23][0])+(.0346*Onkb[23][1])));
    Ankb[24][0] = (((.0175*Onkb[24][1])-(.006*Onkb[24][2]))+((AOnkri[24][0]*c24)
      +(AOnkri[24][1]*s24)));
    Ankb[24][1] = (((.0346*Onkb[24][2])-(.0175*Onkb[24][0]))+((AOnkri[24][1]*c24
      )-(AOnkri[24][0]*s24)));
    Ankb[24][2] = (AOnkri[24][2]+((.006*Onkb[24][0])-(.0346*Onkb[24][1])));
    AnkAtk[2][0] = (Ankb[2][0]+Atk[2][0]);
    AnkAtk[2][1] = (Ankb[2][1]+Atk[2][1]);
    ank[2][0] = ((AnkAtk[2][0]*c2)-(AnkAtk[2][1]*s2));
    ank[2][1] = ((AnkAtk[2][0]*s2)+(AnkAtk[2][1]*c2));
    AnkAtk[3][0] = (Ankb[3][0]+Atk[3][0]);
    AnkAtk[3][1] = (Ankb[3][1]+Atk[3][1]);
    ank[3][0] = ((AnkAtk[3][0]*cnk[3][0][0])+(AnkAtk[3][1]*cnk[3][0][1]));
    ank[3][1] = ((AnkAtk[3][0]*cnk[3][1][0])+(AnkAtk[3][1]*cnk[3][1][1]));
    AnkAtk[6][0] = (Ankb[6][0]+Atk[6][0]);
    AnkAtk[6][1] = (Ankb[6][1]+Atk[6][1]);
    ank[6][0] = ((AnkAtk[6][0]*cnk[6][0][0])+(AnkAtk[6][1]*cnk[6][0][1]));
    ank[6][1] = ((AnkAtk[6][0]*cnk[6][1][0])+(AnkAtk[6][1]*cnk[6][1][1]));
    AnkAtk[9][0] = (Ankb[9][0]+Atk[9][0]);
    AnkAtk[9][1] = (Ankb[9][1]+Atk[9][1]);
    ank[9][0] = ((AnkAtk[9][0]*cnk[9][0][0])+(AnkAtk[9][1]*cnk[9][0][1]));
    ank[9][1] = ((AnkAtk[9][0]*cnk[9][1][0])+(AnkAtk[9][1]*cnk[9][1][1]));
    AnkAtk[10][0] = (Ankb[10][0]+Atk[10][0]);
    AnkAtk[10][1] = (Ankb[10][1]+Atk[10][1]);
    ank[10][0] = ((AnkAtk[10][0]*cnk[10][0][0])+(AnkAtk[10][1]*cnk[10][0][1]));
    ank[10][1] = ((AnkAtk[10][0]*cnk[10][1][0])+(AnkAtk[10][1]*cnk[10][1][1]));
    AnkAtk[11][0] = (Ankb[11][0]+Atk[11][0]);
    AnkAtk[11][1] = (Ankb[11][1]+Atk[11][1]);
    AnkAtk[11][2] = (Ankb[11][2]+Atk[11][2]);
    ank[11][0] = ((AnkAtk[11][2]*cnk[11][0][2])+((AnkAtk[11][0]*cnk[10][0][0])+(
      AnkAtk[11][1]*cnk[11][0][1])));
    ank[11][1] = ((AnkAtk[11][2]*cnk[11][1][2])+((AnkAtk[11][0]*cnk[10][1][0])+(
      AnkAtk[11][1]*cnk[11][1][1])));
    ank[11][2] = ((AnkAtk[11][1]*s11)+(AnkAtk[11][2]*c11));
    AnkAtk[12][0] = (Ankb[12][0]+Atk[12][0]);
    AnkAtk[12][1] = (Ankb[12][1]+Atk[12][1]);
    AnkAtk[12][2] = (Ankb[12][2]+Atk[12][2]);
    ank[12][0] = ((AnkAtk[12][2]*cnk[11][0][2])+((AnkAtk[12][0]*cnk[12][0][0])+(
      AnkAtk[12][1]*cnk[12][0][1])));
    ank[12][1] = ((AnkAtk[12][2]*cnk[11][1][2])+((AnkAtk[12][0]*cnk[12][1][0])+(
      AnkAtk[12][1]*cnk[12][1][1])));
    ank[12][2] = ((AnkAtk[12][2]*c11)+((AnkAtk[12][0]*cnk[12][2][0])+(
      AnkAtk[12][1]*cnk[12][2][1])));
    AnkAtk[13][0] = (Ankb[13][0]+Atk[13][0]);
    AnkAtk[13][1] = (Ankb[13][1]+Atk[13][1]);
    AnkAtk[13][2] = (Ankb[13][2]+Atk[13][2]);
    ank[13][0] = ((AnkAtk[13][2]*cnk[11][0][2])+((AnkAtk[13][0]*cnk[13][0][0])+(
      AnkAtk[13][1]*cnk[13][0][1])));
    ank[13][1] = ((AnkAtk[13][2]*cnk[11][1][2])+((AnkAtk[13][0]*cnk[13][1][0])+(
      AnkAtk[13][1]*cnk[13][1][1])));
    ank[13][2] = ((AnkAtk[13][2]*c11)+((AnkAtk[13][0]*cnk[13][2][0])+(
      AnkAtk[13][1]*cnk[13][2][1])));
    AnkAtk[14][0] = (Ankb[14][0]+Atk[14][0]);
    AnkAtk[14][1] = (Ankb[14][1]+Atk[14][1]);
    ank[14][0] = ((AnkAtk[14][0]*cnk[14][0][0])+(AnkAtk[14][1]*cnk[14][0][1]));
    ank[14][1] = ((AnkAtk[14][0]*cnk[14][1][0])+(AnkAtk[14][1]*cnk[14][1][1]));
    AnkAtk[17][0] = (Ankb[17][0]+Atk[17][0]);
    AnkAtk[17][1] = (Ankb[17][1]+Atk[17][1]);
    ank[17][0] = ((AnkAtk[17][0]*cnk[17][0][0])+(AnkAtk[17][1]*cnk[17][0][1]));
    ank[17][1] = ((AnkAtk[17][0]*cnk[17][1][0])+(AnkAtk[17][1]*cnk[17][1][1]));
    AnkAtk[20][0] = (Ankb[20][0]+Atk[20][0]);
    AnkAtk[20][1] = (Ankb[20][1]+Atk[20][1]);
    ank[20][0] = ((AnkAtk[20][0]*cnk[20][0][0])+(AnkAtk[20][1]*cnk[20][0][1]));
    ank[20][1] = ((AnkAtk[20][0]*cnk[20][1][0])+(AnkAtk[20][1]*cnk[20][1][1]));
    AnkAtk[21][0] = (Ankb[21][0]+Atk[21][0]);
    AnkAtk[21][1] = (Ankb[21][1]+Atk[21][1]);
    ank[21][0] = ((AnkAtk[21][0]*cnk[21][0][0])+(AnkAtk[21][1]*cnk[21][0][1]));
    ank[21][1] = ((AnkAtk[21][0]*cnk[21][1][0])+(AnkAtk[21][1]*cnk[21][1][1]));
    AnkAtk[22][0] = (Ankb[22][0]+Atk[22][0]);
    AnkAtk[22][1] = (Ankb[22][1]+Atk[22][1]);
    AnkAtk[22][2] = (Ankb[22][2]+Atk[22][2]);
    ank[22][0] = ((AnkAtk[22][2]*cnk[22][0][2])+((AnkAtk[22][0]*cnk[21][0][0])+(
      AnkAtk[22][1]*cnk[22][0][1])));
    ank[22][1] = ((AnkAtk[22][2]*cnk[22][1][2])+((AnkAtk[22][0]*cnk[21][1][0])+(
      AnkAtk[22][1]*cnk[22][1][1])));
    ank[22][2] = ((AnkAtk[22][1]*s22)+(AnkAtk[22][2]*c22));
    AnkAtk[23][0] = (Ankb[23][0]+Atk[23][0]);
    AnkAtk[23][1] = (Ankb[23][1]+Atk[23][1]);
    AnkAtk[23][2] = (Ankb[23][2]+Atk[23][2]);
    ank[23][0] = ((AnkAtk[23][2]*cnk[22][0][2])+((AnkAtk[23][0]*cnk[23][0][0])+(
      AnkAtk[23][1]*cnk[23][0][1])));
    ank[23][1] = ((AnkAtk[23][2]*cnk[22][1][2])+((AnkAtk[23][0]*cnk[23][1][0])+(
      AnkAtk[23][1]*cnk[23][1][1])));
    ank[23][2] = ((AnkAtk[23][2]*c22)+((AnkAtk[23][0]*cnk[23][2][0])+(
      AnkAtk[23][1]*cnk[23][2][1])));
    AnkAtk[24][0] = (Ankb[24][0]+Atk[24][0]);
    AnkAtk[24][1] = (Ankb[24][1]+Atk[24][1]);
    AnkAtk[24][2] = (Ankb[24][2]+Atk[24][2]);
    ank[24][0] = ((AnkAtk[24][2]*cnk[22][0][2])+((AnkAtk[24][0]*cnk[24][0][0])+(
      AnkAtk[24][1]*cnk[24][0][1])));
    ank[24][1] = ((AnkAtk[24][2]*cnk[22][1][2])+((AnkAtk[24][0]*cnk[24][1][0])+(
      AnkAtk[24][1]*cnk[24][1][1])));
    ank[24][2] = ((AnkAtk[24][2]*c22)+((AnkAtk[24][0]*cnk[24][2][0])+(
      AnkAtk[24][1]*cnk[24][2][1])));
    anb[0][0] = ank[2][0];
    anb[0][1] = ank[2][1];
    anb[0][2] = 0.;
    anb[1][0] = ank[3][0];
    anb[1][1] = ank[3][1];
    anb[1][2] = 0.;
    anb[2][0] = ank[6][0];
    anb[2][1] = ank[6][1];
    anb[2][2] = 0.;
    anb[3][0] = ank[9][0];
    anb[3][1] = ank[9][1];
    anb[3][2] = 0.;
    anb[4][0] = ank[10][0];
    anb[4][1] = ank[10][1];
    anb[4][2] = 0.;
    anb[5][0] = ank[11][0];
    anb[5][1] = ank[11][1];
    anb[5][2] = ank[11][2];
    anb[6][0] = ank[12][0];
    anb[6][1] = ank[12][1];
    anb[6][2] = ank[12][2];
    anb[7][0] = ank[13][0];
    anb[7][1] = ank[13][1];
    anb[7][2] = ank[13][2];
    anb[8][0] = ank[14][0];
    anb[8][1] = ank[14][1];
    anb[8][2] = 0.;
    anb[9][0] = ank[17][0];
    anb[9][1] = ank[17][1];
    anb[9][2] = 0.;
    anb[10][0] = ank[20][0];
    anb[10][1] = ank[20][1];
    anb[10][2] = 0.;
    anb[11][0] = ank[21][0];
    anb[11][1] = ank[21][1];
    anb[11][2] = 0.;
    anb[12][0] = ank[22][0];
    anb[12][1] = ank[22][1];
    anb[12][2] = ank[22][2];
    anb[13][0] = ank[23][0];
    anb[13][1] = ank[23][1];
    anb[13][2] = ank[23][2];
    anb[14][0] = ank[24][0];
    anb[14][1] = ank[24][1];
    anb[14][2] = ank[24][2];
/*
Compute constraint acceleration errors
*/
    roustate = 3;
    sduaerr(curtim,q,u,udot,&aerr[2]);
/*
 Used 0.02 seconds CPU time,
 0 additional bytes of memory.
 Equations contain  320 adds/subtracts/negates
                    238 multiplies
                      0 divides
                    438 assignments
*/
}

void sdmassmat(double mmat[25][25])
{
/* Return the system mass matrix (LHS)
*/
    int i,j;

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(57,23);
        return;
    }
    sddomm(57);
    for (i = 0; i < 25; i++) {
        for (j = i; j <= 24; j++) {
            mmat[i][j] = mm[i][j];
            mmat[j][i] = mm[i][j];
        }
    }
}

void sdfrcmat(double fmat[25])
{
/* Return the system force matrix (RHS), excluding constraints
*/
    int i;

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(58,23);
        return;
    }
    sddofs0();
    for (i = 0; i < 25; i++) {
        fmat[i] = fs0[i];
    }
}

void sdpseudo(double lqout[1],
    double luout[1])
{
/*
Return pseudo-coordinates for loop joints.

*/
/*
There are no loop joints in this system.

*/
}

void sdpsqdot(double lqdout[1])
{
/*
Return pseudo-coordinate derivatives for loop joints.

*/
/*
There are no loop joints in this system.

*/
}

void sdpsudot(double ludout[1])
{
/*
Return pseudo-coordinate accelerations for loop joints.

*/
/*
There are no loop joints in this system.

*/
}

void sdperr(double errs[12])
{
/*
Return position constraint errors.

*/

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(26,23);
        return;
    }
    perr[0] = (q[11]-upos[11]);
    perr[1] = (q[22]-upos[22]);
    errs[0] = perr[0];
    errs[1] = perr[1];
    errs[2] = perr[2];
    errs[3] = perr[3];
    errs[4] = perr[4];
    errs[5] = perr[5];
    errs[6] = perr[6];
    errs[7] = perr[7];
    errs[8] = perr[8];
    errs[9] = perr[9];
    errs[10] = perr[10];
    errs[11] = perr[11];
}

void sdverr(double errs[12])
{
/*
Return velocity constraint errors.

*/

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(27,23);
        return;
    }
    verr[0] = (u[11]-uvel[11]);
    verr[1] = (u[22]-uvel[22]);
    errs[0] = verr[0];
    errs[1] = verr[1];
    errs[2] = verr[2];
    errs[3] = verr[3];
    errs[4] = verr[4];
    errs[5] = verr[5];
    errs[6] = verr[6];
    errs[7] = verr[7];
    errs[8] = verr[8];
    errs[9] = verr[9];
    errs[10] = verr[10];
    errs[11] = verr[11];
}

void sdaerr(double errs[12])
{
/*
Return acceleration constraint errors.

*/

    if (roustate != 3) {
        sdseterr(35,24);
        return;
    }
    aerr[0] = (udot[11]-uacc[11]);
    aerr[1] = (udot[22]-uacc[22]);
    errs[0] = aerr[0];
    errs[1] = aerr[1];
    errs[2] = aerr[2];
    errs[3] = aerr[3];
    errs[4] = aerr[4];
    errs[5] = aerr[5];
    errs[6] = aerr[6];
    errs[7] = aerr[7];
    errs[8] = aerr[8];
    errs[9] = aerr[9];
    errs[10] = aerr[10];
    errs[11] = aerr[11];
}
int 
sdchkbnum(int routine,
    int bnum)
{

    if ((bnum < -1) || (bnum > 14)) {
        sdseterr(routine,15);
        return 1;
    }
    return 0;
}
int 
sdchkjnum(int routine,
    int jnum)
{

    if ((jnum < 0) || (jnum > 14)) {
        sdseterr(routine,16);
        return 1;
    }
    return 0;
}
int 
sdchkucnum(int routine,
    int ucnum)
{

    if ((ucnum < 0) || (ucnum > 9)) {
        sdseterr(routine,21);
        return 1;
    }
    return 0;
}
int 
sdchkjaxis(int routine,
    int jnum,
    int axnum)
{
    int maxax;

    if (sdchkjnum(routine,jnum) != 0) {
        return 1;
    }
    if ((axnum < 0) || (axnum > 6)) {
        sdseterr(routine,17);
        return 1;
    }
    maxax = njntdof[jnum]-1;
    if ((jtype[jnum] == 4) || (jtype[jnum] == 6) || (jtype[jnum] == 21)) {
        maxax = maxax+1;
    }
    if (axnum > maxax) {
        sdseterr(routine,18);
        return 1;
    }
    return 0;
}
int 
sdchkjpin(int routine,
    int jnum,
    int pinno)
{
    int maxax,pinok;

    if (sdchkjnum(routine,jnum) != 0) {
        return 1;
    }
    if ((pinno < 0) || (pinno > 5)) {
        sdseterr(routine,17);
        return 1;
    }
    if (njntdof[jnum] >= 3) {
        maxax = 2;
    } else {
        maxax = njntdof[jnum]-1;
    }
    if (jtype[jnum] == 4) {
        maxax = -1;
    }
    if (jtype[jnum] == 7) {
        maxax = 0;
    }
    pinok = 0;
    if (pinno <= maxax) {
        pinok = 1;
    }
    if (pinok == 0) {
        sdseterr(routine,18);
        return 1;
    }
    return 0;
}
int 
sdindx(int joint,
    int axis)
{
    int offs,gotit;

    if (sdchkjaxis(36,joint,axis) != 0) {
        return 0;
    }
    gotit = 0;
    if (jtype[joint] == 4) {
        if (axis == 3) {
            offs = ballq[joint];
            gotit = 1;
        }
    } else {
        if ((jtype[joint] == 6) || (jtype[joint] == 21)) {
            if (axis == 6) {
                offs = ballq[joint];
                gotit = 1;
            }
        }
    }
    if (gotit == 0) {
        offs = firstq[joint]+axis;
    }
    return offs;
}

void sdpresacc(int joint,
    int axis,
    double prval)
{

    if (sdchkjaxis(13,joint,axis) != 0) {
        return;
    }
    if (roustate != 2) {
        sdseterr(13,23);
        return;
    }
    if (pres[sdindx(joint,axis)]  !=  0.) {
        uacc[sdindx(joint,axis)] = prval;
    }
}

void sdpresvel(int joint,
    int axis,
    double prval)
{

    if (sdchkjaxis(14,joint,axis) != 0) {
        return;
    }
    if (roustate != 2) {
        sdseterr(14,23);
        return;
    }
    if (pres[sdindx(joint,axis)]  !=  0.) {
        uvel[sdindx(joint,axis)] = prval;
    }
}

void sdprespos(int joint,
    int axis,
    double prval)
{

    if (sdchkjaxis(15,joint,axis) != 0) {
        return;
    }
    if (roustate != 2) {
        sdseterr(15,23);
        return;
    }
    if (pres[sdindx(joint,axis)]  !=  0.) {
        upos[sdindx(joint,axis)] = prval;
    }
}

void sdgetht(int joint,
    int axis,
    double *torque)
{

    if (sdchkjaxis(30,joint,axis) != 0) {
        return;
    }
    if (roustate != 3) {
        sdseterr(30,24);
        return;
    }
    *torque = tauc[sdindx(joint,axis)];
}

void sdhinget(int joint,
    int axis,
    double torque)
{

    if (sdchkjaxis(10,joint,axis) != 0) {
        return;
    }
    if (roustate != 2) {
        sdseterr(10,23);
        return;
    }
    if (mfrcflg != 0) {
        mtau[sdindx(joint,axis)] = mtau[sdindx(joint,axis)]+torque;
    } else {
        fs0flg = 0;
        utau[sdindx(joint,axis)] = utau[sdindx(joint,axis)]+torque;
    }
}

void sdpointf(int body,
    double point[3],
    double force[3])
{
    double torque[3];

    if (sdchkbnum(11,body) != 0) {
        return;
    }
    if (roustate != 2) {
        sdseterr(11,23);
        return;
    }
    if (body == -1) {
        return;
    }
    torque[0] = point[1]*force[2]-point[2]*force[1];
    torque[1] = point[2]*force[0]-point[0]*force[2];
    torque[2] = point[0]*force[1]-point[1]*force[0];
    if (mfrcflg != 0) {
        mfk[body][0] = mfk[body][0]+force[0];
        mtk[body][0] = mtk[body][0]+torque[0];
        mfk[body][1] = mfk[body][1]+force[1];
        mtk[body][1] = mtk[body][1]+torque[1];
        mfk[body][2] = mfk[body][2]+force[2];
        mtk[body][2] = mtk[body][2]+torque[2];
    } else {
        fs0flg = 0;
        ufk[body][0] = ufk[body][0]+force[0];
        utk[body][0] = utk[body][0]+torque[0];
        ufk[body][1] = ufk[body][1]+force[1];
        utk[body][1] = utk[body][1]+torque[1];
        ufk[body][2] = ufk[body][2]+force[2];
        utk[body][2] = utk[body][2]+torque[2];
    }
}

void sdbodyt(int body,
    double torque[3])
{

    if (sdchkbnum(12,body) != 0) {
        return;
    }
    if (roustate != 2) {
        sdseterr(12,23);
        return;
    }
    if (body == -1) {
        return;
    }
    if (mfrcflg != 0) {
        mtk[body][0] = mtk[body][0]+torque[0];
        mtk[body][1] = mtk[body][1]+torque[1];
        mtk[body][2] = mtk[body][2]+torque[2];
    } else {
        fs0flg = 0;
        utk[body][0] = utk[body][0]+torque[0];
        utk[body][1] = utk[body][1]+torque[1];
        utk[body][2] = utk[body][2]+torque[2];
    }
}

void sddoww(int routine)
{
    double pp[12][25],dpp[25][12];
    int i,j,c;
    double sum;
    double dfk[15][3],dtk[15][3],dtau[25],dltci[1][3],dltc[1][3],dlfci[1][3],
      dlfc[1][3];
    double dTinb[1][3],dToutb[1][3],dltaufi[1][3],dltaufo[1][3],dltauti[1][3],
      dltauto[1][3];
    double umult[10];
    double dfs[25],row[25],dinvrow[25];

    roustate = 2;
    if (wwflg == 0) {
/*
Compute constraint effects
*/
        sddovpk();
        sddommldu(routine);
/*
Constraint 0 (prescribed motion)
*/
        dfs[0] = 0.;
        dfs[1] = 0.;
        dfs[2] = 0.;
        dfs[3] = 0.;
        dfs[4] = 0.;
        dfs[5] = 0.;
        dfs[6] = 0.;
        dfs[7] = 0.;
        dfs[8] = 0.;
        dfs[9] = 0.;
        dfs[10] = 0.;
        dfs[11] = 1.;
        dfs[12] = 0.;
        dfs[13] = 0.;
        dfs[14] = 0.;
        dfs[15] = 0.;
        dfs[16] = 0.;
        dfs[17] = 0.;
        dfs[18] = 0.;
        dfs[19] = 0.;
        dfs[20] = 0.;
        dfs[21] = 0.;
        dfs[22] = 0.;
        dfs[23] = 0.;
        dfs[24] = 0.;
        sdldubsl(25,25,mmap,mlo,dfs,row);
        sdldubsd(25,25,mmap,mdi,row,dinvrow);
        for (i = 0; i <= 24; i++) {
            pp[0][i] = row[i];
            dpp[i][0] = dinvrow[i];
        }
        wmap[0] = 0;
/*
Constraint 1 (prescribed motion)
*/
        dfs[0] = 0.;
        dfs[1] = 0.;
        dfs[2] = 0.;
        dfs[3] = 0.;
        dfs[4] = 0.;
        dfs[5] = 0.;
        dfs[6] = 0.;
        dfs[7] = 0.;
        dfs[8] = 0.;
        dfs[9] = 0.;
        dfs[10] = 0.;
        dfs[11] = 0.;
        dfs[12] = 0.;
        dfs[13] = 0.;
        dfs[14] = 0.;
        dfs[15] = 0.;
        dfs[16] = 0.;
        dfs[17] = 0.;
        dfs[18] = 0.;
        dfs[19] = 0.;
        dfs[20] = 0.;
        dfs[21] = 0.;
        dfs[22] = 1.;
        dfs[23] = 0.;
        dfs[24] = 0.;
        sdldubsl(25,25,mmap,mlo,dfs,row);
        sdldubsd(25,25,mmap,mdi,row,dinvrow);
        for (i = 0; i <= 24; i++) {
            pp[1][i] = row[i];
            dpp[i][1] = dinvrow[i];
        }
        wmap[1] = 1;
/*
Constraint 2 (user constraint)
*/
/*
Initialize all multiplier forces to zero.
*/
        for (i = 0; i <= 14; i++) {
            for (j = 0; j <= 2; j++) {
                mfk[i][j] = 0.;
                mtk[i][j] = 0.;
            }
        }
        for (i = 0; i <= 24; i++) {
            mtau[i] = 0.;
        }
/*
Compute user-generated multiplier forces
*/
        umult[0] = 1.;
        umult[1] = 0.;
        umult[2] = 0.;
        umult[3] = 0.;
        umult[4] = 0.;
        umult[5] = 0.;
        umult[6] = 0.;
        umult[7] = 0.;
        umult[8] = 0.;
        umult[9] = 0.;
        mfrcflg = 1;
        sduconsfrc(curtim,q,u,umult);
        mfrcflg = 0;
        sdfsmult();
        sdldubsl(25,25,mmap,mlo,fs,row);
        sdldubsd(25,25,mmap,mdi,row,dinvrow);
        for (i = 0; i <= 24; i++) {
            pp[2][i] = row[i];
            dpp[i][2] = dinvrow[i];
        }
        wmap[2] = 2;
/*
Constraint 3 (user constraint)
*/
/*
Initialize all multiplier forces to zero.
*/
        for (i = 0; i <= 14; i++) {
            for (j = 0; j <= 2; j++) {
                mfk[i][j] = 0.;
                mtk[i][j] = 0.;
            }
        }
        for (i = 0; i <= 24; i++) {
            mtau[i] = 0.;
        }
/*
Compute user-generated multiplier forces
*/
        umult[0] = 0.;
        umult[1] = 1.;
        umult[2] = 0.;
        umult[3] = 0.;
        umult[4] = 0.;
        umult[5] = 0.;
        umult[6] = 0.;
        umult[7] = 0.;
        umult[8] = 0.;
        umult[9] = 0.;
        mfrcflg = 1;
        sduconsfrc(curtim,q,u,umult);
        mfrcflg = 0;
        sdfsmult();
        sdldubsl(25,25,mmap,mlo,fs,row);
        sdldubsd(25,25,mmap,mdi,row,dinvrow);
        for (i = 0; i <= 24; i++) {
            pp[3][i] = row[i];
            dpp[i][3] = dinvrow[i];
        }
        wmap[3] = 3;
/*
Constraint 4 (user constraint)
*/
/*
Initialize all multiplier forces to zero.
*/
        for (i = 0; i <= 14; i++) {
            for (j = 0; j <= 2; j++) {
                mfk[i][j] = 0.;
                mtk[i][j] = 0.;
            }
        }
        for (i = 0; i <= 24; i++) {
            mtau[i] = 0.;
        }
/*
Compute user-generated multiplier forces
*/
        umult[0] = 0.;
        umult[1] = 0.;
        umult[2] = 1.;
        umult[3] = 0.;
        umult[4] = 0.;
        umult[5] = 0.;
        umult[6] = 0.;
        umult[7] = 0.;
        umult[8] = 0.;
        umult[9] = 0.;
        mfrcflg = 1;
        sduconsfrc(curtim,q,u,umult);
        mfrcflg = 0;
        sdfsmult();
        sdldubsl(25,25,mmap,mlo,fs,row);
        sdldubsd(25,25,mmap,mdi,row,dinvrow);
        for (i = 0; i <= 24; i++) {
            pp[4][i] = row[i];
            dpp[i][4] = dinvrow[i];
        }
        wmap[4] = 4;
/*
Constraint 5 (user constraint)
*/
/*
Initialize all multiplier forces to zero.
*/
        for (i = 0; i <= 14; i++) {
            for (j = 0; j <= 2; j++) {
                mfk[i][j] = 0.;
                mtk[i][j] = 0.;
            }
        }
        for (i = 0; i <= 24; i++) {
            mtau[i] = 0.;
        }
/*
Compute user-generated multiplier forces
*/
        umult[0] = 0.;
        umult[1] = 0.;
        umult[2] = 0.;
        umult[3] = 1.;
        umult[4] = 0.;
        umult[5] = 0.;
        umult[6] = 0.;
        umult[7] = 0.;
        umult[8] = 0.;
        umult[9] = 0.;
        mfrcflg = 1;
        sduconsfrc(curtim,q,u,umult);
        mfrcflg = 0;
        sdfsmult();
        sdldubsl(25,25,mmap,mlo,fs,row);
        sdldubsd(25,25,mmap,mdi,row,dinvrow);
        for (i = 0; i <= 24; i++) {
            pp[5][i] = row[i];
            dpp[i][5] = dinvrow[i];
        }
        wmap[5] = 5;
/*
Constraint 6 (user constraint)
*/
/*
Initialize all multiplier forces to zero.
*/
        for (i = 0; i <= 14; i++) {
            for (j = 0; j <= 2; j++) {
                mfk[i][j] = 0.;
                mtk[i][j] = 0.;
            }
        }
        for (i = 0; i <= 24; i++) {
            mtau[i] = 0.;
        }
/*
Compute user-generated multiplier forces
*/
        umult[0] = 0.;
        umult[1] = 0.;
        umult[2] = 0.;
        umult[3] = 0.;
        umult[4] = 1.;
        umult[5] = 0.;
        umult[6] = 0.;
        umult[7] = 0.;
        umult[8] = 0.;
        umult[9] = 0.;
        mfrcflg = 1;
        sduconsfrc(curtim,q,u,umult);
        mfrcflg = 0;
        sdfsmult();
        sdldubsl(25,25,mmap,mlo,fs,row);
        sdldubsd(25,25,mmap,mdi,row,dinvrow);
        for (i = 0; i <= 24; i++) {
            pp[6][i] = row[i];
            dpp[i][6] = dinvrow[i];
        }
        wmap[6] = 6;
/*
Constraint 7 (user constraint)
*/
/*
Initialize all multiplier forces to zero.
*/
        for (i = 0; i <= 14; i++) {
            for (j = 0; j <= 2; j++) {
                mfk[i][j] = 0.;
                mtk[i][j] = 0.;
            }
        }
        for (i = 0; i <= 24; i++) {
            mtau[i] = 0.;
        }
/*
Compute user-generated multiplier forces
*/
        umult[0] = 0.;
        umult[1] = 0.;
        umult[2] = 0.;
        umult[3] = 0.;
        umult[4] = 0.;
        umult[5] = 1.;
        umult[6] = 0.;
        umult[7] = 0.;
        umult[8] = 0.;
        umult[9] = 0.;
        mfrcflg = 1;
        sduconsfrc(curtim,q,u,umult);
        mfrcflg = 0;
        sdfsmult();
        sdldubsl(25,25,mmap,mlo,fs,row);
        sdldubsd(25,25,mmap,mdi,row,dinvrow);
        for (i = 0; i <= 24; i++) {
            pp[7][i] = row[i];
            dpp[i][7] = dinvrow[i];
        }
        wmap[7] = 7;
/*
Constraint 8 (user constraint)
*/
/*
Initialize all multiplier forces to zero.
*/
        for (i = 0; i <= 14; i++) {
            for (j = 0; j <= 2; j++) {
                mfk[i][j] = 0.;
                mtk[i][j] = 0.;
            }
        }
        for (i = 0; i <= 24; i++) {
            mtau[i] = 0.;
        }
/*
Compute user-generated multiplier forces
*/
        umult[0] = 0.;
        umult[1] = 0.;
        umult[2] = 0.;
        umult[3] = 0.;
        umult[4] = 0.;
        umult[5] = 0.;
        umult[6] = 1.;
        umult[7] = 0.;
        umult[8] = 0.;
        umult[9] = 0.;
        mfrcflg = 1;
        sduconsfrc(curtim,q,u,umult);
        mfrcflg = 0;
        sdfsmult();
        sdldubsl(25,25,mmap,mlo,fs,row);
        sdldubsd(25,25,mmap,mdi,row,dinvrow);
        for (i = 0; i <= 24; i++) {
            pp[8][i] = row[i];
            dpp[i][8] = dinvrow[i];
        }
        wmap[8] = 8;
/*
Constraint 9 (user constraint)
*/
/*
Initialize all multiplier forces to zero.
*/
        for (i = 0; i <= 14; i++) {
            for (j = 0; j <= 2; j++) {
                mfk[i][j] = 0.;
                mtk[i][j] = 0.;
            }
        }
        for (i = 0; i <= 24; i++) {
            mtau[i] = 0.;
        }
/*
Compute user-generated multiplier forces
*/
        umult[0] = 0.;
        umult[1] = 0.;
        umult[2] = 0.;
        umult[3] = 0.;
        umult[4] = 0.;
        umult[5] = 0.;
        umult[6] = 0.;
        umult[7] = 1.;
        umult[8] = 0.;
        umult[9] = 0.;
        mfrcflg = 1;
        sduconsfrc(curtim,q,u,umult);
        mfrcflg = 0;
        sdfsmult();
        sdldubsl(25,25,mmap,mlo,fs,row);
        sdldubsd(25,25,mmap,mdi,row,dinvrow);
        for (i = 0; i <= 24; i++) {
            pp[9][i] = row[i];
            dpp[i][9] = dinvrow[i];
        }
        wmap[9] = 9;
/*
Constraint 10 (user constraint)
*/
/*
Initialize all multiplier forces to zero.
*/
        for (i = 0; i <= 14; i++) {
            for (j = 0; j <= 2; j++) {
                mfk[i][j] = 0.;
                mtk[i][j] = 0.;
            }
        }
        for (i = 0; i <= 24; i++) {
            mtau[i] = 0.;
        }
/*
Compute user-generated multiplier forces
*/
        umult[0] = 0.;
        umult[1] = 0.;
        umult[2] = 0.;
        umult[3] = 0.;
        umult[4] = 0.;
        umult[5] = 0.;
        umult[6] = 0.;
        umult[7] = 0.;
        umult[8] = 1.;
        umult[9] = 0.;
        mfrcflg = 1;
        sduconsfrc(curtim,q,u,umult);
        mfrcflg = 0;
        sdfsmult();
        sdldubsl(25,25,mmap,mlo,fs,row);
        sdldubsd(25,25,mmap,mdi,row,dinvrow);
        for (i = 0; i <= 24; i++) {
            pp[10][i] = row[i];
            dpp[i][10] = dinvrow[i];
        }
        wmap[10] = 10;
/*
Constraint 11 (user constraint)
*/
/*
Initialize all multiplier forces to zero.
*/
        for (i = 0; i <= 14; i++) {
            for (j = 0; j <= 2; j++) {
                mfk[i][j] = 0.;
                mtk[i][j] = 0.;
            }
        }
        for (i = 0; i <= 24; i++) {
            mtau[i] = 0.;
        }
/*
Compute user-generated multiplier forces
*/
        umult[0] = 0.;
        umult[1] = 0.;
        umult[2] = 0.;
        umult[3] = 0.;
        umult[4] = 0.;
        umult[5] = 0.;
        umult[6] = 0.;
        umult[7] = 0.;
        umult[8] = 0.;
        umult[9] = 1.;
        mfrcflg = 1;
        sduconsfrc(curtim,q,u,umult);
        mfrcflg = 0;
        sdfsmult();
        sdldubsl(25,25,mmap,mlo,fs,row);
        sdldubsd(25,25,mmap,mdi,row,dinvrow);
        for (i = 0; i <= 24; i++) {
            pp[11][i] = row[i];
            dpp[i][11] = dinvrow[i];
        }
        wmap[11] = 11;
/*
Produce constraint coefficient matrix WW
*/
        for (c = 0; c <= 11; c++) {
            for (i = c; i <= 11; i++) {
                sum = 0.;
                for (j = 0; j <= 24; j++) {
                    sum = sum+pp[wmap[c]][j]*dpp[j][wmap[i]];
                }
                ww[wmap[c]][wmap[i]] = sum;
                ww[wmap[i]][wmap[c]] = sum;
            }
        }
/*
Form QR decomposition of WW
*/
        sdqrdcomp(12,12,12,12,wmap,wmap,ww,qraux,jpvt);
        wwflg = 1;
    }
/*
 Used 0.02 seconds CPU time,
 0 additional bytes of memory.
 Equations contain 1950 adds/subtracts/negates
                   1950 multiplies
                      0 divides
                   4084 assignments
*/
}

void sdxudot0(int routine,
    double oudot0[25])
{
/*
Compute unconstrained equations
*/
    int i;

    sdlhs(routine);
/*
Solve equations ignoring constraints
*/
    sdfs0();
    sdldubslv(25,25,mmap,works,mlo,mdi,fs,udot);
    for (i = 0; i <= 24; i++) {
        oudot0[i] = udot[i];
    }
/*
 Used -0.00 seconds CPU time,
 0 additional bytes of memory.
 Equations contain    0 adds/subtracts/negates
                      0 multiplies
                      0 divides
                     25 assignments
*/
}

void sdudot0(double oudot0[25])
{

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(66,23);
        return;
    }
    sdxudot0(66,oudot0);
}

void sdsetudot(double iudot[25])
{
/*
Assign udots and advance to stage Dynamics Ready
*/
    int i;

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(68,23);
        return;
    }
    for (i = 0; i <= 24; i++) {
        udot[i] = iudot[i];
    }
    sdrhs();
}

void sdxudotm(int routine,
    double imult[12],
    double oudotm[25])
{
/*
Compute udots due only to multipliers
*/
    int i;

    sdlhs(routine);
    sdmfrc(imult);
    sdfsmult();
    sdldubslv(25,25,mmap,works,mlo,mdi,fs,udot);
    for (i = 0; i <= 24; i++) {
        oudotm[i] = udot[i];
    }
/*
 Used -0.00 seconds CPU time,
 0 additional bytes of memory.
 Equations contain    0 adds/subtracts/negates
                      0 multiplies
                      0 divides
                     25 assignments
*/
}

void sdudotm(double imult[12],
    double oudotm[25])
{

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(67,23);
        return;
    }
    sdxudotm(67,imult,oudotm);
}

void sdderiv(double oqdot[25],
    double oudot[25])
{
/*
This is the derivative section for a 15-body ground-based
system with 25 hinge degree(s) of freedom.
2 of the degrees of freedom follow(s) prescribed motion.
There are 12 constraints.
*/
    double workr[12],bb[12],b0[12],v0[12],p0[12];
    int iwork[12];
    int i,j;
    double udot0[25],udot1[25];

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(17,23);
        return;
    }
    if (stabvelq == 1) {
        sdseterr(17,32);
    }
    if (stabposq == 1) {
        sdseterr(17,33);
    }
    wsiz = 12;
/*
Compute unconstrained equations
*/
    sdxudot0(17,udot0);
    sdrhs();
    sdaerr(b0);
    if (stabvel  !=  0.) {
        sdverr(v0);
    }
    if (stabpos  !=  0.) {
        sdperr(p0);
    }
/*
Stabilize constraints using Baumgarte's method
*/
    for (i = 0; i <= 11; i++) {
        bb[i] = -b0[i];
    }
    if (stabvel  !=  0.) {
        for (i = 0; i <= 11; i++) {
            bb[i] = bb[i]-stabvel*v0[i];
        }
    }
    if (stabpos  !=  0.) {
        for (i = 0; i <= 11; i++) {
            bb[i] = bb[i]-stabpos*p0[i];
        }
    }
/*
Compute and decompose constraint matrix WW
*/
    sddoww(17);
/*
Numerically solve for constraint multipliers
*/
    sdqrbslv(12,12,12,12,wmap,wmap,1e-13,workr,iwork,ww,qraux,jpvt,bb,mult,&
      wrank);
    for (i = 0; i <= 11; i++) {
        multmap[i] = 0;
    }
    for (i = 0; i < wrank; i++) {
        multmap[jpvt[i]] = 1;
    }
    j = 0;
    for (i = 0; i <= 11; i++) {
        if (multmap[i] != 0) {
            multmap[j] = wmap[i];
            j = j+1;
        }
    }
/*
Compute final udots
*/
    sdxudotm(17,mult,udot1);
    for (i = 0; i <= 24; i++) {
        udot[i] = udot0[i]+udot1[i];
    }
    sdrhs();
    for (i = 0; i <= 24; i++) {
        oqdot[i] = qdot[i];
    }
    for (i = 0; i <= 24; i++) {
        oudot[i] = udot[i];
    }
/*
 Used -0.00 seconds CPU time,
 0 additional bytes of memory.
 Equations contain   61 adds/subtracts/negates
                     24 multiplies
                      0 divides
                    111 assignments
*/
}
/*
Compute residuals for use with DAE integrator
*/

void sdresid(double eqdot[25],
    double eudot[25],
    double emults[12],
    double resid[62])
{
    int i;
    double uderrs[25],p0[12];

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(16,23);
        return;
    }
    if (stabposq == 1) {
        sdseterr(16,33);
    }
    sdfulltrq(eudot,emults,uderrs);
    for (i = 0; i < 25; i++) {
        resid[i] = eqdot[i]-qdot[i];
    }
    for (i = 0; i < 25; i++) {
        resid[25+i] = uderrs[i];
    }
    sdverr(&resid[50]);
    if (stabpos  !=  0.) {
        sdperr(p0);
        for (i = 0; i < 12; i++) {
            resid[50+i] = resid[50+i]+stabpos*p0[i];
        }
    }
    for (i = 0; i < 25; i++) {
        udot[i] = eudot[i];
    }
    for (i = 0; i < 12; i++) {
        mult[i] = emults[i];
    }
    sdrhs();
/*
 Used -0.00 seconds CPU time,
 0 additional bytes of memory.
 Equations contain   37 adds/subtracts/negates
                     12 multiplies
                      0 divides
                     99 assignments
*/
}

void sdmult(double omults[12],
    int *owrank,
    int omultmap[12])
{
    int i;

    if (roustate != 3) {
        sdseterr(34,24);
        return;
    }
    for (i = 0; i < 12; i++) {
        omults[i] = mult[i];
        if (i <= wrank-1) {
            omultmap[i] = multmap[i];
        } else {
            omultmap[i] = -1;
        }
    }
    *owrank = wrank;
}

void sdreac(double force[15][3],
    double torque[15][3])
{
/*
Generated 22-Jun-2007 09:35:41 by SD/FAST, Kane's formulation
(sdfast B.2.8 #30123) on machine ID unknown
Copyright (c) 1990-1997 Symbolic Dynamics, Inc.
Copyright (c) 1990-1997 Parametric Technology Corp.
RESTRICTED RIGHTS LEGEND: Use, duplication, or disclosure by the U.S.
Government is subject to restrictions as set forth in subparagraph
(c)(1)(ii) of the Rights in Technical Data and Computer Software
clause at DFARS 52.227-7013 and similar clauses in the FAR and NASA
FAR Supplement.  Symbolic Dynamics, Inc., Mountain View, CA 94041
*/

    if (roustate != 3) {
        sdseterr(31,24);
        return;
    }
/*
Compute reaction forces for non-weld tree joints
*/
    ffkb[0][0] = (mfk[0][0]+ufk[0][0]);
    ffkb[0][1] = (mfk[0][1]+ufk[0][1]);
    ffkb[0][2] = (mfk[0][2]+ufk[0][2]);
    ffkb[1][0] = (mfk[1][0]+ufk[1][0]);
    ffkb[1][1] = (mfk[1][1]+ufk[1][1]);
    ffkb[1][2] = (mfk[1][2]+ufk[1][2]);
    ffkb[2][0] = (mfk[2][0]+ufk[2][0]);
    ffkb[2][1] = (mfk[2][1]+ufk[2][1]);
    ffkb[2][2] = (mfk[2][2]+ufk[2][2]);
    ffkb[3][0] = (mfk[3][0]+ufk[3][0]);
    ffkb[3][1] = (mfk[3][1]+ufk[3][1]);
    ffkb[3][2] = (mfk[3][2]+ufk[3][2]);
    ffkb[4][0] = (mfk[4][0]+ufk[4][0]);
    ffkb[4][1] = (mfk[4][1]+ufk[4][1]);
    ffkb[4][2] = (mfk[4][2]+ufk[4][2]);
    ffkb[5][0] = (mfk[5][0]+ufk[5][0]);
    ffkb[5][1] = (mfk[5][1]+ufk[5][1]);
    ffkb[5][2] = (mfk[5][2]+ufk[5][2]);
    ffkb[6][0] = (mfk[6][0]+ufk[6][0]);
    ffkb[6][1] = (mfk[6][1]+ufk[6][1]);
    ffkb[6][2] = (mfk[6][2]+ufk[6][2]);
    ffkb[7][0] = (mfk[7][0]+ufk[7][0]);
    ffkb[7][1] = (mfk[7][1]+ufk[7][1]);
    ffkb[7][2] = (mfk[7][2]+ufk[7][2]);
    ffkb[8][0] = (mfk[8][0]+ufk[8][0]);
    ffkb[8][1] = (mfk[8][1]+ufk[8][1]);
    ffkb[8][2] = (mfk[8][2]+ufk[8][2]);
    ffkb[9][0] = (mfk[9][0]+ufk[9][0]);
    ffkb[9][1] = (mfk[9][1]+ufk[9][1]);
    ffkb[9][2] = (mfk[9][2]+ufk[9][2]);
    ffkb[10][0] = (mfk[10][0]+ufk[10][0]);
    ffkb[10][1] = (mfk[10][1]+ufk[10][1]);
    ffkb[10][2] = (mfk[10][2]+ufk[10][2]);
    ffkb[11][0] = (mfk[11][0]+ufk[11][0]);
    ffkb[11][1] = (mfk[11][1]+ufk[11][1]);
    ffkb[11][2] = (mfk[11][2]+ufk[11][2]);
    ffkb[12][0] = (mfk[12][0]+ufk[12][0]);
    ffkb[12][1] = (mfk[12][1]+ufk[12][1]);
    ffkb[12][2] = (mfk[12][2]+ufk[12][2]);
    ffkb[13][0] = (mfk[13][0]+ufk[13][0]);
    ffkb[13][1] = (mfk[13][1]+ufk[13][1]);
    ffkb[13][2] = (mfk[13][2]+ufk[13][2]);
    ffkb[14][0] = (mfk[14][0]+ufk[14][0]);
    ffkb[14][1] = (mfk[14][1]+ufk[14][1]);
    ffkb[14][2] = (mfk[14][2]+ufk[14][2]);
    ttkb[0][0] = (mtk[0][0]+utk[0][0]);
    ttkb[0][1] = (mtk[0][1]+utk[0][1]);
    ttkb[0][2] = (mtk[0][2]+utk[0][2]);
    ttkb[1][0] = (mtk[1][0]+utk[1][0]);
    ttkb[1][1] = (mtk[1][1]+utk[1][1]);
    ttkb[1][2] = (mtk[1][2]+utk[1][2]);
    ttkb[2][0] = (mtk[2][0]+utk[2][0]);
    ttkb[2][1] = (mtk[2][1]+utk[2][1]);
    ttkb[2][2] = (mtk[2][2]+utk[2][2]);
    ttkb[3][0] = (mtk[3][0]+utk[3][0]);
    ttkb[3][1] = (mtk[3][1]+utk[3][1]);
    ttkb[3][2] = (mtk[3][2]+utk[3][2]);
    ttkb[4][0] = (mtk[4][0]+utk[4][0]);
    ttkb[4][1] = (mtk[4][1]+utk[4][1]);
    ttkb[4][2] = (mtk[4][2]+utk[4][2]);
    ttkb[5][0] = (mtk[5][0]+utk[5][0]);
    ttkb[5][1] = (mtk[5][1]+utk[5][1]);
    ttkb[5][2] = (mtk[5][2]+utk[5][2]);
    ttkb[6][0] = (mtk[6][0]+utk[6][0]);
    ttkb[6][1] = (mtk[6][1]+utk[6][1]);
    ttkb[6][2] = (mtk[6][2]+utk[6][2]);
    ttkb[7][0] = (mtk[7][0]+utk[7][0]);
    ttkb[7][1] = (mtk[7][1]+utk[7][1]);
    ttkb[7][2] = (mtk[7][2]+utk[7][2]);
    ttkb[8][0] = (mtk[8][0]+utk[8][0]);
    ttkb[8][1] = (mtk[8][1]+utk[8][1]);
    ttkb[8][2] = (mtk[8][2]+utk[8][2]);
    ttkb[9][0] = (mtk[9][0]+utk[9][0]);
    ttkb[9][1] = (mtk[9][1]+utk[9][1]);
    ttkb[9][2] = (mtk[9][2]+utk[9][2]);
    ttkb[10][0] = (mtk[10][0]+utk[10][0]);
    ttkb[10][1] = (mtk[10][1]+utk[10][1]);
    ttkb[10][2] = (mtk[10][2]+utk[10][2]);
    ttkb[11][0] = (mtk[11][0]+utk[11][0]);
    ttkb[11][1] = (mtk[11][1]+utk[11][1]);
    ttkb[11][2] = (mtk[11][2]+utk[11][2]);
    ttkb[12][0] = (mtk[12][0]+utk[12][0]);
    ttkb[12][1] = (mtk[12][1]+utk[12][1]);
    ttkb[12][2] = (mtk[12][2]+utk[12][2]);
    ttkb[13][0] = (mtk[13][0]+utk[13][0]);
    ttkb[13][1] = (mtk[13][1]+utk[13][1]);
    ttkb[13][2] = (mtk[13][2]+utk[13][2]);
    ttkb[14][0] = (mtk[14][0]+utk[14][0]);
    ttkb[14][1] = (mtk[14][1]+utk[14][1]);
    ttkb[14][2] = (mtk[14][2]+utk[14][2]);
    fc[24][0] = ((.2058*(AnkAtk[24][0]-gk[24][0]))-ffkb[14][0]);
    fc[24][1] = ((.2058*(AnkAtk[24][1]-gk[24][1]))-ffkb[14][1]);
    fc[24][2] = ((.2058*(AnkAtk[24][2]-gk[22][2]))-ffkb[14][2]);
    tc[24][0] = ((.001*onk[24][0])-(ttkb[14][0]+((.0175*fc[24][1])-(.006*
      fc[24][2]))));
    tc[24][1] = ((.001*onk[24][1])-(ttkb[14][1]+((.0346*fc[24][2])-(.0175*
      fc[24][0]))));
    tc[24][2] = ((.001*onk[24][2])-(ttkb[14][2]+((.006*fc[24][0])-(.0346*
      fc[24][1]))));
    fccikt[24][0] = ((fc[24][0]*c24)-(fc[24][1]*s24));
    fccikt[24][1] = ((fc[24][0]*s24)+(fc[24][1]*c24));
    fccikt[24][2] = fc[24][2];
    ffk[23][0] = (ffkb[13][0]-fccikt[24][0]);
    ffk[23][1] = (ffkb[13][1]-fccikt[24][1]);
    ffk[23][2] = (ffkb[13][2]-fccikt[24][2]);
    ttk[23][0] = (ttkb[13][0]-(((.00108*fccikt[24][1])-(.017*fccikt[24][2]))+((
      tc[24][0]*c24)-(tc[24][1]*s24))));
    ttk[23][1] = (ttkb[13][1]-(((tc[24][0]*s24)+(tc[24][1]*c24))-((.00108*
      fccikt[24][0])+(.0346*fccikt[24][2]))));
    ttk[23][2] = (ttkb[13][2]-(tc[24][2]+((.017*fccikt[24][0])+(.0346*
      fccikt[24][1]))));
    fc[23][0] = ((.4*(AnkAtk[23][0]-gk[23][0]))-ffk[23][0]);
    fc[23][1] = ((.4*(AnkAtk[23][1]-gk[23][1]))-ffk[23][1]);
    fc[23][2] = ((.4*(AnkAtk[23][2]-gk[22][2]))-ffk[23][2]);
    tc[23][0] = ((.001*onk[23][0])-(ttk[23][0]-(.015*fc[23][2])));
    tc[23][1] = ((.001*onk[23][1])-(ttk[23][1]+(.025*fc[23][2])));
    tc[23][2] = ((.001*onk[23][2])-(ttk[23][2]+((.015*fc[23][0])-(.025*fc[23][1]
      ))));
    fccikt[23][0] = ((fc[23][0]*c23)-(fc[23][1]*s23));
    fccikt[23][1] = ((fc[23][0]*s23)+(fc[23][1]*c23));
    fccikt[23][2] = fc[23][2];
    ffk[22][0] = (ffkb[12][0]-fccikt[23][0]);
    ffk[22][1] = (ffkb[12][1]-fccikt[23][1]);
    ffk[22][2] = (ffkb[12][2]-fccikt[23][2]);
    ttk[22][0] = (ttkb[12][0]-(((tc[23][0]*c23)-(tc[23][1]*s23))-(.015*
      fccikt[23][2])));
    ttk[22][1] = (ttkb[12][1]-(((tc[23][0]*s23)+(tc[23][1]*c23))-(.0942*
      fccikt[23][2])));
    ttk[22][2] = (ttkb[12][2]-(tc[23][2]+((.015*fccikt[23][0])+(.0942*
      fccikt[23][1]))));
    fc[22][0] = ((.4183*(AnkAtk[22][0]-gk[21][0]))-ffk[22][0]);
    fc[22][1] = ((.4183*(AnkAtk[22][1]-gk[22][1]))-ffk[22][1]);
    fc[22][2] = ((.4183*(AnkAtk[22][2]-gk[22][2]))-ffk[22][2]);
    tc[22][0] = ((.001*udot[22])-(ttk[22][0]-(.015*fc[22][2])));
    tc[22][1] = ((.001*onk[22][1])-(ttk[22][1]+(.025*fc[22][2])));
    tc[22][2] = ((.001*onk[22][2])-(ttk[22][2]+((.015*fc[22][0])-(.025*fc[22][1]
      ))));
    fccikt[22][0] = fc[22][0];
    fccikt[22][1] = ((fc[22][1]*c22)-(fc[22][2]*s22));
    fccikt[22][2] = ((fc[22][1]*s22)+(fc[22][2]*c22));
    ffk[21][0] = (ffkb[11][0]-fccikt[22][0]);
    ffk[21][1] = (ffkb[11][1]-fccikt[22][1]);
    ffk[21][2] = (ffkb[11][2]-fccikt[22][2]);
    ttk[21][0] = (ttkb[11][0]-(tc[22][0]+((.00792*fccikt[22][1])-(.04195*
      fccikt[22][2]))));
    ttk[21][1] = (ttkb[11][1]-(((.04877*fccikt[22][2])-(.00792*fccikt[22][0]))+(
      (tc[22][1]*c22)-(tc[22][2]*s22))));
    ttk[21][2] = (ttkb[11][2]-(((.04195*fccikt[22][0])-(.04877*fccikt[22][1]))+(
      (tc[22][1]*s22)+(tc[22][2]*c22))));
    fc[21][0] = ((.1*(AnkAtk[21][0]-gk[21][0]))-ffk[21][0]);
    fc[21][1] = ((.1*(AnkAtk[21][1]-gk[21][1]))-ffk[21][1]);
    fc[21][2] = -ffk[21][2];
    tc[21][0] = -ttk[21][0];
    tc[21][1] = -ttk[21][1];
    tc[21][2] = ((.001*Onkb[21][2])-ttk[21][2]);
    fccikt[21][0] = ((fc[21][0]*c21)-(fc[21][1]*s21));
    fccikt[21][1] = ((fc[21][0]*s21)+(fc[21][1]*c21));
    fccikt[21][2] = fc[21][2];
    ffk[17][0] = (ffkb[9][0]-fccikt[21][0]);
    ffk[17][1] = (ffkb[9][1]-fccikt[21][1]);
    ffk[17][2] = (ffkb[9][2]-fccikt[21][2]);
    ttk[17][0] = (ttkb[9][0]-(((tc[21][0]*c21)-(tc[21][1]*s21))-(.2433*
      fccikt[21][2])));
    ttk[17][1] = (ttkb[9][1]-((tc[21][0]*s21)+(tc[21][1]*c21)));
    ttk[17][2] = (ttkb[9][2]-(tc[21][2]+(.2433*fccikt[21][0])));
    fc[20][0] = ((.1*(AnkAtk[20][0]-gk[20][0]))-ffkb[10][0]);
    fc[20][1] = ((.1*(AnkAtk[20][1]-gk[20][1]))-ffkb[10][1]);
    fc[20][2] = -ffkb[10][2];
    tc[20][0] = -ttkb[10][0];
    tc[20][1] = -ttkb[10][1];
    tc[20][2] = ((.001*Onkb[20][2])-ttkb[10][2]);
    fccikt[20][0] = ((fc[20][0]*c20)-(fc[20][1]*s20));
    fccikt[20][1] = ((fc[20][0]*s20)+(fc[20][1]*c20));
    fccikt[20][2] = fc[20][2];
    ffk[19][0] = -fccikt[20][0];
    ffk[19][1] = -fccikt[20][1];
    ffk[19][2] = -fccikt[20][2];
    ttk[19][0] = -((tc[20][0]*c20)-(tc[20][1]*s20));
    ttk[19][1] = -((tc[20][0]*s20)+(tc[20][1]*c20));
    ttk[19][2] = -tc[20][2];
    fc[19][0] = -ffk[19][0];
    fc[19][1] = -ffk[19][1];
    fc[19][2] = -ffk[19][2];
    tc[19][0] = -ttk[19][0];
    tc[19][1] = -ttk[19][1];
    tc[19][2] = -ttk[19][2];
    fccikt[19][0] = fc[19][0];
    fccikt[19][1] = fc[19][1];
    fccikt[19][2] = fc[19][2];
    ffk[18][0] = -fccikt[19][0];
    ffk[18][1] = -fccikt[19][1];
    ffk[18][2] = -fccikt[19][2];
    ttk[18][0] = -(tc[19][0]+(fccikt[19][2]*q[19]));
    ttk[18][1] = -tc[19][1];
    ttk[18][2] = -(tc[19][2]-(fccikt[19][0]*q[19]));
    fc[18][0] = -ffk[18][0];
    fc[18][1] = -ffk[18][1];
    fc[18][2] = -ffk[18][2];
    tc[18][0] = -ttk[18][0];
    tc[18][1] = -ttk[18][1];
    tc[18][2] = -ttk[18][2];
    fccikt[18][0] = fc[18][0];
    fccikt[18][1] = fc[18][1];
    fccikt[18][2] = fc[18][2];
    ffk[17][0] = (ffk[17][0]-fccikt[18][0]);
    ffk[17][1] = (ffk[17][1]-fccikt[18][1]);
    ffk[17][2] = (ffk[17][2]-fccikt[18][2]);
    ttk[17][0] = (ttk[17][0]-(tc[18][0]+((.0024*fccikt[18][1])+(.1867*
      fccikt[18][2]))));
    ttk[17][1] = (ttk[17][1]-(tc[18][1]-((.0024*fccikt[18][0])+(fccikt[18][2]*
      q[18]))));
    ttk[17][2] = (ttk[17][2]-(tc[18][2]+((fccikt[18][1]*q[18])-(.1867*
      fccikt[18][0]))));
    fc[17][0] = ((3.03*(AnkAtk[17][0]-gk[17][0]))-ffk[17][0]);
    fc[17][1] = ((3.03*(AnkAtk[17][1]-gk[17][1]))-ffk[17][1]);
    fc[17][2] = -ffk[17][2];
    tc[17][0] = -(ttk[17][0]+(.1867*fc[17][2]));
    tc[17][1] = -ttk[17][1];
    tc[17][2] = ((.0445*Onkb[17][2])-(ttk[17][2]-(.1867*fc[17][0])));
    fccikt[17][0] = ((fc[17][0]*c17)-(fc[17][1]*s17));
    fccikt[17][1] = ((fc[17][0]*s17)+(fc[17][1]*c17));
    fccikt[17][2] = fc[17][2];
    ffk[16][0] = -fccikt[17][0];
    ffk[16][1] = -fccikt[17][1];
    ffk[16][2] = -fccikt[17][2];
    ttk[16][0] = -((tc[17][0]*c17)-(tc[17][1]*s17));
    ttk[16][1] = -((tc[17][0]*s17)+(tc[17][1]*c17));
    ttk[16][2] = -tc[17][2];
    fc[16][0] = -ffk[16][0];
    fc[16][1] = -ffk[16][1];
    fc[16][2] = -ffk[16][2];
    tc[16][0] = -ttk[16][0];
    tc[16][1] = -ttk[16][1];
    tc[16][2] = -ttk[16][2];
    fccikt[16][0] = fc[16][0];
    fccikt[16][1] = fc[16][1];
    fccikt[16][2] = fc[16][2];
    ffk[15][0] = -fccikt[16][0];
    ffk[15][1] = -fccikt[16][1];
    ffk[15][2] = -fccikt[16][2];
    ttk[15][0] = -(tc[16][0]+(fccikt[16][2]*q[16]));
    ttk[15][1] = -tc[16][1];
    ttk[15][2] = -(tc[16][2]-(fccikt[16][0]*q[16]));
    fc[15][0] = -ffk[15][0];
    fc[15][1] = -ffk[15][1];
    fc[15][2] = -ffk[15][2];
    tc[15][0] = -ttk[15][0];
    tc[15][1] = -ttk[15][1];
    tc[15][2] = -ttk[15][2];
    fccikt[15][0] = fc[15][0];
    fccikt[15][1] = fc[15][1];
    fccikt[15][2] = fc[15][2];
    ffk[14][0] = (ffkb[8][0]-fccikt[15][0]);
    ffk[14][1] = (ffkb[8][1]-fccikt[15][1]);
    ffk[14][2] = (ffkb[8][2]-fccikt[15][2]);
    ttk[14][0] = (ttkb[8][0]-(tc[15][0]+(.17*fccikt[15][2])));
    ttk[14][1] = (ttkb[8][1]-(tc[15][1]-(fccikt[15][2]*q[15])));
    ttk[14][2] = (ttkb[8][2]-(tc[15][2]+((fccikt[15][1]*q[15])-(.17*
      fccikt[15][0]))));
    fc[14][0] = ((7.76*(AnkAtk[14][0]-gk[14][0]))-ffk[14][0]);
    fc[14][1] = ((7.76*(AnkAtk[14][1]-gk[14][1]))-ffk[14][1]);
    fc[14][2] = -ffk[14][2];
    tc[14][0] = -(ttk[14][0]+(.17*fc[14][2]));
    tc[14][1] = -ttk[14][1];
    tc[14][2] = ((.137*Onkb[14][2])-(ttk[14][2]-(.17*fc[14][0])));
    fccikt[14][0] = ((fc[14][0]*c14)-(fc[14][1]*s14));
    fccikt[14][1] = ((fc[14][0]*s14)+(fc[14][1]*c14));
    fccikt[14][2] = fc[14][2];
    ffk[2][0] = (ffkb[0][0]-fccikt[14][0]);
    ffk[2][1] = (ffkb[0][1]-fccikt[14][1]);
    ffk[2][2] = (ffkb[0][2]-fccikt[14][2]);
    ttk[2][0] = (ttkb[0][0]-(((.0835*fccikt[14][1])-(.33*fccikt[14][2]))+((
      tc[14][0]*c14)-(tc[14][1]*s14))));
    ttk[2][1] = (ttkb[0][1]-(((tc[14][0]*s14)+(tc[14][1]*c14))-(.0835*
      fccikt[14][0])));
    ttk[2][2] = (ttkb[0][2]-(tc[14][2]+(.33*fccikt[14][0])));
    fc[13][0] = ((.2058*(AnkAtk[13][0]-gk[13][0]))-ffkb[7][0]);
    fc[13][1] = ((.2058*(AnkAtk[13][1]-gk[13][1]))-ffkb[7][1]);
    fc[13][2] = ((.2058*(AnkAtk[13][2]-gk[11][2]))-ffkb[7][2]);
    tc[13][0] = ((.001*onk[13][0])-(ttkb[7][0]-((.006*fc[13][2])+(.0175*
      fc[13][1]))));
    tc[13][1] = ((.001*onk[13][1])-(ttkb[7][1]+((.0175*fc[13][0])+(.0346*
      fc[13][2]))));
    tc[13][2] = ((.001*onk[13][2])-(ttkb[7][2]+((.006*fc[13][0])-(.0346*
      fc[13][1]))));
    fccikt[13][0] = ((fc[13][0]*c13)-(fc[13][1]*s13));
    fccikt[13][1] = ((fc[13][0]*s13)+(fc[13][1]*c13));
    fccikt[13][2] = fc[13][2];
    ffk[12][0] = (ffkb[6][0]-fccikt[13][0]);
    ffk[12][1] = (ffkb[6][1]-fccikt[13][1]);
    ffk[12][2] = (ffkb[6][2]-fccikt[13][2]);
    ttk[12][0] = (ttkb[6][0]-(((tc[13][0]*c13)-(tc[13][1]*s13))-((.00108*
      fccikt[13][1])+(.017*fccikt[13][2]))));
    ttk[12][1] = (ttkb[6][1]-(((.00108*fccikt[13][0])-(.0346*fccikt[13][2]))+((
      tc[13][0]*s13)+(tc[13][1]*c13))));
    ttk[12][2] = (ttkb[6][2]-(tc[13][2]+((.017*fccikt[13][0])+(.0346*
      fccikt[13][1]))));
    fc[12][0] = ((.4*(AnkAtk[12][0]-gk[12][0]))-ffk[12][0]);
    fc[12][1] = ((.4*(AnkAtk[12][1]-gk[12][1]))-ffk[12][1]);
    fc[12][2] = ((.4*(AnkAtk[12][2]-gk[11][2]))-ffk[12][2]);
    tc[12][0] = ((.001*onk[12][0])-(ttk[12][0]-(.015*fc[12][2])));
    tc[12][1] = ((.001*onk[12][1])-(ttk[12][1]+(.025*fc[12][2])));
    tc[12][2] = ((.001*onk[12][2])-(ttk[12][2]+((.015*fc[12][0])-(.025*fc[12][1]
      ))));
    fccikt[12][0] = ((fc[12][0]*c12)-(fc[12][1]*s12));
    fccikt[12][1] = ((fc[12][0]*s12)+(fc[12][1]*c12));
    fccikt[12][2] = fc[12][2];
    ffk[11][0] = (ffkb[5][0]-fccikt[12][0]);
    ffk[11][1] = (ffkb[5][1]-fccikt[12][1]);
    ffk[11][2] = (ffkb[5][2]-fccikt[12][2]);
    ttk[11][0] = (ttkb[5][0]-(((tc[12][0]*c12)-(tc[12][1]*s12))-(.015*
      fccikt[12][2])));
    ttk[11][1] = (ttkb[5][1]-(((tc[12][0]*s12)+(tc[12][1]*c12))-(.0942*
      fccikt[12][2])));
    ttk[11][2] = (ttkb[5][2]-(tc[12][2]+((.015*fccikt[12][0])+(.0942*
      fccikt[12][1]))));
    fc[11][0] = ((.4183*(AnkAtk[11][0]-gk[10][0]))-ffk[11][0]);
    fc[11][1] = ((.4183*(AnkAtk[11][1]-gk[11][1]))-ffk[11][1]);
    fc[11][2] = ((.4183*(AnkAtk[11][2]-gk[11][2]))-ffk[11][2]);
    tc[11][0] = ((.001*udot[11])-(ttk[11][0]-(.015*fc[11][2])));
    tc[11][1] = ((.001*onk[11][1])-(ttk[11][1]+(.025*fc[11][2])));
    tc[11][2] = ((.001*onk[11][2])-(ttk[11][2]+((.015*fc[11][0])-(.025*fc[11][1]
      ))));
    fccikt[11][0] = fc[11][0];
    fccikt[11][1] = ((fc[11][1]*c11)-(fc[11][2]*s11));
    fccikt[11][2] = ((fc[11][1]*s11)+(fc[11][2]*c11));
    ffk[10][0] = (ffkb[4][0]-fccikt[11][0]);
    ffk[10][1] = (ffkb[4][1]-fccikt[11][1]);
    ffk[10][2] = (ffkb[4][2]-fccikt[11][2]);
    ttk[10][0] = (ttkb[4][0]-(tc[11][0]-((.00792*fccikt[11][1])+(.04195*
      fccikt[11][2]))));
    ttk[10][1] = (ttkb[4][1]-(((.00792*fccikt[11][0])+(.04877*fccikt[11][2]))+((
      tc[11][1]*c11)-(tc[11][2]*s11))));
    ttk[10][2] = (ttkb[4][2]-(((.04195*fccikt[11][0])-(.04877*fccikt[11][1]))+((
      tc[11][1]*s11)+(tc[11][2]*c11))));
    fc[10][0] = ((.1*(AnkAtk[10][0]-gk[10][0]))-ffk[10][0]);
    fc[10][1] = ((.1*(AnkAtk[10][1]-gk[10][1]))-ffk[10][1]);
    fc[10][2] = -ffk[10][2];
    tc[10][0] = -ttk[10][0];
    tc[10][1] = -ttk[10][1];
    tc[10][2] = ((.001*Onkb[10][2])-ttk[10][2]);
    fccikt[10][0] = ((fc[10][0]*c10)-(fc[10][1]*s10));
    fccikt[10][1] = ((fc[10][0]*s10)+(fc[10][1]*c10));
    fccikt[10][2] = fc[10][2];
    ffk[6][0] = (ffkb[2][0]-fccikt[10][0]);
    ffk[6][1] = (ffkb[2][1]-fccikt[10][1]);
    ffk[6][2] = (ffkb[2][2]-fccikt[10][2]);
    ttk[6][0] = (ttkb[2][0]-(((tc[10][0]*c10)-(tc[10][1]*s10))-(.2433*
      fccikt[10][2])));
    ttk[6][1] = (ttkb[2][1]-((tc[10][0]*s10)+(tc[10][1]*c10)));
    ttk[6][2] = (ttkb[2][2]-(tc[10][2]+(.2433*fccikt[10][0])));
    fc[9][0] = ((.1*(AnkAtk[9][0]-gk[9][0]))-ffkb[3][0]);
    fc[9][1] = ((.1*(AnkAtk[9][1]-gk[9][1]))-ffkb[3][1]);
    fc[9][2] = -ffkb[3][2];
    tc[9][0] = -ttkb[3][0];
    tc[9][1] = -ttkb[3][1];
    tc[9][2] = ((.001*Onkb[9][2])-ttkb[3][2]);
    fccikt[9][0] = ((fc[9][0]*c9)-(fc[9][1]*s9));
    fccikt[9][1] = ((fc[9][0]*s9)+(fc[9][1]*c9));
    fccikt[9][2] = fc[9][2];
    ffk[8][0] = -fccikt[9][0];
    ffk[8][1] = -fccikt[9][1];
    ffk[8][2] = -fccikt[9][2];
    ttk[8][0] = -((tc[9][0]*c9)-(tc[9][1]*s9));
    ttk[8][1] = -((tc[9][0]*s9)+(tc[9][1]*c9));
    ttk[8][2] = -tc[9][2];
    fc[8][0] = -ffk[8][0];
    fc[8][1] = -ffk[8][1];
    fc[8][2] = -ffk[8][2];
    tc[8][0] = -ttk[8][0];
    tc[8][1] = -ttk[8][1];
    tc[8][2] = -ttk[8][2];
    fccikt[8][0] = fc[8][0];
    fccikt[8][1] = fc[8][1];
    fccikt[8][2] = fc[8][2];
    ffk[7][0] = -fccikt[8][0];
    ffk[7][1] = -fccikt[8][1];
    ffk[7][2] = -fccikt[8][2];
    ttk[7][0] = -(tc[8][0]+(fccikt[8][2]*q[8]));
    ttk[7][1] = -tc[8][1];
    ttk[7][2] = -(tc[8][2]-(fccikt[8][0]*q[8]));
    fc[7][0] = -ffk[7][0];
    fc[7][1] = -ffk[7][1];
    fc[7][2] = -ffk[7][2];
    tc[7][0] = -ttk[7][0];
    tc[7][1] = -ttk[7][1];
    tc[7][2] = -ttk[7][2];
    fccikt[7][0] = fc[7][0];
    fccikt[7][1] = fc[7][1];
    fccikt[7][2] = fc[7][2];
    ffk[6][0] = (ffk[6][0]-fccikt[7][0]);
    ffk[6][1] = (ffk[6][1]-fccikt[7][1]);
    ffk[6][2] = (ffk[6][2]-fccikt[7][2]);
    ttk[6][0] = (ttk[6][0]-(tc[7][0]+((.1867*fccikt[7][2])-(.0024*fccikt[7][1]))
      ));
    ttk[6][1] = (ttk[6][1]-(tc[7][1]+((.0024*fccikt[7][0])-(fccikt[7][2]*q[7])))
      );
    ttk[6][2] = (ttk[6][2]-(tc[7][2]+((fccikt[7][1]*q[7])-(.1867*fccikt[7][0])))
      );
    fc[6][0] = ((3.03*(AnkAtk[6][0]-gk[6][0]))-ffk[6][0]);
    fc[6][1] = ((3.03*(AnkAtk[6][1]-gk[6][1]))-ffk[6][1]);
    fc[6][2] = -ffk[6][2];
    tc[6][0] = -(ttk[6][0]+(.1867*fc[6][2]));
    tc[6][1] = -ttk[6][1];
    tc[6][2] = ((.0445*Onkb[6][2])-(ttk[6][2]-(.1867*fc[6][0])));
    fccikt[6][0] = ((fc[6][0]*c6)-(fc[6][1]*s6));
    fccikt[6][1] = ((fc[6][0]*s6)+(fc[6][1]*c6));
    fccikt[6][2] = fc[6][2];
    ffk[5][0] = -fccikt[6][0];
    ffk[5][1] = -fccikt[6][1];
    ffk[5][2] = -fccikt[6][2];
    ttk[5][0] = -((tc[6][0]*c6)-(tc[6][1]*s6));
    ttk[5][1] = -((tc[6][0]*s6)+(tc[6][1]*c6));
    ttk[5][2] = -tc[6][2];
    fc[5][0] = -ffk[5][0];
    fc[5][1] = -ffk[5][1];
    fc[5][2] = -ffk[5][2];
    tc[5][0] = -ttk[5][0];
    tc[5][1] = -ttk[5][1];
    tc[5][2] = -ttk[5][2];
    fccikt[5][0] = fc[5][0];
    fccikt[5][1] = fc[5][1];
    fccikt[5][2] = fc[5][2];
    ffk[4][0] = -fccikt[5][0];
    ffk[4][1] = -fccikt[5][1];
    ffk[4][2] = -fccikt[5][2];
    ttk[4][0] = -(tc[5][0]+(fccikt[5][2]*q[5]));
    ttk[4][1] = -tc[5][1];
    ttk[4][2] = -(tc[5][2]-(fccikt[5][0]*q[5]));
    fc[4][0] = -ffk[4][0];
    fc[4][1] = -ffk[4][1];
    fc[4][2] = -ffk[4][2];
    tc[4][0] = -ttk[4][0];
    tc[4][1] = -ttk[4][1];
    tc[4][2] = -ttk[4][2];
    fccikt[4][0] = fc[4][0];
    fccikt[4][1] = fc[4][1];
    fccikt[4][2] = fc[4][2];
    ffk[3][0] = (ffkb[1][0]-fccikt[4][0]);
    ffk[3][1] = (ffkb[1][1]-fccikt[4][1]);
    ffk[3][2] = (ffkb[1][2]-fccikt[4][2]);
    ttk[3][0] = (ttkb[1][0]-(tc[4][0]+(.17*fccikt[4][2])));
    ttk[3][1] = (ttkb[1][1]-(tc[4][1]-(fccikt[4][2]*q[4])));
    ttk[3][2] = (ttkb[1][2]-(tc[4][2]+((fccikt[4][1]*q[4])-(.17*fccikt[4][0]))))
      ;
    fc[3][0] = ((7.76*(AnkAtk[3][0]-gk[3][0]))-ffk[3][0]);
    fc[3][1] = ((7.76*(AnkAtk[3][1]-gk[3][1]))-ffk[3][1]);
    fc[3][2] = -ffk[3][2];
    tc[3][0] = -(ttk[3][0]+(.17*fc[3][2]));
    tc[3][1] = -ttk[3][1];
    tc[3][2] = ((.137*Onkb[3][2])-(ttk[3][2]-(.17*fc[3][0])));
    fccikt[3][0] = ((fc[3][0]*c3)-(fc[3][1]*s3));
    fccikt[3][1] = ((fc[3][0]*s3)+(fc[3][1]*c3));
    fccikt[3][2] = fc[3][2];
    ffk[2][0] = (ffk[2][0]-fccikt[3][0]);
    ffk[2][1] = (ffk[2][1]-fccikt[3][1]);
    ffk[2][2] = (ffk[2][2]-fccikt[3][2]);
    ttk[2][0] = (ttk[2][0]-(((tc[3][0]*c3)-(tc[3][1]*s3))-((.0835*fccikt[3][1])+
      (.33*fccikt[3][2]))));
    ttk[2][1] = (ttk[2][1]-((.0835*fccikt[3][0])+((tc[3][0]*s3)+(tc[3][1]*c3))))
      ;
    ttk[2][2] = (ttk[2][2]-(tc[3][2]+(.33*fccikt[3][0])));
    fc[2][0] = ((51.1*(AnkAtk[2][0]+(9.80665*s2)))-ffk[2][0]);
    fc[2][1] = ((51.1*(AnkAtk[2][1]+(9.80665*c2)))-ffk[2][1]);
    fc[2][2] = -ffk[2][2];
    tc[2][0] = -(ttk[2][0]-(.2639*fc[2][2]));
    tc[2][1] = -(ttk[2][1]-(.0707*fc[2][2]));
    tc[2][2] = ((2.58*udot[2])-(ttk[2][2]+((.0707*fc[2][1])+(.2639*fc[2][0]))));
    fccikt[2][0] = ((fc[2][0]*c2)-(fc[2][1]*s2));
    fccikt[2][1] = ((fc[2][0]*s2)+(fc[2][1]*c2));
    fccikt[2][2] = fc[2][2];
    ffk[1][0] = -fccikt[2][0];
    ffk[1][1] = -fccikt[2][1];
    ffk[1][2] = -fccikt[2][2];
    ttk[1][0] = -((tc[2][0]*c2)-(tc[2][1]*s2));
    ttk[1][1] = -((tc[2][0]*s2)+(tc[2][1]*c2));
    ttk[1][2] = -tc[2][2];
    fc[1][0] = -ffk[1][0];
    fc[1][1] = -ffk[1][1];
    fc[1][2] = -ffk[1][2];
    tc[1][0] = -ttk[1][0];
    tc[1][1] = -ttk[1][1];
    tc[1][2] = -ttk[1][2];
    fccikt[1][0] = fc[1][0];
    fccikt[1][1] = fc[1][1];
    fccikt[1][2] = fc[1][2];
    ffk[0][0] = -fccikt[1][0];
    ffk[0][1] = -fccikt[1][1];
    ffk[0][2] = -fccikt[1][2];
    ttk[0][0] = -(tc[1][0]+(fccikt[1][2]*q[1]));
    ttk[0][1] = -tc[1][1];
    ttk[0][2] = -(tc[1][2]-(fccikt[1][0]*q[1]));
    fc[0][0] = -ffk[0][0];
    fc[0][1] = -ffk[0][1];
    fc[0][2] = -ffk[0][2];
    tc[0][0] = -ttk[0][0];
    tc[0][1] = -ttk[0][1];
    tc[0][2] = -ttk[0][2];
    force[0][0] = fc[2][0];
    torque[0][0] = tc[2][0];
    force[0][1] = fc[2][1];
    torque[0][1] = tc[2][1];
    force[0][2] = fc[2][2];
    torque[0][2] = tc[2][2];
    force[1][0] = fc[3][0];
    torque[1][0] = tc[3][0];
    force[1][1] = fc[3][1];
    torque[1][1] = tc[3][1];
    force[1][2] = fc[3][2];
    torque[1][2] = tc[3][2];
    force[2][0] = fc[6][0];
    torque[2][0] = tc[6][0];
    force[2][1] = fc[6][1];
    torque[2][1] = tc[6][1];
    force[2][2] = fc[6][2];
    torque[2][2] = tc[6][2];
    force[3][0] = fc[9][0];
    torque[3][0] = tc[9][0];
    force[3][1] = fc[9][1];
    torque[3][1] = tc[9][1];
    force[3][2] = fc[9][2];
    torque[3][2] = tc[9][2];
    force[4][0] = fc[10][0];
    torque[4][0] = tc[10][0];
    force[4][1] = fc[10][1];
    torque[4][1] = tc[10][1];
    force[4][2] = fc[10][2];
    torque[4][2] = tc[10][2];
    force[5][0] = fc[11][0];
    torque[5][0] = tc[11][0];
    force[5][1] = fc[11][1];
    torque[5][1] = tc[11][1];
    force[5][2] = fc[11][2];
    torque[5][2] = tc[11][2];
    force[6][0] = fc[12][0];
    torque[6][0] = tc[12][0];
    force[6][1] = fc[12][1];
    torque[6][1] = tc[12][1];
    force[6][2] = fc[12][2];
    torque[6][2] = tc[12][2];
    force[7][0] = fc[13][0];
    torque[7][0] = tc[13][0];
    force[7][1] = fc[13][1];
    torque[7][1] = tc[13][1];
    force[7][2] = fc[13][2];
    torque[7][2] = tc[13][2];
    force[8][0] = fc[14][0];
    torque[8][0] = tc[14][0];
    force[8][1] = fc[14][1];
    torque[8][1] = tc[14][1];
    force[8][2] = fc[14][2];
    torque[8][2] = tc[14][2];
    force[9][0] = fc[17][0];
    torque[9][0] = tc[17][0];
    force[9][1] = fc[17][1];
    torque[9][1] = tc[17][1];
    force[9][2] = fc[17][2];
    torque[9][2] = tc[17][2];
    force[10][0] = fc[20][0];
    torque[10][0] = tc[20][0];
    force[10][1] = fc[20][1];
    torque[10][1] = tc[20][1];
    force[10][2] = fc[20][2];
    torque[10][2] = tc[20][2];
    force[11][0] = fc[21][0];
    torque[11][0] = tc[21][0];
    force[11][1] = fc[21][1];
    torque[11][1] = tc[21][1];
    force[11][2] = fc[21][2];
    torque[11][2] = tc[21][2];
    force[12][0] = fc[22][0];
    torque[12][0] = tc[22][0];
    force[12][1] = fc[22][1];
    torque[12][1] = tc[22][1];
    force[12][2] = fc[22][2];
    torque[12][2] = tc[22][2];
    force[13][0] = fc[23][0];
    torque[13][0] = tc[23][0];
    force[13][1] = fc[23][1];
    torque[13][1] = tc[23][1];
    force[13][2] = fc[23][2];
    torque[13][2] = tc[23][2];
    force[14][0] = fc[24][0];
    torque[14][0] = tc[24][0];
    force[14][1] = fc[24][1];
    torque[14][1] = tc[24][1];
    force[14][2] = fc[24][2];
    torque[14][2] = tc[24][2];
/*
Compute reaction forces for tree weld joints
*/
/*
 Used 0.03 seconds CPU time,
 0 additional bytes of memory.
 Equations contain  594 adds/subtracts/negates
                    299 multiplies
                      0 divides
                    546 assignments
*/
}

void sdmom(double lm[3],
    double am[3],
    double *ke)
{
/*
Compute system linear and angular momentum, and kinetic energy.

Generated 22-Jun-2007 09:35:41 by SD/FAST, Kane's formulation
(sdfast B.2.8 #30123) on machine ID unknown
Copyright (c) 1990-1997 Symbolic Dynamics, Inc.
Copyright (c) 1990-1997 Parametric Technology Corp.
RESTRICTED RIGHTS LEGEND: Use, duplication, or disclosure by the U.S.
Government is subject to restrictions as set forth in subparagraph
(c)(1)(ii) of the Rights in Technical Data and Computer Software
clause at DFARS 52.227-7013 and similar clauses in the FAR and NASA
FAR Supplement.  Symbolic Dynamics, Inc., Mountain View, CA 94041
*/
    double lk[15][3],hnk[15][3];

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(19,23);
        return;
    }
    lm[0] = ((.2058*vnk[24][0])+((.4*vnk[23][0])+((.4183*vnk[22][0])+((.1*
      vnk[21][0])+((.1*vnk[19][0])+((3.03*vnk[17][0])+((7.76*vnk[14][0])+((.2058
      *vnk[13][0])+((.4*vnk[12][0])+((.4183*vnk[11][0])+((.1*vnk[10][0])+((.1*
      vnk[8][0])+((3.03*vnk[6][0])+((7.76*vnk[3][0])+(51.1*vnk[2][0]))))))))))))
      )));
    lm[1] = ((.2058*vnk[24][1])+((.4*vnk[23][1])+((.4183*vnk[22][1])+((.1*
      vnk[21][1])+((.1*vnk[19][1])+((3.03*vnk[17][1])+((7.76*vnk[14][1])+((.2058
      *vnk[13][1])+((.4*vnk[12][1])+((.4183*vnk[11][1])+((.1*vnk[10][1])+((.1*
      vnk[8][1])+((3.03*vnk[6][1])+((7.76*vnk[3][1])+(51.1*vnk[2][1]))))))))))))
      )));
    lm[2] = ((.2058*vnk[24][2])+((.4*vnk[23][2])+((.4183*vnk[22][2])+((.2058*
      vnk[13][2])+((.4*vnk[12][2])+(.4183*vnk[11][2]))))));
    temp[0] = (((.001*((cnk[11][0][2]*wk[12][2])+((cnk[12][0][0]*wk[12][0])+(
      cnk[12][0][1]*wk[12][1]))))+(.4*((rnk[12][1]*vnk[12][2])-(rnk[12][2]*
      vnk[12][1]))))+(((.001*((cnk[11][0][2]*wk[11][2])+((cnk[10][0][0]*u[11])+(
      cnk[11][0][1]*wk[11][1]))))+(.4183*((rnk[11][1]*vnk[11][2])-(rnk[11][2]*
      vnk[11][1]))))-((.00835*vnk[10][1])+((.00859*vnk[8][1])+((.253005*
      vnk[6][1])+(.64796*vnk[3][1]))))));
    temp[1] = (((.001*((cnk[22][0][2]*wk[22][2])+((cnk[21][0][0]*u[22])+(
      cnk[22][0][1]*wk[22][1]))))+(.4183*((rnk[22][1]*vnk[22][2])-(rnk[22][2]*
      vnk[22][1]))))+((.00835*vnk[21][1])+((.00859*vnk[19][1])+((.253005*
      vnk[17][1])+((.64796*vnk[14][1])+(((.001*((cnk[11][0][2]*wk[13][2])+((
      cnk[13][0][0]*wk[13][0])+(cnk[13][0][1]*wk[13][1]))))+(.2058*((rnk[13][1]*
      vnk[13][2])-(rnk[13][2]*vnk[13][1]))))+temp[0]))))));
    am[0] = ((((.001*((cnk[22][0][2]*wk[24][2])+((cnk[24][0][0]*wk[24][0])+(
      cnk[24][0][1]*wk[24][1]))))+(.2058*((rnk[24][1]*vnk[24][2])-(rnk[24][2]*
      vnk[24][1]))))+(((.001*((cnk[22][0][2]*wk[23][2])+((cnk[23][0][0]*
      wk[23][0])+(cnk[23][0][1]*wk[23][1]))))+(.4*((rnk[23][1]*vnk[23][2])-(
      rnk[23][2]*vnk[23][1]))))+temp[1]))-((com[1]*lm[2])-(com[2]*lm[1])));
    temp[0] = (((.001*((cnk[11][1][2]*wk[12][2])+((cnk[12][1][0]*wk[12][0])+(
      cnk[12][1][1]*wk[12][1]))))+(.4*((rnk[12][2]*vnk[12][0])-(rnk[12][0]*
      vnk[12][2]))))+(((.001*((cnk[11][1][2]*wk[11][2])+((cnk[10][1][0]*u[11])+(
      cnk[11][1][1]*wk[11][1]))))+(.4183*((rnk[11][2]*vnk[11][0])-(rnk[11][0]*
      vnk[11][2]))))+((.00835*vnk[10][0])+((.00859*vnk[8][0])+((.253005*
      vnk[6][0])+(.64796*vnk[3][0]))))));
    temp[1] = (((.001*((cnk[22][1][2]*wk[22][2])+((cnk[21][1][0]*u[22])+(
      cnk[22][1][1]*wk[22][1]))))+(.4183*((rnk[22][2]*vnk[22][0])-(rnk[22][0]*
      vnk[22][2]))))+(((((((.001*((cnk[11][1][2]*wk[13][2])+((cnk[13][1][0]*
      wk[13][0])+(cnk[13][1][1]*wk[13][1]))))+(.2058*((rnk[13][2]*vnk[13][0])-(
      rnk[13][0]*vnk[13][2]))))+temp[0])-(.64796*vnk[14][0]))-(.253005*
      vnk[17][0]))-(.00859*vnk[19][0]))-(.00835*vnk[21][0])));
    am[1] = ((((.001*((cnk[22][1][2]*wk[24][2])+((cnk[24][1][0]*wk[24][0])+(
      cnk[24][1][1]*wk[24][1]))))+(.2058*((rnk[24][2]*vnk[24][0])-(rnk[24][0]*
      vnk[24][2]))))+(((.001*((cnk[22][1][2]*wk[23][2])+((cnk[23][1][0]*
      wk[23][0])+(cnk[23][1][1]*wk[23][1]))))+(.4*((rnk[23][2]*vnk[23][0])-(
      rnk[23][0]*vnk[23][2]))))+temp[1]))-((com[2]*lm[0])-(com[0]*lm[2])));
    temp[0] = (((.001*wk[10][2])+(.1*((rnk[10][0]*vnk[10][1])-(rnk[10][1]*
      vnk[10][0]))))+(((.001*wk[9][2])+(.1*((rnk[8][0]*vnk[8][1])-(rnk[8][1]*
      vnk[8][0]))))+(((.0445*wk[6][2])+(3.03*((rnk[6][0]*vnk[6][1])-(rnk[6][1]*
      vnk[6][0]))))+(((.137*wk[3][2])+(7.76*((rnk[3][0]*vnk[3][1])-(rnk[3][1]*
      vnk[3][0]))))+((2.58*u[2])+(51.1*((rnk[2][0]*vnk[2][1])-(rnk[2][1]*
      vnk[2][0]))))))));
    temp[1] = (((.001*((wk[13][2]*c11)+((cnk[13][2][0]*wk[13][0])+(cnk[13][2][1]
      *wk[13][1]))))+(.2058*((rnk[13][0]*vnk[13][1])-(rnk[13][1]*vnk[13][0]))))+
      (((.001*((wk[12][2]*c11)+((cnk[12][2][0]*wk[12][0])+(cnk[12][2][1]*
      wk[12][1]))))+(.4*((rnk[12][0]*vnk[12][1])-(rnk[12][1]*vnk[12][0]))))+(((
      .001*((wk[11][1]*s11)+(wk[11][2]*c11)))+(.4183*((rnk[11][0]*vnk[11][1])-(
      rnk[11][1]*vnk[11][0]))))+temp[0])));
    temp[2] = (((.001*wk[21][2])+(.1*((rnk[21][0]*vnk[21][1])-(rnk[21][1]*
      vnk[21][0]))))+(((.001*wk[20][2])+(.1*((rnk[19][0]*vnk[19][1])-(rnk[19][1]
      *vnk[19][0]))))+(((.0445*wk[17][2])+(3.03*((rnk[17][0]*vnk[17][1])-(
      rnk[17][1]*vnk[17][0]))))+(((.137*wk[14][2])+(7.76*((rnk[14][0]*vnk[14][1]
      )-(rnk[14][1]*vnk[14][0]))))+temp[1]))));
    temp[3] = (((.001*((wk[24][2]*c22)+((cnk[24][2][0]*wk[24][0])+(cnk[24][2][1]
      *wk[24][1]))))+(.2058*((rnk[24][0]*vnk[24][1])-(rnk[24][1]*vnk[24][0]))))+
      (((.001*((wk[23][2]*c22)+((cnk[23][2][0]*wk[23][0])+(cnk[23][2][1]*
      wk[23][1]))))+(.4*((rnk[23][0]*vnk[23][1])-(rnk[23][1]*vnk[23][0]))))+(((
      .001*((wk[22][1]*s22)+(wk[22][2]*c22)))+(.4183*((rnk[22][0]*vnk[22][1])-(
      rnk[22][1]*vnk[22][0]))))+temp[2])));
    am[2] = (temp[3]-((com[0]*lm[1])-(com[1]*lm[0])));
    temp[0] = (((.001*(wk[9][2]*wk[9][2]))+(.1*((vnk[8][0]*vnk[8][0])+(vnk[8][1]
      *vnk[8][1]))))+(((.0445*(wk[6][2]*wk[6][2]))+(3.03*((vnk[6][0]*vnk[6][0])+
      (vnk[6][1]*vnk[6][1]))))+(((.137*(wk[3][2]*wk[3][2]))+(7.76*((vnk[3][0]*
      vnk[3][0])+(vnk[3][1]*vnk[3][1]))))+((2.58*(u[2]*u[2]))+(51.1*((vnk[2][0]*
      vnk[2][0])+(vnk[2][1]*vnk[2][1])))))));
    temp[1] = (((.001*((wk[11][2]*wk[11][2])+((u[11]*u[11])+(wk[11][1]*wk[11][1]
      ))))+(.4183*((vnk[11][2]*vnk[11][2])+((vnk[11][0]*vnk[11][0])+(vnk[11][1]*
      vnk[11][1])))))+(((.001*(wk[10][2]*wk[10][2]))+(.1*((vnk[10][0]*vnk[10][0]
      )+(vnk[10][1]*vnk[10][1]))))+temp[0]));
    temp[2] = (((.001*((wk[13][2]*wk[13][2])+((wk[13][0]*wk[13][0])+(wk[13][1]*
      wk[13][1]))))+(.2058*((vnk[13][2]*vnk[13][2])+((vnk[13][0]*vnk[13][0])+(
      vnk[13][1]*vnk[13][1])))))+(((.001*((wk[12][2]*wk[12][2])+((wk[12][0]*
      wk[12][0])+(wk[12][1]*wk[12][1]))))+(.4*((vnk[12][2]*vnk[12][2])+((
      vnk[12][0]*vnk[12][0])+(vnk[12][1]*vnk[12][1])))))+temp[1]));
    temp[3] = (((.001*(wk[21][2]*wk[21][2]))+(.1*((vnk[21][0]*vnk[21][0])+(
      vnk[21][1]*vnk[21][1]))))+(((.001*(wk[20][2]*wk[20][2]))+(.1*((vnk[19][0]*
      vnk[19][0])+(vnk[19][1]*vnk[19][1]))))+(((.0445*(wk[17][2]*wk[17][2]))+(
      3.03*((vnk[17][0]*vnk[17][0])+(vnk[17][1]*vnk[17][1]))))+(((.137*(
      wk[14][2]*wk[14][2]))+(7.76*((vnk[14][0]*vnk[14][0])+(vnk[14][1]*
      vnk[14][1]))))+temp[2]))));
    temp[4] = (((.001*((wk[23][2]*wk[23][2])+((wk[23][0]*wk[23][0])+(wk[23][1]*
      wk[23][1]))))+(.4*((vnk[23][2]*vnk[23][2])+((vnk[23][0]*vnk[23][0])+(
      vnk[23][1]*vnk[23][1])))))+(((.001*((wk[22][2]*wk[22][2])+((u[22]*u[22])+(
      wk[22][1]*wk[22][1]))))+(.4183*((vnk[22][2]*vnk[22][2])+((vnk[22][0]*
      vnk[22][0])+(vnk[22][1]*vnk[22][1])))))+temp[3]));
    *ke = (.5*(((.001*((wk[24][2]*wk[24][2])+((wk[24][0]*wk[24][0])+(wk[24][1]*
      wk[24][1]))))+(.2058*((vnk[24][2]*vnk[24][2])+((vnk[24][0]*vnk[24][0])+(
      vnk[24][1]*vnk[24][1])))))+temp[4]));
/*
 Used 0.00 seconds CPU time,
 0 additional bytes of memory.
 Equations contain  229 adds/subtracts/negates
                    312 multiplies
                      0 divides
                     20 assignments
*/
}

void sdsys(double *mtoto,
    double cm[3],
    double icm[3][3])
{
/*
Compute system total mass, and instantaneous center of mass and
inertia matrix.

Generated 22-Jun-2007 09:35:41 by SD/FAST, Kane's formulation
(sdfast B.2.8 #30123) on machine ID unknown
Copyright (c) 1990-1997 Symbolic Dynamics, Inc.
Copyright (c) 1990-1997 Parametric Technology Corp.
RESTRICTED RIGHTS LEGEND: Use, duplication, or disclosure by the U.S.
Government is subject to restrictions as set forth in subparagraph
(c)(1)(ii) of the Rights in Technical Data and Computer Software
clause at DFARS 52.227-7013 and similar clauses in the FAR and NASA
FAR Supplement.  Symbolic Dynamics, Inc., Mountain View, CA 94041
*/
    double ikcnkt[25][3][3];

    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(20,23);
        return;
    }
    *mtoto = 75.1282;
    cm[0] = com[0];
    cm[1] = com[1];
    cm[2] = com[2];
    temp[0] = (((.001*((cnk[9][0][0]*cnk[9][0][0])+(cnk[9][0][1]*cnk[9][0][1])))
      +(.1*(.00737881+(rnk[8][1]*rnk[8][1]))))+(((3.03*(.00697225+(rnk[6][1]*
      rnk[6][1])))+((.00383*(cnk[6][0][1]*cnk[6][0][1]))+(.0444*(cnk[6][0][0]*
      cnk[6][0][0]))))+(((.6807*(c2*c2))+(51.1*(rnk[2][1]*rnk[2][1])))+((7.76*(
      .00697225+(rnk[3][1]*rnk[3][1])))+((.0316*(cnk[3][0][1]*cnk[3][0][1]))+(
      .137*(cnk[3][0][0]*cnk[3][0][0])))))));
    temp[1] = (((.001*((cnk[11][0][2]*cnk[11][0][2])+((cnk[10][0][0]*
      cnk[10][0][0])+(cnk[11][0][1]*cnk[11][0][1]))))+(.4183*((rnk[11][1]*
      rnk[11][1])+(rnk[11][2]*rnk[11][2]))))+(((.001*((cnk[10][0][0]*
      cnk[10][0][0])+(cnk[10][0][1]*cnk[10][0][1])))+(.1*(.00697225+(rnk[10][1]*
      rnk[10][1]))))+temp[0]));
    temp[2] = (((.001*((cnk[11][0][2]*cnk[11][0][2])+((cnk[13][0][0]*
      cnk[13][0][0])+(cnk[13][0][1]*cnk[13][0][1]))))+(.2058*((rnk[13][1]*
      rnk[13][1])+(rnk[13][2]*rnk[13][2]))))+(((.001*((cnk[11][0][2]*
      cnk[11][0][2])+((cnk[12][0][0]*cnk[12][0][0])+(cnk[12][0][1]*cnk[12][0][1]
      ))))+(.4*((rnk[12][1]*rnk[12][1])+(rnk[12][2]*rnk[12][2]))))+temp[1]));
    temp[3] = (((.001*((cnk[20][0][0]*cnk[20][0][0])+(cnk[20][0][1]*
      cnk[20][0][1])))+(.1*(.00737881+(rnk[19][1]*rnk[19][1]))))+(((3.03*(
      .00697225+(rnk[17][1]*rnk[17][1])))+((.00383*(cnk[17][0][1]*cnk[17][0][1])
      )+(.0444*(cnk[17][0][0]*cnk[17][0][0]))))+(((7.76*(.00697225+(rnk[14][1]*
      rnk[14][1])))+((.0316*(cnk[14][0][1]*cnk[14][0][1]))+(.137*(cnk[14][0][0]*
      cnk[14][0][0]))))+temp[2])));
    temp[4] = (((.001*((cnk[22][0][2]*cnk[22][0][2])+((cnk[21][0][0]*
      cnk[21][0][0])+(cnk[22][0][1]*cnk[22][0][1]))))+(.4183*((rnk[22][1]*
      rnk[22][1])+(rnk[22][2]*rnk[22][2]))))+(((.001*((cnk[21][0][0]*
      cnk[21][0][0])+(cnk[21][0][1]*cnk[21][0][1])))+(.1*(.00697225+(rnk[21][1]*
      rnk[21][1]))))+temp[3]));
    icm[0][0] = (.7153+((((.001*((cnk[22][0][2]*cnk[22][0][2])+((cnk[24][0][0]*
      cnk[24][0][0])+(cnk[24][0][1]*cnk[24][0][1]))))+(.2058*((rnk[24][1]*
      rnk[24][1])+(rnk[24][2]*rnk[24][2]))))+(((.001*((cnk[22][0][2]*
      cnk[22][0][2])+((cnk[23][0][0]*cnk[23][0][0])+(cnk[23][0][1]*cnk[23][0][1]
      ))))+(.4*((rnk[23][1]*rnk[23][1])+(rnk[23][2]*rnk[23][2]))))+temp[4]))-(
      75.1282*((com[1]*com[1])+(com[2]*com[2])))));
    temp[0] = (((.001*((cnk[9][0][0]*cnk[9][1][0])+(cnk[9][0][1]*cnk[9][1][1])))
      -(.1*(rnk[8][0]*rnk[8][1])))+((((.00383*(cnk[6][0][1]*cnk[6][1][1]))+(
      .0444*(cnk[6][0][0]*cnk[6][1][0])))-(3.03*(rnk[6][0]*rnk[6][1])))+(((.6807
      *(s2*c2))-(51.1*(rnk[2][0]*rnk[2][1])))+(((.0316*(cnk[3][0][1]*
      cnk[3][1][1]))+(.137*(cnk[3][0][0]*cnk[3][1][0])))-(7.76*(rnk[3][0]*
      rnk[3][1]))))));
    temp[1] = (((.001*((cnk[11][0][2]*cnk[11][1][2])+((cnk[12][0][0]*
      cnk[12][1][0])+(cnk[12][0][1]*cnk[12][1][1]))))-(.4*(rnk[12][0]*rnk[12][1]
      )))+(((.001*((cnk[11][0][2]*cnk[11][1][2])+((cnk[10][0][0]*cnk[10][1][0])+
      (cnk[11][0][1]*cnk[11][1][1]))))-(.4183*(rnk[11][0]*rnk[11][1])))+(((.001*
      ((cnk[10][0][0]*cnk[10][1][0])+(cnk[10][0][1]*cnk[10][1][1])))-(.1*(
      rnk[10][0]*rnk[10][1])))+temp[0])));
    temp[2] = ((((.00383*(cnk[17][0][1]*cnk[17][1][1]))+(.0444*(cnk[17][0][0]*
      cnk[17][1][0])))-(3.03*(rnk[17][0]*rnk[17][1])))+((((.001*((cnk[11][0][2]*
      cnk[11][1][2])+((cnk[13][0][0]*cnk[13][1][0])+(cnk[13][0][1]*cnk[13][1][1]
      ))))-(.2058*(rnk[13][0]*rnk[13][1])))+temp[1])+(((.0316*(cnk[14][0][1]*
      cnk[14][1][1]))+(.137*(cnk[14][0][0]*cnk[14][1][0])))-(7.76*(rnk[14][0]*
      rnk[14][1])))));
    temp[3] = (((.001*((cnk[22][0][2]*cnk[22][1][2])+((cnk[21][0][0]*
      cnk[21][1][0])+(cnk[22][0][1]*cnk[22][1][1]))))-(.4183*(rnk[22][0]*
      rnk[22][1])))+(((.001*((cnk[21][0][0]*cnk[21][1][0])+(cnk[21][0][1]*
      cnk[21][1][1])))-(.1*(rnk[21][0]*rnk[21][1])))+(((.001*((cnk[20][0][0]*
      cnk[20][1][0])+(cnk[20][0][1]*cnk[20][1][1])))-(.1*(rnk[19][0]*rnk[19][1])
      ))+temp[2])));
    icm[0][1] = ((75.1282*(com[0]*com[1]))+(((.001*((cnk[22][0][2]*cnk[22][1][2]
      )+((cnk[24][0][0]*cnk[24][1][0])+(cnk[24][0][1]*cnk[24][1][1]))))-(.2058*(
      rnk[24][0]*rnk[24][1])))+(((.001*((cnk[22][0][2]*cnk[22][1][2])+((
      cnk[23][0][0]*cnk[23][1][0])+(cnk[23][0][1]*cnk[23][1][1]))))-(.4*(
      rnk[23][0]*rnk[23][1])))+temp[3])));
    temp[0] = (((.001*((cnk[11][0][2]*c11)+((cnk[12][0][0]*cnk[12][2][0])+(
      cnk[12][0][1]*cnk[12][2][1]))))-(.4*(rnk[12][0]*rnk[12][2])))+(((.001*((
      cnk[11][0][1]*s11)+(cnk[11][0][2]*c11)))-(.4183*(rnk[11][0]*rnk[11][2])))-
      ((.00835*rnk[10][0])+((.00859*rnk[8][0])+((.253005*rnk[6][0])+(.64796*
      rnk[3][0]))))));
    temp[1] = (((.001*((cnk[22][0][1]*s22)+(cnk[22][0][2]*c22)))-(.4183*(
      rnk[22][0]*rnk[22][2])))+((.00835*rnk[21][0])+((.00859*rnk[19][0])+((
      .253005*rnk[17][0])+((.64796*rnk[14][0])+(((.001*((cnk[11][0][2]*c11)+((
      cnk[13][0][0]*cnk[13][2][0])+(cnk[13][0][1]*cnk[13][2][1]))))-(.2058*(
      rnk[13][0]*rnk[13][2])))+temp[0]))))));
    icm[0][2] = ((75.1282*(com[0]*com[2]))+(((.001*((cnk[22][0][2]*c22)+((
      cnk[24][0][0]*cnk[24][2][0])+(cnk[24][0][1]*cnk[24][2][1]))))-(.2058*(
      rnk[24][0]*rnk[24][2])))+(((.001*((cnk[22][0][2]*c22)+((cnk[23][0][0]*
      cnk[23][2][0])+(cnk[23][0][1]*cnk[23][2][1]))))-(.4*(rnk[23][0]*rnk[23][2]
      )))+temp[1])));
    icm[1][0] = icm[0][1];
    temp[0] = (((.001*((cnk[9][1][0]*cnk[9][1][0])+(cnk[9][1][1]*cnk[9][1][1])))
      +(.1*(.00737881+(rnk[8][0]*rnk[8][0]))))+(((3.03*(.00697225+(rnk[6][0]*
      rnk[6][0])))+((.00383*(cnk[6][1][1]*cnk[6][1][1]))+(.0444*(cnk[6][1][0]*
      cnk[6][1][0]))))+(((.6807*(s2*s2))+(51.1*(rnk[2][0]*rnk[2][0])))+((7.76*(
      .00697225+(rnk[3][0]*rnk[3][0])))+((.0316*(cnk[3][1][1]*cnk[3][1][1]))+(
      .137*(cnk[3][1][0]*cnk[3][1][0])))))));
    temp[1] = (((.001*((cnk[11][1][2]*cnk[11][1][2])+((cnk[10][1][0]*
      cnk[10][1][0])+(cnk[11][1][1]*cnk[11][1][1]))))+(.4183*((rnk[11][0]*
      rnk[11][0])+(rnk[11][2]*rnk[11][2]))))+(((.001*((cnk[10][1][0]*
      cnk[10][1][0])+(cnk[10][1][1]*cnk[10][1][1])))+(.1*(.00697225+(rnk[10][0]*
      rnk[10][0]))))+temp[0]));
    temp[2] = (((.001*((cnk[11][1][2]*cnk[11][1][2])+((cnk[13][1][0]*
      cnk[13][1][0])+(cnk[13][1][1]*cnk[13][1][1]))))+(.2058*((rnk[13][0]*
      rnk[13][0])+(rnk[13][2]*rnk[13][2]))))+(((.001*((cnk[11][1][2]*
      cnk[11][1][2])+((cnk[12][1][0]*cnk[12][1][0])+(cnk[12][1][1]*cnk[12][1][1]
      ))))+(.4*((rnk[12][0]*rnk[12][0])+(rnk[12][2]*rnk[12][2]))))+temp[1]));
    temp[3] = (((.001*((cnk[20][1][0]*cnk[20][1][0])+(cnk[20][1][1]*
      cnk[20][1][1])))+(.1*(.00737881+(rnk[19][0]*rnk[19][0]))))+(((3.03*(
      .00697225+(rnk[17][0]*rnk[17][0])))+((.00383*(cnk[17][1][1]*cnk[17][1][1])
      )+(.0444*(cnk[17][1][0]*cnk[17][1][0]))))+(((7.76*(.00697225+(rnk[14][0]*
      rnk[14][0])))+((.0316*(cnk[14][1][1]*cnk[14][1][1]))+(.137*(cnk[14][1][0]*
      cnk[14][1][0]))))+temp[2])));
    temp[4] = (((.001*((cnk[22][1][2]*cnk[22][1][2])+((cnk[21][1][0]*
      cnk[21][1][0])+(cnk[22][1][1]*cnk[22][1][1]))))+(.4183*((rnk[22][0]*
      rnk[22][0])+(rnk[22][2]*rnk[22][2]))))+(((.001*((cnk[21][1][0]*
      cnk[21][1][0])+(cnk[21][1][1]*cnk[21][1][1])))+(.1*(.00697225+(rnk[21][0]*
      rnk[21][0]))))+temp[3]));
    icm[1][1] = (.7153+((((.001*((cnk[22][1][2]*cnk[22][1][2])+((cnk[24][1][0]*
      cnk[24][1][0])+(cnk[24][1][1]*cnk[24][1][1]))))+(.2058*((rnk[24][0]*
      rnk[24][0])+(rnk[24][2]*rnk[24][2]))))+(((.001*((cnk[22][1][2]*
      cnk[22][1][2])+((cnk[23][1][0]*cnk[23][1][0])+(cnk[23][1][1]*cnk[23][1][1]
      ))))+(.4*((rnk[23][0]*rnk[23][0])+(rnk[23][2]*rnk[23][2]))))+temp[4]))-(
      75.1282*((com[0]*com[0])+(com[2]*com[2])))));
    temp[0] = (((.001*((cnk[11][1][2]*c11)+((cnk[12][1][0]*cnk[12][2][0])+(
      cnk[12][1][1]*cnk[12][2][1]))))-(.4*(rnk[12][1]*rnk[12][2])))+(((.001*((
      cnk[11][1][1]*s11)+(cnk[11][1][2]*c11)))-(.4183*(rnk[11][1]*rnk[11][2])))-
      ((.00835*rnk[10][1])+((.00859*rnk[8][1])+((.253005*rnk[6][1])+(.64796*
      rnk[3][1]))))));
    temp[1] = (((.001*((cnk[22][1][1]*s22)+(cnk[22][1][2]*c22)))-(.4183*(
      rnk[22][1]*rnk[22][2])))+((.00835*rnk[21][1])+((.00859*rnk[19][1])+((
      .253005*rnk[17][1])+((.64796*rnk[14][1])+(((.001*((cnk[11][1][2]*c11)+((
      cnk[13][1][0]*cnk[13][2][0])+(cnk[13][1][1]*cnk[13][2][1]))))-(.2058*(
      rnk[13][1]*rnk[13][2])))+temp[0]))))));
    icm[1][2] = ((75.1282*(com[1]*com[2]))+(((.001*((cnk[22][1][2]*c22)+((
      cnk[24][1][0]*cnk[24][2][0])+(cnk[24][1][1]*cnk[24][2][1]))))-(.2058*(
      rnk[24][1]*rnk[24][2])))+(((.001*((cnk[22][1][2]*c22)+((cnk[23][1][0]*
      cnk[23][2][0])+(cnk[23][1][1]*cnk[23][2][1]))))-(.4*(rnk[23][1]*rnk[23][2]
      )))+temp[1])));
    icm[2][0] = icm[0][2];
    icm[2][1] = icm[1][2];
    temp[0] = ((.4183*((rnk[11][0]*rnk[11][0])+(rnk[11][1]*rnk[11][1])))+((.1*((
      rnk[10][0]*rnk[10][0])+(rnk[10][1]*rnk[10][1])))+((.1*((rnk[8][0]*
      rnk[8][0])+(rnk[8][1]*rnk[8][1])))+((3.03*((rnk[6][0]*rnk[6][0])+(
      rnk[6][1]*rnk[6][1])))+((7.76*((rnk[3][0]*rnk[3][0])+(rnk[3][1]*rnk[3][1])
      ))+(51.1*((rnk[2][0]*rnk[2][0])+(rnk[2][1]*rnk[2][1]))))))));
    temp[1] = ((3.03*((rnk[17][0]*rnk[17][0])+(rnk[17][1]*rnk[17][1])))+((7.76*(
      (rnk[14][0]*rnk[14][0])+(rnk[14][1]*rnk[14][1])))+(((.001*((c11*c11)+((
      cnk[13][2][0]*cnk[13][2][0])+(cnk[13][2][1]*cnk[13][2][1]))))+(.2058*((
      rnk[13][0]*rnk[13][0])+(rnk[13][1]*rnk[13][1]))))+(((.001*((c11*c11)+((
      cnk[12][2][0]*cnk[12][2][0])+(cnk[12][2][1]*cnk[12][2][1]))))+(.4*((
      rnk[12][0]*rnk[12][0])+(rnk[12][1]*rnk[12][1]))))+temp[0]))));
    temp[2] = (((.001*((c22*c22)+((cnk[23][2][0]*cnk[23][2][0])+(cnk[23][2][1]*
      cnk[23][2][1]))))+(.4*((rnk[23][0]*rnk[23][0])+(rnk[23][1]*rnk[23][1]))))+
      ((.4183*((rnk[22][0]*rnk[22][0])+(rnk[22][1]*rnk[22][1])))+((.1*((
      rnk[21][0]*rnk[21][0])+(rnk[21][1]*rnk[21][1])))+((.1*((rnk[19][0]*
      rnk[19][0])+(rnk[19][1]*rnk[19][1])))+temp[1]))));
    icm[2][2] = (2.949+((((.001*((c22*c22)+((cnk[24][2][0]*cnk[24][2][0])+(
      cnk[24][2][1]*cnk[24][2][1]))))+(.2058*((rnk[24][0]*rnk[24][0])+(
      rnk[24][1]*rnk[24][1]))))+temp[2])-(75.1282*((com[0]*com[0])+(com[1]*
      com[1])))));
/*
 Used 0.05 seconds CPU time,
 0 additional bytes of memory.
 Equations contain  286 adds/subtracts/negates
                    424 multiplies
                      0 divides
                     34 assignments
*/
}

void sdpos(int body,
    double pt[3],
    double loc[3])
{
/*
Return inertial frame location of a point on a body.

*/
    double pv[3];

    if (sdchkbnum(21,body) != 0) {
        return;
    }
    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(21,23);
        return;
    }
    if (body  ==  -1) {
        loc[0] = pt[0];
        loc[1] = pt[1];
        loc[2] = pt[2];
    } else {
        pv[0] = rnb[body][0]+pt[0]*cnb[body][0][0]+pt[1]*cnb[body][0][1]+pt[2]*
          cnb[body][0][2];
        pv[1] = rnb[body][1]+pt[0]*cnb[body][1][0]+pt[1]*cnb[body][1][1]+pt[2]*
          cnb[body][1][2];
        pv[2] = rnb[body][2]+pt[0]*cnb[body][2][0]+pt[1]*cnb[body][2][1]+pt[2]*
          cnb[body][2][2];
        loc[0] = pv[0];
        loc[1] = pv[1];
        loc[2] = pv[2];
    }
}

void sdvel(int body,
    double pt[3],
    double velo[3])
{
/*
Return inertial frame velocity of a point on a body.

*/
    double pv[3];

    if (sdchkbnum(22,body) != 0) {
        return;
    }
    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(22,23);
        return;
    }
    if (body  ==  -1) {
        velo[0] = 0.;
        velo[1] = 0.;
        velo[2] = 0.;
    } else {
        pv[0] = wb[body][1]*pt[2]-wb[body][2]*pt[1];
        pv[1] = wb[body][2]*pt[0]-wb[body][0]*pt[2];
        pv[2] = wb[body][0]*pt[1]-wb[body][1]*pt[0];
        velo[0] = vnb[body][0]+pv[0]*cnb[body][0][0]+pv[1]*cnb[body][0][1]+pv[2]
          *cnb[body][0][2];
        velo[1] = vnb[body][1]+pv[0]*cnb[body][1][0]+pv[1]*cnb[body][1][1]+pv[2]
          *cnb[body][1][2];
        velo[2] = vnb[body][2]+pv[0]*cnb[body][2][0]+pv[1]*cnb[body][2][1]+pv[2]
          *cnb[body][2][2];
    }
}

void sdorient(int body,
    double dircos[3][3])
{
/*
Return orientation of body w.r.t. ground frame.

*/

    if (sdchkbnum(23,body) != 0) {
        return;
    }
    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(23,23);
        return;
    }
    if (body == -1) {
        dircos[0][0] = 1.;
        dircos[0][1] = 0.;
        dircos[0][2] = 0.;
        dircos[1][0] = 0.;
        dircos[1][1] = 1.;
        dircos[1][2] = 0.;
        dircos[2][0] = 0.;
        dircos[2][1] = 0.;
        dircos[2][2] = 1.;
    } else {
        dircos[0][0] = cnb[body][0][0];
        dircos[0][1] = cnb[body][0][1];
        dircos[0][2] = cnb[body][0][2];
        dircos[1][0] = cnb[body][1][0];
        dircos[1][1] = cnb[body][1][1];
        dircos[1][2] = cnb[body][1][2];
        dircos[2][0] = cnb[body][2][0];
        dircos[2][1] = cnb[body][2][1];
        dircos[2][2] = cnb[body][2][2];
    }
}

void sdangvel(int body,
    double avel[3])
{
/*
Return angular velocity of the body.

*/

    if (sdchkbnum(24,body) != 0) {
        return;
    }
    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(24,23);
        return;
    }
    if (body == -1) {
        avel[0] = 0.;
        avel[1] = 0.;
        avel[2] = 0.;
    } else {
        avel[0] = wb[body][0];
        avel[1] = wb[body][1];
        avel[2] = wb[body][2];
    }
}

void sdtrans(int frbod,
    double ivec[3],
    int tobod,
    double ovec[3])
{
/*
Transform ivec from frbod frame to tobod frame.

*/
    double pv[3];

    if (sdchkbnum(25,frbod) != 0) {
        return;
    }
    if (sdchkbnum(25,tobod) != 0) {
        return;
    }
    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(25,23);
        return;
    }
    if (frbod == tobod) {
        sdvcopy(ivec,ovec);
        return;
    }
    if (frbod == -1) {
        sdvcopy(ivec,pv);
        ovec[0] = pv[0]*cnb[tobod][0][0]+pv[1]*cnb[tobod][1][0]+pv[2]*cnb[tobod
          ][2][0];
        ovec[1] = pv[0]*cnb[tobod][0][1]+pv[1]*cnb[tobod][1][1]+pv[2]*cnb[tobod
          ][2][1];
        ovec[2] = pv[0]*cnb[tobod][0][2]+pv[1]*cnb[tobod][1][2]+pv[2]*cnb[tobod
          ][2][2];
        return;
    }
    if (tobod == -1) {
        sdvcopy(ivec,pv);
        ovec[0] = pv[0]*cnb[frbod][0][0]+pv[1]*cnb[frbod][0][1]+pv[2]*cnb[frbod
          ][0][2];
        ovec[1] = pv[0]*cnb[frbod][1][0]+pv[1]*cnb[frbod][1][1]+pv[2]*cnb[frbod
          ][1][2];
        ovec[2] = pv[0]*cnb[frbod][2][0]+pv[1]*cnb[frbod][2][1]+pv[2]*cnb[frbod
          ][2][2];
        return;
    }
    pv[0] = ivec[0]*cnb[frbod][0][0]+ivec[1]*cnb[frbod][0][1]+ivec[2]*cnb[frbod
      ][0][2];
    pv[1] = ivec[0]*cnb[frbod][1][0]+ivec[1]*cnb[frbod][1][1]+ivec[2]*cnb[frbod
      ][1][2];
    pv[2] = ivec[0]*cnb[frbod][2][0]+ivec[1]*cnb[frbod][2][1]+ivec[2]*cnb[frbod
      ][2][2];
    ovec[0] = pv[0]*cnb[tobod][0][0]+pv[1]*cnb[tobod][1][0]+pv[2]*cnb[tobod][2][
      0];
    ovec[1] = pv[0]*cnb[tobod][0][1]+pv[1]*cnb[tobod][1][1]+pv[2]*cnb[tobod][2][
      1];
    ovec[2] = pv[0]*cnb[tobod][0][2]+pv[1]*cnb[tobod][1][2]+pv[2]*cnb[tobod][2][
      2];
}

void sdrel2cart(int coord,
    int body,
    double point[3],
    double linchg[3],
    double rotchg[3])
{
/* Return derivative of pt loc and body orient w.r.t. hinge rate
*/
    int x,i,gnd;
    double lin[3],pv[3];

    if ((coord < 0) || (coord > 24)) {
        sdseterr(59,45);
        return;
    }
    if (sdchkbnum(59,body) != 0) {
        return;
    }
    if ((roustate != 2) && (roustate != 3)) {
        sdseterr(59,23);
        return;
    }
    gnd = -1;
    if (body == gnd) {
        x = -1;
    } else {
        x = firstq[body]+njntdof[body]-1;
    }
    if (x < coord) {
        sdvset(0.,0.,0.,linchg);
        sdvset(0.,0.,0.,rotchg);
        return;
    }
    sddovpk();
    for (i = 0; i < 3; i++) {
        rotchg[i] = Wpk[coord][x][i];
        lin[i] = Vpk[coord][x][i];
    }
    if (body == gnd) {
        sdvcopy(point,pv);
    } else {
        pv[0] = rcom[body][0]+point[0];
        pv[1] = rcom[body][1]+point[1];
        pv[2] = rcom[body][2]+point[2];
    }
    sdvcross(rotchg,pv,linchg);
    sdvadd(linchg,lin,linchg);
}

void sdacc(int body,
    double pt[3],
    double accel[3])
{
/*
Return linear acceleration a point of the specified body.

*/
    double pv[3];

    if (sdchkbnum(32,body) != 0) {
        return;
    }
    if (roustate != 3) {
        sdseterr(32,24);
        return;
    }
    if (body  ==  -1) {
        accel[0] = 0.;
        accel[1] = 0.;
        accel[2] = 0.;
    } else {
        pv[0] = pt[0]*dyad[body][0][0]+pt[1]*dyad[body][0][1]+pt[2]*dyad[body][0
          ][2];
        pv[1] = pt[0]*dyad[body][1][0]+pt[1]*dyad[body][1][1]+pt[2]*dyad[body][1
          ][2];
        pv[2] = pt[0]*dyad[body][2][0]+pt[1]*dyad[body][2][1]+pt[2]*dyad[body][2
          ][2];
        accel[0] = anb[body][0]+pv[0]*cnb[body][0][0]+pv[1]*cnb[body][0][1]+pv[2
          ]*cnb[body][0][2];
        accel[1] = anb[body][1]+pv[0]*cnb[body][1][0]+pv[1]*cnb[body][1][1]+pv[2
          ]*cnb[body][1][2];
        accel[2] = anb[body][2]+pv[0]*cnb[body][2][0]+pv[1]*cnb[body][2][1]+pv[2
          ]*cnb[body][2][2];
    }
}

void sdangacc(int body,
    double aacc[3])
{
/*
Return angular acceleration of the body.

*/

    if (sdchkbnum(33,body) != 0) {
        return;
    }
    if (roustate != 3) {
        sdseterr(33,24);
        return;
    }
    if (body == -1) {
        aacc[0] = 0.;
        aacc[1] = 0.;
        aacc[2] = 0.;
    } else {
        aacc[0] = onb[body][0];
        aacc[1] = onb[body][1];
        aacc[2] = onb[body][2];
    }
}

void sdgrav(double gravin[3])
{

    sdseterr(1,19);
    roustate = 0;
}

void sdmass(int body,
    double massin)
{

    if (sdchkbnum(2,body) != 0) {
        return;
    }
    if (body == -1) {
        sdseterr(2,15);
        return;
    }
    if (mkq[body] != 0) {
        mk[body] = massin;
        mkq[body] = 3;
    } else {
        sdseterr(2,19);
    }
    roustate = 0;
}

void sdiner(int body,
    double inerin[3][3])
{
    int anyques;

    if (sdchkbnum(3,body) != 0) {
        return;
    }
    if (body == -1) {
        sdseterr(3,15);
        return;
    }
    anyques = 0;
    if (ikq[body][0][0]  !=  0) {
        ik[body][0][0] = inerin[0][0];
        ikq[body][0][0] = 3;
        anyques = 1;
    }
    if (ikq[body][0][1]  !=  0) {
        ik[body][0][1] = inerin[0][1];
        ikq[body][0][1] = 3;
        ik[body][1][0] = inerin[0][1];
        ikq[body][1][0] = 3;
        anyques = 1;
    }
    if (ikq[body][0][2]  !=  0) {
        ik[body][0][2] = inerin[0][2];
        ikq[body][0][2] = 3;
        ik[body][2][0] = inerin[0][2];
        ikq[body][2][0] = 3;
        anyques = 1;
    }
    if (ikq[body][1][1]  !=  0) {
        ik[body][1][1] = inerin[1][1];
        ikq[body][1][1] = 3;
        anyques = 1;
    }
    if (ikq[body][1][2]  !=  0) {
        ik[body][1][2] = inerin[1][2];
        ikq[body][1][2] = 3;
        ik[body][2][1] = inerin[1][2];
        ikq[body][2][1] = 3;
        anyques = 1;
    }
    if (ikq[body][2][2]  !=  0) {
        ik[body][2][2] = inerin[2][2];
        ikq[body][2][2] = 3;
        anyques = 1;
    }
    if (anyques == 0) {
        sdseterr(3,19);
    }
    roustate = 0;
}

void sdbtj(int joint,
    double btjin[3])
{
    int anyques;

    if (sdchkjnum(4,joint) != 0) {
        return;
    }
    anyques = 0;
    if (rkq[joint][0]  !=  0) {
        rk[joint][0] = btjin[0];
        rkq[joint][0] = 3;
        anyques = 1;
    }
    if (rkq[joint][1]  !=  0) {
        rk[joint][1] = btjin[1];
        rkq[joint][1] = 3;
        anyques = 1;
    }
    if (rkq[joint][2]  !=  0) {
        rk[joint][2] = btjin[2];
        rkq[joint][2] = 3;
        anyques = 1;
    }
    if (anyques == 0) {
        sdseterr(4,19);
    }
    roustate = 0;
}

void sditj(int joint,
    double itjin[3])
{
    int anyques;

    if (sdchkjnum(5,joint) != 0) {
        return;
    }
    anyques = 0;
    if (riq[joint][0]  !=  0) {
        ri[joint][0] = itjin[0];
        riq[joint][0] = 3;
        anyques = 1;
    }
    if (riq[joint][1]  !=  0) {
        ri[joint][1] = itjin[1];
        riq[joint][1] = 3;
        anyques = 1;
    }
    if (riq[joint][2]  !=  0) {
        ri[joint][2] = itjin[2];
        riq[joint][2] = 3;
        anyques = 1;
    }
    if (anyques == 0) {
        sdseterr(5,19);
    }
    roustate = 0;
}

void sdpin(int joint,
    int pinno,
    double pinin[3])
{
    int anyques,offs;

    if (sdchkjpin(6,joint,pinno) != 0) {
        return;
    }
    anyques = 0;
    offs = firstq[joint]+pinno;
    if (jtype[joint] == 21) {
        offs = offs+3;
    }
    if (jtype[joint] == 11) {
        offs = offs+1;
    }
    if (pinq[offs][0]  !=  0) {
        pin[offs][0] = pinin[0];
        pinq[offs][0] = 3;
        anyques = 1;
    }
    if (pinq[offs][1]  !=  0) {
        pin[offs][1] = pinin[1];
        pinq[offs][1] = 3;
        anyques = 1;
    }
    if (pinq[offs][2]  !=  0) {
        pin[offs][2] = pinin[2];
        pinq[offs][2] = 3;
        anyques = 1;
    }
    if (anyques == 0) {
        sdseterr(6,19);
    }
    roustate = 0;
}

void sdpres(int joint,
    int axis,
    int presin)
{
    int anyques;

    if (sdchkjaxis(37,joint,axis) != 0) {
        return;
    }
    if ((presin != 0) && (presin != 1)) {
        sdseterr(37,20);
    }
    anyques = 0;
    if (presq[sdindx(joint,axis)]  !=  0) {
        if (presin  !=  0) {
            pres[sdindx(joint,axis)] = 1.;
        } else {
            pres[sdindx(joint,axis)] = 0.;
        }
        presq[sdindx(joint,axis)] = 3;
        anyques = 1;
    }
    if (anyques == 0) {
        sdseterr(37,19);
    }
    wwflg = 0;
}

void sdconschg(void)
{

    wwflg = 0;
}

void sdstab(double velin,
    double posin)
{

    stabvel = velin;
    stabvelq = 3;
    stabpos = posin;
    stabposq = 3;
}

void sdgetgrav(double gravout[3])
{

    gravout[0] = grav[0];
    gravout[1] = grav[1];
    gravout[2] = grav[2];
}

void sdgetmass(int body,
    double *massout)
{

    if (sdchkbnum(40,body) != 0) {
        return;
    }
    if (body == -1) {
        sdseterr(40,15);
        return;
    }
    *massout = mk[body];
}

void sdgetiner(int body,
    double inerout[3][3])
{

    if (sdchkbnum(41,body) != 0) {
        return;
    }
    if (body == -1) {
        sdseterr(41,15);
        return;
    }
    inerout[0][0] = ik[body][0][0];
    inerout[0][1] = ik[body][0][1];
    inerout[0][2] = ik[body][0][2];
    inerout[1][0] = ik[body][1][0];
    inerout[1][1] = ik[body][1][1];
    inerout[1][2] = ik[body][1][2];
    inerout[2][0] = ik[body][2][0];
    inerout[2][1] = ik[body][2][1];
    inerout[2][2] = ik[body][2][2];
}

void sdgetbtj(int joint,
    double btjout[3])
{

    if (sdchkjnum(42,joint) != 0) {
        return;
    }
    btjout[0] = rk[joint][0];
    btjout[1] = rk[joint][1];
    btjout[2] = rk[joint][2];
}

void sdgetitj(int joint,
    double itjout[3])
{

    if (sdchkjnum(43,joint) != 0) {
        return;
    }
    itjout[0] = ri[joint][0];
    itjout[1] = ri[joint][1];
    itjout[2] = ri[joint][2];
}

void sdgetpin(int joint,
    int pinno,
    double pinout[3])
{
    int offs;

    if (sdchkjpin(44,joint,pinno) != 0) {
        return;
    }
    offs = firstq[joint]+pinno;
    if (jtype[joint] == 21) {
        offs = offs+3;
    }
    if (jtype[joint] == 11) {
        offs = offs+1;
    }
    pinout[0] = pin[offs][0];
    pinout[1] = pin[offs][1];
    pinout[2] = pin[offs][2];
}

void sdgetpres(int joint,
    int axis,
    int *presout)
{

    if (sdchkjaxis(45,joint,axis) != 0) {
        return;
    }
    if (pres[sdindx(joint,axis)]  !=  0.) {
        *presout = 1;
    } else {
        *presout = 0;
    }
}

void sdgetstab(double *velout,
    double *posout)
{

    *velout = stabvel;
    *posout = stabpos;
}

void sdinfo(int info[50])
{

    info[0] = ground;
    info[1] = nbod;
    info[2] = ndof;
    info[3] = ncons;
    info[4] = nloop;
    info[5] = nldof;
    info[6] = nloopc;
    info[7] = nball;
    info[8] = nlball;
    info[9] = npres;
    info[10] = nuser;
    info[11] = 12;
/* info entries from 12-49 are reserved */
}

void sdjnt(int joint,
    int info[50],
    int tran[6])
{
    int i,offs;

    if (sdchkjnum(48,joint) != 0) {
        return;
    }
    info[0] = jtype[joint];
    info[1] = 0;
    offs = 0;
    info[2] = inb[joint];
    info[3] = outb[joint];
    info[4] = njntdof[joint];
    info[5] = njntc[joint];
    info[6] = njntp[joint];
    info[7] = firstq[joint];
    info[8] = ballq[joint];
    info[9] = firstm[joint];
    info[10] = firstp[joint];
/* info entries from 11-49 are reserved */

    for (i = 0; i <= 5; i++) {
        if (i  <  njntdof[joint]) {
            tran[i] = trans[offs+firstq[joint]+i];
        } else {
            tran[i] = -1;
        }
    }
}

void sdcons(int consno,
    int info[50])
{

    if (sdchkucnum(49,consno) != 0) {
        return;
    }
    info[0] = 1;
    info[1] = firstu[consno];
/* info entries from 2-49 are reserved */
}

void sdgentime(int *gentm)
{

    *gentm = 93540;
}
/*
Done. CPU seconds used: 0.41  Memory used: 1687552 bytes.
Equation complexity:
  sdstate:   506 adds   688 multiplies     0 divides   784 assignments
  sdderiv: 18983 adds 20014 multiplies   214 divides 20311 assignments
  sdresid:  1659 adds  1569 multiplies     0 divides  1475 assignments
  sdreac:    594 adds   299 multiplies     0 divides   546 assignments
  sdmom:     229 adds   312 multiplies     0 divides    20 assignments
  sdsys:     286 adds   424 multiplies     0 divides    34 assignments
*/
