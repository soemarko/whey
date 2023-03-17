# Whey v2.0

DIY coffee scale / load cell controller.

[![pcb](media/whey-v2.0.jpg?raw=true)](https://youtu.be/54p8B-DXDt8)

Video of it in action: https://youtu.be/54p8B-DXDt8

Whey v2.0 is a wireless 2-piece hardware that is separated between the sensor plate and the display screen. This project currently uses 500g load cell, with ADS1232 analog to digital converter. It uses ESP-Now to communicate between each microcontrollers. More specific information are available in each directories. The story behind this are available in [my blog](https://soemarko.com/blog/whey-v2.0), everything is linked all the way back to [the very first iteration](https://youtu.be/TIj6iEEiVVo).

### Known Issue

- No battery indicator on the plate -- I just have a powerbank ready when it's not turning on
- ~~TFT_eSPI library only works with the one from [TTGO T-Display repo](https://github.com/Xinyuan-LilyGO/TTGO-T-Display), but won't compile when using the latest one by [Bodmer](https://github.com/Bodmer/TFT_eSPI)~~. Fixed: see Sketch readme.

### Todo

☑ Add readme file in each directories for more specific discussion.\
☐ Battery indicator for the plate?\
☐ Make another plate with 3kg load cell\
☐ Add support for multiple plates (at least 2) to screen

## Updates

- 29 Dec 2021: Officially release v2.0
- 19 Jan 2022: Fix so the low batt indicator doesn't blink when the voltage is right on the edge

## Credits

- ADS1232 library was based on the one by [John Sartzetakis](https://gitlab.com/jousis/ads1232-library)
- ADS1232 Breakout was based on [ESPresso Scale](https://gitlab.com/jousis/espresso-scale) (which were also based of the [ADS1232REF](https://www.ti.com/tool/ADS1232REF))
