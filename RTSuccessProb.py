#!/usr/bin/python
from sys import argv
from math import exp

def findMTerms(m,t,N):
	mTermList = [m]
	for i in xrange(1,t):
		#print N*(1-exp(-1 * mTermList[i-1] / N))
		mTermList.append(N * (1-exp(-1*mTermList[i-1]/N)))
	return mTermList
def productSumTerm(m,t,N):
	out = 1.0
	mTerms = findMTerms(m,t,N)
	#print mTerms
	for i in xrange(0,t):
		#print 1-mTerms[i]/N
		out *= (1 - mTerms[i]/N)
	return out
	
def findKeySpaceSize(charSetSize, maxPasswordLength):
	size = 0;
	for i in xrange(1,maxPasswordLength+1):
		size += charSetSize**i
	return size
	
def findSuccessProb():
	numChains = int(argv[1])
	chainLength = int(argv[2])
	charSetSize = int(argv[3])
	maxPasswordLength = int(argv[4])
	
	keySpaceSize = findKeySpaceSize(charSetSize, maxPasswordLength)
	
	return 1-productSumTerm(numChains, chainLength, keySpaceSize)
	
#print findKeySpaceSize(26,8)
#exit()

if(len(argv) != 5):
	print "Usage: python RTSuccessProb.py <numChains> <chainLength> <charSetSize> <maxPasswordLength"
	exit()
print "Probability of success for a rainbow table of these dimensions is:"
print findSuccessProb()