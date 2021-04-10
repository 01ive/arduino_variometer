#include "buzzer.h"
#include "pressure_sensor.h"
#include "accelerometer.h"

// #define DEBUG 1
// #define ACCELEROMETER 1
#define SOUND_TEST 1

#define VARIO_NBR_OF_BIPS 6

// TODO https://www.lebipbip.com/fr/paragliding_variometer_explained/
const Buzzer::t_buzzer_sound bips[VARIO_NBR_OF_BIPS] = {  {2000, 400, 50},    // Down
                                                          {1000, 200, 220},   // Up 1st level
                                                          {800, 200, 240},    // Up 2nd level
                                                          {600, 200, 260},    // Up 3rd level
                                                          {400, 200, 280},    // Up 4th level
                                                          {200, 100, 300} };  // Up 5th level

// Constants settings for variometer
const float sampling_period = 200;
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
int select_sound = 0;

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

      accel_normale = accel.getAccelerationNormal();

      if( (accel.readGyroscope().x < 0.1) && (accel.readGyroscope().y < 0.1) && (accel.readGyroscope().z < 0.1) ) {
        if( (accel_normale - previous_accel_normale > 1) || (accel_normale - previous_accel_normale < -1)) {
          Serial.print("Mount !!!!!!!!!!!!!!!!!!!\n");
        }
      }
      previous_accel_normale = accel_normale;

    #endif

    Serial.print("------------------------------\n");
    Serial.print(F("Current temperature = "));
    Serial.print(pressure.readTemperature());
    Serial.print(" °C\n");
    Serial.print(F("Current pressure = "));
    Serial.print(pressure.readPressure());
    Serial.print(" Pa\n");
    Serial.print("Altitude = ");
    Serial.print(pressure.readAltitude());
    Serial.print(" m\n");
    Serial.print("Speed = ");
    Serial.print(vertical_speed);
    Serial.print(" m/s\n");
  #endif

  #ifndef SOUND_TEST
    current_altitude = pressure.readAltitude();
    delta_altitude = current_altitude - previous_altitude;
    vertical_speed = delta_altitude*(1000/sampling_period);
  #endif

  if (vertical_speed >= speed_sensibility_up) {
    select_sound = (int)vertical_speed;

    if( select_sound >= VARIO_NBR_OF_BIPS) {
      select_sound = VARIO_NBR_OF_BIPS - 1;
    }

    buzzer.repeat_sound(&bips[select_sound]);
  }
  else if (vertical_speed <= speed_sensibility_down) {
    buzzer.repeat_sound(&bips[0]);
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
