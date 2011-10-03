#!/bin/bash

TOTALFILES=`ls -talh ts-flat | wc -l`
PARSEDFILES=0

for file in `ls ts-flat`; do
	./ec ts-flat/$file &> temp.dat ; grep ERROR temp.dat &> /dev/null
	if [ $? != 1 ]; then
		echo "test_all.sh failed on file '$file'"
	else
		echo "test_all.sh succeeded on file '$file'"
		PARSEDFILES=`expr $PARSEDFILES + 1`
	fi
done

rm temp.dat
echo "Total files:	$TOTALFILES"
echo "Parsed files:	$PARSEDFILES"
