void barcodeScan() {
  if (scanner.readBarcode(scanBuffer, BUFFER_LEN)) {
    Serial.print("Code found: ");
    for (int i = 0; i < strlen(scanBuffer); i++) {
      Serial.print(scanBuffer[i]);
    }
    Serial.print("\n");
    ScannedBC = ScannedBC + "\n\tBC:" + String(scanBuffer);
    lcd.setCursor(0, 3);
    lcd.print("BC:" + String(scanBuffer));
    this_scan_TimeStamp = this_scan_TimeStamp + "\nBC:" + String(scanBuffer);
  }
}