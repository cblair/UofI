	.file	"main.c"
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
.global	SECRET_CODE
	.data
	.type	SECRET_CODE, @object
	.size	SECRET_CODE, 4
SECRET_CODE:
	.byte	52
	.byte	53
	.byte	54
	.byte	66
.global	ENTRY_CODE
	.type	ENTRY_CODE, @object
	.size	ENTRY_CODE, 4
ENTRY_CODE:
	.byte	48
	.byte	48
	.byte	48
	.byte	48
.global	ENTRY_COUNT
.global	ENTRY_COUNT
	.section .bss
	.type	ENTRY_COUNT, @object
	.size	ENTRY_COUNT, 1
ENTRY_COUNT:
	.skip 1,0
	.text
.global	delay
	.type	delay, @function
delay:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,10
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 10 */
	std Y+3,r18
	std Y+4,r19
	std Y+5,r20
	std Y+6,r21
	std Y+7,r22
	std Y+8,r23
	std Y+9,r24
	std Y+10,r25
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
	adiw r28,10
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r28
	pop r29
	ret
	.size	delay, .-delay
.global	secret_code_check
	.type	secret_code_check, @function
secret_code_check:
	push r29
	push r28
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
	std Y+2,__zero_reg__
	std Y+1,__zero_reg__
	rjmp .L6
.L9:
	ldd r24,Y+1
	ldd r25,Y+2
	movw r30,r24
	subi r30,lo8(-(ENTRY_CODE))
	sbci r31,hi8(-(ENTRY_CODE))
	ld r18,Z
	ldd r24,Y+1
	ldd r25,Y+2
	movw r30,r24
	subi r30,lo8(-(SECRET_CODE))
	sbci r31,hi8(-(SECRET_CODE))
	ld r24,Z
	cp r18,r24
	breq .L7
	ldi r30,lo8(56)
	ldi r31,hi8(56)
	ldi r24,lo8(-65)
	st Z,r24
	rjmp .L10
.L7:
	ldd r24,Y+1
	ldd r25,Y+2
	adiw r24,1
	std Y+2,r25
	std Y+1,r24
.L6:
	ldd r24,Y+1
	ldd r25,Y+2
	cpi r24,4
	cpc r25,__zero_reg__
	brlt .L9
	ldi r30,lo8(56)
	ldi r31,hi8(56)
	ldi r24,lo8(127)
	st Z,r24
.L10:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	secret_code_check, .-secret_code_check
.global	init_keypad
	.type	init_keypad, @function
init_keypad:
	push r29
	push r28
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
	ldi r24,lo8(-117)
	std Y+1,r24
	ldd r24,Y+1
/* epilogue start */
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	init_keypad, .-init_keypad
.global	get_led_entry_code
	.type	get_led_entry_code, @function
get_led_entry_code:
	push r29
	push r28
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
	std Y+2,r24
	std Y+1,__zero_reg__
	ldd r24,Y+2
	cpi r24,lo8(1)
	brne .L14
	ldi r24,lo8(1)
	std Y+1,r24
	rjmp .L15
.L14:
	ldd r24,Y+2
	cpi r24,lo8(2)
	brne .L16
	ldi r24,lo8(3)
	std Y+1,r24
	rjmp .L15
.L16:
	ldd r24,Y+2
	cpi r24,lo8(3)
	brne .L15
	ldi r24,lo8(7)
	std Y+1,r24
.L15:
	ldd r24,Y+1
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	ret
	.size	get_led_entry_code, .-get_led_entry_code
.global	keypad_key_check
	.type	keypad_key_check, @function
