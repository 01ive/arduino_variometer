#include "buzzer.h"
#include "pressure_sensor.h"
#include "display.h"

// #define DEBUG 1

// Constants settings for variometer
const float sampling_ratio = 2;
const float altitude_sensibility = 1/sampling_ratio;
const unsigned long sampling_period = 1000/sampling_ratio;

// Objects
Pressure_Sensor pressure;
Buzzer buzzer;
Display display;

// Global var
float current_altitude = 0;
float previous_altitude = 0;

// Setup function
void setup() {
  display.startup();

  #ifdef DEBUG
    // init serial DEBUG
    Serial.begin(9600);
    Serial.print("\n\n===========================================================\n");
    Serial.print("================== PARAGLIDING VARIOMETER ==================\n");
    Serial.print("====================== DEBUG mode ON !======================\n");
    Serial.print("===========================================================\n");
  #else
    if( pressure.start_up() == Error_Pressure_Sensor::PRESSURE_SENSOR_OK ) {
      buzzer.startup();
    }
    else {
      while(1) {
        buzzer.play_note(buzzer.NOTE_C5);
        delay(500);
      }
    }
  #endif

  previous_altitude = pressure.readAltitude();
  display.clear();  
}

// Infinite loop
void loop() {
  float delta_altitude;
  
  current_altitude = pressure.readAltitude();
  delta_altitude = current_altitude - previous_altitude;

  #ifdef DEBUG
    Serial.print("*********************************\n");
    Serial.print(F("Current temperature = "));
    Serial.print(pressure.readTemperature());
    Serial.print(" °C\n");
    Serial.print(F("Current pressure = "));
    Serial.print(current_altitude);
    Serial.print(" Pa\n");
    Serial.print(F("Previous pressure = "));
    Serial.print(previous_altitude);
    Serial.print(" Pa\n");
    Serial.print(F("Delta pressure = "));
    Serial.print(delta_altitude);
    Serial.print(" Pa\n");
    Serial.print("Altitude = ");
    Serial.print(pressure.readAltitude());
    Serial.print(" m\n");
  #endif

  display.clear();
  display.print_temp(pressure.readTemperature());
  display.print_altitude(delta_altitude*sampling_ratio);

  if (delta_altitude >= ((float)altitude_sensibility)) {
    #ifdef DEBUG
      Serial.print(F("Up !!!!!\n"));
    #else
      buzzer.down();
    #endif
    display.print_move("Up !  ");
  } 
  else if (delta_altitude < ((float)-altitude_sensibility)){
    #ifdef DEBUG
      Serial.print(F("Down !!!!!\n"));
    #else
      buzzer.up();
    #endif
    display.print_move("Down !");
  }

  previous_altitude = current_altitude;

  delay(sampling_period);
}
