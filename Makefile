all: RainbowTable.cpp testMain.cpp RainbowTable.h
	g++ -lssl -lcrypto RainbowTable.cpp testMain.cpp -o test