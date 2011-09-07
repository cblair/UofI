	.ORIG	x3000
	;; Clear regs
	AND R0, R0, #0
	AND R1, R1, #0
	AND R2, R2, #0
	AND R3, R3, #0
	AND R4, R4, #0
	AND R5, R5, #0
	AND R6, R6, #0
	
	;; Print the initial prompt
	LEA R1, PROMPT
	LDR R0, R1, #-0	
	TRAP x21
	ADD R1, R1, #1
	LDR R0, R1, #-0	
	BRnp -5

	;; Input characters
	LEA R5, LOWASCII
	LDR R4, R5, #-0
	LEA R7, HIGHASCII
	LDR R6, R7, #-0
	LEA R1, TWOSCP		; 
	LDR R2, R1, #-0		; Load R2 with ascii test val
	LEA R1, MESSAGE		; Load Message address in R1
	TRAP x20		; input
	ADD R3, R0, R4
	BRn 3			; Test char too low in val
	ADD R3, R0, R6	
	BRp 1			; Test if char too high
	BRnzp 4
	LEA R0, BEEP
	LDR R0, R0, #-0
	TRAP x21
	BRnzp -10
	TRAP x21		; echo input
	STR R0, R1, #0		; store input in add from R1
	ADD R1, R1, #1		; increment R1
	ADD R3, R0, R2		; test to see if input was period
	BRnp -15			; if so, loop back

	;; Clean up the message
	ADD R1, R1, #-1
	AND R0, R0, #0
	STR R0, R1, #-0
	
	;; Print the encode message prompt
	LEA R1, LINE
	LDR R0, R1, #-0
	TRAP x21
	LEA R1, PROMPT2
	LDR R0, R1, #-0	
	TRAP x21
	ADD R1, R1, #1
	LDR R0, R1, #-0	
	BRnp -5
	
	;; Change mem vals to an XOR with the secret char
	AND R6, R6, #0	; Clear count
	LEA R1, MESSAGE	; A- Load R2 with the char in mem
        LEA R5, SECRET	; B- Load R3 with the secret char; iteration
			; will come back to the line above right here 
        LDR R3, R5, #0	;  (B-cont)
        LDR R2, R1, #0	;  (A-cont)
	BRz 12
        AND R4, R2, R3	;  XOR start, Section a
        NOT R4, R4	;  (see notes)
        AND R2, R2, R4	;  Section b
        NOT R2, R2
        AND R3, R3, R4
        NOT R3, R3
        AND R2, R2, R3	;  Section c
        NOT R2, R2	;  XOR End
        STR R2, R1, #0	;  Stor the XORed R2 back in R1 add
	ADD R6, R6, #1	; Increment the char count 
	ADD R1, R1, #1	; Increment R1
        BRnzp -16
	
	;Reverse input - make sure R1 is still set as the XOR func!!
	;; R1 will be initially moved back b/c it is set to the null
	;; char. 
	LEA R5, MESREV		; R5 set to beginning
	ADD R1, R1, #-1		; R1 will continue to move back...
	LDR R2, R1, #0		; Load R2 with the char from MESSAGE
	STR R2, R5, #0		; and store in MESREV
	ADD R5, R5, #1		; Increment R5
	AND R7, R7, R6		; Test to see if R6 (count) is 0
	ADD R6, R6, #-1		; Decrement count
	BRnp -7

	;; Print the reversed message
	LEA R1, MESREV
	LDR R0, R1, #-0	
	TRAP x21
	ADD R1, R1, #1
	LDR R0, R1, #-0	
	BRnp -5

	;;Print period
	LEA R1, PERIOD
	LDR R0, R1, #-0
	TRAP x21	
		
	HALT

PROMPT	.STRINGZ "Enter message: "
PROMPT2 .STRINGZ "Encoded mesage: "
TWOSCP  .FILL xFFD2		; Two's comp of ascii period, 2E
SECRET	.FILL x0015		; Secret char
LINE	.FILL x000A		; Printable newline
LOWASCII .FILL xFFE0		; x0020 = xFFE0
HIGHASCII .FILL xFF82		; x007E = xFF82
BEEP	.FILL x0007
PERIOD	.FILL x002E
MESSAGE .BLKW 100		; Message limited to 100 chars
MESREV	.BLKW 100		; "                      "
	.END