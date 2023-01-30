/****************************************************************************************************************************
  AsyncEventSource_RP2040W.cpp

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

#if !defined(_RP2040W_AWS_LOGLEVEL_)
  #define _RP2040W_AWS_LOGLEVEL_     1
#endif

#include "AsyncWebServer_RP2040W_Debug.h"

#include "Arduino.h"
#include "AsyncEventSource_RP2040W.h"

/////////////////////////////////////////////////

static String generateEventMessage(const char *message, const char *event, uint32_t id, uint32_t reconnect)
{
  String ev = "";

  if (reconnect)
  {
    ev += "retry: ";
    ev += String(reconnect);
    ev += "\r\n";
  }

  if (id)
  {
    ev += "id: ";
    ev += String(id);
    ev += "\r\n";
  }

  if (event != NULL)
  {
    ev += "event: ";
    ev += String(event);
    ev += "\r\n";
  }

  if (message != NULL)
  {
    size_t messageLen = strlen(message);
    char * lineStart = (char *)message;
    char * lineEnd;

    do
    {
      char * nextN = strchr(lineStart, '\n');
      char * nextR = strchr(lineStart, '\r');

      if (nextN == NULL && nextR == NULL)
      {
        size_t llen = ((char *)message + messageLen) - lineStart;
        char * ldata = (char *)malloc(llen + 1);

        if (ldata != NULL)
        {
          memcpy(ldata, lineStart, llen);
          ldata[llen] = 0;
          ev += "data: ";
          ev += ldata;
          ev += "\r\n\r\n";
          free(ldata);
        }

        lineStart = (char *)message + messageLen;
      }
      else
      {
        char * nextLine = NULL;

        if (nextN != NULL && nextR != NULL)
        {
          if (nextR < nextN)
          {
            lineEnd = nextR;

            if (nextN == (nextR + 1))
              nextLine = nextN + 1;
            else
              nextLine = nextR + 1;
          }
          else
          {
            lineEnd = nextN;

            if (nextR == (nextN + 1))
              nextLine = nextR + 1;
            else
              nextLine = nextN + 1;
          }
        }
        else if (nextN != NULL)
        {
          lineEnd = nextN;
          nextLine = nextN + 1;
        }
        else
        {
          lineEnd = nextR;
          nextLine = nextR + 1;
        }

        size_t llen = lineEnd - lineStart;
        char * ldata = (char *)malloc(llen + 1);

        if (ldata != NULL)
        {
          memcpy(ldata, lineStart, llen);
          ldata[llen] = 0;
          ev += "data: ";
          ev += ldata;
          ev += "\r\n";

          free(ldata);
        }

        lineStart = nextLine;

        if (lineStart == ((char *)message + messageLen))
          ev += "\r\n";
      }
    } while (lineStart < ((char *)message + messageLen));
  }

  return ev;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

// Message

AsyncEventSourceMessage::AsyncEventSourceMessage(const char * data, size_t len)
  : _data(nullptr), _len(len), _sent(0), _acked(0)
{
  _data = (uint8_t*)malloc(_len + 1);

  if (_data == nullptr)
  {
    _len = 0;
  }
  else
  {
    memcpy(_data, data, len);
    _data[_len] = 0;
  }
}

/////////////////////////////////////////////////

AsyncEventSourceMessage::~AsyncEventSourceMessage()
{
  if (_data != NULL)
    free(_data);
}

/////////////////////////////////////////////////

size_t AsyncEventSourceMessage::ack(size_t len, uint32_t time)
{
  RP2040W_AWS_UNUSED(time);

  // If the whole message is now acked...
  if (_acked + len > _len)
  {
    // Return the number of extra bytes acked (they will be carried on to the next message)
    const size_t extra = _acked + len - _len;
    _acked = _len;

    return extra;
  }

  // Return that no extra bytes left.
  _acked += len;

  return 0;
}

/////////////////////////////////////////////////

size_t AsyncEventSourceMessage::send(AsyncClient *client)
{
  const size_t len = _len - _sent;

  if (client->space() < len)
  {
    return 0;
  }

  size_t sent = client->add((const char *)_data, len);

  if (client->canSend())
    client->send();

  _sent += sent;

  return sent;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

// Client

AsyncEventSourceClient::AsyncEventSourceClient(AsyncWebServerRequest *request, AsyncEventSource *server)
  : _messageQueue(LinkedList<AsyncEventSourceMessage * >([](AsyncEventSourceMessage * m)
{
  delete  m;
}))
{
  _client = request->client();
  _server = server;
  _lastId = 0;

  if (request->hasHeader("Last-Event-ID"))
    _lastId = atoi(request->getHeader("Last-Event-ID")->value().c_str());

  _client->setRxTimeout(0);
  _client->onError(NULL, NULL);
  _client->onAck([](void *r, AsyncClient * c, size_t len, uint32_t time)
  {
    RP2040W_AWS_UNUSED(c);
    ((AsyncEventSourceClient*)(r))->_onAck(len, time);
  }, this);

  _client->onPoll([](void *r, AsyncClient * c)
  {
    RP2040W_AWS_UNUSED(c);
    ((AsyncEventSourceClient*)(r))->_onPoll();
  }, this);

  _client->onData(NULL, NULL);

  _client->onTimeout([this](void *r, AsyncClient * c __attribute__((unused)), uint32_t time)
  {
    ((AsyncEventSourceClient*)(r))->_onTimeout(time);
  }, this);

  _client->onDisconnect([this](void *r, AsyncClient * c)
  {
    ((AsyncEventSourceClient*)(r))->_onDisconnect();
    delete c;
  }, this);

  _server->_addClient(this);

  delete request;
}

/////////////////////////////////////////////////

AsyncEventSourceClient::~AsyncEventSourceClient()
{
  _messageQueue.free();
  close();
}

/////////////////////////////////////////////////

void AsyncEventSourceClient::_queueMessage(AsyncEventSourceMessage *dataMessage)
{
  if (dataMessage == NULL)
    return;

  if (!connected())
  {
    delete dataMessage;
    return;
  }

  if (_messageQueue.length() >= SSE_MAX_QUEUED_MESSAGES)
  {
    AWS_LOGERROR("AsyncEventSourceClient::_queueMessage ERROR: Large MsQ");

    delete dataMessage;
  }
  else
  {
    _messageQueue.add(dataMessage);
  }

  if (_client->canSend())
    _runQueue();
}

/////////////////////////////////////////////////

void AsyncEventSourceClient::_onAck(size_t len, uint32_t time)
{
  AWS_LOGDEBUG("AsyncEventSourceClient::_onAck");

  while (len && !_messageQueue.isEmpty())
  {
    len = _messageQueue.front()->ack(len, time);

    if (_messageQueue.front()->finished())
      _messageQueue.remove(_messageQueue.front());
  }

  _runQueue();
}

/////////////////////////////////////////////////

void AsyncEventSourceClient::_onPoll()
{
  AWS_LOGDEBUG("AsyncEventSourceClient::_onPoll");

  if (!_messageQueue.isEmpty())
  {
    _runQueue();
  }
}

/////////////////////////////////////////////////

void AsyncEventSourceClient::_onTimeout(uint32_t time __attribute__((unused)))
{
  AWS_LOGDEBUG("AsyncEventSourceClient::_onTimeout");

  _client->close(true);
}

/////////////////////////////////////////////////

void AsyncEventSourceClient::_onDisconnect()
{
  AWS_LOGDEBUG("AsyncEventSourceClient::_onDisconnect");

  _client = NULL;
  _server->_handleDisconnect(this);
}

/////////////////////////////////////////////////

void AsyncEventSourceClient::close()
{
  AWS_LOGDEBUG("AsyncEventSourceClient::close");

  if (_client != NULL)
    _client->close();
}

/////////////////////////////////////////////////

void AsyncEventSourceClient::write(const char * message, size_t len)
{
  AWS_LOGDEBUG3("AsyncEventSourceClient::write: message =", message, ", len =", len);

  _queueMessage(new AsyncEventSourceMessage(message, len));
}

/////////////////////////////////////////////////

void AsyncEventSourceClient::send(const char *message, const char *event, uint32_t id, uint32_t reconnect)
{
  AWS_LOGDEBUG5("AsyncEventSourceClient::send: message =", message, ", event =", event, ", id =", id);

  String ev = generateEventMessage(message, event, id, reconnect);
  _queueMessage(new AsyncEventSourceMessage(ev.c_str(), ev.length()));
}

/////////////////////////////////////////////////

void AsyncEventSourceClient::_runQueue()
{
  while (!_messageQueue.isEmpty() && _messageQueue.front()->finished())
  {
    _messageQueue.remove(_messageQueue.front());
  }

  for (auto i = _messageQueue.begin(); i != _messageQueue.end(); ++i)
  {
    if (!(*i)->sent())
      (*i)->send(_client);
  }
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

// Handler

AsyncEventSource::AsyncEventSource(const String& url)
  : _url(url)
  , _clients(LinkedList<AsyncEventSourceClient * >([](AsyncEventSourceClient * c)
{
  delete c;
}))
, _connectcb(NULL)
{}

/////////////////////////////////////////////////

AsyncEventSource::~AsyncEventSource()
{
  close();
}

/////////////////////////////////////////////////

void AsyncEventSource::onConnect(ArEventHandlerFunction cb)
{
  AWS_LOGDEBUG("AsyncEventSource::onConnect");

  _connectcb = cb;
}

/////////////////////////////////////////////////

void AsyncEventSource::_addClient(AsyncEventSourceClient * client)
{
  /*char * temp = (char *)malloc(2054);
    if(temp != NULL){
    memset(temp+1,' ',2048);
    temp[0] = ':';
    temp[2049] = '\r';
    temp[2050] = '\n';
    temp[2051] = '\r';
    temp[2052] = '\n';
    temp[2053] = 0;
    client->write((const char *)temp, 2053);
    free(temp);
    }*/

  AWS_LOGDEBUG("AsyncEventSource::_addClient");

  _clients.add(client);

  if (_connectcb)
    _connectcb(client);
}

