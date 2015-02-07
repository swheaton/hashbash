FLAGS = -g -std=c++11 -lssl -lcrypto

all: RainbowTable.h RainbowTable.cpp hashbashDriver.cpp
	g++ $(FLAGS) RainbowTable.cpp hashbashDriver.cpp -o hashbash

test: RainbowTable.cpp testMain.cpp RainbowTable.h
	g++ $(FLAGS)  RainbowTable.cpp testMain.cpp -o test
