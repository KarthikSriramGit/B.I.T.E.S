#include "sensors/flex_driver.h"
#include "core/logger.h"
#include <Arduino.h>

namespace BITS {
namespace Sensors {

FlexDriver::FlexSensor FlexDriver::sensors[MAX_FLEX_SENSORS];
uint8_t FlexDriver::sensorCount = 0;
bool FlexDriver::initialized = false;

void FlexDriver::init() {
    initialized = true;
    sensorCount = 0;
    Logger::info("Flex driver initialized");
}

bool FlexDriver::initSensor(uint8_t id, uint8_t gpio) {
    if (id >= MAX_FLEX_SENSORS) {
        return false;
    }
    
    FlexSensor* sensor = &sensors[id];
    sensor->gpio = gpio;
    sensor->threshold = 0.5f; // Default threshold (normalized)
    sensor->baseline = 0.0f;
    sensor->triggered = false;
    
    pinMode(gpio, INPUT);
    sensorCount++;
    
    Logger::info("Flex sensor %d initialized on GPIO %d", id, gpio);
    return true;
}

void FlexDriver::calibrate(uint8_t id) {
    FlexSensor* sensor = getSensor(id);
    if (!sensor) return;
    
    // Read baseline value (unbent state)
    float sum = 0.0f;
    const int samples = 100;
    for (int i = 0; i < samples; i++) {
        sum += analogRead(sensor->gpio);
        delay(10);
    }
    sensor->baseline = sum / samples;
    Logger::info("Flex sensor %d calibrated: baseline = %.2f", id, sensor->baseline);
}

float FlexDriver::read(uint8_t id) {
    FlexSensor* sensor = getSensor(id);
    if (!sensor) return 0.0f;
    
    int raw = analogRead(sensor->gpio);
    // Normalize: 0.0 = unbent, 1.0 = fully bent
    float normalized = abs(raw - sensor->baseline) / 4095.0f;
    return normalized;
}

bool FlexDriver::isTriggered(uint8_t id) {
    FlexSensor* sensor = getSensor(id);
    if (!sensor) return false;
    
    float value = read(id);
    sensor->triggered = (value > sensor->threshold);
    return sensor->triggered;
}

void FlexDriver::setThreshold(uint8_t id, float threshold) {
    FlexSensor* sensor = getSensor(id);
    if (sensor) {
        sensor->threshold = threshold;
    }
}

float FlexDriver::getThreshold(uint8_t id) {
    FlexSensor* sensor = getSensor(id);
    if (!sensor) return 0.0f;
    return sensor->threshold;
}

FlexDriver::FlexSensor* FlexDriver::getSensor(uint8_t id) {
    if (id >= MAX_FLEX_SENSORS) {
        return nullptr;
    }
    return &sensors[id];
}

} // namespace Sensors
} // namespace BITS
