
#include <TFT_eSPI.h>
#include <SPI.h>

// ***************** GLOBALS

#define ADC_PIN 34
#define BUTTON_1_PIN 35
#define BUTTON_2_PIN 0
int vref = 1100;

// ***************** TIMER

bool timeStart = false;
volatile int seconds = -4;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
	portENTER_CRITICAL_ISR(&timerMux);
	seconds += 1;
	portEXIT_CRITICAL_ISR(&timerMux);
}

float weight = 0.0;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  espnow_setup();

  screen_init();
  button_init();

	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &onTimer, true);
	timerAlarmWrite(timer, 1000000, true);
}

void loop() {
  espnow_loop();
  
	button_loop();
	screen_loop();

	if(timeStart) timerAlarmEnable(timer);
	else timerAlarmDisable(timer);
}
