# AsyncWebServer_RP2040W

[![arduino-library-badge](https://www.ardu-badge.com/badge/AsyncWebServer_RP2040W.svg?)](https://www.ardu-badge.com/AsyncWebServer_RP2040W)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/AsyncWebServer_RP2040W.svg)](https://github.com/khoih-prog/AsyncWebServer_RP2040W/releases)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/AsyncWebServer_RP2040W.svg)](http://github.com/khoih-prog/AsyncWebServer_RP2040W/issues)


<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>


---
---

## Table of contents

* [Table of contents](#table-of-contents)
* [Important Note from v1.4.0](#Important-Note-from-v140)
* [Important Note from v1.2.0](#Important-Note-from-v120)
* [Why do we need this AsyncWebServer_RP2040W library](#why-do-we-need-this-AsyncWebServer_RP2040W-library)
  * [Features](#features)
  * [Why Async is better](#why-async-is-better)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Important things to remember](#important-things-to-remember)
* [Principles of operation](#principles-of-operation)
  * [The Async Web server](#the-async-web-server)
  * [Request Life Cycle](#request-life-cycle)
  * [Rewrites and how do they work](#rewrites-and-how-do-they-work)
  * [Handlers and how do they work](#handlers-and-how-do-they-work)
  * [Responses and how do they work](#responses-and-how-do-they-work)
  * [Template processing](#template-processing)
* [Request Variables](#request-variables)
  * [Common Variables](#common-variables)
  * [Headers](#headers)
  * [GET, POST and FILE parameters](#get-post-and-file-parameters)
  * [JSON body handling with ArduinoJson](#json-body-handling-with-arduinojson)
* [Responses](#responses)
  * [Redirect to another URL](#redirect-to-another-url)
  * [Basic response with HTTP Code](#basic-response-with-http-code)
  * [Basic response with HTTP Code and extra headers](#basic-response-with-http-code-and-extra-headers)
  * [Basic response with string content](#basic-response-with-string-content)
  * [Basic response with string content and extra headers](#basic-response-with-string-content-and-extra-headers)
  * [Respond with content coming from a Stream](#respond-with-content-coming-from-a-stream)
  * [Respond with content coming from a Stream and extra headers](#respond-with-content-coming-from-a-stream-and-extra-headers)
  * [Respond with content coming from a Stream containing templates](#respond-with-content-coming-from-a-stream-containing-templates)
  * [Respond with content coming from a Stream containing templates and extra headers](#respond-with-content-coming-from-a-stream-containing-templates-and-extra-headers)
  * [Respond with content using a callback](#respond-with-content-using-a-callback)
  * [Respond with content using a callback and extra headers](#respond-with-content-using-a-callback-and-extra-headers)
  * [Respond with content using a callback containing templates](#respond-with-content-using-a-callback-containing-templates)
  * [Respond with content using a callback containing templates and extra headers](#respond-with-content-using-a-callback-containing-templates-and-extra-headers)
  * [Chunked Response](#chunked-response)
  * [Chunked Response containing templates](#chunked-response-containing-templates)
  * [Print to response](#print-to-response)
  * [ArduinoJson Basic Response](#arduinojson-basic-response)
  * [ArduinoJson Advanced Response](#arduinojson-advanced-response)
* [Param Rewrite With Matching](#param-rewrite-with-matching)
* [Using filters](#using-filters)
* [Bad Responses](#bad-responses)
  * [Respond with content using a callback without content length to HTTP/1.0 clients](#respond-with-content-using-a-callback-without-content-length-to-http10-clients)
* [Async WebSocket Plugin](#async-websocket-plugin)
  * [Async WebSocket Event](#async-websocket-event)
  * [Methods for sending data to a socket client](#methods-for-sending-data-to-a-socket-client)
  * [Direct access to web socket message buffer](#direct-access-to-web-socket-message-buffer)
  * [Limiting the number of web socket clients](#limiting-the-number-of-web-socket-clients)
* [Async Event Source Plugin](#async-event-source-plugin)
  * [Setup Event Source on the server](#setup-event-source-on-the-server)
  * [Setup Event Source in the browser](#setup-event-source-in-the-browser)
* [Remove handlers and rewrites](#remove-handlers-and-rewrites)
* [Setting up the server](#setting-up-the-server)
  * [Setup global and class functions as request handlers](#setup-global-and-class-functions-as-request-handlers)
  * [Methods for controlling websocket connections](#methods-for-controlling-websocket-connections)
  * [Adding Default Headers](#adding-default-headers)
  * [Path variable](#path-variable)
* [Examples](#examples)
  * [ 1. Async_AdvancedWebServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer)
  * [ 2. Async_HelloServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_HelloServer)
  * [ 3. Async_HelloServer2](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_HelloServer2)
  * [ 4. Async_HttpBasicAuth](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_HttpBasicAuth)
  * [ 5. Async_PostServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_PostServer)
  * [ 6. **MQTTClient_Auth**](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/MQTTClient_Auth)
  * [ 7. **MQTTClient_Basic**](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/MQTTClient_Basic)
  * [ 8. **MQTT_ThingStream**](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/MQTT_ThingStream)
  * [ 9. WebClient](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/WebClient)
  * [10. WebClientRepeating](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/WebClientRepeating)
  * [11. Async_AdvancedWebServer_Country](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_Country)
  * [12. Async_AdvancedWebServer_favicon](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_favicon)
  * [13. Async_AdvancedWebServer_MemoryIssues_SendArduinoString](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_MemoryIssues_SendArduinoString)
  * [14. Async_AdvancedWebServer_MemoryIssues_Send_CString](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_MemoryIssues_Send_CString)
  * [15. Async_WebSocketsServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_WebSocketsServer)
  * [16. Async_WebSocketsServer_Xtreme](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_WebSocketsServer_Xtreme)
  * [17. AsyncFSWebServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncFSWebServer)
  * [18. AsyncFSWebServer_Complex](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncFSWebServer_Complex)
  * [19. Async_AdvancedWebServer_SendChunked](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_SendChunked)
  * [20. AsyncWebServer_SendChunked](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncWebServer_SendChunked)
  * [21. AsyncWebServer_MQTT_RP2040W](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncWebServer_MQTT_RP2040W) **New**
  * [22. Async_AdvancedWebServer_SendChunked_MQTT](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_SendChunked_MQTT) **New**
* [Example Async_AdvancedWebServer](#Example-Async_AdvancedWebServer)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. Async_AdvancedWebServer on RASPBERRY_PI_PICO_W using CYW43439 WiFi](#1-Async_AdvancedWebServer-on-RASPBERRY_PI_PICO_W-using-CYW43439-WiFi)
  * [2. WebClient on RASPBERRY_PI_PICO_W using CYW43439 WiFi](#2-WebClient-on-RASPBERRY_PI_PICO_W-using-CYW43439-WiFi)
  * [3. MQTTClient_Auth on RASPBERRY_PI_PICO_W using CYW43439 WiFi](#3-MQTTClient_Auth-on-RASPBERRY_PI_PICO_W-using-CYW43439-WiFi)
  * [4. MQTTClient_Basic on RASPBERRY_PI_PICO_W using CYW43439 WiFi](#4-MQTTClient_Basic-on-RASPBERRY_PI_PICO_W-using-CYW43439-WiFi)
  * [5. MQTT_ThingStream on RASPBERRY_PI_PICO_W using CYW43439 WiFi](#5-MQTT_ThingStream-on-RASPBERRY_PI_PICO_W-using-CYW43439-WiFi)
  * [6. Async_AdvancedWebServer_Country on RASPBERRY_PI_PICO_W using CYW43439 WiFi](#6-Async_AdvancedWebServer_Country-on-RASPBERRY_PI_PICO_W-using-CYW43439-WiFi)
  * [7. Async_AdvancedWebServer_favicon on RASPBERRY_PI_PICO_W using CYW43439 WiFi](#7-Async_AdvancedWebServer_favicon-on-RASPBERRY_PI_PICO_W-using-CYW43439-WiFi)
  * [8. Async_AdvancedWebServer_MemoryIssues_Send_CString on RASPBERRY_PI_PICO_W](#8-Async_AdvancedWebServer_MemoryIssues_Send_CString-on-RASPBERRY_PI_PICO_W)
  * [9. Async_WebSocketsServer on RASPBERRY_PI_PICO_W using CYW43439 WiFi](#9-Async_WebSocketsServer-on-RASPBERRY_PI_PICO_W-using-CYW43439-WiFi)
  * [10. Async_WebSocketsServer_Xtreme on RASPBERRY_PI_PICO_W using CYW43439 WiFi](#10-Async_WebSocketsServer_Xtreme-on-RASPBERRY_PI_PICO_W-using-CYW43439-WiFi)
  * [11. AsyncFSWebServer_Complex on RASPBERRY_PI_PICO_W using CYW43439 WiFi](#11-AsyncFSWebServer_Complex-on-RASPBERRY_PI_PICO_W-using-CYW43439-WiFi)
  * [12. Async_AdvancedWebServer_SendChunked on RASPBERRY_PI_PICO_W using CYW43439 WiFi](#12-Async_AdvancedWebServer_SendChunked-on-RASPBERRY_PI_PICO_W-using-CYW43439-WiFi)
  * [13. AsyncWebServer_SendChunked on RASPBERRY_PI_PICO_W using CYW43439 WiFi](#13-AsyncWebServer_SendChunked-on-RASPBERRY_PI_PICO_W-using-CYW43439-WiFi)
  * [14. Async_AdvancedWebServer_SendChunked_MQTT on RASPBERRY_PI_PICO_W using CYW43439 WiFi](#14-Async_AdvancedWebServer_SendChunked_MQTT-on-RASPBERRY_PI_PICO_W-using-CYW43439-WiFi)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Important Note from v1.4.0

The new `v1.4.0+` has added a new and powerful feature to use `LittleFS` functions, such as AsyncFSWebServer

Check these new examples

1. [AsyncFSWebServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncFSWebServer)
2. [AsyncFSWebServer_Complex](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncFSWebServer_Complex)

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_RP2040W/raw/main/pics/AsyncFSWebServer_Complex.png">
</p>

---

### Important Note from v1.2.0

The new `v1.2.0+` has added a new and powerful feature to permit using `CString` to save heap to send `very large data`.

Check the `marvelleous` PRs of **@salasidis** in [Portenta_H7_AsyncWebServer library](https://github.com/khoih-prog/Portenta_H7_AsyncWebServer)
- [request->send(200, textPlainStr, jsonChartDataCharStr); - Without using String Class - to save heap #8](https://github.com/khoih-prog/Portenta_H7_AsyncWebServer/pull/8)
- [All memmove() removed - string no longer destroyed #11](https://github.com/khoih-prog/Portenta_H7_AsyncWebServer/pull/11)

and these new examples

1. [Async_AdvancedWebServer_MemoryIssues_Send_CString](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_MemoryIssues_Send_CString)
2. [Async_AdvancedWebServer_MemoryIssues_SendArduinoString](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_MemoryIssues_SendArduinoString)

If using Arduino String, to send a buffer around 30 KBytes, the used `Max Heap` is around **75,264 bytes**

If using CString in regular memory, with the same 30 KBytes, the used `Max Heap` is around **44,000 bytes, saving around a buffer size (30 KBytes)**

This is very critical in use-cases where sending `very large data` is necessary, without `heap-allocation-error`.


1. The traditional function used to send `Arduino String` is

https://github.com/khoih-prog/AsyncWebServer_RP2040W/blob/e98fdf0f1aa79f17071822522cb1a50ce4fdf6f4/src/AsyncWebServer_RP2040W.h#L414

```cpp
void send(int code, const String& contentType = String(), const String& content = String());
```

such as

```cpp
request->send(200, textPlainStr, ArduinoStr);
```
The required additional HEAP is about **3 times of the String size**


2. To use `CString` with copying while sending. Use function

https://github.com/khoih-prog/AsyncWebServer_RP2040W/blob/e98fdf0f1aa79f17071822522cb1a50ce4fdf6f4/src/AsyncWebServer_RP2040W.h#L415

```cpp
void send(int code, const String& contentType, const char *content, bool nonDetructiveSend = true);    // RSMOD
```

such as 

```cpp
request->send(200, textPlainStr, cStr);
```

The required additional HEAP is also about **2 times of the CString size** because of `unnecessary copies` of the CString in HEAP. Avoid this `unefficient` way.


3. To use `CString` without copying while sending. Use function

https://github.com/khoih-prog/AsyncWebServer_RP2040W/blob/e98fdf0f1aa79f17071822522cb1a50ce4fdf6f4/src/AsyncWebServer_RP2040W.h#L415

```cpp
void send(int code, const String& contentType, const char *content, bool nonDetructiveSend = true);    // RSMOD
```

such as 

```cpp
request->send(200, textPlainStr, cStr, false);
```

The required additional HEAP is about **1 times of the CString size**. This way is the best and **most efficient way** to use by avoiding of `unnecessary copies` of the CString in HEAP


---
---

### Why do we need this [AsyncWebServer_RP2040W library](https://github.com/khoih-prog/AsyncWebServer_RP2040W)

#### Features

This library is based on, modified from:

1. [Hristo Gochkov's ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)

to apply the better and faster **asynchronous** feature of the **powerful** [ESPAsyncWebServer Library](https://github.com/me-no-dev/ESPAsyncWebServer) into **RASPBERRY_PI_PICO_W**. Thus [AsyncWebServer_RP2040W](https://github.com/khoih-prog/AsyncWebServer_RP2040W) is part of a series of advanced Async libraries, such as AsyncTCP_RP2040W, AsyncUDP_RP2040W, AsyncWebServer_RP2040W, AsyncHTTPRequest_RP2040W, AsyncHTTPSRequest_RP2040W, etc. to be written or modified to support **RASPBERRY_PI_PICO_W**, using `CYW43439 WiFi`.


#### Why Async is better

- Using asynchronous network means that you can handle **more than one connection at the same time**
- **You are called once the request is ready and parsed**
- When you send the response, you are **immediately ready** to handle other connections while the server is taking care of sending the response in the background
- **Speed is OMG**
- **Easy to use API, HTTP Basic and Digest MD5 Authentication (default), ChunkedResponse**
- Easily extensible to handle **any type of content**
- Supports Continue 100
- **Async WebSocket plugin offering different locations without extra servers or ports**
- Async EventSource (Server-Sent Events) plugin to send events to the browser
- URL Rewrite plugin for conditional and permanent url rewrites
- ServeStatic plugin that supports cache, Last-Modified, default index and more
- Simple template processing engine to handle templates


---

### Currently supported Boards

1. **RASPBERRY_PI_PICO_W with CYW43439 WiFi** using [**arduino-pico core v2.4.0+**](https://github.com/earlephilhower/arduino-pico)

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_RP2040W/raw/main/pics/RP2040W-pinout.svg">
</p>

---
---

## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
 2. [`Earle Philhower's arduino-pico core v2.7.1+`](https://github.com/earlephilhower/arduino-pico) for **RASPBERRY_PI_PICO_W with CYW43439 WiFi**, etc. [![GitHub release](https://img.shields.io/github/release/earlephilhower/arduino-pico.svg)](https://github.com/earlephilhower/arduino-pico/releases/latest)
 3. [`AsyncTCP_RP2040W library v1.1.0+`](https://github.com/khoih-prog/AsyncTCP_RP2040W) for RASPBERRY_PI_PICO_W with CYW43439 WiFi. [![GitHub release](https://img.shields.io/github/release/khoih-prog/AsyncTCP_RP2040W.svg)](https://github.com/khoih-prog/AsyncTCP_RP2040W/releases/latest)
 4. [`AsyncMQTT_Generic library v1.8.1+`](https://github.com/khoih-prog/AsyncMQTT_Generic) to use with some examples. [![GitHub release](https://img.shields.io/github/release/khoih-prog/AsyncMQTT_Generic.svg)](https://github.com/khoih-prog/AsyncMQTT_Generic/releases/latest)

---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for `AsyncWebServer_RP2040W`, then select / install the latest version. You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/AsyncWebServer_RP2040W.svg?)](https://www.ardu-badge.com/AsyncWebServer_RP2040W) for more detailed instructions.

### Manual Install

1. Navigate to [AsyncWebServer_RP2040W](https://github.com/khoih-prog/AsyncWebServer_RP2040W) page.
2. Download the latest release `AsyncWebServer_RP2040W-main.zip`.
3. Extract the zip file to `AsyncWebServer_RP2040W-main` directory 
4. Copy the whole `AsyncWebServer_RP2040W-main` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**AsyncWebServer_RP2040W** library](https://registry.platformio.org/libraries/khoih-prog/AsyncWebServer_RP2040W) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/AsyncWebServer_RP2040W/installation). Search for **AsyncWebServer_RP2040W** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

## Important things to remember

- This is fully asynchronous server and as such does not run on the `loop()` thread.
- You can not use `yield()` or `delay()` or any function that uses them inside the callbacks
- The server is smart enough to know when to close the connection and free resources
- You can not send more than one response to a single request

---

## Principles of operation

### The Async Web server

- Listens for connections
- Wraps the new clients into `Request`
- Keeps track of clients and cleans memory
- Manages `Rewrites` and apply them on the request url
- Manages `Handlers` and attaches them to Requests

### Request Life Cycle

- TCP connection is received by the server
- The connection is wrapped inside `Request` object
- When the request head is received (type, url, get params, http version and host),
  the server goes through all `Rewrites` (in the order they were added) to rewrite the url and inject query parameters,
  next, it goes through all attached `Handlers`(in the order they were added) trying to find one
  that `canHandle` the given request. If none are found, the default(catch-all) handler is attached.
- The rest of the request is received, calling the `handleUpload` or `handleBody` methods of the `Handler` if they are needed (POST+File/Body)
- When the whole request is parsed, the result is given to the `handleRequest` method of the `Handler` and is ready to be responded to
- In the `handleRequest` method, to the `Request` is attached a `Response` object (see below) that will serve the response data back to the client
- When the `Response` is sent, the client is closed and freed from the memory

### Rewrites and how do they work

- The `Rewrites` are used to rewrite the request url and/or inject get parameters for a specific request url path.
- All `Rewrites` are evaluated on the request in the order they have been added to the server.
- The `Rewrite` will change the request url only if the request url (excluding get parameters) is fully match
  the rewrite url, and when the optional `Filter` callback return true.
- Setting a `Filter` to the `Rewrite` enables to control when to apply the rewrite, decision can be based on
  request url, http version, request host/port/target host, get parameters or the request client's localIP or remoteIP.
- The `Rewrite` can specify a target url with optional get parameters, e.g. `/to-url?with=params`

### Handlers and how do they work

- The `Handlers` are used for executing specific actions to particular requests
- One `Handler` instance can be attached to any request and lives together with the server
- Setting a `Filter` to the `Handler` enables to control when to apply the handler, decision can be based on
  request url, http version, request host/port/target host, get parameters or the request client's localIP or remoteIP.
- The `canHandle` method is used for handler specific control on whether the requests can be handled
  and for declaring any interesting headers that the `Request` should parse. Decision can be based on request
  method, request url, http version, request host/port/target host and get parameters
- Once a `Handler` is attached to given `Request` (`canHandle` returned true)
  that `Handler` takes care to receive any file/data upload and attach a `Response`
  once the `Request` has been fully parsed
- `Handlers` are evaluated in the order they are attached to the server. The `canHandle` is called only
  if the `Filter` that was set to the `Handler` return true.
- The first `Handler` that can handle the request is selected, not further `Filter` and `canHandle` are called.

### Responses and how do they work

- The `Response` objects are used to send the response data back to the client
- The `Response` object lives with the `Request` and is freed on end or disconnect
- Different techniques are used depending on the response type to send the data in packets
  returning back almost immediately and sending the next packet when this one is received.
  Any time in between is spent to run the user loop and handle other network packets
- Responding asynchronously is probably the most difficult thing for most to understand
- Many different options exist for the user to make responding a background task

### Template processing

- `AsyncWebServer_RP2040W` contains simple template processing engine.
- Template processing can be added to most response types.
- Currently it supports only replacing template placeholders with actual values. No conditional processing, cycles, etc.
- Placeholders are delimited with `%` symbols. Like this: `%TEMPLATE_PLACEHOLDER%`.
- It works by extracting placeholder name from response text and passing it to user provided function which should return actual value to be used instead of placeholder.
- Since it's user provided function, it is possible for library users to implement conditional processing and cycles themselves.
- Since it's impossible to know the actual response size after template processing step in advance (and, therefore, to include it in response headers), the response becomes [chunked](#chunked-response).

---

## Request Variables

### Common Variables

```cpp
request->version();       // uint8_t: 0 = HTTP/1.0, 1 = HTTP/1.1
request->method();        // enum:    HTTP_GET, HTTP_POST, HTTP_DELETE, HTTP_PUT, HTTP_PATCH, HTTP_HEAD, HTTP_OPTIONS
request->url();           // String:  URL of the request (not including host, port or GET parameters)
request->host();          // String:  The requested host (can be used for virtual hosting)
request->contentType();   // String:  ContentType of the request (not available in Handler::canHandle)
request->contentLength(); // size_t:  ContentLength of the request (not available in Handler::canHandle)
request->multipart();     // bool:    True if the request has content type "multipart"
```

### Headers

```cpp
//List all collected headers
int headers = request->headers();
int i;

for(i=0;i<headers;i++)
{
  AsyncWebHeader* h = request->getHeader(i);
  Serial.printf("HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
}

//get specific header by name
if(request->hasHeader("MyHeader"))
{
  AsyncWebHeader* h = request->getHeader("MyHeader");
  Serial.printf("MyHeader: %s\n", h->value().c_str());
}

//List all collected headers (Compatibility)
int headers = request->headers();
int i;

for(i=0;i<headers;i++)
{
  Serial.printf("HEADER[%s]: %s\n", request->headerName(i).c_str(), request->header(i).c_str());
}

//get specific header by name (Compatibility)
if(request->hasHeader("MyHeader"))
{
  Serial.printf("MyHeader: %s\n", request->header("MyHeader").c_str());
}
```

### GET, POST and FILE parameters

```cpp
//List all parameters
int params = request->params();

for(int i=0;i<params;i++)
{
  AsyncWebParameter* p = request->getParam(i);
  
  if(p->isFile())
  { 
    //p->isPost() is also true
    Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
  } 
  else if(p->isPost())
  {
    Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
  } 
  else 
  {
    Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
  }
}

//Check if GET parameter exists
if(request->hasParam("download"))
  AsyncWebParameter* p = request->getParam("download");

//Check if POST (but not File) parameter exists
if(request->hasParam("download", true))
  AsyncWebParameter* p = request->getParam("download", true);

//Check if FILE was uploaded
if(request->hasParam("download", true, true))
  AsyncWebParameter* p = request->getParam("download", true, true);

//List all parameters (Compatibility)
int args = request->args();

for(int i=0;i<args;i++)
{
  Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
}

//Check if parameter exists (Compatibility)
if(request->hasArg("download"))
  String arg = request->arg("download");
```

### JSON body handling with ArduinoJson

Endpoints which consume JSON can use a special handler to get ready to use JSON data in the request callback:

```cpp
#include "AsyncJson.h"
#include "ArduinoJson.h"

AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/rest/endpoint", [](AsyncWebServerRequest *request, JsonVariant &json) 
{
  JsonObject& jsonObj = json.as<JsonObject>();
  // ...
});

server.addHandler(handler);
```
---

## Responses

### Redirect to another URL

```cpp
//to local url
request->redirect("/login");

//to external url
request->redirect("http://esp8266.com");
```

### Basic response with HTTP Code

```cpp
request->send(404); //Sends 404 File Not Found
```

### Basic response with HTTP Code and extra headers

```cpp
AsyncWebServerResponse *response = request->beginResponse(404); //Sends 404 File Not Found
response->addHeader("Server","AsyncWebServer_RP2040W");
request->send(response);
```

### Basic response with string content

```cpp
request->send(200, "text/plain", "Hello World!");
```

### Basic response with string content and extra headers

```cpp
AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "Hello World!");
response->addHeader("Server","AsyncWebServer");
request->send(response);
```

### Respond with content coming from a Stream

```cpp
//read 12 bytes from Serial and send them as Content Type text/plain
request->send(Serial, "text/plain", 12);
```

### Respond with content coming from a Stream and extra headers

```cpp
//read 12 bytes from Serial and send them as Content Type text/plain
AsyncWebServerResponse *response = request->beginResponse(Serial, "text/plain", 12);
response->addHeader("Server","AsyncWebServer_RP2040W");
request->send(response);
```

### Respond with content coming from a Stream containing templates

```cpp
String processor(const String& var)
{
  if(var == "HELLO_FROM_TEMPLATE")
    return F("Hello world!");
    
  return String();
}

// ...

//read 12 bytes from Serial and send them as Content Type text/plain
request->send(Serial, "text/plain", 12, processor);
```

### Respond with content coming from a Stream containing templates and extra headers

```cpp
String processor(const String& var)
{
  if(var == "HELLO_FROM_TEMPLATE")
    return F("Hello world!");
  return String();
}

// ...

//read 12 bytes from Serial and send them as Content Type text/plain
AsyncWebServerResponse *response = request->beginResponse(Serial, "text/plain", 12, processor);
response->addHeader("Server","AsyncWebServer_RP2040W");
request->send(response);
```

### Respond with content using a callback

```cpp
//send 128 bytes as plain text
request->send("text/plain", 128, [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
{
  //Write up to "maxLen" bytes into "buffer" and return the amount written.
  //index equals the amount of bytes that have been already sent
  //You will not be asked for more bytes once the content length has been reached.
  //Keep in mind that you can not delay or yield waiting for more data!
  //Send what you currently have and you will be asked for more again
  return mySource.read(buffer, maxLen);
});
```

### Respond with content using a callback and extra headers

```cpp
//send 128 bytes as plain text
AsyncWebServerResponse *response = request->beginResponse("text/plain", 128, [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
{
  //Write up to "maxLen" bytes into "buffer" and return the amount written.
  //index equals the amount of bytes that have been already sent
  //You will not be asked for more bytes once the content length has been reached.
  //Keep in mind that you can not delay or yield waiting for more data!
  //Send what you currently have and you will be asked for more again
  return mySource.read(buffer, maxLen);
});

response->addHeader("Server","AsyncWebServer_RP2040W");
request->send(response);
```

### Respond with content using a callback containing templates

```cpp
String processor(const String& var)
{
  if(var == "HELLO_FROM_TEMPLATE")
    return F("Hello world!");
    
  return String();
}

// ...

//send 128 bytes as plain text
request->send("text/plain", 128, [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
{
  //Write up to "maxLen" bytes into "buffer" and return the amount written.
  //index equals the amount of bytes that have been already sent
  //You will not be asked for more bytes once the content length has been reached.
  //Keep in mind that you can not delay or yield waiting for more data!
  //Send what you currently have and you will be asked for more again
  return mySource.read(buffer, maxLen);
}, processor);
```

### Respond with content using a callback containing templates and extra headers

```cpp
String processor(const String& var)
{
  if(var == "HELLO_FROM_TEMPLATE")
    return F("Hello world!");
  return String();
}

// ...

//send 128 bytes as plain text
AsyncWebServerResponse *response = request->beginResponse("text/plain", 128, [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
{
  //Write up to "maxLen" bytes into "buffer" and return the amount written.
  //index equals the amount of bytes that have been already sent
  //You will not be asked for more bytes once the content length has been reached.
  //Keep in mind that you can not delay or yield waiting for more data!
  //Send what you currently have and you will be asked for more again
  return mySource.read(buffer, maxLen);
}, processor);

response->addHeader("Server","AsyncWebServer_RP2040W");
request->send(response);
```

### Chunked Response

Used when content length is unknown. Works best if the client supports HTTP/1.1

```cpp
AsyncWebServerResponse *response = request->beginChunkedResponse("text/plain", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
{
  //Write up to "maxLen" bytes into "buffer" and return the amount written.
  //index equals the amount of bytes that have been already sent
  //You will be asked for more data until 0 is returned
  //Keep in mind that you can not delay or yield waiting for more data!
  return mySource.read(buffer, maxLen);
});

response->addHeader("Server","AsyncWebServer_RP2040W");
request->send(response);
```

### Chunked Response containing templates

Used when content length is unknown. Works best if the client supports HTTP/1.1

```cpp
String processor(const String& var)
{
  if(var == "HELLO_FROM_TEMPLATE")
    return F("Hello world!");
    
  return String();
}

// ...

AsyncWebServerResponse *response = request->beginChunkedResponse("text/plain", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
{
  //Write up to "maxLen" bytes into "buffer" and return the amount written.
  //index equals the amount of bytes that have been already sent
  //You will be asked for more data until 0 is returned
  //Keep in mind that you can not delay or yield waiting for more data!
  return mySource.read(buffer, maxLen);
}, processor);

response->addHeader("Server","AsyncWebServer_RP2040W");
request->send(response);
```

### Print to response

```cpp
AsyncResponseStream *response = request->beginResponseStream("text/html");
response->addHeader("Server","AsyncWebServer_RP2040W");
response->printf("<!DOCTYPE html><html><head><title>Webpage at %s</title></head><body>", request->url().c_str());

response->print("<h2>Hello ");
response->print(request->client()->remoteIP());
response->print("</h2>");

response->print("<h3>General</h3>");
response->print("<ul>");
response->printf("<li>Version: HTTP/1.%u</li>", request->version());
response->printf("<li>Method: %s</li>", request->methodToString());
response->printf("<li>URL: %s</li>", request->url().c_str());
response->printf("<li>Host: %s</li>", request->host().c_str());
response->printf("<li>ContentType: %s</li>", request->contentType().c_str());
response->printf("<li>ContentLength: %u</li>", request->contentLength());
response->printf("<li>Multipart: %s</li>", request->multipart()?"true":"false");
response->print("</ul>");

response->print("<h3>Headers</h3>");
response->print("<ul>");
int headers = request->headers();

for(int i=0;i<headers;i++)
{
  AsyncWebHeader* h = request->getHeader(i);
  response->printf("<li>%s: %s</li>", h->name().c_str(), h->value().c_str());
}

response->print("</ul>");

response->print("<h3>Parameters</h3>");
response->print("<ul>");

int params = request->params();

for(int i=0;i<params;i++)
{
  AsyncWebParameter* p = request->getParam(i);
  
  if(p->isFile())
  {
    response->printf("<li>FILE[%s]: %s, size: %u</li>", p->name().c_str(), p->value().c_str(), p->size());
  } 
  else if(p->isPost())
  {
    response->printf("<li>POST[%s]: %s</li>", p->name().c_str(), p->value().c_str());
  } 
  else 
  {
    response->printf("<li>GET[%s]: %s</li>", p->name().c_str(), p->value().c_str());
  }
}

response->print("</ul>");

response->print("</body></html>");
//send the response last
request->send(response);
```

### ArduinoJson Basic Response

This way of sending Json is great for when the result is **below 4KB**

```cpp
#include "AsyncJson.h"
#include "ArduinoJson.h"

AsyncResponseStream *response = request->beginResponseStream("application/json");
DynamicJsonBuffer jsonBuffer;
JsonObject &root = jsonBuffer.createObject();
root["heap"] = ESP.getFreeHeap();
root["ssid"] = WiFi.SSID();
root.printTo(*response);

request->send(response);
```

### ArduinoJson Advanced Response

This response can handle really **large Json objects (tested to 40KB)**

There isn't any noticeable speed decrease for small results with the method above

Since ArduinoJson does not allow reading parts of the string, the whole Json has to be passed every time a 
chunks needs to be sent, which shows speed decrease proportional to the resulting json packets

```cpp
#include "AsyncJson.h"
#include "ArduinoJson.h"

AsyncJsonResponse * response = new AsyncJsonResponse();
response->addHeader("Server","AsyncWebServer");
JsonObject& root = response->getRoot();
root["IP"] = Ethernet.localIP();
response->setLength();

request->send(response);
```
---

## Param Rewrite With Matching

It is possible to rewrite the request url with parameter matchg. Here is an example with one parameter:
Rewrite for example "/radio/{frequence}" -> "/radio?f={frequence}"

```cpp
class OneParamRewrite : public AsyncWebRewrite
{
  protected:
    String _urlPrefix;
    int _paramIndex;
    String _paramsBackup;

  public:
  OneParamRewrite(const char* from, const char* to)
    : AsyncWebRewrite(from, to) 
    {

      _paramIndex = _from.indexOf('{');

      if( _paramIndex >=0 && _from.endsWith("}")) 
      {
        _urlPrefix = _from.substring(0, _paramIndex);
        int index = _params.indexOf('{');
        
        if(index >= 0) 
        {
          _params = _params.substring(0, index);
        }
      } 
      else 
      {
        _urlPrefix = _from;
      }
      
      _paramsBackup = _params;
  }

  bool match(AsyncWebServerRequest *request) override 
  {
    if(request->url().startsWith(_urlPrefix)) 
    {
      if(_paramIndex >= 0) 
      {
        _params = _paramsBackup + request->url().substring(_paramIndex);
      } 
      else 
      {
        _params = _paramsBackup;
      }
      
      return true;

    } 
    else 
    {
      return false;
    }
  }
};
```

Usage:

```cpp
server.addRewrite( new OneParamRewrite("/radio/{frequence}", "/radio?f={frequence}") );
```
---

## Using filters

Filters can be set to `Rewrite` or `Handler` in order to control when to apply the rewrite and consider the handler.
A filter is a callback function that evaluates the request and return a boolean `true` to include the item
or `false` to exclude it.

---

## Bad Responses

Some responses are implemented, but you should not use them, because they do not conform to HTTP.
The following example will lead to unclean close of the connection and more time wasted
than providing the length of the content

### Respond with content using a callback without content length to HTTP/1.0 clients

```cpp
//This is used as fallback for chunked responses to HTTP/1.0 Clients
request->send("text/plain", 0, [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t 
{
  //Write up to "maxLen" bytes into "buffer" and return the amount written.
  //You will be asked for more data until 0 is returned
  //Keep in mind that you can not delay or yield waiting for more data!
  return mySource.read(buffer, maxLen);
});
```
---

## Async WebSocket Plugin

The server includes a web socket plugin which lets you define different WebSocket locations to connect to
without starting another listening service or using different port

### Async WebSocket Event

```cpp
void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
{
  if(type == WS_EVT_CONNECT)
  {
    //client connected
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    client->printf("Hello Client %u :)", client->id());
    client->ping();
  } 
  else if(type == WS_EVT_DISCONNECT)
  {
    //client disconnected
    Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
  } 
  else if(type == WS_EVT_ERROR)
  {
    //error was received from the other end
    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } 
  else if(type == WS_EVT_PONG)
  {
    //pong message was received (in response to a ping request maybe)
    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } 
  else if(type == WS_EVT_DATA)
  {
    //data packet
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    
    if(info->final && info->index == 0 && info->len == len)
    {
      //the whole message is in a single frame and we got all of it's data
      Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);
      
      if(info->opcode == WS_TEXT)
      {
        data[len] = 0;
        Serial.printf("%s\n", (char*)data);
      } 
      else 
      {
        for(size_t i=0; i < info->len; i++)
        {
          Serial.printf("%02x ", data[i]);
        }
        
        Serial.printf("\n");
      }
      
      if(info->opcode == WS_TEXT)
        client->text("I got your text message");
      else
        client->binary("I got your binary message");
    } 
    else 
    {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if(info->index == 0)
      {
        if(info->num == 0)
          Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          
        Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
      
      if(info->message_opcode == WS_TEXT)
      {
        data[len] = 0;
        Serial.printf("%s\n", (char*)data);
      } 
      else 
      {
        for(size_t i=0; i < len; i++){
          Serial.printf("%02x ", data[i]);
        }
        Serial.printf("\n");
      }

      if((info->index + len) == info->len)
      {
        Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        
        if(info->final)
        {
          Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          
          if(info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}
```

### Methods for sending data to a socket client

```cpp
//Server methods
AsyncWebSocket ws("/ws");
//printf to a client
ws.printf((uint32_t)client_id, arguments...);
//printf to all clients
ws.printfAll(arguments...);
//send text to a client
ws.text((uint32_t)client_id, (char*)text);
ws.text((uint32_t)client_id, (uint8_t*)text, (size_t)len);
//send text to all clients
ws.textAll((char*)text);
ws.textAll((uint8_t*)text, (size_t)len);
//send binary to a client
ws.binary((uint32_t)client_id, (char*)binary);
ws.binary((uint32_t)client_id, (uint8_t*)binary, (size_t)len);
ws.binary((uint32_t)client_id, flash_binary, 4);
//send binary to all clients
ws.binaryAll((char*)binary);
ws.binaryAll((uint8_t*)binary, (size_t)len);
//HTTP Authenticate before switch to Websocket protocol
ws.setAuthentication("user", "pass");

//client methods
AsyncWebSocketClient * client;
//printf
client->printf(arguments...);
//send text
client->text((char*)text);
client->text((uint8_t*)text, (size_t)len);
//send binary
client->binary((char*)binary);
client->binary((uint8_t*)binary, (size_t)len);
```

### Direct access to web socket message buffer

When sending a `websocket` message using the above methods a buffer is created.  Under certain circumstances you might want to manipulate or populate this buffer directly from your application, for example to prevent unnecessary duplications of the data.  This example below shows how to create a buffer and print data to it from an `ArduinoJson` object then send it.

```cpp
void sendDataWs(AsyncWebSocketClient * client)
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["a"] = "abc";
  root["b"] = "abcd";
  root["c"] = "abcde";
  root["d"] = "abcdef";
  root["e"] = "abcdefg";
  size_t len = root.measureLength();
  AsyncWebSocketMessageBuffer * buffer = ws.makeBuffer(len); //  creates a buffer (len + 1) for you.
  
  if (buffer) 
  {
    root.printTo((char *)buffer->get(), len + 1);
    
    if (client) 
    {
        client->text(buffer);
    } 
    else 
    {
        ws.textAll(buffer);
    }
  }
}
```

### Limiting the number of web socket clients

Browsers sometimes do not correctly close the websocket connection, even when the `close()` function is called in javascript.  This will eventually exhaust the web server's resources and will cause the server to crash.  Periodically calling the `cleanClients()` function from the main `loop()` function limits the number of clients by closing the oldest client when the maximum number of clients has been exceeded.  This can called be every cycle, however, if you wish to use less power, then calling as infrequently as once per second is sufficient.

```cpp
void loop()
{
  ws.cleanupClients();
}
```

---

## Async Event Source Plugin

The server includes `EventSource` (Server-Sent Events) plugin which can be used to send short text events to the browser.
Difference between `EventSource` and `WebSockets` is that `EventSource` is single direction, text-only protocol.

### Setup Event Source on the server

```cpp
AsyncWebServer server(80);
AsyncEventSource events("/events");

void setup()
{
  // setup ......
  events.onConnect([](AsyncEventSourceClient *client)
  {
    if(client->lastId())
    {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    
    //send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!",NULL,millis(),1000);
  });
  
  //HTTP Basic authentication
  events.setAuthentication("user", "pass");
  server.addHandler(&events);
  // setup ......
}

void loop()
{
  if(eventTriggered){ // your logic here
    //send event "myevent"
    events.send("my event content","myevent",millis());
  }
}
```

### Setup Event Source in the browser

```javascript
if (!!window.EventSource) 
{
  var source = new EventSource('/events');

  source.addEventListener('open', function(e) 
  {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) 
  {
    if (e.target.readyState != EventSource.OPEN) 
    {
      console.log("Events Disconnected");
    }
  }, false);

  source.addEventListener('message', function(e) 
  {
    console.log("message", e.data);
  }, false);

  source.addEventListener('myevent', function(e) 
  {
    console.log("myevent", e.data);
  }, false);
}
```
---

## Remove handlers and rewrites

Server goes through handlers in same order as they were added. You can't simple add handler with same path to override them.
To remove handler:

```cpp
// save callback for particular URL path
auto handler = server.on("/some/path", [](AsyncWebServerRequest *request){
  //do something useful
});

// when you don't need handler anymore remove it
server.removeHandler(&handler);

// same with rewrites
server.removeRewrite(&someRewrite);

server.onNotFound([](AsyncWebServerRequest *request){
  request->send(404);
});

// remove server.onNotFound handler
server.onNotFound(NULL);

// remove all rewrites, handlers and onNotFound/onFileUpload/onRequestBody callbacks
server.reset();
```
---

## Setting up the server

```cpp
#if !( defined(ARDUINO_RASPBERRY_PI_PICO_W) )
  #error For RASPBERRY_PI_PICO_W only
#endif

#include <AsyncWebServer_RP2040W.h>

char ssid[] = "your_ssid";        // your network SSID (name)
char pass[] = "12345678";         // your network password (use for WPA, or use as key for WEP), length must be 8+

int status = WL_IDLE_STATUS;

AsyncWebServer    server(80);

#define LED_OFF             LOW
#define LED_ON              HIGH

#define BUFFER_SIZE         64
char temp[BUFFER_SIZE];

void handleRoot(AsyncWebServerRequest *request)
{
  digitalWrite(LED_BUILTIN, LED_ON);

  snprintf(temp, BUFFER_SIZE - 1, "Hello from Async_HelloServer on %s\n", BOARD_NAME);

  request->send(200, "text/plain", temp);
  
  digitalWrite(LED_BUILTIN, LED_OFF);
}

void handleNotFound(AsyncWebServerRequest *request)
{
  digitalWrite(LED_BUILTIN, LED_ON);
  
  String message = "File Not Found\n\n";

  message += "URI: ";
  //message += server.uri();
  message += request->url();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += request->args();
  message += "\n";

  for (uint8_t i = 0; i < request->args(); i++)
  {
    message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
  }
 
  request->send(404, "text/plain", message);
  digitalWrite(LED_BUILTIN, LED_OFF);
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("Local IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_OFF);

  Serial.begin(115200);
  while (!Serial);

  delay(200);

  Serial.print("\nStart Async_HelloServer on "); Serial.print(BOARD_NAME);
  Serial.print(" with "); Serial.println(SHIELD_TYPE);
  Serial.println(ASYNCTCP_RP2040W_VERSION);
  Serial.println(ASYNC_WEBSERVER_RP2040W_VERSION);

  ///////////////////////////////////
  
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  Serial.print(F("Connecting to SSID: "));
  Serial.println(ssid);

  status = WiFi.begin(ssid, pass);

  delay(1000);
   
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED)
  {
    delay(500);
        
    // Connect to WPA/WPA2 network
    status = WiFi.status();
  }

  printWifiStatus();
  
  ///////////////////////////////////

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    handleRoot(request);
  });

  server.on("/inline", [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", "This works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();

  Serial.print(F("HTTP EthernetWebServer is @ IP : "));
  Serial.println(WiFi.localIP());
}

void heartBeatPrint()
{
  static int num = 1;

  Serial.print(F("."));

  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(F(" "));
  }
}

void check_status()
{
  static unsigned long checkstatus_timeout = 0;

#define STATUS_CHECK_INTERVAL     10000L

  // Send status report every STATUS_REPORT_INTERVAL (60) seconds: we don't need to send updates frequently if there is no status change.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = millis() + STATUS_CHECK_INTERVAL;
  }
}

void loop()
{
  check_status();
}
```
---

### Setup global and class functions as request handlers

```cpp
#if !( defined(ARDUINO_RASPBERRY_PI_PICO_W) )
  #error For RASPBERRY_PI_PICO_W only
#endif

#include <Arduino.h>

#include <AsyncWebServer_RP2040W.h>

char ssid[] = "your_ssid";        // your network SSID (name)
char pass[] = "12345678";         // your network password (use for WPA, or use as key for WEP), length must be 8+

int status = WL_IDLE_STATUS;

#include <functional>

...

void handleRequest(AsyncWebServerRequest *request){}

class WebClass 
{
public :
  AsyncWebServer classWebServer = AsyncWebServer(81);

  WebClass(){};

  void classRequest (AsyncWebServerRequest *request){}

  void begin()
  {
    // attach global request handler
    classWebServer.on("/example", HTTP_ANY, handleRequest);

    // attach class request handler
    classWebServer.on("/example", HTTP_ANY, std::bind(&WebClass::classRequest, this, std::placeholders::_1));
  }
};

AsyncWebServer globalWebServer(80);
WebClass webClassInstance;

void setup() 
{
  // attach global request handler
  globalWebServer.on("/example", HTTP_ANY, handleRequest);

  // attach class request handler
  globalWebServer.on("/example", HTTP_ANY, std::bind(&WebClass::classRequest, webClassInstance, std::placeholders::_1));
}

void loop() 
{
}
```

### Methods for controlling websocket connections

```cpp
// Disable client connections if it was activated
if ( ws.enabled() )
  ws.enable(false);

// enable client connections if it was disabled
if ( !ws.enabled() )
  ws.enable(true);
```


### Adding Default Headers

In some cases, such as when working with `CORS`, or with some sort of custom authentication system, 
you might need to define a header that should get added to all responses (including static, websocket and EventSource).
The `DefaultHeaders` singleton allows you to do this.

Example:

```cpp
DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
webServer.begin();
```

*NOTE*: You will still need to respond to the OPTIONS method for CORS pre-flight in most cases. (unless you are only using GET)

This is one option:

```cpp
webServer.onNotFound([](AsyncWebServerRequest *request) 
{
  if (request->method() == HTTP_OPTIONS) 
  {
    request->send(200);
  } 
  else 
  {
    request->send(404);
  }
});
```

### Path variable

With path variable you can create a custom regex rule for a specific parameter in a route. 
For example we want a `sensorId` parameter in a route rule to match only a integer.

```cpp
server.on("^\\/sensor\\/([0-9]+)$", HTTP_GET, [] (AsyncWebServerRequest *request) 
{
    String sensorId = request->pathArg(0);
});
```
*NOTE*: All regex patterns starts with `^` and ends with `$`

To enable the `Path variable` support, you have to define the buildflag `-DASYNCWEBSERVER_REGEX`.


For Arduino IDE create/update `platform.local.txt`:

`Windows`: C:\Users\(username)\AppData\Local\Arduino15\packages\\`{xxxx}`\hardware\\`xxxx`\\`{version}`\platform.local.txt

`Linux`: ~/.arduino15/packages/`{xxxx}`/hardware/`{xxxx}`/`{version}`/platform.local.txt

Add/Update the following line:


```ini
compiler.cpp.extra_flags=-DDASYNCWEBSERVER_REGEX
```

For platformio modify `platformio.ini`:

```ini
[env:myboard]
build_flags = 
  -DASYNCWEBSERVER_REGEX
```

*NOTE*: By enabling `ASYNCWEBSERVER_REGEX`, `<regex>` will be included. This will add an 100k to your binary.


---
---

### Examples

 1. [Async_AdvancedWebServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer)
 2. [Async_HelloServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_HelloServer) 
 3. [Async_HelloServer2](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_HelloServer2)
 4. [Async_HttpBasicAuth](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_HttpBasicAuth)
 5. [Async_PostServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_PostServer)
 6. [**MQTTClient_Auth**](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/MQTTClient_Auth)
 7. [**MQTTClient_Basic**](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/MQTTClient_Basic)
 8. [**MQTT_ThingStream**](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/MQTT_ThingStream)
 9. [WebClient](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/WebClient)
10. [WebClientRepeating](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/WebClientRepeating)
11. [Async_AdvancedWebServer_Country](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_Country)
12. [Async_AdvancedWebServer_favicon](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_favicon)
13. [Async_AdvancedWebServer_MemoryIssues_SendArduinoString](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_MemoryIssues_SendArduinoString)
14. [Async_AdvancedWebServer_MemoryIssues_Send_CString](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_MemoryIssues_Send_CString)
15. [Async_WebSocketsServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_WebSocketsServer)
16. [Async_WebSocketsServer_Xtreme](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_WebSocketsServer_Xtreme)
17. [AsyncFSWebServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncFSWebServer)
18. [AsyncFSWebServer_Complex](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncFSWebServer_Complex)
19. [Async_AdvancedWebServer_SendChunked](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_SendChunked)
20. [AsyncWebServer_SendChunked](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncWebServer_SendChunked)
21. [AsyncWebServer_MQTT_RP2040W](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncWebServer_MQTT_RP2040W) **New**
22. [Async_AdvancedWebServer_SendChunked_MQTT](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_SendChunked_MQTT) **New**



---
---

### Example [Async_AdvancedWebServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer)

https://github.com/khoih-prog/AsyncWebServer_RP2040W/blob/2d2f55326766dc79340d906d6a4a386e4291e74e/examples/Async_AdvancedWebServer/Async_AdvancedWebServer.ino#L41-L299

You can access the Async Advanced WebServer @ the server IP

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_RP2040W/raw/main/pics/Async_AdvancedWebServer.png">
</p>

---
---

### Debug Terminal Output Samples

#### 1. Async_AdvancedWebServer on RASPBERRY_PI_PICO_W using CYW43439 WiFi

Following is the debug terminal when running example [Async_AdvancedWebServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer) on `RASPBERRY_PI_PICO_W using CYW43439 WiFi` to demonstrate the operation of AsyncWebServer_RP2040W, based on this [AsyncTCP_RP2040W Library](https://github.com/khoih-prog/AsyncTCP_RP2040W)


```cpp
Start Async_AdvancedWebServer on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
AsyncTCP_RP2040W v1.1.0
AsyncWebServer_RP2040W v1.5.0
Connecting to SSID: HueNet1
SSID: HueNet1
Local IP Address: 192.168.2.180
Country code: CA         <================ Country code CA for CYW43_COUNTRY_CANADA
HTTP EthernetWebServer is @ IP : 192.168.2.180
.......... .......... .......... .......... .......... .......... .......... ..........
.......... .......... .......... .......... .......... ...
```

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_RP2040W/raw/main/pics/Async_AdvancedWebServer.png">
</p>

---

#### 2. WebClient on RASPBERRY_PI_PICO_W using CYW43439 WiFi

Following is debug terminal output when running example [WebClient](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/WebClient) on `RASPBERRY_PI_PICO_W using CYW43439 WiFi`


```cpp
Start WebClient on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
AsyncTCP_RP2040W v1.1.0
AsyncWebServer_RP2040W v1.5.0
Connecting to SSID: HueNet1
SSID: HueNet1
Local IP Address: 192.168.2.180
Country code: CA         <================ Country code CA for CYW43_COUNTRY_CANADA

Starting connection to server...
Connected to server
HTTP/1.1 200 OK
Date: Tue, 16 Aug 2022 01:06:29 GMT
Content-Type: text/plain
Content-Length: 2263
Connection: close
x-amz-id-2: eCCq3bjp3ZIWsEqS9Timqjnr4liaTs2BY2giqfjPt5fRD9UPsPHZgIkhWMmbgfXHm7Rp6g1V1EE=
x-amz-request-id: V2X8BFCZS6E01EA4
Last-Modified: Wed, 23 Feb 2022 14:56:42 GMT
ETag: "667cf48afcc12c38c8c1637947a04224"
CF-Cache-Status: DYNAMIC
Report-To: {"endpoints":[{"url":"https:\/\/a.nel.cloudflare.com\/report\/v3?s=VUTYgtuhcRcBq%2F%2Bj6oRv60yx72aRGR6Z0yNupDNmxvSYpiwM6bHwRA8Xteiu3GM0pjxLcOGB5apbpNOkljur%2FuuNTU%2FnfcLZZc8zF7i8nrDyeplpCRDuJ9oz0HCZKSI%3D"}],"group":"cf-nel","max_age":604800}
NEL: {"success_fraction":0,"report_to":"cf-nel","max_age":604800}
Server: cloudflare
CF-RAY: 73b64482ad06a1e0-YYZ
alt-svc: h3=":443"; ma=86400, h3-29=":443"; ma=86400


           `:;;;,`                      .:;;:.           
        .;;;;;;;;;;;`                :;;;;;;;;;;:     TM 
      `;;;;;;;;;;;;;;;`            :;;;;;;;;;;;;;;;      
     :;;;;;;;;;;;;;;;;;;         `;;;;;;;;;;;;;;;;;;     
    ;;;;;;;;;;;;;;;;;;;;;       .;;;;;;;;;;;;;;;;;;;;    
   ;;;;;;;;:`   `;;;;;;;;;     ,;;;;;;;;.`   .;;;;;;;;   
  .;;;;;;,         :;;;;;;;   .;;;;;;;          ;;;;;;;  
  ;;;;;;             ;;;;;;;  ;;;;;;,            ;;;;;;. 
 ,;;;;;               ;;;;;;.;;;;;;`              ;;;;;; 
 ;;;;;.                ;;;;;;;;;;;`      ```       ;;;;;`
 ;;;;;                  ;;;;;;;;;,       ;;;       .;;;;;
`;;;;:                  `;;;;;;;;        ;;;        ;;;;;
,;;;;`    `,,,,,,,,      ;;;;;;;      .,,;;;,,,     ;;;;;
:;;;;`    .;;;;;;;;       ;;;;;,      :;;;;;;;;     ;;;;;
:;;;;`    .;;;;;;;;      `;;;;;;      :;;;;;;;;     ;;;;;
.;;;;.                   ;;;;;;;.        ;;;        ;;;;;
 ;;;;;                  ;;;;;;;;;        ;;;        ;;;;;
 ;;;;;                 .;;;;;;;;;;       ;;;       ;;;;;,
 ;;;;;;               `;;;;;;;;;;;;                ;;;;; 
 `;;;;;,             .;;;;;; ;;;;;;;              ;;;;;; 
  ;;;;;;:           :;;;;;;.  ;;;;;;;            ;;;;;;  
   ;;;;;;;`       .;;;;;;;,    ;;;;;;;;        ;;;;;;;:  
    ;;;;;;;;;:,:;;;;;;;;;:      ;;;;;;;;;;:,;;;;;;;;;;   
    `;;;;;;;;;;;;;;;;;;;.        ;;;;;;;;;;;;;;;;;;;;    
      ;;;;;;;;;;;;;;;;;           :;;;;;;;;;;;;;;;;:     
       ,;;;;;;;;;;;;;,              ;;;;;;;;;;;;;;       
         .;;;;;;;;;`                  ,;;;;;;;;:         
                                                         
                                                         
                                                         
                                                         
    ;;;   ;;;;;`  ;;;;:  .;;  ;; ,;;;;;, ;;. `;,  ;;;;   
    ;;;   ;;:;;;  ;;;;;; .;;  ;; ,;;;;;: ;;; `;, ;;;:;;  
   ,;:;   ;;  ;;  ;;  ;; .;;  ;;   ,;,   ;;;,`;, ;;  ;;  
   ;; ;:  ;;  ;;  ;;  ;; .;;  ;;   ,;,   ;;;;`;, ;;  ;;. 
   ;: ;;  ;;;;;:  ;;  ;; .;;  ;;   ,;,   ;;`;;;, ;;  ;;` 
  ,;;;;;  ;;`;;   ;;  ;; .;;  ;;   ,;,   ;; ;;;, ;;  ;;  
  ;;  ,;, ;; .;;  ;;;;;:  ;;;;;: ,;;;;;: ;;  ;;, ;;;;;;  
  ;;   ;; ;;  ;;` ;;;;.   `;;;:  ,;;;;;, ;;  ;;,  ;;;;   

Disconnecting from server...
```

---

#### 3. MQTTClient_Auth on RASPBERRY_PI_PICO_W using CYW43439 WiFi

Following is debug terminal output when running example [MQTTClient_Auth](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/MQTTClient_Auth) on `RASPBERRY_PI_PICO_W using CYW43439 WiFi`


```cpp
Start MQTTClient_Auth on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
AsyncTCP_RP2040W v1.1.0
AsyncWebServer_RP2040W v1.5.0
Connecting to SSID: HueNet1
SSID: HueNet1
Local IP Address: 192.168.2.180
Country code: CA         <================ Country code CA for CYW43_COUNTRY_CANADA
Attempting MQTT connection to broker.emqx.io...connected
Message Send : MQTT_Pub => Hello from MQTTClient_Auth on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
Message arrived [MQTT_Pub] Hello from MQTTClient_Auth on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
Message Send : MQTT_Pub => Hello from MQTTClient_Auth on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
Message arrived [MQTT_Pub] Hello from MQTTClient_Auth on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
```


---

#### 4. MQTTClient_Basic on RASPBERRY_PI_PICO_W using CYW43439 WiFi

Following is debug terminal output when running example [MQTTClient_Basic](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/MQTTClient_Basic) on `RASPBERRY_PI_PICO_W using CYW43439 WiFi`


```cpp
Start MQTTClient_Basic on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
AsyncTCP_RP2040W v1.1.0
AsyncWebServer_RP2040W v1.5.0
Connecting to SSID: HueNet1
SSID: HueNet1
Local IP Address: 192.168.2.180
Country code: CA         <================ Country code CA for CYW43_COUNTRY_CANADA
Attempting MQTT connection to broker.emqx.io...connected
Message Send : MQTT_Pub => Hello from MQTTClient_Basic on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
Message arrived [MQTT_Pub] Hello from MQTTClient_Basic on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
Message Send : MQTT_Pub => Hello from MQTTClient_Basic on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
Message arrived [MQTT_Pub] Hello from MQTTClient_Basic on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
Message Send : MQTT_Pub => Hello from MQTTClient_Basic on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
```

---

#### 5. MQTT_ThingStream on RASPBERRY_PI_PICO_W using CYW43439 WiFi

Following is debug terminal output when running example [MQTT_ThingStream](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/MQTT_ThingStream) on `RASPBERRY_PI_PICO_W using CYW43439 WiFi`


```cpp
Start MQTT_ThingStream on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
AsyncTCP_RP2040W v1.1.0
AsyncWebServer_RP2040W v1.5.0
Connecting to SSID: HueNet1
SSID: HueNet1
Local IP Address: 192.168.2.180
Country code: CA         <================ Country code CA for CYW43_COUNTRY_CANADA
***************************************
RP2040W_Pub
***************************************
Attempting MQTT connection to broker.emqx.io
...connected
Published connection message successfully!
Subscribed to: RP2040W_Sub
MQTT Message Send : RP2040W_Pub => Hello from MQTT_ThingStream on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
MQTT Message receive [RP2040W_Pub] Hello from MQTT_ThingStream on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
MQTT Message Send : RP2040W_Pub => Hello from MQTT_ThingStream on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
MQTT Message receive [RP2040W_Pub] Hello from MQTT_ThingStream on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
```

---

#### 6. Async_AdvancedWebServer_Country on RASPBERRY_PI_PICO_W using CYW43439 WiFi

Following is the debug terminal when running example [Async_AdvancedWebServer_Country](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_Country) on `RASPBERRY_PI_PICO_W using CYW43439 WiFi` to demonstrate the operation of AsyncWebServer_RP2040W, based on this [AsyncTCP_RP2040W Library](https://github.com/khoih-prog/AsyncTCP_RP2040W) and to display programmed `country-code`


```cpp
Start Async_AdvancedWebServer_Country on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
AsyncTCP_RP2040W v1.1.0
AsyncWebServer_RP2040W v1.5.0
Connecting to SSID: HueNet1
SSID: HueNet1
Local IP Address: 192.168.2.180
Country code: CA         <================ Country code CA for CYW43_COUNTRY_CANADA
HTTP EthernetWebServer is @ IP : 192.168.2.180
....
```

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_RP2040W/raw/main/pics/Async_AdvancedWebServer_Country.png">
</p>

##### Chrome, Vivaldi and Firefox running simultaneously

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_RP2040W/raw/main/pics/Async_AdvancedWebServer_Country_Multi.png">
</p>

##### Only Firefox running

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_RP2040W/raw/main/pics/Async_AdvancedWebServer_Country_Firefox.png">
</p>


---

#### 7. Async_AdvancedWebServer_favicon on RASPBERRY_PI_PICO_W using CYW43439 WiFi

Following is the debug terminal when running example [Async_AdvancedWebServer_favicon](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_favicon) on `RASPBERRY_PI_PICO_W using CYW43439 WiFi` to demonstrate the operation of AsyncWebServer_RP2040W, based on this [AsyncTCP_RP2040W Library](https://github.com/khoih-prog/AsyncTCP_RP2040W) and to display `favicon.ico`, which many browsers are interested.


```cpp
14:22:06.632 -> Start Async_AdvancedWebServer_favicon on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
14:22:06.632 -> AsyncTCP_RP2040W v1.1.0
14:22:06.632 -> AsyncWebServer_RP2040W v1.5.0
14:22:06.632 -> Connecting to SSID: HueNet1
14:22:13.328 -> SSID: HueNet1
14:22:13.328 -> Local IP Address: 192.168.2.180
14:22:13.328 -> Country code: XX
14:22:13.328 -> HTTP EthernetWebServer is @ IP : 192.168.2.180
14:22:13.328 -> .......... .......... .......... .......... .......... .......... .......... ..........
14:35:53.414 -> .......... .......... .......... .......... ...
```

You can see the `favicon.ico` at the upper left corner

##### On Chrome

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_RP2040W/raw/main/pics/Async_AdvancedWebServer_favicon.png">
</p>

##### On Firefox

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_RP2040W/raw/main/pics/Async_AdvancedWebServer_Firefox_favicon.png">
</p>

---


#### 8. Async_AdvancedWebServer_MemoryIssues_Send_CString on RASPBERRY_PI_PICO_W

Following is the debug terminal and screen shot when running example [Async_AdvancedWebServer_MemoryIssues_Send_CString](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_MemoryIssues_Send_CString) on `RASPBERRY_PI_PICO_W` to demonstrate the new and powerful `HEAP-saving` feature


##### Using CString  ===> small heap (44,000 bytes)

```cpp
Start Async_AdvancedWebServer_MemoryIssues_Send_CString on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
AsyncTCP_RP2040W v1.1.0
AsyncWebServer_RP2040W v1.5.0
Connecting to SSID: HueNet1
SSID: HueNet1
Local IP Address: 192.168.2.74
Country code: XX
HTTP EthernetWebServer is @ IP : 192.168.2.74

HEAP DATA - Pre Create Arduino String  Cur heap: 193000  Free heap: 150928  Max heap: 42072
.75264
HEAP DATA - Pre Send  Cur heap: 193000  Free heap: 149176  Max heap: 43824

HEAP DATA - Post Send  Cur heap: 193000  Free heap: 149016  Max heap: 43984
.
HEAP DATA - Post Send  Cur heap: 193000  Free heap: 149000  Max heap: 44000
.......... .......... .......... ........
Out String Length=31247
.. .......... .......... .......... ..........
```

While using Arduino String, the HEAP usage is very large


#### Async_AdvancedWebServer_MemoryIssues_SendArduinoString  ===> very large heap (75,264 bytes)


```cpp
Start Async_AdvancedWebServer_MemoryIssues_SendArduinoString on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
AsyncTCP_RP2040W v1.1.0
AsyncWebServer_RP2040W v1.5.0
Connecting to SSID: HueNet1
SSID: HueNet1
Local IP Address: 192.168.2.74
Country code: XX
HTTP EthernetWebServer is @ IP : 192.168.2.74

HEAP DATA - Pre Create Arduino String  Cur heap: 193256  Free heap: 191192  Max heap: 2064
.
HEAP DATA - Pre Send  Cur heap: 193256  Free heap: 149432  Max heap: 43824

HEAP DATA - Post Send  Cur heap: 193256  Free heap: 118024  Max heap: 75232

HEAP DATA - Post Send  Cur heap: 193256  Free heap: 117992  Max heap: 75264
....... .......... .......... ..........
.......... .......... .......... ........
Out String Length=31247
.. .......... .
```


You can access the Async Advanced WebServers at the displayed server IP, e.g. `192.168.2.74`

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_RP2040W/raw/main/pics/Async_AdvancedWebServer_MemoryIssues_Send_CString.png">
</p>


---

#### 9. Async_WebSocketsServer on RASPBERRY_PI_PICO_W using CYW43439 WiFi

Following is debug terminal output when running example [Async_WebSocketsServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_WebSocketsServer) on `RASPBERRY_PI_PICO_W using CYW43439 WiFi`. The WSClient is using the provided [WSClient.py](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_WebSocketsServer/WSClient_Python/WSClient.py)


```cpp
Starting Async_WebSocketsServer on RASPBERRY_PI_PICO_W
AsyncTCP_RP2040W v1.1.0
AsyncWebServer_RP2040W v1.5.0
Connecting to SSID: HueNet1
SSID: HueNet1
Local IP Address: 192.168.2.77
ws[Server: /ws][ClientID: 1] WSClient connected
ws[Server: /ws][ClientID: 2] WSClient connected
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 1] WSClient disconnected
ws[Server: /ws][ClientID: 3] WSClient connected
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
ws[Server: /ws][ClientID: 2] text-message[len: 13]: Hello, Server
```


You can access the Async_WebSockets Servers at the displayed server IP, e.g. `192.168.2.77`

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_RP2040W/raw/main/pics/Async_WebSocketsServer.png">
</p>

---


#### 10. Async_WebSocketsServer_Xtreme on RASPBERRY_PI_PICO_W using CYW43439 WiFi

Following is debug terminal output when running example [Async_WebSocketsServer_Xtreme](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_WebSocketsServer_Xtreme) on `RASPBERRY_PI_PICO_W using CYW43439 WiFi`.


```cpp
Starting Async_WebSocketsServer_Xtreme on RASPBERRY_PI_PICO_W
AsyncTCP_RP2040W v1.1.0
AsyncWebServer_RP2040W v1.5.0
Connecting to SSID: HueNet1
SSID: HueNet1
Local IP Address: 192.168.2.77
```


You can access the Async_WebSockets Servers at the displayed server IP, e.g. `192.168.2.77`

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_RP2040W/raw/main/pics/Async_WebSocketsServer_Xtreme.png">
</p>


---


#### 11. AsyncFSWebServer_Complex on RASPBERRY_PI_PICO_W using CYW43439 WiFi

Following is debug terminal output when running example [AsyncFSWebServer_Complex](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncFSWebServer_Complex) on `RASPBERRY_PI_PICO_W using CYW43439 WiFi`.


```cpp
Start AsyncFSWebServer_Complex on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
AsyncTCP_RP2040W v1.1.0
AsyncWebServer_RP2040W v1.5.0
Connecting to SSID: HueNet1
SSID: HueNet1
Local IP Address: 192.168.2.77
Opening / directory
FS File: CanadaFlag_1.png, size: 40.25KB
FS File: CanadaFlag_2.png, size: 8.12KB
FS File: CanadaFlag_3.jpg, size: 10.89KB
FS File: css, size: 0B
FS File: edit.htm.gz, size: 4.02KB
FS File: favicon.ico, size: 1.12KB
FS File: graphs.js.gz, size: 1.92KB
FS File: index.htm, size: 3.63KB
FS File: js, size: 0B

AsyncWebServer started @192.168.2.77
Open http://192.168.2.77/edit to see the file browser
AsyncFSEditor::handleRequest: Sending AsyncWebServerResponse
```


You can access the Async_WebSockets Servers at the displayed server IP, e.g. `192.168.2.77`

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_RP2040W/raw/main/pics/AsyncFSWebServer_Complex.png">
</p>


---


#### 12. Async_AdvancedWebServer_SendChunked on RASPBERRY_PI_PICO_W using CYW43439 WiFi

Following is debug terminal output when running example [Async_AdvancedWebServer_SendChunked](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_SendChunked) on `RASPBERRY_PI_PICO_W using CYW43439 WiFi`, to demo how to use `beginChunkedResponse()` to send large `html` in chunks


```cpp
Start Async_AdvancedWebServer_SendChunked on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
AsyncTCP_RP2040W v1.1.0
AsyncWebServer_RP2040W v1.5.0
Connecting to SSID: HueNet1
SSID: HueNet1
Local IP Address: 192.168.2.77
Country code: XX
AsyncWebServer is @ IP : 192.168.2.77
.[AWS] Total length to send in chunks = 31259
[AWS] Bytes sent in chunk = 11556
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 8031
[AWS] Bytes sent in chunk = 0
[AWS] Total length to send in chunks = 31279
[AWS] Bytes sent in chunk = 11556
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 8051
[AWS] Bytes sent in chunk = 0
```


You can access the AsyncWebServer_RP2040W at the displayed server IP, e.g. `192.168.2.77`

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_RP2040W/raw/main/pics/AsyncWebServer_SendChunked.png">
</p>


---


#### 13. AsyncWebServer_SendChunked on RASPBERRY_PI_PICO_W using CYW43439 WiFi

Following is debug terminal output when running example [AsyncWebServer_SendChunked](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncWebServer_SendChunked) on`RASPBERRY_PI_PICO_W using CYW43439 WiFi`, to demo how to use `beginChunkedResponse()` to send large `html` in chunks


```cpp
Start AsyncWebServer_SendChunked on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
AsyncTCP_RP2040W v1.1.0
AsyncWebServer_RP2040W v1.5.0
Connecting to SSID: HueNet1
SSID: HueNet1
Local IP Address: 192.168.2.77
Country code: XX
AsyncWebServer is @ IP : 192.168.2.77
.[AWS] Total length to send in chunks = 47387
[AWS] Bytes sent in chunk = 11560
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 811
[AWS] Bytes sent in chunk = 0
.[AWS] Total length to send in chunks = 47387
[AWS] Bytes sent in chunk = 11560
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 811
[AWS] Bytes sent in chunk = 0
[AWS] Total length to send in chunks = 47387
[AWS] Bytes sent in chunk = 11560
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 811
[AWS] Bytes sent in chunk = 0
.[AWS] Total length to send in chunks = 47387
[AWS] Bytes sent in chunk = 11560
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 811
[AWS] Bytes sent in chunk = 0
[AWS] Total length to send in chunks = 47387
[AWS] Bytes sent in chunk = 11560
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 11672
[AWS] Bytes sent in chunk = 811
[AWS] Bytes sent in chunk = 0
.[AWS] Total length to send in chunks = 47387
[AWS] Bytes sent in chunk = 11560
...... ...
```

---

#### 14. Async_AdvancedWebServer_SendChunked_MQTT on RASPBERRY_PI_PICO_W using CYW43439 WiFi

Following is debug terminal output when running example [Async_AdvancedWebServer_SendChunked_MQTT](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_SendChunked_MQTT) on `RASPBERRY_PI_PICO_W using CYW43439 WiFi`, to demo how to use `AsyncWebServer_RP2040W` and `AsyncMQTT_Generic` libraries together


```cpp
Start Async_AdvancedWebServer_SendChunked_MQTT on RASPBERRY_PI_PICO_W with RP2040W CYW43439 WiFi
AsyncTCP_RP2040W v1.1.0
AsyncWebServer_RP2040W v1.5.0
AsyncMQTT_Generic v1.8.1 for RP2040W CYW43439 WiFi
Connecting to SSID: HueNet
SSID: HueNet
Local IP Address: 192.168.2.128
Country code: XX
Connecting to MQTT...
AsyncWebServer is @ IP : 192.168.2.128
.Connected to MQTT broker: broker.emqx.io, port: 1883
PubTopic: async-mqtt/RP2040W_Pub
************************************************
Session present: 0
Subscribing at QoS 2, packetId: 1
Publishing at QoS 0
Publishing at QoS 1, packetId: 2
Publishing at QoS 2, packetId: 3
************************************************
Subscribe acknowledged.
  packetId: 1
  qos: 2
Publish received.
  topic: async-mqtt/RP2040W_Pub
  message: RP2040W Test3
  qos: 2
  dup: 0
  retain: 1
  len: 13
  index: 0
  total: 13
Publish acknowledged.
  packetId: 2
Publish received.
  topic: async-mqtt/RP2040W_Pub
  message: RP2040W Test1
  qos: 0
  dup: 0
  retain: 0
  len: 13
  index: 0
  total: 13
Publish received.
  topic: async-mqtt/RP2040W_Pub
  message: RP2040W Test2
  qos: 1
  dup: 0
  retain: 0
  len: 13
  index: 0
  total: 13
Publish received.
  topic: async-mqtt/RP2040W_Pub
  message: RP2040W Test3
  qos: 2
  dup: 0
  retain: 0
  len: 13
  index: 0
  total: 13
Publish acknowledged.
  packetId: 3
...Publish received.
  topic: async-mqtt/RP2040W_Pub
  message: RP2040W Test2
  qos: 1
  dup: 1
  retain: 0
  len: 13
  index: 0
  total: 13
..Disconnected from MQTT.
.... .......... .
```


You can access the AsyncWebServer_RP2040W at the displayed server IP, e.g. `192.168.2.128`

<p align="center">
    <img src="https://github.com/khoih-prog/AsyncWebServer_RP2040W/raw/main/pics/Async_AdvancedWebServer_SendChunked_MQTT.png">
</p>

---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level `_RP2040W_AWS_LOGLEVEL_` from 0 to 4 in the library `cpp` files

```cpp
#define _RP2040W_AWS_LOGLEVEL_     1
```

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of Arduino IDE, the Arduino `arduino-pico` core or depending libraries.

Sometimes, the library will only work if you update the `arduino-pico` core to the latest version because I'm always using the latest cores /libraries.


---

### Issues ###

Submit issues to: [AsyncWebServer_RP2040W issues](https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues)

---
---

## TO DO

 1. Fix bug. Add enhancement


## DONE

 1. Add support to `RASPBERRY_PI_PICO_W` using `CYW43439 WiFi`
 2. Add Table of Contents
 3. Modify examples to display `country-code`
 4. Add tempo method to modify `arduino-pico` core to change `country-code`
 5. Fix issue with slow browsers or network. Check [Target stops responding after variable time when using Firefox on Windows 10 #3](https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues/3)
 6. Add functions and example `Async_AdvancedWebServer_favicon` to support `favicon.ico`
 7. Support using `CString` to save heap to send `very large data`. Check [request->send(200, textPlainStr, jsonChartDataCharStr); - Without using String Class - to save heap #8](https://github.com/khoih-prog/Portenta_H7_AsyncWebServer/pull/8)
 8. Fix `crash` when using `AsyncWebSockets server` and add example [Async_WebSocketsServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_WebSocketsServer) to demo the AsyncWebSockets Server with a Python [WSClient.py](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_WebSocketsServer/WSClient_Python/WSClient.py)
 9. Improve robustness of AsyncWebSockets server. Check [AsyncWebSocketServer_RP2040W crashes with "[AWS] ERROR: Too many messages queued" #6](https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues/6) and add example [Async_WebSocketsServer_Xtreme](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_WebSocketsServer_Xtreme) to demo the nearly highest possible WebSockets Server speed 
10. Add `LittleFS` functions such as `AsyncFSWebServer`
11. Add examples [Async_AdvancedWebServer_SendChunked](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_SendChunked) and [AsyncWebServer_SendChunked](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncWebServer_SendChunked) to demo how to use `beginChunkedResponse()` to send large `html` in chunks
12. Add examples [Async_AdvancedWebServer_SendChunked_MQTT](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_SendChunked_MQTT) and [AsyncWebServer_MQTT_RP2040W](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncWebServer_MQTT_RP2040W) to demo how to use `AsyncWebServer_RP2040W` and `AsyncMQTT_Generic` libraries together
13. Improve `README.md` so that links can be used in other sites, such as `PIO`


---
---

### Contributions and Thanks

1. Based on and modified from [Hristo Gochkov's ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer). Many thanks to [Hristo Gochkov](https://github.com/me-no-dev) for great [ESPAsyncWebServer Library](https://github.com/me-no-dev/ESPAsyncWebServer)
2. Thanks to [revell1](https://github.com/revell1) to 
- report the bug in [LED state appears to be reversed. #2](https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues/2), leading to v1.0.2
- request enhancement in [Target stops responding after variable time when using Firefox on Windows 10 #3](https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues/3), leading to v1.1.0
3. Thanks to [salasidis](https://github.com/salasidis) aka [rs77can](https://forum.arduino.cc/u/rs77can) to discuss and make the following `marvellous` PRs in [Portenta_H7_AsyncWebServer library](https://github.com/khoih-prog/Portenta_H7_AsyncWebServer)
- [request->send(200, textPlainStr, jsonChartDataCharStr); - Without using String Class - to save heap #8](https://github.com/khoih-prog/Portenta_H7_AsyncWebServer/pull/8), leading to `v1.2.0` to support using `CString` to save heap to send `very large data`
- [All memmove() removed - string no longer destroyed #11](https://github.com/khoih-prog/Portenta_H7_AsyncWebServer/pull/11), leading to `v1.2.1` to remove `memmove()` and not to destroy String anymore
4. Thanks to [drmue](https://github.com/drmue) to report the bugs in 
- [Can't connect to AsyncWebSocketServer_RP2040 via javascript #5](https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues/5)
- [AsyncWebSocketServer_RP2040W crashes with "[AWS] ERROR: Too many messages queued" #6](https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues/6)
leading to `v1.3.0` and `v1.3.1` to improve `AsyncWebSockets server`
5. Thanks to [roma2580](https://github.com/roma2580) to report and help fix the bug in 
- [catchAll handler not working #12](https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues/12)
leading to `v1.5.0` to fix `_catchAllHandler` not working bug


<table>
  <tr>
    <td align="center"><a href="https://github.com/me-no-dev"><img src="https://github.com/me-no-dev.png" width="100px;" alt="me-no-dev"/><br /><sub><b>⭐️⭐️ Hristo Gochkov</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/revell1"><img src="https://github.com/revell1.png" width="100px;" alt="revell1"/><br /><sub><b>revell1</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/salasidis"><img src="https://github.com/salasidis.png" width="100px;" alt="salasidis"/><br /><sub><b>⭐️ salasidis</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/drmue"><img src="https://github.com/drmue.png" width="100px;" alt="drmue"/><br /><sub><b>drmue</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/roma2580"><img src="https://github.com/roma2580.png" width="100px;" alt="roma2580"/><br /><sub><b>>⭐️ roma2580</b></sub></a><br /></td>
  </tr> 
</table>

---

### Contributing

If you want to contribute to this project:

- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [GPLv3](https://github.com/khoih-prog/AsyncWebServer_RP2040W/blob/main/LICENSE)

---

## Copyright

Copyright 2022- Khoi Hoang



