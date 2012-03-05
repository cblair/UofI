#!/usr/bin/env python2.7

import time

def find_max_subarray_naive(A):
	sum = 0
	max_sum = 0
	low = 0
	high = 0
	for j in range(0, len(A)):
        	sum = 0
		for i in range(j, len(A)):
            		sum += A[i]
            		if sum > max_sum:
                		max_sum = sum
		                low = j
		                high = i
	return (low, high, max_sum)


#main

A = [-1 ,3, 2, 13, -4, 7, -12, 4, -3 , 6, -5]
#A = [-6 ,-3, -2, -5, -4, -7, -12, -4, -3 , -6, -5]
#A = [-9, -5, -4, -7, -2, -4 ,-5]
#A = [-22, -43, -21, -67, -99, -10]

# test all
print "Results: ",
print A
stime = time.time()
print find_max_subarray_naive(A)
etime = time.time()
ttime = etime - stime
print " took %s seconds" % ttime
