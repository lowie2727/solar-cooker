#include <Arduino.h>

int switchPin = 24;
int ledGreenPin = 22;
int ledRedPin = 23;

void setup() {
  Serial.begin(9600);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);
  pinMode(switchPin, INPUT);
}

void loop() {
  int switchState = digitalRead(switchPin);

  digitalWrite(ledGreenPin, switchState);
  digitalWrite(ledRedPin, !switchState);

  Serial.println(switchState);
}
