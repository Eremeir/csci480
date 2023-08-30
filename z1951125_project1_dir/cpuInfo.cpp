#include "cpuInfo.h"

CPUInfo::CPUInfo()
{
    
}

std::string CPUInfo::readOSInfo()
{
    std::string osType = readProcVal("/proc/sys/kernel/ostype");
    return osType;
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
