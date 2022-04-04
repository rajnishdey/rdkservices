<!-- Generated automatically, DO NOT EDIT! -->
<a name="head.Player_Info_Plugin"></a>
# Player Info Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::white_circle:**

A PlayerInfo plugin for Thunder framework.

### Table of Contents

- [Introduction](#head.Introduction)
- [Description](#head.Description)
- [Configuration](#head.Configuration)
- [Methods](#head.Methods)
- [Properties](#head.Properties)
- [Notifications](#head.Notifications)

<a name="head.Introduction"></a>
# Introduction

<a name="head.Scope"></a>
## Scope

This document describes purpose and functionality of the PlayerInfo plugin. It includes detailed specification about its configuration, methods and properties provided, as well as notifications sent.

<a name="head.Case_Sensitivity"></a>
## Case Sensitivity

All identifiers of the interfaces described in this document are case-sensitive. Thus, unless stated otherwise, all keywords, entities, properties, relations and actions should be treated as such.

<a name="head.Acronyms,_Abbreviations_and_Terms"></a>
## Acronyms, Abbreviations and Terms

The table below provides and overview of acronyms used in this document and their definitions.

| Acronym | Description |
| :-------- | :-------- |
| <a name="acronym.API">API</a> | Application Programming Interface |
| <a name="acronym.HTTP">HTTP</a> | Hypertext Transfer Protocol |
| <a name="acronym.JSON">JSON</a> | JavaScript Object Notation; a data interchange format |
| <a name="acronym.JSON-RPC">JSON-RPC</a> | A remote procedure call protocol encoded in JSON |

The table below provides and overview of terms and abbreviations used in this document and their definitions.

| Term | Description |
| :-------- | :-------- |
| <a name="term.callsign">callsign</a> | The name given to an instance of a plugin. One plugin can be instantiated multiple times, but each instance the instance name, callsign, must be unique. |

<a name="head.References"></a>
## References

| Ref ID | Description |
| :-------- | :-------- |
| <a name="ref.HTTP">[HTTP](http://www.w3.org/Protocols)</a> | HTTP specification |
| <a name="ref.JSON-RPC">[JSON-RPC](https://www.jsonrpc.org/specification)</a> | JSON-RPC 2.0 specification |
| <a name="ref.JSON">[JSON](http://www.json.org/)</a> | JSON specification |
| <a name="ref.Thunder">[Thunder](https://github.com/WebPlatformForEmbedded/Thunder/blob/master/doc/WPE%20-%20API%20-%20WPEFramework.docx)</a> | Thunder API Reference |

<a name="head.Description"></a>
# Description

The `PlayerInfo` plugin helps to get system supported audio and video codecs.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a name="head.Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| callsign | string | Plugin instance name (default: *PlayerInfo*) |
| classname | string | Class name: *PlayerInfo* |
| locator | string | Library name: *libWPEPlayerInfo.so* |
| autostart | boolean | Determines if the plugin shall be started automatically along with the framework |

<a name="head.Methods"></a>
# Methods

The following methods are provided by the PlayerInfo plugin:

PlayerInfo interface methods:

| Method | Description |
| :-------- | :-------- |
| [audiocodecs](#method.audiocodecs) | Returns the audio codec supported by the platform |
| [videocodecs](#method.videocodecs) | Returns the video codec supported by the platform |


<a name="method.audiocodecs"></a>
## *audiocodecs [<sup>method</sup>](#head.Methods)*

Returns the audio codec supported by the platform. 
 
### Events 
 
No Events.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | array |  |
| result[#] | string |  (must be one of the following: *AudioUndefined*, *AudioAac*, *AudioAc3*, *AudioAc3Plus*, *AudioDts*, *AudioMpeg1*, *AudioMpeg2*, *AudioMpeg3*, *AudioMpeg4*, *AudioOpus*, *AudioVorbisOgg*, *AudioWav*) |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "PlayerInfo.1.audiocodecs"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": [
        "AudioUndefined"
    ]
}
```

<a name="method.videocodecs"></a>
## *videocodecs [<sup>method</sup>](#head.Methods)*

Returns the video codec supported by the platform.
 
### Events 
 
No Events.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | array |  |
| result[#] | string |  (must be one of the following: *VideoUndefined*, *VideoH263*, *VideoH264*, *VideoH265*, *VideoH26510*, *VideoMpeg*, *VideoVp8*, *VideoVp9*, *VideoVp10*) |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "PlayerInfo.1.videocodecs"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": [
        "VideoUndefined"
    ]
}
```

<a name="head.Properties"></a>
# Properties

The following properties are provided by the PlayerInfo plugin:

PlayerInfo interface properties:

| Property | Description |
| :-------- | :-------- |
| [playerinfo](#property.playerinfo) <sup>RO</sup> | Player general information |
| [resolution](#property.resolution) <sup>RO</sup> | Current configured video output port resolution |
| [isaudioequivalenceenabled](#property.isaudioequivalenceenabled) <sup>RO</sup> | Check for Loudness Equivalence in the platform |
| [dolby atmosmetadata](#property.dolby_atmosmetadata) <sup>RO</sup> | Atmos capabilities of Sink |
| [dolby soundmode](#property.dolby_soundmode) <sup>RO</sup> | Current sound mode |
| [dolby enableatmosoutput](#property.dolby_enableatmosoutput) <sup>WO</sup> | Audio output enablement for Atmos |
| [dolby mode](#property.dolby_mode) | Dolby mode |


<a name="property.playerinfo"></a>
## *playerinfo [<sup>property</sup>](#head.Properties)*

Provides access to the player general information.

> This property is **read-only**.

### Value

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| (property) | object | Player general information |
| (property).audio | array |  |
| (property).audio[#] | string | Audio Codec supported by the platform (must be one of the following: *Undefined*, *AAC*, *AC3*, *AC3Plus*, *DTS*, *MPEG1*, *MPEG2*, *MPEG3*, *MPEG4*, *OPUS*, *VorbisOGG*, *WAV*) |
| (property).video | array |  |
| (property).video[#] | string | Video Codec supported by the platform (must be one of the following: *Undefined*, *H263*, *H264*, *H265*, *H26510*, *MPEG*, *VP8*, *VP9*, *VP10*) |

### Example

#### Get Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "PlayerInfo.1.playerinfo"
}
```

#### Get Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "audio": [
            "AudioAAC"
        ],
        "video": [
            "VideoH264"
        ]
    }
}
```

<a name="property.resolution"></a>
## *resolution [<sup>property</sup>](#head.Properties)*

Provides access to the current configured video output port resolution.

> This property is **read-only**.

### Value

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| (property) | string | Current configured video output port resolution (must be one of the following: *ResolutionUnknown*, *Resolution480I*, *Resolution480P*, *Resolution576I*, *Resolution576P*, *Resolution720P*, *Resolution1080I*, *Resolution1080P*, *Resolution2160P30*, *Resolution2160P60*) |

### Example

#### Get Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "PlayerInfo.1.resolution"
}
```

#### Get Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": "ResolutionUnknown"
}
```

<a name="property.isaudioequivalenceenabled"></a>
## *isaudioequivalenceenabled [<sup>property</sup>](#head.Properties)*

Provides access to the check for Loudness Equivalence in the platform.

> This property is **read-only**.

### Value

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| (property) | boolean | Check for Loudness Equivalence in the platform |

### Example

#### Get Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "PlayerInfo.1.isaudioequivalenceenabled"
}
```

#### Get Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": false
}
```

<a name="property.dolby_atmosmetadata"></a>
## *dolby_atmosmetadata [<sup>property</sup>](#head.Properties)*

Provides access to the atmos capabilities of Sink.

> This property is **read-only**.

### Value

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| (property) | boolean | Atmos capabilities of Sink |

### Example

#### Get Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "PlayerInfo.1.dolby_atmosmetadata"
}
```

#### Get Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": false
}
```

<a name="property.dolby_soundmode"></a>
## *dolby_soundmode [<sup>property</sup>](#head.Properties)*

Provides access to the current sound mode.

> This property is **read-only**.

### Value

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| (property) | string | Current sound mode (must be one of the following: *Unknown*, *Mono*, *Stereo*, *Surround*, *Passthru*) |

### Example

#### Get Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "PlayerInfo.1.dolby_soundmode"
}
```

#### Get Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": "Unknown"
}
```

<a name="property.dolby_enableatmosoutput"></a>
## *dolby_enableatmosoutput [<sup>property</sup>](#head.Properties)*

Provides access to the audio output enablement for Atmos.

> This property is **write-only**.

### Value

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| (property) | boolean | Audio output enablement for Atmos |

### Example

#### Set Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "PlayerInfo.1.dolby_enableatmosoutput",
    "params": false
}
```

#### Set Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": "null"
}
```

<a name="property.dolby_mode"></a>
## *dolby_mode [<sup>property</sup>](#head.Properties)*

Provides access to the dolby mode.

### Value

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| (property) | string | Dolby mode (must be one of the following: *DigitalPcm*, *DigitalPlus*, *DigitalAc3*, *Auto*, *Ms12*) |

### Example

#### Get Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "PlayerInfo.1.dolby_mode"
}
```

#### Get Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": "DigitalPcm"
}
```

#### Set Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "PlayerInfo.1.dolby_mode",
    "params": "DigitalPcm"
}
```

#### Set Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": "null"
}
```

<a name="head.Notifications"></a>
# Notifications

Notifications are autonomous events, triggered by the internals of the implementation, and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the PlayerInfo plugin:

PlayerInfo interface events:

| Event | Description |
| :-------- | :-------- |
| [dolby audiomodechanged](#event.dolby_audiomodechanged) | Triggered after the audio sound mode changes |


<a name="event.dolby_audiomodechanged"></a>
## *dolby_audiomodechanged [<sup>event</sup>](#head.Notifications)*

Triggered after the audio sound mode changes.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.mode | string | The sound mode (must be one of the following: *Unknown*, *Mono*, *Stereo*, *Surround*, *Passthru*) |
| params.enable | boolean |  |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.dolby_audiomodechanged",
    "params": {
        "mode": "Unknown",
        "enable": true
    }
}
```

