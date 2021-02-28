#include <Adafruit_BMP280.h>

class Error_Pressure_Sensor {
    public:
        typedef enum return_code {
            PRESSURE_SENSOR_OK = 0,
            PRESSURE_SENSOR_FAIL = 1
        }t_return_code;
};

class Pressure_Sensor : public Adafruit_BMP280 {
    public:
        Error_Pressure_Sensor::t_return_code start_up();
};
