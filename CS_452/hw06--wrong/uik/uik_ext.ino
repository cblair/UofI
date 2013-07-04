/*UIK external / user acessible functions*/

#include "uik_stack.h"

/*define if using the AVR, as Arduino otherwise does this stuff automatically*/
//#define USE_AVR

#ifdef USE_AVR
extern void setup();
#endif

extern uint16_t UIKTickLen;


/*Tells index where UIKAddTask should add the next task*/
uint16_t UIKAddTaskI = 0;


/*
 * UIKInitialize
 *
 * @param ticklen The length in microseconds
 * @param MaxTasks The maximum amount of tasks allowed
 * 
 * Note: to have a ticklen big enough for seconds (i.e. 1000000), change
 *       type to uint64_t ticklen.
 */
void UIKInitialize(uint64_t ticklen, uint8_t MaxTasks)
{
  #ifdef USE_AVR
  setup();
  #endif
  
  /* UIKTickLen
   * currently, a ticklen of 250 ~ 1 second, 1,000 ms, or 1,000,000 
   * microseconds. So we scale the value so ticklen = microseconds.
   */
  //ticklen /= 1000000; //TODO: took out to do seconds
  ticklen = (ticklen / 4);

  UIKTickLen = ticklen;
  
  MaxStackSize = MaxTasks;
}


uint8_t UIKAddTask(void(* task)(void), uint8_t priority /*,stack*/)
{
  uint16_t i = UIKAddTaskI;
  
  /*task*/
  TaskStack[i].task_p = task;
  
  /*priority*/
  TaskStack[i].priority = priority;
  
  /*state*/
  TaskStack[i].task_state = TREADY; /*ready to run by default*/
  
  /*tid*/
  TaskStack[i].tid = i;
  
  /*Done with this index, increment for the next time we add*/
  UIKAddTaskI++;
  
  /*TODO: return what?*/
  return(0);
}


void UIKRun(uint8_t taskid)
{
  TaskStack[taskid].task_p();
}


void UIKSchedule(void)
{
  
}
