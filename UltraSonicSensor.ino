// include newping library
#include "NewPing.h"

// define trigger and echo pins
#define TRIG_PIN 12
#define ECHO_PIN 14

// max distance to ping for (cm)
#define MAX_DISTANCE 60

// set up newping object 
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Distance = ");
  Serial.print(sonar.ping_cm());
  Serial.println(" cm");
  delay(10);
}