	.file	"main--broken.c"
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	LCD_getaddr
	.type	LCD_getaddr, @function
LCD_getaddr:
	push r29
	push r28
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
	ldi r26,lo8(53)
	ldi r27,hi8(53)
	ldi r30,lo8(53)
	ldi r31,hi8(53)
	ld r24,Z
	andi r24,lo8(63)
	st X,r24
	ldi r26,lo8(39)
	ldi r27,hi8(39)
	ldi r30,lo8(39)
	ldi r31,hi8(39)
	ld r24,Z
	andi r24,lo8(-2)
	st X,r24
	ldi r26,lo8(39)
	ldi r27,hi8(39)
	ldi r30,lo8(39)
	ldi r31,hi8(39)
	ld r24,Z
	ori r24,lo8(1)
	st X,r24
	ldi r30,lo8(49)
	ldi r31,hi8(49)
	st Z,__zero_reg__
	ldi r26,lo8(53)
	ldi r27,hi8(53)
	ldi r30,lo8(53)
	ldi r31,hi8(53)
	ld r24,Z
	ori r24,lo8(-128)
	st X,r24
/* #APP */
 ;  61 "main--broken.c" 1
	nop
 ;  0 "" 2
 ;  62 "main--broken.c" 1
	nop
 ;  0 "" 2
/* #NOAPP */
	ldi r30,lo8(57)
	ldi r31,hi8(57)
	ld r24,Z
	std Y+1,r24
	ldi r26,lo8(53)
	ldi r27,hi8(53)
	ldi r30,lo8(53)
	ldi r31,hi8(53)
	ld r24,Z
	andi r24,lo8(127)
	st X,r24
	ldi r26,lo8(39)
	ldi r27,hi8(39)
	ldi r30,lo8(39)
	ldi r31,hi8(39)
	ld r24,Z
	ori r24,lo8(1)
	st X,r24
	ldi r26,lo8(39)
	ldi r27,hi8(39)
	ldi r30,lo8(39)
	ldi r31,hi8(39)
	ld r24,Z
	andi r24,lo8(-2)
	st X,r24
	ldi r30,lo8(49)
	ldi r31,hi8(49)
	ldi r24,lo8(-1)
	st Z,r24
	ldd r24,Y+1
/* epilogue start */
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	LCD_getaddr, .-LCD_getaddr
.global	LCD_wait
	.type	LCD_wait, @function
LCD_wait:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
.L4:
	rcall LCD_getaddr
	tst r24
	brlt .L4
/* epilogue start */
	pop r28
	pop r29
	ret
	.size	LCD_wait, .-LCD_wait
.global	LCD_putchar
	.type	LCD_putchar, @function
LCD_putchar:
	push r29
	push r28
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
	std Y+1,r24
	rcall LCD_wait
	ldi r30,lo8(49)
	ldi r31,hi8(49)
	ldi r24,lo8(-1)
	st Z,r24
	ldi r26,lo8(53)
	ldi r27,hi8(53)
	ldi r30,lo8(53)
	ldi r31,hi8(53)
	ld r24,Z
	ori r24,lo8(-64)
	st X,r24
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldd r24,Y+1
	st Z,r24
/* #APP */
 ;  101 "main--broken.c" 1
	nop
 ;  0 "" 2
 ;  102 "main--broken.c" 1
	nop
 ;  0 "" 2
 ;  103 "main--broken.c" 1
	nop
 ;  0 "" 2
 ;  104 "main--broken.c" 1
	nop
 ;  0 "" 2
/* #NOAPP */
	ldi r26,lo8(53)
	ldi r27,hi8(53)
	ldi r30,lo8(53)
	ldi r31,hi8(53)
	ld r24,Z
	andi r24,lo8(127)
	st X,r24
	ldi r30,lo8(49)
	ldi r31,hi8(49)
	st Z,__zero_reg__
