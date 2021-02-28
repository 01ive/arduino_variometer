#include "pressure_sensor.h"

Error_Pressure_Sensor::t_return_code Pressure_Sensor::start_up() {
  if( !this->begin() ) return Error_Pressure_Sensor::PRESSURE_SENSOR_FAIL;

  // https://cdn-shop.adafruit.com/datasheets/BST-BMP280-DS001-11.pdf

  this->setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X4,       /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
                  
  return Error_Pressure_Sensor::PRESSURE_SENSOR_OK;
}
