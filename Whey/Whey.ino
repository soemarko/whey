#include <U8g2lib.h>
#include <U8x8lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#include <EEPROM.h>
#include "ads1232.h"

#define _dout 19
#define _sclk 23
#define _pdwn 18
ads1232 scale;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int tareButtonPin = 4;
unsigned long tareMillis = 0;
int tareState = HIGH;
int tareLastState = HIGH;

int timerButtonPin = 2;
int timerState = HIGH;
int timerLastState = HIGH;
bool timeStart = LOW;
bool recentlyReset = LOW;
unsigned long timerMillis = 0;

volatile int seconds = -4;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

float prevWeight = 0;
float weight = 0;
byte readCount = 15;
int precision = 1;

int voltagePercentage = 0;
float voltage = 0;

void IRAM_ATTR onTimer() {
	portENTER_CRITICAL_ISR(&timerMux);
	seconds += 1;
	portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
	Serial.begin(115200);
	Serial.println();

	// put your setup code here, to run once:

	pinMode(tareButtonPin, INPUT_PULLUP);
	pinMode(timerButtonPin, INPUT_PULLUP);

	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &onTimer, true);
	timerAlarmWrite(timer, 1000000, true);

	u8g2.begin();

	scale.begin(_dout, _sclk, _pdwn);

	EEPROM.begin(sizeof(float));
	float f = 1;
	EEPROM.get(0, f);
	if(!isnan(f)) {
		scale.set_scale(f);
	}

	delay(100);
	scale.tare(true);
}

void loop() {
	unsigned long currentMillis = millis();

	weight = scale.get_units(readCount);

	float delta = prevWeight - weight; // ignoring values if it's wobbling ±0.09g
	if (delta > -0.1 && delta < 0.1) {
		weight = prevWeight;
	}
	prevWeight = weight;

	float power = pow(10, precision); // round it down to the desired decimal points
	weight = round(weight * power) / power;

//	if (weight >= -0.11 && weight <= 0.11) { // removes wobbles around ±0.1g
//		Serial.println("sini");
//		weight = 0;
//	}

	Serial.print(weight);	// speed test
	Serial.print(",");
	Serial.println(millis() - currentMillis);

//	Serial.print(scale.read_average());
//	Serial.print(",");
//	Serial.print(scale.get_offset());
//	Serial.print(",");
//	Serial.println(scale.get_scale());

	int voltageRead = analogRead(33);
	voltage = round(voltageRead * 3.3 / 4095 * 2 * 10) / 10;
	voltagePercentage = map(voltage*10, 28, 40, 0, 100);

	tareState = digitalRead(tareButtonPin);
	if(tareLastState == LOW && tareState == HIGH) {
			scale.tare();
			Serial.println("tare");
	}
	tareLastState = tareState;

	if(tareState == LOW) {
		if(tareMillis == 0) {
			tareMillis = currentMillis;
		}

		if((currentMillis-tareMillis) >= 3000) {
			tareLastState = tareState = HIGH;
			do_calibration();
//			Serial.println("calibrate");
		}
	}
	else {
		tareMillis = 0;
	}

	timerState = digitalRead(timerButtonPin);
	if(recentlyReset == LOW && timerLastState == LOW && timerState == HIGH) {
		timeStart = !timeStart;
		Serial.print("timerstate: ");
		Serial.println(timeStart);
	}
	timerLastState = timerState;
	
	if(timerState == LOW) {
		if(timerMillis == 0) {
			timerMillis = currentMillis;
		}

		if((currentMillis-timerMillis) >= 3000) {
			Serial.println("reset timer");
			seconds = -4;
			recentlyReset = HIGH;
			timeStart = LOW;
		}
	}
	else {
		timerMillis = 0;
		recentlyReset = LOW;
	}
	
	if(timeStart) timerAlarmEnable(timer);
	else timerAlarmDisable(timer);

	updateScreen();
}

void updateScreen() {
	u8g2.clearBuffer();
	u8g2.setFont(u8g2_font_5x7_mr);
	char buf[100];
	sprintf(buf, "%d%% / %.1fV", voltagePercentage, voltage);
//	int strWidth = u8g2.getStrWidth(buf);
	u8g2.setCursor(0,6);
	u8g2.print(buf);

	int batt = map(voltage*10, 28, 40, 48, 53);
	u8g2.setFont(u8g2_font_battery19_tn);
	u8g2.setFontDirection(1);
	u8g2.drawGlyph(106, 3, batt);

	u8g2.setFontDirection(0);
	u8g2.setFont(u8g2_font_logisoso18_tr); //u8g2_font_luBS14_tr or u8g2_font_luBS12_tr
	u8g2.setCursor(0,40);
	u8g2.print(weight, precision);
	u8g2.print("g");

	u8g2.setFont(u8g2_font_luBS12_tr);
	String s = getTimer();
	u8g2.setCursor(126-(s.length()+1)*9,64);
	u8g2.print(s);
	u8g2.sendBuffer();
}

void do_calibration() {
	// remove previous calibration setting
	scale.set_scale();
	// tare
	scale.tare(true);
	float value_double = scale.read_average(readCount);

	u8g2.clearBuffer();
	u8g2.setFont(u8g2_font_inb16_mf);
	u8g2.setCursor(0,16);
	u8g2.print("Calibrate:");
	u8g2.setFont(u8g2_font_t0_14b_mf);
	u8g2.setCursor(0,32);
	u8g2.print("Place 100g weight...");
	Serial.println("Calibrating");

	for(int i=9; i>=0; i--) {
		u8g2.setCursor(92,52);
		u8g2.print("...");
		u8g2.print(i);
		u8g2.sendBuffer();
		Serial.println(i);
		delay(1000);
	}

	float value_double2 = scale.read_average(readCount);

	// do calibration based on a known weight
	float _s = ceil((value_double2-value_double)/100.f);
	scale.set_scale(_s);

	// read weight
	weight = scale.get_units(readCount);

	u8g2.clearBuffer();
	u8g2.setFont(u8g2_font_inb19_mr);
	u8g2.setCursor(0,40);
	u8g2.print("  DONE!");
	u8g2.sendBuffer();
	Serial.println("done");

	EEPROM.put(0, _s);
	EEPROM.commit();
	delay(1000);
}

String getTimer() {
	if(!timeStart && seconds==-4) {
		return "";
	}
	
	char str[8];
	sprintf(str, "%02d:%02d", seconds/60, seconds%60);
	return String(str);
}
