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

#ifndef _MIRACAST_RSTP_MSG_H_
#define _MIRACAST_RTSP_MSG_H_

#include <MiracastCommon.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>

#define MAX_EPOLL_EVENTS 64
#define SOCKET_DFLT_WAIT_TIMEOUT (30 * ONE_SECOND_IN_MILLISEC)
#define RTSP_KEEP_ALIVE_WAIT_TIMEOUT    ( ONE_SECOND_IN_MILLISEC )

typedef enum rtsp_status_e
{
    RTSP_MSG_FAILURE,
    RTSP_MSG_SUCCESS,
    RTSP_INVALID_MSG_RECEIVED,
    RTSP_MSG_TEARDOWN_REQUEST,
    RTSP_TIMEDOUT
} RTSP_STATUS;

#define RTSP_CRLF_STR "\r\n"
#define RTSP_DOUBLE_QUOTE_STR "\""
#define RTSP_SPACE_STR SPACE_CHAR
#define RTSP_SEMI_COLON_STR ";"

/* It will be used to parse the data from WFD Source */
#define RTSP_STD_REQUEST_STR "RTSP/1.0" RTSP_CRLF_STR
#define RTSP_REQ_OPTIONS "OPTIONS * " RTSP_STD_REQUEST_STR
#define RTSP_REQ_TEARDOWN_MODE "wfd_trigger_method: TEARDOWN"
#define RTSP_STD_SEQUENCE_FIELD "CSeq: "
#define RTSP_STD_REQUIRE_FIELD "Require: "
#define RTSP_STD_SESSION_FIELD "Session: "
#define RTSP_STD_UNICAST_FIELD "unicast"
#define RTSP_WFD_CONTENT_PROTECT_FIELD "wfd_content_protection: "
#define RTSP_WFD_VIDEO_FMT_FIELD "wfd_video_formats: "
#define RTSP_WFD_AUDIO_FMT_FIELD "wfd_audio_codecs: "
#define RTSP_WFD_CLIENT_PORTS_FIELD "wfd_client_rtp_ports: "
#define RTSP_WFD_PRESENTATION_URL_FIELD "wfd_presentation_URL: "
#define RTSP_M16_REQUEST_MSG "GET_PARAMETER rtsp://localhost/wfd1.0 RTSP/1.0"

/* Default values*/
#define RTSP_DFLT_CONTENT_PROTECTION "none"
#define RTSP_DFLT_VIDEO_FORMATS "00 00 03 10 0001ffff 1fffffff 00001fff 00 0000 0000 10 none none"
#define RTSP_DFLT_AUDIO_FORMATS "AAC 00000007 00"
#define RTSP_DFLT_TRANSPORT_PROFILE "RTP/AVP/UDP"
#define RTSP_DFLT_STREAMING_PORT "1990"
#define RTSP_DFLT_CLIENT_RTP_PORTS RTSP_DFLT_TRANSPORT_PROFILE RTSP_SEMI_COLON_STR RTSP_STD_UNICAST_FIELD RTSP_SPACE_STR RTSP_DFLT_STREAMING_PORT RTSP_SPACE_STR "0 mode=play"

typedef enum rtsp_message_format_sink2src_e
{
    RTSP_MSG_FMT_M1_RESPONSE = 0x00,
    RTSP_MSG_FMT_M2_REQUEST,
    RTSP_MSG_FMT_M3_RESPONSE,
    RTSP_MSG_FMT_M4_RESPONSE,
    RTSP_MSG_FMT_M5_RESPONSE,
    RTSP_MSG_FMT_M6_REQUEST,
    RTSP_MSG_FMT_M7_REQUEST,
    RTSP_MSG_FMT_M16_RESPONSE,
    RTSP_MSG_FMT_PAUSE_REQUEST,
    RTSP_MSG_FMT_PLAY_REQUEST,
    RTSP_MSG_FMT_TEARDOWN_REQUEST,
    RTSP_MSG_FMT_TEARDOWN_RESPONSE,
    RTSP_MSG_FMT_INVALID
} RTSP_MSG_FMT_SINK2SRC;

typedef struct rtsp_msg_template_info
{
    RTSP_MSG_FMT_SINK2SRC rtsp_msg_fmt_e;
    const char *template_name;
} RTSP_MSG_TEMPLATE_INFO;

class MiracastRTSPMsg
{
public:
    static MiracastRTSPMsg *getInstance(MiracastError &error_code , MiracastThread *controller_thread_id = nullptr);
    static void destroyInstance();

    std::string get_WFDVideoFormat(void);
    std::string get_WFDAudioCodecs(void);
    std::string get_WFDClientRTPPorts(void);
    std::string get_WFDUIBCCapability(void);
    std::string get_WFDContentProtection(void);
    std::string get_WFDSecScreenSharing(void);
    std::string get_WFDPortraitDisplay(void);
    std::string get_WFDSecRotation(void);
    std::string get_WFDSecHWRotation(void);
    std::string get_WFDSecFrameRate(void);
    std::string get_WFDPresentationURL(void);
    std::string get_WFDTransportProfile(void);
    std::string get_WFDStreamingPortNumber(void);
    bool IsWFDUnicastSupported(void);
    std::string get_CurrentWFDSessionNumber(void);

