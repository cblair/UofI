#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

#define LEDPORT1 PORTC
#define LEDPORT2 PORTB

/*
 * Sets the LED port to the abs of param val
*/
void setabs(uint32_t val)
{
	if( (0xE0 & val) == 0xEF)
	{
		val = ~val;
	}

	uint8_t val1 = val & 0x0F;
	uint8_t val2 = val >> 8;

	LEDPORT1 = ~val1;
	LEDPORT2 = ~val2;
}
