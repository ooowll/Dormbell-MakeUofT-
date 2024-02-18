/*
  7-Segment Display - Four Digit, directly wired
  7-seg-multi.ino
  Print a number to a 4-digit 7-segment display

  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/

// Include library
#include "SevSeg.h"

int mode = 0;
int hour = 0;
int minute = 0;
// Create object
SevSeg sevseg;

// Number of digits in display
byte numDigits = 4;
// Display select pins
byte digitPins[] = {19, 18, 14, 13};
// Display segment pins A,B,C,D,E,F,G,DP
byte segmentPins[] = {21, 27, 26, 33, 32, 22, 23, 25};
// Dropping resistors used
bool resistorsOnSegments = true;
// Display type
byte hardwareConfig = COMMON_CATHODE;

void setup() {

  // Start display object
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  // Set brightness
  sevseg.setBrightness(90);
}

void loop() {
      delay(60000);
      if ((minute + 1) > 59) {
        if ((hour + 1) > 23) {
          minute = 0;
          hour = 0;
        }
        else {
          minute = 0;
          hour++;
        }
      }
      else{
        minute++;
      }
  // Set the number to display
  sevseg.setNumber(hour * 100 + minute);
  sevseg.refreshDisplay();
}