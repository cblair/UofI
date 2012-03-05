#!/usr/bin/env python2.7

import time

def find_max_crossing_subarray(A, low, mid, high):
	max_left = 0
	max_right = 0
	#Find a max subarray of the form A[i..mid]
	left_sum = -float("inf")
	sum = 0
	for i in range(mid, low - 1, -1): #for i = mid downto low
		sum = sum + A[i]
		if sum > left_sum:
			left_sum = sum
			max_left = i
	#Find a max subarray of the for A[mid + 1 .. high]
	right_sum = -float("inf")
	sum = 0
	for j in range(mid+1, high):
		sum = sum + A[j]
		if sum > right_sum:
			right_sum = sum
			max_right = j
	return (max_left, max_right, left_sum + right_sum)


def find_max_subarray(A, low, high):
	#best case, one element
	if high == low:
		return (low, high, A[low])
	else:
		mid = (low + high) / 2
		(left_low, left_high, left_sum) = \
			find_max_subarray(A,low, mid)
		(right_low, right_high, right_sum) = \
			find_max_subarray(A, mid + 1, high)
		(cross_low, cross_high, cross_sum) = \
			find_max_crossing_subarray(A, low, mid, high)
		if left_sum >= right_sum and \
			left_sum >= cross_sum:
			return (left_low, left_high, left_sum)
		elif right_sum >= left_sum and \
			right_sum >= cross_sum:
			return (right_low, right_high, right_sum)
		else:
			return (cross_low, cross_high, cross_sum)

#main

A = [-1 ,3, 2, 13, -4, 7, -12, 4, -3 , 6, -5]
#A = [-6 ,-3, -2, -5, -4, -7, -12, -4, -3 , -6, -5]
#A = [-9, -5, -4, -7, -2, -4 ,-5]
#A = [-22, -43, -21, -67, -99, -10]
low = 0
high = len(A)
mid = (low + high) / 2

# test cross search
'''
print "Results: ",
print A
print find_max_crossing_subarray(A, low, mid, high)
'''

# test all
print "Results: ",
print A
stime = time.time()
print find_max_subarray(A, low, high-1)
etime = time.time()
ttime = etime - stime
print " took %s seconds" % ttime
