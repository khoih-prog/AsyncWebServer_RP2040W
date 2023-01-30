/*************************************************************************************************************
  AsyncEventSource_RP2040W.h

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

#ifndef RP2040W_ASYNCEVENTSOURCE_H_
#define RP2040W_ASYNCEVENTSOURCE_H_

#include <Arduino.h>

#include <AsyncTCP_RP2040W.h>
#include <AsyncWebServer_RP2040W.h>
#include "AsyncWebSynchronization_RP2040W.h"

#include <Crypto/Hash.h>

/////////////////////////////////////////////////

#define SSE_MAX_QUEUED_MESSAGES 32
//#define SSE_MAX_QUEUED_MESSAGES 8

#define DEFAULT_MAX_SSE_CLIENTS 8
//#define DEFAULT_MAX_SSE_CLIENTS 4

/////////////////////////////////////////////////

class AsyncEventSource;
class AsyncEventSourceResponse;
class AsyncEventSourceClient;
typedef std::function<void(AsyncEventSourceClient *client)> ArEventHandlerFunction;

/////////////////////////////////////////////////

class AsyncEventSourceMessage
{
  private:
    uint8_t * _data;
    size_t _len;
    size_t _sent;
    //size_t _ack;
    size_t _acked;

  public:
    AsyncEventSourceMessage(const char * data, size_t len);
    ~AsyncEventSourceMessage();
    size_t ack(size_t len, uint32_t time __attribute__((unused)));
    size_t send(AsyncClient *client);

    /////////////////////////////////////////////////

    inline bool finished()
    {
      return _acked == _len;
    }

    /////////////////////////////////////////////////

    inline bool sent()
    {
      return _sent == _len;
    }
};

/////////////////////////////////////////////////
/////////////////////////////////////////////////

class AsyncEventSourceClient
{
  private:
    AsyncClient *_client;
    AsyncEventSource *_server;
    uint32_t _lastId;
    LinkedList<AsyncEventSourceMessage *> _messageQueue;
    void _queueMessage(AsyncEventSourceMessage *dataMessage);
    void _runQueue();

  public:

    AsyncEventSourceClient(AsyncWebServerRequest *request, AsyncEventSource *server);
    ~AsyncEventSourceClient();

    /////////////////////////////////////////////////

    inline AsyncClient* client()
    {
      return _client;
    }

    /////////////////////////////////////////////////

    void close();
    void write(const char * message, size_t len);
    void send(const char *message, const char *event = NULL, uint32_t id = 0, uint32_t reconnect = 0);

    /////////////////////////////////////////////////

    inline bool connected() const
    {
      return (_client != NULL) && _client->connected();
    }

    /////////////////////////////////////////////////

    inline uint32_t lastId() const
    {
      return _lastId;
    }

    /////////////////////////////////////////////////

    inline size_t  packetsWaiting() const
    {
      return _messageQueue.length();
    }

    /////////////////////////////////////////////////

    //system callbacks (do not call)
    void _onAck(size_t len, uint32_t time);
    void _onPoll();
    void _onTimeout(uint32_t time);
    void _onDisconnect();
};

/////////////////////////////////////////////////
/////////////////////////////////////////////////

class AsyncEventSource: public AsyncWebHandler
{
  private:
    String _url;
    LinkedList<AsyncEventSourceClient *> _clients;
    ArEventHandlerFunction _connectcb;

  public:
    AsyncEventSource(const String& url);
    ~AsyncEventSource();

    /////////////////////////////////////////////////

    inline const char * url() const
    {
      return _url.c_str();
    }

    /////////////////////////////////////////////////

    void close();
    void onConnect(ArEventHandlerFunction cb);
    void send(const char *message, const char *event = NULL, uint32_t id = 0, uint32_t reconnect = 0);
    size_t count() const; //number clinets connected
    size_t  avgPacketsWaiting() const;

    //system callbacks (do not call)
    void _addClient(AsyncEventSourceClient * client);
    void _handleDisconnect(AsyncEventSourceClient * client);
    virtual bool canHandle(AsyncWebServerRequest *request) override final;
    virtual void handleRequest(AsyncWebServerRequest *request) override final;
};

/////////////////////////////////////////////////
/////////////////////////////////////////////////

class AsyncEventSourceResponse: public AsyncWebServerResponse
{
  private:
    String _content;
    AsyncEventSource *_server;

  public:
    AsyncEventSourceResponse(AsyncEventSource *server);
    void _respond(AsyncWebServerRequest *request);
    size_t _ack(AsyncWebServerRequest *request, size_t len, uint32_t time);

    /////////////////////////////////////////////////

    inline bool _sourceValid() const
    {
      return true;
    }
};

#endif /* RP2040W_ASYNCEVENTSOURCE_H_ */
