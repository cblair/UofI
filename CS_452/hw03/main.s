	.file	"main.c"
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
.global	cnt
.global	cnt
	.section .bss
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
.global	main
	.type	main, @function
main:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,51
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 51 */
	std Y+1,__zero_reg__
	std Y+2,__zero_reg__
	std Y+3,__zero_reg__
	std Y+4,__zero_reg__
	std Y+5,__zero_reg__
	std Y+6,__zero_reg__
	std Y+7,__zero_reg__
	std Y+8,__zero_reg__
	ldi r30,lo8(58)
	ldi r31,hi8(58)
	ldi r24,lo8(-1)
	st Z,r24
	ldi r30,lo8(55)
	ldi r31,hi8(55)
	ldi r24,lo8(-1)
	st Z,r24
.L21:
	ldi r30,lo8(59)
	ldi r31,hi8(59)
	ldi r24,lo8(-2)
	st Z,r24
	ldi r30,lo8(56)
	ldi r31,hi8(56)
	ldi r24,lo8(-2)
	st Z,r24
	ldd r24,Y+1
	ldd r25,Y+2
	rcall delay
	ldi r30,lo8(59)
	ldi r31,hi8(59)
	ldi r24,lo8(-1)
	st Z,r24
	ldi r30,lo8(56)
	ldi r31,hi8(56)
	ldi r24,lo8(-1)
	st Z,r24
	ldd r24,Y+1
	ldd r25,Y+2
	rcall delay
	ldd r24,Y+1
	std Y+9,r24
	ldd r25,Y+2
	std Y+10,r25
	ldd r24,Y+3
	std Y+11,r24
	ldd r25,Y+4
	std Y+12,r25
	ldd r24,Y+5
	std Y+13,r24
	ldd r25,Y+6
	std Y+14,r25
	ldd r24,Y+7
	std Y+15,r24
	ldd r25,Y+8
	std Y+16,r25
	ldi r24,lo8(64)
	std Y+17,r24
	std Y+18,__zero_reg__
	std Y+19,__zero_reg__
	std Y+20,__zero_reg__
	std Y+21,__zero_reg__
	std Y+22,__zero_reg__
	std Y+23,__zero_reg__
	std Y+24,__zero_reg__
	ldd r25,Y+9
	ldd r24,Y+17
	add r25,r24
	std Y+25,r25
	ldi r25,lo8(1)
	std Y+33,r25
	ldd r24,Y+25
	ldd r25,Y+9
	cp r24,r25
	brlo .L6
	std Y+33,__zero_reg__
.L6:
	ldd r24,Y+10
	ldd r25,Y+18
	add r24,r25
	std Y+26,r24
	ldi r24,lo8(1)
	std Y+34,r24
	ldd r25,Y+26
	ldd r24,Y+10
	cp r25,r24
	brlo .L7
	std Y+34,__zero_reg__
.L7:
	ldd r25,Y+33
	ldd r24,Y+26
	add r25,r24
	std Y+35,r25
	ldi r25,lo8(1)
	std Y+36,r25
	ldd r24,Y+35
	ldd r25,Y+26
	cp r24,r25
	brlo .L8
	std Y+36,__zero_reg__
.L8:
	ldd r24,Y+34
	ldd r25,Y+36
	or r24,r25
	std Y+34,r24
	ldd r24,Y+35
	std Y+26,r24
	ldd r25,Y+11
	ldd r24,Y+19
	add r25,r24
	std Y+27,r25
	ldi r25,lo8(1)
	std Y+37,r25
	ldd r24,Y+27
	ldd r25,Y+11
	cp r24,r25
	brlo .L9
	std Y+37,__zero_reg__
.L9:
	ldd r24,Y+34
	ldd r25,Y+27
	add r24,r25
	std Y+38,r24
	ldi r24,lo8(1)
	std Y+39,r24
	ldd r25,Y+38
	ldd r24,Y+27
	cp r25,r24
	brlo .L10
	std Y+39,__zero_reg__
.L10:
	ldd r25,Y+37
	ldd r24,Y+39
	or r25,r24
	std Y+37,r25
	ldd r25,Y+38
	std Y+27,r25
	ldd r24,Y+12
	ldd r25,Y+20
	add r24,r25
	std Y+28,r24
	ldi r24,lo8(1)
	std Y+40,r24
	ldd r25,Y+28
	ldd r24,Y+12
	cp r25,r24
	brlo .L11
	std Y+40,__zero_reg__
.L11:
	ldd r25,Y+37
	ldd r24,Y+28
	add r25,r24
	std Y+41,r25
	ldi r25,lo8(1)
	std Y+42,r25
	ldd r24,Y+41
	ldd r25,Y+28
	cp r24,r25
	brlo .L12
	std Y+42,__zero_reg__
