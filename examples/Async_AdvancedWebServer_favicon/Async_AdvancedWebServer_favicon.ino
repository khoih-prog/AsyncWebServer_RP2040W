/****************************************************************************************************************************
  Async_AdvancedWebServer.ino

  For RP2040W with CYW43439 WiFi

  AsyncWebServer_RP2040W is a library for the RP2040W with CYW43439 WiFi

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_RP2040W
  Licensed under GPLv3 license

  Copyright (c) 2015, Majenko Technologies
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

  Neither the name of Majenko Technologies nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************************************************************/

// See the list of country codes in
// https://github.com/earlephilhower/cyw43-driver/blob/02533c10a018c6550e9f66f7699e21356f5e4609/src/cyw43_country.h#L59-L111
// To modify https://github.com/earlephilhower/arduino-pico/blob/master/variants/rpipicow/picow_init.cpp
// Check https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues/3#issuecomment-1255676644

#if !( defined(ARDUINO_RASPBERRY_PI_PICO_W) )
	#error For RASPBERRY_PI_PICO_W only
#endif

#define _RP2040W_AWS_LOGLEVEL_     1

///////////////////////////////////////////////////////////////////

#include <pico/cyw43_arch.h>

///////////////////////////////////////////////////////////////////

#include "favicon.h"
#include <AsyncWebServer_RP2040W.h>

char ssid[] = "your_ssid";        // your network SSID (name)
char pass[] = "12345678";         // your network password (use for WPA, or use as key for WEP), length must be 8+

int status = WL_IDLE_STATUS;

AsyncWebServer    server(80);

int reqCount = 0;                // number of requests received

#define LED_OFF             LOW
#define LED_ON              HIGH

#define BUFFER_SIZE         512
char temp[BUFFER_SIZE];

void handleRoot(AsyncWebServerRequest *request)
{
	digitalWrite(LED_BUILTIN, LED_ON);

	int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;
	int day = hr / 24;

	snprintf(temp, BUFFER_SIZE - 1,
	         "<html>\
<head>\
<meta http-equiv='refresh' content='60'/>\
<title>AsyncWebServer-%s</title>\
<style>\
body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
</style>\
</head>\
<body>\
<h2>AsyncWebServer_RP2040W!</h2>\
<h3>running WiFi on %s</h3>\
<p>Uptime: %d d %02d:%02d:%02d</p>\
<img src=\"/test.svg\" />\
</body>\
</html>", BOARD_NAME, BOARD_NAME, day, hr % 24, min % 60, sec % 60);

	request->send(200, "text/html", temp);

	digitalWrite(LED_BUILTIN, LED_OFF);
}

void drawFavicon(AsyncWebServerRequest *request)
{
	AsyncWebServerResponse *response = request->beginResponse(200, "image/x-icon", favicon_ico_gz, favicon_ico_gz_len);

	response->addHeader("Content-Encoding", "gzip");
	request->send(response);
}

void handleNotFound(AsyncWebServerRequest *request)
{
	digitalWrite(LED_BUILTIN, LED_ON);
	String message = "File Not Found\n\n";

	message += "URI: ";
	message += request->url();
	message += "\nMethod: ";
	message += (request->method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += request->args();
	message += "\n";

	for (uint8_t i = 0; i < request->args(); i++)
	{
		message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
	}

	request->send(404, "text/plain", message);
	digitalWrite(LED_BUILTIN, LED_OFF);
}

void drawGraph(AsyncWebServerRequest *request)
{
	String out;

	out.reserve(4000);
	char temp[70];

	digitalWrite(LED_BUILTIN, LED_ON);

	out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"310\" height=\"150\">\n";
	out += "<rect width=\"310\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"2\" stroke=\"rgb(0, 0, 0)\" />\n";
	out += "<g stroke=\"blue\">\n";
	int y = rand() % 130;

	for (int x = 10; x < 300; x += 10)
	{
		int y2 = rand() % 130;
		sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"2\" />\n", x, 140 - y, x + 10, 140 - y2);
		out += temp;
		y = y2;
	}

	out += "</g>\n</svg>\n";

	request->send(200, "image/svg+xml", out);

	digitalWrite(LED_BUILTIN, LED_OFF);
}

void printWifiStatus()
{
	// print the SSID of the network you're attached to:
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());

	// print your board's IP address:
	IPAddress ip = WiFi.localIP();
	Serial.print("Local IP Address: ");
	Serial.println(ip);

	// print your board's country code
	// #define CYW43_COUNTRY(A, B, REV) ((unsigned char)(A) | ((unsigned char)(B) << 8) | ((REV) << 16))
	uint32_t myCountryCode = cyw43_arch_get_country_code();
	char countryCode[3] = { 0, 0, 0 };

	countryCode[0] = myCountryCode & 0xFF;
	countryCode[1] = (myCountryCode >> 8) & 0xFF;

	Serial.print("Country code: ");
	Serial.println(countryCode);
}

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LED_OFF);

	Serial.begin(115200);

	while (!Serial && millis() < 5000);

	delay(200);

	Serial.print("\nStart Async_AdvancedWebServer_favicon on ");
	Serial.print(BOARD_NAME);
	Serial.print(" with ");
	Serial.println(SHIELD_TYPE);
	Serial.println(ASYNCTCP_RP2040W_VERSION);
	Serial.println(ASYNC_WEBSERVER_RP2040W_VERSION);

	///////////////////////////////////

	// check for the WiFi module:
	if (WiFi.status() == WL_NO_MODULE)
	{
		Serial.println("Communication with WiFi module failed!");

		// don't continue
		while (true);
	}

	Serial.print(F("Connecting to SSID: "));
	Serial.println(ssid);

	status = WiFi.begin(ssid, pass);

	delay(1000);

	// attempt to connect to WiFi network
	while ( status != WL_CONNECTED)
	{
		delay(500);

		// Connect to WPA/WPA2 network
		status = WiFi.status();
	}

	printWifiStatus();

	///////////////////////////////////


	server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
	{
		handleRoot(request);
	});

	server.on("/test.svg", HTTP_GET, [](AsyncWebServerRequest * request)
	{
		drawGraph(request);
	});

	server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest * request)
	{
		drawFavicon(request);
	});

	server.on("/inline", [](AsyncWebServerRequest * request)
	{
		request->send(200, "text/plain", "This works as well");
	});

	server.onNotFound(handleNotFound);

	server.begin();

	Serial.print(F("HTTP EthernetWebServer is @ IP : "));
	Serial.println(WiFi.localIP());
}

void heartBeatPrint()
{
	static int num = 1;

	Serial.print(F("."));

	if (num == 80)
	{
		Serial.println();
		num = 1;
	}
	else if (num++ % 10 == 0)
	{
		Serial.print(F(" "));
	}
}

void check_status()
{
	static unsigned long checkstatus_timeout = 0;

#define STATUS_CHECK_INTERVAL     60000L

	// Send status report every STATUS_REPORT_INTERVAL (60) seconds: we don't need to send updates frequently if there is no status change.
	if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
	{
		heartBeatPrint();
		checkstatus_timeout = millis() + STATUS_CHECK_INTERVAL;
	}
}

void loop()
{
	check_status();
}
