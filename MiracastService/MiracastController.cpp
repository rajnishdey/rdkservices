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

#include "MiracastController.h"
#include "MiracastPlayer.h"

void ThunderReqHandlerCallback(void *args);
void ControllerThreadCallback(void *args);

MiracastController *MiracastController::m_miracast_ctrl_obj{nullptr};

MiracastThread::MiracastThread(std::string thread_name, size_t stack_size, size_t msg_size, size_t queue_depth, void (*callback)(void *), void *user_data)
{
    MIRACASTLOG_TRACE("Entering...");
    m_thread_name = thread_name;

    m_thread_stacksize = stack_size;
    m_thread_message_size = msg_size;
    m_thread_message_count = queue_depth;

    m_thread_user_data = user_data;

    m_thread_callback = callback;

    // Create message queue
    m_g_queue = g_async_queue_new();
    // g_async_queue_ref( g_queue );

    sem_init(&m_sem_object, 0, 0);

    // Create thread
    pthread_attr_init(&m_pthread_attr);
    pthread_attr_setstacksize(&m_pthread_attr, m_thread_stacksize);
    MIRACASTLOG_TRACE("Exiting...");
}

MiracastThread::~MiracastThread()
{
    MIRACASTLOG_TRACE("Entering...");
    // Join thread
    pthread_join(m_pthread_id, nullptr);

    // Close message queue
    g_async_queue_unref(m_g_queue);
    MIRACASTLOG_TRACE("Exiting...");
}

void MiracastThread::start(void)
{
    MIRACASTLOG_TRACE("Entering...");
    pthread_create(&m_pthread_id, &m_pthread_attr, reinterpret_cast<void *(*)(void *)>(m_thread_callback), m_thread_user_data);
    MIRACASTLOG_TRACE("Exiting...");
}

void MiracastThread::send_message(void *message, size_t msg_size)
{
    MIRACASTLOG_TRACE("Entering...");
    void *buffer = malloc(msg_size);
    if (nullptr == buffer)
    {
        MIRACASTLOG_ERROR("Memory Allocation Failed for %u\n", msg_size);
        return;
    }
    memset(buffer, 0x00, msg_size);
    // Send message to queue

    memcpy(buffer, message, msg_size);
    g_async_queue_push(m_g_queue, buffer);
    sem_post(&m_sem_object);
    MIRACASTLOG_TRACE("Exiting...");
}

int8_t MiracastThread::receive_message(void *message, size_t msg_size, int sem_wait_timedout)
{
    int8_t status = false;
    MIRACASTLOG_TRACE("Entering...");
    if (THREAD_RECV_MSG_INDEFINITE_WAIT == sem_wait_timedout)
    {
        sem_wait(&m_sem_object);
        status = true;
    }
    else if (0 < sem_wait_timedout)
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += sem_wait_timedout;

        if (-1 != sem_timedwait(&m_sem_object, &ts))
        {
            status = true;
        }
    }
    else
    {
        status = -1;
    }

    if ((true == status) && (nullptr != m_g_queue))
    {
        void *data_ptr = static_cast<void *>(g_async_queue_pop(m_g_queue));
        if ((nullptr != message) && (nullptr != data_ptr))
        {
            memcpy(message, data_ptr, msg_size);
            free(data_ptr);
        }
    }
    MIRACASTLOG_TRACE("Exiting...");
    return status;
}

MiracastController *MiracastController::getInstance(MiracastServiceNotifier *notifier)
{
    MIRACASTLOG_TRACE("Entering...");
    if (nullptr == m_miracast_ctrl_obj)
    {
        m_miracast_ctrl_obj = new MiracastController();
        if (nullptr != m_miracast_ctrl_obj)
        {
            m_miracast_ctrl_obj->m_notify_handler = notifier;
            m_miracast_ctrl_obj->create_ControllerFramework();
        }
    }
    MIRACASTLOG_TRACE("Exiting...");
    return m_miracast_ctrl_obj;
}

void MiracastController::destroyInstance()
{
    MIRACASTLOG_TRACE("Entering...");
    if (nullptr != m_miracast_ctrl_obj)
    {
        m_miracast_ctrl_obj->destroy_ControllerFramework();
        delete m_miracast_ctrl_obj;
        m_miracast_ctrl_obj = nullptr;
    }
    MIRACASTLOG_TRACE("Exiting...");
}

MiracastController::MiracastController(void)
{
    MIRACASTLOG_TRACE("Entering...");
    // To delete the rules so that it would be duplicated in this program execution
    system("iptables -D INPUT -p udp -s 192.168.0.0/16 --dport 1990 -j ACCEPT");
    system("iptables -D INPUT -p tcp -s 192.168.0.0/16 --dport 7236 -j ACCEPT");
    system("iptables -D OUTPUT -p tcp -s 192.168.0.0/16 --dport 7236 -j ACCEPT");

    m_groupInfo = nullptr;
    MIRACASTLOG_TRACE("Exiting...");
}

MiracastController::~MiracastController()
{
    MIRACASTLOG_TRACE("Entering...");

    while (!m_deviceInfoList.empty())
    {
        delete m_deviceInfoList.back();
        m_deviceInfoList.pop_back();
    }

    if (nullptr != m_groupInfo)
    {
        delete m_groupInfo;
        m_groupInfo = nullptr;
    }

    /*@TODO: Check on ACCEPT or REJECT. (p2)*/
    system("iptables -D INPUT -p udp -s 192.168.0.0/16 --dport 1990 -j ACCEPT");
    system("iptables -D INPUT -p tcp -s 192.168.0.0/16 --dport 7236 -j ACCEPT");
    system("iptables -D OUTPUT -p tcp -s 192.168.0.0/16 --dport 7236 -j ACCEPT");
    MIRACASTLOG_TRACE("Exiting...");
}

