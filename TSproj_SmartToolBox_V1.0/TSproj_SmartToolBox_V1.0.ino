/*
  Program File for this project
  10-08-2023
*/
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/52, /* data=*/51, /* CS=*/53, /* reset=*/49);

void setup() {
  u8g2.begin();
}

void loop() {
  u8g2.firstPage();
  do {
    u8g2.drawStr(10, 20, "Hello World!");
  } while (u8g2.nextPage());
}
