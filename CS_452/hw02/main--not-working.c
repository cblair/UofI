/*
 * Class: CS 452
 * Assignment: HW 2
 * Author: Colby Blair
 * Date Due: February 13th, 2012
 *
 * Comments: 	Works with AVR STK500 and an Atmega8515L (Atmege32 untested 
 * 		yet). When a switch is pressed and released, the corresponding
 *		LED is lit for roughly the same time.
 * 		
*/

#include <inttypes.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*factor in which to scale down the button time count (cnt)*/
#define TIME_SCALE 	7U 

uint8_t cnt;                    // global variable
uint8_t led_cnt = 0x00;                    // global variable
uint8_t which_switch = 0x00; 	/*records which switch is pressed*/


void port_init(void)
{
	/*make Port B an output*/
	DDRB = 0xff;

	/*make Port D an input*/
	DDRD = 0x00;
}


void init_Ex1(void)
{
	//DDRB  = 0xFF;                 // Set Port B as output
	//TCCR0 = (1<<CS02)|(1<<CS00);  // Timer clock = Sysclk/1024
	TCCR0 = (1<<CS02)|(1<<CS10);
	TIFR  = 1<<TOV0;              // Clear TOV0, any pending interrupts
	TIMSK = 1<<TOIE0;             // Enable Timer0 Overflow interrupt
} // END init_Ex1 


// void interrupt [TIMER0_OVF_vect] ISR_TOV0 (void)
ISR(TIMER0_OVF_vect) 
{
	cnt++;

	if(cnt == 30)
	{
		//PORTB = ~(led_cnt);// | which_switch);

		led_cnt -= 0xF0;	//increment only for pins 4-7
		
		/*make sure lower bits are not set*/
		led_cnt &= 0xF0;

		/*reset count*/
		cnt = 0;
	}
} // end ISR


void delay (unsigned int dly)
{
	unsigned int i;
	for(i = dly; i != 0; i--) ;
}


int button_led_echo_init(void)
{
	/*shut off all LEDs*/
	PORTB = ~0x00;
}


/*
 * This function will blink led0, and when a button is pressed, the time
 * held down is timed, and on button release, the respective led is lit
 * for the same amount of time.
 */
void button_led_echo(void)
{
	uint64_t bcnt = 0u; 	/*counts how long switch is pressed*/
	//uint8_t which_switch; 	/*records which switch is pressed*/
	uint64_t x = 0; 	/*used to iterate up to bcnt*/
	uint64_t port_state;

	/*record which switch is being pressed*/
	//which_switch = PIND;
	port_state = ~PIND;

	/*
	 * if no switches pressed, set LEDs to off / no press
	 * state
	 */	
	if( PIND == 0xff ) 
	{
		/*
		PORTB = ~0x01;
		delay(65000U);
		PORTB = 0xff;
		delay(65000U);
		*/
	}
	/*else, count how long switch is pressed*/
	else
	{
		/*while Port D stays the same*/
		//while(PIND == ~which_switch)
		while(PIND == ~port_state)
		{
			bcnt++;
		}

		/*set to port state, for only lower pins*/
		which_switch = port_state & 0x0F;

		/*
		 * set Port B to the LED with the same value 
		 * (lighting up the LED that is next to the switch)
		 */
		//PORTB = which_switch;

		/* 
		 * no op for the same count as switch was pressed,
		 * adjusting for the differences in clock cycles
		 */	
		for(x = 0; x <= (bcnt / TIME_SCALE); x++) 
		{
		}

		/*set all LEDs to off*/
		//PORTB &= ~0x0F;
		
		which_switch = 0x00;

		/*reset the count*/
		bcnt = 0U;
	}
}


int main(void)
{
	/*make Port B an output*/
	DDRB = 0xff;




		PORTB = ~0x01;
		delay(65000U);
		PORTB = 0xff;
		delay(65000U);

	return(0);

	port_init();


	button_led_echo_init();

	//init_Ex1();

	/* Enable Interrupts */
	//sei();

	while (1)
	{
		//button_led_echo();	
	}
   
	return(0);
}  // END main
