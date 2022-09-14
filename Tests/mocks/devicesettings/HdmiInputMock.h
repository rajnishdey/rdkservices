#pragma once

#include <gmock/gmock.h>

#include "hdmiIn.hpp"

class HdmiInputImplMock : public device::HdmiInputImpl {
public:
    virtual ~HdmiInputImplMock() = default;

    MOCK_METHOD(uint8_t, getNumberOfInputs, (), (const, override));
    MOCK_METHOD(bool, isPortConnected, (int8_t Port), (const, override));
    MOCK_METHOD(std::string, getCurrentVideoMode, (), (const, override));

    MOCK_METHOD(vector<uint8_t>, getEDIDBytesInfo, (int8_t iPort, vector<uint8_t> edidVec2), (const, override));
    MOCK_METHOD(vector<uint8_t>, getHDMISPDInfo, (int8_t iPort, vector<uint8_t> edidVec2), (const, override));
    MOCK_METHOD(int8_t, setEdidVersion, (int8_t iPort, int8_t iEdidVer), (const, override));
    MOCK_METHOD(int8_t, getEdidVersion, (int8_t iPort, int8_t edidVersion), (const, override));

    MOCK_METHOD(int8_t, selectPort, (int8_t iPort), (const, override));
    MOCK_METHOD(int8_t, scaleVideo, (int8_t x, int8_t y, int8_t width, int8_t height), (const, override));

    MOCK_METHOD(vector<string>, getSupportedGameFeatures, (vector<string> supportedFeatures), (const, override));

    MOCK_METHOD(bool, getHdmiALLMStatus, (int8_t iPort, bool allm), (const, override));

};
