/****************************************************************************************************************************
  AsyncWebSocket_RP2040W.h

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

#ifndef RP2040W_ASYNCWEBSOCKET_H_
#define RP2040W_ASYNCWEBSOCKET_H_

#include <Arduino.h>
#include <stdarg.h>

// AsyncTCP_RP2040W
#include <AsyncTCP_RP2040W.h>

// KH, Fix Message Queue too long error. Reduce from 32 to 4
// Longer queue takes longer time to process. If system is slow, longer queue is worse
// Anyway any overflowed message will be discarded later
// ESP32 using 8
#define WS_MAX_QUEUED_MESSAGES 4

//#define DEFAULT_MAX_WS_CLIENTS 8
#define DEFAULT_MAX_WS_CLIENTS 4

#include "AsyncWebSynchronization_RP2040W.h"

class AsyncWebSocket;
class AsyncWebSocketResponse;
class AsyncWebSocketClient;
class AsyncWebSocketControl;

/////////////////////////////////////////////////

typedef struct
{
  /** Message type as defined by enum AwsFrameType.
     Note: Applications will only see WS_TEXT and WS_BINARY.
     All other types are handled by the library. */
  uint8_t message_opcode;
  /** Frame number of a fragmented message. */
  uint32_t num;
  /** Is this the last frame in a fragmented message ?*/
  uint8_t final;
  /** Is this frame masked? */
  uint8_t masked;
  /** Message type as defined by enum AwsFrameType.
     This value is the same as message_opcode for non-fragmented
     messages, but may also be WS_CONTINUATION in a fragmented message. */
  uint8_t opcode;
  /** Length of the current frame.
     This equals the total length of the message if num == 0 && final == true */
  uint64_t len;
  /** Mask key */
  uint8_t mask[4];
  /** Offset of the data inside the current frame. */
  uint64_t index;
} AwsFrameInfo;

/////////////////////////////////////////////////

typedef enum
{
  WS_DISCONNECTED,
  WS_CONNECTED,
  WS_DISCONNECTING
} AwsClientStatus;

typedef enum
{
  WS_CONTINUATION,
  WS_TEXT,
  WS_BINARY,
  WS_DISCONNECT = 0x08,
  WS_PING,
  WS_PONG
} AwsFrameType;

typedef enum
{
  WS_MSG_SENDING,
  WS_MSG_SENT,
  WS_MSG_ERROR
} AwsMessageStatus;

typedef enum
{
  WS_EVT_CONNECT,
  WS_EVT_DISCONNECT,
  WS_EVT_PONG,
  WS_EVT_ERROR,
  WS_EVT_DATA
} AwsEventType;

/////////////////////////////////////////////////

class AsyncWebSocketMessageBuffer
{
  private:
    uint8_t * _data;
    size_t _len;
    bool _lock;
    uint32_t _count;

  public:
    AsyncWebSocketMessageBuffer();
    AsyncWebSocketMessageBuffer(size_t size);
    AsyncWebSocketMessageBuffer(uint8_t * data, size_t size);
    AsyncWebSocketMessageBuffer(const AsyncWebSocketMessageBuffer &);
    AsyncWebSocketMessageBuffer(AsyncWebSocketMessageBuffer &&);
    ~AsyncWebSocketMessageBuffer();

    /////////////////////////////////////////////////

    void operator ++(int i)
    {
      RP2040W_AWS_UNUSED(i);
      _count++;
    }

    /////////////////////////////////////////////////

    void operator --(int i)
    {
      RP2040W_AWS_UNUSED(i);

      if (_count > 0)
      {
        _count--;
      }
    }

    /////////////////////////////////////////////////

    bool reserve(size_t size);

    /////////////////////////////////////////////////

    inline void lock()
    {
      _lock = true;
    }

    /////////////////////////////////////////////////

    inline void unlock()
    {
      _lock = false;
    }

    /////////////////////////////////////////////////

    inline uint8_t * get()
    {
      return _data;
    }

    /////////////////////////////////////////////////

    inline size_t length()
    {
      return _len;
    }

    /////////////////////////////////////////////////

    inline uint32_t count()
    {
      return _count;
    }

    /////////////////////////////////////////////////

    inline bool canDelete()
    {
      return (!_count && !_lock);
    }

    /////////////////////////////////////////////////

    friend AsyncWebSocket;

};

/////////////////////////////////////////////////

class AsyncWebSocketMessage
{
  protected:
    uint8_t _opcode;
    bool _mask;
    AwsMessageStatus _status;

  public:
    AsyncWebSocketMessage(): _opcode(WS_TEXT), _mask(false), _status(WS_MSG_ERROR) {}
    virtual ~AsyncWebSocketMessage() {}
    virtual void ack(size_t len __attribute__((unused)), uint32_t time __attribute__((unused))) {}

    /////////////////////////////////////////////////

    virtual size_t send(AsyncClient *client __attribute__((unused)))
    {
      return 0;
    }

    /////////////////////////////////////////////////