void MiracastController::create_ControllerFramework(void)
{
    MIRACASTLOG_TRACE("Entering...");

    m_thunder_req_handler_thread = new MiracastThread(THUNDER_REQ_HANDLER_THREAD_NAME, 
                                                        THUNDER_REQ_HANDLER_THREAD_STACK, 
                                                        THUNDER_REQ_HANDLER_MSGQ_COUNT, 
                                                        THUNDER_REQ_HANDLER_MSGQ_SIZE, 
                                                        reinterpret_cast<void (*)(void *)>(&ThunderReqHandlerCallback), 
                                                        this);
    m_thunder_req_handler_thread->start();

    m_controller_thread = new MiracastThread(CONTROLLER_THREAD_NAME, 
                                            CONTROLLER_THREAD_STACK, 
                                            CONTROLLER_MSGQ_COUNT, 
                                            CONTROLLER_MSGQ_SIZE, 
                                            reinterpret_cast<void (*)(void *)>(&ControllerThreadCallback), 
                                            this);
    m_controller_thread->start();

    m_rtsp_msg = MiracastRTSPMsg::getInstance(m_controller_thread);

    m_p2p_ctrl_obj = MiracastP2P::getInstance();
    MIRACASTLOG_TRACE("Exiting...");
}

void MiracastController::destroy_ControllerFramework(void)
{
    MIRACASTLOG_TRACE("Entering...");

    send_msg_thunder_msg_hdler_thread(MIRACAST_SERVICE_SHUTDOWN);

    MiracastP2P::destroyInstance();
    m_p2p_ctrl_obj = nullptr;

    delete m_controller_thread;
    delete m_thunder_req_handler_thread;
    MiracastRTSPMsg::destroyInstance();
    m_rtsp_msg = nullptr;

    m_controller_thread = nullptr;
    m_thunder_req_handler_thread = nullptr;
    MIRACASTLOG_TRACE("Exiting...");
}

std::string MiracastController::parse_p2p_event_data(const char *tmpBuff, const char *lookup_data)
{
    char return_buf[1024] = {0};
    const char *ret = nullptr, *ret_equal = nullptr, *ret_space = nullptr;
    ret = strstr(tmpBuff, lookup_data);
    if (nullptr != ret)
    {
        if (0 == strncmp(ret, lookup_data, strlen(lookup_data)))
        {
            ret_equal = strstr(ret, "=");
            ret_space = strstr(ret_equal, " ");
            if (ret_space)
            {
                snprintf(return_buf, (int)(ret_space - ret_equal), "%s", ret + strlen(lookup_data) + 1);
                MIRACASTLOG_VERBOSE("Parsed Data is - %s", return_buf);
            }
            else
            {
                snprintf(return_buf, strlen(ret_equal - 1), "%s", ret + strlen(lookup_data) + 1);
                MIRACASTLOG_VERBOSE("Parsed Data is - %s", return_buf);
            }
        }
    }
    if (return_buf != nullptr)
        return std::string(return_buf);
    else
        return std::string(" ");
}

std::string MiracastController::start_DHCPClient(std::string interface, std::string &default_gw_ip_addr)
{
    MIRACASTLOG_TRACE("Entering...");
    FILE *fp;
    std::string IP;
    char command[128] = {0};
    char data[1024] = {0};
    size_t len = 0;
    char *ln = nullptr;
    std::string local_addr = "";

    sprintf(command, "/sbin/udhcpc -v -i ");
    sprintf(command + strlen(command), interface.c_str());
    sprintf(command + strlen(command), " 2>&1");

    MIRACASTLOG_VERBOSE("command : [%s]", command);

    fp = popen(command, "r");

    if (!fp)
    {
        MIRACASTLOG_ERROR("Could not open pipe for output.");
    }
    else
    {
        while (getline(&ln, &len, fp) != -1)
        {
            sprintf(data + strlen(data), ln);
            MIRACASTLOG_VERBOSE("data : [%s]", data);
        }
        pclose(fp);

        std::string popen_buffer = data;

        MIRACASTLOG_VERBOSE("popen_buffer is %s\n", popen_buffer.c_str());

        std::string leaseof_str = "lease of ";
        std::string gw_str = "route add default gw ";

        std::size_t local_ip_pos = popen_buffer.find(leaseof_str.c_str()) + leaseof_str.length();
        local_addr = popen_buffer.substr(local_ip_pos, popen_buffer.find(" obtained") - local_ip_pos);
        MIRACASTLOG_VERBOSE("local IP addr obtained is %s\n", local_addr.c_str());

        /* Here retrieved the default gw ip address. Later it can be used as GO IP address if P2P-GROUP started as PERSISTENT */
        std::size_t gw_pos = popen_buffer.find(gw_str.c_str()) + gw_str.length();
        default_gw_ip_addr = popen_buffer.substr(gw_pos, popen_buffer.find(" dev") - gw_pos);
        MIRACASTLOG_VERBOSE("default_gw_ip_addr obtained is %s\n", default_gw_ip_addr.c_str());

        free(ln);
    }
    MIRACASTLOG_TRACE("Exiting...");
    return local_addr;
}

MiracastError MiracastController::initiate_TCP(std::string goIP)
{
    MIRACASTLOG_TRACE("Entering...");
    MiracastError ret = MIRACAST_FAIL;
    ret = m_rtsp_msg->initiate_TCP(goIP);
    MIRACASTLOG_TRACE("Exiting...");
    return ret;
}

eCONTROLLER_FW_STATES MiracastController::convertP2PtoSessionActions(P2P_EVENTS eventId)
{
    eCONTROLLER_FW_STATES state = CONTROLLER_INVALID_STATE;

    switch (eventId)
    {
    case EVENT_FOUND:
    {
        state = CONTROLLER_GO_DEVICE_FOUND;
    }
    break;
    case EVENT_PROVISION:
    {
        state = CONTROLLER_GO_DEVICE_PROVISION;
    }
    break;
    case EVENT_STOP:
    {
        state = CONTROLLER_GO_STOP_FIND;
    }
    break;
    case EVENT_GO_NEG_REQ:
    {
        state = CONTROLLER_GO_NEG_REQUEST;
    }
    break;
    case EVENT_GO_NEG_SUCCESS:
    {
        state = CONTROLLER_GO_NEG_SUCCESS;
    }
    break;
    case EVENT_GO_NEG_FAILURE:
    {
        state = CONTROLLER_GO_NEG_FAILURE;
    }
    break;
    case EVENT_GROUP_STARTED:
    {
        state = CONTROLLER_GO_GROUP_STARTED;
    }
    break;
    case EVENT_FORMATION_SUCCESS:
    {
        state = CONTROLLER_GO_GROUP_FORMATION_SUCCESS;
    }
    break;
    case EVENT_FORMATION_FAILURE:
    {
        state = CONTROLLER_GO_GROUP_FORMATION_FAILURE;
    }
    break;
    case EVENT_DEVICE_LOST:
    {
        state = CONTROLLER_GO_DEVICE_LOST;
    }
    break;
    case EVENT_GROUP_REMOVED:
    {
        state = CONTROLLER_GO_GROUP_REMOVED;
    }
    break;
    case EVENT_ERROR:
    {
        state = CONTROLLER_GO_EVENT_ERROR;
    }
    break;
    default:
    {
        state = CONTROLLER_GO_UNKNOWN_EVENT;
    }
    break;
    }
    return state;
}

