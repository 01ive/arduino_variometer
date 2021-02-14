#include "pitches.h"
#include "pressure_sensor.h"

const int buzPin  = 2;

Pressure_Sensor pressure;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("\n\n===========================================================\n");
  Serial.print("Hello Olive !!!\n\n");

  pinMode(LED_BUILTIN, OUTPUT);
  pressure.start_up();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  //tone(buzPin, NOTE_C5, 500);
  Serial.print(F("Temperature = "));
  Serial.print(pressure.readTemperature());
  Serial.println(" *C");
  Serial.print(F("Pressure = "));
  Serial.print(pressure.readPressure());
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(pressure.readAltitude(1013.25)); /* Adjusted to local forecast! */
  Serial.println(" m");

  Serial.println();
  delay(3000);
  digitalWrite(LED_BUILTIN, LOW);
  //tone(buzPin, NOTE_A6, 500);
  delay(1000);
}
