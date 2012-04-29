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

#define CTRL PORTD
#define DATA PORTA

#define e 0
#define rs 1
#define rw 2


//sends data to lcd
int sendData(uint8_t value){
	CTRL=3;//set rs=1 and e=1 to send data to lcd
	DATA = value;//put data on data port
	_delay_ms(20);//2 ms delay
	CTRL = 2; // set e to 0 to get lcd to recieve data
	return 1;
}
//send control signal to the lcd

void sendControl(uint8_t value){
	CTRL = 1;//set e bit and only e bit on ctrl port
	DATA = value;//put data on port
	_delay_ms(2);//2 ms delay
	CTRL =0 ;//flip e bit off
	_delay_ms(8); //8ms delay
}
void LCD_INIT(){
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
//just move to second line
void newline(){
	/*just move cursor to address 40
	which is line 2
	*/
	sendControl(0b10101000);
}
//put a single char to the dat port
int putchar(int ch){
	
	sendData(ch);
	return 1;
}
//put a series of chars on the lcd
void putstr(char *c,int len){
	int i;
	for(i=0;i<len;i++){
		putchar(c[i]);
	}
	
}
int main(){
	DDRA = 0xff;//set data  dirrection to out
	DDRD = 0xff;//set data direction to out
	//DDRB = 0xFF;
	LCD_INIT();//initialize
	_delay_ms(100);//wait
	//print stuff
	putstr("Hola",4);
	newline();
	putstr("Mundo!",6);
	
	sendData(0b00110011);
	CTRL=3;
	//normaly we would  loop forever ...but we dont really need to here
	return 0;
}