void MiracastController::restart_session(bool start_discovering_enabled)
{
    MIRACASTLOG_TRACE("Entering...");
    stop_session();
    if (start_discovering_enabled){
        discover_devices();
    }
    MIRACASTLOG_TRACE("Exiting...");
}

void MiracastController::stop_session(bool stop_streaming_needed)
{
    MIRACASTLOG_TRACE("Entering...");

    if (true == stop_streaming_needed){
        stop_streaming(CONTROLLER_TEARDOWN_REQ_FROM_THUNDER);
    }

    stop_discover_devices();
    if (m_groupInfo)
    {
        delete m_groupInfo;
        m_groupInfo = nullptr;
    }
    MIRACASTLOG_TRACE("Exiting...");
}

void MiracastController::event_handler(P2P_EVENTS eventId, void *data, size_t len, bool isIARMEnabled)
{
    CONTROLLER_MSGQ_STRUCT controller_msgq_data = {0};
    std::string event_buffer;
    MIRACASTLOG_VERBOSE("event received");
    if (isIARMEnabled)
    {
        IARM_BUS_WiFiSrvMgr_P2P_EventData_t *EventData = (IARM_BUS_WiFiSrvMgr_P2P_EventData_t *)data;
        event_buffer = EventData->event_data;
    }
    else
    {
        event_buffer = (char *)data;
        free(data);
    }

    controller_msgq_data.msg_type = P2P_MSG;
    controller_msgq_data.state = convertP2PtoSessionActions(eventId);
    strcpy(controller_msgq_data.msg_buffer, event_buffer.c_str());

    MIRACASTLOG_INFO("event_handler to Controller Action[%#08X] buffer:%s  ", controller_msgq_data.state, event_buffer.c_str());
    m_controller_thread->send_message(&controller_msgq_data, sizeof(controller_msgq_data));
    MIRACASTLOG_VERBOSE("event received : %d buffer:%s  ", eventId, event_buffer.c_str());
}

MiracastError MiracastController::set_WFDParameters(void)
{
    MIRACASTLOG_TRACE("Entering...");
    MiracastError ret = MIRACAST_FAIL;
    ret = m_p2p_ctrl_obj->set_WFDParameters();
    MIRACASTLOG_TRACE("Exiting...");
    return ret;
}

MiracastError MiracastController::discover_devices(void)
{
    MIRACASTLOG_TRACE("Entering...");
    MiracastError ret = MIRACAST_FAIL;
    ret = m_p2p_ctrl_obj->discover_devices();
    MIRACASTLOG_TRACE("Exiting...");
    return ret;
}

MiracastError MiracastController::stop_discover_devices(void)
{
    MIRACASTLOG_TRACE("Entering...");
    MiracastError ret = MIRACAST_FAIL;
    ret = m_p2p_ctrl_obj->stop_discover_devices();
    MIRACASTLOG_TRACE("Exiting...");
    return ret;
}

MiracastError MiracastController::connect_device(std::string MAC)
{
    MIRACASTLOG_TRACE("Entering...");
    MIRACASTLOG_VERBOSE("Connecting to the MAC - %s", MAC.c_str());
    MiracastError ret = MIRACAST_FAIL;
    ret = m_p2p_ctrl_obj->connect_device(MAC);
    MIRACASTLOG_TRACE("Exiting...");
    return ret;
}

MiracastError MiracastController::start_streaming()
{
    MIRACASTLOG_TRACE("Entering...");
    const char *mcastIptableFile = "/opt/mcast_iptable.txt";
    std::ifstream mIpfile(mcastIptableFile);
    std::string mcast_iptable;
    if (mIpfile.is_open())
    {
        std::getline(mIpfile, mcast_iptable);
        MIRACASTLOG_INFO("Iptable reading from file [%s] as [ %s] ", mcastIptableFile, mcast_iptable.c_str());
        system(mcast_iptable.c_str());
        mIpfile.close();
    }
    else
    {
        system("iptables -I INPUT -p udp -s 192.168.0.0/16 --dport 1990 -j ACCEPT");
    }
    MIRACASTLOG_INFO("Casting started. Player initiated");
    std::string gstreamerPipeline;

    const char *mcastfile = "/opt/mcastgstpipline.txt";
    std::ifstream mcgstfile(mcastfile);

    if (mcgstfile.is_open())
    {
        std::getline(mcgstfile, gstreamerPipeline);
        MIRACASTLOG_INFO("gstpipeline reading from file [%s], gstreamerPipeline as [ %s] ", mcastfile, gstreamerPipeline.c_str());
        mcgstfile.close();
        if (0 == system(gstreamerPipeline.c_str()))
            MIRACASTLOG_INFO("Pipeline created successfully ");
        else
        {
            MIRACASTLOG_INFO("Pipeline creation failure");
            return MIRACAST_FAIL;
        }
    }
    else
    {
        if (access("/opt/miracast_gst", F_OK) == 0)
        {
            gstreamerPipeline = "GST_DEBUG=3 gst-launch-1.0 -vvv playbin uri=udp://0.0.0.0:1990 video-sink=\"westerossink\"";
            MIRACASTLOG_INFO("pipeline constructed is --> %s", gstreamerPipeline.c_str());
            if (0 == system(gstreamerPipeline.c_str()))
                MIRACASTLOG_INFO("Pipeline created successfully ");
            else
            {
                MIRACASTLOG_INFO("Pipeline creation failure");
                return MIRACAST_FAIL;
            }
        }
        else
        {
            MiracastPlayer *miracastPlayerObj = MiracastPlayer::getInstance();
            std::string port = get_wfd_streaming_port_number();
            std::string local_ip = get_localIp();
            miracastPlayerObj->launch(local_ip, port);
        }
    }

    std::string MAC = m_rtsp_msg->get_WFDSourceMACAddress();
    std::string device_name = m_rtsp_msg->get_WFDSourceName();

    m_notify_handler->onMiracastServiceClientConnectionStarted(MAC, device_name);
    MIRACASTLOG_TRACE("Exiting...");
    return MIRACAST_OK;
}

