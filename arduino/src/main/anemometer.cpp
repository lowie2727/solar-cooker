#include <Arduino.h>

float mapfloat(float x, float in_min, float in_max, float out_min,
               float out_max);

const int ANEMO_PIN = A0;

void anemoSetup() { pinMode(ANEMO_PIN, INPUT); }

float getWindSpeed() {
  float sensorValue = analogRead(ANEMO_PIN);
  float voltage = (sensorValue / 1023) * 5;
  float windSpeed = mapfloat(voltage, 0, 5, 0, 30);
  return windSpeed;
}

float mapfloat(float x, float in_min, float in_max, float out_min,
               float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
