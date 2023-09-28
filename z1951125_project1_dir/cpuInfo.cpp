#include "cpuInfo.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

/**
 * @brief Construct a CPUInfo object.
 * 
 * Initializes a CPUInfo object with default values for its member variables, 
 * setting integers to 0 and strings to an empty string.
 * 
 */
CPUInfo::CPUInfo()
{
    numLogicalCores = 0;
    numPhysicalCores = 0;
    numSockets = 0;
    uptimeSeconds = 0;
    swapSizeKB = 0;
    osType= "";
    hostname= "";
    osRelease = "";
    osVersion= "";
}

/**
 * @brief Read OS Information
 * 
 * Reads and stores information about the operating system retrieved from the /proc/sys/kernel/ directory,
 * including the operating system, the computer hostname, the OS release version, and the OS version name.
 * 
 * Calls readProcVal with a filepath for code simplicity.
 * 
 */
void CPUInfo::readOSInfo()
{
    osType = readProcVal("/proc/sys/kernel/ostype");
    hostname = readProcVal("/proc/sys/kernel/hostname");
    osRelease = readProcVal("/proc/sys/kernel/osrelease");
    osVersion = readProcVal("/proc/sys/kernel/version");
}

/**
 * @brief Read CPU Information
 * 
 * Reads and parses CPU information from /proc/cpuinfo/ and populates the processors vector 
 * with coreDetail objects holding specific information on individual cores. 
 * Calculates logical cores and socket counts for later printing.
 * 
 * Ouputs an error message if /proc/cpuinfo/ cannot be opened.
 * 
 */
void CPUInfo::readCPUInfo()
{
    std::ifstream cpuFile("/proc/cpuinfo"); //Open filepath.
    if(!cpuFile.is_open()) //Check file validity.
    {
        std::cerr << "Error opening cpu file." << std::endl;
        return;
    }

    int coreNum; //Initialize variables for constructing processor object.
    std::string vendor;
    std::string model;
    int physAddrSize;
    int virtAddrSize;

    std::string line;
    unsigned int totalProcessors = 0; //Total number of processors.
    unsigned int coresPerSocket = 0;  //Total number of cores per CPU Socket
    while(std::getline(cpuFile, line)) //Read through each line to find keyword information lines.
    {
        if(line.find("processor") != std::string::npos)
        {
            ++totalProcessors; //Increment processor count
            coreNum = std::stoi(line.substr(line.find(":") + 2)); //Store integer for core number.
        }
        else if(line.find("vendor_id") != std::string::npos)
        {
            vendor = line.substr(line.find(":") + 2); //Store string for vendor name.
        }
        else if(line.find("model name") != std::string::npos)
        {
            model = line.substr(line.find(":") + 2); //Store string for model name.
        }

        else if(line.find("cpu cores") != std::string::npos)
        {
            coresPerSocket = std::stoi(line.substr(line.find(":") + 2)); //Store integer for core count information.
        }
        else if(line.find("address sizes") != std::string::npos)
        {
            std::istringstream iss(line);
            std::string temp; //Create dump variable to throw unused string sections.
            iss >> temp >> temp >> temp >> physAddrSize >> temp >> temp >> virtAddrSize; //Cycle through line for desired variables.
            processors.push_back(CoreDetail(coreNum, vendor, model, physAddrSize, virtAddrSize)); //Create new CoreDetail object with stored variables and add to holding vector.
        }
    }
    numLogicalCores = totalProcessors; //For non-hyperthreading usecases, physical and logical cores will be the same.
    numSockets = totalProcessors / coresPerSocket; //Calculate socket count from processor count and cores per socket.

    cpuFile.close(); //Close the file.
}

/**
 * @brief Read CPU Uptime
 * 
 * Reads and retrieves system uptime in seconds from /proc/uptime/ and stores it in member variable.
 * 
 * Ouputs an error message if /proc/uptime/ cannot be opened.
 * 
 */
