#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "config.h"
#include "ve.direct.h"

WiFiClient client;

void setup() {
	CONF_PHOENIX_SERIAL.begin(CONF_PHOENIX_BAUT);
	delay(200);

	#ifdef CONF_WIFI_PASS
	WiFi.begin(CONF_WIFI_SSID, CONF_WIFI_PASS);
	#else
	WiFi.begin(CONF_WIFI_SSID);
	#endif

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
	}

	if (!client.connect(CONF_COLLECTD_SERVER_HOST, CONF_COLLECTD_SERVER_PORT)) {
   		Serial.println("Connection to host failed");
   		delay(1000);
   		return;
	}
}

void loop() {
	uint16_t addresses[] {
		0x0100, // Product Id
		0x0101, // Hardware version
		0x0102, // Software version
    		0x0104, // Group Id
		0x0105, // Device instance
    		0x0106, // Device class
		0x010A, // Serial number
		0x010B, // Model name
		0x0140, // Capabilities
		0x0200, // Device mode
		0x0201, // Device state
		0x0202, // Remote control used
		0x0205, // Device off reason (*1)
		0x2203, // VE_REG_AC_OUT_RATED_POWER
		0x2202, // VE_REG_AC_OUT_NOM_VOLTAGE
		0xED8D, // BATTERY VOLTAGE
		0x2201,
		0x2200,
		0xEDFF, // Batterysafe mode (*1)
		0xEDFD, // Automatic equalization mode (*2)
		0xEDFC, // Battery bulk time limit
		0xEDFB, // Battery absorption time limit
		0xEDF7, // Battery absorption voltage (*5)
		0xEDF6, // Battery float voltage (*5)
		0xEDF4, // Battery equalization voltage (*3,*5)
		0xEDF2, // Battery temp. compensation (*5)
		0xEDF1, // Battery type
		0xEDF0, // Battery maximum current
		0xEDE0, // Battery low temperature level (*8)
		0xEDEF, // Battery voltage (*4)
		0xEDEC, // Battery temperature (*8)
		0xEDEA, // Battery voltage setting (*4, *7)
		0xEDE8, // BMS present (*6)
		0xEDE6, // Low temperature charge current (*8)
		0xEDDF, // Charger maximum current (*1)
		0xEDDD, // System yield (*2)
		0xEDDC, // User yield (resettable) (*2)
		0xEDDB, // Charger internal temperature
		0xEDDA, // Charger error code
		0xEDD7, // Charger current (*3)
		0xEDD5, // Charger voltage (*3)
		0xEDD4, // Additional charger state info
		0xEDD3, // Yield today (*2)
		0xEDD2, // Maximum power today (*2)
		0xEDD1, // Yield yesterday (*2)
		0xEDD0, // Maximum power yesterday (*2)
		0xEDCE, // Voltage settings range (*5)
		0xEDCD, // History version (*1)
		0xEDCC, // Streetlight version (*1)
		0x2211, // Adjustable voltage minimum (*6)
		0x2212, // Adjustable voltage maximum (*6)
		0xEDBC, // Panel power
		0xEDBB, // Panel voltage
		0xEDBD, // Panel current (*1)
		0xEDB8, // Panel maximum voltage (*2)
		0xEDAD, // Load current
		0xEDAC, // Load offset voltage
		0xEDAB, // Load output control
		0xEDA9, // Load output voltage (*1)
		0xEDA8, // Load output state
		0xED9D, // Load switch high level (*2)
		0xED9C, // Load switch low level (*2)
		0xED91, // Load output off reason (*3)
		0xEDD9, // Relay operation mode
		0x0350, // Relay battery low voltage set
		0x0351, // Relay battery low voltage clear
		0x0352, // Relay battery high voltage set
		0x0353, // Relay battery high voltage clear
		0xEDBA, // Relay panel high voltage set
		0xEDB9, // Relay panel high voltage clear
		0x100A, // Relay minimum enabled time
	};
	while (true) {
		for (int i = 0; i < (sizeof(addresses) / sizeof(uint16_t)); i +=5) {
			String input;
			for (int j = i; j < i+5; j++){
				char *req = ve::get(addresses[j]);
				Serial.print(req);
			}
			#ifdef CONF_SCRAPE_INTERVAL
			delay(CONF_SCRAPE_INTERVAL);
			#endif // CONF_SCRAPE_INTERVAL
			while (CONF_PHOENIX_SERIAL.available()) {
				client.print(CONF_PHOENIX_NAME);
				client.println(CONF_PHOENIX_SERIAL.readStringUntil('\n'));
			}
		}
	}
}