/* epilogue start */
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	LCD_putchar, .-LCD_putchar
.global	LCD_command
	.type	LCD_command, @function
LCD_command:
	push r29
	push r28
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
	std Y+1,r24
	rcall LCD_wait
	ldi r30,lo8(49)
	ldi r31,hi8(49)
	ldi r24,lo8(-1)
	st Z,r24
	ldi r26,lo8(53)
	ldi r27,hi8(53)
	ldi r30,lo8(53)
	ldi r31,hi8(53)
	ld r24,Z
	andi r24,lo8(-65)
	st X,r24
	ldi r26,lo8(53)
	ldi r27,hi8(53)
	ldi r30,lo8(53)
	ldi r31,hi8(53)
	ld r24,Z
	ori r24,lo8(-128)
	st X,r24
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldd r24,Y+1
	st Z,r24
/* #APP */
 ;  120 "main--broken.c" 1
	nop
 ;  0 "" 2
 ;  121 "main--broken.c" 1
	nop
 ;  0 "" 2
 ;  122 "main--broken.c" 1
	nop
 ;  0 "" 2
 ;  123 "main--broken.c" 1
	nop
 ;  0 "" 2
/* #NOAPP */
	ldi r26,lo8(53)
	ldi r27,hi8(53)
	ldi r30,lo8(53)
	ldi r31,hi8(53)
	ld r24,Z
	andi r24,lo8(127)
	st X,r24
	ldi r30,lo8(49)
	ldi r31,hi8(49)
	st Z,__zero_reg__
/* epilogue start */
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	LCD_command, .-LCD_command
.global	LCD_init
	.type	LCD_init, @function
LCD_init:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,14
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 14 */
	ldi r26,lo8(52)
	ldi r27,hi8(52)
	ldi r30,lo8(52)
	ldi r31,hi8(52)
	ld r24,Z
	ori r24,lo8(-64)
	st X,r24
	ldi r26,lo8(53)
	ldi r27,hi8(53)
	ldi r30,lo8(53)
	ldi r31,hi8(53)
	ld r24,Z
	andi r24,lo8(63)
	st X,r24
	ldi r26,lo8(38)
	ldi r27,hi8(38)
	ldi r30,lo8(38)
	ldi r31,hi8(38)
	ld r24,Z
	ori r24,lo8(1)
	st X,r24
	ldi r26,lo8(39)
	ldi r27,hi8(39)
	ldi r30,lo8(39)
	ldi r31,hi8(39)
	ld r24,Z
	andi r24,lo8(-2)
	st X,r24
	ldi r30,lo8(49)
	ldi r31,hi8(49)
	ldi r24,lo8(-1)
	st Z,r24
	ldi r24,lo8(0x42480000)
	ldi r25,hi8(0x42480000)
	ldi r26,hlo8(0x42480000)
	ldi r27,hhi8(0x42480000)
	std Y+11,r24
	std Y+12,r25
	std Y+13,r26
	std Y+14,r27
	ldd r22,Y+11
	ldd r23,Y+12
	ldd r24,Y+13
	ldd r25,Y+14
	ldi r18,lo8(0x44fa0000)
	ldi r19,hi8(0x44fa0000)
	ldi r20,hlo8(0x44fa0000)
	ldi r21,hhi8(0x44fa0000)
	rcall __mulsf3
	movw r26,r24
	movw r24,r22
	std Y+7,r24
	std Y+8,r25
	std Y+9,r26
	std Y+10,r27
	ldd r22,Y+7
	ldd r23,Y+8
	ldd r24,Y+9
	ldd r25,Y+10
	ldi r18,lo8(0x3f800000)
	ldi r19,hi8(0x3f800000)
	ldi r20,hlo8(0x3f800000)
	ldi r21,hhi8(0x3f800000)
	rcall __ltsf2
	tst r24
	brge .L22
.L20:
	ldi r24,lo8(1)
	ldi r25,hi8(1)
	std Y+6,r25
	std Y+5,r24
	rjmp .L13
