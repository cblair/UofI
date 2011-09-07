	.ORIG 3000		; Origin at 0x3000
	AND R0, R0, #0		;  Clear the registers
	AND R1, R1, #0			
	AND R2, R2, #0
	AND R3, R3, #0
	AND R4, R4, #0
	
	ADD R4, R4, #15  	; Store neg ASCII 2's comp of = in R4
	ADD R4, R4, #15		;in dec - 3d = 0011 1110 =  1100 0010 = C2 = FFC2 
	ADD R4, R4, #15
	ADD R4, R4, #15
	NOT R4, R4

	ADD R2, R0, R1		; Start of loop. R1 is the sum of the last
	ADD R1, R2, #0		; iteration. Add it to the new input R0,
	TRAP X23		; and store the sun back in R1. Input next.
	ADD R3, R0, R4		; If z flag set, then input was an =, and we
				; branch to the end process
	BRz 5
	ADD R0, R0, #-15	; Otherwise, convert the ASCII val to the real
	ADD R0, R0, #-15	; numerical val for arithmetic operations
	ADD R0, R0, #-15 
	ADD R0, R0, #-3 	
	BRnzp -10		; Since this val was not =, branch back
				; unconditionally. 
	AND R0, R0, #0		; Clear R0 for the output...
	ADD R0, R1, #0		; and put the sum, R1, in it.
	ADD R0, R0, #15		; Convert back to ASCII...
	ADD R0, R0, #15 
	ADD R0, R0, #15 
	ADD R0, R0, #3 	
	TRAP x21		; And output.
	HALT
	.END