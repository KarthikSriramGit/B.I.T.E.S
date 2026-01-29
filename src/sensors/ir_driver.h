#ifndef BITS_SENSORS_IR_DRIVER_H
#define BITS_SENSORS_IR_DRIVER_H

#include <stdint.h>

namespace BITS {
namespace Sensors {

class IRDriver {
public:
    static void init();
    static bool initSensor(uint8_t id, uint8_t gpio);
    
    static bool read(uint8_t id);
    static bool isTriggered(uint8_t id);
    
    static void setThreshold(uint8_t id, bool threshold);
    static void setDebounceTime(uint8_t id, uint32_t timeMs);

private:
    static constexpr uint8_t MAX_IR_SENSORS = 8;
    
    struct IRSensor {
        uint8_t gpio;
        bool threshold; // true = trigger on LOW, false = trigger on HIGH
        uint32_t lastTriggerTime;
        uint32_t debounceTime;
        bool triggered;
    };
    
    static IRSensor sensors[MAX_IR_SENSORS];
    static uint8_t sensorCount;
    static bool initialized;
    
    static IRSensor* getSensor(uint8_t id);
};

} // namespace Sensors
} // namespace BITS

#endif // BITS_SENSORS_IR_DRIVER_H
