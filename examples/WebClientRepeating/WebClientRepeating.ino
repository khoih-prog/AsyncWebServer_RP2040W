/****************************************************************************************************************************
  WebClientRepeating.ino

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

#include "defines.h"

char server[] = "arduino.tips";

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10000L; // delay between updates, in milliseconds

// Initialize the Web client object
WiFiClient client;

// this method makes a HTTP connection to the server
void httpRequest()
{
	Serial.println();

	// close any connection before send a new request
	// this will free the socket on the WiFi shield
	client.stop();

	// if there's a successful connection
	if (client.connect(server, 80))
	{
		Serial.println(F("Connecting..."));

		// send the HTTP PUT request
		client.println(F("GET /asciilogo.txt HTTP/1.1"));
		client.println(F("Host: arduino.tips"));
		client.println(F("Connection: close"));
		client.println();

		// note the time that the connection was made
		lastConnectionTime = millis();
	}
	else
	{
		// if you couldn't make a connection
		Serial.println(F("Connection failed"));
	}
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
	// Open serial communications and wait for port to open:
	Serial.begin(115200);

	while (!Serial && millis() < 5000);

	Serial.print("\nStart WebClientRepeating on ");
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
}

void loop()
{
	// if there's incoming data from the net connection send it out the serial port
	// this is for debugging purposes only
	while (client.available())
	{
		char c = client.read();
		Serial.write(c);
	}

	// if 10 seconds have passed since your last connection,
	// then connect again and send data
	if (millis() - lastConnectionTime > postingInterval)
	{
		httpRequest();
	}
}
