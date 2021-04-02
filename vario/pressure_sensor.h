#include <Adafruit_BMP280.h>
#include <Wire.h>

class Pressure_Sensor : public Adafruit_BMP280 {
    public:
        typedef enum return_code {
            PRESSURE_SENSOR_OK = 0,
            PRESSURE_SENSOR_FAIL = 1
        }t_return_code;

        Pressure_Sensor::Pressure_Sensor(TwoWire *);
        Pressure_Sensor::t_return_code start_up();
};
