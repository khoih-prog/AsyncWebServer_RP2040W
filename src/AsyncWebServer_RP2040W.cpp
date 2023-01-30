/****************************************************************************************************************************
  AsyncWebServer_RP2040W.cpp

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

/////////////////////////////////////////////////

#include "AsyncWebServer_RP2040W_Debug.h"

#include "AsyncWebServer_RP2040W.h"
#include "AsyncWebHandlerImpl_RP2040W.h"

/////////////////////////////////////////////////

bool ON_STA_FILTER(AsyncWebServerRequest *request) 
{
  return WiFi.localIP() == request->client()->localIP();
}

/////////////////////////////////////////////////

bool ON_AP_FILTER(AsyncWebServerRequest *request) 
{
  return WiFi.localIP() != request->client()->localIP();
}

/////////////////////////////////////////////////

AsyncWebServer::AsyncWebServer(uint16_t port)
  : _server(port), _rewrites(LinkedList<AsyncWebRewrite * >([](AsyncWebRewrite * r)
{
  delete r;
}))
, _handlers(LinkedList<AsyncWebHandler*>([](AsyncWebHandler* h)
{
  delete h;
}))
{
  _catchAllHandler = new AsyncCallbackWebHandler();

  if (_catchAllHandler == NULL)
  {
    AWS_LOGERROR("AsyncWebServer: _catchAllHandler NULL");
    
    return;
  }  

  _server.onClient([](void *s, AsyncClient * c)
  {
    if (c == NULL)
      return;

    // KH set no RxTimeout for slower Firefox / network
    //c->setRxTimeout(3);
    c->setRxTimeout(0);
    //////

    AsyncWebServerRequest *r = new AsyncWebServerRequest((AsyncWebServer*)s, c);

    if (r == NULL)
    {
      c->close(true);
      c->free();
      delete c;
    }
  }, this);
}

/////////////////////////////////////////////////

AsyncWebServer::~AsyncWebServer()
{
  reset();
  end();

  if (_catchAllHandler)
    delete _catchAllHandler;
}

/////////////////////////////////////////////////

AsyncWebRewrite& AsyncWebServer::addRewrite(AsyncWebRewrite* rewrite)
{
  _rewrites.add(rewrite);

  return *rewrite;
}

/////////////////////////////////////////////////

bool AsyncWebServer::removeRewrite(AsyncWebRewrite *rewrite)
{
  return _rewrites.remove(rewrite);
}

/////////////////////////////////////////////////

AsyncWebRewrite& AsyncWebServer::rewrite(const char* from, const char* to)
{
  return addRewrite(new AsyncWebRewrite(from, to));
}

/////////////////////////////////////////////////

AsyncWebHandler& AsyncWebServer::addHandler(AsyncWebHandler* handler)
{
  _handlers.add(handler);

  return *handler;
}

/////////////////////////////////////////////////

bool AsyncWebServer::removeHandler(AsyncWebHandler *handler)
{
  return _handlers.remove(handler);
}

/////////////////////////////////////////////////

void AsyncWebServer::begin()
{
  _server.setNoDelay(true);
  _server.begin();
}

/////////////////////////////////////////////////

void AsyncWebServer::end()
{
  _server.end();
}

/////////////////////////////////////////////////

#if ASYNC_TCP_SSL_ENABLED
void AsyncWebServer::onSslFileRequest(AcSSlFileHandler cb, void* arg)
{
  _server.onSslFileRequest(cb, arg);
}

/////////////////////////////////////////////////

void AsyncWebServer::beginSecure(const char *cert, const char *key, const char *password)
{
  _server.beginSecure(cert, key, password);
}
#endif

/////////////////////////////////////////////////

void AsyncWebServer::_handleDisconnect(AsyncWebServerRequest *request)
{
  delete request;
}

/////////////////////////////////////////////////

void AsyncWebServer::_rewriteRequest(AsyncWebServerRequest *request)
{
  for (const auto& r : _rewrites)
  {
    if (r->match(request))
    {
      request->_url = r->toUrl();
      request->_addGetParams(r->params());
    }
  }
}

/////////////////////////////////////////////////

void AsyncWebServer::_attachHandler(AsyncWebServerRequest *request)
{
  for (const auto& h : _handlers)
  {
    if (h->filter(request) && h->canHandle(request))
    {
      request->setHandler(h);

      return;
    }
  }

  request->addInterestingHeader("ANY");

  request->setHandler(_catchAllHandler);
}

/////////////////////////////////////////////////

AsyncCallbackWebHandler& AsyncWebServer::on(const char* uri, WebRequestMethodComposite method,
                                            ArRequestHandlerFunction onRequest,
                                            ArUploadHandlerFunction onUpload, ArBodyHandlerFunction onBody)
{
  AsyncCallbackWebHandler* handler = new AsyncCallbackWebHandler();
  
  handler->setUri(uri);
  handler->setMethod(method);
  handler->onRequest(onRequest);
  handler->onUpload(onUpload);
  handler->onBody(onBody);
  addHandler(handler);

  return *handler;
}

/////////////////////////////////////////////////

AsyncCallbackWebHandler& AsyncWebServer::on(const char* uri, WebRequestMethodComposite method,
                                            ArRequestHandlerFunction onRequest,
                                            ArUploadHandlerFunction onUpload)
{
  AsyncCallbackWebHandler* handler = new AsyncCallbackWebHandler();
   
  handler->setUri(uri);
  handler->setMethod(method);
  handler->onRequest(onRequest);
  handler->onUpload(onUpload);
  addHandler(handler);

  return *handler;
}

/////////////////////////////////////////////////

AsyncCallbackWebHandler& AsyncWebServer::on(const char* uri, WebRequestMethodComposite method,
                                            ArRequestHandlerFunction onRequest)
{
  AsyncCallbackWebHandler* handler = new AsyncCallbackWebHandler();
   
  handler->setUri(uri);
  handler->setMethod(method);
  handler->onRequest(onRequest);
  addHandler(handler);

  return *handler;
}

/////////////////////////////////////////////////

AsyncCallbackWebHandler& AsyncWebServer::on(const char* uri, ArRequestHandlerFunction onRequest)
{
  AsyncCallbackWebHandler* handler = new AsyncCallbackWebHandler();
   
  handler->setUri(uri);
  handler->onRequest(onRequest);
  addHandler(handler);

  return *handler;
}

/////////////////////////////////////////////////

// KH Test add serveStatic
AsyncStaticWebHandler& AsyncWebServer::serveStatic(const char* uri, fs::FS& fs, const char* path,
                                                   const char* cache_control)
{
  AsyncStaticWebHandler* handler = new AsyncStaticWebHandler(uri, fs, path, cache_control);
  addHandler(handler);

  return *handler;
}
//////

/////////////////////////////////////////////////

void AsyncWebServer::onNotFound(ArRequestHandlerFunction fn)
{ 
  _catchAllHandler->onRequest(fn);
}

/////////////////////////////////////////////////

void AsyncWebServer::onFileUpload(ArUploadHandlerFunction fn)
{
  _catchAllHandler->onUpload(fn);
}

/////////////////////////////////////////////////

void AsyncWebServer::onRequestBody(ArBodyHandlerFunction fn)
{
  _catchAllHandler->onBody(fn);
}

/////////////////////////////////////////////////

void AsyncWebServer::reset()
{
  _rewrites.free();
  _handlers.free();

  if (_catchAllHandler != NULL)
  {
    _catchAllHandler->onRequest(NULL);
    _catchAllHandler->onUpload(NULL);
    _catchAllHandler->onBody(NULL);
  }
}

/////////////////////////////////////////////////


