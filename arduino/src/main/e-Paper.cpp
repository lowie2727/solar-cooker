#define ENABLE_GxEPD2_GFX 0

#include "bitmaps.h"

#include <GxEPD2_3C.h>

#include <Fonts/FreeMonoBold12pt7b.h>

#define GxEPD2_DISPLAY_CLASS GxEPD2_3C
#define GxEPD2_DRIVER_CLASS GxEPD2_583c_Z83 // 648x480

const uint8_t CS_paper = 10;
const uint8_t DC_paper = 9;
const uint8_t RST_paper = 8;
const uint8_t BUSY_paper = 7;

const uint32_t MAX_DISPLAY_BUFFER_SIZE = 5000;
#define MAX_HEIGHT(EPD)                                                        \
  (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8)             \
       ? EPD::HEIGHT                                                           \
       : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)>
    display(GxEPD2_DRIVER_CLASS(CS_paper, DC_paper, RST_paper, BUSY_paper));

void e_PaperSetup() {
  display.init(9600, true, 2, false);
  display.setFullWindow();
  display.clearScreen();
  display.setRotation(2);
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(GxEPD_BLACK);
}

void e_PaperPrint(String dateTime, String AM2315Temp, String AM2315Hum,
                  String windSpeed, String BME680Temp, String BME680Pres,
                  String BME680Hum, String PT100Temp) {
  int16_t tbx_line1, tby_line1, tbx_line2, tby_line2, tbx_line3, tby_line3,
      tbx_line4, tby_line4, tbx_line5, tby_line5, tbx_line6, tby_line6,
      tbx_line11, tby_line11, tbx_line21, tby_line21, tbx_line31, tby_line31,
      tbx_line41, tby_line41, tbx_line51, tby_line51, tbx_line61, tby_line61;
  uint16_t tbw_line1, tbh_line1, tbw_line2, tbh_line2, tbw_line3, tbh_line3,
      tbw_line4, tbh_line4, tbw_line5, tbh_line5, tbw_line6, tbh_line6,
      tbw_line11, tbh_line11, tbw_line21, tbh_line21, tbw_line31, tbh_line31,
      tbw_line41, tbh_line41, tbw_line51, tbh_line51, tbw_line61, tbh_line61;

  const char line1[] = "Water temperature: ";
  const char line2[] = "Wind speed: ";
  const char line3[] = "Outside temperature: ";
  const char line4[] = "Air pressure: ";
  const char line5[] = "Humidity: ";
  const char line6[] = "Last updated: ";

  String line11 = PT100Temp + " *C";
  String line21 = windSpeed + " m/s";
  String line31 = AM2315Temp + " *C";
  String line41 = BME680Pres + " hPa";
  String line51 = AM2315Hum + " %";
  String line61 = dateTime;

  display.getTextBounds(line1, 0, 0, &tbx_line1, &tby_line1, &tbw_line1,
                        &tbh_line1);
  display.getTextBounds(line2, 0, 0, &tbx_line2, &tby_line2, &tbw_line2,
                        &tbh_line2);
  display.getTextBounds(line3, 0, 0, &tbx_line3, &tby_line3, &tbw_line3,
                        &tbh_line3);
  display.getTextBounds(line4, 0, 0, &tbx_line4, &tby_line4, &tbw_line4,
                        &tbh_line4);
  display.getTextBounds(line5, 0, 0, &tbx_line5, &tby_line5, &tbw_line5,
                        &tbh_line5);
  display.getTextBounds(line6, 0, 0, &tbx_line6, &tby_line6, &tbw_line6,
                        &tbh_line6);
  display.getTextBounds(line11, 0, 0, &tbx_line11, &tby_line11, &tbw_line11,
                        &tbh_line11);
  display.getTextBounds(line21, 0, 0, &tbx_line21, &tby_line21, &tbw_line21,
                        &tbh_line21);
  display.getTextBounds(line31, 0, 0, &tbx_line31, &tby_line31, &tbw_line31,
                        &tbh_line31);
  display.getTextBounds(line41, 0, 0, &tbx_line41, &tby_line41, &tbw_line41,
                        &tbh_line41);
  display.getTextBounds(line51, 0, 0, &tbx_line51, &tby_line51, &tbw_line51,
                        &tbh_line51);
  display.getTextBounds(line61, 0, 0, &tbx_line61, &tby_line61, &tbw_line61,
                        &tbh_line61);

  uint16_t x PROGMEM = 40;

  uint16_t x11 PROGMEM = (display.width() - tbw_line11 - 40);
  uint16_t x21 PROGMEM = (display.width() - tbw_line21 - 40);
  uint16_t x31 PROGMEM = (display.width() - tbw_line31 - 40);
  uint16_t x41 PROGMEM = (display.width() - tbw_line41 - 40);
  uint16_t x51 PROGMEM = (display.width() - tbw_line51 - 40);
  uint16_t x61 PROGMEM = (display.width() - tbw_line61 - 40);

  uint16_t y PROGMEM = ((display.height() - tbh_line1) / 2) - tby_line1;

  const uint8_t y1 PROGMEM = y - 40;
  const uint8_t y3 PROGMEM = y + 40;
  const uint8_t y4 PROGMEM = y + 80;
  const uint8_t y5 PROGMEM = y + 120;
  const uint8_t y6 PROGMEM = y + 200;
  const uint8_t y11 PROGMEM = y - 40;
  const uint8_t y31 PROGMEM = y + 40;
  const uint8_t y41 PROGMEM = y + 80;
  const uint8_t y51 PROGMEM = y + 120;
  const uint8_t y61 PROGMEM = y + 200;

  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y1);
    display.print(line1);
    display.setCursor(x, y);
    display.print(line2);
    display.setCursor(x, y3);
    display.print(line3);
    display.setCursor(x, y4);
    display.print(line4);
    display.setCursor(x, y5);
    display.print(line5);
    display.setCursor(x, y6);
    display.print(line6);
    display.setCursor(x11, y11);
    display.print(line11);
    display.setCursor(x21, y);
    display.print(line21);
    display.setCursor(x31, y31);
    display.print(line31);
    display.setCursor(x41, y41);
    display.print(line41);
    display.setCursor(x51, y51);
    display.print(line51);
    display.setCursor(x61, y61);
    display.print(line61);
    display.drawBitmap(308, 40, uhasselt_logo, 300, 71, GxEPD_BLACK);
    display.drawBitmap(30, 40, UNILU_logo, 248, 71, GxEPD_BLACK);
  } while (display.nextPage());
}
