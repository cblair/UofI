#!/usr/bin/python
import sys
from numpy import *

## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## 
# printLabeledMatrix
#    print matrix mat using the row and col label lists
#    width of column is width
def printLabeledMatrix(width, mat, rowLabel, colLabel):
    rowSize = len(rowLabel)
    colSize = len(colLabel)

    print "".rjust(width),
    for label in rowLabel :
        print label.rjust(width),
    print

    for i in range(0, colSize) :
        print colLabel[i].rjust(width),
        for j in range(0, rowSize) :
            print str(mat[i][j]).rjust(width),
        print


## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## 
# returns not only the maximum value but the location 
# of the first place that has that value.   Assumes
# list is nonempty.
def maxarg(x):
    max = x[0];
    loc = 0;
    for i in range(1, len(x)) :
        #if x[i]>=max:
        if x[i]>max:
            max = x[i]
            loc = i
    return (loc, max)


## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## 
# read a file that has a protein X protein matrix AND a gap score
# from the file with the given name
def getSubstMatrix(file):
    data = open(file, "r")

    # read the alphabet for the substitution matrix
    # make a mapping from letter to position in substList
    substList = data.readline().rstrip().split()
    substDict = dict(zip(substList, range(0, len(substList))))
 
    # read the substitution matrix
    subst = []
    for i in range(0, len(substList)):
        line_list = data.readline().rstrip().split()
        del line_list[0]         # remove the residue label in table
        line_list = map(int, line_list)
        subst.append(line_list)

    # read the score for a gap
    gap = int(data.readline())

    return (subst, gap, substList, substDict)


## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## 
def printSubstMatrix(subst, substList, gap) :
    printLabeledMatrix(2, subst, substList, substList)
    print "Gap:", gap


## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## 
# get a protein in a fasta sequence from the file with the given name.
# This assumes the first line is the '>' line as in NCBI FASTA format
def getProteinFasta(file):
    data = open(file, "r")

    # read in string
    str = data.readline().rstrip()

    if str[0]==">":
        seq = data.read()
    else:
        print "file", file, "not in FASTA format."
        quit()

    return seq.replace("\n", "")
    


## ## ## ## ## ## ## ## ## ## ## ## ## ## ## 
# 
# do a global alignment
#
def align(subst, substDict, gap, eastStr, southStr) :
    # associate a number with a direction
    (east, south, both, stop) = range(0, 4)

    # do the dynammic programming through the matrix
    # recording a score in f and a direction in dir
    # the zero row and column do not refer to any char
    # so the matrix is 1 larger in each dimension.
    # This is where we pointed out there is one more "street"
    # than the number of blocks in the "walk to school" problem.
    lene = len(eastStr)+1
    lens = len(southStr)+1
    f = zeros((lens, lene))
    dir = zeros((lens, lene))
    # loop through the matrix by rows
    for s in range(0, lens):               
        for e in range(0, lene):
            if s==0 : 
                if e==0 :
                    f[0, 0] = 0
                    dir[0][0] = stop
                else: 
                    f[0][e] = f[0][e-1] + gap
                    dir[0][e] = east
            else:
                if e==0 :
                    f[s][0] = f[s-1][0] + gap
                    dir[s][0] = south
                else: 
                    (whichway, f[s][e]) = maxarg([
                            f[s-1][e-1] + subst[substDict[southStr[s-1]]][substDict[ eastStr[e-1]]],  # for both
                            f[s][e-1] + gap,               # for east
                            f[s-1][e] + gap,               # for south
                            ])
                    #dir[s][e] = [east, south, both][whichway]   # which was max in above gives which dir to go
                    dir[s][e] = [both,east, south][whichway]   # which was max in above gives which dir to go

    # print internal state                    
    print
    print "The scores:"
    printLabeledMatrix(5, f, " " + eastStr, " " + southStr)
    print
    print "The Directions:"
    printLabeledMatrix(5, dir, " " + eastStr, " " + southStr)
                        
    # walk back through the matrix to recover the match
    (s, e) = (lens-1, lene-1)
    newEastStr = newSouthStr = ""
    while dir[s][e]!=stop :
        # going east matchs an east char and inserts a gap in the south string
        if dir[s][e]==east: 
            newEastStr = eastStr[e-1] + newEastStr
            newSouthStr = "-" + newSouthStr
            e -= 1
            # going south matchs an south char and inserts a gap in the east string
        elif dir[s][e]==south: 
            newEastStr = "-" + newEastStr
            newSouthStr = southStr[s-1] + newSouthStr
            s -= 1
            # matches both east and south
        elif dir[s][e]==both: 
            newEastStr = eastStr[e-1] + newEastStr
            newSouthStr = southStr[s-1] + newSouthStr
            e -= 1
            s -= 1
        else:
            print "should not get here ", (e, s), " with dir ", dir[e][s]
            # print the alignment
    return (f[lens-1, lene-1], newEastStr, newSouthStr)


## ## ## ## ## ## ## ## ## ## ## ## ## ## ## 
# Begin main 
#
#  build a main procedure so the variables here are not globals.
#
def main():
    print "Do a Global Alignment"
    print

    # get subst matrix and gap
    (subst, gap, substList, substDict) = getSubstMatrix(sys.argv[1])
    printSubstMatrix(subst, substList, gap)
    print

    # get strings
    eastStr = getProteinFasta(sys.argv[2])
    southStr = getProteinFasta(sys.argv[3])
    print "East: ", eastStr
    print "South:", southStr
    print

    # do alignment
    (score, newEastStr, newSouthStr) = align(subst, substDict, gap, eastStr, southStr)
    print "Score:", score
    print
    print newEastStr
    print newSouthStr


## ## ## ## ## ## ## ## ## ## ## ## ## ## ## 
# call main
main()

