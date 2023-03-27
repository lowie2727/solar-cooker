# Main

## Dependencies

All libraries are available in the Arduino library manager, if not this will be stated.

### clockModule

The clockModule uses a library that can be found on github.

- [GravityRtc](https://github.com/DFRobot/Gravity-I2C-SD2405-RTC-Module) (v1.0.1)

### e-Paper

- [GxEPD2](https://github.com/ZinggJM/GxEPD2) (v1.5.1)
    - [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library) (v1.11.5)
    - [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO) (v1.14.1)

### GPS

- [TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus) (v1.0.3)

### microSD

- [SD](https://github.com/arduino-libraries/SD) (v1.2.4)

### temperatureOutside

- [Adafruit AM2315](https://github.com/adafruit/Adafruit_AM2315) (v2.2.1)
    - [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO) (v1.14.1)

## Notes

csv files are generated in the following folder structure: YYYY/MM/DDTHH-MM.csv (the leading 0 is not shown yet).

When pressing the reset button the clockModule does not update the time.

## TODO

- [x] list all the dependencies
- [x] arduino-cli fix
