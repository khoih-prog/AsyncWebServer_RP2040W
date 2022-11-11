/****************************************************************************************************************************
  defines.h

  For RP2040W with CYW43439 WiFi

  AsyncWebServer_RP2040W is a library for the RP2040W with CYW43439 WiFi

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_RP2040W
  Licensed under GPLv3 license
 *****************************************************************************************************************************/


#ifndef defines_h
#define defines_h

#if !( defined(ARDUINO_RASPBERRY_PI_PICO_W) )
	#error For RASPBERRY_PI_PICO_W only
#endif

#define _RP2040W_AWS_LOGLEVEL_     1

///////////////////////////////////////////////////////////////////

#include <pico/cyw43_arch.h>

///////////////////////////////////////////////////////////////////

#include <AsyncWebServer_RP2040W.h>

char ssid[] = "your_ssid";        // your network SSID (name)
char pass[] = "12345678";         // your network password (use for WPA, or use as key for WEP), length must be 8+

int status = WL_IDLE_STATUS;

#endif    //defines_h
