#include <gtest/gtest.h>

#include "HdmiInput.h"

#include "FactoriesImplementation.h"

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
    IARM_EventHandler_t dsHdmiStatusEventHandler;

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
                        dsHdmiStatusEventHandler = handler;
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


class HdmiInputInitializedEventTest : public HdmiInputInitializedTest {
protected:
    ServiceMock service;
    Core::JSONRPC::Message message;
    FactoriesImplementation factoriesImplementation;
    PluginHost::IDispatcher* dispatcher;

    HdmiInputInitializedEventTest()
        : HdmiInputInitializedTest()
    {
        PluginHost::IFactories::Assign(&factoriesImplementation);

        dispatcher = static_cast<PluginHost::IDispatcher*>(
            plugin->QueryInterface(PluginHost::IDispatcher::ID));
        dispatcher->Activate(&service);
    }

    virtual ~HdmiInputInitializedEventTest() override
    {
        dispatcher->Deactivate();
        dispatcher->Release();

        PluginHost::IFactories::Assign(nullptr);
    }
};

class HdmiInputInitializedEventDsTest : public HdmiInputInitializedEventTest {
protected:
    HdmiInputImplMock hdmiInputImplMock;

    HdmiInputInitializedEventDsTest()
        : HdmiInputInitializedEventTest()
    {
        device::HdmiInput::getInstance().impl = &hdmiInputImplMock;
    }

    virtual ~HdmiInputInitializedEventDsTest() override
    {
        device::HdmiInput::getInstance().impl = nullptr;
    }
};



TEST_F(HdmiInputTest, RegisteredMethods)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("getHDMIInputDevices")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("writeEDID")));
    EXPECT_EQ(Core::ERROR_NONE, handler.Exists(_T("readEDID")));
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Exists(_T("getRawHDMISPD")));
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Exists(_T("getHDMISPD")));
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Exists(_T("setEdidVersion")));
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Exists(_T("getEdidVersion")));
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
    ON_CALL(hdmiInputImplMock, isPortConnected(::testing::_))
        .WillByDefault(::testing::Return(true));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getHDMIInputDevices"), _T("{}"), response));
    EXPECT_EQ(response, string("{\"devices\":[{\"id\":0,\"locator\":\"hdmiin:\\/\\/localhost\\/deviceid\\/0\",\"connected\":\"true\"}],\"success\":true}"));
}


TEST_F(HdmiInputDsTest, writeEDID)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("writeEDID"), _T("{\"deviceId\": 0, \"message\": \"message\"}"), response));
    EXPECT_EQ(response, string("{\"success\":true}"));
}

TEST_F(HdmiInputDsTest, readEDID)
{
    ON_CALL(hdmiInputImplMock, getEDIDBytesInfo(::testing::_,::testing::_))
        .WillByDefault(::testing::Invoke(
            [&](int iport, std::vector<uint8_t> &edidVec2) {
                edidVec2 = std::vector<uint8_t>({ 't', 'e', 's', 't' });
            }));        
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("readEDID"), _T("{\"deviceId\": 0}"), response));
    EXPECT_EQ(response, string("{\"EDID\":\"dGVzdA==\",\"success\":true}"));
}

TEST_F(HdmiInputDsTest, getRawHDMISPD)
{
    ON_CALL(hdmiInputImplMock, getHDMISPDInfo(::testing::_,::testing::_))
        .WillByDefault(::testing::Invoke(
            [&](int iport, std::vector<uint8_t>& edidVec2) {
                edidVec2 = { 't', 'e', 's', 't' };
            }));   
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Invoke(connection, _T("getRawHDMISPD"), _T("{\"portId\":0}"), response));
    EXPECT_EQ(response, string("{\"HDMISPD\":\"dGVzdA\",\"success\":true}"));
}

