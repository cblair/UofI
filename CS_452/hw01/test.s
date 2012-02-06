	.file	"test.c"
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
	std Y+2,r25
	std Y+1,r24
	ldd r24,Y+1
	ldd r25,Y+2
	sts (i)+1,r25
	sts i,r24
	rjmp .L2
.L3:
	lds r24,i
	lds r25,(i)+1
	sbiw r24,1
	sts (i)+1,r25
	sts i,r24
.L2:
	lds r24,i
	lds r25,(i)+1
	sbiw r24,0
	brne .L3
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
	push r29
	push r28
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
	ldi r30,lo8(55)
	ldi r31,hi8(55)
	ldi r24,lo8(-1)
	st Z,r24
	std Y+1,__zero_reg__
.L6:
	ldi r30,lo8(56)
	ldi r31,hi8(56)
	ldd r24,Y+1
	com r24
	st Z,r24
	ldi r24,lo8(-6072)
	ldi r25,hi8(-6072)
	rcall delay
	ldd r24,Y+1
	subi r24,lo8(-(1))
	std Y+1,r24
	rjmp .L6
	.size	main, .-main
	.comm i,2,1
.global __do_clear_bss
