
all: ../athlete.h ../race.h ../race.cc
	g++ -o athTest athTest.cc ../athlete.cc -std=c++11 -I.. -g
	g++ -o raceTest raceTest.cc ../race.cc ../athlete.cc -std=c++11 -I.. -g

clean:
	rm athTest raceTest
