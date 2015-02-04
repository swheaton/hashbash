#include "RainbowTable.h"
#define numChars 8
#define keyspacesize 217180147158

string RainbowTable::reduce(unsigned char* hashVal, unsigned int size, int reductionNumber)
{
	uint64_t n;
	if(size >= 8)
	{
		n = *((uint64_t*) hashVal);
	}
	else 
	{
		n = 0;
	}

	n += reductionNumber;
	n %= keyspacesize;
	string outString(1,(char) ((n % 26) + 'a'));
	n /= 26;
	while (n > 0)
	{
		n--;
		outString += (char)((n % 26) + 'a');
		n /= 26;
	}
	return outString;
}

unsigned int RainbowTable::applyHash(string password, unsigned char* result)
{
	EVP_MD_CTX *mdctx = EVP_MD_CTX_create();
	const EVP_MD *md = EVP_md5();
	unsigned int size;

	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, password.c_str(), password.length());
	EVP_DigestFinal(mdctx, result, &size);

	for(int i=0; i<size; i++)
	{
		printf("%2x",result[i]);
	}
	printf(" ");
	return size;
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
		unsigned char* hashVal = new unsigned char[EVP_MAX_MD_SIZE];
		for(int i=0; i<chainLength; i++)
		{
			unsigned int size = applyHash(currKey, hashVal);
			currKey=reduce(hashVal, size, i);
			cout << currKey << " " ;
		}
		table[currKey]=startKey;
		cout << "\n";
	}
}

/*string lookup(hash)
{
	
}*/
