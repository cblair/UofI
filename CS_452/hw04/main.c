/*
 * Class: CS 452
 * Assignment: HW 1
 * Author: Colby Blair
 * Date Due: February 6th, 2012
 *
 * Comments: 	Works with AVR STK500 and an Atmega8515L (Atmege32 untested 
 * 		yet). When a switch is pressed and released, the corresponding
 *		LED is lit for roughly the same time.
 * 		
*/

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>


/*Keypad stuff*/
/*how long the combonation is (digits)*/
#define COMBO_LENGTH 4

/*char SECRET_CODE[COMBO_LENGTH] = {'1','2','3','A'};*/
char SECRET_CODE[COMBO_LENGTH] = {'4','5','6','B'};
char ENTRY_CODE[COMBO_LENGTH] = {'0','0','0','0'};

uint8_t ENTRY_COUNT = 0u; /*Tells us how many times keypad buttons have been pressed*/

/* (ROW/COL)NAME OUTPUT-PIN-NO.*/
#define ROW2 0b00000001 /*1*/
#define ROW3 0b00000010 /*2*/
#define COL1 0b00000100 /*3*/
#define ROW4 0b00001000 /*4*/
#define COL2 0b00010000 /*5*/
#define COL3 0b00100000 /*6*/
#define COL4 0b01000000 /*7*/
#define ROW1 0b10000000 /*8*/


void delay (uint64_t dly)
{
	unsigned int i;
	for(i = dly; i != 0; i--) ;
}


/*checks to see if the secret code matches the inputed code*/
void secret_code_check(void)
{
	int i;
	for(i = 0; i < COMBO_LENGTH; i++)
	{
		if(ENTRY_CODE[i] != SECRET_CODE[i])
		{
			/*assert high led 6 for fail*/
			PORTB = ~0b01000000;
			return;
		}
	}
	
	/*all must have matched, assert led 7 for success*/
	PORTB = ~0b10000000;
}


/*Initializes rows as inputs, columns as outputs*/
uint8_t init_keypad(void)
{
	/*To set a pin as an input, set it as 0. For output, set is as 1.*/
	uint8_t retval = (ROW1 | ROW2 | ROW3 | ROW4) /*inputs*/ \
			| \
			~(COL1 | COL2 | COL3 | COL4); /*outputs*/

	return(retval);
}


/*Returns a value that lights up an led for each value entry_count is greater than
 * 0. i.e.:
 *	1 = 00000001
 *	2 = 00000011
 *	3 = 00000111
 *	4 = 00001111
 * Mostly did it this way because I wasn't getting bit shifts right.
 *
 * Notice: only does 0-3 for now.
 */
uint8_t get_led_entry_code(uint8_t entry_count)
{
	uint8_t retval = 0;
	if(entry_count == 1)
	{
		retval = 0b00000001;
	}
	else if(entry_count == 2)
	{
		retval = 0b00000011;
	}
	else if(entry_count == 3)
	{
		retval = 0b00000111;
	}

	return(retval);
}


void keypad_key_check(uint8_t PINX, uint8_t COLX, char ival)
{
	/*if PORTD with the COL value masked off equals low*/
	if( ((~PINX) & COLX) != 0 ) 
	{
		/*debounce - delay and compare again*/
		delay(95000000u); /*two func calls b/c of precision lim on arg*/
		delay(95000000u);

		if( ((~PINX) & COLX) != 0 ) 
		{
			ENTRY_CODE[ENTRY_COUNT] = ival; /*save the inputed value*/
			ENTRY_COUNT++; /*increment the input count*/
			/*one led for each input*/
			PORTB = ~(get_led_entry_code(ENTRY_COUNT)); 
		}
	}
}


