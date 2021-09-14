
#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x4C, 0x11, 0xAE, 0xA5, 0xF3, 0x1C};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    long value;
} struct_message;

// Create a struct_message called myData
struct_message myData;
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  randomSeed(analogRead(0));
}
 
void loop() {
  // Set values to send
  myData.value = random(2147483647);

  // Send message via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(250);
}
