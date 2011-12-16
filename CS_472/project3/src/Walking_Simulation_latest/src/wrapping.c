/*******************************************************************************

   WRAPPING.C

   Authors: Ken Smith
            Peter Loan

   Date: 23-SEP-99

   Copyright (c) 1999-2001 MusculoGraphics, a division of Motion Analysis Corp.
   All rights reserved.

   Description: This file contains routines that deal with wrapping
      muscle points. A wrapping point is a muscle point which is
      active (turned on) only for certain joint angles. They are a
      temporary fix for muscles that wrap around bone. Specifically
      these routines determine which wrapping points in a given
      muscle should be turned on, and if one is turned off, what the
      new 'current point' should be, in the muscle editor tool.

   Routines:
      check_wrapping_points     : determines which points in a muscle are active
      check_wrap_objects        : checks for intersection between muscle and wrap object
      wrap_muscle_seg           : wraps a muscle segment over a wrap object
      calc_muscle_length_change : calculates the change in length due to wrapping

*******************************************************************************/

#include "universal.h"

/*************** DEFINES (for this file only) *********************************/

/*************** STATIC GLOBAL VARIABLES (for this file only) *****************/

/*************** EXTERNED VARIABLES (declared in another file) ****************/
extern SDModelStruct sdm;

/*************** PROTOTYPES for STATIC FUNCTIONS (for this file only) *********/
static SBoolean wrap_muscle_seg (MuscleStruct* ms, WrapObject* wo, WrapParams* wp);
static double calc_muscle_length_change (MuscleStruct* ms, WrapObject* wo, WrapParams* wp);
static void check_wrap_objects (MuscleStruct* ms);


/* -------------------------------------------------------------------------
   check_wrapping_points - traverse the muscle's list of muscle points,
      updating wrapping points if necessary.
---------------------------------------------------------------------------- */
void check_wrapping_points(MuscleStruct* ms, double state[])
{

   int i, j, genc;
   double value;

   /* If the wrapping points have already been calculated, just return. */

   if (ms->wrap_calced == yes)
      return;

   if (ms->has_wrapping_points == yes)
   {
      /* If the current gencoord values
       * fall within all of the specified ranges for the point, turn the point
       * on; else turn it off.
       */

      for (i=0; i<*(ms->num_orig_points); i++)
      {
         ms->mp_orig[i].state = on;
         for (j=0; j<ms->mp_orig[i].numranges; j++)
         {
            genc = ms->mp_orig[i].ranges[j].genc;
            value = state[genc] * sdm.q[genc].conversion;
            if (value < ms->mp_orig[i].ranges[j].start ||
               value > ms->mp_orig[i].ranges[j].end)
            {
               ms->mp_orig[i].state = off;
               break;
            }
         }
      }
   }

   //check_wrap_objects(ms);

   /* Now find the first active point */

   for (i=0; i<ms->num_points; i++)
   {
      if (ms->mp[i]->state == on)
      {
         ms->first_on_point = i;
         return;
      }
   }

   ms->first_on_point = -1;

} /* check_wrapping_points */


