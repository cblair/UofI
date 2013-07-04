#include <util/delay.h>

unsigned int LedMode = LOW;

uint8_t led_inited = 0; /*Has the led env been initialized (let_setup()) yet?*/


void led_init()
{
  /*Haven't found these to really be necessary. Doing them anyway.*/
  pinMode(13, OUTPUT);   /*sets the digital pin as output*/
  pinMode(12, OUTPUT);   /*sets the digital pin as output*/
  pinMode(11, OUTPUT);   /*sets the digital pin as output*/
  
  led_inited = 1;            /*no longer need setup*/
}


/*
 * Counts on timers for delays, don't want to use with RTOS scheduling.
 * Uses on board LED, doesn't need led_setup() above.
 */
void led_update(void)
{
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


/*
 * Use this with RTOS scheduling.
 * Uses on board LED, doesn't need led_setup() above.
 */
void led_update_delay(void)
{
    if(led_inited == 0)
    {
     led_init(); 
    }
  
    while(1)
    {
      digitalWrite(13, LOW);   // set the LED
      _delay_ms(50);
      digitalWrite(13, HIGH);   // set the LED
      _delay_ms(50);
    }
}


/*Test function to hog run time from above*/
void led_hog(void)
{
    digitalWrite(12, LOW);   // set the LED
    _delay_ms(50);
    digitalWrite(12, HIGH);   // set the LED
    _delay_ms(50);
}


/*Test function to hog run time from above*/
void led_hog2(void)
{
    digitalWrite(11, LOW);   // set the LED
    _delay_ms(50);
    digitalWrite(11, HIGH);   // set the LED
    _delay_ms(50);
}
