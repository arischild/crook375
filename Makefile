#Ari Schild

all: crook

crook: crook.cpp
	g++ -g -Wall -o crook crook.cpp

clean:
	-rm crook
