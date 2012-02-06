	.file	"main_manual_assembly.c"
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
.global	enable_port_b
	.type	enable_port_b, @function
enable_port_b:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
	ldi r30,lo8(55)
	ldi r31,hi8(55)
	ldi r24,lo8(-1)
	st Z,r24
/* epilogue start */
	pop r28
	pop r29
	ret
	.size	enable_port_b, .-enable_port_b
.global	enable_port_d
	.type	enable_port_d, @function
enable_port_d:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
	ldi r30,lo8(49)
	ldi r31,hi8(49)
	st Z,__zero_reg__
/* epilogue start */
	pop r28
	pop r29
	ret
	.size	enable_port_d, .-enable_port_d
.global	turn_off_leds
	.type	turn_off_leds, @function
turn_off_leds:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
	ldi r30,lo8(56)
	ldi r31,hi8(56)
	ldi r24,lo8(-1)
	st Z,r24
/* epilogue start */
	pop r28
	pop r29
	ret
	.size	turn_off_leds, .-turn_off_leds
.global	turn_on_leds
	.type	turn_on_leds, @function
turn_on_leds:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
	ldi r30,lo8(56)
	ldi r31,hi8(56)
	st Z,__zero_reg__
/* epilogue start */
	pop r28
	pop r29
	ret
	.size	turn_on_leds, .-turn_on_leds
.global	main
	.type	main, @function
main:
	push r29
	push r28
	rcall .
	rcall .
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 6 */
	std Y+6,__zero_reg__
	std Y+5,__zero_reg__
	std Y+2,__zero_reg__
	std Y+1,__zero_reg__
	rcall enable_port_b
	rcall enable_port_d
	rcall turn_off_leds
.L19:
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	mov r24,r24
	ldi r25,lo8(0)
	std Y+4,r25
	std Y+3,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	cpi r24,lo8(-1)
	breq .L19
	rjmp .L15
.L16:
	ldd r24,Y+5
	ldd r25,Y+6
	adiw r24,1
	std Y+6,r25
	std Y+5,r24
.L15:
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	mov r18,r24
	ldi r19,lo8(0)
	ldd r24,Y+3
	ldd r25,Y+4
	cp r18,r24
	cpc r19,r25
	breq .L16
	rcall turn_on_leds
	std Y+2,__zero_reg__
	std Y+1,__zero_reg__
	rjmp .L17
.L18:
	ldd r24,Y+1
	ldd r25,Y+2
	adiw r24,1
	std Y+2,r25
	std Y+1,r24
.L17:
	ldd r30,Y+1
	ldd r31,Y+2
	ldd r24,Y+5
	ldd r25,Y+6
	ldi r18,lo8(7)
	ldi r19,hi8(7)
	movw r22,r18
	rcall __udivmodhi4
	movw r24,r22
	cp r24,r30
	cpc r25,r31
	brsh .L18
	rcall turn_off_leds
	std Y+6,__zero_reg__
	std Y+5,__zero_reg__
	rjmp .L19
	.size	main, .-main
