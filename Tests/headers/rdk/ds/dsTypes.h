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
    dsVIDEO_PIXELRES_720x480,     /**< 720x480 Resolution.                         */
    dsVIDEO_PIXELRES_720x576,     /**< 720x576 Resolution.                         */
    dsVIDEO_PIXELRES_1280x720,    /**< 1280x720 Resolution.                        */
    dsVIDEO_PIXELRES_1920x1080,   /**< 1920x1080 Resolution.                       */

    dsVIDEO_PIXELRES_3840x2160,   /**< 3840x2160 Resolution.                       */
    dsVIDEO_PIXELRES_4096x2160,   /**< 3840x2160 Resolution.                       */

    dsVIDEO_PIXELRES_MAX         /**< Maximum index for Video ouptut resolutions. */
}dsVideoResolution_t;
/*
typedef enum _dsVideoAspectRatio_t{
    dsVIDEO_ASPECT_RATIO_4x3,     /**< 4:3 aspect ratio.                      */
    dsVIDEO_ASPECT_RATIO_16x9,    /**< 16:9 aspect ratio.                     */
    dsVIDEO_ASPECT_RATIO_MAX     /**< Maximum index for video aspect ratios. */
}dsVideoAspectRatio_t;

typedef enum _dsVideoStereoScopicMode_t {
    dsVIDEO_SSMODE_UNKNOWN = 0,          /**< Unknown mode.                                */
    dsVIDEO_SSMODE_2D,                   /**< 2D mode.                                     */
    dsVIDEO_SSMODE_3D_SIDE_BY_SIDE,      /**< 3D side by side (L/R) stereo mode.           */
    dsVIDEO_SSMODE_3D_TOP_AND_BOTTOM,    /**< 3D top & bottom stereo mode.                 */
    dsVIDEO_SSMODE_MAX                   /**< Maximum index for video stereoscopic modes.  */
}dsVideoStereoScopicMode_t;

typedef enum _dsVideoFrameRate_t{
    dsVIDEO_FRAMERATE_UNKNOWN,    /**< Unknown frame rate.                       */
    dsVIDEO_FRAMERATE_24,         /**< Played at 24 frames per second.           */
    dsVIDEO_FRAMERATE_25,         /**< Played at 25 frames per second.           */
    dsVIDEO_FRAMERATE_30,         /**< Played at 30 frames per second.           */
    dsVIDEO_FRAMERATE_60,         /**< Played at 60 frames per second.           */
    dsVIDEO_FRAMERATE_23dot98,    /**< Played at 23.98 frames per second.        */
    dsVIDEO_FRAMERATE_29dot97,    /**< Played at 29.97 frames per second.        */
    dsVIDEO_FRAMERATE_50,         /**< Played at 50 frames per second.           */
    dsVIDEO_FRAMERATE_59dot94,    /**< Played at 59.94 frames per second.        */
    dsVIDEO_FRAMERATE_MAX         /**< Maximum index for video frame rates.      */
}dsVideoFrameRate_t;
*/
typedef struct _dsVideoPortResolution_t {
    char name[32];                                    /**< Name the resolution (e.g. 480i, 480p, 1080p24).   */
    dsVideoResolution_t  pixelResolution;           /**< The resolution associated with the name.                 */
    dsVideoAspectRatio_t  aspectRatio;              /**< The associated aspect ratio.                             */
    dsVideoStereoScopicMode_t  stereoScopicMode;    /**< The associated stereoscopic mode.                        */
    dsVideoFrameRate_t  frameRate;                  /**< The associated frame rate.                               */
    bool interlaced;                                /**< The associated scan mode(@a true if interlaced, @a false if progressive). */
}dsVideoPortResolution_t;
typedef enum tv_hdmi_edid_version_e {
    HDMI_EDID_VER_14 = 0,
    HDMI_EDID_VER_20,
    HDMI_EDID_VER_MAX,
}tv_hdmi_edid_version_e;

