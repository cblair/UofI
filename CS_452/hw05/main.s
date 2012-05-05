	.file	"main.c"
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	sendData
	.type	sendData, @function
sendData:
/* prologue: function */
/* frame size = 0 */
	ldi r25,lo8(3)
	out 50-32,r25
	out 59-32,r24
	ldi r24,lo8(-25536)
	ldi r25,hi8(-25536)
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(2)
	out 50-32,r24
	ldi r24,lo8(1)
	ldi r25,hi8(1)
/* epilogue start */
	ret
	.size	sendData, .-sendData
.global	sendControl
	.type	sendControl, @function
sendControl:
/* prologue: function */
/* frame size = 0 */
	ldi r25,lo8(1)
	out 50-32,r25
	out 59-32,r24
	ldi r24,lo8(4000)
	ldi r25,hi8(4000)
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	out 50-32,__zero_reg__
	ldi r24,lo8(16000)
	ldi r25,hi8(16000)
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* epilogue start */
/* #NOAPP */
	ret
	.size	sendControl, .-sendControl
.global	lcdInit
	.type	lcdInit, @function
lcdInit:
/* prologue: function */
/* frame size = 0 */
	out 50-32,__zero_reg__
	out 59-32,__zero_reg__
	ldi r24,lo8(-5536)
	ldi r25,hi8(-5536)
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	ldi r25,lo8(1)
	out 50-32,r25
	ldi r24,lo8(56)
	out 59-32,r24
	ldi r18,lo8(4000)
	ldi r19,hi8(4000)
	movw r30,r18
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	out 50-32,__zero_reg__
	ldi r20,lo8(16000)
	ldi r21,hi8(16000)
	movw r30,r20
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	movw r30,r18
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	out 50-32,r25
	out 59-32,r25
	movw r30,r18
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	out 50-32,__zero_reg__
	movw r30,r20
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	movw r30,r18
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	out 50-32,r25
	ldi r24,lo8(13)
	out 59-32,r24
	movw r30,r18
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	out 50-32,__zero_reg__
	movw r30,r20
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	movw r30,r18
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	out 50-32,r25
	ldi r24,lo8(6)
	out 59-32,r24
	movw r30,r18
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	out 50-32,__zero_reg__
	movw r30,r20
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	movw r30,r18
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	out 50-32,r25
	ldi r24,lo8(-128)
	out 59-32,r24
	movw r24,r18
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	out 50-32,__zero_reg__
	movw r24,r20
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* epilogue start */
/* #NOAPP */
	ret
	.size	lcdInit, .-lcdInit
.global	newline
	.type	newline, @function
newline:
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(1)
	out 50-32,r24
	ldi r24,lo8(-88)
	out 59-32,r24
	ldi r24,lo8(4000)
	ldi r25,hi8(4000)
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	out 50-32,__zero_reg__
	ldi r24,lo8(16000)
	ldi r25,hi8(16000)
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* epilogue start */
/* #NOAPP */
	ret
	.size	newline, .-newline
.global	putchar
	.type	putchar, @function
putchar:
/* prologue: function */
/* frame size = 0 */
	ldi r25,lo8(3)
	out 50-32,r25
	out 59-32,r24
	ldi r24,lo8(-25536)
	ldi r25,hi8(-25536)
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(2)
	out 50-32,r24
	ldi r24,lo8(1)
	ldi r25,hi8(1)
/* epilogue start */
	ret
	.size	putchar, .-putchar
.global	putstr
	.type	putstr, @function
putstr:
	push r16
	push r17
/* prologue: function */
/* frame size = 0 */
	movw r20,r24
	ldi r18,lo8(0)
	ldi r19,hi8(0)
	ldi r16,lo8(3)
	ldi r26,lo8(-25536)
	ldi r27,hi8(-25536)
	ldi r17,lo8(2)
	rjmp .L12
.L13:
	movw r30,r20
	add r30,r18
	adc r31,r19
	ld r24,Z
	clr r25
	sbrc r24,7
	com r25
	out 50-32,r16
	out 59-32,r24
	movw r24,r26
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	out 50-32,r17
	subi r18,lo8(-(1))
	sbci r19,hi8(-(1))
.L12:
	cp r18,r22
	cpc r19,r23
	brlt .L13
/* epilogue start */
	pop r17
	pop r16
	ret
	.size	putstr, .-putstr
	.data
.LC0:
	.string	"Hello"
.LC1:
	.string	"World"
	.text
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(-1)
	out 58-32,r24
	out 49-32,r24
	rcall lcdInit
	ldi r24,lo8(1000)
	ldi r25,hi8(1000)
	ldi r18,lo8(200)
	ldi r19,hi8(200)
.L16:
	movw r30,r18
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	sbiw r24,1
	brne .L16
	ldi r24,lo8(.LC0)
	ldi r25,hi8(.LC0)
	ldi r22,lo8(5)
	ldi r23,hi8(5)
	rcall putstr
	ldi r24,lo8(1)
	out 50-32,r24
	ldi r24,lo8(-88)
	out 59-32,r24
	ldi r24,lo8(4000)
	ldi r25,hi8(4000)
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	out 50-32,__zero_reg__
	ldi r24,lo8(16000)
	ldi r25,hi8(16000)
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(.LC1)
	ldi r25,hi8(.LC1)
	ldi r22,lo8(5)
	ldi r23,hi8(5)
	rcall putstr
	ldi r24,lo8(0)
	ldi r25,hi8(0)
/* epilogue start */
	ret
	.size	main, .-main
.global __do_copy_data
