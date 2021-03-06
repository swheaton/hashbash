#include "RainbowTable.h"
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <list>
#include <dirent.h>
#include <string.h>
#include <cassert>
//#include <cstring>
using std::cin;
using std::cerr;

void generateTable()
{
	string hasFile = "n";
	string infile, outfile;
	int chainLength;
	int numChains = 0;
	cout << "\n===========================\n";
	do
	{
		cout << "Do you have a dictionary file (y/n): ";
		cin >> hasFile;
	} while (hasFile != "y" && hasFile != "n");
	
	if(hasFile == "y")
	{
		cout << "Enter input file name: ";
		cin >> infile;
	}
	else
	{
		bool tryAgain;
		do{
			tryAgain = false;
			cout << "Enter number of chains: ";
			cin >> numChains;
			if(numChains <= 0)
			{
				cout << "Number of chains <= 0. Try again.\n";
				tryAgain = true;
			}
		} while(tryAgain);
	}
	
	//Input chain length
	bool tryAgain;
	do
	{
		tryAgain = false;
		cout << "Enter chain length: ";
		
		cin >> chainLength;
		if(chainLength < 0 || chainLength > 10000)
		{
			cout << "Chain length < 0 or > 10,000.  Try again.\n";
			tryAgain = true;
		}
	} while (tryAgain);
	
	//Read output file
	cout << "Enter output file name: ";
	cin >> outfile;
	
	cout << "\n\nGenerating table...\n";
	RainbowTable * rt;
	if(hasFile == "y")
	{
		rt = new RainbowTable(chainLength, infile);
	}
	else
	{
		rt = new RainbowTable(chainLength, numChains);
	}
	cout << "Done. Printing to file...\n";
	rt->outputToFile(outfile);
	delete rt;
}

void quickDebug()
{
	RainbowTable rt(3, "testDictionary.txt");
	rt.outputToFile("out.txt");
}

bool getCommandLineOptions(int argc, char* argv[], string & directory, string & hashFile)
{
	//Command line word options
	struct option longOptions[] = {
		{"genTable", no_argument,0,'g'},
		{"quickdebug", no_argument, 0,'f'},
		{"help", no_argument, 0,'h'},
		{"directory", required_argument,0,'d'},
		{"file", required_argument,0,'f'},
		{0,0,0,0}
	};
	
	int c, optIndex;
	while ((c=getopt_long(argc,argv,"gqd:hf:",longOptions,&optIndex)) != -1)
	{
		switch(c)
		{
			case 'd':
				directory = optarg;
				break;
				
			case 'f':
				hashFile = optarg;
				break;
				
			case 'g':
				cout << "Generating rainbow table!\nQuit and specify -h/--help";
				cout << " to see how to do lookup of a rainbow table\n";
				generateTable();
				return false;
				
			case 'q':
				quickDebug();
				return false;
				
			case 'h':
				cout << "Welcome to hashbash! Command line options are:\n\n";
				cout << "-g/--genTable:\tEnter rainbow table generation menu\n";
				cout << "-h\t\tDisplay this help message\n";
				break;
				
			default:
				cerr << "Command line option not recognized. -h for help\n";
				return false;
		}
	}
	
	if(directory == "")
	{
		cerr << "Must specify a directory for lookup: -d/--directory.\n";
		exit(1);
	}
	
	if(hashFile == "")
	{
		cerr << "Must specify an input file of hashes: -f/--file.\n";
		exit(1);
	}
	return true;
}

int main(int argc, char * argv[])
{
	string directory="", hashFile="";
	if(!getCommandLineOptions(argc,argv, directory, hashFile))
	{
		return 0;
	}
	
	//Read in hashes into vector
	list<std::pair<int, unsigned char*> > hashes;
	std::ifstream fin(hashFile);
	if(!fin)
	{
		cerr << "hashFile can't be opened\n";
		return 1;
	}
	string hash;
	int hashCount=0;
	while (fin >> hash)
	{
		char *endptr;
		unsigned hlength = hash.length();
		if(hlength == 40)
		{
			unsigned char* byteVal = new unsigned char[EVP_MAX_MD_SIZE];
			assert(byteVal != NULL);
			for(int k=0; k<hlength; k+=2)
			{
				char buf[5] = {'0','x',hash[k],hash[k+1],0};
				byteVal[k/2] = strtol(buf, &endptr, 0);
				if(endptr[0] != '\0')
				{
					cerr << "non-hex character encountered\n";
					break;
				}
			}
			hashes.push_back(std::make_pair(hashCount++,byteVal));
		}
	}
	int numHashes = hashes.size();

	RainbowTable dummy(1,1);
	list<vector<string> > precomputed;	
	dummy.generateReductions(hashes, 2200, precomputed);
	
	//Read directory
	struct dirent *entry;
	DIR *dp;
	dp = opendir(directory.c_str());
	if(dp == NULL)
	{
		cerr << "Directory can't be opened\n";
		return 1;
	}

	//For each file in directory
	int foundCount=0;
	vector<bool> deleted(hashes.size(), false);
	while((entry = readdir(dp)) && !hashes.empty())
	{
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name,".."))
			continue;
		
		string rtName = entry->d_name;
		cout << "loading..." << rtName << std::endl;
		RainbowTable rt(directory+"/"+rtName);
		auto preiter = precomputed.begin();
		auto hashiter = hashes.begin();
		for(int i=0; i<deleted.size() && hashiter != hashes.end() && preiter != precomputed.end(); hashiter++,preiter++, i++)
		{
			if(!deleted[i])
			{
				string found = rt.batchLookup(*preiter, (*hashiter).second);
				if(found != "")
				{
					cout << "hash " << (*hashiter).first << " is: " << found << "\n";
					foundCount++;
					deleted[i] = true;
					//delete (*hashiter).second;
					//precomputed.erase(preiter);
					//hashes.erase(hashiter);
				}
			}
		}
	}
	if(foundCount == numHashes)
	{
		cout << "\n============\nCongrats, you found all the passwords!\n";
	}
	else
	{
		cout << "\n=============\nWe found " << foundCount;
		cout << " out of " << numHashes << "\n";
	}
	for(auto iter = hashes.begin(); iter != hashes.end(); iter++)
	{
		delete (*iter).second;
	}
	closedir(dp);

	return 0;
}
