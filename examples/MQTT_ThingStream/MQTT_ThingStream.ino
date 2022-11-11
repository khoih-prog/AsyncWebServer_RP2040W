/****************************************************************************************************************************
  MQTT_ThingStream.ino

  For RP2040W with CYW43439 WiFi

  AsyncWebServer_RP2040W is a library for the RP2040W with CYW43439 WiFi

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_RP2040W
  Licensed under GPLv3 license
 *****************************************************************************************************************************/

/*
  Basic MQTT example (without SSL!)
  This sketch demonstrates the basic capabilities of the library.
  It connects to an MQTT server then:
  - publishes {Hello from MQTTClient_SSL on RP2040W} to the topic [RP2040W_Pub]
  - subscribes to the topic [RP2040W_Sub], printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.

  You will need to populate "certificates.h" with your trust anchors
  (see https://github.com/OPEnSLab-OSU/SSLClient/blob/master/TrustAnchors.md)
  and my_cert/my_key with your certificate/private key pair
  (see https://github.com/OPEnSLab-OSU/SSLClient#mtls).
*/

// See the list of country codes in
// https://github.com/earlephilhower/cyw43-driver/blob/02533c10a018c6550e9f66f7699e21356f5e4609/src/cyw43_country.h#L59-L111
// To modify https://github.com/earlephilhower/arduino-pico/blob/master/variants/rpipicow/picow_init.cpp
// Check https://github.com/khoih-prog/AsyncWebServer_RP2040W/issues/3#issuecomment-1255676644

// To remove boolean warnings caused by PubSubClient library
#define boolean     bool

#include "defines.h"

#include <PubSubClient.h>

const char my_cert[]  = "FIXME";
const char my_key[]   = "FIXME";

#define USING_THINGSTREAM_IO      false   //true

#if USING_THINGSTREAM_IO

	const char *MQTT_PREFIX_TOPIC   = "esp32-sniffer/";
	const char *MQTT_ANNOUNCE_TOPIC = "/status";
	const char *MQTT_CONTROL_TOPIC  = "/control";
	const char *MQTT_BLE_TOPIC      = "/ble";


	// GOT FROM ThingsStream!
	const char *MQTT_SERVER     = "mqtt.thingstream.io";
	const char *MQTT_USER       = "MQTT_USER";
	const char *MQTT_PASS       = "MQTT_PASS";
	const char *MQTT_CLIENT_ID  = "MQTT_CLIENT_ID";

	String topic    = MQTT_PREFIX_TOPIC + String("12345678") + MQTT_BLE_TOPIC;
	String subTopic = MQTT_PREFIX_TOPIC + String("12345678") + MQTT_BLE_TOPIC;

#else

	const char* MQTT_SERVER = "broker.emqx.io";           // Broker address

	const char*  ID         = "MQTT_ThingStream";         // Name of our device, must be unique
	String      topic       = "RP2040W_Pub";              // Topic to publish
	String      subTopic    = "RP2040W_Sub";              // Topic to subcribe to

#endif

void mqtt_receive_callback(char* topic, byte* payload, unsigned int length);

const int   MQTT_PORT           = 1883; //if you use SSL //1883 no SSL

unsigned long lastMsg = 0;

// Initialize the SSL client library
// Arguments: WiFiClient, our trust anchors

WiFiClient      wifiClient;
PubSubClient    client(MQTT_SERVER, MQTT_PORT, mqtt_receive_callback, wifiClient);

String data         = "Hello from MQTT_ThingStream on " + String(BOARD_NAME) + " with " + String(SHIELD_TYPE);
const char *pubData = data.c_str();

/*
   Called whenever a payload is received from a subscribed MQTT topic
*/
void mqtt_receive_callback(char* topic, byte* payload, unsigned int length)
{
	Serial.print("MQTT Message receive [");
	Serial.print(topic);
	Serial.print("] ");

	for (unsigned int i = 0; i < length; i++)
	{
		Serial.print((char)payload[i]);
	}

	Serial.println();
}

void reconnect()
{
	// Loop until we're reconnected
	while (!client.connected())
	{
		Serial.print("Attempting MQTT connection to ");
		Serial.println(MQTT_SERVER);

		// Attempt to connect

#if USING_THINGSTREAM_IO
		int connect_status = client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS, topic.c_str(), 2, false, "");
#else
		int connect_status = client.connect(ID);
#endif

		if (connect_status)
		{
			Serial.println("...connected");

			// Once connected, publish an announcement...
			client.publish(topic.c_str(), data.c_str());

			Serial.println("Published connection message successfully!");

			Serial.print("Subcribed to: ");
			Serial.println(subTopic);

			// This is a workaround to address https://github.com/OPEnSLab-OSU/SSLClient/issues/9
			//ethClientSSL.flush();
			// ... and resubscribe
			client.subscribe(subTopic.c_str());
			// for loopback testing
			client.subscribe(topic.c_str());
			// This is a workaround to address https://github.com/OPEnSLab-OSU/SSLClient/issues/9
			//ethClientSSL.flush();
		}
		else
		{
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");

			// Wait 5 seconds before retrying
			delay(5000);
		}
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

	Serial.print("\nStart MQTT_ThingStream on ");
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

	// Note - the default maximum packet size is 256 bytes. If the
	// combined length of clientId, username and password exceed this use the
	// following to increase the buffer size:
	//client.setBufferSize(256);

	Serial.println("***************************************");
	Serial.println(topic);
	Serial.println("***************************************");
}

#define MQTT_PUBLISH_INTERVAL_MS      5000L

void loop()
{
	static unsigned long now;

	if (!client.connected())
	{
		reconnect();
	}

	// Sending Data
	now = millis();

	if (now - lastMsg > MQTT_PUBLISH_INTERVAL_MS)
	{
		lastMsg = now;

		if (!client.publish(topic.c_str(), pubData))
		{
			Serial.println("Message failed to send.");
		}

		Serial.print("MQTT Message Send : " + topic + " => ");
		Serial.println(data);
	}

	client.loop();
}