/////////////////////////////////////////////////

void AsyncEventSource::_handleDisconnect(AsyncEventSourceClient * client)
{
  AWS_LOGDEBUG("AsyncEventSource::_handleDisconnect");
  _clients.remove(client);
}

/////////////////////////////////////////////////

void AsyncEventSource::close()
{
  AWS_LOGDEBUG("AsyncEventSource::close");

  for (const auto &c : _clients)
  {
    if (c->connected())
      c->close();
  }
}

/////////////////////////////////////////////////

// pmb fix
size_t AsyncEventSource::avgPacketsWaiting() const
{
  if (_clients.isEmpty())
    return 0;

  size_t    aql = 0;
  uint32_t  nConnectedClients = 0;

  for (const auto &c : _clients)
  {
    if (c->connected())
    {
      aql += c->packetsWaiting();
      ++nConnectedClients;
    }
  }

  //  return aql / nConnectedClients;
  return ((aql) + (nConnectedClients / 2)) / (nConnectedClients); // round up
}

/////////////////////////////////////////////////

void AsyncEventSource::send(const char *message, const char *event, uint32_t id, uint32_t reconnect)
{
  String ev = generateEventMessage(message, event, id, reconnect);

  for (const auto &c : _clients)
  {
    if (c->connected())
    {
      c->write(ev.c_str(), ev.length());
    }
  }
}

