#ifndef CPU_INFO_H
#define CPU_INFO_H

#include <vector>
#include "coreDetail.h"

/**
 * @brief CPUInfo Class.
 * 
 * Retrieves and prints system information, including details about the operating system, 
 * CPU configuration, system uptime, swap space, and CPU time statistics.
 * 
 */
class CPUInfo
{
public: 
    CPUInfo(); //Initializer Constructor.
    void readOSInfo();                          //Read OS Information.
    void readCPUInfo();                         //Read CPU Information.
    void readUptime();                          //Read CPU Uptime.
    void readSwapInfo();                        //Read Swap Device Information.
    void printInfo(int testCPU1, int testCPU2); //Print Information.

private:
    std::string osType;     //Operating System Type
    std::string hostname;   //OS Hostname
    std::string osRelease;  //OS Relase Version
    std::string osVersion;  //OS Version Name

    std::vector<CoreDetail> processors; //Vector holding CoreDetail objects for encapsulating detail on individual cores.
    unsigned int numLogicalCores;       //Number of Logical Cores
    unsigned int numPhysicalCores;      //Number of Physical Cores (Unused)
    unsigned int numSockets;            //Number of physical chip sockets
    double uptimeSeconds;               //Host uptime in seconds.
    unsigned long swapSizeKB;           //Swap device size in KB.

    std::string readProcVal(const std::string &path);    //Read /proc/ Value
    std::string printLongTime(const double &cpuSeconds); //Print Long Time
    void printCPUTimes(int cpuNum);                      //Print CPU Times
    void readCPUState(const int &cpuNum, unsigned long &userSeconds, unsigned long &sysSeconds, unsigned long &idleSeconds); //Read CPU State

};

#endif
