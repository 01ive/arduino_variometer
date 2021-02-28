#include "buzzer.h"
#include "pressure_sensor.h"

#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// #define DEBUG 1

const float pressure_sensibility = 1;

Pressure_Sensor pressure;
Buzzer buzzer;

float current_pressure = 0;
float previous_pressure = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Variometer");

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
  lcd.clear();  
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

  lcd.setCursor(0,0);
  lcd.print(pressure.readTemperature());
  lcd.print("C");
  lcd.print(" ");
  lcd.print(delta_pressure);
  lcd.setCursor(0,1);

  if (delta_pressure >= ((float)pressure_sensibility)) {
    #ifdef DEBUG
      Serial.print(F("Up !!!!!\n"));
    #else
      buzzer.down();
    #endif
    lcd.print("Up !");
  } 
  else if (delta_pressure < ((float)-pressure_sensibility)){
    #ifdef DEBUG
      Serial.print(F("Down !!!!!\n"));
    #else
      buzzer.up();
    #endif
    lcd.print("Down !");
  }

  previous_pressure = current_pressure;
  
  delay(1000);
}
