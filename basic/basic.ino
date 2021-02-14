#include "pitches.h"

const int buzPin  = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  tone(buzPin, NOTE_C5, 500);
  delay(3000);
  digitalWrite(LED_BUILTIN, LOW);
  tone(buzPin, NOTE_A6, 500);
  delay(1000);
}
