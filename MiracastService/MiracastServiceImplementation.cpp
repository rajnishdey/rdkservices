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
#include <MiracastCore.h>
#include <unistd.h>

using namespace MIRACAST;

MiracastServiceImplementation *MiracastServiceImplementation::create(MiracastServiceNotifier *notifier)
{
    MIRACASTLOG_VERBOSE("Service created\n");

    return new MiracastServiceImplementation(notifier);
}

void MiracastServiceImplementation::Destroy(MiracastServiceImplementation *object)
{
    if (object)
    {
        delete object;
    }
}

MiracastServiceImplementation::MiracastServiceImplementation(MiracastServiceNotifier *notifier)
{
    MIRACASTLOG_VERBOSE("MiracastServiceImplementation::ctor...\n");
    m_miracastCore = MiracastCore::getInstance(notifier);
}

MiracastServiceImplementation::MiracastServiceImplementation()
{
}

MiracastServiceImplementation::~MiracastServiceImplementation()
{
    MIRACASTLOG_VERBOSE("Destructor...\n");
    MiracastCore::destroyInstance();
    m_miracastCore = nullptr;
}

void MiracastServiceImplementation::setFriendlyName(std::string friendly_name)
{
    m_miracastCore->setFriendlyName(friendly_name);
}

std::string MiracastServiceImplementation::getFriendlyName(void)
{
    return m_miracastCore->getFriendlyName();
}

MiracastError MiracastServiceImplementation::startStreaming()
{
    return m_miracastCore->startStreaming();
}

std::string MiracastServiceImplementation::getConnectedMAC()
{
    return m_miracastCore->getConnectedMAC();
}

std::vector<DeviceInfo *> MiracastServiceImplementation::getAllPeers()
{
    return m_miracastCore->getAllPeers();
}

DeviceInfo *MiracastServiceImplementation::getDeviceDetails(std::string MAC)
{
    return m_miracastCore->getDeviceDetails(MAC);
}

bool MiracastServiceImplementation::getConnectionStatus()
{
    return m_miracastCore->getConnectionStatus();
}

bool MiracastServiceImplementation::stopStreaming()
{
    return m_miracastCore->stopStreaming();
}

void MiracastServiceImplementation::Shutdown(void)
{
    m_miracastCore->SendMessageToClientReqHandler(MIRACAST_SERVICE_SHUTDOWN);
}

void MiracastServiceImplementation::setEnable(bool is_enabled)
{
    MIRACAST_SERVICE_STATES action;
 
    if ( true == is_enabled)
    {
        action = MIRACAST_SERVICE_WFD_START;
    }
    else
    {
        action = MIRACAST_SERVICE_WFD_STOP;
    }
    /*Check for polimorphism to default value in defination*/
    m_miracastCore->SendMessageToClientReqHandler(action);
}

void MiracastServiceImplementation::acceptClientConnectionRequest(std::string is_accepted)
{
    MIRACAST_SERVICE_STATES action;

    if ("Accept" == is_accepted)
    {
        MIRACASTLOG_VERBOSE("Client Connection Request accepted\n");
        action = MIRACAST_SERVICE_ACCEPT_CLIENT;
    }
    else
    {
        MIRACASTLOG_VERBOSE("Client Connection Request Rejected\n");
        action = MIRACAST_SERVICE_REJECT_CLIENT;
    }
    m_miracastCore->SendMessageToClientReqHandler(action);
}

bool MiracastServiceImplementation::StopClientConnection(std::string mac_address)
{
    if (0 != (mac_address.compare(m_miracastCore->getConnectedMAC())))
    {
        return false;
    }
    m_miracastCore->SendMessageToClientReqHandler(MIRACAST_SERVICE_STOP_CLIENT_CONNECTION, mac_address);
    return true;
}