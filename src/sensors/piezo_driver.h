#ifndef BITS_SENSORS_PIEZO_DRIVER_H
#define BITS_SENSORS_PIEZO_DRIVER_H

#include <stdint.h>

namespace BITS {
namespace Sensors {

class PiezoDriver {
public:
    static void init();
    static bool initSensor(uint8_t id, uint8_t gpio);
    static void calibrate(uint8_t id);
    
    static float read(uint8_t id);
    static float getVelocity(uint8_t id);
    static bool isTriggered(uint8_t id);
    
    static void setThreshold(uint8_t id, float threshold);
    static float getThreshold(uint8_t id);
    static void setDebounceTime(uint8_t id, uint32_t timeMs);

private:
    static constexpr uint8_t MAX_PIEZO_SENSORS = 8;
    
    struct PiezoSensor {
        uint8_t gpio;
        float threshold;
        float lastValue;
        float velocity;
        uint32_t lastTriggerTime;
        uint32_t debounceTime;
        bool triggered;
    };
    
    static PiezoSensor sensors[MAX_PIEZO_SENSORS];
    static uint8_t sensorCount;
    static bool initialized;
    
    static PiezoSensor* getSensor(uint8_t id);
};

} // namespace Sensors
} // namespace BITS

#endif // BITS_SENSORS_PIEZO_DRIVER_H
