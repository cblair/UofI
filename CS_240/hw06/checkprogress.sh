#!/bin/bash

i="0"

while [ $i -lt 10000 ]
do
    wc data/*
    sleep 3
    echo
    echo
    echo --------------------------------------------------------------
    i=$[$i+1]
done
