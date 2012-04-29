void init_Ex3(void) 
{
	ASSR= 1<<AS2; 	// Enable asynchronous 
			// mode

	// Clear timer on compare match / Timer Clock = 
	// system clock / 1024
	TCCR2 = (1<<CTC2)|(1<<CS22)|(1<<CS21)|(1<<CS20);
	
	// Set Port B as output

	TIFR= 1<<OCF2;	// Clear OCF2/ Clear 
			// pending interrupts

	TIMSK= ( 1<<TOV0)|(1<<OCIE2);	// Clear TOV0, Enable Timer2 Output
					// Compare Match Interrupt

	OCR2= 32;	// Set Output Compare 
			// Value to 32

	//DDRB= 0xFF;

	while (ASSR&(1<<OCR2UB))
		; // Wait for registers to update
}
