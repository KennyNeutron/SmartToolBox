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
  }

  if (status_PB[PB_KeyB] && !buttonPressed) {
    action = 2;
  }

  if (status_PB[PB_Key1] && action != 0 && !buttonPressed) {
    action = (action * 10) + 1;
  }

  if (PB_NoPress()) {
    buttonPressed = false;
  } else {
    buttonPressed = true;
  }
}