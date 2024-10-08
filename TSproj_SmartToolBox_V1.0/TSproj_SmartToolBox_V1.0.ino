/*
  Program File for this project
  10-08-2023
*/
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/52, /* data=*/51, /* CS=*/53, /* reset=*/49);

uint32_t count = 0;
bool count_toggle = false;

void setup() {
  u8g2.begin();
}

void loop() {
  u8g2.firstPage();
  do {
    u8g2_prepare();
    if (count_toggle) {
      u8g2.drawStr(0, 0, "Meron   ");
    } else {
      u8g2.drawStr(0, 0, "Wala    ");
    }
    u8g2.drawStr(10, 20, "Hello World!");
  } while (u8g2.nextPage());
}

void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}
