# makefile

all: nonGA

nonGA.o: nonGA.cpp
	g++ -c -g nonGA.cpp

nonGA: nonGA.o
	g++ -o nonGA nonGA.o
