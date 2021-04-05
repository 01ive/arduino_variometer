#include <Adafruit_MPU6050.h>
#include <Wire.h>

class Accelerometer_Sensor : public Adafruit_MPU6050 {
    public:
        typedef enum return_code {
            ACCEL_SENSOR_OK = 0,
            ACCEL_SENSOR_FAIL = 1
        }t_return_code;

        Accelerometer_Sensor::Accelerometer_Sensor(TwoWire *);
        Accelerometer_Sensor::t_return_code Accelerometer_Sensor::start_up();
    
    private:
        TwoWire *_pI2c;
};