    virtual bool finished()
    {
      return _status != WS_MSG_SENDING;
    }

    /////////////////////////////////////////////////

    virtual bool betweenFrames() const
    {
      return false;
    }
};

/////////////////////////////////////////////////

class AsyncWebSocketBasicMessage: public AsyncWebSocketMessage
{
  private:
    size_t _len;
    size_t _sent;
    size_t _ack;
    size_t _acked;
    uint8_t * _data;

  public:
    AsyncWebSocketBasicMessage(const char * data, size_t len, uint8_t opcode = WS_TEXT, bool mask = false);
    AsyncWebSocketBasicMessage(uint8_t opcode = WS_TEXT, bool mask = false);
    virtual ~AsyncWebSocketBasicMessage() override;

    /////////////////////////////////////////////////

    virtual bool betweenFrames() const override
    {
      return _acked == _ack;
    }

    /////////////////////////////////////////////////

    virtual void ack(size_t len, uint32_t time) override;
    virtual size_t send(AsyncClient *client) override;
};

/////////////////////////////////////////////////

class AsyncWebSocketMultiMessage: public AsyncWebSocketMessage
{
  private:
    uint8_t * _data;
    size_t _len;
    size_t _sent;
    size_t _ack;
    size_t _acked;
    AsyncWebSocketMessageBuffer * _WSbuffer;

  public:
    AsyncWebSocketMultiMessage(AsyncWebSocketMessageBuffer * buffer, uint8_t opcode = WS_TEXT, bool mask = false);
    virtual ~AsyncWebSocketMultiMessage() override;

    /////////////////////////////////////////////////

    virtual bool betweenFrames() const override
    {
      return _acked == _ack;
    }

    /////////////////////////////////////////////////

    virtual void ack(size_t len, uint32_t time) override ;
    virtual size_t send(AsyncClient *client) override ;
};

/////////////////////////////////////////////////

class AsyncWebSocketClient
{
  private:
    AsyncClient *_client;
    AsyncWebSocket *_server;
    uint32_t _clientId;
    AwsClientStatus _status;

    LinkedList<AsyncWebSocketControl *> _controlQueue;
    LinkedList<AsyncWebSocketMessage *> _messageQueue;

    uint8_t _pstate;
    AwsFrameInfo _pinfo;

    uint32_t _lastMessageTime;
    uint32_t _keepAlivePeriod;

    void _queueMessage(AsyncWebSocketMessage *dataMessage);
    void _queueControl(AsyncWebSocketControl *controlMessage);
    void _runQueue();

  public:
    void *_tempObject;

    AsyncWebSocketClient(AsyncWebServerRequest *request, AsyncWebSocket *server);
    ~AsyncWebSocketClient();

    /////////////////////////////////////////////////

    //client id increments for the given server
    inline uint32_t id()
    {
      return _clientId;
    }

    /////////////////////////////////////////////////

    inline AwsClientStatus status()
    {
      return _status;
    }

    /////////////////////////////////////////////////

    inline AsyncClient* client()
    {
      return _client;
    }

    /////////////////////////////////////////////////

    inline AsyncWebSocket *server()
    {
      return _server;
    }

    /////////////////////////////////////////////////

    inline AwsFrameInfo const &pinfo() const
    {
      return _pinfo;
    }

    /////////////////////////////////////////////////

    IPAddress remoteIP();
    uint16_t  remotePort();

    //control frames
    void close(uint16_t code = 0, const char * message = NULL);
    void ping(uint8_t *data = NULL, size_t len = 0);

    /////////////////////////////////////////////////

    //set auto-ping period in seconds. disabled if zero (default)
    inline void keepAlivePeriod(uint16_t seconds)
    {
      _keepAlivePeriod = seconds * 1000;
    }

    /////////////////////////////////////////////////

    inline uint16_t keepAlivePeriod()
    {
      return (uint16_t)(_keepAlivePeriod / 1000);
    }

    /////////////////////////////////////////////////

    //data packets
    void message(AsyncWebSocketMessage *message)
    {
      _queueMessage(message);
    }

    /////////////////////////////////////////////////

    bool queueIsFull();

    size_t printf(const char *format, ...)  __attribute__ ((format (printf, 2, 3)));

    void text(const char * message, size_t len);
    void text(const char * message);
    void text(uint8_t * message, size_t len);
    void text(char * message);
    void text(const String &message);
    void text(AsyncWebSocketMessageBuffer *buffer);

    void binary(const char * message, size_t len);
    void binary(const char * message);
    void binary(uint8_t * message, size_t len);
    void binary(char * message);
    void binary(const String &message);
    void binary(AsyncWebSocketMessageBuffer *buffer);

    /////////////////////////////////////////////////

    inline bool canSend()
    {
      return _messageQueue.length() < WS_MAX_QUEUED_MESSAGES;
    }

    /////////////////////////////////////////////////

