#!/usr/bin/env python2.7

l = [1,3,2,2,7,9,8]
l2 = []

print "Before: ",
print l

for i in range(0, len(l)):
	#find min_val
	min_val = l[0]
	for j in range(0, len(l)):
		if l[j] < min_val:
			min_val = l[j]
	l.remove(min_val)
	l2.append(min_val)

print "After:",
print l2
