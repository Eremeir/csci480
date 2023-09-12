#
# Assignment #2
# AUTHOR:  Matt Borek
#

CXX = g++
CXXFLAGS = -g -Wall -Werror -pedantic -std=c++20
PROGS = z1951125_project2

all: $(PROGS) 

z1951125_project2: main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o *.txt $(PROGS)
