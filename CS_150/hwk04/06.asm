.ORIG 3000
	START
	;; R0
	and r0, r0, #0
	
	;; R1
	and r1, r1, #0
	add r1,r1,#15
	add r1,r1,#15	
	add r1,r1,#15
	add r1,r1,#15
	add r1,r1,#9

	;; R2
	and r2, r2, #0
	add r2,r2,#15
	add r2,r2,#1

	;; R3
	and r3, r3, #0

	;; R6
	and r6, r6, #0
	
	add r6,r6,#-2
	str r2,r6,#0
	str r3,r6,#1
	and r0,r0,#0
	and r3,r3,#0
	add r2,r2,#0
	brzp L02
	not r2,r2
	add r2,r2,#1
	add r3,r3,#-1
	L02 add r2,r2,#0
	brz L04
	add r2,r2,#-1
	add r0,r0,r1
	brnzp L02
	L04 add r3,r3,#0
	brz L06
	not r0,r0
	add r0,r0,#1
	L06 ldr r2,r6,#0
	ldr r3,r6,#1
	add r6,r6,#2
	ret
HALT
.END