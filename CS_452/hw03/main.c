/*
 * Class: CS 452
 * Assignment: HW 3
 * Author: Colby Blair
 * Date Due: February xxth, 2012
 *
 * Comments: 	Works with AVR STK500 and an Atmega8515L (Atmege32 untested 
 * 		yet). 
 * 		
*/

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

/*factor in which to scale down the button time count (cnt)*/
#define TIME_SCALE 7U 

/*Music stuff*/
#define SONG_SIZE 9
int SONG_INDEX = 1;
uint8_t SONG[SONG_SIZE];
//0 for false, 1 for true
uint8_t ENABLE_MUSIC = 0; 
uint8_t NOTE_COUNT = 0; 


/*LED Echo stuff*/
uint8_t cnt = 0u; 	/*counts how long switch is pressed*/
uint8_t led_cnt = 0x00; 
uint8_t which_switch = 0x00; 	/*records which switch is pressed*/

void delay (unsigned int dly)
{
	unsigned int i;
	for(i = dly; i != 0; i--) ;
}


/*Interrupt stuff*/
void init_Ex1(void)
{
	//DDRB  = 0xFF;                 // Set Port B as output
	//TCCR0 = (1<<CS02)|(1<<CS00);  // Timer clock = Sysclk/1024
	TCCR0 = (1<<CS02)|(1<<CS10);
	TIFR  = 1<<TOV0;              // Clear TOV0, any pending interrupts
	TIMSK = 1<<TOIE0;             // Enable Timer0 Overflow interrupt
} // END init_Ex1


void init_Ex2(void) 
{
	/*Output Compare Mode*/
	TCCR1B = (1<<CS02)|(1<<CS10);
	TCCR1B = ((1 << CS10) | (1 << CS11));
	//TCCR1B = (1<<CS02);

	/*Overflow Mode
	TCCR1B = (1<<CS02)|(1<<CS10);
	TIFR  = 1<<TOV0;              // Clear TOV0, any pending interrupts
	TIMSK = (1<<TOIE0)|(1<<TOIE1);// Enable Timer0 and Timer1 Overflow interrupt
	*/
} 


// void interrupt [TIMER0_OVF_vect] ISR_TOV0 (void)
ISR(TIMER0_OVF_vect) 
{
	cnt++;

	if(cnt == 15)
	{
		ENABLE_MUSIC = 1;
	}
	else if (cnt == 30)
	{
		/*reset count*/
		cnt = 0;
		
		/*LED Echo stuff*/
		//PORTB = ~(led_cnt | which_switch);
		PORTB = ~(led_cnt & 0xF0);

		led_cnt -= 0xF0;	//increment only for pins 4-7
		//led_cnt += 16u;		//increment only for pins 4-7
		
		/*make sure led_cnt never has bits set in the lower 4 pins*/
		led_cnt &= 0xF0;

		/*Music Stuff*/
		ENABLE_MUSIC = 0;		
		if(SONG_INDEX == SONG_SIZE - 1)
		{
			SONG_INDEX = 0; //start over song
		}
		else
		{
			SONG_INDEX++;
		}
	}

	if(ENABLE_MUSIC == 1)
	{
		//PORTB = ~0x02;
	}
	else
	{
		//PORTB = ~0x01;
	}
} // end ISR


ISR(TIMER1_OVF_vect)
{
	//PORTB = ~PORTB;
}


void play_note(uint8_t period, uint8_t count)
{
	if(period == 0)
	{
		PORTB = ~(~PORTB & 0xFE); // Toggle the LED 
	}
	else
	{
		if (TCNT1 >= period) 
		{
			PORTA = ~PORTA; // Toggle the Speaker
			PORTB = ~(~PORTB | 0x02); // Toggle the LED 

			TCNT1 = 0; // Reset timer value 
		}
	}
}


int main(void)
{
	/*make Port B and A an output*/
	DDRA = 0xff;
	DDRB = 0xff;

	/*make Port D an input*/
	DDRD = 0x00;

	/*shut off all LEDs*/
	PORTB = ~0x00;

	/*enable timer*/
	init_Ex1();
	init_Ex2();

	/* Enable Interrupts */
	sei();

	SONG[0] = 90;
	SONG[1] = 82;
	SONG[2] = 76;
	SONG[3] = 69;
	SONG[4] = 62;
	SONG[5] = 55;
	SONG[6] = 49;
	SONG[7] = 43;
	SONG[8] = 0;
	//SONG[1] = 10;
	while(1)
	{
		play_note(SONG[SONG_INDEX], 1);
	}

	return(0);
}
