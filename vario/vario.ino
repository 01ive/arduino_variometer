#include "buzzer.h"
#include "pressure_sensor.h"
#include "accelerometer.h"

#define DEBUG 1
#define ACCELEROMETER 1
// #define PRESSURE_CENSOR 1

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
Buzzer buzzer;

#ifdef PRESSURE_CENSOR
  Pressure_Sensor pressure(&I2c);
#endif

#ifdef ACCELEROMETER
  Accelerometer_Sensor accel(&I2c);
#endif

// Global var
float current_altitude = 0;
float previous_altitude = 0;
float vertical_speed = 0;
float current_accel = 0;
float previous_accel = 0;
float vertical_speed_from_accel = 0;

float gyro_rot_x = 0;
float gyro_rot_x_new = 0;
float gyro_rot_y = 0;
float gyro_rot_Z = 0;

float accel_rot_x = 0;

float angle = 0;

static Buzzer::t_buzzer_sound getSound(float speed) {
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

static void playErrorSound(int error_code) {
  #ifndef DEBUG
    while(1) {
      buzzer.repeat_sound(&bips[0]);
      buzzer.sound_tick();
      delay(500);
    }
  #else
    Serial.print("ERROR: ");
    Serial.println(error_code);
    while(1);
  #endif
}

// Setup function
void setup() {

  #if DEBUG
    // init serial DEBUG
    Serial.begin(9600);
    // Serial.print("\n\n===========================================================\n");
    // Serial.print("================== PARAGLIDING VARIOMETER ==================\n");
    // Serial.print("====================== DEBUG mode ON !======================\n");
    // Serial.print("===========================================================\n");
  #endif
  #ifdef PRESSURE_CENSOR
  if( pressure.start_up() == Pressure_Sensor::PRESSURE_SENSOR_FAIL ) {

    playErrorSound(10);
  }
  #endif
  #ifdef ACCELEROMETER
    if(accel.start_up() == Accelerometer_Sensor::ACCEL_SENSOR_FAIL) {
      playErrorSound(11);
    }
  #endif
  #ifndef DEBUG
    buzzer.startup();
  #endif
  previous_altitude = pressure.readAltitude();
}

// Infinite loop
void loop() {
  float delta_altitude;
  static float accel_normale, previous_accel_normale;
  
  // Pressure data
  #ifdef PRESSURE_CENSOR
    current_altitude = pressure.readAltitude();
    delta_altitude = current_altitude - previous_altitude;
    vertical_speed = delta_altitude*(1000/sampling_period);
  #endif

  #ifdef ACCELEROMETER
  accel.tick();
    #ifdef DEBUG
      Serial.print(accel.getRotationFromGyroX());
      Serial.print(",");
      Serial.print(accel.getRotationFromAccelX());
      Serial.print(",");
    #endif
  #endif

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
  buzzer.sound_tick();
  delay(sampling_period);
}
