#include "buzzer.h"
#include "pressure_sensor.h"

#define DISPLAY 1
// #define DEBUG 1

#ifdef DISPLAY
  #include "display.h"
#endif

// Constants settings for variometer
const float sampling_ratio = 2;
const float altitude_sensibility = 1/sampling_ratio;
const unsigned long sampling_period = 1000/sampling_ratio;

// Objects
Pressure_Sensor pressure;
Buzzer buzzer;

#ifdef DISPLAY
  Display display;
#endif

// Global var
float current_altitude = 0;
float previous_altitude = 0;

// Setup function
void setup() {
  #ifdef DISPLAY
    display.startup();
  #endif

  #ifdef DEBUG
    // init serial DEBUG
    Serial.begin(9600);
    Serial.print("\n\n===========================================================\n");
    Serial.print("================== PARAGLIDING VARIOMETER ==================\n");
    Serial.print("====================== DEBUG mode ON !======================\n");
    Serial.print("===========================================================\n");
  #else
    if( pressure.start_up() == Pressure_Sensor::PRESSURE_SENSOR_OK ) {
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

  #ifdef DISPLAY
    display.clear();  
  #endif
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

  #ifdef DISPLAY
    display.clear();
    display.print_temp(pressure.readTemperature());
    display.print_altitude(delta_altitude*sampling_ratio);
  #endif

  if (delta_altitude >= ((float)altitude_sensibility)) {
    #ifdef DEBUG
      Serial.print(F("Up !!!!!\n"));
    #else
      buzzer.down();
    #endif
    #ifdef DISPLAY
      display.print_move("Up !  ");
    #endif
  } 
  else if (delta_altitude < ((float)-altitude_sensibility)){
    #ifdef DEBUG
      Serial.print(F("Down !!!!!\n"));
    #else
      buzzer.up();
    #endif
    #ifdef DISPLAY
      display.print_move("Down !");
    #endif
  }

  previous_altitude = current_altitude;
  delay(sampling_period);
}
