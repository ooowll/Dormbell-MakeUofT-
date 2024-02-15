#define BUTTON_MODE 21
#define BUTTON_INC 19
#define BUTTON_DEC 18

int modePressed;
int mode = 0; //Max 3
bool modeState = false;

int incPressed;
int hours = 0;  //Max 23
bool incState = false;

int decPressed;
int minute = 0;  //Max 59
bool decState = false;

void setup() {
  Serial.begin(115200);
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
    case 2: //HOUR IS BLINKING
      incPressed = digitalRead(BUTTON_INC);
      button(&incPressed, &incState, &hours, 23, 1);

      decPressed = digitalRead(BUTTON_DEC);
      button(&decPressed, &decState, &hours, 23, -1);

      break;
    case 3: //MINUTES IS BLINKING
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
