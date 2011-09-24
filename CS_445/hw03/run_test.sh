#!/bin/bash

make clean
make &> temp.dat ; head temp.dat
./ec DataRange.as
#./ec DataRange.as > temp.dat
#diff -y temp.dat test_data/test.out
#rm temp.dat
