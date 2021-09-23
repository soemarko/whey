
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

//#ifndef STASSID
//#define STASSID "your-ssid"
//#define STAPSK  "your-password"
//#endif
#include "wificreds.h" // create wificreds.h and use above as the content with your ssid and password

const char* host = "whey-sender";
const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void ota_setup(void) {

  Serial.println();
  Serial.println("Booting Sketch...");

  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }

  MDNS.begin(host);

  httpUpdater.setup(&httpServer);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
}

void ota_loop(void) {
  httpServer.handleClient();
  MDNS.update();
}
