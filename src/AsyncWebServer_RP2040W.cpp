/****************************************************************************************************************************
  AsyncWebServer_RP2040W.cpp
  
  For RP2040W with CYW43439 WiFi
  
  AsyncWebServer_RP2040W is a library for the RP2040W with CYW43439 WiFi
  
  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_RP2040W
  Licensed under GPLv3 license
 
  Version: 1.0.3
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      13/08/2022 Initial coding for RP2040W with CYW43439 WiFi
  1.0.1   K Hoang      15/08/2022 Fix bug in examples, `library.json`
  1.0.2   K Hoang      15/08/2022 Fix LED bug in examples
  1.0.3   K Hoang      22/09/2022 To display country-code and tempo method to modify in arduino-pico core
 *****************************************************************************************************************************/

#if !defined(_RP2040W_AWS_LOGLEVEL_)
  #define _RP2040W_AWS_LOGLEVEL_     1
#endif

#include "AsyncWebServer_RP2040W_Debug.h"

#include "AsyncWebServer_RP2040W.h"
#include "AsyncWebHandlerImpl_RP2040W.h"

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
    return;

  _server.onClient([](void *s, AsyncClient * c)
  {
    if (c == NULL)
      return;

    c->setRxTimeout(3);
    AsyncWebServerRequest *r = new AsyncWebServerRequest((AsyncWebServer*)s, c);

    if (r == NULL)
    {
      c->close(true);
      c->free();
      delete c;
    }
  }, this);
}

AsyncWebServer::~AsyncWebServer()
{
  reset();
  end();

  if (_catchAllHandler)
    delete _catchAllHandler;
}

AsyncWebRewrite& AsyncWebServer::addRewrite(AsyncWebRewrite* rewrite)
{
  _rewrites.add(rewrite);

  return *rewrite;
}

bool AsyncWebServer::removeRewrite(AsyncWebRewrite *rewrite)
{
  return _rewrites.remove(rewrite);
}

AsyncWebRewrite& AsyncWebServer::rewrite(const char* from, const char* to)
{
  return addRewrite(new AsyncWebRewrite(from, to));
}

AsyncWebHandler& AsyncWebServer::addHandler(AsyncWebHandler* handler)
{
  _handlers.add(handler);
  return *handler;
}

bool AsyncWebServer::removeHandler(AsyncWebHandler *handler)
{
  return _handlers.remove(handler);
}

void AsyncWebServer::begin()
{
  _server.setNoDelay(true);
  _server.begin();
}

void AsyncWebServer::end()
{
  _server.end();
}

#if ASYNC_TCP_SSL_ENABLED
void AsyncWebServer::onSslFileRequest(AcSSlFileHandler cb, void* arg)
{
  _server.onSslFileRequest(cb, arg);
}

void AsyncWebServer::beginSecure(const char *cert, const char *key, const char *password)
{
  _server.beginSecure(cert, key, password);
}
#endif

void AsyncWebServer::_handleDisconnect(AsyncWebServerRequest *request)
{ 
  delete request;
}

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
  
  // KH, fix crashing issue from v1.2.0, for Arduino-mbed core v2.6.1
  //request->setHandler(_catchAllHandler);
  request->setHandler(NULL);
}


AsyncCallbackWebHandler& AsyncWebServer::on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest,
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

AsyncCallbackWebHandler& AsyncWebServer::on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest, ArUploadHandlerFunction onUpload)
{
  AsyncCallbackWebHandler* handler = new AsyncCallbackWebHandler();
  handler->setUri(uri);
  handler->setMethod(method);
  handler->onRequest(onRequest);
  handler->onUpload(onUpload);
  addHandler(handler);

  return *handler;
}

AsyncCallbackWebHandler& AsyncWebServer::on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction onRequest)
{
  AsyncCallbackWebHandler* handler = new AsyncCallbackWebHandler();
  handler->setUri(uri);
  handler->setMethod(method);
  handler->onRequest(onRequest);
  addHandler(handler);

  return *handler;
}

AsyncCallbackWebHandler& AsyncWebServer::on(const char* uri, ArRequestHandlerFunction onRequest)
{
  AsyncCallbackWebHandler* handler = new AsyncCallbackWebHandler();
  handler->setUri(uri);
  handler->onRequest(onRequest);
  addHandler(handler);

  return *handler;
}

void AsyncWebServer::onNotFound(ArRequestHandlerFunction fn)
{
  _catchAllHandler->onRequest(fn);
}

void AsyncWebServer::onRequestBody(ArBodyHandlerFunction fn)
{
  _catchAllHandler->onBody(fn);
}

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