TEST_F(HdmiInputDsTest, getHDMISPD)
{
    ON_CALL(hdmiInputImplMock, getHDMISPDInfo(::testing::_,::testing::_))
        .WillByDefault(::testing::Invoke(
            [&](int iport, std::vector<uint8_t>& edidVec2) {
                edidVec2 = {'0','1','2','n', 'p', '0'};
            })); 
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Invoke(connection, _T("getHDMISPD"), _T("{\"portId\":0}"), response));
    EXPECT_EQ(response, string("{\"HDMISPD\":\"Packet Type:30,Version:49,Length:50,vendor name:0n,product des:,source info:00\",\"success\":true}"
));
}


TEST_F(HdmiInputDsTest, setEdidVersionEmpty)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handlerV2.Invoke(connection, _T("setEdidVersion"), _T("{}"), response));
    EXPECT_EQ(response, string(""));
}

TEST_F(HdmiInputDsTest, setEdidVersion)
{
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Invoke(connection, _T("setEdidVersion"), _T("{\"portId\": \"0\", \"edidVersion\":\"HDMI1.4\"}"), response));
    EXPECT_EQ(response, string("{\"success\":true}"));
}

TEST_F(HdmiInputDsTest, getEdidVersionEmpty)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handlerV2.Invoke(connection, _T("getEdidVersion"), _T("{}"), response));
    EXPECT_EQ(response, string(""));
}
TEST_F(HdmiInputDsTest, getEdidVersionVer14)
{
    ON_CALL(hdmiInputImplMock, getEdidVersion(::testing::_,::testing::_))
        .WillByDefault(::testing::Invoke(
            [&](int iPort, int *edidVersion) {
                *edidVersion = 0;
            })); 
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Invoke(connection, _T("getEdidVersion"), _T("{\"portId\": \"0\"}"), response));
    EXPECT_EQ(response, string("{\"edidVersion\":\"HDMI1.4\",\"success\":true}"));
}
TEST_F(HdmiInputDsTest, getEdidVersionVer20)
{
    ON_CALL(hdmiInputImplMock, getEdidVersion(::testing::_,::testing::_))
        .WillByDefault(::testing::Invoke(
            [&](int iPort, int *edidVersion) {
                *edidVersion = 1;
            })); 
    EXPECT_EQ(Core::ERROR_NONE, handlerV2.Invoke(connection, _T("getEdidVersion"), _T("{\"portId\": \"0\"}"), response));
    EXPECT_EQ(response, string("{\"edidVersion\":\"HDMI2.0\",\"success\":true}"));
}

TEST_F(HdmiInputDsTest, startHdmiInputEmpty)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("startHdmiInput"), _T("{}"), response));
    EXPECT_EQ(response, string(""));
}

TEST_F(HdmiInputDsTest, startHdmiInput)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("startHdmiInput"), _T("{\"portId\": \"0\"}"), response));
    EXPECT_EQ(response, string("{\"success\":true}")); 
}


TEST_F(HdmiInputDsTest, stopHdmiInput)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("stopHdmiInput"), _T("{}"), response));
    EXPECT_EQ(response, string("{\"success\":true}")); 
}

TEST_F(HdmiInputDsTest, setVideoRectangleEmpty)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("setVideoRectangle"), _T("{}"), response));
    EXPECT_EQ(response, string(""));
}

TEST_F(HdmiInputDsTest, setVideoRectangle)
{
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("setVideoRectangle"), _T("{\"x\": 0,\"y\": 0,\"w\": 1920,\"h\": 1080}"), response));
    EXPECT_EQ(response, string("{\"success\":true}")); 
}


TEST_F(HdmiInputDsTest, getSupportedGameFeatures)
{
    ON_CALL(hdmiInputImplMock, getSupportedGameFeatures(::testing::_))
        .WillByDefault(::testing::Invoke(
            [&](std::vector<std::string> &supportedFeatures) {
                supportedFeatures = {"ALLM"};
            })); 
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getSupportedGameFeatures"), _T("{\"supportedGameFeatures\": \"ALLM\"}"), response));
    EXPECT_EQ(response, string("{\"supportedGameFeatures\":[\"ALLM\"],\"success\":true}")); 
}

TEST_F(HdmiInputDsTest, getHdmiGameFeatureStatusEmpty)
{
    EXPECT_EQ(Core::ERROR_GENERAL, handler.Invoke(connection, _T("getHdmiGameFeatureStatus"), _T("{}"), response));
    EXPECT_EQ(response, string(""));
}