MiracastError MiracastController::stop_streaming(eCONTROLLER_FW_STATES state )
{
    MIRACASTLOG_TRACE("Entering...");

    system("iptables -D INPUT -p udp -s 192.168.0.0/16 --dport 1990 -j ACCEPT");

    if ((CONTROLLER_SELF_ABORT == state)||
        (CONTROLLER_TEARDOWN_REQ_FROM_THUNDER == state)||
        (CONTROLLER_STOP_STREAMING == state))
    {
        if (CONTROLLER_STOP_STREAMING != state){
            eCONTROLLER_FW_STATES rtsp_state = RTSP_TEARDOWN_FROM_SINK2SRC;

            if (CONTROLLER_SELF_ABORT == state){
                rtsp_state = RTSP_SELF_ABORT;
            }
            send_msg_rtsp_msg_hdler_thread(rtsp_state);
        }
        if (!get_connected_device_mac().empty())
        {
            if (CONTROLLER_SELF_ABORT == state)
            {
                MiracastPlayer::destroyInstance();
            }
            else
            {
                MiracastPlayer *miracastPlayerObj = MiracastPlayer::getInstance();
                miracastPlayerObj->stop();
            }
        }
    }
    MIRACASTLOG_TRACE("Exiting...");
    return MIRACAST_OK;
}

MiracastError MiracastController::disconnect_device()
{
    MIRACASTLOG_TRACE("Entering...");
    stop_streaming(CONTROLLER_TEARDOWN_REQ_FROM_THUNDER);
    MIRACASTLOG_TRACE("Exiting...");
    return MIRACAST_OK;
}

std::string MiracastController::get_localIp()
{
    MIRACASTLOG_TRACE("Entering...");
    std::string ip_addr = "";
    if (nullptr != m_groupInfo)
    {
        ip_addr = m_groupInfo->localIPAddr;
    }
    MIRACASTLOG_TRACE("Exiting...");
    return ip_addr;
}

std::string MiracastController::get_wfd_streaming_port_number()
{
    return m_rtsp_msg->get_WFDStreamingPortNumber();
}

std::string MiracastController::get_connected_device_mac()
{
    return m_rtsp_msg->get_WFDSourceMACAddress();
}

std::vector<DeviceInfo *> MiracastController::get_allPeers()
{
    return m_deviceInfoList;
}

bool MiracastController::get_connection_status()
{
    return m_connectionStatus;
}

DeviceInfo *MiracastController::get_device_details(std::string MAC)
{
    DeviceInfo *deviceInfo = nullptr;
    std::size_t found;
    MIRACASTLOG_TRACE("Entering...");
    for (auto device : m_deviceInfoList)
    {
        found = device->deviceMAC.find(MAC);
        if (found != std::string::npos)
        {
            deviceInfo = device;
            break;
        }
    }
    MIRACASTLOG_TRACE("Exiting...");
    return deviceInfo;
}

std::string MiracastController::get_device_name(std::string mac_address)
{
    MIRACASTLOG_TRACE("Entering...");
    size_t found;
    std::string device_name = "";
    int i = 0;
    for (auto devices : m_deviceInfoList)
    {
        found = devices->deviceMAC.find(mac_address);
        if (found != std::string::npos)
        {
            device_name = devices->modelName;
            break;
        }
        i++;
    }
    MIRACASTLOG_TRACE("Exiting...");
    return device_name;
}

MiracastError MiracastController::set_FriendlyName(std::string friendly_name)
{
    MiracastError ret = MIRACAST_OK;
    MIRACASTLOG_TRACE("Entering..");

    ret = m_p2p_ctrl_obj->set_FriendlyName(friendly_name, true);

    MIRACASTLOG_TRACE("Exiting..");
    return ret;
}

std::string MiracastController::get_FriendlyName(void)
{
    MIRACASTLOG_TRACE("Entering and Exiting...");

    return m_p2p_ctrl_obj->get_FriendlyName();
}

