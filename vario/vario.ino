#include "buzzer.h"
#include "pressure_sensor.h"

#define DISPLAY 1
#define DEBUG 1

#ifdef DISPLAY
  #include "display.h"
#endif

#define VARIO_NBR_OF_BIPS 6

const Buzzer::t_buzzer_sound bips[VARIO_NBR_OF_BIPS] = {  {2000, 400, 50}, 
                                                          {1000, 400, 200},
                                                          {800, 300, 300},
                                                          {600, 200, 400},
                                                          {400, 100, 500},
                                                          {200, 100, 600} };

// Constants settings for variometer
const float sampling_period = 200;
const float speed_sensibility_up = 0.7;
const float speed_sensibility_down = -2;

// Objects
Pressure_Sensor pressure;
Buzzer buzzer;

#ifdef DISPLAY
  Display display;
#endif

// Global var
float current_altitude = 0;
float previous_altitude = 0;
float vertical_speed = 0;
int select_sound = 0;

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
        buzzer.repeat_sound(&bips[0]);
        buzzer.sound_tick();
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
  
  #ifdef DEBUG
    Serial.print("*********************************\n");
    Serial.print(F("Current temperature = "));
    Serial.print(pressure.readTemperature());
    Serial.print(" °C\n");
    Serial.print(F("Current pressure = "));
    Serial.print(current_altitude);
    Serial.print(" Pa\n");
    Serial.print("Altitude = ");
    Serial.print(pressure.readAltitude());
    Serial.print(" m\n");
    Serial.print("Speed = ");
    Serial.print(vertical_speed);
    Serial.print(" m/s\n");
  #else
    current_altitude = pressure.readAltitude();
    delta_altitude = current_altitude - previous_altitude;
    vertical_speed = delta_altitude*(1000/sampling_period);
  #endif

  #ifdef DISPLAY
    display.clear();
    display.print_temp(pressure.readTemperature());
    display.print_altitude(vertical_speed);
  #endif

  if (vertical_speed >= speed_sensibility_up) {
    select_sound = (int)vertical_speed;

    if( select_sound >= VARIO_NBR_OF_BIPS) {
      select_sound = VARIO_NBR_OF_BIPS - 1;
    }

    buzzer.repeat_sound(&bips[select_sound]);
    #ifdef DISPLAY
      display.print_move("Up !  ");
    #endif
  }
  else if (vertical_speed <= speed_sensibility_down) {
    buzzer.repeat_sound(&bips[0]);
    #ifdef DISPLAY
      display.print_move("Down !");
    #endif
  }
  else {
    buzzer.stop_sound();
  }

  previous_altitude = current_altitude;
  buzzer.sound_tick();
  delay(sampling_period);
}

#ifdef DEBUG
  void serialEvent() {
    String inputString = "";

    while (Serial.available()) {
      char inChar = (char)Serial.read();
      inputString += inChar;
    }

    // Serial.print(inputString);
    vertical_speed = inputString.toFloat();
    // buzzer.play_note(inputString.toInt());
  }
#endif
