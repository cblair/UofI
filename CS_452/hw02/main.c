/*
 * Class: CS 452
 * Assignment: HW 1
 * Author: Colby Blair
 * Date Due: February 6th, 2012
 *
 * Comments: 	Works with AVR STK500 and an Atmega8515L (Atmege32 untested 
 * 		yet). When a switch is pressed and released, the corresponding
 *		LED is lit for roughly the same time.
 * 		
*/

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

/*factor in which to scale down the button time count (cnt)*/
#define TIME_SCALE 7U 


uint8_t cnt = 0u; 	/*counts how long switch is pressed*/
uint8_t led_cnt = 0x00; 
uint8_t which_switch = 0x00; 	/*records which switch is pressed*/

void delay (unsigned int dly)
{
	unsigned int i;
	for(i = dly; i != 0; i--) ;
}


void init_Ex1(void)
{
	//DDRB  = 0xFF;                 // Set Port B as output
	//TCCR0 = (1<<CS02)|(1<<CS00);  // Timer clock = Sysclk/1024
	TCCR0 = (1<<CS02)|(1<<CS10);
	TIFR  = 1<<TOV0;              // Clear TOV0, any pending interrupts
	TIMSK = 1<<TOIE0;             // Enable Timer0 Overflow interrupt
} // END init_Ex1 


// void interrupt [TIMER0_OVF_vect] ISR_TOV0 (void)
ISR(TIMER0_OVF_vect) 
{
	cnt++;

	if(cnt == 30)
	{
		//PORTB = ~(led_cnt | which_switch);
		PORTB = ~(led_cnt & 0xF0);

		led_cnt -= 0xF0;	//increment only for pins 4-7
		//led_cnt += 16u;		//increment only for pins 4-7
		
		/*make sure led_cnt never has bits set in the lower 4 pins*/
		led_cnt &= 0xF0;

		/*reset count*/
		cnt = 0;
	}
} // end ISR


void led_switch_echo(void)
{
	uint64_t bcnt = 0u; 	/*counts how long switch is pressed*/
	uint64_t x = 0u; 	/*used to iterate up to bcnt*/
	uint8_t port_state;

	/*record which switch is being pressed*/
	port_state = PIND;

	/*
	 * if no switches pressed, set LEDs to off / no press
	 * state
	 */
	if( port_state == 0xff ) 
	{
		PORTB = ~(led_cnt | 0x01);
		delay(65000U);
		PORTB = ~(led_cnt | 0x00); //really just led_cnt
		delay(65000U);
	}
	/*else, count how long switch is pressed*/
	else
	{
		/*while Port D stays the same*/
		while(PIND == port_state)
		{
			bcnt++;
		}

		/*
		 * set Port B to the LED with the same value 
		 * (lighting up the LED that is next to the switch)
		 */
		//which_switch = ~port_state;
		which_switch = 0x01;

		/*make sure which_switch never has bits set in the upper 4 pins*/
		which_switch &= 0x0F;

		/* 
		 * no op for the same count as switch was pressed,
		 * adjusting for the differences in clock cycles
		 */	
		for(x = 0; x <= (bcnt / TIME_SCALE); x++) 
		{
			PORTB = ~(which_switch | led_cnt);
		}

		/*reset which switch to none*/
		which_switch = 0x00;

		/*reset the count*/
		bcnt = 0U;
	}
}



int main(void)
{
	/*make Port B an output*/
	DDRB = 0xff;

	/*make Port D an input*/
	DDRD = 0x00;

	/*shut off all LEDs*/
	PORTB = ~0x00;

	/*enable timer*/
	init_Ex1();

	/* Enable Interrupts */
	sei();

	while(1)
	{
		led_switch_echo();
	}

	return(0);
}