void MiracastController::Controller_Thread(void *args)
{
    CONTROLLER_MSGQ_STRUCT controller_msgq_data = {0};
    bool thunder_req_client_connection_sent = false;
    bool start_discovering_enabled = false;
    bool session_restart_required = false;

    MIRACASTLOG_TRACE("Entering...");

    while (true)
    {
        std::string event_buffer;
        event_buffer.clear();

        MIRACASTLOG_TRACE("[%s] Waiting for Event .....\n", __FUNCTION__);
        m_controller_thread->receive_message(&controller_msgq_data, CONTROLLER_MSGQ_SIZE, THREAD_RECV_MSG_INDEFINITE_WAIT);

        event_buffer = controller_msgq_data.msg_buffer;

        MIRACASTLOG_TRACE("[%s] Received Action[%#08X]Data[%s]\n", __FUNCTION__, controller_msgq_data.state, event_buffer.c_str());

        if (CONTROLLER_SELF_ABORT == controller_msgq_data.state)
        {
            MIRACASTLOG_TRACE("CONTROLLER_SELF_ABORT Received.\n");

            stop_streaming(CONTROLLER_SELF_ABORT);
            break;
        }

        switch (controller_msgq_data.msg_type)
        {
            case P2P_MSG:
            {
                MIRACASTLOG_TRACE("P2P_MSG type received");

                switch (controller_msgq_data.state)
                {
                    case CONTROLLER_GO_DEVICE_FOUND:
                    {
                        MIRACASTLOG_TRACE("CONTROLLER_GO_DEVICE_FOUND Received\n");
                        std::string wfdSubElements;
                        DeviceInfo *device = new DeviceInfo;
                        device->deviceMAC = parse_p2p_event_data(event_buffer.c_str(), "p2p_dev_addr");
                        device->deviceType = parse_p2p_event_data(event_buffer.c_str(), "pri_dev_type");
                        device->modelName = parse_p2p_event_data(event_buffer.c_str(), "name");
                        wfdSubElements = parse_p2p_event_data(event_buffer.c_str(), "wfd_dev_info");
                        #if 0
                            device->isCPSupported = ((strtol(wfdSubElements.c_str(), nullptr, 16) >> 32) && 256);
                            device->deviceRole = (DEVICEROLE)((strtol(wfdSubElements.c_str(), nullptr, 16) >> 32) && 3);
                        #endif
                        MIRACASTLOG_TRACE("Device data parsed & stored successfully");

                        m_deviceInfoList.push_back(device);
                    }
                    break;
                    case CONTROLLER_GO_DEVICE_LOST:
                    {
                        MIRACASTLOG_TRACE("CONTROLLER_GO_DEVICE_LOST Received\n");
                        std::string lostMAC = parse_p2p_event_data(event_buffer.c_str(), "p2p_dev_addr");
                        size_t found;
                        int i = 0;
                        for (auto devices : m_deviceInfoList)
                        {
                            found = devices->deviceMAC.find(lostMAC);
                            if (found != std::string::npos)
                            {
                                delete devices;
                                m_deviceInfoList.erase(m_deviceInfoList.begin() + i);
                                break;
                            }
                            i++;
                        }
                    }
                    break;
                    case CONTROLLER_GO_DEVICE_PROVISION:
                    {
                        MIRACASTLOG_TRACE("CONTROLLER_GO_DEVICE_PROVISION Received\n");
                        // m_authType = "pbc";
                        std::string MAC = parse_p2p_event_data(event_buffer.c_str(), "p2p_dev_addr");
                    }
                    break;
                    case CONTROLLER_GO_NEG_REQUEST:
                    {
                        THUNDER_REQ_HDLR_MSGQ_STRUCT thunder_req_msgq_data = {0};
                        MIRACASTLOG_TRACE("CONTROLLER_GO_NEG_REQUEST Received\n");
                        std::string MAC;
                        size_t space_find = event_buffer.find(" ");
                        size_t dev_str = event_buffer.find("dev_passwd_id");
                        if ((space_find != std::string::npos) && (dev_str != std::string::npos))
                        {
                            MAC = event_buffer.substr(space_find, dev_str - space_find);
                            REMOVE_SPACES(MAC);
                        }

                        std::string device_name = get_device_name(MAC);

                        if (false == thunder_req_client_connection_sent)
                        {
                            if (m_rtsp_msg->get_WFDSourceMACAddress().empty())
                            {
                                thunder_req_msgq_data.state = THUNDER_REQ_HLDR_CONNECT_DEVICE_FROM_CONTROLLER;
                                strcpy(thunder_req_msgq_data.msg_buffer, MAC.c_str());
                                strcpy(thunder_req_msgq_data.buffer_user_data, device_name.c_str());
                                m_thunder_req_handler_thread->send_message(&thunder_req_msgq_data, sizeof(thunder_req_msgq_data));
                                thunder_req_client_connection_sent = true;
                            }
                            else
                            {
                                // TODO
                                //  Need to handle connect request received evenafter connection already established with other client
                            }
                        }
                        else
                        {
                            // TODO
                            //  Need to handle connect request received evenafter connection already established with other client
                        }
                    }
                    break;
                    case CONTROLLER_GO_GROUP_STARTED:
                    {
                        std::string device_name = "";
                        MIRACASTLOG_TRACE("CONTROLLER_GO_GROUP_STARTED Received\n");
                        m_groupInfo = new GroupInfo;
                        MiracastError ret = MIRACAST_FAIL;
                        size_t found = event_buffer.find("client");
                        size_t found_space = event_buffer.find(" ");

                        if (found != std::string::npos)
                        {
                            m_groupInfo->ipAddr = parse_p2p_event_data(event_buffer.c_str(), "ip_addr");
                            m_groupInfo->ipMask = parse_p2p_event_data(event_buffer.c_str(), "ip_mask");
                            m_groupInfo->goIPAddr = parse_p2p_event_data(event_buffer.c_str(), "go_ip_addr");
                            m_groupInfo->goDevAddr = parse_p2p_event_data(event_buffer.c_str(), "go_dev_addr");
                            m_groupInfo->SSID = parse_p2p_event_data(event_buffer.c_str(), "ssid");

                            device_name = get_device_name(m_groupInfo->goDevAddr);

                            size_t found_client = event_buffer.find("client");
                            m_groupInfo->interface = event_buffer.substr(found_space, found_client - found_space);
                            REMOVE_SPACES(m_groupInfo->interface);

                            if (getenv("GET_PACKET_DUMP") != nullptr)
                            {
                                std::string tcpdump;
                                tcpdump.append("tcpdump -i ");
                                tcpdump.append(m_groupInfo->interface);
                                tcpdump.append(" -s 65535 -w /opt/dump.pcap &");
                                MIRACASTLOG_VERBOSE("Dump command to execute - %s", tcpdump.c_str());
                                system(tcpdump.c_str());
                            }

                            std::string default_gw_ip = "";

                            // STB is a client in the p2p group
                            m_groupInfo->isGO = false;
                            m_groupInfo->localIPAddr = start_DHCPClient(m_groupInfo->interface, default_gw_ip);
                            if (m_groupInfo->localIPAddr.empty())
                            {
                                MIRACASTLOG_ERROR("Local IP address is not obtained");
                                session_restart_required = true;
                            }
                            else
                            {
                                if (m_groupInfo->goIPAddr.empty())
                                {
                                    MIRACASTLOG_VERBOSE("default_gw_ip [%s]\n", default_gw_ip.c_str());
                                    m_groupInfo->goIPAddr.append(default_gw_ip);
                                }
                                MIRACASTLOG_VERBOSE("initiate_TCP started GO IP[%s]\n", m_groupInfo->goIPAddr.c_str());
                                ret = initiate_TCP(m_groupInfo->goIPAddr);
                                MIRACASTLOG_VERBOSE("initiate_TCP done ret[%x]\n", ret);
                                if (MIRACAST_OK == ret)
                                {
                                    MIRACASTLOG_TRACE("RTSP Thread Initialated with RTSP_START_RECEIVE_MSGS\n");
                                    send_msg_rtsp_msg_hdler_thread(RTSP_START_RECEIVE_MSGS);
                                    ret = MIRACAST_FAIL;
                                }
                                else
                                {
                                    MIRACASTLOG_ERROR("TCP connection Failed");
                                    session_restart_required = true;
                                }
                            }
                        }
                        else
                        {
                            size_t found_go = event_buffer.find("GO");
                            m_groupInfo->interface = event_buffer.substr(found_space, found_go - found_space);
                            // STB is the GO in the p2p group
                            m_groupInfo->isGO = true;
                        }
                        if ( true == session_restart_required ){
                            m_notify_handler->onMiracastServiceClientConnectionError(m_groupInfo->goDevAddr, device_name);
                            restart_session(start_discovering_enabled);
                        }
                    }
                    break;
                    case CONTROLLER_GO_GROUP_REMOVED:
                    {
                        MIRACASTLOG_TRACE("CONTROLLER_GO_GROUP_REMOVED Received\n");
                    }
                    break;
                    case CONTROLLER_GO_STOP_FIND:
                    case CONTROLLER_GO_NEG_SUCCESS:
                    case CONTROLLER_GO_GROUP_FORMATION_SUCCESS:
                    {
                        MIRACASTLOG_TRACE("[STOP_FIND/NEG_SUCCESS/GROUP_FORMATION_SUCCESS] Received\n");
                    }
                    break;
                    case CONTROLLER_GO_EVENT_ERROR:
                    case CONTROLLER_GO_UNKNOWN_EVENT:
                    {
                        MIRACASTLOG_ERROR("[GO_EVENT_ERROR/GO_UNKNOWN_EVENT] Received\n");
                    }
                    break;
                    default:
                    {
                        MIRACASTLOG_ERROR("Invalid state received with P2P_MSG\n");
                    }
                    break;
                }
            }
            break;
            case RTSP_MSG:
            {
                MIRACASTLOG_TRACE("RTSP_MSG type received");
                switch (controller_msgq_data.state)
                {
                    case CONTROLLER_RTSP_MSG_RECEIVED_PROPERLY:
                    {
                        MIRACASTLOG_TRACE("[CONTROLLER_RTSP_MSG_RECEIVED_PROPERLY] Received\n");
                        start_streaming();
                        m_connectionStatus = true;
                    }
                    break;
                    case CONTROLLER_RTSP_MSG_TIMEDOUT:
                    case CONTROLLER_RTSP_INVALID_MESSAGE:
                    case CONTROLLER_RTSP_SEND_REQ_RESP_FAILED:
                    case CONTROLLER_RTSP_TEARDOWN_REQ_RECEIVED:
                    case CONTROLLER_RTSP_RESTART_DISCOVERING:
                    {
                        std::string MAC = m_rtsp_msg->get_WFDSourceMACAddress();
                        std::string device_name = m_rtsp_msg->get_WFDSourceName();

                        m_connectionStatus = false;

                        if (CONTROLLER_RTSP_TEARDOWN_REQ_RECEIVED == controller_msgq_data.state)
                        {
                            MIRACASTLOG_TRACE("[TEARDOWN_REQ] Received\n");
                            m_notify_handler->onMiracastServiceClientStopRequest(MAC, device_name);
                        }
                        else if (CONTROLLER_RTSP_RESTART_DISCOVERING != controller_msgq_data.state)
                        {
                            MIRACASTLOG_TRACE("[TIMEDOUT/SEND_REQ_RESP_FAIL/INVALID_MESSAG/GO_NEG/GROUP_FORMATION_FAILURE] Received\n");
                            m_notify_handler->onMiracastServiceClientConnectionError(MAC, device_name);
                        }
                        stop_streaming();
                        m_rtsp_msg->reset_WFDSourceMACAddress();
                        m_rtsp_msg->reset_WFDSourceName();
                        restart_session(start_discovering_enabled);
                    }
                    break;
                    default:
                    {
                        MIRACASTLOG_ERROR("Invalid state received with RTSP_MSG\n");
                    }
                    break;
                }
            }
            break;
            case CONTRLR_FW_MSG:
            {
                MIRACASTLOG_TRACE("CONTRLR_FW_MSG type received");
                switch (controller_msgq_data.state)
                {
                    case CONTROLLER_START_DISCOVERING:
                    {
                        MIRACASTLOG_TRACE("CONTROLLER_START_DISCOVERING Received\n");
                        set_WFDParameters();
                        discover_devices();
                        start_discovering_enabled = true;
                    }
                    break;
                    case CONTROLLER_STOP_DISCOVERING:
                    {
                        MIRACASTLOG_TRACE("CONTROLLER_STOP_DISCOVERING Received\n");
                        stop_session(true);
                        start_discovering_enabled = false;
                    }
                    break;
                    case CONTROLLER_START_STREAMING:
                    {
                        MIRACASTLOG_TRACE("[CONTROLLER_START_STREAMING] Received\n");
                        start_streaming();
                    }
                    break;
                    case CONTROLLER_PAUSE_STREAMING:
                    {
                        MIRACASTLOG_TRACE("CONTROLLER_PAUSE_STREAMING Received\n");
                    }
                    break;
                    case CONTROLLER_STOP_STREAMING:
                    {
                        MIRACASTLOG_TRACE("CONTROLLER_STOP_STREAMING Received\n");
                        stop_streaming();
                    }
                    break;
                    case CONTROLLER_CONNECT_REQ_FROM_THUNDER:
                    {
                        MIRACASTLOG_TRACE("CONTROLLER_CONNECT_REQ_FROM_THUNDER Received\n");
                        std::string mac_address = event_buffer;
                        std::string device_name = get_device_name(mac_address);

                        connect_device(mac_address);
                        m_rtsp_msg->set_WFDSourceMACAddress(mac_address);
                        m_rtsp_msg->set_WFDSourceName(device_name);
                        thunder_req_client_connection_sent = false;
                    }
                    break;
                    case CONTROLLER_CONNECT_REQ_REJECT:
                    case CONTROLLER_CONNECT_REQ_TIMEOUT:
                    {
                        MIRACASTLOG_TRACE("CONNECT_REQ_REJECT/TIMEOUT Received\n");
                        thunder_req_client_connection_sent = false;
                    }
                    break;
                    case CONTROLLER_TEARDOWN_REQ_FROM_THUNDER:
                    {
                        MIRACASTLOG_TRACE("TEARDOWN request sent to RTSP handler\n");
                        stop_streaming(CONTROLLER_TEARDOWN_REQ_FROM_THUNDER);
                    }
                    break;
                    default:
                    {
                        MIRACASTLOG_ERROR("Invalid state received with CONTRLR_FW_MSG\n");
                    }
                    break;
                }
            }
            break;
            default:
            {
                MIRACASTLOG_ERROR("Invalid MsgType received.\n");
            }
            break;
        }        
    }
    MIRACASTLOG_TRACE("Exiting...");
}

