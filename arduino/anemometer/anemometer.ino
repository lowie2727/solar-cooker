void setup() {
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  float sensorValue = analogRead(A0);
  Serial.print("Analog Value =");
  Serial.println(sensorValue);

  float voltage = (sensorValue / 1023) * 5;
  Serial.print("Voltage =");
  Serial.print(voltage);
  Serial.println(" V");

  float windSpeed = mapfloat(voltage, 0.4, 2, 0, 32.4);
  Serial.print("Wind Speed =");
  Serial.print(windSpeed);
  Serial.println("m/s");
  Serial.println(" ");
  delay(2000);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
