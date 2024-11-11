//Push Buttons
uint8_t PB_ScanCount = 0;
bool status_PB[18];  //16+2 just to be sure


//RFID HArdware Pins
#define SS_PIN 8
#define RST_PIN 4


#define PB_test false  //for testing push button assignment
bool RFID_scanned = false;
bool Transaction_Cancelled= false;

String this_RFID_scanned="";
String this_scan_TimeStamp="";


bool transact_Withdraw=false;
bool transact_Deposit = false;

bool display_flag=false;

uint8_t DoneCount=0;

uint8_t DrawerOfConcern=0;
