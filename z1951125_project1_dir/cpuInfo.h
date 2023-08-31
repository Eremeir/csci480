#ifndef CPU_INFO_H
#define CPU_INFO_H

#include "coreDetail.h"

class CPUInfo
{
public: 
    CPUInfo();
    void readOSInfo();
    void readCPUInfo();
    void readUptime();
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
    unsigned long swapSizeKB;

    std::string readProcVal(const std::string &path);
    void readCPUState(const int &cpuNum, unsigned long &userSeconds, unsigned long &sysSeconds, unsigned long &idleSeconds);
    std::string printLongTime(const double &cpuSeconds);
    void printCPUTimes(int cpuNum);

};

#endif
