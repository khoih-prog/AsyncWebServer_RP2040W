/****************************************************************************************************************************
  AsyncWebSynchronization_RP2040W.h
  
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

#ifndef RP2040W_ASYNCWEBSYNCHRONIZATION_H_
#define RP2040W_ASYNCWEBSYNCHRONIZATION_H_

#include <AsyncWebServer_RP2040W.h>

// This is the RP2040W version of the Sync Lock which is currently unimplemented
class AsyncWebLock
{

  public:
    AsyncWebLock()  {}

    ~AsyncWebLock() {}

    bool lock() const 
    {
      return false;
    }

    void unlock() const {}
};

class AsyncWebLockGuard
{
  private:
    const AsyncWebLock *_lock;

  public:
    AsyncWebLockGuard(const AsyncWebLock &l) 
    {
      if (l.lock()) 
      {
        _lock = &l;
      } 
      else 
      {
        _lock = NULL;
      }
    }

    ~AsyncWebLockGuard() 
    {
      if (_lock) 
      {
        _lock->unlock();
      }
    }
};

#endif // RP2040W_ASYNCWEBSYNCHRONIZATION_H_
