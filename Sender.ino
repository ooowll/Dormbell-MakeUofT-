//Rashaad
//#include <iostream>
using namespace std;
//ULTRASONIC SENSOR
#include "NewPing.h"

// define trigger and echo pins
#define TRIG_PIN 33
#define ECHO_PIN 26

// max distance to ping for (cm)
#define MAX_DISTANCE 300


#include <esp_now.h>
#include <WiFi.h>
#define DOORBELL 22
#define LEDRED 32
#define LEDGREEN 21


// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress1[] = { 0xD4, 0xD4, 0xDA, 0x5E, 0x2E, 0x40 };  //D4:D4:DA:5E:2E:40

//vars
int current_distance;
//amount of time in seconds since reading was non-zero
float timeSinceLast = 0;
//setup
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
//END ULTRASONIC SENSOR

// settings (make sure readInterval divides evenly into 1000*readSeconds)
// readSeconds - how long of a period you would like the ultrasonic sensor to
// average (in seconds)
const int readSeconds = 10;
// readInterval - delay between each ultrasonic sensor reading (in milliseconds)
const int readInterval = 150;
//zeroTolerance - denominator of fraction of the array must be populated (non-zero) for alarm to go off
// 2 means 1/2 of the array must be populated
const int zeroTolerance = 2;
//what distance (in cm) must user be within to be considered as present
const int maxDistance = 120;

// auto-settings
// create distanceArray holding last x seconds of readings
const int distanceArraySize = 1000 * readSeconds / readInterval;
int distanceArray[(int)distanceArraySize];
int *distanceArrayPtr = distanceArray;

typedef struct test_struct {
  int bellPressed;
  bool someoneAtDoor = false;
} test_struct;

test_struct rashaad;

esp_now_peer_info_t peerInfo;

int enqueueDistanceArrayAndReturnAverage(int distance, int distanceArraySize,
                                         int *distanceArrayPtr) {
  // enqueue distance
  // shift everything left one destroying first (oldest) item in array
  for (int i = 0; i < distanceArraySize - 1; i++) {
    distanceArrayPtr[i] = distanceArrayPtr[i + 1];
  }
  distanceArrayPtr[distanceArraySize - 1] = distance;
  // return average of array
  int arrayTotal = 0;
  for (int i = 0; i < distanceArraySize; i++) {
    arrayTotal += distanceArray[i];
  }
  // return average of array
  return arrayTotal / distanceArraySize;
}
void outputArray(int array[], int distanceArraySize) {
  Serial.print("Array:");
  for (int i = 0; i < distanceArraySize; i++) {
    Serial.print(" ");
    Serial.print(array[i]);
  }
  Serial.print("\n");
}

bool isPersonPresent(int distanceArray[], int distanceArraySize, int currentAverage) {
  int zeroCounter = 0;
  for (int i = 0; i < distanceArraySize; i++) {
    if (distanceArray[i] == 0) {
      zeroCounter++;
    }
  }

  if (distanceArraySize - zeroCounter > distanceArraySize / zeroTolerance && currentAverage < maxDistance) return true;
  else return false;
}

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // register peer
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  // register first peer
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(DOORBELL, INPUT_PULLDOWN);
  pinMode(LEDGREEN, OUTPUT);
  pinMode(LEDRED, OUTPUT);
}

void loop() {
  rashaad.bellPressed = digitalRead(DOORBELL);
  esp_err_t result = esp_now_send(0, (uint8_t *)&rashaad, sizeof(test_struct));

  if (rashaad.bellPressed) {
    digitalWrite(LEDGREEN, HIGH);
    digitalWrite(LEDRED, LOW);
  } else {
    digitalWrite(LEDGREEN, LOW);
    digitalWrite(LEDRED, HIGH);
  }
  //   put your main code here, to run repeatedly:
  // ULTRASONIC SENSOR
  current_distance = sonar.ping_cm();
  int currentAverage;
  currentAverage = enqueueDistanceArrayAndReturnAverage(current_distance, distanceArraySize, distanceArrayPtr);
  outputArray(distanceArray, distanceArraySize);
  if (isPersonPresent(distanceArray, distanceArraySize, currentAverage)) {
    //person is at door
    Serial.print("Person at door\n");
    rashaad.someoneAtDoor = true;
  } else {  // person is not at dooer
    Serial.print("Person not at door\n");
    rashaad.someoneAtDoor = false;
  }
  delay(readInterval);
  if (distanceArray[distanceArraySize - 1] == 0) timeSinceLast += ((float)readInterval) / 1000;
  else timeSinceLast = 0;
  Serial.print("Time Since Last: ");
  Serial.print(timeSinceLast);
  Serial.print(" seconds\n");
}
