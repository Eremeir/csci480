#include "cpuInfo.h"
#include <iostream>
#include <sstream>
#include <iomanip>

CPUInfo::CPUInfo()
{
    numLogicalCores = 0;
    numPhysicalCores = 0;
    numSockets = 0;
    uptimeSeconds = 0;
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

    std::string line;
    unsigned int totalProcessors = 0;
    unsigned int coresPerSocket = 0;
    while(std::getline(cpuFile, line))
    {
        if(line.find("processor") != std::string::npos)
        {
            ++totalProcessors;
        }
        if(line.find("cpu cores") != std::string::npos)
        {
            coresPerSocket = std::stoi(line.substr(line.find(":") + 2));
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
        double idleTime;
        std::istringstream iss(line);
        if(iss >> totalUptime >> idleTime)
        {
            uptimeSeconds = totalUptime;
        }

    }

    upFile.close();
}

void CPUInfo::printInfo()
{
    std::cout << "A: Questions about " << hostname << "'s OS:" << std::endl;
    std::cout << "1. The type of " << hostname << "'s OS is " << osType << "." << std::endl;
    std::cout << "2. " << hostname << "'s hostname is " << hostname << "." << std::endl;
    std::cout << "3. The OS release of " << hostname << " is " << osRelease << "." << std::endl;
    std::cout << "4. " << hostname << "'s kernel version is " << osVersion << "." << std::endl << std::endl;

    std::cout << "A: Questions about " << hostname << "'s processors:" << std::endl;
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

std::string CPUInfo::printLongTime(const double &uptimeSeconds)
{
    double seconds = uptimeSeconds;
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
