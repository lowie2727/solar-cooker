#include <Arduino.h>

#include <SD.h>
#include <SPI.h>

const int chipSelect = 4;
const String fileName = "data.csv";
const String headers = "column1;column2;column3";

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1)
      ;
  }
  Serial.println("card initialized.");

  File dataFile = SD.open(fileName, FILE_WRITE);

  if (dataFile) {
    dataFile.println(headers);
    dataFile.close();
    // print to the serial port too:
    Serial.println(headers);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening " + fileName);
  }
}

void loop() {
  for (int x = 0; x < 10; x++) {
    // make a string for assembling the data to log:
    String dataString = "";

    // read three sensors and append to the string:
    for (int analogPin = 0; analogPin < 3; analogPin++) {
      int sensor = analogRead(analogPin);
      dataString += String(sensor);
      if (analogPin < 2) {
        dataString += ";";
      }
    }

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open(fileName, FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening " + fileName);
    }
    delay(100);
  }
  exit(0);
}
