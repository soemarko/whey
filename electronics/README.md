# Electronics: ADS1232 Breakout Board

![PCB parts mapping](../media/pcb-parts-mapping.png?raw=true)

After receiving my PCB, the only thing that's missing is that I should've label the which wire goes where for the Load Cell input (CN1). Which I have updated in current version of the Gerber file.

That said, let's start from the beginning. CN2 accepts power (right most 2 pins, VCC and GND), and output the ADS1232 to connect to the microcontroller. Thinking back, I should've split this into two 6-pins and 2-pins connection. Anyway, The power go straight to LDO to drop it into a very low noise 3.3V which power the rest of the board.

Then we have RC filter for ADS1232 reference power (REFN and REFP), another RC filter for the load cell output. Then we have feed through caps for the ADS1232 outputs.

That's all for this circuit. I can make it a lot smaller, tighen the gaps here and there. Anyway I think it's pretty good for my first fabbed PCB. For the future version, if I ever make one, it'll be all build in. The ESP32, Linear regulator, charging circuit with under and over voltage protection, the whole shebang. Maybe.

