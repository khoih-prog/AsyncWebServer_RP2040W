/****************************************************************************************************************************
  AsyncFSEditor_RP2040W.h
  
  For RP2040W with CYW43439 WiFi
  
  AsyncWebServer_RP2040W is a library for the RP2040W with CYW43439 WiFi
  
  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_RP2040W
  Licensed under GPLv3 license
 
  Version: 1.4.0
  
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
 *****************************************************************************************************************************/

#ifndef RP2040W_ASYNC_FSEDITOR_H_
#define RP2040W_ASYNC_FSEDITOR_H_

#include "AsyncWebServer_RP2040W.h"

#include <LittleFS.h>

class AsyncFSEditor: public AsyncWebHandler
{
  private:

    FS _fs;

    String _username;
    String _password;
    bool _authenticated;
    uint32_t _startTime;

  public:

    AsyncFSEditor(const String& username = String(), const String& password = String(), const FS& fs = LittleFS);

    virtual bool canHandle(AsyncWebServerRequest *request) override final;
    virtual void handleRequest(AsyncWebServerRequest *request) override final;
    virtual void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data,
                              size_t len, bool final) override final;

    virtual bool isRequestHandlerTrivial() override final
    {
      return false;
    }
};

#endif // RP2040W_ASYNC_FSEDITOR_H_
