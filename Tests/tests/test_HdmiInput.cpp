#include <gtest/gtest.h>

#include "HdmiInput.h"


#include "HdmiInputMock.h"
#include "IarmBusMock.h"
#include "ServiceMock.h"

#include "dsMgr.h"


using namespace WPEFramework;

class HdmiInputTest : public ::testing::Test {
protected:
    Core::ProxyType<Plugin::HdmiInput> plugin;
    Core::JSONRPC::Handler& handler;
    Core::JSONRPC::Handler& handlerV2;
    Core::JSONRPC::Connection connection;
    string response;

    HdmiInputTest()
        : plugin(Core::ProxyType<Plugin::HdmiInput>::Create())
        , handler(*(plugin))
        , handlerV2(*(plugin->GetHandler(2)))
        , connection(1, 0)
    {
    }
    virtual ~HdmiInputTest() = default;
};

class HdmiInputDsTest : public HdmiInputTest {
protected:
    HdmiInputImplMock hdmiInputImplMock;

    HdmiInputDsTest()
        : HdmiInputTest()
    {
        device::HdmiInput::getInstance().impl = &hdmiInputImplMock;
    }
    virtual ~HdmiInputDsTest() override
    {
        device::HdmiInput::getInstance().impl = nullptr;
    }
};

class HdmiInputInitializedTest : public HdmiInputTest {
protected:
    IarmBusImplMock iarmBusImplMock;
    IARM_EventHandler_t dsHdmiEventHandler;

    HdmiInputInitializedTest()
        : HdmiInputTest()
    {
        IarmBus::getInstance().impl = &iarmBusImplMock;

        ON_CALL(iarmBusImplMock, IARM_Bus_RegisterEventHandler(::testing::_, ::testing::_, ::testing::_))
            .WillByDefault(::testing::Invoke(
                [&](const char* ownerName, IARM_EventId_t eventId, IARM_EventHandler_t handler) {
                    if ((string(IARM_BUS_DSMGR_NAME) == string(ownerName)) && (eventId == IARM_BUS_DSMGR_EVENT_HDMI_IN_HOTPLUG)) {
                        EXPECT_TRUE(handler != nullptr);
                        dsHdmiEventHandler = handler;
                    }
                    return IARM_RESULT_SUCCESS;
                }));

        EXPECT_EQ(string(""), plugin->Initialize(nullptr));
    }
    virtual ~HdmiInputInitializedTest() override
    {
        plugin->Deinitialize(nullptr);

        IarmBus::getInstance().impl = nullptr;
    }
};



TEST_F(HdmiInputTest, RegisteredMethods)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getHDMIInputDevices")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("writeEDID")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("readEDID")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getRawHDMISPD")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getHDMISPD")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setEdidVersion")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getEdidVersion")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("startHdmiInput")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("stopHdmiInput")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("setVideoRectangle")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getSupportedGameFeatures")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getHdmiGameFeatureStatus")));

    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Exists(_T("getHDMIInputDevices")));
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Exists(_T("writeEDID")));
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Exists(_T("readEDID")));
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Exists(_T("startHdmiInput")));
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Exists(_T("stopHdmiInput")));
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Exists(_T("setVideoRectangle")));
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Exists(_T("getSupportedGameFeatures")));
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Exists(_T("getHdmiGameFeatureStatus")));
}

TEST_F(HdmiInputDsTest, getHDMIInputDevices)
{

    ON_CALL(hdmiInputImplMock, getNumberOfInputs())
        .WillByDefault(::testing::Return(1));
    ON_CALL(hdmiInputImplMock, isPortConnected())
        .WillByDefault(::testing::Return(true));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getHDMIInputDevices"), _T("{}"), response));
    EXPECT_EQ(response, string({"devices\":[{\"id\":0,\"locator\":\"hdmiin://localhost/deviceid/0\",\"connected\": true}], \"success\": true}}"));
}


TEST_F(HdmiInputDsTest, writeEDID)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("writeEDID"), _T("{}"), response));
    EXPECT_EQ(response, string({"{\"success\": true}"));
}

TEST_F(HdmiInputDsTest, readEDID)
{
    ON_CALL(hdmiInputImplMock, getEDIDBytesInfo())
        .WillByDefault(::testing::Return({"t","e","s","t"}));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("readEDID"), _T("{}"), response));
    EXPECT_EQ(response, string({"{\"EDID\": \"test\",\"success\": true}"));
}

