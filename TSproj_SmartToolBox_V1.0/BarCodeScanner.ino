void barcodeScan() {
  if (scanner.readBarcode(scanBuffer, BUFFER_LEN)) {
    Serial.print("Code found: ");
    for (int i = 0; i < strlen(scanBuffer); i++) {
      Serial.print(scanBuffer[i]);
    }
    Serial.print("\n");
    ScannedBC = ScannedBC + "\n\tBC:" + String(scanBuffer);
  }
}