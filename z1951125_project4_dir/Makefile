#
# Assignment #4
# AUTHOR:  Matt Borek
#

CXX = g++
CXXFLAGS = -g -Wall -Wextra -Werror -Wpedantic -std=c++20
PROGS = z1951125_project4 z1951125_project4_p2

all: $(PROGS) 

z1951125_project4: main.o 
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpthread

main.o: main.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $<

z1951125_project4_p2: mainvariant.o 
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpthread

mainvariant.o: mainvariant.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o $(PROGS)