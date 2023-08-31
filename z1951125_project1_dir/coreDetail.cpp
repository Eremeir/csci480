#include "coreDetail.h"

CoreDetail::CoreDetail(const unsigned int &coreNum, const std::string &vendor, const std::string &model, const unsigned int &physAddrSize, const unsigned int &virtAddrSize)
    : coreNum(coreNum), vendor(vendor), model(model), physAddrSize(physAddrSize), virtAddrSize(virtAddrSize)
{

}

const unsigned int& CoreDetail::getCoreNum() const
{
    return coreNum;
}
    
const std::string& CoreDetail::getVendor() const
{
    return vendor;
}
    
const std::string& CoreDetail::getModel() const
{
    return model;
}
   
const unsigned int& CoreDetail::getPhysAddrSize() const
{
    return physAddrSize;
}

const unsigned int& CoreDetail::getvirtAddrSize() const
{
    return virtAddrSize;
}
