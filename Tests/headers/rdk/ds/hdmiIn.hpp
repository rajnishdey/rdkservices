#pragma once

#include <stdint.h>
#include "dsTypes.h"

namespace device {

class HdmiInputImpl {
public:
    virtual ~HdmiInputImpl() = default;

    virtual uint8_t getNumberOfInputs() const = 0;
    virtual bool isPortConnected(int8_t Port) const = 0;
    virtual std::string getCurrentVideoMode() const = 0;
    virtual void selectPort (int8_t Port) const = 0;
    virtual void scaleVideo (int32_t x, int32_t y, int32_t width, int32_t height) const = 0;
    virtual std::vector<uint8_t>  getEDIDBytesInfo (int iHdmiPort, std::vector<uint8_t> &edid) const = 0;
    virtual std::vector<uint8_t>  getHDMISPDInfo (int iHdmiPort, std::vector<uint8_t> &data) const = 0;
    virtual void setEdidVersion (int iHdmiPort, int iEdidVersion) const = 0;
    virtual int getEdidVersion (int iHdmiPort, int *iEdidVersion) const = 0;
    virtual bool  getHdmiALLMStatus (int iHdmiPort, bool *allmStatus) const = 0;
    virtual std::vector<std::string>  getSupportedGameFeatures (std::vector<std::string> &featureList) const = 0;
};

class HdmiInput {
public:
    static HdmiInput& getInstance()
    {
        static HdmiInput instance;
        return instance;
    }

    HdmiInputImpl* impl;

    uint8_t getNumberOfInputs() const
    {
        return impl->getNumberOfInputs();
    }
    bool isPortConnected(int8_t Port) const
    {
        return impl->isPortConnected(Port);
    }
    std::string getCurrentVideoMode() const
    {
        return impl->getCurrentVideoMode();
    }
    void selectPort (int8_t Port) const
    {
        return impl->selectPort(Port);
    }
    void scaleVideo (int32_t x, int32_t y, int32_t width, int32_t height) const
    {
        return impl->scaleVideo (x, y, width, height);
    }
    std::vector<uint8_t>  getEDIDBytesInfo (int iHdmiPort, std::vector<uint8_t> &edid) const
    {
        return impl->getEDIDBytesInfo(iHdmiPort, edid);
    }
    std::vector<uint8_t>  getHDMISPDInfo (int iHdmiPort, std::vector<uint8_t> &data) const
    {
        return impl->getHDMISPDInfo(iHdmiPort, data);
    }
    void setEdidVersion (int iHdmiPort, int iEdidVersion) const
    {
        return impl->setEdidVersion(iHdmiPort, iEdidVersion);
    }
    int getEdidVersion (int iHdmiPort, int *iEdidVersion) const
    {
        return impl->getEdidVersion(iHdmiPort,iEdidVersion);
    }
    bool  getHdmiALLMStatus (int iHdmiPort, bool *allmStatus) const
    {
        return impl->getHdmiALLMStatus(iHdmiPort, allmStatus);
    }
    std::vector<std::string>  getSupportedGameFeatures (std::vector<std::string> &featureList) const
    {
        return impl->getSupportedGameFeatures(featureList);
    }
};

}
