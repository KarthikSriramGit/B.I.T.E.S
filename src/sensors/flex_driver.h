#ifndef BITS_SENSORS_FLEX_DRIVER_H
#define BITS_SENSORS_FLEX_DRIVER_H

#include <stdint.h>

namespace BITS {
namespace Sensors {

class FlexDriver {
public:
    static void init();
    static bool initSensor(uint8_t id, uint8_t gpio);
    static void calibrate(uint8_t id);
    
    static float read(uint8_t id);
    static bool isTriggered(uint8_t id);
    
    static void setThreshold(uint8_t id, float threshold);
    static float getThreshold(uint8_t id);

private:
    static constexpr uint8_t MAX_FLEX_SENSORS = 8;
    
    struct FlexSensor {
        uint8_t gpio;
        float threshold;
        float baseline;
        bool triggered;
    };
    
    static FlexSensor sensors[MAX_FLEX_SENSORS];
    static uint8_t sensorCount;
    static bool initialized;
    
    static FlexSensor* getSensor(uint8_t id);
};

} // namespace Sensors
} // namespace BITS

#endif // BITS_SENSORS_FLEX_DRIVER_H
