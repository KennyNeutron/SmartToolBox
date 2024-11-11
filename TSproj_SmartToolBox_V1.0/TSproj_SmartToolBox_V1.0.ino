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


#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

uint32_t last_millis = 0;
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
  //ds3231_setTime(24,11,8,6,10,50,0); //PARAMETERS: [year(2digits)],[month],[date],[DOW],[hour],[minute],[second]
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

  solenoid_setup();
  last_millis = millis();
  Serial.println("SYSTEM INITIALIZATION DONE....\n\n\nReady to Operate");
}

void loop() {
  // Serial.println("TIME:" + ds3231_getTime());
  // Serial.println("DATE:" + ds3231_getDate());
  //Serial.println("looping");
  readRfid();
  printRfid();
  KeyFunctions();

  switch (DoneCount) {
    case 0:
      if (Transaction_Cancelled) {
        lcd.setCursor(0, 0);
        lcd.print("TRANSACTION");
        lcd.setCursor(0, 1);
        lcd.print("CANCELLED!");
        lcd.setCursor(0, 2);
        lcd.print("Press C to Continue");
      } else {
        if (!RFID_scanned) {
          lcd.setCursor(0, 0);
          lcd.print("SCAN RFID TO START");
          lcd.setCursor(0, 1);
          lcd.print("A TRANSACTION");
          lcd.setCursor(0, 2);
          lcd.print("                    ");  //20 blank
        } else {
          if (!transact_Withdraw && !transact_Deposit) {
            lcd.setCursor(0, 0);
            lcd.print("RFID DETECTED");
            lcd.setCursor(0, 1);
            lcd.print(this_RFID_scanned);
            lcd.setCursor(0, 2);
            lcd.print("PRESS");
            lcd.setCursor(6, 2);
            lcd.print("[C]-Cancel");
            lcd.setCursor(0, 3);
            lcd.print("A-Withdraw B-Deposit");
          }
          if (transact_Withdraw) {
            barcodeScan();
            if (!display_flag) {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Withdraw Item on D#?");
              lcd.setCursor(0, 1);
              lcd.print("Select Drawer #");
              display_flag = true;
            }
          }
        }
      }
      break;

    case 1:
      if (!display_flag) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Transaction DONE!");
        lcd.setCursor(0, 1);
        lcd.print("Press D again");
        lcd.setCursor(0, 2);
        lcd.print("to start a new");
        lcd.setCursor(0, 3);
        lcd.print("Transaction");
        Serial.println("TRANSACTION Done!");
        display_flag = true;
      }
      break;
    default:
      VariableReset();
      break;
  }

  //barcodeScan();
  PB_NoPress();
}


void readRfid() {
  if (rfid.isCard() && !RFID_scanned) {
    if (rfid.readCardSerial()) {
      for (int i = 0; i <= 4; i++)  //card value: "xyz xyz xyz xyz xyz" (15 digits maximum; 5 pairs of xyz)hence 0<=i<=4 //
      {
        RFID = rfid.serNum[i];
        cardNum += RFID;  // store RFID value into string "cardNum" and concatinate it with each iteration
      }
      Serial.println("RFID ID Scanned:" + cardNum);
      this_RFID_scanned = cardNum;
      cardNum = "";
      RFID_scanned = true;
      lcd.clear();
      this_scan_TimeStamp = "TIME:" + ds3231_getTime() + "\nDATE:" + ds3231_getDate();
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

    cardNum.remove(0);
    //This is an arduino function.
    //remove the stored value after printing. else the new card value that is read
    // will be concatinated with the previous string.
    delay(50);
  }
}
