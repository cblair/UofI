#ifndef __ADOS_INCLUDE
#define __ADOS_INCLUDE

#ifdef __cplusplus
extern "C"{
#endif

#if !defined(NULL)
    #define NULL 0
#endif

typedef enum UIK_status_t
{
    ADOS_STATUS_RESERVED,
    ADOS_STATUS_OK,
    ADOS_STATUS_ERROR,
    ADOS_STATUS_BAD_PARAM,
    ADOS_STATUS_NOT_READY,
}UIK_status_t;

typedef enum UIK_state_t
{
    ADOS_STATE_RESERVED,
    ADOS_STATE_READY_TO_RUN,
    ADOS_STATE_NOT_READY_TO_RUN,
    ADOS_STATE_SLEEP,
}UIK_state_t;

typedef unsigned long UIK_timestamp_t;
typedef unsigned char UIK_priority_t;

typedef void(*UIK_taskptr_t)();

typedef struct UIK_tcb_t
{
    unsigned char*  m_stack;
    UIK_taskptr_t  m_taskptr;
    UIK_priority_t  m_priority;
    UIK_timestamp_t  m_nexttimetorun;
    UIK_state_t  m_state;
    volatile struct UIK_tcb_t* m_next;
    volatile struct UIK_event_t* m_waitingfor;
}UIK_tcb_t;

typedef struct UIK_event_t
{
    unsigned char   m_state;
    volatile void*  m_param;
}UIK_event_t;

typedef struct UIK_ctrl_t
{
    unsigned char*          m_stack;
    volatile UIK_tcb_t*    m_firstcb;
    volatile UIK_tcb_t*    m_currenttcb;
}UIK_ctrl_t;

void UIK_init();
void UIK_start();

UIK_status_t UIK_addTask(volatile UIK_tcb_t* tcb, UIK_taskptr_t taskptr,
    unsigned char* stack, unsigned int stacksize, UIK_priority_t priority);
void UIK_sleep(unsigned long msec);

unsigned char UIK_eventTest(volatile UIK_event_t* event);
volatile void* UIK_eventWaitFor(volatile UIK_event_t* event, unsigned long timeout);
void UIK_eventPulse(volatile UIK_event_t* event, volatile void* param);
void UIK_eventSet(volatile UIK_event_t* event, volatile void* param);
void UIK_eventReset(volatile UIK_event_t* event);

#ifdef __cplusplus
} // extern "C"
#endif

#endif //__ADOS_INCLUDE
