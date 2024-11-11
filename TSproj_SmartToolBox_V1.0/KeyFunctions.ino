#include "Keymapping.h"
bool buttonPressed = false;


/*
  Action 1 : WITHDRAW
*/


void KeyFunctions() {

  if (status_PB[PB_KeyA] && !buttonPressed && RFID_scanned) {
    transact_Withdraw = true;
  }

  if (status_PB[PB_KeyB] && !buttonPressed && RFID_scanned) {
    transact_Deposit = true;
  }

  if (status_PB[PB_KeyD] && !buttonPressed && RFID_scanned) {
    DoneCount++;
    Serial.println("Pressed D, DoneCount:" + String(DoneCount));
    if (DoneCount >= 2) {
      FinishedTransaction();
    }
    display_flag=false;
  }

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if (status_PB[PB_Key1] && !buttonPressed && RFID_scanned) {
    solenoidOpen(1);
    lcd.setCursor(19, 0);
    lcd.print("1");
    lcd.setCursor(0, 1);
    lcd.print("Scan Items now!");
    lcd.setCursor(0, 2);
    lcd.print("Press D when DONE");
    DrawerOfConcern=1;
  }
    if (status_PB[PB_Key2] && !buttonPressed && RFID_scanned) {
    solenoidOpen(2);
    lcd.setCursor(19, 0);
    lcd.print("2");
    lcd.setCursor(0, 1);
    lcd.print("Scan Items now!");
    lcd.setCursor(0, 2);
    lcd.print("Press D when DONE");
    DrawerOfConcern=2;
  }
    if (status_PB[PB_Key3] && !buttonPressed && RFID_scanned) {
    solenoidOpen(3);
    lcd.setCursor(19, 0);
    lcd.print("3");
    lcd.setCursor(0, 1);
    lcd.print("Scan Items now!");
    lcd.setCursor(0, 2);
    lcd.print("Press D when DONE");
    DrawerOfConcern=3;
  }
    if (status_PB[PB_Key4] && !buttonPressed && RFID_scanned) {
    solenoidOpen(4);
    lcd.setCursor(19, 0);
    lcd.print("4");
    lcd.setCursor(0, 1);
    lcd.print("Scan Items now!");
    lcd.setCursor(0, 2);
    lcd.print("Press D when DONE");
    DrawerOfConcern=4;
  }
    if (status_PB[PB_Key5] && !buttonPressed && RFID_scanned) {
    solenoidOpen(5);
    lcd.setCursor(19, 0);
    lcd.print("5");
    lcd.setCursor(0, 1);
    lcd.print("Scan Items now!");
    lcd.setCursor(0, 2);
    lcd.print("Press D when DONE");
    DrawerOfConcern=5;
  }
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



  if (status_PB[PB_KeyC] && !buttonPressed && RFID_scanned) {
    if (Transaction_Cancelled) {
      CancelledTransaction();
    } else {
      Transaction_Cancelled = true;
    }
    lcd.clear();
  }


  if (PB_AllNoPress()) {
    buttonPressed = false;
  } else {
    buttonPressed = true;
  }
}

void key_WithdrawDeposit() {
}