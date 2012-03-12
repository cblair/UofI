	.file	"main.c"
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
.global	SONG_INDEX
	.data
	.type	SONG_INDEX, @object
	.size	SONG_INDEX, 2
SONG_INDEX:
	.word	1
.global	ENABLE_MUSIC
.global	ENABLE_MUSIC
	.section .bss
	.type	ENABLE_MUSIC, @object
	.size	ENABLE_MUSIC, 1
ENABLE_MUSIC:
	.skip 1,0
.global	NOTE_COUNT
.global	NOTE_COUNT
	.type	NOTE_COUNT, @object
	.size	NOTE_COUNT, 1
NOTE_COUNT:
	.skip 1,0
.global	cnt
.global	cnt
	.type	cnt, @object
	.size	cnt, 1
cnt:
	.skip 1,0
.global	led_cnt
.global	led_cnt
	.type	led_cnt, @object
	.size	led_cnt, 1
led_cnt:
	.skip 1,0
.global	which_switch
.global	which_switch
	.type	which_switch, @object
	.size	which_switch, 1
which_switch:
	.skip 1,0
.global	idelay
.global	idelay
	.type	idelay, @object
	.size	idelay, 8
idelay:
	.skip 8,0
	.text
.global	delay
	.type	delay, @function
delay:
	push r29
	push r28
	rcall .
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 4 */
	std Y+4,r25
	std Y+3,r24
	ldd r24,Y+3
	ldd r25,Y+4
	std Y+2,r25
	std Y+1,r24
	rjmp .L2
.L3:
	ldd r24,Y+1
	ldd r25,Y+2
	sbiw r24,1
	std Y+2,r25
	std Y+1,r24
.L2:
	ldd r24,Y+1
	ldd r25,Y+2
	sbiw r24,0
	brne .L3
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	delay, .-delay
.global	init_Ex1
	.type	init_Ex1, @function
init_Ex1:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
	ldi r30,lo8(83)
	ldi r31,hi8(83)
	ldi r24,lo8(5)
	st Z,r24
	ldi r30,lo8(88)
	ldi r31,hi8(88)
	ldi r24,lo8(2)
	st Z,r24
	ldi r30,lo8(89)
	ldi r31,hi8(89)
	ldi r24,lo8(2)
	st Z,r24
/* epilogue start */
	pop r28
	pop r29
	ret
	.size	init_Ex1, .-init_Ex1
.global	init_Ex2
	.type	init_Ex2, @function
init_Ex2:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
	ldi r30,lo8(78)
	ldi r31,hi8(78)
	ldi r24,lo8(5)
	st Z,r24
	ldi r30,lo8(78)
	ldi r31,hi8(78)
	ldi r24,lo8(3)
	st Z,r24
/* epilogue start */
	pop r28
	pop r29
	ret
	.size	init_Ex2, .-init_Ex2
.global	__vector_7
	.type	__vector_7, @function
__vector_7:
	push __zero_reg__
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r24
	push r25
	push r30
	push r31
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: Signal */
/* frame size = 0 */
	lds r24,cnt
	subi r24,lo8(-(1))
	sts cnt,r24
	lds r24,cnt
	cpi r24,lo8(15)
	brne .L10
	ldi r24,lo8(1)
	sts ENABLE_MUSIC,r24
	rjmp .L11
.L10:
	lds r24,cnt
	cpi r24,lo8(30)
	brne .L11
	sts cnt,__zero_reg__
	sts ENABLE_MUSIC,__zero_reg__
	lds r24,SONG_INDEX
	lds r25,(SONG_INDEX)+1
	cpi r24,8
	cpc r25,__zero_reg__
	brne .L12
	sts (SONG_INDEX)+1,__zero_reg__
	sts SONG_INDEX,__zero_reg__
	rjmp .L11
.L12:
	lds r24,SONG_INDEX
	lds r25,(SONG_INDEX)+1
	adiw r24,1
	sts (SONG_INDEX)+1,r25
	sts SONG_INDEX,r24
