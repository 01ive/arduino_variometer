#include <Adafruit_MPU6050.h>
#include <Wire.h>

#define SAMPLING_INIT 5
#define START_UP_STABILIZATION_TIME_IN_MS 1000
#define SAMPLING_INIT_INTERVAL_IN_MS 1000

class Accelerometer_Sensor : public Adafruit_MPU6050 {
    public:
        typedef enum return_code {
            ACCEL_SENSOR_OK = 0,
            ACCEL_SENSOR_FAIL = 1
        }t_return_code;

        Accelerometer_Sensor::Accelerometer_Sensor(TwoWire *);
        Accelerometer_Sensor::t_return_code Accelerometer_Sensor::start_up();
        Accelerometer_Sensor::tick();

        sensors_event_t Accelerometer_Sensor::readAcceleration();
        sensors_event_t Accelerometer_Sensor::readGyroscope();
        float Accelerometer_Sensor::readTemperature();
        float Accelerometer_Sensor::getAccelerationNormal();
        float Accelerometer_Sensor::getRotationFromAccelX();
        float Accelerometer_Sensor::getRotationFromAccelY();
        float Accelerometer_Sensor::getRotationFromGyroX();
        float Accelerometer_Sensor::getRotationFromGyroY();
        float Accelerometer_Sensor::getRotationFromGyroZ();

    private:
        TwoWire *_pI2c;
        sensors_event_t _accel, _gyro, _temp;
        sensors_event_t _previous_gyro_x, _previous_gyro_y, _previous_gyro_z;
        sensors_event_t _calibration;
        sensors_event_t _init_sampling_table_accel, _init_sampling_table_gyro;

};

