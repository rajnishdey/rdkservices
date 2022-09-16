#pragma once

#include <stdint.h>
#include <vector>


namespace device {

class HdmiInputImpl {
public:
    virtual ~HdmiInputImpl() = default;

    virtual uint8_t getNumberOfInputs() const = 0;
    virtual bool isPortConnected(int8_t Port) const = 0;
    virtual std::string getCurrentVideoMode() const = {'u','n','k','n','o','w','n' };

    virtual std::string getEDIDBytesInfo(int8_t iPort, vector<uint8_t> edidVec2) const = {'u','n','k','n','o','w','n' };
    virtual std::string getHDMISPDInfo(int8_t iPort, vector<uint8_t> edidVec2) const = {'u','n','k','n','o','w','n' };
    virtual int8_t setEdidVersion(int8_t iPort, int8_t iEdidVer) const = 0;
    virtual int8_t setEdidVersion(int8_t iPort, int8_t edidVersion) const = 0;

    virtual int8_t selectPort(int8_t iPort) const = 0;

    virtual int8_t scaleVideo(int8_t x, int8_t y, int8_t width, int8_t height) const = 0;
    virtual std::string getSupportedGameFeatures(vector<string> supportedFeatures) const = {'u','n','k','n','o','w','n' };


    virtual bool getHdmiALLMStatus(int8_t iPort, bool allm) const = 0;


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

    std::string getEDIDBytesInfo(int8_t iPort, vector<uint8_t> edidVec2) const
    {
        return impl->getEDIDBytesInfo(iPort,edidVec2);
    }
    std::string getHDMISPDInfo(iPort,edidVec2) const
    {
        return impl->getHDMISPDInfo(iPort, edidVec2);
    }
    int8_t setEdidVersion(iPort, iEdidVer) const
    {
        return impl->setEdidVersion(iPort, iEdidVer);
    }
    int8_t getEdidVersion(iPort, edidVersion) const
    {
        return impl->getEdidVersion(iPort, edidVersion);
    }

    int8_t selectPort(int8_t iPort) const
    {
        return impl->selectPort(iPort);
    }

    int8_t scaleVideo(int8_t x, int8_t y, int8_t width, int8_t height) const
    {
        return impl->scaleVideo(x, y, width, height);
    }

    
    std::string getSupportedGameFeatures(vector<string> supportedFeatures) const
    {
        return impl->getSupportedGameFeatures(supportedFeatures);
    }


    bool getHdmiALLMStatus(int8_t iPort, bool allm) const
    {
        return impl->getHdmiALLMStatus(iPort, allm);
    }
};

}