    bool set_WFDVideoFormat(std::string video_formats);
    bool set_WFDAudioCodecs(std::string audio_codecs);
    bool set_WFDClientRTPPorts(std::string client_rtp_ports);
    bool set_WFDUIBCCapability(std::string uibc_caps);
    bool set_WFDContentProtection(std::string content_protection);
    bool set_WFDSecScreenSharing(std::string screen_sharing);
    bool set_WFDPortraitDisplay(std::string portrait_display);
    bool set_WFDSecRotation(std::string rotation);
    bool set_WFDSecHWRotation(std::string hw_rotation);
    bool set_WFDSecFrameRate(std::string framerate);
    bool set_WFDPresentationURL(std::string URL);
    bool set_WFDTransportProfile(std::string profile);
    bool set_WFDStreamingPortNumber(std::string port_number);
    bool set_WFDEnableDisableUnicast(bool enable_disable_unicast);
    bool set_WFDSessionNumber(std::string session);

    void set_WFDSourceMACAddress(std::string MAC_Addr);
    void set_WFDSourceName(std::string device_name);
    std::string get_WFDSourceName(void);
    std::string get_WFDSourceMACAddress(void);
    void reset_WFDSourceMACAddress(void);
    void reset_WFDSourceName(void);

    void send_msgto_rtsp_msg_hdler_thread(eCONTROLLER_FW_STATES state);
    MiracastError initiate_TCP(std::string goIP);
    void RTSPMessageHandler_Thread(void *args);

    static std::string format_string(const char *fmt, const std::vector<const char *> &args)
    {
        std::string result = fmt;
        size_t arg_index = 0;
        size_t arg_count = args.size();
        while (arg_index < arg_count)
        {
            size_t found = result.find("%s");
            if (found != std::string::npos)
            {
                result.replace(found, 2, args[arg_index]);
            }
            ++arg_index;
        }
        return result;
    };

private:
    static MiracastRTSPMsg *m_rtsp_msg_obj;
    MiracastRTSPMsg();
    virtual ~MiracastRTSPMsg();
    MiracastRTSPMsg &operator=(const MiracastRTSPMsg &) = delete;
    MiracastRTSPMsg(const MiracastRTSPMsg &) = delete;

    int m_tcpSockfd;
    std::string m_connected_mac_addr;
    std::string m_connected_device_name;
    std::string m_wfd_video_formats;
    std::string m_wfd_audio_codecs;
    std::string m_wfd_client_rtp_ports;
    std::string m_wfd_uibc_capability;
    std::string m_wfd_content_protection;
    std::string m_wfd_sec_screensharing;
    std::string m_wfd_sec_portrait_display;
    std::string m_wfd_sec_rotation;
    std::string m_wfd_sec_hw_rotation;
    std::string m_wfd_sec_framerate;
    std::string m_wfd_presentation_URL;
    std::string m_wfd_transport_profile;
    std::string m_wfd_streaming_port;
    bool m_is_unicast;
    std::string m_wfd_session_number;
    std::string m_current_sequence_number;
    static RTSP_MSG_TEMPLATE_INFO rtsp_msg_template_info[];
    MiracastThread *m_rtsp_msg_handler_thread;
    MiracastThread *m_controller_thread;

    void send_msgto_controller_thread(eCONTROLLER_FW_STATES state);
    MiracastError create_RTSPThread(void);

    RTSP_STATUS validate_rtsp_msg_response_back(std::string rtsp_msg_buffer, eCONTROLLER_FW_STATES state);
    RTSP_STATUS validate_rtsp_m1_msg_m2_send_request(std::string rtsp_m1_msg_buffer);
    RTSP_STATUS validate_rtsp_m2_request_ack(std::string rtsp_m1_response_ack_buffer);
    RTSP_STATUS validate_rtsp_m3_response_back(std::string rtsp_m3_msg_buffer);
    RTSP_STATUS validate_rtsp_m4_response_back(std::string rtsp_m4_msg_buffer);
    RTSP_STATUS validate_rtsp_m5_msg_m6_send_request(std::string rtsp_m5_msg_buffer);
    RTSP_STATUS validate_rtsp_m6_ack_m7_send_request(std::string rtsp_m6_ack_buffer);
    RTSP_STATUS validate_rtsp_m7_request_ack(std::string rtsp_m7_ack_buffer);
    RTSP_STATUS validate_rtsp_post_m1_m7_xchange(std::string rtsp_post_m1_m7_xchange_buffer);
    RTSP_STATUS rtsp_sink2src_request_msg_handling(eCONTROLLER_FW_STATES state);

    const char *get_RequestResponseFormat(RTSP_MSG_FMT_SINK2SRC format_type);
    std::string generate_request_response_msg(RTSP_MSG_FMT_SINK2SRC msg_fmt_needed, std::string received_session_no, std::string append_data1);
    std::string get_RequestSequenceNumber(void);

    RTSP_STATUS receive_buffer_timedOut(int sockfd, void *buffer, size_t buffer_len , unsigned int wait_time_ms = SOCKET_DFLT_WAIT_TIMEOUT );
    bool wait_data_timeout(int m_Sockfd, unsigned int ms);
    RTSP_STATUS send_rstp_msg(int sockfd, std::string rtsp_response_buffer);
};

#endif