.L11:
	lds r24,ENABLE_MUSIC
	cpi r24,lo8(1)
	brne .L13
	ldi r30,lo8(56)
	ldi r31,hi8(56)
	ldi r24,lo8(-3)
	st Z,r24
	rjmp .L15
.L13:
	ldi r30,lo8(56)
	ldi r31,hi8(56)
	ldi r24,lo8(-2)
	st Z,r24
.L15:
/* epilogue start */
	pop r28
	pop r29
	pop r31
	pop r30
	pop r25
	pop r24
	pop r0
	out __SREG__,r0
	pop r0
	pop __zero_reg__
	reti
	.size	__vector_7, .-__vector_7
.global	__vector_6
	.type	__vector_6, @function
__vector_6:
	push __zero_reg__
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r24
	push r26
	push r27
	push r30
	push r31
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: Signal */
/* frame size = 0 */
	ldi r26,lo8(56)
	ldi r27,hi8(56)
	ldi r30,lo8(56)
	ldi r31,hi8(56)
	ld r24,Z
	com r24
	st X,r24
/* epilogue start */
	pop r28
	pop r29
	pop r31
	pop r30
	pop r27
	pop r26
	pop r24
	pop r0
	out __SREG__,r0
	pop r0
	pop __zero_reg__
	reti
	.size	__vector_6, .-__vector_6
.global	play_note
	.type	play_note, @function
play_note:
	push r29
	push r28
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
	std Y+1,r24
	std Y+2,r22
	ldi r30,lo8(76)
	ldi r31,hi8(76)
	ld r18,Z
	ldd r19,Z+1
	ldd r24,Y+1
	mov r24,r24
	ldi r25,lo8(0)
	cp r18,r24
	cpc r19,r25
	brlo .L20
	ldi r26,lo8(56)
	ldi r27,hi8(56)
	ldi r30,lo8(56)
	ldi r31,hi8(56)
	ld r24,Z
	com r24
	ori r24,lo8(4)
	com r24
	st X,r24
	ldi r26,lo8(59)
	ldi r27,hi8(59)
	ldi r30,lo8(59)
	ldi r31,hi8(59)
	ld r24,Z
	com r24
	st X,r24
	ldi r30,lo8(76)
	ldi r31,hi8(76)
	std Z+1,__zero_reg__
	st Z,__zero_reg__
.L20:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	play_note, .-play_note
.global	main
	.type	main, @function
main:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
	ldi r30,lo8(58)
	ldi r31,hi8(58)
	ldi r24,lo8(-1)
	st Z,r24
	ldi r30,lo8(55)
	ldi r31,hi8(55)
	ldi r24,lo8(-1)
	st Z,r24
	ldi r30,lo8(56)
	ldi r31,hi8(56)
	ldi r24,lo8(-1)
	st Z,r24
	rcall init_Ex1
	rcall init_Ex2
/* #APP */
 ;  133 "main.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(90)
	sts SONG,r24
	ldi r24,lo8(82)
	sts SONG+1,r24
	ldi r24,lo8(76)
	sts SONG+2,r24
	ldi r24,lo8(69)
	sts SONG+3,r24
	ldi r24,lo8(62)
	sts SONG+4,r24
	ldi r24,lo8(55)
	sts SONG+5,r24
	ldi r24,lo8(49)
	sts SONG+6,r24
	ldi r24,lo8(44)
	sts SONG+7,r24
	sts SONG+8,__zero_reg__
.L22:
	lds r24,SONG_INDEX
	lds r25,(SONG_INDEX)+1
	movw r30,r24
	subi r30,lo8(-(SONG))
	sbci r31,hi8(-(SONG))
	ld r24,Z
	ldi r22,lo8(1)
	rcall play_note
	rjmp .L22
	.size	main, .-main
	.comm SONG,9,1
.global __do_copy_data
.global __do_clear_bss
