#include "pressure_sensor.h"

Pressure_Sensor::Pressure_Sensor(TwoWire *theWire) : Adafruit_BMP280(theWire) {

}

Pressure_Sensor::t_return_code Pressure_Sensor::start_up() {
  if( !this->begin(0x76, 0x58) ) return Pressure_Sensor::PRESSURE_SENSOR_FAIL;
  
  // https://cdn-shop.adafruit.com/datasheets/BST-BMP280-DS001-11.pdf

  this->setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X8,       /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_250); /* Standby time. */
                  
  return Pressure_Sensor::PRESSURE_SENSOR_OK;
}
