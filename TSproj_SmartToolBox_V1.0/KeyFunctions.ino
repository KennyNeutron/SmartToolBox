#include "Keymapping.h"
bool buttonPressed = false;

void KeyFunctions() {
  if (status_PB[PB_KeyAsterisk] && !buttonPressed) {
    count_toggle = !count_toggle;
  }


  if (PB_NoPress()) {
    buttonPressed = false;
  } else {
    buttonPressed = true;
  }
}