void MiracastController::ThunderReqHandler_Thread(void *args)
{
    CONTROLLER_MSGQ_STRUCT controller_msgq_data = {0};
    THUNDER_REQ_HDLR_MSGQ_STRUCT thunder_req_hdlr_msgq_data = {0};
    bool send_message = false;

    MIRACASTLOG_TRACE("Entering...");

    while (true)
    {
        send_message = true;
        memset(&controller_msgq_data, 0x00, CONTROLLER_MSGQ_SIZE);

        MIRACASTLOG_TRACE("[%s] Waiting for Event .....\n", __FUNCTION__);
        m_thunder_req_handler_thread->receive_message(&thunder_req_hdlr_msgq_data, sizeof(thunder_req_hdlr_msgq_data), THREAD_RECV_MSG_INDEFINITE_WAIT);

        MIRACASTLOG_TRACE("[%s] Received Action[%#08X]\n", __FUNCTION__, thunder_req_hdlr_msgq_data.state);

        switch (thunder_req_hdlr_msgq_data.state)
        {
            case THUNDER_REQ_HLDR_START_DISCOVER:
            {
                MIRACASTLOG_TRACE("[THUNDER_REQ_HLDR_START_DISCOVER]\n");
                controller_msgq_data.state = CONTROLLER_START_DISCOVERING;
            }
            break;
            case THUNDER_REQ_HLDR_STOP_DISCOVER:
            {
                MIRACASTLOG_TRACE("[THUNDER_REQ_HLDR_STOP_DISCOVER]\n");
                controller_msgq_data.state = CONTROLLER_STOP_DISCOVERING;
            }
            break;
            case THUNDER_REQ_HLDR_CONNECT_DEVICE_FROM_CONTROLLER:
            {
                std::string device_name = thunder_req_hdlr_msgq_data.buffer_user_data;
                std::string MAC = thunder_req_hdlr_msgq_data.msg_buffer;

                send_message = true;
                MIRACASTLOG_TRACE("\n################# GO DEVICE[%s - %s] wants to connect: #################\n", device_name.c_str(), MAC.c_str());
                m_notify_handler->onMiracastServiceClientConnectionRequest(MAC, device_name);

                if (0 == access("/opt/miracast_autoconnect", F_OK)){
                    strcpy(controller_msgq_data.msg_buffer, MAC.c_str());
                    controller_msgq_data.state = CONTROLLER_CONNECT_REQ_FROM_THUNDER;
                }
                else if (true == m_thunder_req_handler_thread->receive_message(&thunder_req_hdlr_msgq_data, sizeof(thunder_req_hdlr_msgq_data), THUNDER_REQ_THREAD_CLIENT_CONNECTION_WAITTIME))
                {
                        MIRACASTLOG_TRACE("ThunderReqHandler Msg Received [%#08X]\n", thunder_req_hdlr_msgq_data.state);
                        if (THUNDER_REQ_HLDR_CONNECT_DEVICE_ACCEPTED == thunder_req_hdlr_msgq_data.state)
                        {
                            strcpy(controller_msgq_data.msg_buffer, MAC.c_str());
                            controller_msgq_data.state = CONTROLLER_CONNECT_REQ_FROM_THUNDER;
                        }
                        else if (THUNDER_REQ_HLDR_CONNECT_DEVICE_REJECTED == thunder_req_hdlr_msgq_data.state)
                        {
                            controller_msgq_data.state = CONTROLLER_CONNECT_REQ_REJECT;
                        }
                        else if (THUNDER_REQ_HLDR_SHUTDOWN_APP == thunder_req_hdlr_msgq_data.state)
                        {
                            controller_msgq_data.state = CONTROLLER_SELF_ABORT;
                        }
                        else if (THUNDER_REQ_HLDR_STOP_DISCOVER == thunder_req_hdlr_msgq_data.state)
                        {
                            controller_msgq_data.state = CONTROLLER_STOP_DISCOVERING;
                        }
                        else
                        {
                            controller_msgq_data.state = CONTROLLER_INVALID_STATE;
                        }
                }
                else
                {
                    MIRACASTLOG_TRACE("[CONTROLLER_CONNECT_REQ_TIMEOUT]\n");
                    controller_msgq_data.state = CONTROLLER_CONNECT_REQ_TIMEOUT;
                }
            }
            break;
            case THUNDER_REQ_HLDR_SHUTDOWN_APP:
            {
                MIRACASTLOG_TRACE("[THUNDER_REQ_HLDR_SHUTDOWN_APP]\n");
                controller_msgq_data.state = CONTROLLER_SELF_ABORT;
            }
            break;
            case THUNDER_REQ_HLDR_TEARDOWN_CONNECTION:
            {
                MIRACASTLOG_TRACE("[THUNDER_REQ_HLDR_TEARDOWN_CONNECTION]\n");
                controller_msgq_data.state = CONTROLLER_TEARDOWN_REQ_FROM_THUNDER;
            }
            break;
            default:
            {
                //
            }
            break;
        }

        if (true == send_message)
        {
            controller_msgq_data.msg_type = CONTRLR_FW_MSG;
            MIRACASTLOG_INFO("Msg to Controller Action[%#08X]\n", controller_msgq_data.state);
            m_controller_thread->send_message(&controller_msgq_data, CONTROLLER_MSGQ_SIZE);
            if (THUNDER_REQ_HLDR_SHUTDOWN_APP == thunder_req_hdlr_msgq_data.state)
            {
                break;
            }
        }
    }
    MIRACASTLOG_TRACE("Exiting...");
}

