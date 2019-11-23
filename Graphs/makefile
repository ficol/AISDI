CXX=g++
CXXFLAGS= -Wall -std=c++11
	
all: main.o
	$(CXX) $(CXXFLAGS) -o graph main.o

main.o: main.cpp graph.h
	$(CXX) $(CXXFLAGS) -c -o main.o main.cpp

clean:
	rm *.o