TEST_F(HdmiInputDsTest, getRawHDMISPD)
{
    ON_CALL(hdmiInputImplMock, getHDMISPDInfo())
        .WillByDefault(::testing::Return({"t","e","s","t"}));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getRawHDMISPD"), _T("{}"), response));
    EXPECT_EQ(response, string({"{\"HDMISPD\": \"test\",\"success\": true}"));
}

TEST_F(HdmiInputDsTest, getHDMISPD)
{
    ON_CALL(hdmiInputImplMock, getHDMISPDInfo())
        .WillByDefault(::testing::Return({"pkttype": 0,"version": 1,"length": 2,"vendor_name": "test_name", "product_des": "test_product", "source_info": 0}));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getHDMISPD"), _T("{}"), response));
    EXPECT_EQ(response, string({"{\"HDMISPD\": \"Packet Type:0,Version:1,Length:2,vendor name:test_name,product des:test_product,source info:0\",\"success\": true}"));
}

TEST_F(HdmiInputDsTest, setEdidVersionEmpty)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setEdidVersion"), _T("{}"), response));
    EXPECT_EQ(response, string({"{\"success\": false}"));
}
TEST_F(HdmiInputDsTest, setEdidVersion)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setEdidVersion"), _T("{\"portId\": \"0\", \"edidVersion\":\"HDMI1.4\"}"), response));
    EXPECT_EQ(response, string({"{\"success\": true}"));
}

TEST_F(HdmiInputDsTest, getEdidVersionEmpty)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("getEdidVersion"), _T("{}"), response));
    EXPECT_EQ(response, string({"{\"success\": false}"));
}
TEST_F(HdmiInputDsTest, getEdidVersion)
{
    ON_CALL(HdmiInputeMock, setFRFMode(::testing::_))
        .WillByDefault(::testing::Invoke(
            [&](int param) {
                EXPECT_EQ(param, 0);
                return 0;
            }));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getEdidVersion"), _T("{\"portId\": \"0\"}"), response));
    EXPECT_EQ(response, string({"{\"success\": true}")); //Need to define HDMI_EDID_VER_14/HDMI_EDID_VER_20
}

TEST_F(HdmiInputDsTest, startHdmiInputEmpty)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("startHdmiInput"), _T("{}"), response));
    EXPECT_EQ(response, string({"{\"success\": false}"));
}

TEST_F(HdmiInputDsTest, startHdmiInput)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("startHdmiInput"), _T("{\"portId\": \"0\"}"), response));
    EXPECT_EQ(response, string({"{\"success\": true}")); 
}


TEST_F(HdmiInputDsTest, stopHdmiInput)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("stopHdmiInput"), _T("{}"), response));
    EXPECT_EQ(response, string({"{\"success\": true}")); 
}

TEST_F(HdmiInputDsTest, setVideoRectangleEmpty)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setVideoRectangle"), _T("{}"), response));
    EXPECT_EQ(response, string({"{\"success\": false}"));
}

TEST_F(HdmiInputDsTest, setVideoRectangle)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setVideoRectangle"), _T("{\"x\": 0,\"y\": 0,\"w\": 1920,\"h\": 1080}"), response));
    EXPECT_EQ(response, string({"{\"success\": true}")); 
}

TEST_F(HdmiInputDsTest, getSupportedGameFeatures)
{
    ON_CALL(hdmiInputImplMock, getSupportedGameFeatures())
        .WillByDefault(::testing::Return({"ALLM"}));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getSupportedGameFeatures"), _T("{\"supportedGameFeatures\": \"ALLM\",\"success\": true}"), response));
    EXPECT_EQ(response, string({"{\"success\": true}")); 
}
TEST_F(HdmiInputDsTest, getHdmiGameFeatureStatusEmpty)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("getHdmiGameFeatureStatus"), _T("{}"), response));
    EXPECT_EQ(response, string({"{\"success\": false}"));
}

TEST_F(HdmiInputDsTest, getHdmiGameFeatureStatus)
{
    ON_CALL(hdmiInputImplMock, getSupportedGameFeatures())
        .WillByDefault(::testing::Return(false));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getHdmiGameFeatureStatus"), _T("{\"portId\": \"0\",\"gameFeature\": \"ALLM\"}"), response));
    EXPECT_EQ(response, string({"{\"success\": true}")); 
}
