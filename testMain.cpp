#include "RainbowTable.h"
#include <openssl/evp.h>
#include <iostream>
//typedef hash char[20];

int main()
{
	RainbowTable rt(3,"testDictionary.txt");
	cout << "\n\n";
	
	string password = "a";
	unsigned char* result = new unsigned char[EVP_MAX_MD_SIZE];
	EVP_MD_CTX *mdctx = EVP_MD_CTX_create();
	const EVP_MD *md = EVP_md5();
	unsigned int size;

	cout << "yes??" << std::endl;
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, password.c_str(), password.length());
	EVP_DigestFinal(mdctx, result, &size);
	
	
	string pass = rt.lookup(result);
	cout << "\n\n";
	if(pass.length() == 0)
		cout << "NO PASS FOUND" << std::endl;
	cout << pass << std::endl;
	delete result;
	
	return 0;
}