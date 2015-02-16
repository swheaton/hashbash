import hashlib
from sys import argv

with open(argv[1],'r') as file:
	for line in file:
		hash_object = hashlib.sha1(line[0:len(line)])
		hex_dig = hash_object.hexdigest()
		print(hex_dig)
