# Build Guide

## Parts

1. Printed parts (I highly recommend you re-design the Controller)
2. Load Cell (choose wisely)
3. TTGO Mini32
4. ADS1232 breakout board
5. 0.96" OLED SPI 128x64
6. 2 buttons
7. LiPo Battery

## ADS1232 and Load Cell

To get it working as close as HX711 in terms of usage, just follow exactly below. Further explanation will be available on my blog.

You'll need 5 pins out of the ESP32, 2 of them are 3.3V and GND. 3 are connected to PDMN, DOUT, and SCLK. So here are the connection from top left, counter-clock wise.

- AINP2, and AINN2 (2nd signal, ignore it).
- AINP1, and AINN1: signal voltage from load cell (usually green and white, doesn't matter if you switched it around, calibration will fix it).
- CLK, A0, TEMP, GND all connected together to GND on ESP32. You can just blob it together.
- 3.3V, 5V, REFP, SPEED, GAIN1, and GAIN0 all connected to 3.3V on ESP32.
- Use the same 3.3V from ESP32 for excitation voltage (red and black on load cell).

## TTGO Mini32 and LiPo Battery

Main reason I went with the Mini32 because it has BLE (not used at the moment), and it has the LiPo battery charging circuit built in. Expose the micro USB port, then I can recharge or flash updates without opening the box. I just made a simple voltage divider out of 2 pin and 3 pin male JST, the one pin to read battery level.

## OLED and Buttons

OLED are simple enough, use 2 pins (SDA and SCL pins), pop in the u8g2 library, and we're done.

Buttons are optional, on the prototype build I actually just used a wire from pin 4 (Touch0), and tie it to a bolt, use it as capacitive button. It's a bit spotty, totally acceptable for taring or initiate calibration, but it's super annoying as a timer button.

Pop everything into the printed parts, and finished.