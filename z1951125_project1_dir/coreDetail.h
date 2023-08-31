#ifndef CORE_DETAIL_H
#define CORE_DETAIL_H

#include <string>
#include <vector>

class CoreDetail
{
public:
    CoreDetail(const unsigned int &coreNum, const std::string &vendor, const std::string &model, const unsigned int &physAddrSize, const unsigned int &virtAddrSize);
    
    const unsigned int& getCoreNum() const;
    const std::string& getVendor() const;
    const std::string& getModel() const;
    const unsigned int& getPhysAddrSize() const;
    const unsigned int& getvirtAddrSize() const;

private:
    unsigned int coreNum;
    std::string vendor;
    std::string model;
    unsigned int physAddrSize;
    unsigned int virtAddrSize;

};

#endif
