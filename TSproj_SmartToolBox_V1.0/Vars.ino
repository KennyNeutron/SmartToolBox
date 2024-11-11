void VariableReset() {
  RFID_scanned = false;
  Transaction_Cancelled = false;

  this_RFID_scanned = "";
  this_scan_TimeStamp = "";


  transact_Withdraw = false;
  transact_Deposit = false;

  display_flag = false;

  DoneCount = 0;
  DrawerOfConcern = 0;
  lcd.clear();
}