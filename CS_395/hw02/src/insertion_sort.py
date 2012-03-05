#!/usr/bin/env python2.7

def insertion_sort_rec(A):
	for j in range(1, len(A)): #for j <- 2 to length(A)
		key = A[j]
		#Insert A[j] into the sorted sequence A[1...j-1]
		i = j - 1
		while i > -1 and A[i] > key: #while i > 0 and A[i] > key
			A[i+1] = A[i]
			i = i - 1
		A[i+1] = key
	return A

A = [2,4,5,7,1,2,3,6]
print "Insertion Sort"
print " Before: ",
print A
A = insertion_sort_rec(A)
print " After: ",
print A
