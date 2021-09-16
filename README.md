# Whey

DIY coffee scale / load cell controller.

Build guide is available on `build guide.md` file. It'll be updated to reflect v2.0, but the big picture, it's basically the same project, except now it's wireless and I designed and printed the ADS1232 breakout board exactly as the [reference design](https://www.ti.com/tool/ADS1232REF).

Discussion behind this build is available on [my blog](https://soemarko.com/blog/whey-diy-coffee-scale).

## V2.0

I kinda broke v1.0 when trying to smoothout the output from the sensor, rather than rebuild and do the same thing again, I want to rebuild from the ground-up. First step is to make the ADS1232 reference design almost exactly as is. Next is I want to simplify more things, especially in terms of battery management and workflow. In the todo list below, items with question mark is something to test and see if it'll work.

Again the grand scheme is basically still the same as v1.0, except this time just done properly rather than cobbled together. The programming is practically done until I receive the PCB from Elecrow and test with an actual loadcell. Until then, all I can do is designing a new, non wobbly plate.

For the display / controller part I just plan on using [this thing](https://www.thingiverse.com/thing:4843426), and superglue a magnet behind it. I need to get a new T-Display, since I have soldered on the headers.

## V2.0 Updates

1. PCB has been ordred on Elecrow. Lead time is 3 weeks. It'll be awhile untill I can test it.
2. [ESP Now](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html) has been tested by sending a random long value every 250ms, and it works great.
3. Restructured some of the paths here so we can access v1.0 files without reverting the commit.
4. Using [Digistump Oak](http://digistump.com/wiki/oak/tutorials/pinout) for the plate. Found it my old bag of µC. Digistump practically dead, but I can still use it like a generic ESP8266, which also support ESP Now. And it can send message to the TTGO.

### Power Consumptions

#### TTGO T-Display

The TTGO T-Display uses around 150-160 mA with screen and wifi on. However it has amazingly low consumption when in deep sleep mode, under 1mA. I don't have anything to measure that low of current. I've tested it with 600mAH LiPo battery and it lasted more than 3.5 hours with everything turned on. And takes about 90mins to fully charge one. Keep in mind that the T-Display doesn't have low voltage protection so it'll go below 3.2V, I've set it so the battery indicator will say "LOW" in red if it's under 3.5V.

### Digistump Oak

The Oak uses under 60mA while broadcasting random long values. This shouldn't be any issues since I have the 1800mAH LiPo battery from v1 for this.

### Todo

☑ Design ADS1232 breakout board with filters\
☐ Order and test the PCB\
☑ Wireless comm between the plate and display with [ESP Now](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html)?\
☐ Use 18650 battery for the plate? Voltage divider setup on v1 isn't accurate enough.\
☐ Battery indicator for the plate?\
☑ ~~Find ESP32 for the plate (either the Mini32 like the one used for v1, or random long but thin TTGO I found -- depends on enclosure design)~~ Using Digistump Oak, basically a small ESP8266\
☑ TTGO T-Display for the screen: Has internal battery management. Great display. Has 2 buttons built-in

## Credits

- [TTGO T-Display case STL](https://www.thingiverse.com/thing:4843426)
- ADS1232 library was based on the one by [John Sartzetakis](https://gitlab.com/jousis/ads1232-library)
- ADS1232 Breakout was based on [ESPresso Scale](https://gitlab.com/jousis/espresso-scale) (which were also based of the [ADS1232REF](https://www.ti.com/tool/ADS1232REF))