TEST_F(HdmiInputDsTest, getHdmiGameFeatureStatus)
{
    ON_CALL(hdmiInputImplMock, getHdmiALLMStatus(::testing::_,::testing::_))
        .WillByDefault(::testing::Invoke(
            [&](int iport, bool *allm) {
                *allm = true;
            }));
    EXPECT_EQ(Core::ERROR_NONE, handler.Invoke(connection, _T("getHdmiGameFeatureStatus"), _T("{\"portId\": \"0\",\"gameFeature\": \"ALLM\"}"), response));
    EXPECT_EQ(response, string("{\"mode\":true,\"success\":true}")); 
}

TEST_F(HdmiInputInitializedEventDsTest, onDevicesChanged)
{
   ASSERT_TRUE(dsHdmiEventHandler != nullptr);
    ON_CALL(hdmiInputImplMock, getNumberOfInputs())
        .WillByDefault(::testing::Return(1));
    ON_CALL(hdmiInputImplMock, isPortConnected(::testing::_))
        .WillByDefault(::testing::Return(true));

    EXPECT_CALL(service, Submit(::testing::_, ::testing::_))
        .Times(1)
        .WillOnce(::testing::Invoke(
            [&](const uint32_t, const Core::ProxyType<Core::JSON::IElement>& json) {
                string text;
                EXPECT_TRUE(json->ToString(text));
                EXPECT_EQ(text, string(_T("{\"jsonrpc\":\"2.0\",\"method\":\"client.events.onDevicesChanged.onDevicesChanged\",\"params\":{\"devices\":[{\"id\":0,\"locator\":\"hdmiin:\\/\\/localhost\\/deviceid\\/0\",\"connected\":\"true\"}]}}")));

                return Core::ERROR_NONE;
            }));


    IARM_Bus_DSMgr_EventData_t eventData;
    eventData.data.hdmi_in_connect.port =dsHDMI_IN_PORT_0;
    eventData.data.hdmi_in_connect.isPortConnected = true;	
    handler.Subscribe(0, _T("onDevicesChanged"), _T("client.events.onDevicesChanged"), message);

    dsHdmiEventHandler(IARM_BUS_DSMGR_NAME, IARM_BUS_DSMGR_EVENT_HDMI_IN_HOTPLUG, &eventData , 0);

    handler.Unsubscribe(0, _T("onDevicesChanged"), _T("client.events.onDevicesChanged"), message); 
}
TEST_F(HdmiInputInitializedEventDsTest, onInputStatusChange)
{
   ASSERT_TRUE(dsHdmiStatusEventHandler != nullptr);

    EXPECT_CALL(service, Submit(::testing::_, ::testing::_))
        .Times(1)
        .WillOnce(::testing::Invoke(
            [&](const uint32_t, const Core::ProxyType<Core::JSON::IElement>& json) {
                string text;
                EXPECT_TRUE(json->ToString(text));
                EXPECT_EQ(text, string(_T("{\"jsonrpc\":\"2.0\",\"method\":\"client.events.onDevicesChanged.onDevicesChanged\",\"params\":{\"devices\":[{\"id\":0,\"locator\":\"hdmiin:\\/\\/localhost\\/deviceid\\/0\",\"connected\":\"true\"}]}}")));

                return Core::ERROR_NONE;
            }));


    IARM_Bus_DSMgr_EventData_t eventData;
    eventData.data.hdmi_in_status.port =dsHDMI_IN_PORT_0;
    eventData.data.hdmi_in_status.isPresented = true;	
    handler.Subscribe(0, _T("onDevicesChanged"), _T("client.events.onInputStatusChanged"), message);

    dsHdmiStatusEventHandler(IARM_BUS_DSMGR_NAME, IARM_BUS_DSMGR_EVENT_HDMI_IN_HOTPLUG, &eventData , 0);

    handler.Unsubscribe(0, _T("onDevicesChanged"), _T("client.events.onInputStatusChanged"), message); 
}

