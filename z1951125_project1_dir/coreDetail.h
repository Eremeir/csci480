#ifndef CORE_DETAIL_H
#define CORE_DETAIL_H

#include <string>
#include <vector>

class CoreDetail
{
public:
    CoreDetail(const int &coreNum, const std::string &vendor, const std::string &model, const int &physAddrSize, const int &virtAddrSize);
    const int& getCoreNum() const;
    const std::string& getVendor() const;
    const std::string& getModel() const;
    const int& getPhysAddrSize() const;
    const int& getvirtAddrSize() const;

private:
    int coreNum;
    std::string vendor;
    std::string model;
    int physAddrSize;
    int virtAddrSize;

};

#endif
