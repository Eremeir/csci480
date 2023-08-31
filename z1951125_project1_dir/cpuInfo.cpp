#include "cpuInfo.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

CPUInfo::CPUInfo()
{
    numLogicalCores = 0;
    numPhysicalCores = 0;
    numSockets = 0;
    uptimeSeconds = 0;
    swapSizeKB = 0;
}

void CPUInfo::readOSInfo()
{
    osType = readProcVal("/proc/sys/kernel/ostype");
    hostname = readProcVal("/proc/sys/kernel/hostname");
    osRelease = readProcVal("/proc/sys/kernel/osrelease");
    osVersion = readProcVal("/proc/sys/kernel/version");
}

void CPUInfo::readCPUInfo()
{
    std::ifstream cpuFile("/proc/cpuinfo");
    if(!cpuFile.is_open())
    {
        std::cerr << "Error opening cpu file." << std::endl;
        return;
    }

    int coreNum;
    std::string vendor;
    std::string model;
    int physAddrSize;
    int virtAddrSize;

    std::string line;
    unsigned int totalProcessors = 0;
    unsigned int coresPerSocket = 0;
    while(std::getline(cpuFile, line))
    {
        if(line.find("processor") != std::string::npos)
        {
            ++totalProcessors;
            coreNum = std::stoi(line.substr(line.find(":") + 2));
        }
        else if(line.find("vendor_id") != std::string::npos)
        {
            vendor = line.substr(line.find(":") + 2);
        }
        else if(line.find("model name") != std::string::npos)
        {
            model = line.substr(line.find(":") + 2);
        }

        else if(line.find("cpu cores") != std::string::npos)
        {
            coresPerSocket = std::stoi(line.substr(line.find(":") + 2));
        }
        else if(line.find("address sizes") != std::string::npos)
        {
            std::istringstream iss(line);
            std::string temp;
            iss >> temp >> temp >> temp >> physAddrSize >> temp >> temp >> virtAddrSize;
            processors.push_back(CoreDetail(coreNum, vendor, model, physAddrSize, virtAddrSize));
        }
    }
    numLogicalCores = totalProcessors;
    numSockets = totalProcessors / coresPerSocket;

    cpuFile.close();
}

void CPUInfo::readUptime()
{
    std::ifstream upFile("/proc/uptime");
    if(!upFile.is_open())
    {
        std::cerr << "Error opening uptime file." << std::endl;
        return;
    }

    std::string line;
    if(std::getline(upFile, line))
    {
        double totalUptime;
        std::istringstream iss(line);
        if(iss >> totalUptime)
        {
            uptimeSeconds = totalUptime;
        }
    }

    upFile.close();
}

void CPUInfo::readSwapInfo()
{
    std::ifstream swapFile("/proc/swaps");
    if(!swapFile.is_open())
    {
        std::cerr << "Error opening swaps file." << std::endl;
        return;
    }

    std::string line;
    std::string temp;
    std::getline(swapFile, temp);
    swapFile >> temp >>temp >> swapSizeKB >> temp;
}


