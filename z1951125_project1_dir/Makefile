#
# Assignment #1
# AUTHOR:  Matt Borek
#

CXX = g++
CXXFLAGS = -g -Wall -Werror -std=c++20
PROGS = z1951125_project_1

all: $(PROGS) 

z1951125_project_1: main.o cpuInfo.o
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp cpuInfo.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

cpuInfo.o: cpuInfo.cpp cpuInfo.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o $(PROGS)
