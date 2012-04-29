/*UIK internal / hidden functions*/

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
extern uint16_t UIKCurrentTaskI;

/*
 * How many times the timer overflows
 */
static uint64_t UIKTimerCnt = 0;


/*This is the scheduler from class*/
void UIKDispatcher(void)
{
  /*Suspend the current task*/
  //TODO
  
  /*Increment index to next task*/
  UIKCurrentTaskI++;  
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
  
}


/*Interrupt stuff*/
void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);     

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
  UIKTimerCnt++;
  
  /*if time to run new task*/
  if(UIKTimerCnt >= UIKTickLen)
  {
    /*reset the count of how many times the timer has overflowed*/
    UIKTimerCnt = 0; 
    
    /*Increment the count of how many ticks have occured*/
    UIKTickNum++;

    led_update();
  }
}
