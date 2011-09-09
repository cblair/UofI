#!/bin/bash

make clean
make
./ec DataRange.as > temp.dat
diff -y temp.dat test_data/test.out
rm temp.dat
