	.file "main_manual_assmbly.c"

;I don't know what this stuff is
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	main
	.type	main, @function

main:
	;Define port b as output

	;load binary number to register
	; ldi = LoaD Imediate
	; loads 8 bit values to register.
	; only valid for registers 16-31
	;;ldi r16, 0b11111111	
	;set to port b
	; out rights register val to port
	;;out DDRB,r16
	ldi r30,lo8(55)
	ldi r31,hi8(55)
	ldi r24,lo8(-1)
	st Z,r24

	;Define port d as an input
	/* frame size = 0 */
	ldi r30,lo8(49)
	ldi r31,hi8(49)
	st Z,__zero_reg__

	
;Write 0s and 1s to the port
loop:
	;Set the LEDs on
	;;ldi r16,0x00
	;;out PORTB,r16
	;ldi r30,lo8(56)
	;ldi r31,hi8(56)
	;st Z,__zero_reg__

	;if PIND != 0xff
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	cpi r24,lo8(-1)
	; if PIND == 0xff, jump out of if
	breq .L1 
	; else, jump to delay
	rjmp delay
.L1:
	;Set the LEDs off
	;;ldi r16,0xFF
	;;out PORTB,r16
	ldi r30,lo8(56)
	ldi r31,hi8(56)
	ldi r24,lo8(-1)
	st Z,r24

	rjmp loop

delay:
	;uint64_t i = 0
	;std Y+1,__zero_reg__
	;std Y+2,__zero_reg__
	;std Y+3,__zero_reg__
	;std Y+4,__zero_reg__
	;std Y+5,__zero_reg__
	;std Y+6,__zero_reg__
	;std Y+7,__zero_reg__
	;std Y+8,__zero_reg__
	;ldi r24,lo8(0)
	;ldi r25,hi8(0)

	;uint8_t i = 0
	ldi r24,lo8(0)
	std Y+1,r24

delay_while:
	;if PIND != 0xff
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	cpi r24,lo8(-1)
	; if PIND == 0xff, jump out of if
	breq .L2
	; else, increment i
	;i = i + 1
	ldd r24,Y+1
	subi r24,lo8(-(1))
	std Y+1,r24
	; loop back to start of while
	rjmp delay_while	

	;Increment i down and on LEDs light
	; while i != 0 ...
.L2:	
	; turn on LEDs
	ldi r30,lo8(56)
	ldi r31,hi8(56)
	st Z,__zero_reg__

	; i != 0 ?
	ldd r24,Y+1
	tst r24
	; if it is not 0, jump 
	brne .L3
	; otherwise, jump passed end of while
	rjmp .L4
	
.L3:	; i = i + 1
	ldd r24,Y+1
	subi r24,lo8(-(1))
	std Y+1,r24
	rjmp .L2

.L4:
	ret
