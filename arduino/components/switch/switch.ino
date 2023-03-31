int switchPin = 6;
int ledPin = LED_BUILTIN;
int switchState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);
}

void loop() {
  switchState = digitalRead(switchPin);

  if (switchState == HIGH) {
    digitalWrite(ledPin, HIGH);
    Serial.println(switchState);
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println(switchState);
  }
}
