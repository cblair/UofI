#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

/*factor in which to scale down the button time count (cnt)*/
#define TIME_SCALE 7U 


unsigned int i;

void delay (unsigned int dly)
{
	for(i = dly; i != 0; i--) ;
}



int main(void)
{
	uint64_t cnt = 0u; 	/*counts how long switch is pressed*/
	uint8_t which_switch; 	/*records which switch is pressed*/
	uint64_t x = 0; 	/*used to iterate up to cnt*/

	/*make Port B an output*/
	DDRB = 0xff;

	/*make Port D an input*/
	DDRD = 0x00;

	/*shut off all LEDs*/
	PORTB = ~0x00;

	while(1)
	{
		/*record which switch is being pressed*/
		which_switch = PIND;		

		/*
		 * if no switches pressed, set LEDs to off / no press
		 * state
		 */
		if( PIND == 0xff ) 
		{
			PORTB = ~0x01;
			delay(65000U);
			PORTB = 0xff;
			delay(65000U);
		}
		/*else, count how long switch is pressed*/
		else
		{
			/*while Port D stays the same*/
			while(PIND == which_switch)
			{
				cnt++;
			}

			/*
			 * set Port B to the LED with the same value 
			 * (lighting up the LED that is next to the switch)
			 */
			PORTB = which_switch;

			/* 
			 * no op for the same count as switch was pressed,
			 * adjusting for the differences in clock cycles
			 */	
			for(x = 0; x <= (cnt / TIME_SCALE); x++) ;

			/*set all LEDs to off*/
			PORTB = ~0x00;

			/*reset the count*/
			cnt = 0U;
		}
	
	}

	return(0);
}