#if 0
/* -------------------------------------------------------------------------
   check_wrap_objects - check the muscle for intersection with its associated
      wrap object.  Currently a muscle can wrap over only one wrap object.
      If this is a multi-segment muscle, then choose the "best" muscle segment
      to wrap over the wrap object.
---------------------------------------------------------------------------- */
static void check_wrap_objects(MuscleStruct* ms)
{

   int i, start, end, pt1, pt2;
   int wrapStart, wrapEnd;
   double min_length_change = DBL_MAX;
   WrapParams best_wrap = { -1, -1, NULL };
   WrapObject* wo;

   if (ms->muscwrap == NULL || ms->muscwrap->wrap_object == NULL)
      return;

   wo = ms->muscwrap->wrap_object;

   /* find the first active point: */
   for (i = 0; i < *(ms->num_orig_points); i++)
      if (ms->mp_orig[i].state == on)
         break;
      
   if (i == *ms->num_orig_points)
      return;

   if (ms->muscwrap->startPoint < 1)
      wrapStart = 0;
   else
      wrapStart = ms->muscwrap->startPoint - 1;

   if (ms->muscwrap->endPoint < 1)
      wrapEnd = *ms->num_orig_points - 1;
   else
      wrapEnd = ms->muscwrap->endPoint - 1;

   start = MAX(wrapStart, i);
   end = wrapEnd;

   /* loop through muscle segments in the user-specified range, choosing the best
    * wrap as the one that changes the muscle segment length the least:
    */
   for (pt1 = start; 1; pt1 = pt2)
   {
      pt2 = pt1 + 1;

      while (pt2 < *ms->num_orig_points && ms->mp_orig[pt2].state == off)
         pt2++;

      if (pt2 >= *ms->num_orig_points || pt2 > end)
         break;

      if (ms->mp_orig[pt1].segment != ms->mp_orig[pt2].segment)
      {
         WrapParams wp = { -1, -1, NULL };

         wp.start = pt1;
         wp.end   = pt2;

         if (wrap_muscle_seg(ms, wo, &wp))
         {
            double muscle_length_change = calc_muscle_length_change(ms, wo, &wp);

            if (muscle_length_change < min_length_change)
            {
               if (best_wrap.wrap_pts != NULL)
                  free(best_wrap.wrap_pts);

               best_wrap = wp;

               min_length_change = muscle_length_change;
            }
         }
      }
   }

   /* deallocate previous wrapping points if necessary: */
   if (ms->mp_wrap[1].wrap_pts != NULL)
   {
      free(ms->mp_wrap[1].wrap_pts);
      ms->mp_wrap[1].wrap_pts = NULL;
   }
   
   if (best_wrap.wrap_pts == NULL)
   {
      /* if no wrapping occurred... */
      for (i = 0; i < *ms->num_orig_points; i++)
         ms->mp[i] = &ms->mp_orig[i];

      ms->num_points = *ms->num_orig_points;

      if (wo)
      {
         for (i = 0; i < 3; i++)
         {
            ms->muscwrap->c[i]  = MINMDOUBLE;
            ms->muscwrap->r1[i] = MINMDOUBLE;
            ms->muscwrap->r2[i] = MINMDOUBLE;
         }
      }
   }
   else
   {
      /* if wrapping did occur, copy wrap information into the MuscleStruct: */
      ms->mp_wrap[0].wrap_pts = NULL;
      ms->mp_wrap[0].num_wrap_pts = 0;
      
      ms->mp_wrap[1].wrap_pts = best_wrap.wrap_pts;
      ms->mp_wrap[1].num_wrap_pts = best_wrap.num_wrap_pts;
#if SIMM
      for (i = 0; i < ms->mp_wrap[1].num_wrap_pts; i++)
      {
         convert_from_wrap_object_frame(wo, &ms->mp_wrap[1].wrap_pts[i * 3]);

         convert(ms->modelnum, &ms->mp_wrap[1].wrap_pts[i * 3], wo->segment,
                 ms->ground_segment);
      }
#endif
      ms->mp_wrap[0].segment = wo->segment;
      ms->mp_wrap[1].segment = wo->segment;
      ms->mp_wrap[0].state = on;
      ms->mp_wrap[1].state = on;
      ms->mp_wrap[0].is_auto_wrap_point = yes;
      ms->mp_wrap[1].is_auto_wrap_point = yes;
      ms->mp_wrap[0].wrap_distance = 0.0;
      ms->mp_wrap[1].wrap_distance = best_wrap.wrap_path_length;
      
      for (i = 0; i < 3; i++)
      {
         ms->mp_wrap[0].point[i] = best_wrap.r1[i];
         ms->mp_wrap[1].point[i] = best_wrap.r2[i];
      }
      
      for (i = 0; i < best_wrap.end; i++)
         ms->mp[i] = &ms->mp_orig[i];
      
      ms->mp[best_wrap.end]   = &ms->mp_wrap[0];
      ms->mp[best_wrap.end+1] = &ms->mp_wrap[1];
      
      for (i = best_wrap.end; i < *ms->num_orig_points; i++)
         ms->mp[i+2] = &ms->mp_orig[i];
      
      ms->num_points = *ms->num_orig_points + 2;
   }
} /* check_wrap_objects */


