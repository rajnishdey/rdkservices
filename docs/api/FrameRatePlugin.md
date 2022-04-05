<!-- Generated automatically, DO NOT EDIT! -->
<a name="head.FrameRate_Plugin"></a>
# FrameRate Plugin

**Version: 2.0**

**Status: :black_circle::black_circle::black_circle:**

A org.rdk.FrameRate plugin for Thunder framework.

### Table of Contents

- [Introduction](#head.Introduction)
- [Description](#head.Description)
- [Configuration](#head.Configuration)
- [Methods](#head.Methods)
- [Notifications](#head.Notifications)

<a name="head.Introduction"></a>
# Introduction

<a name="head.Scope"></a>
## Scope

This document describes purpose and functionality of the org.rdk.FrameRate plugin. It includes detailed specification about its configuration, methods provided and notifications sent.

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

The `FrameRate` plugin allows you to collect FPS data.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a name="head.Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| callsign | string | Plugin instance name (default: *org.rdk.FrameRate*) |
| classname | string | Class name: *org.rdk.FrameRate* |
| locator | string | Library name: *libWPEFrameworkFrameRate.so* |
| autostart | boolean | Determines if the plugin shall be started automatically along with the framework |

<a name="head.Methods"></a>
# Methods

The following methods are provided by the org.rdk.FrameRate plugin:

FrameRate interface methods:

| Method | Description |
| :-------- | :-------- |
| [getDisplayFrameRate](#method.getDisplayFrameRate) | (Version 2) Returns the current display frame rate values |
| [getFrmMode](#method.getFrmMode) | (Version 2) Returns the current auto framerate mode |
| [setCollectionFrequency](#method.setCollectionFrequency) | Sets the FPS data collection interval |
| [setDisplayFrameRate](#method.setDisplayFrameRate) | (Version 2) Sets the display framerate values |
| [setFrmMode](#method.setFrmMode) | (Version 2) Sets the auto framerate mode |
| [startFpsCollection](#method.startFpsCollection) | Starts the FPS data collection |
| [stopFpsCollection](#method.stopFpsCollection) | Stops the FPS data collection |
| [updateFps](#method.updateFps) | Updates Fps values |


<a name="method.getDisplayFrameRate"></a>
## *getDisplayFrameRate [<sup>method</sup>](#head.Methods)*

(Version 2) Returns the current display frame rate values.
  
### Events 

 No events.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.framerate | string | The display framerate setting (width x height x framerate) |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.FrameRate.1.getDisplayFrameRate"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "framerate": "3840x2160px48",
        "success": true
    }
}
```

<a name="method.getFrmMode"></a>
## *getFrmMode [<sup>method</sup>](#head.Methods)*

(Version 2) Returns the current auto framerate mode.
  
### Events 

 No events.

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.auto-frm-mode | integer | `0` for auto framerate mode disabled, `1` for auto framerate mode enabled (must be one of the following: *0*, *1*) |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.FrameRate.1.getFrmMode"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "auto-frm-mode": 0,
        "success": true
    }
}
```

<a name="method.setCollectionFrequency"></a>
## *setCollectionFrequency [<sup>method</sup>](#head.Methods)*

Sets the FPS data collection interval.
  
### Events 

 No events.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.frequency | integer | The amount of time in milliseconds. The default frequency is 10000 milliseconds |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.FrameRate.1.setCollectionFrequency",
    "params": {
        "frequency": 1000
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.setDisplayFrameRate"></a>
## *setDisplayFrameRate [<sup>method</sup>](#head.Methods)*

(Version 2) Sets the display framerate values. 
 
### Events 
| Event | Description | 
| :----------- | :----------- |
| `onDisplayFrameRateChanging`|Triggered when the framerate changes started.| 
| `onDisplayFrameRateChanged`|Triggered when the framerate changed.|.

Also see: [onDisplayFrameRateChanging](#event.onDisplayFrameRateChanging), [onDisplayFrameRateChanged](#event.onDisplayFrameRateChanged)

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.framerate | string | The display framerate setting (width x height x framerate) |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.FrameRate.1.setDisplayFrameRate",
    "params": {
        "framerate": "3840x2160px48"
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.setFrmMode"></a>
## *setFrmMode [<sup>method</sup>](#head.Methods)*

(Version 2) Sets the auto framerate mode.
  
### Events 

 No events.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.frmmode | integer | `0` for auto framerate mode disabled, `1` for auto framerate mode enabled (must be one of the following: *0*, *1*) |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.FrameRate.1.setFrmMode",
    "params": {
        "frmmode": 0
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.startFpsCollection"></a>
## *startFpsCollection [<sup>method</sup>](#head.Methods)*

Starts the FPS data collection.
 
### Events 
| Event | Description | 
| :----------- | :----------- |
| `onFpsEvent`|Triggered at the end of each interval as defined by the `setCollectionFrequency` method.|.

Also see: [onFpsEvent](#event.onFpsEvent)

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.FrameRate.1.startFpsCollection"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.stopFpsCollection"></a>
## *stopFpsCollection [<sup>method</sup>](#head.Methods)*

Stops the FPS data collection.
 
### Events 
| Event | Description | 
| :----------- | :----------- |
| `onFpsEvent`|Triggered once after the `stopFpsCollection` method is invoked.|.

Also see: [onFpsEvent](#event.onFpsEvent)

### Parameters

This method takes no parameters.

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.FrameRate.1.stopFpsCollection"
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="method.updateFps"></a>
## *updateFps [<sup>method</sup>](#head.Methods)*

Updates Fps values.
  
### Events 

 No events.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.newFpsValue | integer | New Frames per Second (Fps) value |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.success | boolean | Whether the request succeeded |

### Example

#### Request

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "method": "org.rdk.FrameRate.1.updateFps",
    "params": {
        "newFpsValue": 60
    }
}
```

#### Response

```json
{
    "jsonrpc": "2.0",
    "id": 42,
    "result": {
        "success": true
    }
}
```

<a name="head.Notifications"></a>
# Notifications

Notifications are autonomous events, triggered by the internals of the implementation, and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the org.rdk.FrameRate plugin:

FrameRate interface events:

| Event | Description |
| :-------- | :-------- |
| [onDisplayFrameRateChanging](#event.onDisplayFrameRateChanging) | Triggered when the framerate changes started |
| [onDisplayFrameRateChanged](#event.onDisplayFrameRateChanged) | Triggered when the framerate changed |
| [onFpsEvent](#event.onFpsEvent) | Triggered at the end of each interval as defined by the `setCollectionFrequency` method and once after the `stopFpsCollection` method is invoked |


<a name="event.onDisplayFrameRateChanging"></a>
## *onDisplayFrameRateChanging [<sup>event</sup>](#head.Notifications)*

Triggered when the framerate changes started.

### Parameters

This event carries no parameters.

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.onDisplayFrameRateChanging"
}
```

<a name="event.onDisplayFrameRateChanged"></a>
## *onDisplayFrameRateChanged [<sup>event</sup>](#head.Notifications)*

Triggered when the framerate changed.

### Parameters

This event carries no parameters.

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.onDisplayFrameRateChanged"
}
```

<a name="event.onFpsEvent"></a>
## *onFpsEvent [<sup>event</sup>](#head.Notifications)*

Triggered at the end of each interval as defined by the `setCollectionFrequency` method and once after the `stopFpsCollection` method is invoked.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.average | integer | The average FPS |
| params.min | integer | The minimum FPS |
| params.max | integer | The maximum FPS |

### Example

```json
{
    "jsonrpc": "2.0",
    "method": "client.events.1.onFpsEvent",
    "params": {
        "average": 0,
        "min": 0,
        "max": 0
    }
}
```

