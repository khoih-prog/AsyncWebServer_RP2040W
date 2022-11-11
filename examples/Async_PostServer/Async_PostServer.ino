/****************************************************************************************************************************
  Async_PostServer.ino

  For RP2040W with CYW43439 WiFi

  AsyncWebServer_RP2040W is a library for the RP2040W with CYW43439 WiFi

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_RP2040W
  Licensed under GPLv3 license
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

#include <AsyncWebServer_RP2040W.h>

char ssid[] = "your_ssid";        // your network SSID (name)
char pass[] = "12345678";         // your network password (use for WPA, or use as key for WEP), length must be 8+

int status = WL_IDLE_STATUS;

AsyncWebServer    server(80);

const String postForms =
  "<html>\
<head>\
<title>AsyncWebServer POST handling</title>\
<style>\
body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
</style>\
</head>\
<body>\
<h1>POST plain text to /postplain/</h1><br>\
<form method=\"post\" enctype=\"text/plain\" action=\"/postplain/\">\
<input type=\"text\" name=\'{\"hello\": \"world\", \"trash\": \"\' value=\'\"}\'><br>\
<input type=\"submit\" value=\"Submit\">\
</form>\
<h1>POST form data to /postform/</h1><br>\
<form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/postform/\">\
<input type=\"text\" name=\"hello\" value=\"world\"><br>\
<input type=\"submit\" value=\"Submit\">\
</form>\
</body>\
</html>";

void handleRoot(AsyncWebServerRequest *request)
{
	request->send(200, "text/html", postForms);
}

void handlePlain(AsyncWebServerRequest *request)
{
	if (request->method() != HTTP_POST)
	{
		request->send(405, "text/plain", "Method Not Allowed");
	}
	else
	{
		request->send(200, "text/plain", "POST body was:\n" + request->arg("plain"));
	}
}

void handleForm(AsyncWebServerRequest *request)
{
	if (request->method() != HTTP_POST)
	{
		request->send(405, "text/plain", "Method Not Allowed");
	}
	else
	{
		String message = "POST form was:\n";

		for (uint8_t i = 0; i < request->args(); i++)
		{
			message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
		}

		request->send(200, "text/plain", message);
	}
}

void handleNotFound(AsyncWebServerRequest *request)
{
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
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

	delay(200);

	Serial.print("\nStart Async_PostServer on ");
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

	//server.on("/postplain/", handlePlain);
	server.on("/postplain/", HTTP_POST, [](AsyncWebServerRequest * request)
	{
		handlePlain(request);
	});

	//server.on("/postform/", handleForm);
	server.on("/postform/", HTTP_POST, [](AsyncWebServerRequest * request)
	{
		handleForm(request);
	});

	server.onNotFound(handleNotFound);

	server.begin();

	Serial.print(F("HTTP Async_PostServer started @ IP : "));
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

#define STATUS_CHECK_INTERVAL     10000L

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
