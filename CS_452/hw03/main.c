/*
 * Class: CS 452
 * Assignment: HW 3
 * Author: Colby Blair
 * Date Due: February xxth, 2012
 *
 * Comments: 	Works with AVR STK500 and an Atmega8515L (Atmege32 untested 
 * 		yet). 
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

int main(void)
{
	uint64_t idelay = 0x00;

	/*make Port B and A an output*/
	DDRA = 0xff;
	DDRB = 0xff;

	while(1)
	{
		PORTA = ~0x01;
		PORTB = ~0x01;
		delay(idelay);
		PORTA = ~0x00;
		PORTB = ~0x00;
		delay(idelay);
		idelay += 0x40;
		if(idelay > 9000u)
		{
			idelay = 0x00;
		}
	}

	return(0);
}
