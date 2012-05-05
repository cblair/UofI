/*
 * Class: CS 452
 * Assignment: HW 5
 * Author: Colby Blair
 * Date Due: March 9th, 2012
 *
 * Comments: 	Works with AVR STK500 and an Atmega8515L (Atmege32 untested 
 * 		yet). 
 * 		
*/

#include <inttypes.h>
#include <avr/pgmspace.h>
#include <avr/io.h>

#define F_CPU 8000000UL
#include <util/delay.h>

#define CTRL 		PORTD
#define CTRL_SETUP	DDRD
#define DATA 		PORTA
#define DATA_SETUP	DDRA

/*
//TODO: get these back in after troubleshooting
#define LCD_E 0
#define LCD_RS 1
#define LCD_RW 2
*/


/*Sends data to lcd*/
int sendData(uint8_t value)
{
	CTRL=3;		/*set rs=1 and e=1 to send data to lcd*/
	DATA = value;	/*put data on data port*/
	_delay_ms(20);	/*20 ms delay*/
	CTRL = 2; 	/*set e to 0 to get lcd to recieve data*/
	return(1);
}


/*Send control signal to the lcd*/
void sendControl(uint8_t value)
{
	CTRL = 1;	/*set e bit and only e bit on ctrl port*/
	DATA = value;	/*put data on port*/
	_delay_ms(2);	/*2 ms delay*/
	CTRL =0 ;	/*flip e bit off*/
	_delay_ms(8);	/*8ms delay*/
}


/*Initializes the LCD*/
void lcdInit(){
	CTRL = 0x0;//0 out CTRL 
	DATA = 0x0;//0 out data
	_delay_ms(30);//at least 20 ms
	//initialize screen 2 lines, 5x10 units
	sendControl(0b00111000);
	_delay_ms(2);
        //clear display
	sendControl(0b00000001);
	_delay_ms(2);
	sendControl(0b00001101);
	_delay_ms(2);
	//set cursor to move to the right
	sendControl(0b00000110);
	_delay_ms(2);
	//move to home
	sendControl(0b10000000);
}


void newline(){
	/*just move cursor to address 40
	which is line 2
	*/
	sendControl(0b10101000);
}


int putchar(int ch){
	
	sendData(ch);
	return 1;
}


void putstr(char *c,int len){
	int i;
	for(i=0;i<len;i++){
		putchar(c[i]);
	}
	
}


int main(){
	DATA_SETUP = 0xff;//set data  dirrection to out
	CTRL_SETUP = 0xff;//set data direction to out

	lcdInit();//initialize
	_delay_ms(100);//wait


	putstr("Hello",5);
	newline();
	putstr("World",5);
	
	/*Don't need to loop, because the LCD has memory!*/
	return 0;
}
