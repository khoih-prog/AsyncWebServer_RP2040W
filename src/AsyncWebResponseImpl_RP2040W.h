/****************************************************************************************************************************
  AsyncWebResponseImpl_RP2040W.h

  For RP2040W with CYW43439 WiFi

  AsyncWebServer_RP2040W is a library for the RP2040W with CYW43439 WiFi

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_RP2040W
  Licensed under GPLv3 license

  Version: 1.5.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      13/08/2022 Initial coding for RP2040W with CYW43439 WiFi
  ...
  1.3.0   K Hoang      10/10/2022 Fix crash when using AsyncWebSockets server
  1.3.1   K Hoang      10/10/2022 Improve robustness of AsyncWebSockets server
  1.4.0   K Hoang      20/10/2022 Add LittleFS functions such as AsyncFSWebServer
  1.4.1   K Hoang      10/11/2022 Add examples to demo how to use beginChunkedResponse() to send in chunks
  1.4.2   K Hoang      28/01/2023 Add Async_AdvancedWebServer_SendChunked_MQTT and AsyncWebServer_MQTT_RP2040W examples
  1.5.0   K Hoang      30/01/2023 Fix _catchAllHandler not working bug
 *****************************************************************************************************************************/

#pragma once

#ifndef RP2040W_ASYNCWEBSERVERRESPONSEIMPL_H_
#define RP2040W_ASYNCWEBSERVERRESPONSEIMPL_H_

/////////////////////////////////////////////////

#ifdef Arduino_h
  // arduino is not compatible with std::vector
  #undef min
  #undef max
#endif

/////////////////////////////////////////////////

#include <vector>

/////////////////////////////////////////////////

// It is possible to restore these defines, but one can use _min and _max instead. Or std::min, std::max.

class AsyncBasicResponse: public AsyncWebServerResponse
{
  private:
    String _content;

    char *_contentCstr;      // RSMOD
    String _partialHeader;

  public:
    AsyncBasicResponse(int code, const String& contentType = String(), const String& content = String());

    AsyncBasicResponse(int code, const String& contentType, const char *content = nullptr);     // RSMOD

    // KH add for favicon
    //AsyncBasicResponse(int code, const String& contentType, const uint8_t * content, size_t len);
    //////

    void _respond(AsyncWebServerRequest *request);

    size_t _ack(AsyncWebServerRequest *request, size_t len, uint32_t time);

    /////////////////////////////////////////////////

    inline bool _sourceValid() const
    {
      return true;
    }

    /////////////////////////////////////////////////
};

/////////////////////////////////////////////////

class AsyncAbstractResponse: public AsyncWebServerResponse
{
  private:
    String _head;
    // Data is inserted into cache at begin().
    // This is inefficient with vector, but if we use some other container,
    // we won't be able to access it as contiguous array of bytes when reading from it,
    // so by gaining performance in one place, we'll lose it in another.
    std::vector<uint8_t> _cache;
    size_t _readDataFromCacheOrContent(uint8_t* data, const size_t len);
    size_t _fillBufferAndProcessTemplates(uint8_t* buf, size_t maxLen);

  protected:
    AwsTemplateProcessor _callback;

  public:
    AsyncAbstractResponse(AwsTemplateProcessor callback = nullptr);
    void _respond(AsyncWebServerRequest *request);
    size_t _ack(AsyncWebServerRequest *request, size_t len, uint32_t time);

    /////////////////////////////////////////////////

    inline bool _sourceValid() const
    {
      return false;
    }

    /////////////////////////////////////////////////

    virtual size_t _fillBuffer(uint8_t *buf __attribute__((unused)), size_t maxLen __attribute__((unused)))
    {
      return 0;
    }

    /////////////////////////////////////////////////

};

/////////////////////////////////////////////////

/*
   Fake Progmem Response
 * */
class AsyncProgmemResponse: public AsyncAbstractResponse
{
  private:
    const uint8_t * _content;
    size_t _readLength;

  public:
    AsyncProgmemResponse(int code, const String& contentType, const uint8_t * content, size_t len,
                         AwsTemplateProcessor callback = nullptr);

    /////////////////////////////////////////////////

    inline bool _sourceValid() const
    {
      return true;
    }

    /////////////////////////////////////////////////

    virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override;
};

/////////////////////////////////////////////////

#ifndef TEMPLATE_PLACEHOLDER
  #define TEMPLATE_PLACEHOLDER '%'
#endif

#define TEMPLATE_PARAM_NAME_LENGTH 32

/////////////////////////////////////////////////

class AsyncFileResponse: public AsyncAbstractResponse
{
  private:
    File _content;
    String _path;
    void _setContentType(const String& path);

  public:
    AsyncFileResponse(FS &fs, const String& path, const String& contentType = String(), bool download = false,
                      AwsTemplateProcessor callback = nullptr);
    AsyncFileResponse(File content, const String& path, const String& contentType = String(), bool download = false,
                      AwsTemplateProcessor callback = nullptr);

    ~AsyncFileResponse();

    inline bool _sourceValid() const
    {
      return !!(_content);
    }

    virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override;
};

/////////////////////////////////////////////////

class AsyncStreamResponse: public AsyncAbstractResponse
{
  private:
    Stream *_content;

  public:
    AsyncStreamResponse(Stream &stream, const String& contentType, size_t len, AwsTemplateProcessor callback = nullptr);

    /////////////////////////////////////////////////

    inline bool _sourceValid() const
    {
      return !!(_content);
    }

    /////////////////////////////////////////////////

    virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override;
};

/////////////////////////////////////////////////

class AsyncCallbackResponse: public AsyncAbstractResponse
{
  private:
    AwsResponseFiller _content;
    size_t _filledLength;

  public:
    AsyncCallbackResponse(const String& contentType, size_t len, AwsResponseFiller callback,
                          AwsTemplateProcessor templateCallback = nullptr);

    /////////////////////////////////////////////////

    inline bool _sourceValid() const
    {
      return !!(_content);
    }

    /////////////////////////////////////////////////

    virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override;
};

/////////////////////////////////////////////////

class AsyncChunkedResponse: public AsyncAbstractResponse
{
  private:
    AwsResponseFiller _content;
    size_t _filledLength;

  public:
    AsyncChunkedResponse(const String& contentType, AwsResponseFiller callback,
                         AwsTemplateProcessor templateCallback = nullptr);

    /////////////////////////////////////////////////

    inline bool _sourceValid() const
    {
      return !!(_content);
    }

    /////////////////////////////////////////////////

    virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override;
};

/////////////////////////////////////////////////

class cbuf;

/////////////////////////////////////////////////

class AsyncResponseStream: public AsyncAbstractResponse, public Print
{
  private:
    cbuf *_content;

  public:
    AsyncResponseStream(const String& contentType, size_t bufferSize);
    ~AsyncResponseStream();

    /////////////////////////////////////////////////

    inline bool _sourceValid() const
    {
      return (_state < RESPONSE_END);
    }

    /////////////////////////////////////////////////

    virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override;
    size_t write(const uint8_t *data, size_t len);
    size_t write(uint8_t data);
    using Print::write;
};

/////////////////////////////////////////////////

#endif /* RP2040W_ASYNCWEBSERVERRESPONSEIMPL_H_ */
