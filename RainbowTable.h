#ifndef RAINBOW_TABLE_H
#define RAINBOW_TABLE_H

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
class RainbowTable
{
	public:
		RainbowTable(string fileName);
		RainbowTable(int chainLength, string dictName);
		RainbowTable(int chainLength, string dictName, bool variants); 	//TODO: Write a version where we end up with ~1000 variants on each dictionary entry
		RainbowTable(int numChains, int chainLength);			
		void outputToFile(string fileName);
		string lookup(char[]);
		
	private:
		string reduce(unsigned char* hashVal, unsigned int size, int reductionNumber);
		unsigned int applyHash(string password, unsigned char* result);
		unordered_map<string,string> table;
};
#endif
