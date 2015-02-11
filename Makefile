FLAGS = -O3 -std=c++11 -lssl -lcrypto

all: RainbowTable.h RainbowTable.cpp hashbashDriver.cpp
	g++  RainbowTable.cpp hashbashDriver.cpp -o hashbash $(FLAGS)

test: RainbowTable.cpp testMain.cpp RainbowTable.h
	g++   RainbowTable.cpp testMain.cpp -o test $(FLAGS)
