#ifndef RAINBOW_TABLE_H
#define RAINBOW_TABLE_H


/*
Stuart's MacBook can generate ~ 562 chains of length 2000 per second

*/

#include <unordered_map>
#include <fstream>
#include <iostream>
#include <string>
#include <openssl/evp.h>
using std::unordered_map;
using std::cout;
using std::string;
using std::ifstream;
using std::ofstream;
using std::cerr;
class RainbowTable
{
	public:
		RainbowTable(string fileName);
		RainbowTable(int chainLength, string dictName);
		RainbowTable(int chainLength, int numChains);
		void outputToFile(string fileName);
		string lookup(unsigned char* hashVal);
		
	private:
		unsigned _chainLength;
		unordered_map<string,string> table;
		
		string reduce(unsigned char* hashVal, unsigned int size, int reductionNumber);
		unsigned int applyHash(string password, unsigned char* result);
		string walkChain(string currKey, unsigned char* lookupHash);
		bool equalHashes(unsigned char* hash1, unsigned char* hash2, unsigned size);
};
#endif
