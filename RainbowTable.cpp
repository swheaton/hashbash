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
		printf("%02x",result[i]);
	}
	printf(" ");
	return size;
}

RainbowTable::RainbowTable(int chainLength, string dictName)
{
	_chainLength = chainLength;
	ifstream fin(dictName);
	//check errors
	
	string startKey;
	while(fin >> startKey)
	{
		string currKey = startKey;
		cout << startKey << " ";
		unsigned char* hashVal = new unsigned char[EVP_MAX_MD_SIZE];
		for(int i=0; i<chainLength-1; i++)
		{
			unsigned int size = applyHash(currKey, hashVal);
			currKey=reduce(hashVal, size, i);
			cout << currKey << " " ;
		}
		delete hashVal;
		table[currKey]=startKey;
		cout << "\n";
	}
}

bool RainbowTable::equalHashes(unsigned char* hash1, unsigned char* hash2, unsigned size)
{
	for(int k=0; k<size; k++)
	{
		if(hash1[k] != hash2[k])
			return false;
	}
	return true;
}

string RainbowTable::walkChain(string currKey, unsigned char* lookupHash)
{
	unsigned char* tmpHash = new unsigned char[EVP_MAX_MD_SIZE];
	
	for(int reduceStep = 0; reduceStep < _chainLength - 1; reduceStep++)
	{
		unsigned size = applyHash(currKey, tmpHash);
		if(equalHashes(lookupHash, tmpHash, size))
		{
			return currKey;
		}
		currKey = reduce(tmpHash, size, reduceStep);
	}
	
}

string RainbowTable::lookup(unsigned char* hashVal)
{
	cout << "lookup time!" << std::endl;
	for (int startReduceStep = _chainLength - 2; 
		startReduceStep >= 0; startReduceStep--)
	{
		//First reduction
		unsigned char* tmpHash = new unsigned char[EVP_MAX_MD_SIZE];
		string currReduction = reduce(hashVal, 128, startReduceStep);
		cout << "reduce " << startReduceStep << " " << currReduction << std::endl;
		
		//Middle hash-reductions
		for (int midReduceStep = startReduceStep + 1;
			midReduceStep < _chainLength - 1; midReduceStep++)
		{
			unsigned size = applyHash(currReduction, tmpHash);
			currReduction = reduce(tmpHash, size, midReduceStep);
			cout << "reduce " << midReduceStep << " " << currReduction << std::endl;
		}
		delete tmpHash;
		//Try to find the final reduction in the table
		auto foundEndpoint = table.find(currReduction);
		
		//We found it! Time to walk that chain and look for the password
		string foundPass = "";
		if(foundEndpoint != table.end())
		{
			foundPass = walkChain((*foundEndpoint).second, hashVal);
			if(foundPass.length() > 0)
				return foundPass;
		}
	}
	return "";
}