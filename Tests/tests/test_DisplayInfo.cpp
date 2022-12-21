#include <gtest/gtest.h>

#include "DisplayInfo.h"
#if 0

#include <interfaces/IDisplayInfo.h>
#include "FactoriesImplementation.h"
#include "IarmBusMock.h"
#include "ServiceMock.h"


#include "host.hpp"
#include "exception.hpp"
#include "VideoOutputPortMock.h"
#include "VideoOutputPortTypeMock.h"
#include "VideoOutputPortConfigMock.h"
#include "VideoResolutionMock.h"
#include "AudioOutputPortMock.h"
#include "AudioOutputPortTypeMock.h"
#include "AudioOutputPortConfigMock.h"


#include "ManagerMock.h"

#include "edid-parserMock.h"
#include "UtilsIarm.h"

#include "libIBus.h"
#include "libIBusDaemon.h"
#include "dsMgr.h"


using namespace WPEFramework;

namespace {
const string webPrefix = _T("/Service/DisplayInfo");
}

class DisplayInfoTest : public ::testing::Test {
protected:
    Core::ProxyType<Plugin::DisplayInfo> plugin;
    Core::JSONRPC::Handler& handler;
    Core::JSONRPC::Connection connection;
    string response;
    //Exchange::JsonData_DisplayInfo* interface;


    DisplayInfoTest()
        : plugin(Core::ProxyType<Plugin::DisplayInfo>::Create())
        , handler(*plugin)
        , connection(1, 0)
    {
    }
    virtual ~DisplayInfoTest() = default;
};


class DisplayInfoInitializedTest : public DisplayInfoTest {
protected:
    IarmBusImplMock iarmBusImplMock;
    ManagerImplMock managerImplMock;
    ServiceMock service;

    DisplayInfoInitializedTest()
        : DisplayInfoTest()
    {
        IarmBus::getInstance().impl = &iarmBusImplMock;
        device::Manager::getInstance().impl = &managerImplMock;

        EXPECT_EQ(string(""), plugin->Initialize(&service));
    }
    virtual ~DisplayInfoInitializedTest() override
    {
        plugin->Deinitialize(&service);

        IarmBus::getInstance().impl = nullptr;
        device::Manager::getInstance().impl = nullptr;
    }
};
#endif
