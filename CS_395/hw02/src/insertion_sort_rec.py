#!/usr/bin/env python2.7

def insertion_sort_rec(A):
	if len(A) == 1:
		return A
	j = len(A) - 1 # j = length(A)
	key = A[j]
	A2 = A[0:j]
	A2 = insertion_sort_rec(A2)
	i = len(A2) - 1 # i = length(A2)
	while i >= 0 and A2[i] > key:
		i = i - 1
	A2.insert(i + 1, key) # A[i + 1] = key
	return A2

A = [2,4,5,7,1,2,3,6]
print "Insertion Sort"
print " Before: ",
print A
A = insertion_sort_rec(A)
print " After: ",
print A
