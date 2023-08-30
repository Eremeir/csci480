#ifndef CPU_INFO_H
#define CPU_INFO_H

#include <fstream>
#include <string>
#include <vector>

class CPUInfo
{
public: 
    CPUInfo();
    std::string readOSInfo();

private:
    std::string readProcVal(const std::string &path);




};


#endif
