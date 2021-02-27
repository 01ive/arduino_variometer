#include "buzzer.h"
#include "pressure_sensor.h"

#define DEBUG 1

const float pressure_sensibility = 1;

Pressure_Sensor pressure;
Buzzer buzzer;

float current_pressure = 0;
float previous_pressure = 0;

void setup() {
  #ifdef DEBUG
    // init serial DEBUG
    Serial.begin(9600);
    Serial.print("\n\n===========================================================\n");
    Serial.print("================== PARAGLIDING VARIOMETER ==================\n");
    Serial.print("====================== DEBUG mode ON !======================\n");
    Serial.print("===========================================================\n");
  #endif

  // Setup app
  if( pressure.start_up() == Error_Pressure_Sensor::PRESSURE_SENSOR_OK ) {
    buzzer.startup();
  }
  else {
    while(1) {
      buzzer.down();
      delay(500);
    }
  }
  previous_pressure = pressure.readPressure();
}

void loop() {
  float delta_pressure;
  
  current_pressure = pressure.readPressure();
  delta_pressure = current_pressure - previous_pressure;

  #ifdef DEBUG
  Serial.print("*********************************\n");
    Serial.print(F("Current temperature = "));
    Serial.print(pressure.readTemperature());
    Serial.print(" °C\n");
    Serial.print(F("Current pressure = "));
    Serial.print(current_pressure);
    Serial.print(" Pa\n");
    Serial.print(F("Previous pressure = "));
    Serial.print(previous_pressure);
    Serial.print(" Pa\n");
    Serial.print(F("Delta pressure = "));
    Serial.print(delta_pressure);
    Serial.print(" Pa\n");
  #endif

  if (delta_pressure >= ((float)pressure_sensibility)) {
    #ifdef DEBUG
      Serial.print(F("Up !!!!!\n"));
    #else
      buzzer.down();
    #endif
  } 
  else if (delta_pressure < ((float)-pressure_sensibility)){
    #ifdef DEBUG
      Serial.print(F("Down !!!!!\n"));
    #else
      buzzer.up();
    #endif
  }

  previous_pressure = current_pressure;
  
  delay(1000);
}