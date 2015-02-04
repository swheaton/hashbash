#include "RainbowTable.h"
#include <openssl/sha.h>
//typedef hash char[20];

int main()
{
	RainbowTable rt(2,"testDictionary.txt");
	rt.outputToFile("testDictionary.dict");
	RainbowTable second("testDictionary.dict");
	return 0;
}
