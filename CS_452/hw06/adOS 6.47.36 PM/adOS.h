#ifndef __ADOS_INCLUDE
#define __ADOS_INCLUDE
/*
  adOS - Cooperative multitasking scheduler for Arduino.
  Copyright (c) 2010 Alexander P. Kolesnikov. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifdef __cplusplus
extern "C"{
#endif

#if !defined(NULL)
    #define NULL 0
#endif

typedef enum ados_status_t
{
    ADOS_STATUS_RESERVED,
    ADOS_STATUS_OK,
    ADOS_STATUS_ERROR,
    ADOS_STATUS_BAD_PARAM,
    ADOS_STATUS_NOT_READY,
}ados_status_t;

typedef enum ados_state_t
{
    ADOS_STATE_RESERVED,
    ADOS_STATE_READY_TO_RUN,
    ADOS_STATE_NOT_READY_TO_RUN,
    ADOS_STATE_SLEEP,
}ados_state_t;

typedef unsigned long ados_timestamp_t;
typedef unsigned char ados_priority_t;

typedef void(*ados_taskptr_t)();

typedef struct ados_tcb_t
{
    unsigned char*  m_stack;
    ados_taskptr_t  m_taskptr;
    ados_priority_t  m_priority;
    ados_timestamp_t  m_nexttimetorun;
    ados_state_t  m_state;
    volatile struct ados_tcb_t* m_next;
    volatile struct ados_event_t* m_waitingfor;
}ados_tcb_t;

typedef struct ados_event_t
{
    unsigned char   m_state;
    volatile void*  m_param;
}ados_event_t;

typedef struct ados_ctrl_t
{
    unsigned char*          m_stack;
    volatile ados_tcb_t*    m_firstcb;
    volatile ados_tcb_t*    m_currenttcb;
}ados_ctrl_t;

void ados_init();
void ados_start();

ados_status_t ados_addTask(volatile ados_tcb_t* tcb, ados_taskptr_t taskptr,
    unsigned char* stack, unsigned int stacksize, ados_priority_t priority);
void ados_sleep(unsigned long msec);

unsigned char ados_eventTest(volatile ados_event_t* event);
volatile void* ados_eventWaitFor(volatile ados_event_t* event, unsigned long timeout);
void ados_eventPulse(volatile ados_event_t* event, volatile void* param);
void ados_eventSet(volatile ados_event_t* event, volatile void* param);
void ados_eventReset(volatile ados_event_t* event);

#ifdef __cplusplus
} // extern "C"
#endif

#endif //__ADOS_INCLUDE
