# Whey v2.0

DIY coffee scale / load cell controller. Each directory will have their own `readme` files.

[![pcb](media/whey-v2.0.jpg?raw=true)](https://youtu.be/54p8B-DXDt8)

Video of it in action: https://youtu.be/54p8B-DXDt8

## Updates

1. ~~PCB has been ordred on Elecrow. Lead time is 3 weeks. It'll be awhile untill I can test it.~~ It arrived, see point 5.
2. [ESP Now](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html) has been tested by sending a random long value every 250ms, and it works great.
3. Restructured some of the paths here so we can access v1.0 files without reverting the commit.
4. Using [Digistump Oak](http://digistump.com/wiki/oak/tutorials/pinout) for the plate. Found it my old bag of µC. Digistump practically dead, but I can still use it like a generic ESP8266, which also support ESP Now. And it can send message to the TTGO.
5. PCB from Elecrow finally arrived. I assembled a couple of them so I can use one with connectors and confirm everything. Surprisingly my pre-written codes works, just need some fine tuning.

### Microcontrollers

#### TTGO T-Display

This one has become my favorite ESP32 board. It has a decent size screen built in, JST-PH (?) for battery input, internal voltage divider to check the battery level, charging circuit, two customizable buttons -- perfect for this project. The only downside is it doesn't have undervoltage protection, so it'll keep going well below 3.2V and ruin your battery.

The TTGO T-Display uses around 150-160 mA with screen and wifi on. However it has amazingly low consumption when in deep sleep mode, under 1mA. I don't have anything to measure that low of current. I've tested it with 600mAH LiPo battery and it lasted more than 3.5 hours with everything turned on. And takes about 90mins to fully charge one. Keep in mind that the T-Display doesn't have low voltage protection so it'll go below 3.2V, I've set it so the battery indicator will say "LOW" in red if it's under 3.5V. However I'm going to with a 1000mAH and use this [case](https://www.thingiverse.com/thing:4843426), superglue a magnet on the back and calling it done.

### Digistump Oak

The Oak uses under 60mA while broadcasting random long values. This shouldn't be any issues since I have the 1800mAH LiPo battery from v1 for this.

### Todo

☑ Design ADS1232 breakout board with filters\
☑ Order and test the PCB\
☑ Wireless comm between the plate and display with [ESP Now](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html)?\
☑ Battery: 600mAH for the plate. 1000mAH for the display.\
☐ Battery indicator for the plate?\
☑ ~~Find ESP32 for the plate (either the Mini32 like the one used for v1, or random long but thin TTGO I found -- depends on enclosure design)~~ Using Digistump Oak, basically a small ESP8266\
☑ TTGO T-Display for the screen: Has internal battery management. Great display. Has 2 buttons built-in\
☐ Add readme file in each directories for more specific discussion.\
☐ Add another plate with 3kg load cell\
☐ Add support for multiple plates (at least 2) to screen

## Credits

- [TTGO T-Display case STL](https://www.thingiverse.com/thing:4843426)
- ADS1232 library was based on the one by [John Sartzetakis](https://gitlab.com/jousis/ads1232-library)
- ADS1232 Breakout was based on [ESPresso Scale](https://gitlab.com/jousis/espresso-scale) (which were also based of the [ADS1232REF](https://www.ti.com/tool/ADS1232REF))