void MiracastController::send_msg_rtsp_msg_hdler_thread(eCONTROLLER_FW_STATES state)
{
    MIRACASTLOG_TRACE("Entering...");
    m_rtsp_msg->send_msgto_rtsp_msg_hdler_thread(state);
    MIRACASTLOG_TRACE("Exiting...");
}

void MiracastController::send_msg_thunder_msg_hdler_thread(MIRACAST_SERVICE_STATES state, std::string action_buffer, std::string user_data)
{
    THUNDER_REQ_HDLR_MSGQ_STRUCT thunder_req_msgq_data = {0};
    bool valid_mesage = true;
    MIRACASTLOG_TRACE("Entering...");
    switch (state)
    {
        case MIRACAST_SERVICE_WFD_START:
        {
            MIRACASTLOG_TRACE("[MIRACAST_SERVICE_WFD_START]\n");
            thunder_req_msgq_data.state = THUNDER_REQ_HLDR_START_DISCOVER;
        }
        break;
        case MIRACAST_SERVICE_WFD_STOP:
        {
            MIRACASTLOG_TRACE("[MIRACAST_SERVICE_WFD_STOP]\n");
            thunder_req_msgq_data.state = THUNDER_REQ_HLDR_STOP_DISCOVER;
        }
        break;
        case MIRACAST_SERVICE_SHUTDOWN:
        {
            MIRACASTLOG_TRACE("[MIRACAST_SERVICE_SHUTDOWN]\n");
            thunder_req_msgq_data.state = THUNDER_REQ_HLDR_SHUTDOWN_APP;
        }
        break;
        case MIRACAST_SERVICE_STOP_CLIENT_CONNECTION:
        {
            MIRACASTLOG_TRACE("[MIRACAST_SERVICE_STOP_CLIENT_CONNECTION]\n");
            thunder_req_msgq_data.state = THUNDER_REQ_HLDR_TEARDOWN_CONNECTION;
            memcpy(thunder_req_msgq_data.msg_buffer, action_buffer.c_str(), action_buffer.length());
        }
        break;
        case MIRACAST_SERVICE_ACCEPT_CLIENT:
        {
            MIRACASTLOG_TRACE("[MIRACAST_SERVICE_ACCEPT_CLIENT]\n");
            thunder_req_msgq_data.state = THUNDER_REQ_HLDR_CONNECT_DEVICE_ACCEPTED;
        }
        break;
        case MIRACAST_SERVICE_REJECT_CLIENT:
        {
            MIRACASTLOG_TRACE("[MIRACAST_SERVICE_REJECT_CLIENT]\n");
            thunder_req_msgq_data.state = THUNDER_REQ_HLDR_CONNECT_DEVICE_REJECTED;
        }
        break;
        default:
        {
            MIRACASTLOG_ERROR("Unknown Action Received [%#08X]\n", state);
            valid_mesage = false;
        }
        break;
    }

    MIRACASTLOG_VERBOSE("MiracastController::SendMessageToThunderReqHandler received Action[%#08X]\n", state);

    if (true == valid_mesage)
    {
        MIRACASTLOG_VERBOSE("Msg to ThunderReqHdlr Action[%#08X]\n", thunder_req_msgq_data.state);
        m_thunder_req_handler_thread->send_message(&thunder_req_msgq_data, sizeof(thunder_req_msgq_data));
    }
    MIRACASTLOG_TRACE("Exiting...");
}

