#ifndef BITS_SENSORS_PRESSURE_DRIVER_H
#define BITS_SENSORS_PRESSURE_DRIVER_H

#include <stdint.h>

namespace BITS {
namespace Sensors {

class PressureDriver {
public:
    static void init();
    static bool initSensor(uint8_t id, uint8_t gpio);
    static void calibrate(uint8_t id);
    
    static float read(uint8_t id);
    static float getVelocity(uint8_t id);
    static bool isTriggered(uint8_t id);
    
    static void setThreshold(uint8_t id, float threshold);
    static float getThreshold(uint8_t id);

private:
    static constexpr uint8_t MAX_PRESSURE_SENSORS = 8;
    
    struct PressureSensor {
        uint8_t gpio;
        float threshold;
        float lastValue;
        float velocity;
        bool triggered;
    };
    
    static PressureSensor sensors[MAX_PRESSURE_SENSORS];
    static uint8_t sensorCount;
    static bool initialized;
    
    static PressureSensor* getSensor(uint8_t id);
};

} // namespace Sensors
} // namespace BITS

#endif // BITS_SENSORS_PRESSURE_DRIVER_H
