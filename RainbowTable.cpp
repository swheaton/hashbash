#include "RainbowTable.h"
#define numChars 8

string RainbowTable::reduce(unsigned char* hashVal, int reductionNumber)
{
	uint64_t n = *((uint64_t*) hashVal);
	n += reductionNumber;
	string outString="";
	for(int i=0; i<numChars; i++)
	{
		outString += (char)((n % 26) + 'a');
		n /= 26;
	}
	return outString;
}

unsigned char* RainbowTable::applyHash(string password)
{
	unsigned char* ret = new unsigned char[20];
	
	ret=SHA1((const unsigned char*) password.c_str(), password.length(), NULL);
	for(int i=0; i<20; i++)
	{
		printf("%2x",ret[i]);
	}
	printf(" ");
	return ret;
}

RainbowTable::RainbowTable(int chainLength, string dictName)
{
	ifstream fin(dictName);
	//check errors
	
	string startKey;
	while(fin >> startKey)
	{
		string currKey = startKey;
		cout << startKey << " ";
		for(int i=0; i<chainLength; i++)
		{
			unsigned char* hashVal = applyHash(currKey);
			currKey=reduce(hashVal, i);
			delete[] hashVal;
			cout << currKey << " " ;
		}
		table[currKey]=startKey;
		cout << "\n";
	}
}

/*string lookup(hash)
{
	
}*/