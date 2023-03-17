# Arduino Sketch

## Receiver / Screen

At minimum you'll need a screen, and two buttons. I'm using TTGO T-Display to avoid building anything, print the enclosure, and superglue magnets behind it and call it a day. After flashing the sketch, take note of the ESP-Now address.

### Button 1 / Top / Pin 35

- Single click: tare
- Click and hold: re-calibrate (I'm using 100g known weight, adjust the `KNOWN_WEIGHT` in file `espnow.ino`)
- Double click: deep sleep

### Button 2 / Bottom / Pin 0

- Single click: start / pause timer. Timer start at -3 and counting up, this is to give me time to do other things and still able to start the brew at 00:00.
- Click and Hold: reset timer.
- Double click: N/A

### Known Issue

When using the TTGO T-Display, if you're using the latest TFT_eSPI library by Bodmer instead of the one from TTGO T-Display repo, don't forget to set the `User_Setup_Select.h` file. Remove / comment out line 22 `//#include <User_Setup.h>`, then uncomment line 53 `#include <User_Setups/Setup25_TTGO_T_Display.h>`. Then everything should compile normally.

## Sender / Plate

Update the sketch with the pins you're using for the ADS1232 (line 20-25). Insert the screen address (line 8). Create a new file named `wificreds.h` for OTA (over the air) update. Upon finishing the build, you'd no longer have access to the microcontroller. This will make updating / changing things much easier than dismantling the whole thing to reflash.

For OTA update, just connect to the same wifi as the scale, and go to `http://whey-sender.local/update` to flash a new one.

To create a new firmware for flashing, from Arduino IDE toolbar, Sketch > Export compiled Binary. Or cmd + option +S.

