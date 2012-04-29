#include <util/delay.h>

unsigned int LedMode = LOW;

int ledPin = 13;  /* LED connected to digital pin 13*/

uint8_t led_inited = 0; /*Has the led env been initialized (let_setup()) yet?*/


void led_setup()
{
  pinMode(ledPin, OUTPUT);   /*sets the digital pin as output*/
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
    digitalWrite(13, LOW);   // set the LED
    _delay_ms(50);
    digitalWrite(13, HIGH);   // set the LED
    _delay_ms(50);
}


/*Test function to hog run time from above*/
void led_hog(void)
{
  /*
  if(led_inited == 0)
  {
    led_setup();
  }
  
  digitalWrite(ledPin, HIGH);   // sets the LED on
  delay(1000);                  // waits for a second
  digitalWrite(ledPin, LOW);    // sets the LED off
  delay(1000);                  // waits for a second
  */
  
    digitalWrite(12, LOW);   // set the LED
    _delay_ms(50);
    digitalWrite(12, HIGH);   // set the LED
    _delay_ms(50);
}
