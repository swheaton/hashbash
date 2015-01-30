#include <unordered_map>
#include <fstream>
#include <iostream>
#include <string>
#include <openssl/sha.h>
using std::unordered_map;
using std::cout;
using std::string;
using std::ifstream;
class RainbowTable
{
	public:
		RainbowTable(string fileName);
		RainbowTable(int chainLength, string dictName);
		RainbowTable(int numChains, int chainLength);
		void outputToFile(string fileName);
		string lookup(char[]);
		
	private:
		string reduce(unsigned char* hashVal, int reductionNumber);
		unsigned char* applyHash(string password);
		unordered_map<string,string> table;
};