void keypad_to_led(void)
{

	/*Set COL to low, and see if ROW reflects the low*/

	/*Row 1*/
	/*1 key*/
	PORTD = (uint8_t)(~ROW1 | ROW2 | ROW3 | ROW4 ) | ~(~COL1 | COL2 | COL3 | COL4);
	keypad_key_check(PIND, COL1, '1');

	/*2 key*/
	PORTD = (uint8_t)(~ROW1 | ROW2 | ROW3 | ROW4 ) | ~(COL1 | ~COL2 | COL3 | COL4);
	keypad_key_check(PIND, COL2, '2');

	/*3 key*/
	PORTD = (uint8_t)(~ROW1 | ROW2 | ROW3 | ROW4 ) | ~(COL1 | COL2 | ~COL3 | COL4);
	keypad_key_check(PIND, COL3, '3');

	/*A key*/
	PORTD = (uint8_t)(~ROW1 | ROW2 | ROW3 | ROW4 ) | ~(COL1 | COL2 | COL3 | ~COL4);
	keypad_key_check(PIND, COL4, 'A');

	/*Row 2*/
	/*4 key*/
	PORTD = (uint8_t)(ROW1 | ~ROW2 | ROW3 | ROW4 ) | ~(~COL1 | COL2 | COL3 | COL4);
	keypad_key_check(PIND, COL1, '4');

	/*5 key. */
	PORTD = (uint8_t)(ROW1 | ~ROW2 | ROW3 | ROW4 ) | ~(COL1 | ~COL2 | COL3 | COL4);
	keypad_key_check(PIND, COL2, '5');

	/*6 key*/
	PORTD = (uint8_t)(ROW1 | ~ROW2 | ROW3 | ROW4 ) | ~(COL1 | COL2 | ~COL3 | COL4);
	keypad_key_check(PIND, COL3, '6');

	/*B key*/
	PORTD = (uint8_t)(ROW1 | ~ROW2 | ROW3 | ROW4 ) | ~(COL1 | COL2 | COL3 | ~COL4);
	keypad_key_check(PIND, COL4, 'B');

	/*Row 3*/
	/*7 key*/
	PORTD = (uint8_t)(ROW1 | ROW2 | ~ROW3 | ROW4 ) | ~(~COL1 | COL2 | COL3 | COL4);
	keypad_key_check(PIND, COL1, '7');

	/*8 key*/
	PORTD = (uint8_t)(ROW1 | ROW2 | ~ROW3 | ROW4 ) | ~(COL1 | ~COL2 | COL3 | COL4);
	keypad_key_check(PIND, COL2, '8');

	/*9 key*/
	PORTD = (uint8_t)(ROW1 | ROW2 | ~ROW3 | ROW4 ) | ~(COL1 | COL2 | ~COL3 | COL4);
	keypad_key_check(PIND, COL3, '9');

	/*C key*/
	PORTD = (uint8_t)(ROW1 | ROW2 | ~ROW3 | ROW4 ) | ~(COL1 | COL2 | COL3 | ~COL4);
	keypad_key_check(PIND, COL4, 'C');

	/*Row 4*/
	/*'*' key*/
	PORTD = (uint8_t)(ROW1 | ROW2 | ROW3 | ~ROW4 ) | ~(~COL1 | COL2 | COL3 | COL4);
	keypad_key_check(PIND, COL1, '*');

	/*0 key*/
	PORTD = (uint8_t)(ROW1 | ROW2 | ROW3 | ~ROW4 ) | ~(COL1 | ~COL2 | COL3 | COL4);
	keypad_key_check(PIND, COL2, '0');

	/*'#' key*/
	PORTD = (uint8_t)(ROW1 | ROW2 | ROW3 | ~ROW4 ) | ~(COL1 | COL2 | ~COL3 | COL4);
	keypad_key_check(PIND, COL3, '#');

	/*D key*/
	PORTD = (uint8_t)(ROW1 | ROW2 | ROW3 | ~ROW4 ) | ~(COL1 | COL2 | COL3 | ~COL4);
	keypad_key_check(PIND, COL4, 'D');

	/*
	 * if ENTRY_COUNT is equal to COMBO_LENGTH, then we should check
	 * if the code is clear, or get ready for another code.
	 */
	if(ENTRY_COUNT >= COMBO_LENGTH)
	{
		secret_code_check();
		ENTRY_COUNT = 0;
	}	

}


int main(void)
{
	uint8_t PORT_SETTINGS = init_keypad();

	/*make Port B an output*/
	DDRB = 0xff;


	/*To set a pin as an input, set it as 0. For output, set is as 1.*/
	//DDRD = 0b00000100; //TODO: test, row 1 input, col 1 output
	/*	 ^    ^		*/
	/*	 |    |		*/
	/*      row1 col1	*/
	/*
	DDRD = 0b00001111;
	PORTD = 0b11110000;
	*/
	DDRD = PORT_SETTINGS;
	PORTD = ~PORT_SETTINGS;

	/*shut off all LEDs*/
	PORTB = ~0x00;

	while(1)
	{
			/*
		PORTB = ~( 0x01 );
		delay(950000u);
		PORTB = ~( 0x00 );
		delay(950000u);
		*/
		keypad_to_led();
	}

	return(0);
}
