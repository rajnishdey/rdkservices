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

#include <MiracastServiceImplementation.h>
#include <MiracastController.h>
#include <unistd.h>

using namespace MIRACAST;

MiracastServiceImplementation *MiracastServiceImplementation::create(MiracastServiceNotifier *notifier)
{
    MIRACASTLOG_VERBOSE("Service created\n");

    return new MiracastServiceImplementation(notifier);
}

void MiracastServiceImplementation::Destroy(MiracastServiceImplementation *object)
{
    MIRACASTLOG_TRACE("Entering...");
    if (object)
    {
        delete object;
    }
    MIRACASTLOG_TRACE("Exiting...");
}

MiracastServiceImplementation::MiracastServiceImplementation(MiracastServiceNotifier *notifier)
{
    MIRACASTLOG_TRACE("Entering...");
    m_miracast_obj = MiracastController::getInstance(notifier);
    MIRACASTLOG_TRACE("Exiting...");
}

MiracastServiceImplementation::MiracastServiceImplementation()
{

}

MiracastServiceImplementation::~MiracastServiceImplementation()
{
    MIRACASTLOG_TRACE("Entering...");
    MiracastController::destroyInstance();
    m_miracast_obj = nullptr;
    MIRACASTLOG_TRACE("Exiting...");
}

void MiracastServiceImplementation::setFriendlyName(std::string friendly_name)
{
    MIRACASTLOG_TRACE("Entering...");
    m_miracast_obj->set_FriendlyName(friendly_name);
    MIRACASTLOG_TRACE("Exiting...");
}

std::string MiracastServiceImplementation::getFriendlyName(void)
{
    return m_miracast_obj->get_FriendlyName();
}

MiracastError MiracastServiceImplementation::startStreaming()
{
    return m_miracast_obj->start_streaming();
}

std::string MiracastServiceImplementation::getConnectedMAC()
{
    return m_miracast_obj->get_connected_device_mac();
}

std::vector<DeviceInfo *> MiracastServiceImplementation::getAllPeers()
{
    return m_miracast_obj->get_allPeers();
}

DeviceInfo *MiracastServiceImplementation::getDeviceDetails(std::string MAC)
{
    return m_miracast_obj->get_device_details(MAC);
}

bool MiracastServiceImplementation::getConnectionStatus()
{
    return m_miracast_obj->get_connection_status();
}

bool MiracastServiceImplementation::stopStreaming()
{
    return m_miracast_obj->stop_streaming();
}

void MiracastServiceImplementation::Shutdown(void)
{
    MIRACASTLOG_TRACE("Entering...");
    m_miracast_obj->send_msg_thunder_msg_hdler_thread(MIRACAST_SERVICE_SHUTDOWN);
    MIRACASTLOG_TRACE("Exiting...");
}

void MiracastServiceImplementation::setEnable(bool is_enabled)
{
    size_t action;
    MIRACASTLOG_TRACE("Entering...");

    if ( true == is_enabled)
    {
        action = MIRACAST_SERVICE_WFD_START;
    }
    else
    {
        action = MIRACAST_SERVICE_WFD_STOP;
    }
    m_miracast_obj->send_msg_thunder_msg_hdler_thread(action);
    MIRACASTLOG_TRACE("Exiting...");
}

void MiracastServiceImplementation::acceptClientConnectionRequest(std::string is_accepted)
{
    MIRACAST_SERVICE_STATES state;

    MIRACASTLOG_TRACE("Entering...");

    if ("Accept" == is_accepted)
    {
        MIRACASTLOG_VERBOSE("Client Connection Request accepted\n");
        state = MIRACAST_SERVICE_ACCEPT_CLIENT;
    }
    else
    {
        MIRACASTLOG_VERBOSE("Client Connection Request Rejected\n");
        state = MIRACAST_SERVICE_REJECT_CLIENT;
    }
    m_miracast_obj->send_msg_thunder_msg_hdler_thread(state);
    MIRACASTLOG_TRACE("Exiting...");
}

bool MiracastServiceImplementation::StopClientConnection(std::string mac_address)
{
    MIRACASTLOG_TRACE("Entering...");

    if (0 != (mac_address.compare(m_miracast_obj->get_connected_device_mac())))
    {
        MIRACASTLOG_TRACE("Exiting...");
        return false;
    }
    m_miracast_obj->send_msg_thunder_msg_hdler_thread(MIRACAST_SERVICE_STOP_CLIENT_CONNECTION, mac_address);
    MIRACASTLOG_TRACE("Exiting...");
    return true;
}