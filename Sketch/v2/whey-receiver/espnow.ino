#include <esp_now.h>
#include <WiFi.h>

int32_t tareOffset = 0;
float calFactor = 1.0;
bool autoCalibrate  = false;

typedef struct struct_message {
    int32_t value;
} struct_message;

struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  weight = raw_to_unit(myData.value);
  if (autoCalibrate) autoCalibration();
}

void espnow_setup() {
	// Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  
//  Serial.print("mac: ");
//  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void espnow_loop() {
	
}

// ***************** WEIGHT

void tare() {
	tareOffset = myData.value;
}

void autoCalibration() {
	if(weight >= -0.001 || weight <= 0.001) {
		// auto calibration complete
		autoCalibrate  = false;
		return;
	}
	
	if (weight > 0.0) {
		//increase calfactor
		calFactor = calFactor + 1;
	}
	else {
		//decrease calfactor
		calFactor = calFactor - 1;
	}
}


void calibrate(TFT_eSPI tft) {
	calFactor = 1.0;
	tare();

	Serial.println("Calibrating");
	
  tft.fillScreen(TFT_BLACK);
	tft.setTextFont(1);
	tft.setTextSize(2);
	tft.setCursor(0, 13);
	tft.print("   Calibrating!\n\nPlace 100g object\n");

	for(int i=9; i>=0; i--) {
		tft.print(".");
		Serial.println(i);
		delay(1000);
	}

	calFactor = (myData.value - tareOffset) / 100.0;
	Serial.println("done");
	tft.print("\n\n        Done.");
	delay(1000);
  tft.fillScreen(TFT_BLACK);
}

double raw_to_unit(long raw) {
	return (double)(raw - tareOffset)/(double)calFactor;
}
