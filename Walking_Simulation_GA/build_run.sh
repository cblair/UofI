#!/bin/sh

make clean; make; 
time ./test
R < graph.r --no-save
