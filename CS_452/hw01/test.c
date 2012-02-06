#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

int i;

void delay (unsigned int dly)
{
	for(i = dly; i != 0; i--) ;
}

int main(void)
{
	uint8_t cnt;

	/*Are these the same?*/
	/*DRB = 0xff;*/
	DDRB = 0xff;

	while(1)
	{
		for ( cnt=0; cnt <= 0xff; cnt++ )
		{
			PORTB = ~cnt;
			/*delay(65000U);*/
			delay(125000U);
		}
	}

	return(0);
}
