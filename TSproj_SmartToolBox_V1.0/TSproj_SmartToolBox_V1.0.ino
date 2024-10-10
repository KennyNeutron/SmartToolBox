/*
  Program File for this project
  10-08-2023
*/
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <MFRC522.h>

#include "variables.h"
#include "Keymapping.h"


//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
#define SS_PIN 9
#define RST_PIN 8
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
byte readCard[4];
String MasterTag = "40263755";
String tagID = "";
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM


U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/46, /* data=*/45, /* CS=*/47, /* reset=*/49);

#define PB_test false  //for testing push button assignment

uint32_t count = 0;
bool count_toggle = false;

void setup() {
  Serial.begin(9600);
  Serial.println("SmartToolBox\nSYSTEM START...");
  u8g2.begin();

  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  //Show details of PCD - MFRC522 Card Reader
  mfrc522.PCD_DumpVersionToSerial();
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  Serial.println("--------------------------");
  Serial.println(" Access Control ");
  Serial.println("Scan Your Card>>");
  //MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

  interrupt_setup();
  PushButton_Setup();
}

void loop() {
  u8g2.firstPage();
  do {
    u8g2_prepare();

    if (getID()) {
      Serial.println("got ID");
    }
    u8g2.drawStr(0, 0, tagID.c_str());
    u8g2.drawStr(10, 20, "Hello World!");
  } while (u8g2.nextPage());

  PB_NoPress();
}

void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}
