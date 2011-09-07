	;; Doesn't work!
	.ORIG 3000 		; Origin at 0x3000
	AND R0, R0, #0		; Clear the registers
	AND R1, R1, #0
	AND R2, R2, #0
	AND R3, R3, #0
	AND R4, R4, #0
	
	ADD R4, R4, #15  	; Store neg ASCII 2's comp of = in R4
	ADD R4, R4, #15		;in dec - 3d = 0011 1110 =  1100 0010 = C2 = FFC2 
	ADD R4, R4, #15
	ADD R4, R4, #15
	NOT R4,	R4
	ADD R2, R0, R1		; Start of loop
	ADD R1, R2, #0
	TRAP X23	
	ADD R3, R0, R4
	BRz 1
	BRnzp -6
	AND R0, R0, #0
	ADD R0, R1, #0
	TRAP x21
	HALT
	.END 		