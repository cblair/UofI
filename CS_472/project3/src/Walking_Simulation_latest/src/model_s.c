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
#include <math.h>

/* These routines are passed to sdroot. */

void sdposfunc(double vars[25],
    double param[1],
    double resid[12])
{
    int i;
    double pos[25],vel[25];

    for (i = 0; i < 25; i++) {
        vel[i] = 0.;
    }
    sdang2st(vars,pos);
    sdstate(param[0],pos,vel);
    sdumotion(param[0],pos,vel);
    sdperr(resid);
}

void sdvelfunc(double vars[25],
    double param[26],
    double resid[12])
{

    sdstate(param[25],param,vars);
    sdumotion(param[25],param,vars);
    sdverr(resid);
}

void sdstatfunc(double vars[25],
    double param[26],
    double resid[37])
{
    double pos[25],qdotdum[25];

    sdang2st(vars,pos);
    sdstate(param[25],pos,param);
    sdumotion(param[25],pos,param);
    sduforce(param[25],pos,param);
    sdperr(resid);
    sdderiv(qdotdum,&resid[12]);
}

void sdstdyfunc(double vars[50],
    double param[1],
    double resid[49])
{
    double pos[25],qdotdum[25];

    sdang2st(vars,pos);
    sdstate(param[0],pos,&vars[25]);
    sdumotion(param[0],pos,&vars[25]);
    sduforce(param[0],pos,&vars[25]);
    sdperr(resid);
    sdverr(&resid[12]);
    sdderiv(qdotdum,&resid[24]);
}

/* This routine is passed to the integrator. */

void sdmotfunc(double time,
    double state[50],
    double dstate[50],
    double param[1],
    int *status)
{
    double err[12];
    int i;

    sdstate(time,state,&state[25]);
    sdumotion(time,state,&state[25]);
    sduforce(time,state,&state[25]);
    sdderiv(dstate,&dstate[25]);
    *status = 1;
    sdverr(err);
    for (i = 0; i < 12; i++) {
        if (fabs(err[i]) > param[0]) {
            return;
        }
    }
    sdperr(err);
    for (i = 0; i < 12; i++) {
        if (fabs(err[i]) > param[0]) {
            return;
        }
    }
    *status = 0;
}

/* This routine performs assembly analysis. */

void sdassemble(double time,
    double state[50],
    int lock[25],
    double tol,
    int maxevals,
    int *fcnt,
    int *err)
{
    double perrs[12],param[1];
    int i;
    double jw[300],dw[2738],rw[309];
    int iw[148],rooterr;

    sdgentime(&i);
    if (i != 93540) {
        sdseterr(50,42);
    }
    param[0] = time;
    sdroot(sdposfunc,state,param,12,25,0,lock,tol,tol,maxevals,
      jw,dw,rw,iw,perrs,fcnt,&rooterr);
    sdposfunc(state,param,perrs);
    *fcnt = *fcnt+1;
    if (rooterr == 0) {
        *err = 0;
    } else {
        if (*fcnt >= maxevals) {
            *err = 2;
        } else {
            *err = 1;
        }
    }
}

/* This routine performs initial velocity analysis. */

void sdinitvel(double time,
    double state[50],
    int lock[25],
    double tol,
    int maxevals,
    int *fcnt,
    int *err)
{
    double verrs[12],param[26];
    int i;
    double jw[300],dw[2738],rw[309];
    int iw[148],rooterr;

    sdgentime(&i);
    if (i != 93540) {
        sdseterr(51,42);
    }
    for (i = 0; i < 25; i++) {
        param[i] = state[i];
    }
    param[25] = time;
    sdroot(sdvelfunc,&state[25],param,12,25,0,lock,tol,tol,maxevals,
      jw,dw,rw,iw,verrs,fcnt,&rooterr);
    sdvelfunc(&state[25],param,verrs);
    *fcnt = *fcnt+1;
    if (rooterr == 0) {
        *err = 0;
    } else {
        if (*fcnt >= maxevals) {
            *err = 2;
        } else {
            *err = 1;
        }
    }
}

/* This routine performs static analysis. */

void sdstatic(double time,
    double state[50],
    int lock[25],
    double ctol,
    double tol,
    int maxevals,
    int *fcnt,
    int *err)
{
    double resid[37],param[26],jw[925],dw[7688],rw[484];
    int iw[248],rooterr,i;

    sdgentime(&i);
    if (i != 93540) {
        sdseterr(52,42);
    }
    for (i = 0; i < 25; i++) {
        param[i] = state[25+i];
    }
    param[25] = time;
    sdroot(sdstatfunc,state,param,37,25,25,lock,
      ctol,tol,maxevals,jw,dw,rw,iw,resid,fcnt,&rooterr);
    sdstatfunc(state,param,resid);
    *fcnt = *fcnt+1;
    if (rooterr == 0) {
        *err = 0;
    } else {
        if (*fcnt >= maxevals) {
            *err = 2;
        } else {
            *err = 1;
        }
    }
}

/* This routine performs steady motion analysis. */

void sdsteady(double time,
    double state[50],
    int lock[50],
    double ctol,
    double tol,
    int maxevals,
    int *fcnt,
    int *err)
{
    double resid[49],param[1];
    double jw[2450],dw[19602],rw[793];
    int iw[396],rooterr,i;

    sdgentime(&i);
    if (i != 93540) {
        sdseterr(53,42);
    }
    param[0] = time;
    sdroot(sdstdyfunc,state,param,49,50,25,lock,
      ctol,tol,maxevals,jw,dw,rw,iw,resid,fcnt,&rooterr);
    sdstdyfunc(state,param,resid);
    *fcnt = *fcnt+1;
    if (rooterr == 0) {
        *err = 0;
    } else {
        if (*fcnt >= maxevals) {
            *err = 2;
        } else {
            *err = 1;
        }
    }
}

/* This routine performs state integration. */

void sdmotion(double *time,
    double state[50],
    double dstate[50],
    double dt,
    double ctol,
    double tol,
    int *flag,
    int *err)
{
    static double step;
    double work[300],ttime,param[1];
    int vintgerr,which,ferr,i;

    sdgentime(&i);
    if (i != 93540) {
        sdseterr(54,42);
    }
    param[0] = ctol;
    ttime = *time;
    if (*flag != 0) {
        sdmotfunc(ttime,state,dstate,param,&ferr);
        step = dt;
        *flag = 0;
    }
    if (step <= 0.) {
        step = dt;
    }
    sdvinteg(sdmotfunc,&ttime,state,dstate,param,dt,&step,50,tol,work,&vintgerr,
      &which);
    *time = ttime;
    *err = vintgerr;
}

/* This routine performs state integration with a fixed-step integrator. */

void sdfmotion(double *time,
    double state[50],
    double dstate[50],
    double dt,
    double ctol,
    int *flag,
    double *errest,
    int *err)
{
    double work[200],ttime,param[1];
    int ferr,i;

    sdgentime(&i);
    if (i != 93540) {
        sdseterr(55,42);
    }
    param[0] = ctol;
    *err = 0;
    ttime = *time;
    if (*flag != 0) {
        sdmotfunc(ttime,state,dstate,param,&ferr);
        *flag = 0;
    }
    sdfinteg(sdmotfunc,&ttime,state,dstate,param,dt,50,work,errest,&ferr);
    if (ferr != 0) {
        *err = 1;
    }
    *time = ttime;
}
