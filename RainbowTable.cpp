#include "RainbowTable.h"
#define keyspacesize 503611515004

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
	return numberToKey(n);
}

string RainbowTable::numberToKey(uint64_t n)
{
	n %= keyspacesize;
	string outString(1, FirstLetter[n % 52]);
	int previous = 52;
	while (n >= previous)
	{
		n /= previous;
		n--;
		outString += Domain[n % 46];
		previous = 46;
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
	const EVP_MD *md = EVP_sha1();
	unsigned int size;

	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, password.c_str(), password.length());
	EVP_DigestFinal(mdctx, result, &size);

	return size;
}

RainbowTable::RainbowTable(int chainLength, int numChains)
{
	_chainLength = chainLength;
	mdctx = EVP_MD_CTX_create();
	std::random_device rd;
	mt19937_64 randomNumber(rd());

	int key = 1;
	for(int i=0; i<numChains; i++)
	{
		string startKey = numberToKey(randomNumber());
		string currKey = startKey;
		unsigned char* hashVal = new unsigned char[EVP_MAX_MD_SIZE];
		for(int i=0; i<chainLength-1; i++)
		{
			unsigned int size = applyHash(currKey, hashVal);
			currKey=reduce(hashVal, size, i);
		}
		delete[] hashVal;
		table[currKey]=startKey;

		if(key % 10000 == 0)
			cout << "processing key " << key << "\n";

		key++;
	}
}

RainbowTable::RainbowTable(int chainLength, string dictName)
{
	_chainLength = chainLength;
	ifstream fin(dictName);
	mdctx = EVP_MD_CTX_create();
	if(!fin)
	{
		cerr << "Couldn't open dictionary file\n";
		exit(1);
	}
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

		if(key % 10000 == 0)
			cout << "processing key " << key << "\n";

		key++;
	}

}
		
RainbowTable::RainbowTable(string fileName)
{
	ifstream fin(fileName);
	if(!fin)
	{
		cout << "Couldn't open rainbow table file\n";
		exit(1);
	}
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

void RainbowTable::produceTable(int chainLength, int numChains, string outputFile)
{	
	ofstream fout(outputFile);
	fout << chainLength << "\n";
	std::random_device rd;
	mt19937_64 randomNumber(rd());

	int key = 1;
	for(int i=0; i<numChains; i++)
	{
		string startKey = numberToKey(randomNumber());
		string currKey = startKey;
		unsigned char* hashVal = new unsigned char[EVP_MAX_MD_SIZE];
		for(int i=0; i<chainLength-1; i++)
		{
			unsigned int size = applyHash(currKey, hashVal);
			currKey=reduce(hashVal, size, i);
		}
		delete[] hashVal;
		table[currKey]=startKey;
		fout << startKey << " " << currKey << "\n";

		if(key % 10000 == 0)
			cout << "processing key " << key << "\n";

		key++;
	}
	fout.close();
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
