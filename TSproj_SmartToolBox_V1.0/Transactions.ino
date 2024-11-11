void CancelledTransaction() {
  myFile = SD.open(fileName, FILE_WRITE);
  if (myFile) {
    Serial.println("TRANSACTION CANCELLED");
    myFile.println("\n\n===========================================");
    myFile.println("RFID SCANNED [TRANSACTION CANCELLED]");
    myFile.println("USER ID:" + this_RFID_scanned);
    myFile.println(this_scan_TimeStamp);
    myFile.println("===========================================");
    Serial.println("\n\n\n");

    // close the file:
    myFile.close();
    Serial.println("\n\ndone.");
  }


  RFID_scanned = false;
  Transaction_Cancelled = false;

  this_RFID_scanned = "";
  this_scan_TimeStamp = "";


  transact_Withdraw = false;
  transact_Deposit = false;

  display_flag = false;

  DoneCount = 0;
}

void FinishedTransaction() {
  myFile = SD.open(fileName, FILE_WRITE);
  if (myFile) {
    Serial.println("TRANSACTION DONE!");
    myFile.println("\n\n===========================================");
    myFile.println("RFID SCANNED [TRANSACTION SUCCESSFUL]");
    if (transact_Withdraw && !transact_Deposit) {
      myFile.println("TYPE: Item Withdrawal");
      myFile.println("Drawer: #" + String(DrawerOfConcern));
    } else if (!transact_Withdraw && transact_Deposit) {
      myFile.println("TYPE: Item Deposit");
      myFile.println("Drawer: #" + String(DrawerOfConcern));
    }
    myFile.println("USER ID:" + this_RFID_scanned);
    myFile.println(this_scan_TimeStamp);
    myFile.println("===========================================");

    // close the file:
    myFile.close();
  }
  VariableReset();
  delay(300);
  lcd.clear();
  solenoidCloseAll();
}