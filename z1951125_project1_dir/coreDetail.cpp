#include "coreDetail.h"

/**
 * @brief Construct a CoreDetail object.
 * 
 * Initializes a CoreDetail object with an initializer list of const refrences.
 * This object holds constant refrences to information and has no set methods as the information
 * should never change once set, and only need to be accessed to see details.
 * 
 * @param coreNum The core number of the processor.
 * @param vendor The vendor name of the processor.
 * @param model The model name of the processor.
 * @param physAddrSize The physical address size of the processor in bits.
 * @param virtAddrSize The virtual address size of the processor in bits.
 */
CoreDetail::CoreDetail(const unsigned int &coreNum, const std::string &vendor, const std::string &model, const unsigned int &physAddrSize, const unsigned int &virtAddrSize)
    : coreNum(coreNum), vendor(vendor), model(model), physAddrSize(physAddrSize), virtAddrSize(virtAddrSize)
{

}

/**
 * @brief Get core number.
 * 
 * Retrieves the core number assigned to the processor object.
 * 
 * @return const unsigned& for the core number.
 */
const unsigned int& CoreDetail::getCoreNum() const
{
    return coreNum;
}

/**
 * @brief Get vendor name.
 * 
 * Retrieves the vendor name set for the processor object.
 * 
 * @return const std::string& for the vendor name.
 */
const std::string& CoreDetail::getVendor() const
{
    return vendor;
}

/**
 * @brief Get model name.
 * 
 * Retrieves the model name set for the processor object.
 * 
 * @return const std::string& for the model name.
 */
const std::string& CoreDetail::getModel() const
{
    return model;
}

/**
 * @brief Get the physical address size.
 * 
 * Retrieves the physical address size for the core, in bits.
 * 
 * @return const unsigned& for the bits of physical address space.
 */
const unsigned int& CoreDetail::getPhysAddrSize() const
{
    return physAddrSize;
}

/**
 * @brief Get the virtual address size.
 * 
 * Retrieves the virtual address size for the core, in bits.
 * 
 * @return const unsigned& for the bits of virtual address space.
 */
const unsigned int& CoreDetail::getvirtAddrSize() const
{
    return virtAddrSize;
}
