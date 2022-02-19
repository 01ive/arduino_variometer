#include "accelerometer.h"

Accelerometer_Sensor::Accelerometer_Sensor(TwoWire *theWire) {
    this->_pI2c = theWire;
}

Accelerometer_Sensor::t_return_code Accelerometer_Sensor::start_up() {
    if(! this->begin(0x68, this->_pI2c, 0) ) return Accelerometer_Sensor::ACCEL_SENSOR_FAIL;
    this->setAccelerometerRange(MPU6050_RANGE_4_G);
    this->setGyroRange(MPU6050_RANGE_500_DEG);
    this->setFilterBandwidth(MPU6050_BAND_5_HZ);

    return Accelerometer_Sensor::ACCEL_SENSOR_OK;
}

Accelerometer_Sensor::tick() {
    this->getEvent(&this->_accel, &this->_gyro, &this->_temp);
    this->_accel.acceleration.x += -0.02;
    this->_accel.acceleration.y += -0.08;
    this->_accel.acceleration.z += -8.08+SENSORS_GRAVITY_STANDARD;
    this->_gyro.gyro.x += 0.04;
    this->_gyro.gyro.y += -0.01;
    this->_gyro.gyro.z += -0.01;
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
    return -(sqrt( pow(this->_accel.acceleration.x, 2) + pow(this->_accel.acceleration.y, 2) + pow(this->_accel.acceleration.z, 2)) - SENSORS_GRAVITY_STANDARD );
}

float Accelerometer_Sensor::getRotationFromAccelX(){
    float rotation_x;
    rotation_x = (atan2(this->_accel.acceleration.y, this->_accel.acceleration.z)) * RAD_TO_DEG;
    if (rotation_x <= 360 && rotation_x >= 180) {
    rotation_x = 360 - rotation_x;
    }
    return rotation_x;
}

float Accelerometer_Sensor::getRotationFromGyroX(){
    float sampling_period, rotation_x;

    sampling_period = this->_gyro.timestamp - this->_previous_gyro_x.timestamp;

    rotation_x = this->_gyro.gyro.x * RAD_TO_DEG * sampling_period / 1000;
    if(abs(rotation_x) > 0.1) {
        rotation_x += this->_previous_gyro_x.orientation.x;
        this->_previous_gyro_x.orientation.x = rotation_x;
        this->_previous_gyro_x.timestamp = this->_gyro.timestamp;
    }

    return rotation_x;
}
