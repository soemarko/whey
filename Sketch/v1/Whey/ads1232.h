#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class ads1232 {
	public:
		ads1232();
		
		virtual ~ads1232();
		
		// Initialize library
		void begin(byte pin_DOUT, byte pin_SCLK, byte pin_PDWN);

		// check if chip is ready
		// from the datasheet: When output data is not ready for retrieval, digital output pin DOUT is high. Serial clock
		// input PD_SCK should be low. When DOUT goes to low, it indicates data is ready for retrieval.
		bool is_ready();
		
		// waits for the chip to be ready and returns a reading
		long read(bool Calibrating = false);

		// returns an average reading; times = how many times to read
		float read_average(byte times = 10, bool Calibrating = false);

		// returns (read_average() - OFFSET), that is the current value without the tare weight; times = how many readings to do
		float get_value(byte times = 5, bool Calibrating = false);

		// returns get_value() divided by SCALE, that is the raw value divided by a value obtained via calibration
		// times = how many readings to do
		float get_units(byte times = 10);

		// set the OFFSET value for tare weight; times = how many times to read the tare value
		void tare(bool Calibrating = false);

		// set the SCALE value; this value is used to convert the raw data to "human readable" data (measure units)
		void set_scale(float scale = 1.f);

		// get the current SCALE
		float get_scale();

		// set OFFSET, the value that's subtracted from the actual reading (tare weight)
		void set_offset(long offset = 0);

		// get the current OFFSET
		float get_offset();
		
		// puts the chip into power down mode
		void power_down();

		// wakes up the chip after power down mode
		void power_up();

	private:
		int _pin_DOUT;
		int _pin_SCLK;
		int _pin_PDWN;
		
		long OFFSET = 0;	// used for tare weight
		float SCALE = 0;	// used to return weight in grams, kg, ounces, whatever
};
