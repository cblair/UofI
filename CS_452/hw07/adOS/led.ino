#include <util/delay.h>

unsigned int LedMode = LOW;

uint8_t led_inited = 0; /*Has the led env been initialized (let_setup()) yet?*/


void led_init()
{
  pinMode(2, OUTPUT);   /*sets the digital pin as output*/
  pinMode(12, OUTPUT);   /*sets the digital pin as output*/
  pinMode(11, OUTPUT);   /*sets the digital pin as output*/
  
  led_inited = 1;            /*no longer need setup*/
}


void led_update_delay(void)
{ 
      pinMode(13, OUTPUT);
      digitalWrite(13, LOW);   // set the LED
      _delay_ms(50);
      digitalWrite(13, HIGH);   // set the LED
      _delay_ms(50);
}


void led_update_delay2(void)
{ 
      pinMode(2, OUTPUT);
      digitalWrite(2, LOW);   // set the LED
      _delay_ms(50);
      digitalWrite(2, HIGH);   // set the LED
      _delay_ms(50);
}

void led_update_delay3(void)
{ 
      pinMode(3, OUTPUT);
      digitalWrite(3, LOW);   // set the LED
      _delay_ms(50);
      digitalWrite(3, HIGH);   // set the LED
      _delay_ms(50);
}


void led_update_delay13(void)
{ 
      pinMode(13, OUTPUT);
      digitalWrite(13, LOW);   // set the LED
      _delay_ms(50);
      digitalWrite(13, HIGH);   // set the LED
      _delay_ms(50);
}
