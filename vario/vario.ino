#include "buzzer.h"
#include "pressure_sensor.h"

#define VARIO_NBR_OF_BIPS 7

// https://www.lebipbip.com/fr/paragliding_variometer_explained/
const Buzzer::t_buzzer_sound bips_down =                  {800, 1000, 100};  // Down
const Buzzer::t_buzzer_sound bips[VARIO_NBR_OF_BIPS] = {  {800, 300, 100},   // 0
                                                          {600, 300, 220},   // Up 1st level
                                                          {400, 200, 240},   // Up 2nd level
                                                          {300, 150, 260},   // Up 3rd level
                                                          {200, 100, 280},   // Up 4th level
                                                          {200, 100, 300},   // Up 5th level
                                                          {200, 100, 300} }; // Up 6th level

Buzzer::t_buzzer_sound current_bip;

// Constants settings for variometer
const float sampling_period = 100;
const float speed_sensibility_up = 0.8;
const float speed_sensibility_down = -2;

// Objects
TwoWire I2c;
Pressure_Sensor pressure(&I2c);
Buzzer buzzer;

// Global var
float current_altitude = 0;
float previous_altitude = 0;
float vertical_speed = 0;
float current_time = 0;
float previous_time = 0;

Buzzer::t_buzzer_sound getSound(float speed) {
  Buzzer::t_buzzer_sound bip;
  float vertical_speed_float = 0;
  int vertical_speed_int = 0;

  if( speed > 0 ) {
    vertical_speed_int = (int)speed;
    vertical_speed_float = vertical_speed_int - speed;

    if( vertical_speed_int >= VARIO_NBR_OF_BIPS) {
      vertical_speed_int = VARIO_NBR_OF_BIPS - 1;
      vertical_speed_float = 0;
    }

    bip = bips[vertical_speed_int];

    bip.bip_duration -= (bips[vertical_speed_int].bip_duration - bips[vertical_speed_int+1].bip_duration) * vertical_speed_float;
    bip.bip_note += (bips[vertical_speed_int].bip_note - bips[vertical_speed_int+1].bip_note) * vertical_speed_float;
    bip.duration -= (bips[vertical_speed_int].duration - bips[vertical_speed_int+1].duration) * vertical_speed_float;
  } else {
    bip = bips_down;
    if(speed < -5) {
      speed = -5;
    }
    bip.bip_note += (10 * speed);
  }

  return bip;
}

// Setup function
void setup() {

  if( pressure.start_up() == Pressure_Sensor::PRESSURE_SENSOR_OK ) {
      buzzer.startup();
  }
  else {
      while(1) {
        buzzer.repeat_sound(&bips[0]);
        buzzer.sound_tick();
        delay(500);
      }
  }  
  previous_altitude = pressure.readAltitude();
}

// Infinite loop
void loop() {
  float delta_altitude;
  float delta_time;

  // Calculate vertical speed
  current_altitude = pressure.readAltitude();
  delta_altitude = current_altitude - previous_altitude;
  current_time = millis();
  delta_time = current_time - previous_time;
  vertical_speed = delta_altitude*(1000/delta_time);
    
  if (vertical_speed >= speed_sensibility_up) {
    current_bip = getSound(vertical_speed);
    buzzer.repeat_sound(&current_bip);
  }
  else if (vertical_speed <= speed_sensibility_down) {
    current_bip = getSound(vertical_speed);
    buzzer.repeat_sound(&current_bip);
  }
  else {
    buzzer.stop_sound();
  }

  previous_altitude = current_altitude;
  previous_time = current_time;
  buzzer.sound_tick();
  delay(sampling_period);
}
