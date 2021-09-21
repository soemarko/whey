
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "ADS1232.h"

// ***************** ESP Now

uint8_t broadcastAddress[] = {0x4C, 0x11, 0xAE, 0xA5, 0xE5, 0x34};

typedef struct struct_message {
    int32_t value;
} struct_message;

struct_message myData;

// ***************** Scale

#define _samples 10

#define _dout_pin 13  // P7 (pins on Oak Digistump silkscreen)
#define _sclk_pin 14  // P9
#define _pdwn_pin 12  // P8
#define _speed_pin 15 // P6
#define _gain0_pin 4  // P5
#define _gain1_pin 3  // P3
#define _a0_pin 0
#define _temp_pin 0

ADS1232 scale = ADS1232(_pdwn_pin, _sclk_pin, _dout_pin, _a0_pin, _speed_pin, _gain1_pin, _gain0_pin, _temp_pin);

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_AP_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  ota_setup();
  
  scale.begin();
  delay(100);

//  randomSeed(analogRead(0));
}
 
void loop() {
  // Set values to send
//  myData.value = random(2147483647);
	myData.value = scale.readRaw(_samples);

  // Send message via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  Serial.println(scale.readUnits(_samples));

	ota_loop();
//  delay(250);
}

/* Calibrate offset. For testing without screen. */

void calibrate() {
	scale.calFactor = 1.0;

	int32_t offset = scale.readRaw(_samples);

	Serial.println("Calibrating");

	for(int i=9; i>=0; i--) {
		Serial.println(i);
		delay(1000);
	}

	scale.calFactor = (scale.readRaw(_samples) - offset) / 100.0;
	Serial.println("done");
	delay(1000);
}
