#include "accelerometer.h"

Accelerometer_Sensor::Accelerometer_Sensor(TwoWire *theWire) {
    this->_pI2c = theWire;
}

Accelerometer_Sensor::t_return_code Accelerometer_Sensor::start_up() {
    if(! this->begin(0x68, this->_pI2c, 0) ) return Accelerometer_Sensor::ACCEL_SENSOR_FAIL;
    this->setAccelerometerRange(MPU6050_RANGE_4_G);
    this->setGyroRange(MPU6050_RANGE_500_DEG);
    this->setFilterBandwidth(MPU6050_BAND_5_HZ);

    this->getEvent(&this->_accel, &this->_gyro, &this->_temp);
    delay(START_UP_STABILIZATION_TIME_IN_MS);

    for(int i=0; i<SAMPLING_INIT; i++) {
        delay(SAMPLING_INIT_INTERVAL_IN_MS);
        this->getEvent(&this->_accel, &this->_gyro, &this->_temp);
        this->_init_sampling_table_accel.acceleration.x += this->_accel.acceleration.x;
        this->_init_sampling_table_accel.acceleration.y += this->_accel.acceleration.y;
        this->_init_sampling_table_accel.acceleration.z += this->_accel.acceleration.z;
        this->_init_sampling_table_gyro.gyro.x += this->_gyro.gyro.x;
        this->_init_sampling_table_gyro.gyro.y += this->_gyro.gyro.y;
        this->_init_sampling_table_gyro.gyro.z += this->_gyro.gyro.z;
    }

    this->_init_sampling_table_accel.acceleration.x /= SAMPLING_INIT;
    this->_init_sampling_table_accel.acceleration.y /= SAMPLING_INIT;
    this->_init_sampling_table_accel.acceleration.z /= SAMPLING_INIT;
    this->_init_sampling_table_gyro.gyro.x /= SAMPLING_INIT;
    this->_init_sampling_table_gyro.gyro.y /= SAMPLING_INIT;
    this->_init_sampling_table_gyro.gyro.z /= SAMPLING_INIT;

    // Serial.print("Init Values: ");
    // Serial.print(this->_init_sampling_table_accel.acceleration.x);
    // Serial.print(", ");
    // Serial.print(this->_init_sampling_table_accel.acceleration.y);
    // Serial.print(", ");
    // Serial.print(this->_init_sampling_table_accel.acceleration.z);
    // Serial.print(", ");
    // Serial.print(this->_init_sampling_table_gyro.gyro.x);
    // Serial.print(", ");
    // Serial.print(this->_init_sampling_table_gyro.gyro.y);
    // Serial.print(", ");
    // Serial.print(this->_init_sampling_table_gyro.gyro.z);
    // Serial.println("\n");
        

    return Accelerometer_Sensor::ACCEL_SENSOR_OK;
}

Accelerometer_Sensor::tick() {
    this->getEvent(&this->_accel, &this->_gyro, &this->_temp);
    this->_accel.acceleration.x -= this->_init_sampling_table_accel.acceleration.x;
    this->_accel.acceleration.y -= this->_init_sampling_table_accel.acceleration.y;
    this->_accel.acceleration.z -= this->_init_sampling_table_accel.acceleration.z;
    this->_gyro.gyro.x -= this->_init_sampling_table_gyro.gyro.x;
    this->_gyro.gyro.y -= this->_init_sampling_table_gyro.gyro.y;
    this->_gyro.gyro.z -= this->_init_sampling_table_gyro.gyro.z;
}

sensors_event_t Accelerometer_Sensor::readAcceleration(){
    return this->_accel;
}

sensors_event_t Accelerometer_Sensor::readGyroscope(){
    return this->_gyro;
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

float Accelerometer_Sensor::getRotationFromAccelY(){
    float rotation_y;
    rotation_y = atan(-this->_accel.acceleration.x / sqrt(pow(this->_accel.acceleration.y, 2) + pow(this->_accel.acceleration.z, 2))) * RAD_TO_DEG;
    if (rotation_y <= 360 && rotation_y >= 180) {
    rotation_y = 360 - rotation_y;
    }
    return rotation_y;
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

float Accelerometer_Sensor::getRotationFromGyroY(){
    float sampling_period, rotation_y;

    sampling_period = this->_gyro.timestamp - this->_previous_gyro_y.timestamp;

    rotation_y = this->_gyro.gyro.y * RAD_TO_DEG * sampling_period / 1000;
    if(abs(rotation_y) > 0.1) {
        rotation_y += this->_previous_gyro_x.orientation.y;
        this->_previous_gyro_y.orientation.y = rotation_y;
        this->_previous_gyro_y.timestamp = this->_gyro.timestamp;
    }

    return rotation_y;
}

float Accelerometer_Sensor::getRotationFromGyroZ(){
    float sampling_period, rotation_z;

    sampling_period = this->_gyro.timestamp - this->_previous_gyro_z.timestamp;

    rotation_z = this->_gyro.gyro.z * RAD_TO_DEG * sampling_period / 1000;
    if(abs(rotation_z) > 0.1) {
        rotation_z += this->_previous_gyro_z.orientation.z;
        this->_previous_gyro_z.orientation.z = rotation_z;
        this->_previous_gyro_z.timestamp = this->_gyro.timestamp;
    }

    return rotation_z;
}