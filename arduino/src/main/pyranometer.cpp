#include <Arduino.h>

const int PYRANO_PIN = A10;

void pyranoSetup() { pinMode(PYRANO_PIN, INPUT); }

float getSolarIrradiance() {
  int pyranoValue = analogRead(PYRANO_PIN);
  float pyranoVoltage = pyranoValue * (5.0 / 1023.0);
  float pyranoIrradiance = pyranoVoltage * 400;
  return pyranoIrradiance;
}