/////////////////////////////////////////////////

size_t AsyncEventSource::count() const
{
  return _clients.count_if([](AsyncEventSourceClient * c)
  {
    return c->connected();
  });
}

/////////////////////////////////////////////////

bool AsyncEventSource::canHandle(AsyncWebServerRequest *request)
{
  if (request->method() != HTTP_GET || !request->url().equals(_url))
  {
    return false;
  }

  request->addInterestingHeader("Last-Event-ID");

  return true;
}

/////////////////////////////////////////////////

void AsyncEventSource::handleRequest(AsyncWebServerRequest *request)
{
  if ((_username != "" && _password != "") && !request->authenticate(_username.c_str(), _password.c_str()))
    return request->requestAuthentication();

  request->send(new AsyncEventSourceResponse(this));
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

// Response

AsyncEventSourceResponse::AsyncEventSourceResponse(AsyncEventSource *server)
{
  _server = server;
  _code = 200;
  _contentType = "text/event-stream";
  _sendContentLength = false;
  addHeader("Cache-Control", "no-cache");
  addHeader("Connection", "keep-alive");
}

/////////////////////////////////////////////////

void AsyncEventSourceResponse::_respond(AsyncWebServerRequest *request)
{
  String out = _assembleHead(request->version());
  request->client()->write(out.c_str(), _headLength);
  _state = RESPONSE_WAIT_ACK;
}

/////////////////////////////////////////////////

size_t AsyncEventSourceResponse::_ack(AsyncWebServerRequest *request, size_t len, uint32_t time __attribute__((unused)))
{
  if (len)
  {
    new AsyncEventSourceClient(request, _server);
  }

  return 0;
}

/////////////////////////////////////////////////


