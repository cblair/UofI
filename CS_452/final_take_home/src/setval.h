#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

#define LEDPORT1 PORTB
#define LEDPORT2 PORTC

/*
 * Sets the LED port to the param val
*/
void setval(uint32_t val)
{
	uint8_t val1 = val & 0x0F;
	uint8_t val2 = val >> 8;

	LEDPORT1 = ~val1;
	LEDPORT2 = ~val2;
}