void CPUInfo::readUptime()
{
    std::ifstream upFile("/proc/uptime"); //Open filepath.
    if(!upFile.is_open()) //Check file validity.
    {
        std::cerr << "Error opening uptime file." << std::endl;
        return;
    }

    std::string line;
    if(std::getline(upFile, line))
    {
        double totalUptime;
        std::istringstream iss(line);
        if(iss >> totalUptime) //Checks that the file read can be stored as a double.
        {
            uptimeSeconds = totalUptime;
        }
    }

    upFile.close(); //Close the file.
}

void CPUInfo::readSwapInfo()
{
    std::ifstream swapFile("/proc/swaps"); //Open filepath.
    if(!swapFile.is_open()) //Check file validity.
    {
        std::cerr << "Error opening swaps file." << std::endl;
        return;
    }

    std::string temp;
    std::getline(swapFile, temp); //Dump header line.
    swapFile >> temp >>temp >> swapSizeKB >> temp; //Cycle through remaining line for desired variable.

    swapFile.close(); //Close the file.
}

/**
 * @brief Print Information
 * 
 * Prints full spectrum of system and processor information of the host computer, provides information on two specified test core numbers,
 * and outputs information on the host swap device size.
 * 
 * @param testCPU1 //Test core number to query for core details.
 * @param testCPU2 //Test core number to query for system time information.
 */
void CPUInfo::printInfo(int testCPU1, int testCPU2)
{
    std::cout << "A: Questions about " << hostname << "'s OS:" << std::endl; //OS detail question block
    std::cout << "1. The type of " << hostname << "'s OS is " << osType << "." << std::endl;
    std::cout << "2. " << hostname << "'s hostname is " << hostname << "." << std::endl;
    std::cout << "3. The OS release of " << hostname << " is " << osRelease << "." << std::endl;
    std::cout << "4. " << hostname << "'s kernel version is " << osVersion << "." << std::endl << std::endl;

    std::cout << "B: Questions about " << hostname << "'s processors:" << std::endl; //Processor detail question block
    std::cout << "1. " << hostname << " has " << numLogicalCores << " processors." << std::endl;
    std::cout << "2. " << hostname << " has " << numSockets << " physical multi-core ";
    if(numSockets > 1) //Adjust print grammar if host is multi-chip.
    {
        std::cout << "chips." << std::endl;
    }
    else
    {
        std::cout << "chip." << std::endl;
    }
    std::cout << "3. " << hostname << " has been up for " << std::fixed << std::setprecision(1) << uptimeSeconds << " seconds." << std::endl;
    std::cout << "4. " << hostname << " has been up for " << printLongTime(uptimeSeconds) << std::endl << std::endl;

    std::cout << "C: Questions about processor " << testCPU1 << std::endl; //Individual core detail question block
    std::cout << "1. The vendor of processor " << testCPU1 << " is " << processors.at(testCPU1).getVendor() << "." << std::endl;
    std::cout << "2. The model name of processor " << testCPU1 << " is " << processors.at(testCPU1).getModel() << "." << std::endl;
    std::cout << "3. The physical address size of processor " << testCPU1 << " is " << processors.at(testCPU1).getPhysAddrSize() << " bits." << std::endl;
    std::cout << "4. The virtual address size of processor " << testCPU1 << " is " << processors.at(testCPU1).getPhysAddrSize() << " bits." << std::endl << std::endl;

    std::cout << "D: Questions about processor " << testCPU2 << std::endl; //Individual core system times question block
    printCPUTimes(testCPU2); //Call sub print function for time statistics printing.

    std::cout << "E. The size of " << hostname << "'s swap device is " << swapSizeKB / 1024 << " MB." << std::endl << std::endl; //Print swap device size in mb
}

/**
 * @brief Read Proc Value
 * 
 * Reads the contents of the file located at the specified path and returns the value as a string.
 * 
 * Outputs an error if the file cannot be opened or read.
 * 
 * @param path /proc/sys filepath to read.
 * @return std::string for the file contents or error string.
 */
std::string CPUInfo::readProcVal(const std::string &path)
{
    std::ifstream file(path); //Open filepath.
    if(!file.is_open()) //Check file validity.
    {
        return "Error opening file.";
    }

    std::string val;
    std::getline(file, val); //Store line string in val.
    file.close(); //Close the file.

    return val;
}

