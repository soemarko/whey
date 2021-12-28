# Whey v2.0

DIY coffee scale / load cell controller.

[![pcb](media/whey-v2.0.jpg?raw=true)](https://youtu.be/54p8B-DXDt8)

Video of it in action: https://youtu.be/54p8B-DXDt8

## Updates

- 29 Dec 2021: Officially release v2.0. Removed checked todo list.

Whey v2.0 is a wireless 2-piece hardware that is separated between the sensor plate and the display screen. This project currently uses 500g load cell, with ADS1232 analog to digital converter. It uses ESP-Now to communicate between each microcontrollers. More specific information are available in each directories.

### Todo

☑ Add readme file in each directories for more specific discussion.\
☐ Battery indicator for the plate?\
☐ Add another plate with 3kg load cell\
☐ Add support for multiple plates (at least 2) to screen

## Credits

- ADS1232 library was based on the one by [John Sartzetakis](https://gitlab.com/jousis/ads1232-library)
- ADS1232 Breakout was based on [ESPresso Scale](https://gitlab.com/jousis/espresso-scale) (which were also based of the [ADS1232REF](https://www.ti.com/tool/ADS1232REF))
