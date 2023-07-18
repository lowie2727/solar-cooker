#include <Arduino.h>

// Ported to SdFat from the native Arduino SD library example by Bill Greiman
// On the Ethernet Shield, CS is pin 4. SdFat handles setting SS
const int chipSelect = 4;
/*
 SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created   Nov 2010
 by David A. Mellis
 updated 2 Dec 2010
 by Tom Igoe
 modified by Bill Greiman 11 Apr 2011
 This example code is in the public domain.

 */
#include <SdFat.h>
SdFat sd;
SdFile myFile;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  } // wait for Leonardo

  // Initialize SdFat or print a detailed error message and halt
  // Use half speed (SPI_HALF_SPEED) like the native library.
  // change to SPI_FULL_SPEED for more performance.
  if (!sd.begin(chipSelect, SPI_FULL_SPEED))
    sd.initErrorHalt();

  // open the file for write at end like the Native SD library
  if (!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening test.txt for write failed");
  }
  // if the file opened okay, write to it:
  Serial.println("Writing to test.txt...");
  myFile.println("testing 1, 2, 3.");

  // close the file:
  myFile.close();
  Serial.println("done.");
}

void loop() {
  // nothing happens after setup
}
