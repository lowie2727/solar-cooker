#include <Arduino.h>

const int PYRANO_PIN = A1;

String getSolarIrradiance() {
  int pyranoValue = analogRead(PYRANO_PIN);
  float pyranoVoltage = pyranoValue * (5.0 / 1023.0);
  float pyranoIrradiance = pyranoVoltage * 20.0;
  return String(pyranoIrradiance);
}
