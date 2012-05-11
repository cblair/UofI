#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

#define LEDPORT1 PORTB
#define LEDPORT2 PORTC

/*
 * Sets the LED port bit specified by bitnum to 0.
 * Retains all other values.
*/
void bitclr(uint16_t bitnum)
{
	uint8_t bitnum1 = bitnum & 0x0F; 
	uint8_t bitnum2 = bitnum >> 8;
	/*
	 * Logic AND inverse of int 1 shifted bitnum times with the 
	 * current LEDPORT values
	 */
	LEDPORT1 = ~( ~LEDPORT1 & ~(1 << (bitnum1)) );
	
	LEDPORT2 = ~( ~LEDPORT2 & ~(1 << (bitnum2)) );
}
