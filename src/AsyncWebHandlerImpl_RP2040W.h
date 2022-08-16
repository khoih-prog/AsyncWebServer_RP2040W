/****************************************************************************************************************************
  AsyncWebHandlerImpl_RP2040W.h
  
  For RP2040W with CYW43439 WiFi
  
  AsyncWebServer_RP2040W is a library for the RP2040W with CYW43439 WiFi
  
  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_RP2040W
  Licensed under GPLv3 license
 
  Version: 1.0.1
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      13/08/2022 Initial coding for RP2040W with CYW43439 WiFi
  1.0.1   K Hoang      15/08/2022 Fix bug in examples, `library.json`
 *****************************************************************************************************************************/

#pragma once

#ifndef RP2040W_ASYNCWEBSERVERHANDLERIMPL_H_
#define RP2040W_ASYNCWEBSERVERHANDLERIMPL_H_

#include <string>

#ifdef ASYNCWEBSERVER_REGEX
#include <regex>
#endif

#include "stddef.h"
#include <time.h>

class AsyncStaticWebHandler: public AsyncWebHandler
{
  private:
    uint8_t _countBits(const uint8_t value) const;

  protected:
    String _uri;
    String _path;
    String _cache_control;
    String _last_modified;
    AwsTemplateProcessor _callback;
    bool _isDir;
    bool _gzipFirst;
    uint8_t _gzipStats;

  public:
    AsyncStaticWebHandler(const char* uri, const char* path, const char* cache_control);
    virtual bool canHandle(AsyncWebServerRequest *request) override final;
    virtual void handleRequest(AsyncWebServerRequest *request) override final;
    AsyncStaticWebHandler& setIsDir(bool isDir);
    AsyncStaticWebHandler& setCacheControl(const char* cache_control);
    AsyncStaticWebHandler& setLastModified(const char* last_modified);
    AsyncStaticWebHandler& setLastModified(struct tm* last_modified);

    AsyncStaticWebHandler& setLastModified(time_t last_modified);
    AsyncStaticWebHandler& setLastModified(); //sets to current time. Make sure sntp is runing and time is updated

    AsyncStaticWebHandler& setTemplateProcessor(AwsTemplateProcessor newCallback)
    {
      _callback = newCallback;
      return *this;
    }
};

class AsyncCallbackWebHandler: public AsyncWebHandler
{
  private:
  protected:
    String _uri;
    WebRequestMethodComposite _method;
    ArRequestHandlerFunction _onRequest;
    ArUploadHandlerFunction _onUpload;
    ArBodyHandlerFunction _onBody;
    bool _isRegex;

  public:
    AsyncCallbackWebHandler() : _uri(), _method(HTTP_ANY), _onRequest(NULL), _onUpload(NULL), _onBody(NULL), _isRegex(false) {}

    void setUri(const String& uri)
    {
      _uri = uri;
      _isRegex = uri.startsWith("^") && uri.endsWith("$");
    }

    void setMethod(WebRequestMethodComposite method)
    {
      _method = method;
    }
    void onRequest(ArRequestHandlerFunction fn)
    {
      _onRequest = fn;
    }

    void onUpload(ArUploadHandlerFunction fn)
    {
      _onUpload = fn;
    }

    void onBody(ArBodyHandlerFunction fn)
    {
      _onBody = fn;
    }

    virtual bool canHandle(AsyncWebServerRequest *request) override final
    {
      if (!_onRequest)
        return false;

      if (!(_method & request->method()))
        return false;

#ifdef ASYNCWEBSERVER_REGEX
      if (_isRegex)
      {
        std::regex pattern(_uri.c_str());
        std::smatch matches;
        std::string s(request->url().c_str());

        if (std::regex_search(s, matches, pattern))
        {
          for (size_t i = 1; i < matches.size(); ++i)
          {
            // start from 1
            request->_addPathParam(matches[i].str().c_str());
          }
        }
        else
        {
          return false;
        }
      }
      else
#endif
        if (_uri.length() && _uri.endsWith("*"))
        {
          String uriTemplate = String(_uri);
          uriTemplate = uriTemplate.substring(0, uriTemplate.length() - 1);

          if (!request->url().startsWith(uriTemplate))
            return false;
        }
        else if (_uri.length() && (_uri != request->url() && !request->url().startsWith(_uri + "/")))
          return false;

      request->addInterestingHeader("ANY");

      return true;
    }

    virtual void handleRequest(AsyncWebServerRequest *request) override final
    {
      if (_onRequest)
        _onRequest(request);
      else
        request->send(500);
    }

    virtual void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) override final
    {
      if (_onBody)
        _onBody(request, data, len, index, total);
    }

    virtual bool isRequestHandlerTrivial() override final
    {
      return _onRequest ? false : true;
    }
};

#endif /* RP2040W_ASYNCWEBSERVERHANDLERIMPL_H_ */
