#include "buzzer.h"
#include "pressure_sensor.h"

Pressure_Sensor pressure;
Buzzer buzzer;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("\n\n===========================================================\n");
  Serial.print("Hello !!!\n\n");

  pinMode(LED_BUILTIN, OUTPUT);
  pressure.start_up();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  buzzer.down();
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
  buzzer.up();
  delay(1000);
}
