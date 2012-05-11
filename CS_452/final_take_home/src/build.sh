#!/bin/sh


#File mode
POSTFIX=".c"
if [[ $# -gt 1 ]] ; then
	POSTFIX=".asm"
fi


#Link objcopy, cause something here is stupid
rm ./objcopy
ln -s `which gobjcopy` ./objcopy


#Part number
#PARTNO=atmega32
PARTNO=atmega8515

#Fuses
FUSES="-U flash:w:$1.hex:i"
#FUSES="-U hfuse:w:0x40:m -U lfuse:w:0xA4:m"


#COM port settings
#COM=/dev/ttys0
COM=/dev/tty.usbserial
#BAUD=9600
BAUD_OPTS=""

if [[ "$BAUD" != "" ]] ; then
	BAUD_OPTS="-p $BAUD"
fi


#Compile
#include dirs
#INC= -I/usr/local/CrossPack-AVR-20100115/avr-4/include
#INC=-I/usr/local/CrossPack-AVR/avr-4/include

#if is a .c file, dump an assembly file, just to read over
if [[ $POSTFIX == ".c" ]]; then
	avr-gcc $INC -Os -S -mmcu=$PARTNO "$1""$POSTFIX"
fi

#real compile
CMD="avr-gcc -Os $INC -o $1 -mmcu=$PARTNO $1$POSTFIX"
echo $CMD
$CMD

#if fail, bail
if (( $? != 0 ));
then
	echo Compilation Failed
	exit 1
fi

echo Compilation Successful


#Copy object to hex
./objcopy -v -S -O ihex $1 $1.hex


#Avr Dude to the COM port of the kit
avrdude -v -y -p $PARTNO -P $COM $BAUD_OPTS -c stk500v2 $FUSES
