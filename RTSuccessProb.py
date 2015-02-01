#!/usr/bin/python
from sys import argv
from math import exp
from decimal import Decimal

def findMTerms(m,t,N):
	mTermList = [m]
	for i in xrange(1,t):
		mTermList.append(N * (1-exp(-1*mTermList[i-1]/N)))
	return mTermList
def productSumTerm(m,t,N):
	out = 1.0
	mTerms = findMTerms(m,t,N)
	for i in xrange(0,t):
		out *= (1 - mTerms[i]/N)
	return out
	
def findKeySpaceSize():
	charSetSize = int(argv[3])
	maxPasswordLength = int(argv[4])
	size = 0.0;
	for i in xrange(1,maxPasswordLength+1):
		size += charSetSize**i
	return size
	
def findSuccessProb(keySpaceSize):
	numChains = int(argv[1])
	chainLength = int(argv[2])
	
	return 1-productSumTerm(numChains, chainLength, keySpaceSize)

if(len(argv) != 5):
	print "Usage: python RTSuccessProb.py <numChains> <chainLength> <charSetSize> <maxPasswordLength"
	exit()
print "Keyspace size is: "
keySpace=findKeySpaceSize()
print int(keySpace)
print ""
print "Probability of success for a rainbow table of these dimensions is:"
print findSuccessProb(keySpace)