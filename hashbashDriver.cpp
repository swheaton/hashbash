#include "RainbowTable.h"
#include <getopt.h>
#include <iostream>
#include <fstream>
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

bool getCommandLineOptions(int argc, char* argv[])
{
	//Command line word options
	struct option longOptions[] = {
		{"genTable", no_argument,0,'g'},
		{"help", no_argument, 0,'h'},
		{0,0,0,0}
	};
	
	int c, optIndex;
	while ((c=getopt_long(argc,argv,"gh",longOptions,&optIndex)) != -1)
	{
		switch(c)
		{
			case 'g':
				cout << "Generating rainbow table!\nQuit and specify -h/--help";
				cout << " to see how to do lookup of a rainbow table\n";
				generateTable();
				return false;
				
			case 'h':
				cout << "help\n";
				break;
				
			default:
				cerr << "Command line option not recognized\n";
				return false;
		}
	}
}

int main(int argc, char * argv[])
{
	if(!getCommandLineOptions(argc,argv))
	{
		return 0;
	}
	return 0;
}