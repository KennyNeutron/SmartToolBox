void CancelledTransaction() {
  // if the file opened okay, write to it:
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


  VariableReset();
}