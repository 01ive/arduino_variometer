#include "pressure_sensor.h"

Pressure_Sensor::start_up() {
  if( !this->begin() ) return PRESSURE_SENSOR_FAIL;

  this->setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
                  
  return PRESSURE_SENSOR_OK;
}
