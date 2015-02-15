#ifndef RAINBOW_TABLE_H
#define RAINBOW_TABLE_H


/*
Stuart's MacBook can generate ~ 712 chains of length 2000 per second

*/

#include <unordered_map>
#include <fstream>
#include <iostream>
#include <string>
#include <random>
#include <openssl/evp.h>
using std::unordered_map;
using std::cout;
using std::string;
using std::ifstream;
using std::ofstream;
using std::cerr;
using std::mt19937_64;
class RainbowTable
{
	public:
		RainbowTable(string fileName);
		RainbowTable(int chainLength, string dictName);
		RainbowTable(int chainLength, string dictName, bool variants); 	//TODO: Write a version where we end up with ~1000 variants on each dictionary entry
		RainbowTable(int chainLength, int numChains);
		void produceTable(int chainLength, int numChains, string outputFile);
		void outputToFile(string fileName);
		string lookup(unsigned char* hashVal);
		~RainbowTable();
		
	private:
		unsigned _chainLength;
		EVP_MD_CTX *mdctx;
		unordered_map<string,string> table;
		const string FirstLetter = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		const string Domain = "abcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()";
		
		string numberToKey(uint64_t n);
		string reduce(unsigned char* hashVal, unsigned int size, int reductionNumber);
		void printHash(unsigned char* hash, unsigned int size);
		unsigned int applyHash(string &password, unsigned char* result);
		string walkChain(string currKey, unsigned char* lookupHash);
		bool equalHashes(unsigned char* hash1, unsigned char* hash2, unsigned size);
};
#endif
