// void timeModule_Setup() {
//   Serial.print("compiled: ");
//   Serial.print(__DATE__);
//   Serial.println(__TIME__);

//   Rtc.Begin();

//   RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
//  // printDateTime(compiled);
//  // Serial.println();

//   if (!Rtc.IsDateTimeValid()) {
//     // Common Causes:
//     //    1) first time you ran and the device wasn't running yet
//     //    2) the battery on the device is low or even missing

//     Serial.println("RTC lost confidence in the DateTime!");
//     Rtc.SetDateTime(compiled);
//   }

//   if (Rtc.GetIsWriteProtected()) {
//     Serial.println("RTC was write protected, enabling writing now");
//     Rtc.SetIsWriteProtected(false);
//   }

//   if (!Rtc.GetIsRunning()) {
//     Serial.println("RTC was not actively running, starting now");
//     Rtc.SetIsRunning(true);
//   }

//   RtcDateTime now = Rtc.GetDateTime();
//   if (now < compiled) {
//     Serial.println("RTC is older than compile time!  (Updating DateTime)");
//     Rtc.SetDateTime(compiled);
//   } else if (now > compiled) {
//     Serial.println("RTC is newer than compile time. (this is expected)");
//   } else if (now == compiled) {
//     Serial.println("RTC is the same as compile time! (not expected but all is fine)");
//   }
// }

// #define countof(a) (sizeof(a) / sizeof(a[0]))

// String getDateTime(const RtcDateTime& dt)
// {
//     char datestring[26];

//     snprintf_P(datestring,
//             countof(datestring),
//             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
//             dt.Month(),
//             dt.Day(),
//             dt.Year(),
//             dt.Hour(),
//             dt.Minute(),
//             dt.Second() );
//     Serial.print(datestring);

//     return  datestring;
// }


//DS3231
DS3231 myRTC;
bool century = false;
bool h12Flag;
bool pmFlag;

void ds3231_setTime(uint8_t this_year,
                    uint8_t this_month,
                    uint8_t this_date,
                    uint8_t this_DOW,
                    uint8_t this_Hour,
                    uint8_t this_Minute,
                    uint8_t this_Second) {
  //SET-UP DATE and Time
  myRTC.setClockMode(false);  // set to 24h
  myRTC.setYear(this_year);
  myRTC.setMonth(this_month);
  myRTC.setDate(this_date);
  myRTC.setDoW(this_DOW);
  myRTC.setHour(this_Hour);
  myRTC.setMinute(this_Minute);
  myRTC.setSecond(this_Second);
}

void ds3231_printTime() {
  char ch_Time[30] = "";
  sprintf(ch_Time, "%02d:%02d:%02d", myRTC.getHour(h12Flag, pmFlag), myRTC.getMinute(), myRTC.getSecond());
  String str_Time = String(ch_Time);
  Serial.print("TIME: ");
  Serial.println(str_Time);

  char ch_Date[30] = "";
  sprintf(ch_Date, "%02d/%02d/20%02d", myRTC.getMonth(century), myRTC.getDate(), myRTC.getYear());
  String str_Date = String(ch_Date);
  Serial.print("DATE: ");
  Serial.println(str_Date);
}

String ds3231_getTime() {
  char ch_Time[30] = "";
  sprintf(ch_Time, "%02d:%02d:%02d", myRTC.getHour(h12Flag, pmFlag), myRTC.getMinute(), myRTC.getSecond());
  String str_Time = String(ch_Time);
  return str_Time;
}

String ds3231_getDate() {
  char ch_Date[30] = "";
  sprintf(ch_Date, "%02d/%02d/20%02d", myRTC.getMonth(century), myRTC.getDate(), myRTC.getYear());
  String str_Date = String(ch_Date);
  return str_Date;
}