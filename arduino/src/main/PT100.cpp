#include <Adafruit_MAX31865.h>

const uint8_t CS_PT100_1 = 11;
const uint8_t CS_PT100_2 = 12;
const uint8_t CS_PT100_3 = 13;

Adafruit_MAX31865 thermo1 = Adafruit_MAX31865(CS_PT100_1);
Adafruit_MAX31865 thermo2 = Adafruit_MAX31865(CS_PT100_2);
Adafruit_MAX31865 thermo3 = Adafruit_MAX31865(CS_PT100_3);

const float RREF = 430.0;
const float RNOMINAL = 100.0;

void PT100Fault();

void PT100Setup() {
  thermo1.begin(MAX31865_4WIRE);
  thermo2.begin(MAX31865_4WIRE);
  thermo2.begin(MAX31865_4WIRE);
}

String getPT100Temp1() {
  thermo1.readRTD();
  float temperature = thermo1.temperature(RNOMINAL, RREF);
  return String(temperature);
}

String getPT100Temp2() {
  thermo2.readRTD();
  float temperature = thermo2.temperature(RNOMINAL, RREF);
  return String(temperature);
}

String getPT100Temp3() {
  thermo3.readRTD();
  float temperature = thermo3.temperature(RNOMINAL, RREF);
  return String(temperature);
}
