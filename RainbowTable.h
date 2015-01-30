typedef hash __uint128_t;
class RainbowTable
{
	public:
		RainbowTable(string fileName);
		RainbowTable(int numChains, int chainLength, string dictName);
		RainbowTable(int numChains, int chainLength);
		void outputToFile(string fileName);
		string lookup(hash);
}