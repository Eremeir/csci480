#ifndef CPU_INFO_H
#define CPU_INFO_H

#include <fstream>
#include <string>
#include <vector>
#include "coreDetail.h"

class CPUInfo
{
public: 
    CPUInfo();
    void readOSInfo();
    void readCPUInfo();
    void readUptime();
    
    void readCPUState(int cpuNum);
    void readSwapInfo();
    void printInfo(int testCPU1, int testCPU2);

private:
    std::string osType;
    std::string hostname;
    std::string osRelease;
    std::string osVersion;

    std::vector<CoreDetail> processors;
    unsigned int numLogicalCores;
    unsigned int numPhysicalCores;
    unsigned int numSockets;
    double uptimeSeconds;
    unsigned long userTime;
    unsigned long systemTime;
    unsigned long idleTime;
    unsigned long spawSizeKB;

    std::string readProcVal(const std::string &path);
    std::string printLongTime(const double &uptimeSeconds);
    std::string printCPUDetal(int cpuNum);

};

#endif