keypad_key_check:
	push r16
	push r17
	push r29
	push r28
	rcall .
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 3 */
	std Y+1,r24
	std Y+2,r22
	std Y+3,r20
	ldd r24,Y+1
	mov r24,r24
	ldi r25,lo8(0)
	movw r18,r24
	com r18
	com r19
	ldd r24,Y+2
	mov r24,r24
	ldi r25,lo8(0)
	and r24,r18
	and r25,r19
	sbiw r24,0
	breq .L20
	ldi r18,lo8(-64)
	ldi r19,lo8(-107)
	ldi r20,lo8(-87)
	ldi r21,lo8(5)
	ldi r22,lo8(0)
	ldi r23,lo8(0)
	ldi r24,lo8(0)
	ldi r25,lo8(0)
	rcall delay
	ldi r18,lo8(-64)
	ldi r19,lo8(-107)
	ldi r20,lo8(-87)
	ldi r21,lo8(5)
	ldi r22,lo8(0)
	ldi r23,lo8(0)
	ldi r24,lo8(0)
	ldi r25,lo8(0)
	rcall delay
	ldd r24,Y+1
	mov r24,r24
	ldi r25,lo8(0)
	movw r18,r24
	com r18
	com r19
	ldd r24,Y+2
	mov r24,r24
	ldi r25,lo8(0)
	and r24,r18
	and r25,r19
	sbiw r24,0
	breq .L20
	lds r24,ENTRY_COUNT
	mov r24,r24
	ldi r25,lo8(0)
	movw r30,r24
	subi r30,lo8(-(ENTRY_CODE))
	sbci r31,hi8(-(ENTRY_CODE))
	ldd r24,Y+3
	st Z,r24
	lds r24,ENTRY_COUNT
	subi r24,lo8(-(1))
	sts ENTRY_COUNT,r24
	ldi r16,lo8(56)
	ldi r17,hi8(56)
	lds r24,ENTRY_COUNT
	rcall get_led_entry_code
	com r24
	movw r30,r16
	st Z,r24
.L20:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	pop r17
	pop r16
	ret
	.size	keypad_key_check, .-keypad_key_check
.global	keypad_to_led
	.type	keypad_to_led, @function
keypad_to_led:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 0 */
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(127)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(4)
	ldi r20,lo8(49)
	rcall keypad_key_check
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(127)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(16)
	ldi r20,lo8(50)
	rcall keypad_key_check
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(127)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(32)
	ldi r20,lo8(51)
	rcall keypad_key_check
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(127)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(64)
	ldi r20,lo8(65)
	rcall keypad_key_check
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(-2)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(4)
	ldi r20,lo8(52)
	rcall keypad_key_check
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(-2)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(16)
	ldi r20,lo8(53)
	rcall keypad_key_check
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(-2)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(32)
	ldi r20,lo8(54)
	rcall keypad_key_check
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(-2)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(64)
	ldi r20,lo8(66)
	rcall keypad_key_check
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(-3)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(4)
	ldi r20,lo8(55)
	rcall keypad_key_check
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(-3)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(16)
	ldi r20,lo8(56)
	rcall keypad_key_check
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(-3)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(32)
	ldi r20,lo8(57)
	rcall keypad_key_check
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(-3)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(64)
	ldi r20,lo8(67)
	rcall keypad_key_check
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(-9)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(4)
	ldi r20,lo8(42)
	rcall keypad_key_check
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(-9)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(16)
	ldi r20,lo8(48)
	rcall keypad_key_check
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(-9)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(32)
	ldi r20,lo8(35)
	rcall keypad_key_check
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldi r24,lo8(-9)
	st Z,r24
	ldi r30,lo8(48)
	ldi r31,hi8(48)
	ld r24,Z
	ldi r22,lo8(64)
	ldi r20,lo8(68)
	rcall keypad_key_check
	lds r24,ENTRY_COUNT
	cpi r24,lo8(4)
	brlo .L23
	rcall secret_code_check
	sts ENTRY_COUNT,__zero_reg__
.L23:
/* epilogue start */
	pop r28
	pop r29
	ret
	.size	keypad_to_led, .-keypad_to_led
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
	rcall init_keypad
	std Y+1,r24
	ldi r30,lo8(55)
	ldi r31,hi8(55)
	ldi r24,lo8(-1)
	st Z,r24
	ldi r30,lo8(49)
	ldi r31,hi8(49)
	ldd r24,Y+1
	st Z,r24
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ldd r24,Y+1
	com r24
	st Z,r24
	ldi r30,lo8(56)
	ldi r31,hi8(56)
	ldi r24,lo8(-1)
	st Z,r24
.L25:
	rcall keypad_to_led
	rjmp .L25
	.size	main, .-main
.global __do_copy_data
.global __do_clear_bss
