#include "ads1232.h"

ads1232::ads1232() {
}

ads1232::~ads1232() {
}

void ads1232::begin(byte pin_DOUT, byte pin_SCLK, byte pin_PDWN) {
	_pin_DOUT = pin_DOUT;
	_pin_SCLK = pin_SCLK;
	_pin_PDWN = pin_PDWN;

	pinMode(_pin_DOUT,  INPUT_PULLUP);
	pinMode(_pin_SCLK, OUTPUT);
	pinMode(_pin_PDWN, OUTPUT);
	
	power_up();
}

bool ads1232::is_ready(void) {
	return digitalRead(_pin_DOUT) == LOW;
}

void ads1232::power_up(void) {
	digitalWrite(_pin_PDWN, HIGH);
	digitalWrite(_pin_SCLK, LOW);
}

void ads1232::power_down(void) {
	digitalWrite(_pin_PDWN, LOW);
	digitalWrite(_pin_SCLK, HIGH);
}

long ads1232::read(bool Calibrating) {
	int i=0;
	unsigned long start;
	long value;

	start=millis();
	while(digitalRead(_pin_DOUT) != HIGH) {
		if(millis()-start > 1000) {
			Serial.println(F("ADS1232 DOUT Timeout HIGH"));
			return 0;
		}
	}

	start=millis();
	while(digitalRead(_pin_DOUT) != LOW) {
		if(millis()-start > 1000) {
			Serial.println(F("ADS1232 DOUT Timeout LOW"));
			return 0;
		}
	}

    // Read 24 bits
    for(i=23 ; i >= 0; i--) {
        digitalWrite(_pin_SCLK, HIGH);
        value = (value << 1) + digitalRead(_pin_DOUT);
        digitalWrite(_pin_SCLK, LOW);
    }

	
	if(Calibrating){
	// 2 extra bits for calibrating
		for(i=1 ; i >= 0; i--) {
			digitalWrite(_pin_SCLK, HIGH);
			digitalWrite(_pin_SCLK, LOW); 
		}
	}
	
    /* Bit 23 is acutally the sign bit. Shift by 8 to get it to the
     * right position (31), divide by 256 to restore the correct value.
     */
	value = (value << 8) / 256;

	if(!Calibrating){
		/* The data pin now is high or low depending on the last bit that
		 * was read.
		 * To get it to the default state (high) we toggle the clock one
		 * more time (see datasheet).
		 */
		digitalWrite(_pin_SCLK, HIGH);
		digitalWrite(_pin_SCLK, LOW);
	}
	return value; // Success
}

float ads1232::read_average(byte times, bool Calibrating) {	
	if(times==0) return 0;
	if(times<3) times = 3;

	long sum = 0;
	long _min = 0;
	long _max = 0;

	for (byte i = 0; i < times; i++) {
		long val = read(Calibrating);

		sum += val;

		if(i == 0) _min = _max = val;
		else {
			if(val < _min) _min = val;
			if(val > _max) _max = val;
		}
	}

	return (float)(sum-_max-_min)/(times-2);
}

float ads1232::get_value(byte times, bool Calibrating) {
	float val = read_average(times, Calibrating);
	return val - OFFSET;
}

float ads1232::get_units(byte times) {
	if(SCALE==0) return 0;

	float val = get_value(10, false);
	return val / SCALE;
}

void ads1232::tare(bool Calibrating) {
	long sum = ceil(read_average(30, Calibrating));
	set_offset(sum);
}

void ads1232::set_scale(float scale) {
	SCALE = scale;
}

float ads1232::get_scale() {
	return SCALE;
}

void ads1232::set_offset(long offset) {
	OFFSET = offset;
}

float ads1232::get_offset() {
	return OFFSET;
}
