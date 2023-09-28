#ifndef CORE_DETAIL_H
#define CORE_DETAIL_H

#include <string>

/**
 * @brief CoreDetail class.
 * 
 * Encapsulates details for an individual processor core, holding information and accessors for a core's number, vendor, model name and address sizes. 
 * 
 */
class CoreDetail
{
public:
    CoreDetail(const unsigned int &coreNum, const std::string &vendor, const std::string &model, const unsigned int &physAddrSize, const unsigned int &virtAddrSize); //Constructor

    const unsigned int& getCoreNum() const;       //Get core number.
    const std::string& getVendor() const;         //Get vendor name.
    const std::string& getModel() const;          //Get model name.
    const unsigned int& getPhysAddrSize() const;  //Get the physical address size.
    const unsigned int& getvirtAddrSize() const;  //Get the virtual address size.

private:
    unsigned int coreNum;       //Core number of the processor
    std::string vendor;         //Vendor name of the processor
    std::string model;          //Model name of the processor
    unsigned int physAddrSize;  //Physical address Size in bits
    unsigned int virtAddrSize;  //Virtual address size in bits.

};

#endif