.L22:
	ldd r22,Y+7
	ldd r23,Y+8
	ldd r24,Y+9
	ldd r25,Y+10
	ldi r18,lo8(0x477fff00)
	ldi r19,hi8(0x477fff00)
	ldi r20,hlo8(0x477fff00)
	ldi r21,hhi8(0x477fff00)
	rcall __gtsf2
	cp __zero_reg__,r24
	brge .L23
.L21:
	ldd r22,Y+11
	ldd r23,Y+12
	ldd r24,Y+13
	ldd r25,Y+14
	ldi r18,lo8(0x41200000)
	ldi r19,hi8(0x41200000)
	ldi r20,hlo8(0x41200000)
	ldi r21,hhi8(0x41200000)
	rcall __mulsf3
	movw r26,r24
	movw r24,r22
	movw r22,r24
	movw r24,r26
	rcall __fixunssfsi
	movw r26,r24
	movw r24,r22
	std Y+6,r25
	std Y+5,r24
	rjmp .L16
.L17:
	ldi r24,lo8(200)
	ldi r25,hi8(200)
	std Y+4,r25
	std Y+3,r24
	ldd r24,Y+3
	ldd r25,Y+4
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	std Y+4,r25
	std Y+3,r24
	ldd r24,Y+5
	ldd r25,Y+6
	sbiw r24,1
	std Y+6,r25
	std Y+5,r24
.L16:
	ldd r24,Y+5
	ldd r25,Y+6
	sbiw r24,0
	brne .L17
	rjmp .L18
.L23:
	ldd r22,Y+7
	ldd r23,Y+8
	ldd r24,Y+9
	ldd r25,Y+10
	rcall __fixunssfsi
	movw r26,r24
	movw r24,r22
	std Y+6,r25
	std Y+5,r24
.L13:
	ldd r24,Y+5
	ldd r25,Y+6
	std Y+2,r25
	std Y+1,r24
	ldd r24,Y+1
	ldd r25,Y+2
/* #APP */
 ;  105 "/usr/local/CrossPack-AVR-20100115/lib/gcc/avr/4.3.3/../../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	std Y+2,r25
	std Y+1,r24
.L18:
	ldi r24,lo8(1)
	rcall LCD_command
	ldi r24,lo8(56)
	rcall LCD_command
	ldi r24,lo8(6)
	rcall LCD_command
	ldi r24,lo8(16)
	rcall LCD_command
	ldi r24,lo8(15)
	rcall LCD_command
	ldi r24,lo8(-128)
	rcall LCD_command
/* epilogue start */
	adiw r28,14
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r28
	pop r29
	ret
	.size	LCD_init, .-LCD_init
.global	LCD_write
	.type	LCD_write, @function
LCD_write:
	push r29
	push r28
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
	std Y+2,r25
	std Y+1,r24
	rjmp .L25
.L26:
	ldd r30,Y+1
	ldd r31,Y+2
	ld r24,Z
	mov r18,r24
	ldd r24,Y+1
	ldd r25,Y+2
	adiw r24,1
	std Y+2,r25
	std Y+1,r24
	mov r24,r18
	rcall LCD_putchar
.L25:
	ldd r30,Y+1
	ldd r31,Y+2
	ld r24,Z
	tst r24
	brne .L26
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	LCD_write, .-LCD_write
	.data
.LC0:
	.string	"Hello World"
.LC1:
	.string	"is here"
	.text
.global	main
	.type	main, @function
main:
	push r29
	push r28
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
	rcall LCD_init
	ldi r24,lo8(.LC0)
	ldi r25,hi8(.LC0)
	rcall LCD_write
	ldi r24,lo8(-59)
	rcall LCD_command
	ldi r24,lo8(.LC1)
	ldi r25,hi8(.LC1)
	rcall LCD_write
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	main, .-main
.global __do_copy_data
