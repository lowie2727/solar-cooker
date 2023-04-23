#include <Arduino.h>
#include <SPI.h>

#include <Adafruit_AM2315.h>

Adafruit_AM2315 am2315;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }
  Serial.println("AM2315 Test!");

  if (!am2315.begin()) {
    Serial.println("Sensor not found, check wiring & pullups!");
    while (1)
      ;
  }

  // begin() does a test read, so need to wait 2secs before first read
  delay(2000);
}

void loop() {
  float temperature, humidity;

  if (!am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
    Serial.println("Failed to read data from AM2315");
    return;
  }
  Serial.print("Temp *C: ");
  Serial.println(temperature);
  Serial.print("Hum %: ");
  Serial.println(humidity);

  delay(2000);
}