void MiracastController::set_enable(bool is_enabled)
{
    MIRACAST_SERVICE_STATES state = MIRACAST_SERVICE_WFD_STOP;

    MIRACASTLOG_TRACE("Entering...");

    if ( true == is_enabled)
    {
        state = MIRACAST_SERVICE_WFD_START;
    }

    send_msg_thunder_msg_hdler_thread(state);
    MIRACASTLOG_TRACE("Exiting...");
}

void MiracastController::accept_client_connection(std::string is_accepted)
{
    MIRACAST_SERVICE_STATES state = MIRACAST_SERVICE_REJECT_CLIENT;

    MIRACASTLOG_TRACE("Entering...");

    if ("Accept" == is_accepted)
    {
        MIRACASTLOG_VERBOSE("Client Connection Request accepted\n");
        state = MIRACAST_SERVICE_ACCEPT_CLIENT;
    }
    else
    {
        MIRACASTLOG_VERBOSE("Client Connection Request Rejected\n");
    }
    send_msg_thunder_msg_hdler_thread(state);
    MIRACASTLOG_TRACE("Exiting...");
}

bool MiracastController::stop_client_connection(std::string mac_address)
{
    MIRACASTLOG_TRACE("Entering...");

    if (0 != (mac_address.compare(get_connected_device_mac())))
    {
        MIRACASTLOG_TRACE("Exiting...");
        return false;
    }
    send_msg_thunder_msg_hdler_thread(MIRACAST_SERVICE_STOP_CLIENT_CONNECTION, mac_address);
    MIRACASTLOG_TRACE("Exiting...");
    return true;
}

void ThunderReqHandlerCallback(void *args)
{
    MiracastController *miracast_ctrler_obj = (MiracastController *)args;
    MIRACASTLOG_TRACE("Entering...");
    miracast_ctrler_obj->ThunderReqHandler_Thread(nullptr);
    MIRACASTLOG_TRACE("Exiting...");
}

void ControllerThreadCallback(void *args)
{
    MiracastController *miracast_ctrler_obj = (MiracastController *)args;
    MIRACASTLOG_TRACE("Entering...");
    miracast_ctrler_obj->Controller_Thread(nullptr);
    MIRACASTLOG_TRACE("Exiting...");
}