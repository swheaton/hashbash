def findKeySpaceSize(charSetSize, maxPasswordLength):
	size = 0;
	for i in xrange(1,maxPasswordLength+1):
		size += charSetSize**i
	return size
	
numChars = 2
strSet = set([])
for n in xrange(0, findKeySpaceSize(26,numChars)):
	outString = chr((((n) % 26) + ord('a')))
	n/=26
	while n > 0:
		outString += chr((((n-1) % 26) + ord('a')))
		n = (n-1)/26
	#print outString
	#strSet.add(outString)

print findKeySpaceSize(26,8)
blah="""for k in xrange(ord('a'),ord('z')+1):
	strOut = ""+chr(k)
	if strOut not in strSet:
		print "FAIL " + strOut
	else:
		strSet.remove(strOut)
	
for k in xrange(ord('a'),ord('z')+1):
	for i in xrange(ord('a'),ord('z')+1):
		strOut = ""+chr(k)+chr(i)
		if strOut not in strSet:
			print "FAIL " + strOut
		else:
			strSet.remove(strOut)

for k in xrange(ord('a'),ord('z')+1):
	for i in xrange(ord('a'),ord('z')+1):
		for j in xrange(ord('a'),ord('z')+1):
			strOut = ""+chr(k)+chr(i)+chr(j)
			if strOut not in strSet:
				print "FAIL " + strOut
			else:
				strSet.remove(strOut)
				
for k in xrange(ord('a'),ord('z')+1):
	for i in xrange(ord('a'),ord('z')+1):
		for j in xrange(ord('a'),ord('z')+1):
			for m in xrange(ord('a'),ord('z')+1):
				strOut = ""+chr(k)+chr(i)+chr(j) + chr(m)
				if strOut not in strSet:
					print "FAIL " + strOut
				else:
					strSet.remove(strOut)
	
if len(strSet) > 0:
	print "FAIL SIZE " + str(len(strSet))"""

