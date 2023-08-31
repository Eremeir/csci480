#include "coreDetail.h"

CoreDetail::CoreDetail(const int &coreNum, const std::string &vendor, const std::string &model, const int &physAddrSize, const int &virtAddrSize)
    : coreNum(coreNum), vendor(vendor), model(model), physAddrSize(physAddrSize), virtAddrSize(virtAddrSize)
{

}

const int& CoreDetail::getCoreNum() const
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
   
const int& CoreDetail::getPhysAddrSize() const
{
    return physAddrSize;
}

const int& CoreDetail::getvirtAddrSize() const
{
    return virtAddrSize;
}