/* -------------------------------------------------------------------------
   wrap_muscle_seg - 
---------------------------------------------------------------------------- */
static SBoolean wrap_muscle_seg (MuscleStruct* ms, WrapObject* wo, WrapParams* wp)
{

   static double origin[] = {0,0,0}, axis[] = {0,0,1};
   int i, p_flag, error_code;
   double pt1[4], pt2[4], npt1[4], npt2[4], tpt1[4], tpt2[4];
   SBoolean didWrap = no;

   for (i = 0; i < 3; i++)
   {
      pt1[i] = ms->mp_orig[wp->start].point[i];
      pt2[i] = ms->mp_orig[wp->end].point[i];
   }

   convert_point(pt1,ms->mp_orig[wp->start].segment,wo->segment);
   convert_point(pt2,ms->mp_orig[wp->end].segment,wo->segment);

   pt1[3] = pt2[3] = 1.0;
   mult_4x4matrix_by_vector(wo->to_local_xform,pt1,npt1);
   mult_4x4matrix_by_vector(wo->to_local_xform,pt2,npt2);

   if (wo->wrap_type == wrap_sphere)
   {
       error_code = calc_line_intersect_sphere(npt1, npt2,
                                               origin, wo->radius[0],
                                               &wp->wrap_path_length,
                                               tpt1, tpt2,
                                               &wp->wrap_pts, &wp->num_wrap_pts,
                                               wo->wrap_axis, wo->wrap_sign,
                                               ms->muscwrap, &p_flag, wo);
   }
   else if (wo->wrap_type == wrap_ellipsoid)
   {
       error_code = calc_line_intersect_ellipsoid(npt1, npt2,
                                                  origin, wo->radius,
                                                  &wp->wrap_path_length,
                                                  tpt1, tpt2,
                                                  &wp->wrap_pts, &wp->num_wrap_pts,
                                                  wo->wrap_axis, wo->wrap_sign,
                                                  ms->muscwrap, &p_flag, wo);
   }
   else if (wo->wrap_type == wrap_cylinder)
   {
       error_code = calc_line_intersect_cylinder(npt1, npt2,
                                                 origin, axis, wo->radius[0],
                                                 &wp->wrap_path_length,
                                                 tpt1, tpt2,
                                                 &wp->wrap_pts, &wp->num_wrap_pts,
                                                 wo->wrap_axis, wo->wrap_sign,
                                                 ms->muscwrap, &p_flag, wo);
   }
   else
   {
       error_code = -1;
       p_flag = 0;
   }

   if (p_flag == 1 && error_code == 0)
   {
      tpt1[3] = tpt2[3] = 1.0;
      mult_4x4matrix_by_vector(wo->from_local_xform,tpt1,wp->r1);
      mult_4x4matrix_by_vector(wo->from_local_xform,tpt2,wp->r2);
      didWrap = yes;
   }
   return didWrap;

} /* wrap_muscle_seg */

/* -------------------------------------------------------------------------
   calc_muscle_length_change - given the output of a successful muscle wrap
      over a wrap object, determine the percent change in length of the
      muscle segment incurred by wrapping.
---------------------------------------------------------------------------- */
static double calc_muscle_length_change (MuscleStruct* ms, WrapObject* wo, WrapParams* wp)
{
   int i;

   double p1[3], p2[3], straight_length, wrap_length = 0.0;

   for (i = 0; i < 3; i++)
   {
      p1[i] = ms->mp_orig[wp->start].point[i];
      p2[i] = ms->mp_orig[wp->end].point[i];
   }
   convert_point(p1,ms->mp_orig[wp->start].segment,wo->segment);
   convert_point(p2,ms->mp_orig[wp->end].segment,wo->segment);

   straight_length = sqrt(distancesqr_between_vertices(p1, p2));

   wrap_length += sqrt(distancesqr_between_vertices(p1, wp->r1));
   wrap_length += wp->wrap_path_length;
   wrap_length += sqrt(distancesqr_between_vertices(wp->r2, p2));

   return wrap_length / straight_length;
}

#endif
