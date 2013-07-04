/*UIK internal / hidden functions*/

#include "uik_context.h"

/* 32 bit global variable that is incremented
 * by the tick handler. It is a count of the
 * number of ticks that have occured so far.
 */
static uint32_t UIKTickNum = 0;

/* Variable that holds the the length of the 
 * tick, in microseconds.
 */
uint16_t UIKTickLen = 0;

/* Index of current task being run*/
extern uint8_t UIKCurrentTaskI;

/*
 * How many times the timer overflows
 */
static uint64_t UIKTimerCnt = 0;


/*This is the scheduler from class*/
void UIKDispatcher(void)
{ 
  /*Get the next task to run*/
  UIKCurrentTaskI = UIKGetMaxPriority();
}


/*
 * Executers when there is nothing else to do. 
 * Could just be the lowest priority task.
 */
void UIKIdle(void)
{
  
}


/*Device driver for the UIK main tick timer*/
void UIKTickHandler(void)
{
  UIKTimerCnt++;
  
  /*if time to run new task*/
  if(UIKTimerCnt >= UIKTickLen)
  {
    /*reset the count of how many times the timer has overflowed*/
    UIKTimerCnt = 0; 
    
    /*Increment the count of how many ticks have occured*/
    UIKTickNum++;
    
    if(digitalRead(10) == LOW)
    {
     TaskStack[1].priority = 10; 
    }
    else
    {
     TaskStack[1].priority = 1;  
    }
    
    UIKDispatcher();
  }
}


/*
 * UIKGetMaxPriority
 *
 * Gets the task with the max priority (duh). Tie goes to the 
 * task that got added first.
 */
uint8_t UIKGetMaxPriority(void)
{
  /*
   * Increment the index of the task that should be run by default. If the 
   * incemented index is the same value as where to add new tasks, it is too 
   * big, and should go back to the start.
   */
  uint8_t maxi = UIKCurrentTaskI + 1;
  if(maxi >= UIKAddTaskI)
  {
   maxi = 0; 
  }
  uint8_t max_val = TaskStack[maxi].priority;
  
  uint8_t i;
  for(i = 0; i < MaxStackSize; i++)
  {
   if(TaskStack[i].priority > max_val)
   {
     max_val = TaskStack[i].priority;
     maxi = i; 
   }
  }
  
  return(maxi);
}


/*
 * Internal interrupt stuff. Automatically gets called on init in Arduino,
 * otherwise you should #define USE_AVR to have UIKInitialize call this
 * on init.
 */
void setup() {                
  pinMode(10, INPUT);
  digitalWrite(10, HIGH);   

  /* First disable the timer overflow interrupt*/
  TIMSK2 &= ~(1<<TOIE2);

  /* Configure timer2 in normal mode (no PWM) */
  TCCR2A &= ~((1<<WGM21) | (1<<WGM20));
  TCCR2B &= ~(1<<WGM22);

  /* Select clock source: internal I/O clock */
  ASSR &= ~(1<<AS2);

  /* Disable Compare Match A interrupt (only overflow) */
  TIMSK2 &= ~(1<<OCIE2A);

  /* Configure the prescaler to CPU clock divided by 128 */
  TCCR2B |= (1<<CS22)  | (1<<CS20); // Set bits
  TCCR2B &= ~(1<<CS21);             // Clear bit

  /* We need to calculate a proper value to load the counter.
  * The following loads the value 248 into the Timer 2 counter
  * The math behind this is:
  * (Desired period) = 64us.
  * (CPU frequency) / (prescaler value) = 125000 Hz -> 8us.
  * (desired period) / 8us = 8.
  * MAX(uint8) - 8 = 248;
  */
  /* Save value globally for later reload in ISR */
  
  /* Finally load end enable the timer */
  //TCNT2 = tcnt2;
  TCNT2 = 248;
  TIMSK2 |= (1<<TOIE2);
} // END init_Ex1


/*Interrupt callback*/
ISR(TIMER2_OVF_vect) 
{
  UIKContextSave();
  
  UIKTickHandler();
}
