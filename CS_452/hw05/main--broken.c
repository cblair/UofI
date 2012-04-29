/* Program to drive LCD on STK200 board normally for Hitachi type 16 x 2 LCD, 
but will work on other LCD sizes.  For STK300, RD and WR need to be on PORTG - 
see schematic. This program uses Busy Flag on LCD (PORTA.7) instead of delays 
between each instruction.  The current LCD address is read at same time but 
not used in this program. 
Change to correct device in Project - Configuration Options */

//change to match target clock 
//Note: default AVR CLKSEL is 1MHz internal RC
#define F_CPU 8000000UL  

#include <avr/io.h>
#include <util/delay.h>

/*
// used pins on port C and D
#define LCD_E  7    // PORTC.7
#define LCD_RS 6    //PORTC.6
#define LCD_RD 7    // PORTD.7
#define LCD_WR 6    // PORTD.6
*/
// used pins on port C and E
#define LCD_E  7    // PORTC.7
#define LCD_RS 6    //PORTC.6
#define LCD_RD 0    // PORTE.0
#define LCD_WR 0    // PORTE.0


// Data and Busy flag are on PORTD
//LCD commands
#define LCD_CLR 0x01  // clear LCD
#define LCD_8BIT 0x38 // 8-bit mode
#define LCD_INC 0x06  //Increment, display freeze
#define LCD_MOV 0x10  //Cursor move, not shift
#define LCD_ALL  0x0F // LCD On, LCD display on, cursor on and blink on
#define LCD_ON           0x08      // turn lcd/cursor on             
#define LCD_ON_DISPLAY   0x04      // turn display on              
#define LCD_ON_CURSOR    0x02      // turn cursor on               
#define LCD_ON_BLINK     0x01      // cursor blink
#define LCD_LINE1  0x80        // cursor Pos on line 1 (or with column)
#define LCD_LINE2  0xC0        // cursor Pos on line 2 (or with column)

//LCD_getaddress reads the address counter and busy flag. For the address only,
//mask off bit7 of the return value.

unsigned char LCD_getaddr(void)
{	
	unsigned char address;	
    
        PORTC &= ~((1<<LCD_RS)|(1<<LCD_E));  
				//RS low for busy flag read and enable low	
        //RD pin low (for enable circuit)
	PORTE &= ~(1<<LCD_RD);
    
        // WR connected to read/write pin - high for read
	PORTE |= (1<<LCD_WR);
	//PortA is input
	DDRD = 0;
        PORTC |= (1<<LCD_E);  // enable high
        // short delay
	asm volatile ("nop");  
	asm volatile ("nop");

	//while E is high, get data from LCD
	address = PINA;
	PORTC &= ~(1<<LCD_E);   // E low (for strobe)
	// restore other settings
	PORTE |= (1<<LCD_RD);   // RD pin high (for enable circuit)
	PORTE &= ~(1<<LCD_WR);   // WR read/write goes low for write  
	DDRD = 0xFF; //back to output
	//return address and busy flag
	return address;
}

//LCD_wait reads the address counter (which contains the busy flag) and loops until
//the busy flag is cleared.

void LCD_wait(void){
	//get address and busy flag
	//and loop until busy flag (bit7) cleared

	while((LCD_getaddr() & 0x80) == 0x80)
	{
        }
}

/*LCD_putchar writes a character to the LCD at the current address,
usage: LCD_putchar('A'); or LCD_putchar(0x55); */

void LCD_putchar(unsigned char data){
	LCD_wait();  // is it busy?
	//PortA is output
	DDRD = 0xFF;

	//RS high for data and Enable high
	PORTC |= ((1<<LCD_RS)|(1<<LCD_E));
		//put data on bus
	PORTD = data;
	/*the number of nops required varies with your clock frequency, 
	Can be altered */
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	// Enable low again
	PORTC &= ~(1<<LCD_E);
	//release port
	DDRD = 0;
}
/*LCD_command works EXACTLY like LCD_putchar, but takes RS low for 
accessing the command reg */

void LCD_command(unsigned char command){
	
	LCD_wait();  // is it still busy?
	DDRD = 0xFF;  // data port as output
	PORTC &= ~(1<<LCD_RS);  //RS low for Command
	PORTC |= (1<<LCD_E);    //Enable pin high
	PORTD = command;         // put data on Port
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");    
	PORTC &= ~(1<<LCD_E);   //Enable Pin Low
	DDRD = 0;                // release bus
}

/*LCD_init initialises the LCD with the following paramters:
8 bit mode, 5*7 font, 2 lines (also for 4 lines)
auto-inc cursor after write and read
cursor and display on, cursor blinking.*/

void LCD_init(void){
	//setup the LCD control signals on PortC  (RS and Enable as output)
	DDRC |= ((1<<LCD_RS)|(1<<LCD_E));
	PORTC &= ~((1<<LCD_RS)|(1<<LCD_E));// set both low
	//setup the LCD control signals on PortD
  	DDRE |= ((1<<LCD_RD)|(1<<LCD_WR));// read and write pins (STK300 on PortG)
	PORTE &= ~(1<<LCD_WR); // write pin low
	// data Port A as output
	DDRD = 0xFF;
	/*if called right after power-up, we'll have to wait a bit (fine-tune 
	for faster execution) */
	_delay_ms(50);
   
        LCD_command(LCD_CLR);       // clear display
	LCD_command(LCD_8BIT);      // set 8 data bits  
	LCD_command(LCD_INC);       // cursor increments automatically 
	LCD_command(LCD_MOV);       // cursor  set to move
	//LCD_command(LCD_ALL);     // can call all instead of next line
	LCD_command(LCD_ON | LCD_ON_DISPLAY | LCD_ON_CURSOR | LCD_ON_BLINK);
        LCD_command(LCD_LINE1); // set cursor to  row 1, position 0
}

// function for showing strings on the LCD. It uses the low-level
//functions above. usage example: LCD_write("Hello World!");

void LCD_write(char* dstring){

	//is the character pointed at by dstring a zero? If not, write character to LCD
	while(*dstring)
	{		
	// write the character from dstring to the LCD, then post-inc the string pointer.
	       LCD_putchar(*dstring++);
	}
}

int main( void )
{
  LCD_init();
  LCD_write("Hello World");
  LCD_command(LCD_LINE2 | 5); // move cursor  to row 2, position 5 
  LCD_write("is here");
}
/*Note: As LCD has a RAM memory, it will display what was written 
until its power is removed hence no loop needed */
