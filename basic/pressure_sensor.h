#include <Adafruit_BMP280.h>

class Pressure_Sensor : public Adafruit_BMP280 {
    public:
        typedef enum return_code {
            PRESSURE_SENSOR_OK = 0,
            PRESSURE_SENSOR_FAIL = 1
        };

        start_up();
};
