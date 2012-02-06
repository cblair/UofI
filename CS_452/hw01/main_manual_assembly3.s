;
; Class: CS 452
; Assignment: HW 1
; Author: Colby Blair
; Date Due: February 6th, 2012
;
; Comments: 	Works with AVR STK500 and an Atmega8515L (Atmege32 untested 
;  		yet). When a switch is pressed and released, the corresponding
;		LED is lit for roughly the same time.
; 		
;

	.file "main_manual_assmbly.c"

;I don't know what this stuff is
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1

	.text

.global	delay
	.type	delay, @function
delay:
	push r29
	push r28
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
	std Y+2,__zero_reg__
	std Y+1,__zero_reg__
	rjmp .L2
.L3:
	ldd r24,Y+1
	ldd r25,Y+2
	adiw r24,1
	std Y+2,r25
	std Y+1,r24
.L2:
	ldd r24,Y+1
	ldd r25,Y+2
	ldi r18,hi8(-535)
	cpi r24,lo8(-535)
	cpc r25,r18
	brlo .L3
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	delay, .-delay

.global	main
	.type	main, @function

main:
	rcall enable_port_b
	rcall enable_port_d

loop:
	;in r25, $16
	clr r16
	ser r17
	;out 1, r17
	rjmp loop

enable_port_b:
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
	
	ret


enable_port_d:
	;Define port d as an input
	/* frame size = 0 */
	ldi r30,lo8(49)
	ldi r31,hi8(49)
	st Z,__zero_reg__

	ret
