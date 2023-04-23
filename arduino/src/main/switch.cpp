#include <Arduino.h>

const uint8_t SWITCH_PIN = 24;
const uint8_t LED_GREEN_PIN = 22;
const uint8_t LED_RED_PIN = 23;

void switchSetup() {
  pinMode(SWITCH_PIN, INPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
}
