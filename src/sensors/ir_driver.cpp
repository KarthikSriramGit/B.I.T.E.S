#include "sensors/ir_driver.h"
#include "core/logger.h"
#include <Arduino.h>

namespace BITS {
namespace Sensors {

IRDriver::IRSensor IRDriver::sensors[MAX_IR_SENSORS];
uint8_t IRDriver::sensorCount = 0;
bool IRDriver::initialized = false;

void IRDriver::init() {
    initialized = true;
    sensorCount = 0;
    Logger::info("IR driver initialized");
}

bool IRDriver::initSensor(uint8_t id, uint8_t gpio) {
    if (id >= MAX_IR_SENSORS) {
        return false;
    }
    
    IRSensor* sensor = &sensors[id];
    sensor->gpio = gpio;
    sensor->threshold = true; // Default: trigger on LOW
    sensor->lastTriggerTime = 0;
    sensor->debounceTime = 10; // 10ms debounce
    sensor->triggered = false;
    
    pinMode(gpio, INPUT_PULLUP);
    sensorCount++;
    
    Logger::info("IR sensor %d initialized on GPIO %d", id, gpio);
    return true;
}

bool IRDriver::read(uint8_t id) {
    IRSensor* sensor = getSensor(id);
    if (!sensor) return false;
    
    return digitalRead(sensor->gpio) == LOW;
}

bool IRDriver::isTriggered(uint8_t id) {
    IRSensor* sensor = getSensor(id);
    if (!sensor) return false;
    
    bool state = read(id);
    uint32_t now = millis();
    
    if (state == sensor->threshold) {
        // Check debounce
        if (now - sensor->lastTriggerTime > sensor->debounceTime) {
            sensor->triggered = true;
            sensor->lastTriggerTime = now;
            return true;
        }
    } else {
        sensor->triggered = false;
    }
    
    return false;
}

void IRDriver::setThreshold(uint8_t id, bool threshold) {
    IRSensor* sensor = getSensor(id);
    if (sensor) {
        sensor->threshold = threshold;
    }
}

void IRDriver::setDebounceTime(uint8_t id, uint32_t timeMs) {
    IRSensor* sensor = getSensor(id);
    if (sensor) {
        sensor->debounceTime = timeMs;
    }
}

IRDriver::IRSensor* IRDriver::getSensor(uint8_t id) {
    if (id >= MAX_IR_SENSORS) {
        return nullptr;
    }
    return &sensors[id];
}

} // namespace Sensors
} // namespace BITS
