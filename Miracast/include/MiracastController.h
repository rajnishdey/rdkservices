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

#ifndef _MIRACAST_CONTROLLER_H_
#define _MIRACAST_CONTROLLER_H_

#include <string>
#include <sstream>
#include <fcntl.h>
#include <algorithm>
#include <vector>
#include <glib.h>
#include <semaphore.h>
#include "libIBus.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fstream>
#include <MiracastCommon.h>
#include "MiracastP2P.h"
#include "MiracastLogger.h"
#include "MiracastRtspMsg.h"
//#include "wifiSrvMgrIarmIf.h"

using namespace std;
using namespace MIRACAST;

#define THUNDER_REQ_THREAD_CLIENT_CONNECTION_WAITTIME (30)

class MiracastController
{
public:
    static MiracastController *getInstance( MiracastError &error_code , 
                                            MiracastServiceNotifier *notifier = nullptr);
    static void destroyInstance();

    void event_handler(P2P_EVENTS eventId, void *data, size_t len, bool isIARMEnabled = false);

    MiracastError discover_devices();
    MiracastError connect_device(std::string mac);
    MiracastError start_streaming();

    std::string get_localIp();
    std::string get_wfd_streaming_port_number();
    std::string get_connected_device_mac();
    std::vector<DeviceInfo *> get_allPeers();

    bool get_connection_status();
    DeviceInfo *get_device_details(std::string mac);

    MiracastError stop_streaming(eCONTROLLER_FW_STATES state = CONTROLLER_STOP_STREAMING);
    MiracastError disconnect_device();
    void send_msg_thunder_msg_hdler_thread(MIRACAST_SERVICE_STATES state, std::string buffer = "", std::string user_data = "");
    void send_msg_rtsp_msg_hdler_thread(eCONTROLLER_FW_STATES state);
    void send_msgto_test_notifier_thread(uint32_t state);

    void Controller_Thread(void *args);
    void ThunderReqHandler_Thread(void *args);
    void TestNotifier_Thread(void *args);
    // void HDCPTCPServerHandlerThread(void *args);
    // void DumpBuffer(char *buffer, int length);

    MiracastError stop_discover_devices();
    MiracastError set_WFDParameters(void);
    void restart_session_discovery(void );
    void restart_session(bool start_discovering_enabled);
    void stop_session(bool stop_streaming_needed = false);
    std::string get_device_name(std::string mac);
    MiracastError set_FriendlyName(std::string friendly_name , bool apply = false);
    std::string get_FriendlyName(void);
    void set_enable(bool is_enabled);
    void accept_client_connection(std::string is_accepted);
    bool stop_client_connection(std::string mac_address);
    bool set_WFDVideoFormat( RTSP_WFD_VIDEO_FMT_STRUCT video_fmt );
    bool set_WFDAudioCodecs( RTSP_WFD_AUDIO_FMT_STRUCT st_audio_fmt );
    eMIRA_PLAYER_STATES m_ePlayer_state;

    void set_WFDSourceMACAddress(std::string MAC_Addr);
    void set_WFDSourceName(std::string device_name);
    std::string get_WFDSourceName(void);
    std::string get_WFDSourceMACAddress(void);
    void reset_WFDSourceMACAddress(void);
    void reset_WFDSourceName(void);

private:
    static MiracastController *m_miracast_ctrl_obj;
    MiracastController();
    virtual ~MiracastController();
    MiracastController &operator=(const MiracastController &) = delete;
    MiracastController(const MiracastController &) = delete;

    std::string parse_p2p_event_data(const char *tmpBuff, const char *lookup_data );
    std::string start_DHCPClient(std::string interface, std::string &default_gw_ip_addr);
    MiracastError initiate_TCP(std::string go_ip);
    MiracastError connect_Sink();
    MiracastError create_ControllerFramework(void);
    MiracastError destroy_ControllerFramework(void);

    void set_localIp(std::string ipAddr);

    MiracastServiceNotifier *m_notify_handler;
    std::string m_connected_mac_addr;
    std::string m_connected_device_name;
    std::string m_localIp;
    vector<DeviceInfo *> m_deviceInfoList;
    GroupInfo *m_groupInfo;
    bool m_connectionStatus;
    // int m_hdcptcpSockfd;

    /*members for interacting with wpa_supplicant*/
    MiracastP2P *m_p2p_ctrl_obj;

    MiracastRTSPMsg *m_rtsp_msg;
    MiracastThread *m_thunder_req_handler_thread;
    MiracastThread *m_controller_thread;
    //MiracastThread *m_hdcp_handler_thread;
    int m_tcpserverSockfd;
#ifdef ENABLE_TEST_NOTIFIER
    MiracastThread  *m_test_notifier_thread;
#endif
    eCONTROLLER_FW_STATES convertP2PtoSessionActions(P2P_EVENTS eventId);
    MiracastError start_DHCPServer(std::string interface);
};

#endif