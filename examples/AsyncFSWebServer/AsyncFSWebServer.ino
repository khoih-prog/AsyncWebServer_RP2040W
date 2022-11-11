/****************************************************************************************************************************
  AsyncFSWebServer.ino

  For RP2040W with CYW43439 WiFi

  AsyncWebServer_RP2040W is a library for the RP2040W with CYW43439 WiFi

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_RP2040W
  Licensed under GPLv3 license
 *****************************************************************************************************************************/

/*****************************************************************************************************************************
   How To Use:
   1) Upload the contents of the data folder with MkSPIFFS Tool ("ESP8266 Sketch Data Upload" in Tools menu in Arduino IDE)
   2) or you can upload the contents of a folder if you CD in that folder and run the following command:
      for file in `\ls -A1`; do curl -F "file=@$PWD/$file" localIPAddress/edit; done
   3) access the sample web page at http://localIPAddress/
*****************************************************************************************************************************/

#if !( defined(ARDUINO_RASPBERRY_PI_PICO_W) )
	#error For RASPBERRY_PI_PICO_W only
#endif

#define _RP2040W_AWS_LOGLEVEL_     4

///////////////////////////////////////////////////////////////////

#include <pico/cyw43_arch.h>

///////////////////////////////////////////////////////////////////

#include <LittleFS.h>

#include <AsyncFSEditor_RP2040W.h>

#include <AsyncWebServer_RP2040W.h>

char ssid[] = "your_ssid";        // your network SSID (name)
char pass[] = "12345678";         // your network password (use for WPA, or use as key for WEP), length must be 8+

int status = WL_IDLE_STATUS;

// SKETCH BEGIN
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncEventSource events("/events");

const char* hostName      = "rp2040w-async";
const char* http_username = "admin";
const char* http_password = "admin";

///////////////////////////////////////////////////

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
{
	if (type == WS_EVT_CONNECT)
	{
		Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
		client->printf("Hello Client %ld :)", client->id());
		client->ping();
	}
	else if (type == WS_EVT_DISCONNECT)
	{
		Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
	}
	else if (type == WS_EVT_ERROR)
	{
		Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
	}
	else if (type == WS_EVT_PONG)
	{
		Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char*)data : "");
	}
	else if (type == WS_EVT_DATA)
	{
		AwsFrameInfo * info = (AwsFrameInfo*)arg;
		String msg = "";

		if (info->final && info->index == 0 && info->len == len)
		{
			//the whole message is in a single frame and we got all of it's data
			Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);

			if (info->opcode == WS_TEXT)
			{
				for (size_t i = 0; i < info->len; i++)
				{
					msg += (char) data[i];
				}
			}
			else
			{
				char buff[3];

				for (size_t i = 0; i < info->len; i++)
				{
					sprintf(buff, "%02x ", (uint8_t) data[i]);
					msg += buff ;
				}
			}

			Serial.printf("%s\n", msg.c_str());

			if (info->opcode == WS_TEXT)
				client->text("I got your text message");
			else
				client->binary("I got your binary message");
		}
		else
		{
			//message is comprised of multiple frames or the frame is split into multiple packets
			if (info->index == 0)
			{
				if (info->num == 0)
					Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");

				Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
			}

			Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);

			if (info->opcode == WS_TEXT)
			{
				for (size_t i = 0; i < len; i++)
				{
					msg += (char) data[i];
				}
			}
			else
			{
				char buff[3];

				for (size_t i = 0; i < len; i++)
				{
					sprintf(buff, "%02x ", (uint8_t) data[i]);
					msg += buff ;
				}
			}

			Serial.printf("%s\n", msg.c_str());

			if ((info->index + len) == info->len)
			{
				Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);

				if (info->final)
				{
					Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");

					if (info->message_opcode == WS_TEXT)
						client->text("I got your text message");
					else
						client->binary("I got your binary message");
				}
			}
		}
	}
}

////////////////////////////////////////////////////

void initFS()
{
	// Initialize LittleFS/SPIFFS file-system
	if (!LittleFS.begin())
	{
		LittleFS.format();

		if (!LittleFS.begin())
		{
			while (true)
			{
				Serial.println(F("LittleFS failed!"));

				// Stay forever here as useless to go further
				delay(5000);
			}
		}
	}
}

///////////////////////////////////////////////////

void initWebServer()
{
	ws.onEvent(onWsEvent);
	server.addHandler(&ws);

	events.onConnect([](AsyncEventSourceClient * client)
	{
		client->send("hello!", NULL, millis(), 1000);
	});

	server.addHandler(&events);

	server.addHandler(new AsyncFSEditor(http_username, http_password));

	server.serveStatic("/", LittleFS, "/").setDefaultFile("index.htm");

	server.onNotFound([](AsyncWebServerRequest * request)
	{
		Serial.printf("NOT_FOUND: ");

		if (request->method() == HTTP_GET)
			Serial.printf("GET");
		else if (request->method() == HTTP_POST)
			Serial.printf("POST");
		else if (request->method() == HTTP_DELETE)
			Serial.printf("DELETE");
		else if (request->method() == HTTP_PUT)
			Serial.printf("PUT");
		else if (request->method() == HTTP_PATCH)
			Serial.printf("PATCH");
		else if (request->method() == HTTP_HEAD)
			Serial.printf("HEAD");
		else if (request->method() == HTTP_OPTIONS)
			Serial.printf("OPTIONS");
		else
			Serial.printf("UNKNOWN");

		Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

		if (request->contentLength())
		{
			Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
			Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
		}

		int headers = request->headers();
		int i;

		for (i = 0; i < headers; i++)
		{
			AsyncWebHeader* h = request->getHeader(i);
			Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
		}

		int params = request->params();

		for (i = 0; i < params; i++)
		{
			AsyncWebParameter* p = request->getParam(i);

			if (p->isFile())
			{
				Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
			}
			else if (p->isPost())
			{
				Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
			}
			else
			{
				Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
			}
		}

		request->send(404);
	});

	server.onFileUpload([](AsyncWebServerRequest * request, const String & filename, size_t index, uint8_t *data, size_t len, bool final)
	{
		if (!index)
			Serial.printf("UploadStart: %s\n", filename.c_str());

		Serial.printf("%s", (const char*)data);

		if (final)
			Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index + len);
	});

	server.onRequestBody([](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total)
	{
		if (!index)
			Serial.printf("BodyStart: %u\n", total);

		Serial.printf("%s", (const char*)data);

		if (index + len == total)
			Serial.printf("BodyEnd: %u\n", total);
	});

	server.begin();
}

///////////////////////////////////////////////////

void printWifiStatus()
{
	// print the SSID of the network you're attached to:
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());

	// print your board's IP address:
	IPAddress ip = WiFi.localIP();
	Serial.print("Local IP Address: ");
	Serial.println(ip);
}

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

	delay(200);

	Serial.print("\nStart AsyncFSWebServer on ");
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

	initFS();

	initWebServer();

	Serial.print("AsyncWebServer started @");
	Serial.println(WiFi.localIP());

	Serial.print(F("Open http://"));
	Serial.print(WiFi.localIP());
	Serial.println(F("/edit to see the file browser"));
}

void loop()
{
	ws.cleanupClients();
}
