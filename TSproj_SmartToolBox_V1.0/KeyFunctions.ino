#include "Keymapping.h"
bool buttonPressed = false;


/*
  Action 1 : WITHDRAW
*/


void KeyFunctions() {

  if (status_PB[PB_KeyA] && !buttonPressed && RFID_scanned) {
  }

  if (status_PB[PB_KeyB] && !buttonPressed && RFID_scanned) {
  }

  if (status_PB[PB_KeyC] && !buttonPressed && (RFID_scanned || Transaction_Cancelled)) {
    Transaction_Cancelled = !Transaction_Cancelled;
    lcd.clear();
  }


  if (PB_NoPress()) {
    buttonPressed = false;
  } else {
    buttonPressed = true;
  }
}

void key_WithdrawDeposit() {
}