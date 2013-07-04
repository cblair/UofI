#include <util/delay.h>

uint8_t counter_inited = 0; /*Has the led env been initialized (let_setup()) yet?*/


void counter_init()
{
  /*Haven't found these to really be necessary. Doing them anyway.*/
  pinMode(7, OUTPUT);   /*sets the digital pin as output*/
  pinMode(6, OUTPUT);   /*sets the digital pin as output*/
  pinMode(5, OUTPUT);   /*sets the digital pin as output*/
  pinMode(4, OUTPUT);   /*sets the digital pin as output*/
  
  counter_inited = 1;            /*no longer need setup*/
}


void uik_counter(void)
{
  uint8_t cnt = 0;
  
  if(counter_inited == 0)
  {
    counter_init();
  }
  
  
  while(1)
  {
    if(cnt == 0)
    {
      digitalWrite(4, HIGH);   // set the LED
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      cnt++;
    }
    else if(cnt == 1)
    {
      digitalWrite(4, LOW);   // set the LED
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      cnt++;
    }
    else if(cnt == 2)
    {
      digitalWrite(4, LOW);   // set the LED
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      cnt++;
    }
    else if(cnt == 3)
    {
      digitalWrite(4, LOW);   // set the LED
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);      
      cnt = 0;                //reset cnt
    }
  }

  _delay_ms(50);
  
}