void CPUInfo::printInfo(int testCPU1, int testCPU2)
{
    std::cout << "A: Questions about " << hostname << "'s OS:" << std::endl;
    std::cout << "1. The type of " << hostname << "'s OS is " << osType << "." << std::endl;
    std::cout << "2. " << hostname << "'s hostname is " << hostname << "." << std::endl;
    std::cout << "3. The OS release of " << hostname << " is " << osRelease << "." << std::endl;
    std::cout << "4. " << hostname << "'s kernel version is " << osVersion << "." << std::endl << std::endl;

    std::cout << "B: Questions about " << hostname << "'s processors:" << std::endl;
    std::cout << "1. " << hostname << " has " << numLogicalCores << " processors." << std::endl;
    std::cout << "2. " << hostname << " has " << numSockets << " physical multi-core ";
    if(numSockets > 1)
    {
        std::cout << "chips." << std::endl;
    }
    else
    {
        std::cout << "chip." << std::endl;
    }
    std::cout << "3. " << hostname << " has been up for " << std::fixed << std::setprecision(1) << uptimeSeconds << " seconds." << std::endl;
    std::cout << "4. " << hostname << " has been up for " << printLongTime(uptimeSeconds) << std::endl << std::endl;

    std::cout << "C: Questions about processor " << testCPU1 << std::endl;
    std::cout << "1. The vendor of processor " << testCPU1 << " is " << processors.at(testCPU1).getVendor() << "." << std::endl;
    std::cout << "2. The model name of processor " << testCPU1 << " is " << processors.at(testCPU1).getModel() << "." << std::endl;
    std::cout << "3. The physical address size of processor " << testCPU1 << " is " << processors.at(testCPU1).getPhysAddrSize() << " bits." << std::endl;
    std::cout << "4. The virtual address size of processor " << testCPU1 << " is " << processors.at(testCPU1).getPhysAddrSize() << " bits." << std::endl << std::endl;

    std::cout << "D: Questions about processor " << testCPU2 << std::endl;
    printCPUTimes(testCPU2);

    std::cout << "E. The size of " << hostname << "'s swap device is " << swapSizeKB / 1024 << " MB." << std::endl << std::endl;
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

void CPUInfo::readCPUState(const int &cpuNum, unsigned long &userSeconds, unsigned long &sysSeconds, unsigned long &idleSeconds)
{
    int BCLK = 100; //Hardcoded HZ value for turing, inaccurate elsewhere.
    std::ifstream statFile("/proc/stat");
    if(!statFile.is_open())
    {
        std::cerr << "Error opening stat file." << std::endl;
        return;
    }

    std::string cpuString = "cpu" + std::to_string(cpuNum);
    std::string line;
    while(std::getline(statFile, line))
    {
        if(line.find(cpuString) != std::string::npos)
        {
            std::istringstream iss(line);
            std::string temp;
            iss >> temp >> userSeconds >> temp >> sysSeconds >> idleSeconds;
        }
    }
    userSeconds /= BCLK;
    sysSeconds /= BCLK;
    idleSeconds /= BCLK;

    statFile.close();
}

std::string CPUInfo::printLongTime(const double &cpuTime)
{
    double seconds = cpuTime;
    unsigned int days = static_cast<unsigned int>(seconds) / 86400;
    seconds -= days * 86400.0;

    unsigned int hours = static_cast<unsigned int>(seconds) / 3600;
    seconds -= hours * 3600.0;

    unsigned int minutes = static_cast<unsigned int>(seconds) / 60;
    seconds -= minutes * 60.0;

    std::ostringstream longTime;
    if(days > 0)
    {
        longTime << days << " days, ";
    }
    if(hours > 0)
    {
        longTime << hours << " hours, ";
    }
    if(minutes > 0)
    {
        longTime << minutes << " minutes, and ";
    }

    longTime << std::fixed << std::setprecision(1) << seconds << " seconds.";

    return longTime.str();
}

void CPUInfo::printCPUTimes(int cpuNum)
{
    unsigned long userSeconds = 0;
    unsigned long sysSeconds = 0;
    unsigned long idleSeconds = 0;

    readCPUState(cpuNum, userSeconds, sysSeconds, idleSeconds);

    std::cout << "1. Processor " << cpuNum << " has spent " << userSeconds << " seconds in user mode." << std::endl;
    std::cout << "2. Processor " << cpuNum << " has spent " << sysSeconds << " seconds in system mode." << std::endl;
    std::cout << "3. Processor " << cpuNum << " has spent " << idleSeconds << " seconds in idle mode." << std::endl;
    std::cout << "4. Processor " << cpuNum << " has spent " << printLongTime(idleSeconds) << " in idle mode." << std::endl << std::endl;

}