/**
 * @brief Print Long Time
 * 
 * Convert a time in seconds to a human readable format extending days, hours, minutes and seconds.
 * Will only output a section if time unit is greater than 0.
 * 
 * @param cpuTime Time value in seconds to be converted.
 * @return std::string String representing time in the format "X days, X hours, X minutes, and X.X seconds."
 */
std::string CPUInfo::printLongTime(const double &cpuTime)
{
    double seconds = cpuTime;
    unsigned int days = static_cast<unsigned int>(seconds) / 86400; //Divide time into days
    seconds -= days * 86400.0;

    unsigned int hours = static_cast<unsigned int>(seconds) / 3600; //Divide remaining time into hours.
    seconds -= hours * 3600.0;

    unsigned int minutes = static_cast<unsigned int>(seconds) / 60; //Divide remaining time into minutes.
    seconds -= minutes * 60.0;

    std::ostringstream longTime;
    if(days > 0) //Append time sections if they are greater than 0.
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

    longTime << std::fixed << std::setprecision(1) << seconds << " seconds."; //Set precision to one decimal.

    return longTime.str();
}

/**
 * @brief Print CPU Times
 * 
 * Prints CPU Time statistics for a specified core number in a human readable format.
 * 
 * Calls readCPUStats to populate variables and printLongTime for extended time format output.
 * 
 * @param cpuNum CPU Core number to retrieve time statistics for.
 */
void CPUInfo::printCPUTimes(int cpuNum)
{
    unsigned long userSeconds = 0;
    unsigned long sysSeconds = 0;
    unsigned long idleSeconds = 0;

    readCPUState(cpuNum, userSeconds, sysSeconds, idleSeconds); //Populate second counts.

    std::cout << "1. Processor " << cpuNum << " has spent " << userSeconds << " seconds in user mode." << std::endl;
    std::cout << "2. Processor " << cpuNum << " has spent " << sysSeconds << " seconds in system mode." << std::endl;
    std::cout << "3. Processor " << cpuNum << " has spent " << idleSeconds << " seconds in idle mode." << std::endl;
    std::cout << "4. Processor " << cpuNum << " has spent " << printLongTime(idleSeconds) << " in idle mode." << std::endl << std::endl;
}

/**
 * @brief Read CPU State
 * 
 * Read and retrieve CPU state information from /proc/stat/ for a specific core, calculate and store total user, system and idle second
 * times in to passed in refrence variables.
 * 
 * Outputs an error if the file cannot be opened or read.
 * 
 * @param cpuNum CPU Core number to retrieve time statistics for.
 * @param userSeconds Refrence to store total user time in seconds.
 * @param sysSeconds Refrence to store total system time in seconds.
 * @param idleSeconds Refrence to store total idle time in seconds.
 */
void CPUInfo::readCPUState(const int &cpuNum, unsigned long &userSeconds, unsigned long &sysSeconds, unsigned long &idleSeconds)
{
    int BCLK = 100; //Hardcoded baseclock HZ value for turing, inaccurate elsewhere.
    std::ifstream statFile("/proc/stat"); //Open filepath.
    if(!statFile.is_open()) //Check file validity.
    {
        std::cerr << "Error opening stat file." << std::endl;
        return;
    }

    std::string cpuString = "cpu" + std::to_string(cpuNum); //Construct search string from core num and prefix.
    std::string line;
    while(std::getline(statFile, line)) //Read through each line to find cpu keyword information line.
    {
        if(line.find(cpuString) != std::string::npos)
        {
            std::istringstream iss(line);
            std::string temp; //Create dump variable to throw unused string sections.
            iss >> temp >> userSeconds >> temp >> sysSeconds >> idleSeconds; //Cycle through line for desired variables.
        }
    }
    userSeconds /= BCLK; //Convert from jiffies to seconds using hardcoded baseclock value.
    sysSeconds /= BCLK;
    idleSeconds /= BCLK;

    statFile.close(); //Close file.
}
