#include "esp_adc_cal.h"
#include <esp_now.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Button2.h>

#define ADC_PIN 34
#define BUTTON_1 35
#define BUTTON_2 0

int vref = 1100;

TFT_eSPI tft = TFT_eSPI(135, 240); 

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    long value;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("value: ");
  Serial.println(myData.value);
  Serial.println();
}

Button2 btn1(BUTTON_1);
Button2 btn2(BUTTON_2);

bool timeStart = false;
volatile int seconds = -4;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
	portENTER_CRITICAL_ISR(&timerMux);
	seconds += 1;
	portEXIT_CRITICAL_ISR(&timerMux);
}

float getVoltage() {
    uint16_t v = analogRead(ADC_PIN);
    return ((float) v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
}

void button_init() {
	btn1.setPressedHandler([](Button2 & b) {
		Serial.println("tare");
//		showVoltage();
//		tare();
	});

	btn1.setLongClickHandler([](Button2 & b) {
		Serial.println("start calibration");
	});
	
	btn2.setPressedHandler([](Button2 & b) {
		Serial.println("start / stop timer");
		timeStart = !timeStart;
	});

	btn2.setLongClickHandler([](Button2 & b) {
		Serial.println("reset timer");
		seconds = -4;
		timeStart = LOW;
	});
}

void button_loop() {
    btn1.loop();
    btn2.loop();
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
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
  
  tft.init();
  tft.setRotation(1);
//  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(7);
  tft.setTextWrap(true);
  

  button_init();

	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &onTimer, true);
	timerAlarmWrite(timer, 1000000, true);

  esp_adc_cal_characteristics_t adc_chars;
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize((adc_unit_t) ADC_UNIT_1, 
  (adc_atten_t) ADC1_CHANNEL_6, (adc_bits_width_t) ADC_WIDTH_BIT_12, 1100, & adc_chars);

  //Check type of calibration value used to characterize ADC
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    Serial.print("eFuse Vref:");
    Serial.println(adc_chars.vref);
    vref = adc_chars.vref;
  }
  else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
    Serial.printf("Two Point --> coeff_a:%umV coeff_b:%umV\n", adc_chars.coeff_a, adc_chars.coeff_b);
  }
  else {
    Serial.println("Default Vref: 1100mV");
  }
}

void loop() {
	button_loop();
	screen_loop();
}

void screen_loop() {
	char buff[64];
	sprintf(buff, "%lu", myData.value);
	tft.setCursor(0,0);
	tft.print(buff);
	
}
