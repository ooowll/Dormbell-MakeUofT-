#include <esp_now.h>
#include <WiFi.h>

#define BUTTON_MODE 21
#define BUTTON_INC 19
#define BUTTON_DEC 18

bool bellState = false;
//Structure example to receive data
//Must match the sender structure
typedef struct test_struct {
  int bellPressed;
} test_struct;

//Create a struct_message called myData
test_struct rashaad;

//callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&rashaad, incomingData, sizeof(rashaad));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  if (rashaad.bellPressed == 1 && bellState == false) {
    bellState = true;
    Serial.print("\nOPEN NA NOOR!!!");
  }
  if (rashaad.bellPressed == 0 && bellState == true) {
    bellState = false;
  }
}

int modePressed;
int mode = 0;  //Max 3
bool modeState = false;

int incPressed;
int hours = 0;  //Max 23
bool incState = false;

int decPressed;
int minute = 0;  //Max 59
bool decState = false;

void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);
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
  // clock button setup
  pinMode(BUTTON_MODE, INPUT_PULLDOWN);
  pinMode(BUTTON_INC, INPUT_PULLDOWN);
  pinMode(BUTTON_DEC, INPUT_PULLDOWN);
  Serial.printf("\nmode = %d, %02d:%02d", mode, hours, minute);
}

void loop() {
  modePressed = digitalRead(BUTTON_MODE);
  button(&modePressed, &modeState, &mode, 3, 1);

  switch (mode) {
    case 1: break;  //ENTIRE CLOCK IS BLINKING
    case 2:         //HOUR IS BLINKING
      incPressed = digitalRead(BUTTON_INC);
      button(&incPressed, &incState, &hours, 23, 1);

      decPressed = digitalRead(BUTTON_DEC);
      button(&decPressed, &decState, &hours, 23, -1);

      break;
    case 3:  //MINUTES IS BLINKING
      incPressed = digitalRead(BUTTON_INC);
      button(&incPressed, &incState, &minute, 59, 1);

      decPressed = digitalRead(BUTTON_DEC);
      button(&decPressed, &decState, &minute, 59, -1);
      break;
  }
}

void button(int *buttonPressed, bool *buttonState, int *curr, int max, int increment) {
  if (*buttonPressed == 1 && *buttonState == false) {  // when button is initially pressed
    *buttonState = true;
    *curr += increment;
    if (*curr > max) {
      *curr = 0;
    }
    if (*curr < 0) {
      *curr = max;
    }
  }
  if (*buttonPressed == 0 && *buttonState == true) {  // if button isn't pressed
    *buttonState = false;
    Serial.printf("\nmode = %d, %02d:%02d", mode, hours, minute);
  }
}
