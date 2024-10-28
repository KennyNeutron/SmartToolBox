#include "Keymapping.h"
bool buttonPressed = false;


/*
  Action 1 : WITHDRAW
*/
void KeyFunctions() {
  if (status_PB[PB_KeyAsterisk] && !buttonPressed) {
    count_toggle = !count_toggle;
  }

  if (status_PB[PB_KeyA] && !buttonPressed) {
    action = 1;
    ScannedBC = "";
    lcd.clear();
  }

  if (status_PB[PB_KeyB] && !buttonPressed) {
    action = 2;
    ScannedBC = "";
    lcd.clear();
  }

  if (action == 1 || action == 2) {
    key_WithdrawDeposit();
    lcd.clear();
  }

  if (PB_NoPress()) {
    buttonPressed = false;
  } else {
    buttonPressed = true;
  }
}

void key_WithdrawDeposit() {
  if (status_PB[PB_Key1] && !buttonPressed) {
    action = (action * 10) + 1;
  }
  if (status_PB[PB_Key2] && !buttonPressed) {
    action = (action * 10) + 2;
  }
  if (status_PB[PB_Key3] && !buttonPressed) {
    action = (action * 10) + 3;
  }
  if (status_PB[PB_Key4] && !buttonPressed) {
    action = (action * 10) + 4;
  }
  if (status_PB[PB_Key5] && !buttonPressed) {
    action = (action * 10) + 5;
  }
}