.L12:
	ldd r24,Y+40
	ldd r25,Y+42
	or r24,r25
	std Y+40,r24
	ldd r24,Y+41
	std Y+28,r24
	ldd r25,Y+13
	ldd r24,Y+21
	add r25,r24
	std Y+29,r25
	ldi r25,lo8(1)
	std Y+43,r25
	ldd r24,Y+29
	ldd r25,Y+13
	cp r24,r25
	brlo .L13
	std Y+43,__zero_reg__
.L13:
	ldd r24,Y+40
	ldd r25,Y+29
	add r24,r25
	std Y+44,r24
	ldi r24,lo8(1)
	std Y+45,r24
	ldd r25,Y+44
	ldd r24,Y+29
	cp r25,r24
	brlo .L14
	std Y+45,__zero_reg__
.L14:
	ldd r25,Y+43
	ldd r24,Y+45
	or r25,r24
	std Y+43,r25
	ldd r25,Y+44
	std Y+29,r25
	ldd r24,Y+14
	ldd r25,Y+22
	add r24,r25
	std Y+30,r24
	ldi r24,lo8(1)
	std Y+46,r24
	ldd r25,Y+30
	ldd r24,Y+14
	cp r25,r24
	brlo .L15
	std Y+46,__zero_reg__
.L15:
	ldd r25,Y+43
	ldd r24,Y+30
	add r25,r24
	std Y+47,r25
	ldi r25,lo8(1)
	std Y+48,r25
	ldd r24,Y+47
	ldd r25,Y+30
	cp r24,r25
	brlo .L16
	std Y+48,__zero_reg__
.L16:
	ldd r24,Y+46
	ldd r25,Y+48
	or r24,r25
	std Y+46,r24
	ldd r24,Y+47
	std Y+30,r24
	ldd r25,Y+15
	ldd r24,Y+23
	add r25,r24
	std Y+31,r25
	ldi r25,lo8(1)
	std Y+49,r25
	ldd r24,Y+31
	ldd r25,Y+15
	cp r24,r25
	brlo .L17
	std Y+49,__zero_reg__
.L17:
	ldd r24,Y+46
	ldd r25,Y+31
	add r24,r25
	std Y+50,r24
	ldi r24,lo8(1)
	std Y+51,r24
	ldd r25,Y+50
	ldd r24,Y+31
	cp r25,r24
	brlo .L18
	std Y+51,__zero_reg__
.L18:
	ldd r25,Y+49
	ldd r24,Y+51
	or r25,r24
	std Y+49,r25
	ldd r25,Y+50
	std Y+31,r25
	ldd r24,Y+16
	ldd r25,Y+24
	add r24,r25
	std Y+32,r24
	ldd r24,Y+49
	ldd r25,Y+32
	add r24,r25
	std Y+32,r24
	ldd r24,Y+25
	std Y+1,r24
	ldd r25,Y+26
	std Y+2,r25
	ldd r24,Y+27
	std Y+3,r24
	ldd r25,Y+28
	std Y+4,r25
	ldd r24,Y+29
	std Y+5,r24
	ldd r25,Y+30
	std Y+6,r25
	ldd r24,Y+31
	std Y+7,r24
	ldd r25,Y+32
	std Y+8,r25
	ldd r24,Y+8
	tst r24
	brne .L23
	ldd r24,Y+8
	tst r24
	breq .+2
	rjmp .L21
	ldd r24,Y+7
	tst r24
	brne .L23
	ldd r24,Y+7
	tst r24
	breq .+2
	rjmp .L21
	ldd r24,Y+6
	tst r24
	brne .L23
	ldd r24,Y+6
	tst r24
	breq .+2
	rjmp .L21
	ldd r24,Y+5
	tst r24
	brne .L23
	ldd r24,Y+5
	tst r24
	breq .+2
	rjmp .L21
	ldd r24,Y+4
	tst r24
	brne .L23
	ldd r24,Y+4
	tst r24
	breq .+2
	rjmp .L21
	ldd r24,Y+3
	tst r24
	brne .L23
	ldd r24,Y+3
	tst r24
	breq .+2
	rjmp .L21
	ldd r24,Y+2
	cpi r24,lo8(36)
	brsh .L23
	ldd r24,Y+2
	cpi r24,lo8(35)
	breq .+2
	rjmp .L21
	ldd r24,Y+1
	cpi r24,lo8(41)
	brsh .+2
	rjmp .L21
.L23:
	std Y+1,__zero_reg__
	std Y+2,__zero_reg__
	std Y+3,__zero_reg__
	std Y+4,__zero_reg__
	std Y+5,__zero_reg__
	std Y+6,__zero_reg__
	std Y+7,__zero_reg__
	std Y+8,__zero_reg__
	rjmp .L21
	.size	main, .-main
.global __do_clear_bss
