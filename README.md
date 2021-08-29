# Whey

DIY coffee scale / load cell controller.

Build guide is available on `build guide.md` file.

Discussion behind this build is available on [my blog](https://soemarko.com/blog/whey-diy-coffee-scale).

## V2.0

I kinda broke v1.0 when trying to smoothout the output from the sensor, rather than rebuild and do the same thing again, I want to rebuild from the ground-up. First step is to make the ADS1232 reference design almost exactly as is. Next is I want to simplify more things, especially in terms of battery management and workflow. In the todo list below, items with question mark is something to test and see if it'll work.

However the grand scheme is basically still the same as v1.0, this time just done properly rather than cobbled together.

### Todo

☑ Design ADS1232 breakout board with filters
☐ Order and test the PCB
☐ Wireless comm between the plate and display with [ESP Now](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html)?
☐ Use 18650 battery for the plate? Voltage divider setup on v1 isn't accurate enough.
☐ Find ESP32 for the plate
☑ TTGO T-Display for the screen:Has internal battery management. Great display. Has 2 buttons built-in

## Credits

- FontAwesome icons were use on the STL
- ADS1232 library was based on the one by [Hamid Saffari](https://github.com/HamidSaffari/ADS123X)
