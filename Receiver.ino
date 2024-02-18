#include <esp_now.h>
#include <WiFi.h>
#include <SevSeg.h>
#include <time.h>

#define FLASH 4
#define BUZZER 5

// seven segment display
SevSeg sevseg;
// Number of digits in display
byte numDigits = 4;
// Display select pins
byte digitPins[] = { 19, 18, 14, 13 };
// Display segment pins A,B,C,D,E,F,G,DP
byte segmentPins[] = { 21, 27, 26, 33, 32, 22, 23, 25 };
// Dropping resistors used
bool resistorsOnSegments = true;
// Display type
byte hardwareConfig = COMMON_CATHODE;


// recieve doorbell
bool bellState = false;
// Structure example to receive data
// Must match the sender structure
typedef struct test_struct {
  int bellPressed;
  bool someoneAtDoor = false;
} test_struct;

// Create a struct_message called myData
test_struct reciever;


int getLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return -1;
  }
  int hour = timeinfo.tm_hour;
  int minute = timeinfo.tm_min;
  int timeInDigits = hour * 100 + minute;
  return timeInDigits;
}

// connect to wifi for time stuff
const char *ssid = "iPhone (8)";
const char *password = "Harish10";

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -18000;
const int daylightOffset_sec = 3600;


//callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&reciever, incomingData, sizeof(reciever));
  Serial.print("Bytes received: ");
  Serial.println(len);
  if (reciever.bellPressed == 1 && bellState == false) {
    bellState = true;
    Serial.print("\nOPEN NA NOOR!!!");
    tone(BUZZER, 660);
    digitalWrite(FLASH, HIGH);
    delay(250);
    digitalWrite(FLASH, LOW);
    tone(BUZZER, 552);
    digitalWrite(FLASH, HIGH);
    delay(250);
    digitalWrite(FLASH, LOW);
    noTone(BUZZER);
  }
  if (reciever.bellPressed == 0 && bellState == true) {
    bellState = false;
  }
}


void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);

  // Start display object
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  // Set brightness
  sevseg.setBrightness(90);

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  getLocalTime();

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  pinMode(FLASH, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  sevseg.setNumber(getLocalTime());
  sevseg.refreshDisplay();
}
