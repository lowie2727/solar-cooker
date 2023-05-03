# Main

## Notes

CSV files are generated in the following folder structure: YYYY/MM/DDHHMMSS.csv

the header of the CSV file currently looks like this:

| Year | Month | Day | Hour (12-hour clock) | Minute | Second | Outside temperature [°C] | Wind speed [m/s] | Air pressure (inside box) [hPa] | Relative humidity (inside box) [%] | Temperature inside pot [°C] | Solar irradiance [W/m²] |
|------|-------|-----|----------------------|--------|--------|--------------------------|------------------|---------------------------------|------------------------------------|-----------------------------|-------------------------|
## Dependencies

All libraries are available in the Arduino library manager, if not this will be stated.

### [anemometer](../components/anemometer)

/

### [BME680](../components/BME680)

/

### [clockModule](../components/clockModule)

The clockModule uses a library that can be found on github.

- [GravityRtc](https://github.com/DFRobot/Gravity-I2C-SD2405-RTC-Module) (v1.0.1)

### [e-Paper](../components/e-Paper)

- [GxEPD2](https://github.com/ZinggJM/GxEPD2) (v1.5.1)
    - [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO) (v1.14.1)
    - [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library) (v1.11.5)

### [GPS](../components/GPS)

- [TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus) (v1.0.3)

### [microSD](../components/microSD)

- [SD](https://github.com/arduino-libraries/SD) (v1.2.4)

### [PT100](../components/PT100)

/

### [switch](../components/switch)

/

### [temperatureOutside](../components/temperatureOutside)

- [Adafruit AM2315](https://github.com/adafruit/Adafruit_AM2315) (v2.2.1)
    - [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO) (v1.14.1)
