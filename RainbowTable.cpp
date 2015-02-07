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

void RainbowTable::printHash(unsigned char* hash, unsigned int size)
{
	for(int i=0; i<size; i++)
	{
		printf("%02x", hash[i]);
	}
	printf(" ");
}

unsigned int RainbowTable::applyHash(string password, unsigned char* result)
{
	const EVP_MD *md = EVP_md5();
	unsigned int size;

	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, password.c_str(), password.length());
	EVP_DigestFinal(mdctx, result, &size);

	return size;
}

RainbowTable::RainbowTable(int chainLength, string dictName)
{
	_chainLength = chainLength;
	ifstream fin(dictName);
	mdctx = EVP_MD_CTX_create();
	//check errors
	
	string startKey;
	int key = 1;
	while(fin >> startKey)
	{
		string currKey = startKey;
		unsigned char* hashVal = new unsigned char[EVP_MAX_MD_SIZE];
		for(int i=0; i<chainLength-1; i++)
		{
			unsigned int size = applyHash(currKey, hashVal);
			currKey=reduce(hashVal, size, i);
		}
		delete[] hashVal;
		table[currKey]=startKey;

		if(key % 1000 == 0)
			cout << "processing key " << key << "\n";

		key++;
	}
}
		
RainbowTable::RainbowTable(string fileName)
{
	ifstream fin(fileName);
	string head, tail;
	mdctx = EVP_MD_CTX_create();

	fin >> _chainLength;
	while(fin >> head >> tail)
	{
		table[tail] = head;
	}

	fin.close();
	return;
}
		
void RainbowTable::outputToFile(string fileName)
{
	ofstream fout(fileName);
	fout << _chainLength << "\n";
	for(auto iter = table.begin(); iter != table.end(); ++iter)
	{
		fout << iter->second << " " << iter->first << "\n";
	}
	fout.close();
	return;
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
	for (int startReduceStep = _chainLength - 2; 
		startReduceStep >= 0; startReduceStep--)
	{
		//First reduction
		unsigned char* tmpHash = new unsigned char[EVP_MAX_MD_SIZE];
		string currReduction = reduce(hashVal, 128, startReduceStep);
		
		//Middle hash-reductions
		for (int midReduceStep = startReduceStep + 1;
			midReduceStep < _chainLength - 1; midReduceStep++)
		{
			unsigned size = applyHash(currReduction, tmpHash);
			currReduction = reduce(tmpHash, size, midReduceStep);
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

RainbowTable::~RainbowTable()
{
	EVP_MD_CTX_destroy(mdctx);
}
