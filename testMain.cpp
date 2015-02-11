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
	int tables = atoi(argv[2]);
	int chainsPerTable = atoi(argv[3]);
	int reductions = atoi(argv[4]);

	cout << process << " " << tables << " " << chainsPerTable << " " << reductions << "\n";

	for(int i=0; i<tables; i++)
	{
		RainbowTable rt(reductions,chainsPerTable);

		cout << "Done with table\n";

		char filename[100000];
		sprintf(filename, "/media/benvds/522665A726658CB3/RAINBOWTABLE/proc%dtable%d.rt", process, i);
		rt.outputToFile(filename);
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
