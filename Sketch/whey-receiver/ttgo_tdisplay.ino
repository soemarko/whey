#include "esp_adc_cal.h"
#include <Button2.h>

// ***************** GLOBALS

#define ADC_PIN 34
#define BUTTON_1_PIN 35
#define BUTTON_2_PIN 0
int vref = 1100;

TFT_eSPI tft = TFT_eSPI(135, 240);

Button2 btn1(BUTTON_1_PIN);
Button2 btn2(BUTTON_2_PIN);

float getVoltage() {
    uint16_t v = analogRead(ADC_PIN);
    return ((float) v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
}

void button_init() {
	btn1.setLongClickTime(1000);
	btn2.setLongClickTime(1000);
	
	btn1.setClickHandler([](Button2 & b) {
		Serial.println("tare");
		tare();
	});

	btn1.setLongClickDetectedHandler([](Button2 & b) {
		Serial.println("start calibration");
		calibrate(tft);
	});

	btn1.setDoubleClickHandler([](Button2 & b) {
		Serial.println("deep sleep");
		int r = digitalRead(TFT_BL);
		tft.fillScreen(TFT_BLACK);
		tft.setTextColor(0xFFE0, TFT_BLACK);
		tft.setTextDatum(MC_DATUM);
		tft.setTextFont(1);
		tft.setTextSize(2);
		tft.drawString("Goodbye...", tft.width() / 2, tft.height() / 2 );
		delay(2500);
		tft.fillScreen(TFT_BLACK);

		digitalWrite(TFT_BL, !r);
		tft.writecommand(TFT_DISPOFF);
		tft.writecommand(TFT_SLPIN);

		esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, 0);
		esp_deep_sleep_start();
	});
	
	btn2.setClickHandler([](Button2 & b) {
		Serial.println("start / stop timer");
		timeStart = !timeStart;
	});

	btn2.setLongClickDetectedHandler([](Button2 & b) {
		Serial.println("reset timer");
		seconds = -4;
		timeStart = false;
	});
}

void button_loop() {
    btn1.loop();
    btn2.loop();
}

void screen_init() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

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

void screen_loop() {
	// voltage: 5.04 charged. Don't let it get below 3.25!
	float v = getVoltage();
	int ypos = tft.height()-tft.fontHeight(4);
//	tft.drawFloat(v, 2, 0, ypos, 4);

	if(v < 3.5) {
		tft.setTextColor(0xF800, TFT_BLACK); // red
		tft.drawString("LOW      ", 0, ypos, 4);
	}
	else if(v > 5.0) {
	  tft.setTextColor(0x7E0, TFT_BLACK); // green
		tft.drawString("FULL      ", 0, ypos, 4);
	}
	else if(v > 4.7) {
	  tft.setTextColor(0xFFE0, TFT_BLACK); // yellow
		tft.drawString("CHRG      ", 0, ypos, 4);
	}
	else {
	  tft.setTextColor(0xBDF7, TFT_BLACK); // light grey
		tft.drawString("                 ", 0, ypos, 4);
	}

	// weight
  tft.setTextColor(0xFBE0, TFT_BLACK);
  int xpos = tft.drawFloat(weight, 2, 0, 2, 7);
  tft.drawString("           ", xpos, 2, 7);

	// timer
	String timer = getTimer();
	xpos = tft.width() - tft.textWidth(timer, 7);
  tft.setTextColor(0x7FF, TFT_BLACK);
	tft.drawString(timer, xpos, tft.height()/2, 7);
}

String getTimer() {
	if(!timeStart && seconds==-4) {
		return "              ";
	}
	
	char str[20];
	sprintf(str, "%02d:%02d", seconds/60, seconds%60);
	return String(str);
}

float roundToDecimal(double value, int dec) {
  double mlt = powf( 10.0f, dec );
  value = roundf( value * mlt ) / mlt;
  return (float)value;
}
