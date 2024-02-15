void setup() {
  pinMode(12, OUTPUT);
}

void loop() {
  tone(12, 440); //A
  delay(500);

  noTone(12);
  delay(50);

  tone(12, 440); //A
  delay(500);

  noTone(12);
  delay(50); 

  tone(12, 392); //G
  delay(500);

  noTone(12);
  delay(500);

  tone(12, 392); //G
  delay(250);

  noTone(12);
  delay(50); 

  tone(12, 330); //E
  delay(250);

  noTone(12);
  delay(50); 

  tone(12, 522); //Middle C
  delay(250);

  noTone(12);
  delay(50); 

  tone(12, 440); //A
  delay(250);

  noTone(12);
  delay(1000);

  tone(12, 440); //A
  delay(500);

  noTone(12);
  delay(50);

  tone(12, 660); //E
  delay(500);

  noTone(12);
  delay(50);

  tone(12, 588); //D
  delay(500);

  noTone(12);
  delay(500);

  tone(12, 392); //G
  delay(500);

  noTone(12);
  delay(50); 

  tone(12, 440); //A
  delay(500);

  noTone(12);
  delay(2000);


  /*
  tone(12, 261); //Middle C
  delay(1000);

  tone(12, 277); //C#
  delay(1000);

  tone(12, 294); //D
  delay(1000);

  tone(12, 311); //D#
  delay(1000);

  tone(12, 330); //E
  delay(1000);
 
  tone(12, 349); //F
  delay(1000);

  tone(12, 370); //F#
  delay(1000);

  tone(12, 392); //G
  delay(1000);

  tone(12, 415); //G#
  delay(1000);

  tone(12, 440); //A
  delay(1000);*/

}