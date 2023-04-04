/***************************************************
  Temperature outside (AM2315)
  Connect red (5V) -> 5V (on arduino mega)
  Connect black (GND) -> GND (on arduino mega)
  Connect yellow (SDA) -> SDA pin, I2C (on arduino mega: 1 pin above AREF pin OR D20 pin)
  Connect white (SCL) -> SCl pin, I2C (on arduino mega: 2 pins above AREF pin OR D21 pin)

  DC-DC step-up boost convertor (MT3608 2A)
  Connect VIN+ -> 5V (on arduino mega)
  Connect VIN- -> (GEEN IDEE WANT VOUT- GEBRUIKEN WE NIET, LOWIE???????)
  
  Anemometer
  Connect brown (7V-24V) -> VOUT+ (on step-up boost convertor VOUT+ pin)
  Connect black (GND) -> GND (on arduino mega)
  Connect red (SIGNAL) -> A0 pin (on arduino mega)

  Clock module (SD2405 RTC Module)
  Connect VCC -> 5V (on arduino mega)
  Connect GND -> GND (on arduino mega)
  Connect SDA -> SDA pin, I2C (on arduino mega: 1 pin above AREF pin OR D20 pin)
  Connect SCL -> SCl pin, I2C (on arduino mega: 2 pins above AREF pin OR D21 pin)

  Air pressure, air quality, temperature, humidity (BME680)
  Connect VCC -> 5V (on arduino mega)
  Connect GND -> GND (on arduino mega)
  Connect SCL -> SCK pin, SPI (on arduino mega: D52 pin)
  Connect SDA -> MOSI pin, SPI (on arduino mega: D51 pin)
  Connect SDD -> MISO pin, SPI (on arduino mega: D50 pin)
  Connect CS -> D6 pin (on arduino mega)

  GPS module (GY-NEO6MV2)
  Connect VCC -> 5V (on arduino mega)
  Connect GND -> GND (on arduino mega)
  Connect RX -> ////////////////////////////////////////////////////////////////////////////////////TODO
  Connect TX -> ////////////////////////////////////////////////////////////////////////////////////TODO

  SD-card
  Connect VCC -> 5V (on arduino mega)
  Connect GND -> GND (on arduino mega)
  Connect MISO -> MISO pin, SPI (on arduino mega: D50 pin)
  Connect MOSI -> MOSI pin, SPI (on arduino mega: D51 pin)
  Connect SCK -> SCK pin, SPI (on arduino mega: D52 pin)
  Connect CS -> D4 pin (on arduino mega)

  
 ****************************************************/

//
//libraries
//
#include <Adafruit_AM2315.h>
//#include "libraries/GravityRtc/GravityRtc.cpp" //////////////////////////////////////TODO GEEN IDEE, LOWIE HELP
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SD.h>


//
//library's initializations
//

// Temperature outside
Adafruit_AM2315 am2315;

///////////////////////////////////////////////////////

// Clock module
GravityRtc rtc;

///////////////////////////////////////////////////////

// Air pressure and quality
#define BME_SCK 52
#define BME_MISO 50
#define BME_MOSI 51
#define BME_CS 6
#define SEALEVELPRESSURE_HPA (1013.25)
//Adafruit_BME680 bme; // I2C
Adafruit_BME680 bme(BME_CS);  // hardware SPI
//Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

/////////////////////////////////////////////////////////

// GPS module
static const int RXPin = 4, TXPin = 3;  ////////////////////////////////////////////////////// TODO (are these correct voor arduino mega?)
static const uint32_t GPSBaud = 9600;
// The TinyGPS++ object
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

////////////////////////////////////////////////////////

// SD-card
#include <SPI.h>
#include <SD.h>
const int chipSelect = 4;
const String fileName = "data.csv";
const String headers = "column1;column2;column3";


//
//variables
//
float temperature_out;
float humidity_out;
float wind_speed;

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }

  ////////////////////////////////////////////////////////

  // Temp sens outside
  Serial.println("AM2315 Test!");
  if (!am2315.begin()) {
    Serial.println("Sensor not found, check wiring & pullups!");
    while (1)
      ;
  }
  delay(50);

  ////////////////////////////////////////////////////////

  // Clock module
  rtc.setup();
  //Set the RTC time automatically: Calibrate RTC time by your computer time
  rtc.adjustRtc(F(__DATE__), F(__TIME__));
  //Set the RTC time manually
  //rtc.adjustRtc(2017,6,19,1,12,7,0);  //Set time: 2017/6/19, Monday, 12:07:00
  delay(50);

  ///////////////////////////////////////////////////////

  // Air pressure and quality
  Serial.println(F("BME680 test"));
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1)
      ;
  }
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);  // 320*C for 150 ms
  delay(50);

  ///////////////////////////////////////////////////////

  // GPS
  ss.begin(GPSBaud);
  delay(50);

  //////////////////////////////////////////////////////

  // SD-card
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1)
      ;
  }
  Serial.println("card initialized.");
  File dataFile = SD.open(fileName, FILE_WRITE);
  if (dataFile) {
    dataFile.println(headers);
    dataFile.close();
    // print to the serial port too:
    Serial.println(headers);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening " + fileName);
  }
  delay(50);


  //////////////////////////////////////////////////////


  delay(2000);
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

void loop() {
  // Temp sens outside
  if (!am2315.readTemperatureAndHumidity(&temperature_out, &humidity_out)) {
    Serial.println("Failed to read data from AM2315");
    return;
  }

  // Anemometer
  float sensorValue = analogRead(A0);
  float voltage = (sensorValue / 1023) * 5;
  windSpeed = mapfloat(voltage, 0.4, 2, 0, 32.4);

  // Clock module
  //after this function the following commands can be used to get the time variables: rtc.year , rtc.month , rtc.day , rtc.week , rtc.hour , rtc.minute , rtc.second
  rtc.read()

    // Air pressure and quality
    //after this function the following commands can be used to get the measured variables: bme.temperature [°C], bme.pressure/100.0 [hPa], bme.humidity [%], bme.gas_resistance/1000.0 [KOhms], bme.readAltitude(SEALEVELPRESSURE_HPA) [m]
    if (!bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  // GPS
  //after this function the following commands can be used to get the measurements: gps.location.lat() , gps.location.lng()
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  // SD-card
  String message = ""  /////////////////////////////////////////////////////////////////////////////////////// TODO fill in this string to contain all correct values
    SaveOnSdCard(message);


  delay(2000);

  //print all variables -> end goal is to show on e-ink
  Serial.print("Temperature out: ");
  Serial.print(temperature_out);
  Serial.println("°C");

  Serial.print("Humidity out: ");
  Serial.print(humidity_out);
  Serial.println("%");

  Serial.print("Wind Speed: ");
  Serial.print(windSpeed);
  Serial.println("m/s");

  Serial.print("Time: ");
  Serial.println(rtc.hour + ":" + rtc.minute);

  Serial.print("Air pressure: ");
  Serial.print(bme.pressure / 100.0);
  Serial.println("hPa");
  Serial.print("Gas resistance: ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println("KOhms");
  Serial.print("Altitude: ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println("m");

  Serial.print("Latitude: ");
  Serial.print(gps.location.lat(), 6);
  Serial.print("  -  Longitude: ");
  Serial.println(gps.location.lng(), 6);

  Serial.println();
  Serial.println();
  Serial.println();
}


//function to map the voltage value of anemometer to wind speed value
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void SaveOnSdCard(String dataString) {
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(fileName, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening " + fileName);
  }
  delay(100);
}
