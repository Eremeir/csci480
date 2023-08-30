/********************************************************************
CSCI 480 - Assignment 1 - Fall 2023

Progammer: Matt Borek
Section:   1
TA:        Sai Dinesh Reddy Bandi
Date Due:  September 10, 2023

Purpose:   This program queries the system kernel to identify key
           information about the OS and CPU and parses it to
           a human readable output.
*********************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include "cpuInfo.h"


int main()
{
    CPUInfo reader;

    reader.readOSInfo();
    reader.readProcessorInfo();
    reader.printInfo();




    return 0;
}