#include <Arduino.h>

#include <TinyGPS++.h>

static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);
  Serial1.begin(GPSBaud);
}

void loop() {
  // This sketch displays information every time a new sentence is correctly
  // encoded.
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());
    Serial.print("Latitude= ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(" Longitude= ");
    Serial.println(gps.location.lng(), 6);
  }
}
