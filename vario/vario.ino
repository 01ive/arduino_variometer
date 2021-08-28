#include "buzzer.h"
#include "pressure_sensor.h"
#include "accelerometer.h"

// #define DEBUG 1
// #define ACCELEROMETER 1
// #define SOUND_TEST 1

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

#ifdef ACCELEROMETER
  Accelerometer_Sensor accel(&I2c);
#endif

// Global var
float current_altitude = 0;
float previous_altitude = 0;
float vertical_speed = 0;

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
    Serial.print("\nBip duration ");
    Serial.print(bip.bip_duration);
    Serial.print("\nNote ");
    Serial.print(bip.bip_note);
    Serial.print("\nDuration ");
    Serial.print(bip.bip_duration);
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

  #if DEBUG || SOUND_TEST
    // init serial DEBUG
    Serial.begin(9600);
    Serial.print("\n\n===========================================================\n");
    Serial.print("================== PARAGLIDING VARIOMETER ==================\n");
    Serial.print("====================== DEBUG mode ON !======================\n");
    Serial.print("===========================================================\n");
  #endif
  if( pressure.start_up() == Pressure_Sensor::PRESSURE_SENSOR_OK ) {
    // #ifndef DEBUG
      buzzer.startup();
    // #endif
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
       while(1);
    #endif
  }

  #ifdef ACCELEROMETER
    if(accel.start_up() == Accelerometer_Sensor::ACCEL_SENSOR_FAIL) {
      Serial.print("ERROR during accel initialization\n");
      while(1);
    }
  #endif
  
  previous_altitude = pressure.readAltitude();
}

// Infinite loop
void loop() {
  float delta_altitude;

  static float accel_normale, previous_accel_normale;

  #ifndef SOUND_TEST
    // Pressure data
    current_altitude = pressure.readAltitude();
    delta_altitude = current_altitude - previous_altitude;
    vertical_speed = delta_altitude*(1000/sampling_period);
  #endif

  #ifdef DEBUG
    #ifdef ACCELEROMETER
      accel.tick();
      Serial.print("*********************************\n");
      Serial.print("Acceleration X: ");
      Serial.print(accel.readAcceleration().x);
      Serial.print(", Y: ");
      Serial.print(accel.readAcceleration().y);
      Serial.print(", Z: ");
      Serial.print(accel.readAcceleration().z);
      Serial.print(" m/s^2\n");
      Serial.print("Rotation X: ");
      Serial.print(accel.readGyroscope().x);
      Serial.print(", Y: ");
      Serial.print(accel.readGyroscope().y);
      Serial.print(", Z: ");
      Serial.print(accel.readGyroscope().z);
      Serial.print(" rad/s\n");
      Serial.print("Temperature: ");
      Serial.print(accel.readTemperature());
      Serial.print(" °C\n");
      Serial.print("Normal: ");
      Serial.print(accel.getAccelerationNormal());
      Serial.print("\n");
    #endif
    Serial.print("\naltitude = ");
    Serial.print(current_altitude);
    Serial.print(" m ; delta = ");
    Serial.print(delta_altitude);
    Serial.print(" m ; speed = ");
    Serial.print(vertical_speed);
    Serial.print(" m/s");
  #endif

  #ifndef SOUND_TEST
    // Accel data
    #ifdef ACCELEROMETER
      accel_normale = accel.getAccelerationNormal();

      if( (accel.readGyroscope().x < 0.1) && (accel.readGyroscope().y < 0.1) && (accel.readGyroscope().z < 0.1) ) {
          vertical_speed = (vertical_speed - (accel_normale - 7.85)) / 2;
      }
      Serial.print(vertical_speed);
      Serial.print(" m/s\n");
      previous_accel_normale = accel_normale;
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

#ifdef SOUND_TEST
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
