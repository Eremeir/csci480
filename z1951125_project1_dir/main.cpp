/********************************************************************
CSCI 480 - Assignment 1 - Fall 2023

Progammer: Matt Borek
Section:   Your section number goes here
TA:        Your Teaching Assistant's name goes here
Date Due:  The assignment due date goes here

Purpose:   A brief (2-4 sentences) description of what the program does
           goes here. For example:

           This program accepts a single number from the keyboard
           representing a temperature in Fahrenheit. It then converts
           it to Centigrade, and displays it.
*********************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include "cpuInfo.h"


int main()
{
    CPUInfo reader;

    std::cout << reader.readOSInfo() << std::endl;




    return 0;
}