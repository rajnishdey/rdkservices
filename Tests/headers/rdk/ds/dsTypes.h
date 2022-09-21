#pragma once

typedef enum _dsHdcpProtocolVersion_t {
    dsHDCP_VERSION_1X = 0,
    dsHDCP_VERSION_2X,
    dsHDCP_VERSION_MAX
} dsHdcpProtocolVersion_t;

typedef enum _dsAudioCapabilities_t {
    dsAUDIOSUPPORT_NONE = 0x0,
    dsAUDIOSUPPORT_ATMOS = 0x01,
    dsAUDIOSUPPORT_DD = 0x02,
    dsAUDIOSUPPORT_DDPLUS = 0x04,
    dsAUDIOSUPPORT_DAD = 0x08,
    dsAUDIOSUPPORT_DAPv2 = 0x10,
    dsAUDIOSUPPORT_MS12 = 0x20,
    dsAUDIOSUPPORT_MS12V2 = 0x40,
    dsAUDIOSUPPORT_Invalid = 0x80,
} dsAudioCapabilities_t;

typedef enum _dsMS12Capabilities_t {
    dsMS12SUPPORT_NONE = 0x0,
    dsMS12SUPPORT_DolbyVolume = 0x01,
    dsMS12SUPPORT_InteligentEqualizer = 0x02,
    dsMS12SUPPORT_DialogueEnhancer = 0x04,
    dsMS12SUPPORT_Invalid = 0x80,
} dsMS12Capabilities_t;

struct dsSpd_infoframe_st {
    uint8_t pkttype;
    uint8_t version;
    uint8_t length;
    uint8_t rsd;
    uint8_t checksum;

    uint8_t vendor_name[8];
    uint8_t product_des[16];

    uint8_t source_info;
};


typedef enum _dsVideoResolution_t{
    dsVIDEO_PIXELRES_720x460,
    dsVIDEO_PIXELRES_720x576,
    dsVIDEO_PIXELRES_1280x720,
    dsVIDEO_PIXELRES_1920x1080,
    dsVIDEO_PIXELRES_3840z2160,
    dsVIDEO_PIXELRES_4096x2160,
    dsVIDEO_PIXELRES_MAX
}dsVideoResolution_t;
/*
typedef enum _dsVideoAspectRatio_t{
    dsVIDEO_ASPECT_RATIO_4x3,
    dsVIDEO_ASPECT_RATIO_16x9,
    dsVIDEO_ASPECT_RATIO_MAX
}dsVideoAspectRatio_t;

typedef enum _dsVideoStereoScopicMode_t{
    dsVIDEO_SSMODE_UNKNOWN = 0,
    dsVIDEO_SSMODE_2D,
    dsVIDEO_SSMODE_3D_SIDE_BY_SIDE,
    dsVIDEO_SSMODE_3D_TOP_AND_BOTTOM,
    dsVIDEO_SSMODE_MAX
}dsVideoStereoScopicMode_t;

typedef enum _dsVideoFrameRate_t{
    dsVIDEO_FRAMERATE_UNKNOWN,
    dsVIDEO_FRAMERATE_24,
    dsVIDEO_FRAMERATE_25,
    dsVIDEO_FRAMERATE_30,
    dsVIDEO_FRAMERATE_60,
    dsVIDEO_FRAMERATE_23dot98,
    dsVIDEO_FRAMERATE_29dot97,
    dsVIDEO_FRAMERATE_50,
    dsVIDEO_FRAMERATE_59dot94,
    dsVIDEO_FRAMERATE_MAX
}dsVideoFrameRate_t;
*/
typedef struct _dsVideoPortResolution_t {
    char name[32];
    dsVideoResolution_t pixelResolution;
//    dsVideoAspectRatio_t aspectRatio;
//    dsVideoStereoScopicMode_t stereoScopicMode;
//    dsVideoFrameRate_t frameRate;
//    bool interlaced;
}dsVideoPortResolution_t;

typedef enum tv_hdmi_edid_version_e {
    HDMI_EDID_VER_14 = 0,
    HDMI_EDID_VER_20,
    HDMI_EDID_VER_MAX,
}tv_hdmi_edid_version_e;

