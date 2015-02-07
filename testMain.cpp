#include "RainbowTable.h"
#include <openssl/evp.h>
#include <iostream>
using std::cout;
using std::cin;
//typedef hash char[20];

int main()
{
	RainbowTable rt(2000,"/usr/share/dict/web2");
	cout << "Done with table\n";
	
	string password = "";
	/*while (cin >> password)
	{
	unsigned char* result = new unsigned char[EVP_MAX_MD_SIZE];
	EVP_MD_CTX *mdctx = EVP_MD_CTX_create();
	const EVP_MD *md = EVP_md5();
	unsigned int size;

	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, password.c_str(), password.length());
	EVP_DigestFinal(mdctx, result, &size);
	
	
	string pass = rt.lookup(result);
	//cout << "\n\n";
	if(pass.length() == 0)
		cout << "NO PASS FOUND" << std::endl;
	cout << pass << std::endl;
	delete result;
	}*/
	
	return 0;
}
