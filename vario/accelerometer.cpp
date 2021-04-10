#include "accelerometer.h"

Accelerometer_Sensor::Accelerometer_Sensor(TwoWire *theWire) {
    this->_pI2c = theWire;
}

Accelerometer_Sensor::t_return_code Accelerometer_Sensor::start_up() {
    if(! this->begin(0x68, this->_pI2c, 0) ) return Accelerometer_Sensor::ACCEL_SENSOR_FAIL;

    return Accelerometer_Sensor::ACCEL_SENSOR_OK;
}

Accelerometer_Sensor::tick() {
    this->getEvent(&this->_accel, &this->_gyro, &this->_temp);
}

sensors_vec_t Accelerometer_Sensor::readAcceleration(){
    return this->_accel.acceleration;
}

sensors_vec_t Accelerometer_Sensor::readGyroscope(){
    return this->_gyro.gyro;
}

float Accelerometer_Sensor::readTemperature(){
    return this->_temp.temperature;
}

float Accelerometer_Sensor::getAccelerationNormal(){
    return sqrt( pow(this->_accel.acceleration.x, 2) + pow(this->_accel.acceleration.y, 2) + pow(this->_accel.acceleration.z, 2) );
}
