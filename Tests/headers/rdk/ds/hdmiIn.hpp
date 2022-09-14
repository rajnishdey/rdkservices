#pragma once

#include <stdint.h>

namespace device {

class HdmiInputImpl {
public:
    virtual ~HdmiInputImpl() = default;

    virtual uint8_t getNumberOfInputs() const = 0;
    virtual bool isPortConnected(int8_t Port) const = 0;
    virtual std::string getCurrentVideoMode() const = {'u','n','k','n','o','w','n' };

    virtual vector<uint8_t> getEDIDBytesInfo(int8_t iPort, vector<uint8_t> edidVec2) const = {'u','n','k','n','o','w','n' };
    virtual vector<uint8_t> getHDMISPDInfo(int8_t iPort, vector<uint8_t> edidVec2) const = {'u','n','k','n','o','w','n' };
    virtual int8_t setEdidVersion(int8_t iPort, int8_t iEdidVer) const = 0;
    virtual int8_t setEdidVersion(int8_t iPort, int8_t edidVersion) const = 0;

    virtual int8_t selectPort(int8_t iPort) const = 0;
    virtual int8_t setVideoRectangle(int8_t x, int8_t y, int8_t width, int8_t height) const = 0;

    virtual vector<uint8_t> getEDIDBytesInfo(vector<string> supportedFeatures) const = {'u','n','k','n','o','w','n' };


    bool getHdmiALLMStatus(int8_t iPort, bool allm) const = 0;


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

    std::string vector<uint8_t> getEDIDBytesInfo(int8_t iPort, vector<uint8_t> edidVec2) const
    {
        return impl->getEDIDBytesInfo(int8_t iPort, vector<uint8_t> edidVec2);
    }
    std::string vector<uint8_t> getHDMISPDInfo(int8_t iPort, vector<uint8_t> edidVec2) const
    {
        return impl->getHDMISPDInfo(int8_t iPort, vector<uint8_t> edidVec2);
    }
    int8_t setEdidVersion(int8_t iPort, int8_t iEdidVer) const
    {
        return impl->setEdidVersion(int8_t iPort, int8_t iEdidVer);
    }
    int8_t getEdidVersion(int8_t iPort, int8_t edidVersion) const
    {
        return impl->getEdidVersion(int8_t iPort, int8_t edidVersion);
    }

    int8_t selectPort(int8_t iPort) const
    {
        return impl->selectPort(int8_t iPort);
    }

    int8_t setVideoRectangle(int8_t x, int8_t y, int8_t width, int8_t height) const
    {
        return impl->setVideoRectangle(int8_t x, int8_t y, int8_t width, int8_t height);
    }
    vector<uint8_t> getEDIDBytesInfo(vector<string> supportedFeatures) const
    {
        return impl->getEDIDBytesInfo(vector<string> supportedFeatures);
    }

    vector<uint8_t> getEDIDBytesInfo(vector<string> supportedFeatures) const
    {
        return impl->getEDIDBytesInfo(vector<string> supportedFeatures);
    }

    bool getHdmiALLMStatus(int8_t iPort, bool allm) const
    {
        return impl->getHdmiALLMStatus(int8_t iPort, bool allm);
    }
};

}