    //system callbacks (do not call)
    void _onAck(size_t len, uint32_t time);
    void _onError(int8_t);
    void _onPoll();
    void _onTimeout(uint32_t time);
    void _onDisconnect();
    void _onData(void *pbuf, size_t plen);
};

/////////////////////////////////////////////////

typedef std::function<void(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)>
AwsEventHandler;

/////////////////////////////////////////////////

//WebServer Handler implementation that plays the role of a socket server
class AsyncWebSocket: public AsyncWebHandler
{
  public:
    typedef LinkedList<AsyncWebSocketClient *> AsyncWebSocketClientLinkedList;

  private:
    String _url;
    AsyncWebSocketClientLinkedList _clients;
    uint32_t _cNextId;
    AwsEventHandler _eventHandler;
    bool _enabled;
    AsyncWebLock _lock;

  public:
    AsyncWebSocket(const String& url);
    ~AsyncWebSocket();

    /////////////////////////////////////////////////

    inline const char * url() const
    {
      return _url.c_str();
    }

    /////////////////////////////////////////////////

    inline void enable(bool e)
    {
      _enabled = e;
    }

    /////////////////////////////////////////////////

    inline bool enabled() const
    {
      return _enabled;
    }

    /////////////////////////////////////////////////

    bool availableForWriteAll();
    bool availableForWrite(uint32_t id);

    size_t count() const;
    AsyncWebSocketClient * client(uint32_t id);

    /////////////////////////////////////////////////

    inline bool hasClient(uint32_t id)
    {
      return client(id) != NULL;
    }

    /////////////////////////////////////////////////

    void close(uint32_t id, uint16_t code = 0, const char * message = NULL);
    void closeAll(uint16_t code = 0, const char * message = NULL);
    void cleanupClients(uint16_t maxClients = DEFAULT_MAX_WS_CLIENTS);

    void ping(uint32_t id, uint8_t *data = NULL, size_t len = 0);
    void pingAll(uint8_t *data = NULL, size_t len = 0); //  done

    void text(uint32_t id, const char * message, size_t len);
    void text(uint32_t id, const char * message);
    void text(uint32_t id, uint8_t * message, size_t len);
    void text(uint32_t id, char * message);
    void text(uint32_t id, const String &message);

    void textAll(const char * message, size_t len);
    void textAll(const char * message);
    void textAll(uint8_t * message, size_t len);
    void textAll(char * message);
    void textAll(const String &message);
    void textAll(AsyncWebSocketMessageBuffer * buffer);

    void binary(uint32_t id, const char * message, size_t len);
    void binary(uint32_t id, const char * message);
    void binary(uint32_t id, uint8_t * message, size_t len);
    void binary(uint32_t id, char * message);
    void binary(uint32_t id, const String &message);

    void binaryAll(const char * message, size_t len);
    void binaryAll(const char * message);
    void binaryAll(uint8_t * message, size_t len);
    void binaryAll(char * message);
    void binaryAll(const String &message);
    void binaryAll(AsyncWebSocketMessageBuffer * buffer);

    void message(uint32_t id, AsyncWebSocketMessage *message);
    void messageAll(AsyncWebSocketMultiMessage *message);

    size_t printf(uint32_t id, const char *format, ...)  __attribute__ ((format (printf, 3, 4)));
    size_t printfAll(const char *format, ...)  __attribute__ ((format (printf, 2, 3)));

    /////////////////////////////////////////////////

    //event listener
    inline void onEvent(AwsEventHandler handler)
    {
      _eventHandler = handler;
    }

    /////////////////////////////////////////////////

    //system callbacks (do not call)
    inline uint32_t _getNextId()
    {
      return _cNextId++;
    }

    /////////////////////////////////////////////////

    void _addClient(AsyncWebSocketClient * client);
    void _handleDisconnect(AsyncWebSocketClient * client);
    void _handleEvent(AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
    virtual bool canHandle(AsyncWebServerRequest *request) override final;
    virtual void handleRequest(AsyncWebServerRequest *request) override final;

    //  messagebuffer functions/objects.
    AsyncWebSocketMessageBuffer * makeBuffer(size_t size = 0);
    AsyncWebSocketMessageBuffer * makeBuffer(uint8_t * data, size_t size);
    LinkedList<AsyncWebSocketMessageBuffer *> _buffers;
    void _cleanBuffers();

    AsyncWebSocketClientLinkedList getClients() const;
};

/////////////////////////////////////////////////

//WebServer response to authenticate the socket and detach the tcp client from the web server request
class AsyncWebSocketResponse: public AsyncWebServerResponse
{
  private:
    String _content;
    AsyncWebSocket *_server;

  public:
    AsyncWebSocketResponse(const String& key, AsyncWebSocket *server);
    void _respond(AsyncWebServerRequest *request);
    size_t _ack(AsyncWebServerRequest *request, size_t len, uint32_t time);

    /////////////////////////////////////////////////

    inline bool _sourceValid() const
    {
      return true;
    }
};

/////////////////////////////////////////////////

#endif /* RP2040W_ASYNCWEBSOCKET_H_ */
