#include <Wire.h>
#include <Adafruit_AM2315.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>

// Pin connections for the AM2315 temperature sensor
#define AM2315_SDA 20
#define AM2315_SCL 21

// Pin connections for the GPS module
#define GPS_RX 15
#define GPS_TX 14

// Pin connections for the e-paper display
#define EPD_CS 10
#define EPD_DC 9
#define EPD_RST 8
#define EPD_BUSY 7

// Create a new instance of the Adafruit_AM2315 class for the temperature sensor
Adafruit_AM2315 am2315 = Adafruit_AM2315(&Wire);

// Create a new instance of the TinyGPSPlus class for the GPS module
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
TinyGPSPlus gps;

// Create a new instance of the GxEPD2_BW class for the e-paper display
GxEPD2_BW<GxEPD2_213_B72, GxEPD2_213_B72::HEIGHT> display(GxEPD2_213_B72(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

void setup() {
  // Initialize the serial communication with the GPS module
  gpsSerial.begin(9600);

  if (!am2315.begin()) {
    Serial.println("Failed to initialize AM2315 sensor.");
    while (1);
  }
  
  // Initialize the e-paper display
  display.init();
  Serial.println("Init");

  // Clear the display
  display.clearScreen();

  // Set the font for the text
  display.setFont(&FreeMonoBold9pt7b);
}

void loop() {
  // Wait for a new GPS fix
  while (!gps.location.isValid()) {
    while (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
    }
  }

  // Read the temperature from the AM2315 sensor
  float temperature = am2315.readTemperature();

  // Convert the latitude and longitude from the GPS module to strings
  String latitude = String(gps.location.lat(), 6);
  String longitude = String(gps.location.lng(), 6);

  // Clear the display
  display.clearScreen();

  // Print the temperature and the latitude and longitude on the display
  display.setCursor(0, 20);
  display.println("Temperature: " + String(temperature) + " C");
  display.println("Latitude: " + latitude);
  display.println("Longitude: " + longitude);
}
