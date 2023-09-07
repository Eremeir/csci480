#
# Assignment #1
# AUTHOR:  Matt Borek
#

CXX = g++
CXXFLAGS = -g -Wall -Werror -pedantic -std=c++20
PROGS = z1951125_project1

all: $(PROGS) 

z1951125_project1: main.o cpuInfo.o coreDetail.o
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp cpuInfo.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

cpuInfo.o: cpuInfo.cpp cpuInfo.h coreDetail.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

coreDetail.o: coreDetail.cpp coreDetail.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o $(PROGS)
