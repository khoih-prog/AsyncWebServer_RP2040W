/****************************************************************************************************************************
  AsyncWebAuthentication_RP2040W.h

  For RP2040W with CYW43439 WiFi

  AsyncWebServer_RP2040W is a library for the RP2040W with CYW43439 WiFi

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_RP2040W
  Licensed under GPLv3 license

  Version: 1.4.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      13/08/2022 Initial coding for RP2040W with CYW43439 WiFi
  1.0.1   K Hoang      15/08/2022 Fix bug in examples, `library.json`
  1.0.2   K Hoang      15/08/2022 Fix LED bug in examples
  1.0.3   K Hoang      22/09/2022 To display country-code and tempo method to modify in arduino-pico core
  1.1.0   K Hoang      25/09/2022 Fix issue with slow browsers or network
  1.1.2   K Hoang      26/09/2022 Add function and example to support favicon.ico
  1.2.0   K Hoang      03/10/2022 Option to use cString instead of String to save Heap
  1.2.1   K Hoang      05/10/2022 Don't need memmove(), String no longer destroyed
  1.3.0   K Hoang      10/10/2022 Fix crash when using AsyncWebSockets server
  1.3.1   K Hoang      10/10/2022 Improve robustness of AsyncWebSockets server
  1.4.0   K Hoang      20/10/2022 Add LittleFS functions such as AsyncFSWebServer
  1.4.1   K Hoang      10/11/2022 Add examples to demo how to use beginChunkedResponse() to send in chunks
 *****************************************************************************************************************************/

#pragma once

#ifndef RP2040W_ASYNCWEB_AUTHENTICATION_H_
#define RP2040W_ASYNCWEB_AUTHENTICATION_H_

#include "Arduino.h"
#include "AsyncWebServer_RP2040W_Debug.h"

/////////////////////////////////////////////////

bool checkBasicAuthentication(const char * header, const char * username, const char * password);

String requestDigestAuthentication(const char * realm);

bool checkDigestAuthentication(const char * header, const char * method, const char * username, const char * password,
                               const char * realm, bool passwordIsHash, const char * nonce, const char * opaque, const char * uri);

//for storing hashed versions on the device that can be authenticated against
String generateDigestHash(const char * username, const char * password, const char * realm);

/////////////////////////////////////////////////

#endif    // ARP2040W_SYNCWEB_AUTHENTICATIO_H_
