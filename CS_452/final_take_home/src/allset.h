#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

#define LEDPORT1 PORTB
#define LEDPORT2 PORTC

/*
 * Sets all the LED port bits to 1
*/
void allset(void)
{
	LEDPORT1 = ~0xFF;
	LEDPORT2 = ~0xFF;
}
