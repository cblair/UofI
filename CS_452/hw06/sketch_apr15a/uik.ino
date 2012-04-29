unsigned int LedMode = LOW;

/* Timer reload value, globally available */
unsigned int tcnt2;
int count1 = 0;

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
  tcnt2 = 248;
  
  /* Finally load end enable the timer */
  TCNT2 = tcnt2;
  TIMSK2 |= (1<<TOIE2);
} // END init_Ex1


/*Interrupt callback*/
ISR(TIMER2_OVF_vect) 
{
  count1++;
  
  if (count1 == 60)
  {
    count1 = 0;
    digitalWrite(13, LedMode);   // set the LED

    if(LedMode == HIGH)
    {
      LedMode = LOW;
    } 
    else
    {
     LedMode = HIGH; 
    }
  }
}


void loop()
{
  /*Just interrupts for now*/
}
