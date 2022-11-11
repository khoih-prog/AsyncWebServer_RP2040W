/****************************************************************************************************************************
  Async_WebSocketsServer_Xtreme.ino

  For RP2040W with CYW43439 WiFi

  AsyncWebServer_RP2040W is a library for the RP2040W with CYW43439 WiFi

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_RP2040W
  Licensed under GPLv3 license
 *****************************************************************************************************************************/

// Modified from code of https://github.com/drmue
// Posted in https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues/6

#if !( defined(ARDUINO_RASPBERRY_PI_PICO_W) )
	#error For RASPBERRY_PI_PICO_W only
#endif

#define _RP2040W_AWS_LOGLEVEL_       4

#include <AsyncWebServer_RP2040W.h>

#include "webpage.h"

char ssid[] = "your_ssid";        // your network SSID (name)
char pass[] = "12345678";         // your network password (use for WPA, or use as key for WEP), length must be 8+

int status = WL_IDLE_STATUS;

String JSONtxt;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

AsyncWebSocketClient * globalClient = NULL;

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
{
	if (type == WS_EVT_CONNECT)
	{
		Serial.printf("ws[Server: %s][ClientID: %u] WSClient connected\n", server->url(), client->id());

		globalClient = client;
		//client->text("Hello from RP2040W Server");
	}
	else if (type == WS_EVT_DISCONNECT)
	{
		Serial.printf("ws[Server: %s][ClientID: %u] WSClient disconnected\n", server->url(), client->id());

		globalClient = NULL;
	}
	else if (type == WS_EVT_ERROR)
	{
		//error was received from the other end
		Serial.printf("ws[Server: %s][ClientID: %u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
	}
	else if (type == WS_EVT_PONG)
	{
		//pong message was received (in response to a ping request maybe)
		Serial.printf("ws[Server: %s][ClientID: %u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char*)data : "");
	}
	else if (type == WS_EVT_DATA)
	{
		//data packet
		AwsFrameInfo * info = (AwsFrameInfo*)arg;

		if (info->final && info->index == 0 && info->len == len)
		{
			//the whole message is in a single frame and we got all of it's data
			Serial.printf("ws[Server: %s][ClientID: %u] %s-message[len: %llu]: ", server->url(), client->id(),
			              (info->opcode == WS_TEXT) ? "text" : "binary", info->len);

			if (info->opcode == WS_TEXT)
			{
				data[len] = 0;
				Serial.printf("%s\n", (char*)data);
			}
			else
			{
				for (size_t i = 0; i < info->len; i++)
				{
					Serial.printf("%02x ", data[i]);
				}

				Serial.printf("\n");
			}

			if (info->opcode == WS_TEXT)
				client->text("Got your text message");
			else
				client->binary("Got your binary message");
		}
		else
		{
			//message is comprised of multiple frames or the frame is split into multiple packets
			if (info->index == 0)
			{
				if (info->num == 0)
				{
					Serial.printf("ws[Server: %s][ClientID: %u] %s-message start\n", server->url(), client->id(),
					              (info->message_opcode == WS_TEXT) ? "text" : "binary");
				}

				Serial.printf("ws[Server: %s][ClientID: %u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
			}

			Serial.printf("ws[Server: %s][ClientID: %u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(),
			              info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);

			if (info->message_opcode == WS_TEXT)
			{
				data[len] = 0;
				Serial.printf("%s\n", (char*)data);
			}
			else
			{
				for (size_t i = 0; i < len; i++)
				{
					Serial.printf("%02x ", data[i]);
				}

				Serial.printf("\n");
			}

			if ((info->index + len) == info->len)
			{
				Serial.printf("ws[Server: %s][ClientID: %u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);

				if (info->final)
				{
					Serial.printf("ws[Server: %s][ClientID: %u] %s-message end\n", server->url(), client->id(),
					              (info->message_opcode == WS_TEXT) ? "text" : "binary");

					if (info->message_opcode == WS_TEXT)
						client->text("I got your text message");
					else
						client->binary("I got your binary message");
				}
			}
		}
	}
}

void handleRoot(AsyncWebServerRequest *request)
{
	request->send(200, "text/html", webpageCont);
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
}

void setup()
{
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

	delay(200);

	Serial.print("\nStarting Async_WebSocketsServer_Xtreme on ");
	Serial.println(BOARD_NAME);
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

	ws.onEvent(onEvent);
	server.addHandler(&ws);

	server.on("/", handleRoot);
	server.begin();
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

void sendStatus()
{
	static unsigned long sendStatus_timeout  = 1000;
	static unsigned long checkstatus_timeout = 1000;

#define STATUS_CHECK_INTERVAL     10000L

#define SEND_INTERVAL             30L

	// Send status report every SEND_INTERVAL (30) millis
	if (millis() > sendStatus_timeout)
	{
		//if (globalClient != NULL && globalClient->status() == WS_CONNECTED)
		// Sending only when gessage_queue not full
		if (globalClient != NULL && globalClient->status() == WS_CONNECTED && globalClient->canSend())
		{
			String POTvalString = String(rand() % 256);

			JSONtxt = "{\"POT\":\"" + POTvalString + "\"}";

			globalClient->text(JSONtxt);
		}

		sendStatus_timeout = millis() + SEND_INTERVAL;
	}

	// Send status report every STATUS_REPORT_INTERVAL (10) seconds
	if (millis() > checkstatus_timeout)
	{
		heartBeatPrint();
		checkstatus_timeout = millis() + STATUS_CHECK_INTERVAL;
	}
}

void loop()
{
	sendStatus();
}
