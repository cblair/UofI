	.file	"main.c"
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	setabs
	.type	setabs, @function
setabs:
/* prologue: function */
/* frame size = 0 */
	mov r18,r22
	andi r18,lo8(15)
	com r18
	out 53-32,r18
	mov r22,r23
	mov r23,r24
	mov r24,r25
	clr r25
	com r22
	out 56-32,r22
/* epilogue start */
	ret
	.size	setabs, .-setabs
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(-1)
	out 55-32,r24
	out 56-32,r24
	ldi r21,lo8(-12)
	ldi r18,lo8(25)
	ldi r19,hi8(25)
	ldi r20,lo8(-1)
.L6:
	out 53-32,r21
	out 56-32,__zero_reg__
	ldi r24,lo8(5000)
	ldi r25,hi8(5000)
.L4:
	movw r30,r18
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	sbiw r24,1
	brne .L4
	out 56-32,r20
	ldi r24,lo8(5000)
	ldi r25,hi8(5000)
.L5:
	movw r30,r18
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	sbiw r24,1
	brne .L5
	rjmp .L6
	.size	main, .-main
