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
#include "cpuInfo.h"

#define TEST_CPU_1 0 //Test CPU for Question Block 3
#define TEST_CPU_2 5 //Test CPU for Question Block 4

/**
 * @brief Main function to test the CPUInfo class.
 * 
 * Creates a CPUInfo object, calls class reader functions, and prints information on the operation system, processors and two individual cores.
 * 
 * Program specifies to answer questions about two specific cores, those cores can be adjusted to print information on other cores.
 * 
 * @return int indicating program's exit status.
 */
int main()
{
    CPUInfo reader;

    reader.readOSInfo(); //Read /proc/ files and load information into the class.
    reader.readCPUInfo();
    reader.readUptime();
    reader.readSwapInfo();
    reader.printInfo(TEST_CPU_1, TEST_CPU_2); //Print parsed information and details on two test cores.

    return 0;
}
