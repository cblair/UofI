#!/usr/bin/env python2.7

#really bad python, but maps to pseudocode

def merge(A, p, q, r):
	n1 = q - p + 1
	n2 = r - q
	#create arrays L and R
	L = [] # L = [1 ... n1 + 1]
	R = [] # R = [1 ... n2 + 1]
	for i in range(0, n1 - 1): # i <- 1 to n1
		L.append(A[p + i]) #L[i] = A[p + i - 1]	
	for j in range(0, n2): # j <- 1 to n2
		R.append(A[q + j]) #R[j] = A[q + j]
	L.append(float("inf")) #L[n1 + 1] = infinity
	R.append(float("inf")) #R[n2 + 1] = infinity
	i = 0 #i = 1
	j = 0 #j = 1
	for k in range(p, r):
		if L[i] <= R[j]:
			A[k] = L[i]
			i = i + 1
		else:
			A[k] = R[j]
			j = j + 1
	return A

def merge_without_sentinels(A, p, q, r):
	n1 = q - p + 1
	n2 = r - q
	#create arrays L and R
	L = [] # L = [1 ... n1 + 1]
	R = [] # R = [1 ... n2 + 1]
	for i in range(0, n1 - 1): # i <- 1 to n1
		L.append(A[p + i]) #L[i] = A[p + i - 1]	
	for j in range(0, n2): # j <- 1 to n2
		R.append(A[q + j]) #R[j] = A[q + j]
	i = 0 #i = 1
	j = 0 #j = 1
	for k in range(p, r):
		#if R is empty, copy the rest of L and break
		if j == len(R):
			for i2 in range(i, len(L)):
				A[k] = L[i2]
				i2 = i2 + 1
				k = k + 1
			return A
		#if L is empty, copy the rest of R and break
		if i == len(L):
			for j2 in range(j, len(L)):
				A[k] = L[j2]
				j2 = j2 + 1
				k = k + 1
			return A
		if L[i] <= R[j]:
			A[k] = L[i]
			i = i + 1
		else:
			A[k] = R[j]
			j = j + 1
	return A

"""
def merge_sort(A, p, r):
	'''WIP - doesn't work yet'''
	if p < r:
		q = (p + r) / 2 #TODO: floor / ceiling?
		#q = q - 1 #TODO: take out for pseudocode
		L = merge_sort(A, p, q) #merge_sort(A, p, q)
		R = merge_sort(A, q + 1, r) #merge_sort(A, q + 1, r)
		#TODO: take out next line for pseudocode
		A = L.extend(R)
		print L
		print R
		print A
		print
		return merge(A, p, q, r) #merge(A, p, q, r)
	print A
	return A #TODO: take out for pseudocode
"""


#main

A = [2,4,5,7,1,2,3,6]

print "Merge"
print " Before: ",
print A
#A = merge(A, 0,4,8)
A = merge_without_sentinels(A, 0,4,8)
print " After: ",
print A

'''
A = [2,4,5,7,1,2,3,6]
print "Merge Sort"
print " Before: ",
print A
A = merge_sort(A, 0, 8)
print " After: ",
print A
'''
