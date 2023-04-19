/**
 * If not stated otherwise in this file or this component's LICENSE
 * file the following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
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
 **/

#pragma once

#include <mutex>
#include <vector>

#include "Module.h"
#include <securityagent/SecurityTokenUtil.h>
#include "MiracastServiceImplementation.h"
#include "libIARM.h"

using std::vector;
namespace WPEFramework
{
    namespace Plugin
    {
        // This is a server for a JSONRPC communication channel.
        // For a plugin to be capable to handle JSONRPC, inherit from PluginHost::JSONRPC.
        // By inheriting from this class, the plugin realizes the interface PluginHost::IDispatcher.
        // This realization of this interface implements, by default, the following methods on this plugin
        // - exists
        // - register
        // - unregister
        // Any other methood to be handled by this plugin  can be added can be added by using the
        // templated methods Register on the PluginHost::JSONRPC class.
        // As the registration/unregistration of notifications is realized by the class PluginHost::JSONRPC,
        // this class exposes a public method called, Notify(), using this methods, all subscribed clients
        // will receive a JSONRPC message as a notification, in case this method is called.
        class MiracastService : public PluginHost::IPlugin, public PluginHost::JSONRPC, public MiracastServiceNotifier
        {
        private:
            // We do not allow this plugin to be copied !!
            MiracastService(const MiracastService &) = delete;
            MiracastService &operator=(const MiracastService &) = delete;

        public:
            MiracastService();
            virtual ~MiracastService();
            virtual const string Initialize(PluginHost::IShell *shell) override;
            virtual void Deinitialize(PluginHost::IShell *service) override;
            virtual string Information() const override;

            virtual void onMiracastServiceClientConnectionRequest(string client_mac, string client_name) override;
            virtual void onMiracastServiceClientStopRequest(string client_mac, string client_name) override;
            virtual void onMiracastServiceClientConnectionStarted(string client_mac, string client_name) override;
            virtual void onMiracastServiceClientConnectionError(string client_mac, string client_name) override;

            BEGIN_INTERFACE_MAP(MiracastService)
            INTERFACE_ENTRY(PluginHost::IPlugin)
            INTERFACE_ENTRY(PluginHost::IDispatcher)
            END_INTERFACE_MAP
        public:
            // constants
            static const short API_VERSION_NUMBER_MAJOR;
            static const short API_VERSION_NUMBER_MINOR;
            static const string SERVICE_NAME;

            // methods
            static const string METHOD_MIRACAST_SET_ENABLE;
            static const string METHOD_MIRACAST_GET_ENABLE;
            static const string METHOD_MIRACAST_CLIENT_CONNECT_REQUEST;
            static const string METHOD_MIRACAST_STOP_CLIENT_CONNECT;

        private:
            bool m_isPlatInitialized;
            bool m_isDiscoverEnabled;
            WPEFramework::JSONRPC::LinkType<WPEFramework::Core::JSON::IElement> *remoteObjectXCast = NULL;
            uint32_t setEnable(const JsonObject &parameters, JsonObject &response);
            uint32_t getEnable(const JsonObject &parameters, JsonObject &response);
            uint32_t acceptClientConnectionRequest(const JsonObject &parameters, JsonObject &response);
            uint32_t stopClientConnection(const JsonObject &parameters, JsonObject &response);
            int get_XCastFriendlyName(std::string &friendlyname);

        public:
            PluginHost::IShell *mCurrentService;
            static MiracastService *_instance;
            static MiracastServiceImplementation *m_miracast_service_impl;
        };
    } // namespace Plugin
} // namespace WPEFramework
