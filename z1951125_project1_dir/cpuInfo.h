#ifndef CPU_INFO_H
#define CPU_INFO_H

#include <fstream>
#include <string>
#include <vector>

class CPUInfo
{
public: 
    CPUInfo();
    void readOSInfo();
    void readProcessorInfo();
    void readUptime();
    void readCPUState();
    void readSwapInfo();
    void printInfo();

private:
    std::string osType;
    std::string hostname;
    std::string osRelease;
    std::string osVersion;

    std::vector<std::string> processorBlocks;
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




};


#endif
