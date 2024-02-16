//Rashaad
#include <esp_now.h>
#include <WiFi.h>
bool bellState = false;
//Structure example to receive data
//Must match the sender structure
typedef struct test_struct {
int bellPressed;

} test_struct;

//Create a struct_message called myData
test_struct rashaad;

//callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&rashaad, incomingData, sizeof(rashaad));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  if(rashaad.bellPressed == 1 && bellState == false){
    bellState = true;
  Serial.print("\nOPEN NA NOOR!!!");
  }
 if(rashaad.bellPressed == 0 && bellState == true){
  bellState = false;
 }
}
 
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
}
 
void loop() {

}
