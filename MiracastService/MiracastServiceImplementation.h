/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2023 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _MIRACAST_SERVICE_H_
#define _MIRACAST_SERVICE_H_

#include <MiracastCommon.h>
#include <string>
#include <vector>

using namespace std;
using namespace MIRACAST;

class MiracastController;

class MiracastServiceImplementation
{
public:
    static MiracastServiceImplementation *create(MiracastServiceNotifier *notifier);
    static void Destroy(MiracastServiceImplementation *object);
    void setEnable(bool is_enabled);
    void acceptClientConnectionRequest(std::string is_accepted);
    bool StopClientConnection(std::string mac_address);

    void setFriendlyName(std::string friendly_name);
    std::string getFriendlyName(void);

    void Shutdown(void);
    // Global APIs
    MiracastError startStreaming();

    // APIs to request for device/connection related details
    std::string getConnectedMAC();
    std::vector<DeviceInfo *> getAllPeers();
    bool getConnectionStatus();
    DeviceInfo *getDeviceDetails(std::string MAC);

    // APIs to disconnect
    bool stopStreaming();
    bool disconnectDevice(); // @TODO: Remove if not needed

private:
    MiracastServiceImplementation(MiracastServiceNotifier *notifier);
    MiracastServiceImplementation();
    MiracastServiceImplementation(MiracastServiceImplementation &);
    ~MiracastServiceImplementation();
    MiracastController *m_miracast_obj;
};

#endif