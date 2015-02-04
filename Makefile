all: RainbowTable.cpp testMain.cpp RainbowTable.h
	g++ -std=c++11  RainbowTable.cpp testMain.cpp -o test -lssl -lcrypto
