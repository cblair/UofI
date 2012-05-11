#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

#define LEDPORT1 PORTB
#define LEDPORT2 PORTC

/*
 * Sets all the LED port bits to 0
*/
void allclr(void)
{
	LEDPORT1 = ~0x00;
	LEDPORT2 = ~0x00;
}
