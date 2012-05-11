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
#include <util/delay.h>

#include "setabs.h"

#define OVAL 0x00

int main(void)
{
	/*make Port B an output*/
	DDRB = 0xff;

	/*set LEDs to value that should be preserved*/
	PORTB = ~OVAL;

	while(1)
	{
		setabs(-5);
		_delay_ms(500);
		PORTB = ~OVAL;
		_delay_ms(500);
	}
}
