#include "accelerometer.h"

Accelerometer_Sensor::Accelerometer_Sensor(TwoWire *theWire) {
    this->_pI2c = theWire;
}

Accelerometer_Sensor::t_return_code Accelerometer_Sensor::start_up() {
    if(! this->begin(0x68, this->_pI2c, 0) ) return Accelerometer_Sensor::ACCEL_SENSOR_FAIL;

    return Accelerometer_Sensor::ACCEL_SENSOR_OK;
}