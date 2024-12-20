#define sol1 A0
#define sol2 A1
#define sol3 A2
#define sol4 A3
#define sol5 A4


void solenoid_setup() {
  pinMode(sol1, OUTPUT);
  pinMode(sol2, OUTPUT);
  pinMode(sol3, OUTPUT);
  pinMode(sol4, OUTPUT);
  pinMode(sol5, OUTPUT);
  delay(10);
  solenoidCloseAll();
}

void solenoidCloseAll() {
  solenoidClose(1);
  solenoidClose(2);
  solenoidClose(3);
  solenoidClose(4);
  solenoidClose(5);
}



void solenoidOpen(uint8_t solNum) {
  switch (solNum) {
    case 1:
      digitalWrite(sol1, LOW);
      break;
    case 2:
      digitalWrite(sol2, LOW);
      break;
    case 3:
      digitalWrite(sol3, LOW);
      break;
    case 4:
      digitalWrite(sol4, LOW);
      break;
    case 5:
      digitalWrite(sol5, LOW);
      break;
  }
}

void solenoidClose(uint8_t solNum) {
  switch (solNum) {
    case 1:
      digitalWrite(sol1, HIGH);
      break;
    case 2:
      digitalWrite(sol2, HIGH);
      break;
    case 3:
      digitalWrite(sol3, HIGH);
      break;
    case 4:
      digitalWrite(sol4, HIGH);
      break;
    case 5:
      digitalWrite(sol5, HIGH);
      break;
  }
}