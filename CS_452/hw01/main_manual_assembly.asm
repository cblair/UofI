; Test 1: Learn to know parts of the board hardware output to the LEDs.
; What to learn here:
; - to access a output-port (Port B and the LEDs)
; - the different parts of a typical assembler program
; General conventions used here:
; - Words in uppercase letters are command words of the assembler language
;   or predefined ports of the processor.
; - I define Words in lowercaseletters.
; Assembler directive: define the target chip type
;
; Usually you do not have to include the def.inc-file. But my installation
; does not find the definition files in the proper subdirectory. So I include
; it manually by adding these lines to my source code. Failure to include
; these definitions would result in a number of error messages. Simply
; copy the def.inc-file to the same directory where your source code resides
; or add your whole path to the include-statement below.
.NOLIST
.INCLUDE "8515def.inc"
.LIST
; The directives NOLIST and LIST switch off the listing of the INCLUDE-file
; (the listing will be found in the file TEST1.LST).
; Define register
;
; This register is defined for the purpose of interim storage of values.
; The .DEF statement renames one of the 32 8-bit registers to a easier
; to remember name (here: mp). It is easier to use than its real name
; R16. Renaming these register also has the advantage that whenever you
; want to change such a location you'll just have to change the .DEF
; statement. Otherwise you would have to go through all commands re-
; ferencing the register R16 and change all these lines.
.DEF	mp = R16
; Restart
;
; Up till now not one byte of code was created. Here we start with the
; first code, we define the program start adress. Later we will see, that
; this jump command at adress 0 is useful. Here the first byte of code is
; created.
; Whenever the AVR is restarted it starts its program execution at adress
; 0000. Such a restart may be activated by a power-on, by a hardware
; reset on the respective pin or by a watchdog-timer reaching its zero
; count. (The watchdog timer is not used here.)
; In all these cases a jump to a program called "main" will be executed.
; RJMP means "Relative Jump". In a relative jump a relative distance
; is added to the current execution adress and the program is executed
; at this new adress. We don't have to care about this distance by now
; because the assembler calculates this relative distance from the information
; in our source code and adds it automatically - as long as the maximum
; permissible distance of 2 kB for- or backwards is not exceeded, otherwise
; the assembler stops with an error message.
	rjmp	main
; Here we start with our main program code. First we have to define a
; label, because the assembler has to know that this is the target of a
; jump. A label is a freely defined name, followed by ":". For a better
; overview labels start in column 1 of a line, all commands start with a
; blank or a tab character. Behind the label there could be a command,
; but this is not used here.
main:
; First we need to define port B as output, because the LEDs are connected
; to these port pins. This is done by writing eight ones to the data direction
; register of port B. The data direction register of port B is named DDRB,
; we don't have to care about its real adress, as the .def-file holds this
; information already. We do that in two steps. First we load binary 1111.1111
; to a register:
	ldi	mp,0b11111111
; The command LDI (LoaD Immediate) loads an 8-bit value into the register
; mp. This command is only valid for the registers R16 to R31. That is why we
; defined mp that way at the beginning of this source code file. Commands
; with two parameters are generally defined such that the first parameter
; is the target (here: register mp), where the result of the operation goes to.
; The second parameter is the value or location where the result comes from.
; After execution of this command the register mp has the value 1111.1111
; binary, FF hex or 255 decimal.
; The 0b... of the beginning of the number is always a binary, 0x... is a hex
; notation. The leading zero is the signal for the assembler to expect a number,
; not a register or something else. Numbers without 0b... or 0x.. are decimal
; by default. (LDI mp,255 would have the same result.)
; This value has to be brought to the data direction register of port B now
; to make all port pins to outputs. A 1 in the data direction register brings
; the pin to switch on its output drivers, a zero makes it an input.
	out	DDRB,mp
; The command OUT writes register values (here: mp or R16) to a port
; (here DDRB). DDRB is defined in the file "8515def.inc", which is already
; read by the assembler when the .DEVICE-command or the .INCLUDE-
; command above has been processed. So we don't need to care here
; for the real adress of this port.
; The following part of the program now writes zeros and ones to the port.
; The LEDs are switched on and off with a high frequency. Because this
; part of the program is repeated over and over again, I called it loop.
; The label loop is jumped to whenver the processing of writing ones and
; zeros is ended.
loop:
	ldi	mp,0x00
	out	PORTB,mp
; LDI load eight zero bits to the universial register mp. OUT copies these
; zeros to the port B. This time they are written to the port's output register
; named PORTB.
; The zeros set the LEDs on, because they are connected to the supply
; voltage over resistors of 1 k (0=on, 1=off).
	ldi	mp,0xFF
	out	PORTB,mp
; After that eight ones are loaded into the register mp and copied to
; the same data port. That switches the LEDs off.
	rjmp	loop
; With this relative jump we jump back to the start of the loop and the
; loop is repeated as long as power is supplied.
; At 4 MHz xtal frequency every LDI- and OUT-command execution requires
; 250 ns, the RJMP need 2 cycles and 500 ns. Every 1,500 ns the loop is
; executed once, so with these values the LEDs are switched at 667 kHz.
;
; After assembling this sourcefile the program will have eight words.
; The file TEST1.LST holds the result of the assembling process in form
; of a listing.
; This was the first lecture, wait for more ...
