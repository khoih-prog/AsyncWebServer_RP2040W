# AsyncWebServer_RP2040W

[![arduino-library-badge](https://www.ardu-badge.com/badge/AsyncWebServer_RP2040W.svg?)](https://www.ardu-badge.com/AsyncWebServer_RP2040W)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/AsyncWebServer_RP2040W.svg)](https://github.com/khoih-prog/AsyncWebServer_RP2040W/releases)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/AsyncWebServer_RP2040W.svg)](http://github.com/khoih-prog/AsyncWebServer_RP2040W/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>


---
---

## Table of Contents

* [Changelog](#changelog)
  * [Release v1.5.0](#Release-v150)
  * [Release v1.4.2](#Release-v142)
  * [Release v1.4.1](#Release-v141)
  * [Release v1.4.0](#Release-v140)
  * [Release v1.3.1](#Release-v131)
  * [Release v1.3.0](#Release-v130)
  * [Release v1.2.1](#Release-v121)
  * [Release v1.2.0](#Release-v120)
  * [Release v1.1.2](#Release-v112)
  * [Release v1.1.1](#Release-v111)
  * [Release v1.1.0](#Release-v110)
  * [Release v1.0.3](#Release-v103)
  * [Release v1.0.2](#Release-v102)
  * [Release v1.0.1](#Release-v101)
  * [Initial Release v1.0.0](#Initial-Release-v100)

---
---

## Changelog

#### Release v1.5.0

1. Fix `_catchAllHandler` not working bug. Check [catchAll handler not working #12](https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues/12)

#### Release v1.4.2

1. Add examples [Async_AdvancedWebServer_SendChunked_MQTT](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_SendChunked_MQTT) and [AsyncWebServer_MQTT_RP2040W](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncWebServer_MQTT_RP2040W) to demo how to use `AsyncWebServer_RP2040W` and `AsyncMQTT_Generic` libraries together
2. Improve `README.md` so that links can be used in other sites, such as `PIO`

#### Release v1.4.1

1. Add examples [Async_AdvancedWebServer_SendChunked](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_SendChunked) and [AsyncWebServer_SendChunked](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncWebServer_SendChunked) to demo how to use `beginChunkedResponse()` to send large `html` in chunks
2. Use `allman astyle` and add `utils`

#### Release v1.4.0

1. Add `LittleFS` functions such as AsyncFSWebServer
2. Add examples [AsyncFSWebServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncFSWebServer) and [AsyncFSWebServer_Complex](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/AsyncFSWebServer_Complex) to demo the new feature

#### Release v1.3.1

1. Improve robustness of AsyncWebSockets server. Check [AsyncWebSocketServer_RP2040W crashes with "[AWS] ERROR: Too many messages queued" #6](https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues/6)
2. Add example [Async_WebSocketsServer_Xtreme](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_WebSocketsServer_Xtreme) to demo the nearly highest possible WebSockets Server speed

#### Release v1.3.0

1. Fix `crash` when using `AsyncWebSockets server`. Check [Can't connect to AsyncWebSocketServer_RP2040 via javascript #5](https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues/5)
2. Add example [Async_WebSocketsServer](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_WebSocketsServer) to demo the AsyncWebSockets Server with a `Python` [WSClient.py](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_WebSocketsServer/WSClient_Python/WSClient.py)

#### Release v1.2.1

1. Don't need `memmove()`, CString no longer destroyed. Check [All memmove() removed - string no longer destroyed #11](https://github.com/khoih-prog/Portenta_H7_AsyncWebServer/pull/11)

### Release v1.2.0

1. Support using `CString` to save heap to send `very large data`. Check [request->send(200, textPlainStr, jsonChartDataCharStr); - Without using String Class - to save heap #8](https://github.com/khoih-prog/Portenta_H7_AsyncWebServer/pull/8)
2. Add multiple examples to demo the new feature

### Release v1.1.2

1. Add functions and example `Async_AdvancedWebServer_favicon` to support `favicon.ico`

### Release v1.1.1

1. To avoid the issue `CRC doesn't match, file is corrupted` with Arduino IDE Library Manager installation of v1.1.0

### Release v1.1.0

1. Fix issue with slow browsers or network. Check [Target stops responding after variable time when using Firefox on Windows 10 #3](https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues/3)


### Release v1.0.3

1. Modify examples to display `country-code`
2. Add tempo method to modify `arduino-pico` core to change `country-code`
3. Add example [Async_AdvancedWebServer_Country](https://github.com/khoih-prog/AsyncWebServer_RP2040W/tree/main/examples/Async_AdvancedWebServer_Country)

### Release v1.0.2

1. Fix LED bug in examples

### Release v1.0.1

1. Fix bug in examples, `library.json`
2. Enhance `README.md`

### Initial Release v1.0.0

1. Initial coding to support **RASPBERRY_PI_PICO_W with CYW43439 WiFi**, using [**arduino-pico core v2.4.0+**](https://github.com/earlephilhower/arduino-pico)
