#include "buzzer.h"
#include "pressure_sensor.h"

#define DISPLAY 1
// #define DEBUG 1
// #define TONE_SETUP 1

#ifdef DISPLAY
  #include "display.h"
#endif

// Constants settings for variometer
const float sampling_period = 200;
const float speed_sensibility = 0.7;

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
  #endif
  if( pressure.start_up() == Pressure_Sensor::PRESSURE_SENSOR_OK ) {
    #ifndef DEBUG
      buzzer.startup();
    #endif
  }
  else {
    #ifndef DEBUG
      while(1) {
        buzzer.play_note(buzzer.NOTE_C5);
        delay(500);
      }
    #else
       Serial.print("ERROR during pressure initialization\n");
    #endif
  }
  
  previous_altitude = pressure.readAltitude();

  #ifdef DISPLAY
    display.clear();  
  #endif
}

// Infinite loop
void loop() {
  float delta_altitude;
  float vertical_speed;
  
  current_altitude = pressure.readAltitude();
  delta_altitude = current_altitude - previous_altitude;
  vertical_speed = delta_altitude*(1000/sampling_period);

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
    Serial.print("Speed = ");
    Serial.print(vertical_speed);
    Serial.print(" m\n");
  #endif

  #ifdef DISPLAY
    display.clear();
    display.print_temp(pressure.readTemperature());
    display.print_altitude(vertical_speed);
  #endif

  if (vertical_speed >= speed_sensibility) {
    #ifdef DEBUG
      Serial.print(F("Up !!!!!\n"));
    #else
      buzzer.up(vertical_speed);
    #endif
    #ifdef DISPLAY
      display.print_move("Up !  ");
    #endif
  }

  previous_altitude = current_altitude;
  delay(sampling_period);
}

#ifdef TONE_SETUP
  void serialEvent() {
    String inputString = "";

    while (Serial.available()) {
      char inChar = (char)Serial.read();
      inputString += inChar;
    }

    Serial.print(inputString);
    buzzer.play_note(inputString.toInt());
  }
#endif