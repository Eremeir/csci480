#include "cpuInfo.h"
#include <iostream>

CPUInfo::CPUInfo()
{
    numLogicalCores = 0;
    numPhysicalCores = 0;
    numSockets = 0;
}

void CPUInfo::readOSInfo()
{
    osType = readProcVal("/proc/sys/kernel/ostype");
    hostname = readProcVal("/proc/sys/kernel/hostname");
    osRelease = readProcVal("/proc/sys/kernel/osrelease");
    osVersion = readProcVal("/proc/sys/kernel/version");
}

void CPUInfo::readProcessorInfo()
{
    std::ifstream cpuFile("/proc/cpuinfo");
    if(!cpuFile.is_open())
    {
        std::cerr << "Error opening cpu file." << std::endl;
        return;
    }

    std::string infoLine;
    std::string currentProcessor;
    unsigned int totalProcessors = 0;
    unsigned int coresPerSocket = 0;
    while(std::getline(cpuFile, infoLine))
    {
        if(infoLine.find("processor") != std::string::npos)
        {
            ++totalProcessors;
        }
        if(infoLine.find("cpu cores") != std::string::npos)
        {
            coresPerSocket = std::stoi(infoLine.substr(infoLine.find(":") + 2));
            std::cout << coresPerSocket;
        }
    }
    numLogicalCores = totalProcessors;
    numSockets = totalProcessors / coresPerSocket;
    cpuFile.close();
}


std::string CPUInfo::readProcVal(const std::string &path)
{
    std::ifstream file(path);
    if(!file.is_open())
    {
        return "Error opening file.";
    }

    std::string val;
    std::getline(file, val);
    file.close();

    return val;
}


void CPUInfo::printInfo()
{
    std::cout << "A: Questions about turing's OS:" << std::endl;
    std::cout << "1. The type of turing's OS is " << osType << "." << std::endl;
    std::cout << "2. Turing's hostname is " << hostname << "." << std::endl;
    std::cout << "3. The OS release of turing is " << osRelease << "." << std::endl;
    std::cout << "4. Turing's kernel version is " << osVersion << "." << std::endl << std::endl;

    std::cout << "A: Questions about turing's processors:" << std::endl;
    std::cout << "1. Turing has " << numLogicalCores << " processors." << std::endl;
    std::cout << "2. Turing has " << numSockets << " physical multi-core chips." << std::endl;
    std::cout << "3. Turing has been up for" << osRelease << " seconds." << std::endl;
    std::cout << "4. Turing has been up for " << osVersion << "." << std::endl << std::endl;

}