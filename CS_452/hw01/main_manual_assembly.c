#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

void delay ()
{
	unsigned int i;
	for(i = 0; i <= 65000U; i++) ;
}

void enable_port_b ()
{
	/*make Port B an output*/
	DDRB = 0xff;
}

void enable_port_d()
{
	/*make Port D an input*/
	DDRD = 0x00;
}

void turn_off_leds ()
{
	/*shut off all LEDs*/
	PORTB = ~0x00;
}

void turn_on_leds ()
{
	/*shut off all LEDs*/
	PORTB = ~0xff;
}


int main(void)
{
	//uint64_t cnt = 0u; 	/*counts how long switch is pressed*/
	int cnt = 0;
	//uint8_t which_switch; 	/*records which switch is pressed*/
	int which_switch;
	//uint64_t x = 0; 	/*used to iterate up to cnt*/
	int x = 0;
	
	enable_port_b();
	enable_port_d();
	
	turn_off_leds();

	while(1)
	{
		which_switch = PIND;

		if( PIND != 0xff ) 
		{
			while(PIND == which_switch)
			{
				cnt++;
			}
			

			turn_on_leds();

			x = 0; 
			while(x <= (cnt / 7U))
			{
				x++;
			}

			turn_off_leds();

			cnt = 0U;
		}
	}
	return(0);
}
