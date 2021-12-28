# Electronics

## ADS1232 Breakout Board

![PCB parts mapping](../media/pcb-parts-mapping.png?raw=true)

After receiving my PCB, the only thing that's missing is that I should've label the which wire goes where for the Load Cell input (CN1). Which I have updated in current version of the Gerber file.

That said, let's start from the beginning. CN2 accepts power (right most 2 pins, VCC and GND), and output the ADS1232 to connect to the microcontroller. Thinking back, I should've split this into two 6-pins and 2-pins connection. VCC pin is 5V. The power go straight to LDO to drop it into a very low noise 3.3V which power the rest of the board.

Then we have RC filter for ADS1232 reference power (REFN and REFP), another RC filter for the load cell output. Then we have feed through caps for the ADS1232 outputs.

That's all for this circuit. I can make it a lot smaller, tighen the gaps here and there. Anyway I think it's pretty good for my first fabbed PCB. For the future version, if I ever make one, it'll be all build in. The ESP32, Linear regulator, charging circuit with under and over voltage protection, the whole shebang. Maybe.

## Batteries

Using LiPo battery for both. For the screen, I'm using 503450 1000mAH battery because it's what the enlosure was designed for. I didn't design the screen case. And I'm using J5019 (voltage booster + battery charger) as the battery manager for the sender, boosted up to 5V. It manages the battery charging. At somepoint I might make something with battery level indicator. For them moment, I just keep using it until it won't turn on anymore.

## Load Cell

At this point I am just using a $10 500g load cell. I have a size limitation because I want a better scale, better workflow for my espresso brew. I'll make another plate with 3kg load cell. There I don't have a maximum size limit, in fact I need it to fit my 6-cup Chemex.

## Putting it together

### Receiver / Screen

It's basically stand-alone ESP32 if you use the TTGO T-Display. Otherwise you'll need at least a screen and two buttons and then just adjust the sketch. Take note of the ESP-Now address from Serial monitor and pop it to the sender 

### Sender / Plate

The way I set mine up is LiPo battery -> J5019 -> on/off switch -> ESP -> ADS1232 -> Load cell. I am using a discontinued ESP8266 board, Digistump Oak, but really any ESP board will do. Where the things connect should be obvious from the board, and in theory any pins should work for the ADS, however during my testing some pins (especially the SCLK pin) doesn't work until I switched it around.

