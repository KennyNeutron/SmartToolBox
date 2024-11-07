/*
  Program File for this project
  10-08-2023
*/
#include <Arduino.h>
#include <DigitalIO.h>  // our software SPI library
#include <SPI.h>
#include <SD.h>
#include "variables.h"
#include "Keymapping.h"
#include "RFID.h"
//#include <RtcDS1302.h>
//#include <virtuabotixRTC.h>
#include <Wire.h>
#include <DS3231.h>

#define SS_PIN 8
#define RST_PIN 4


#define PB_test false  //for testing push button assignment
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


//ThreeWire myWire(21, 20, 3);  // IO, SCLK, CE
//RtcDS1302<ThreeWire> Rtc(myWire);

#include "SoftwareSerial.h"
SoftwareSerial softSerial(12, 13);  //RX, TX: Connect Arduino pin 10 to scanner TX pin. Connect Arduino pin 11 to scanner RX pin.

#include "SparkFun_DE2120_Arduino_Library.h"  //Click here to get the library: http://librarymanager/All#SparkFun_DE2120
DE2120 scanner;

#define BUFFER_LEN 40
char scanBuffer[BUFFER_LEN];
String ScannedBC = "";

bool flag_print = false;
bool flag_print2 = false;
bool flag_print3 = false;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup() {
  Serial.begin(9600);
  Serial.println("\n\n\nSmartToolBox\nSYSTEM START...");

  //LCD Begin Here
  lcd.init();  // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();


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
  //timeModule_Setup();
  //ds3231_setTime(24,11,7,5,11,0,0); //PARAMETERS: [year(2digits)],[month],[date],[DOW],[hour],[minute],[second]
  ds3231_printTime();
  Serial.println("TimeModule Init done!");

  Serial.println("\nDE2120 Scanner Initializing...");
  if (scanner.begin(softSerial) == false) {
    Serial.println("Scanner did not respond. Please check wiring. Did you scan the POR232 barcode? Freezing...");
    while (1)
      ;
  }
  Serial.println("Scanner online!");

  interrupt_setup();
  PushButton_Setup();

  Serial.println("SYSTEM INITIALIZATION DONE....\n\n\nReady to Operate");

  solenoid_setup();

  // RtcDateTime now = Rtc.GetDateTime();
  // Serial.print("TIME:");
  // Serial.println(getDateTime(now));
}

void loop() {
  // Serial.println("TIME:" + ds3231_getTime());
  // Serial.println("DATE:" + ds3231_getDate());
  readRfid();
  printRfid();
  KeyFunctions();


  if (action == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Press A - Withdraw ");
    lcd.setCursor(0, 1);
    lcd.print("Press B - Deposit");
    solenoidCloseAll();
  }

  if (action == 1) {
    if (!flag_print) {
      Serial.println("WITHDRAW ITEM... PRESS THE DESIRED DRAWER (1-5)");
      flag_print = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("WITHDRAW ITEM");
      lcd.setCursor(0, 1);
      lcd.print("Select Drawer (1-5)");
    }
  } else if (action == 2) {
    if (!flag_print) {
      Serial.println("DEPOSIT ITEM... PRESS THE DESIRED DRAWER (1-5)");
      flag_print = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("DEPOSIT ITEM");
      lcd.setCursor(0, 1);
      lcd.print("Select Drawer (1-5)");
    }
  } else if (action > 10 && action < 20) {
    if (!flag_print2) {
      Serial.println("DRAWER" + String(action - 10) + " is selected, SCAN Barcode now.. Scan RFID when done.");
      solenoidOpen(action - 10);
      flag_print2 = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("WITHDRAW ITEM on");
      lcd.setCursor(0, 1);
      lcd.print("Drawer" + String(action - 10) + " Scan Barcode");
      lcd.setCursor(0, 2);
      lcd.print("Scan RFID when done");
    }
    barcodeScan();
  } else if (action > 20 && action < 30) {
    if (!flag_print2) {
      Serial.println("DRAWER" + String(action - 20) + " is selected, SCAN Barcode now.. Scan RFID when done.");
      solenoidOpen(action - 20);
      flag_print2 = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("DEPOSIT ITEM on");
      lcd.setCursor(0, 1);
      lcd.print("Drawer" + String(action - 20) + " Scan Barcode");
      lcd.setCursor(0, 2);
      lcd.print("Scan RFID when done");
    }

    barcodeScan();
  } else if (action == 100) {
    if (!flag_print3) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Press D to");
      lcd.setCursor(0, 1);
      lcd.print("start NEW");
      lcd.setCursor(0, 2);
      lcd.print("transaction");
      flag_print3 = true;
    }
  }

  //barcodeScan();


  PB_NoPress();
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

    //RtcDateTime now = Rtc.GetDateTime();
    //getDateTime(now);

    // if the file opened okay, write to it:
    if (myFile) {
      Serial.println("Scanned BC: \n" + ScannedBC);
      Serial.print("\nWriting to " + fileName);
      myFile.println("\n\n===========================================");
      if (action > 10 && action < 20) {
        myFile.println("ITEM WITHRAWAL BY: " + cardNum);
        //myFile.println("TIME: " + getDateTime(now));
        myFile.println("TIME:" + ds3231_getTime());
        myFile.println("DATE:" + ds3231_getDate());

        myFile.println("on DRAWER#" + String(action - 10));
      } else if (action > 20 && action < 30) {
        myFile.println("ITEM DEPOSIT BY: " + cardNum);
        myFile.println("on DRAWER#" + String(action - 20));
        myFile.println("TIME:" + ds3231_getTime());
        myFile.println("DATE:" + ds3231_getDate());
        //myFile.println("TIME: " + getDateTime(now));
      }
      myFile.println("ITEMS:\n");
      myFile.println(ScannedBC);
      myFile.println("===========================================");
      Serial.println("\n\n\n");
      // close the file:
      myFile.close();
      Serial.println("\n\ndone.");
      action = 100;
    }

    cardNum.remove(0);
    //This is an arduino function.
    //remove the stored value after printing. else the new card value that is read
    // will be concatinated with the previous string.
    delay(500);
  }
}
