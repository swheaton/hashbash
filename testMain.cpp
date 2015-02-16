#include "RainbowTable.h"
#include <openssl/evp.h>
#include <iostream>
#include<string.h>
using std::cout;
using std::cin;
//typedef hash char[20];

int main(int argc, char** argv)
{
	int process = atoi(argv[1]);

	

	for(int i=2; i<argc; i++)
	{
		RainbowTable rt(string(argv[i]));

		cout << "Done with table " << (i-2) << "\n";

		char filename[100000];
		sprintf(filename, "/media/benvds/522665A726658CB3/RAINBOWTABLE/dictCombo/proc%dtable%d.rt", process, i-2);
	}


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
