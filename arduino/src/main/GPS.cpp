#include <SoftwareSerial.h>
#include <TinyGPS++.h>

const uint8_t RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void GPSSetup() { ss.begin(GPSBaud); }

String getGPSLatitude() {
  gps.encode(ss.read());
  return String(gps.location.lat());
}

String getGPSLongitude() {
  gps.encode(ss.read());
  return String(gps.location.lng());
}
