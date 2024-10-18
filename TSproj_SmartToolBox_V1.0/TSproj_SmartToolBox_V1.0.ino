/*
  Program File for this project
  10-08-2023
*/
#include <Arduino.h>
#include <DigitalIO.h>  // our software SPI library
#include <U8g2lib.h>
#include <SPI.h>
#include <SD.h>
#include "variables.h"
#include "Keymapping.h"
#include "RFID.h"
#include <RtcDS1302.h>

#define SS_PIN 8
#define RST_PIN 4



U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /*E clock=*/46, /* RW data=*/45, /* RS CS=*/47, /* reset=*/49);

#define PB_test true  //for testing push button assignment
uint8_t action = 0;
uint32_t count = 0;
bool count_toggle = false;

//##################################
//RFID
RFID rfid(SS_PIN, RST_PIN);  //create an instance rfid for the class RFID
// varables to store data
String cardNum;
unsigned long RFID;
//##################################

String testStr = "blabla";

const int chipSel = 53;
File myFile;
String fileName = "BlackBox.txt";


ThreeWire myWire(21, 20, 3);  // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

void setup() {
  Serial.begin(9600);
  Serial.println("\n\n\nSmartToolBox\nSYSTEM START...");
  u8g2.begin();

  Serial.println("\ninitilizing RFID...");
  rfid.init();  // initilize the RFID module
  Serial.println("RFID start done ");

  Serial.print("\nInitializing SD card...");
  if (!SD.begin(chipSel)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
    while (true)
      ;
  }
  Serial.println("SD initialization done.");


  // char buffer[7];
  // cardNum.toCharArray(buffer, 6);

  // String fileName = "A" + String(buffer) + "B.txt";
  // Serial.println("File Name:" + fileName);

  myFile = SD.open(fileName, FILE_WRITE);

  Serial.println("\nInit Time Module");
  timeModule_Setup();
  Serial.println("TimeModule Init done!");

  interrupt_setup();
  PushButton_Setup();

  Serial.println("SYSTEM INITIALIZATION DONE.... \n\nReady to SCAN...");
}

void loop() {
  readRfid();
  printRfid();
  KeyFunctions();

  if (action == 1) {
    Serial.println("WITHDRAW ITEM... PRESS THE DESIRED DRAWER (1-5)");
  } else if (action > 10 && action < 20) {
    Serial.println("DRAWER" + String(action - 10) + " is selected, SCAN RFID now...");
  }

  u8g2.firstPage();
  do {
    u8g2_prepare();

    u8g2.drawStr(0, 0, testStr.c_str());
    u8g2.drawStr(10, 20, "RFID!");
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

void readRfid() {
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      for (int i = 0; i <= 4; i++)  //card value: "xyz xyz xyz xyz xyz" (15 digits maximum; 5 pairs of xyz)hence 0<=i<=4 //
      {
        RFID = rfid.serNum[i];
        cardNum += RFID;  // store RFID value into string "cardNum" and concatinate it with each iteration
      }
    }
  }
  rfid.halt();
}

void printRfid() {
  if (cardNum != '\0')  //if string cardNum is not empty, print the value
  {
    Serial.println("Card found");
    Serial.print("Cardnumber: ");
    Serial.println(cardNum);


    RtcDateTime now = Rtc.GetDateTime();
    //getDateTime(now);

    // if the file opened okay, write to it:
    if (myFile) {
      Serial.print("Writing to " + fileName);
      if (action > 10 && action < 20) {
        myFile.println("ITEM WITHRAWAL BY: " + cardNum);
        myFile.println("TIME: " + getDateTime(now));
        myFile.println("on DRAWER#" + String(action - 10) + "\n\n\n");
      } else if (action > 20 && action < 30) {
        myFile.println("ITEM DEPOSIT BY: " + cardNum);
        myFile.println("TIME: " + getDateTime(now) + "\n\n\n");
      }


      Serial.println("\n\n\n");
      // close the file:
      myFile.close();
      Serial.println("\n\ndone.");
      action = 0;
    }

    cardNum.remove(0);
    //This is an arduino function.
    //remove the stored value after printing. else the new card value that is read
    // will be concatinated with the previous string.
    delay(500);
  }
}
