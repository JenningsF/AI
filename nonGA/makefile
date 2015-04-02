# makefile

all: nonGA1 nonGA2

nonGA1.o: nonGA1.cpp
	g++ -std=c++11 -c -g nonGA1.cpp

nonGA2.o: nonGA2.cpp
	g++ -std=c++11 -c -g nonGA2.cpp

nonGA1: nonGA1.o
	g++ -std=c++11 -o nonGA1 nonGA1.o

nonGA2: nonGA2.o
	g++ -std=c++11 -o nonGA2 nonGA2.o