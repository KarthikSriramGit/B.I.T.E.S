#include "sensors/piezo_driver.h"
#include "core/logger.h"
#include <Arduino.h>

namespace BITS {
namespace Sensors {

PiezoDriver::PiezoSensor PiezoDriver::sensors[MAX_PIEZO_SENSORS];
uint8_t PiezoDriver::sensorCount = 0;
bool PiezoDriver::initialized = false;

void PiezoDriver::init() {
    initialized = true;
    sensorCount = 0;
    Logger::info("Piezo driver initialized");
}

bool PiezoDriver::initSensor(uint8_t id, uint8_t gpio) {
    if (id >= MAX_PIEZO_SENSORS) {
        return false;
    }
    
    PiezoSensor* sensor = &sensors[id];
    sensor->gpio = gpio;
    sensor->threshold = 100.0f; // Default threshold (mV)
    sensor->lastValue = 0.0f;
    sensor->velocity = 0.0f;
    sensor->lastTriggerTime = 0;
    sensor->debounceTime = 50; // 50ms debounce
    sensor->triggered = false;
    
    pinMode(gpio, INPUT);
    sensorCount++;
    
    Logger::info("Piezo sensor %d initialized on GPIO %d", id, gpio);
    return true;
}

void PiezoDriver::calibrate(uint8_t id) {
    PiezoSensor* sensor = getSensor(id);
    if (!sensor) return;
    
    // Read baseline value
    float sum = 0.0f;
    const int samples = 100;
    for (int i = 0; i < samples; i++) {
        sum += analogRead(sensor->gpio);
        delay(10);
    }
    float baseline = (sum / samples) * (3.3f / 4095.0f) * 1000.0f; // Convert to mV
    
    // Set threshold slightly above baseline
    sensor->threshold = baseline + 50.0f;
    Logger::info("Piezo sensor %d calibrated: threshold = %.2f mV", id, sensor->threshold);
}

float PiezoDriver::read(uint8_t id) {
    PiezoSensor* sensor = getSensor(id);
    if (!sensor) return 0.0f;
    
    int raw = analogRead(sensor->gpio);
    float voltage = (raw / 4095.0f) * 3.3f * 1000.0f; // Convert to mV
    
    // Calculate velocity (rate of change)
    sensor->velocity = abs(voltage - sensor->lastValue);
    sensor->lastValue = voltage;
    
    return voltage;
}

float PiezoDriver::getVelocity(uint8_t id) {
    PiezoSensor* sensor = getSensor(id);
    if (!sensor) return 0.0f;
    return sensor->velocity;
}

bool PiezoDriver::isTriggered(uint8_t id) {
    PiezoSensor* sensor = getSensor(id);
    if (!sensor) return false;
    
    float value = read(id);
    uint32_t now = millis();
    
    if (value > sensor->threshold) {
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

void PiezoDriver::setThreshold(uint8_t id, float threshold) {
    PiezoSensor* sensor = getSensor(id);
    if (sensor) {
        sensor->threshold = threshold;
    }
}

float PiezoDriver::getThreshold(uint8_t id) {
    PiezoSensor* sensor = getSensor(id);
    if (!sensor) return 0.0f;
    return sensor->threshold;
}

void PiezoDriver::setDebounceTime(uint8_t id, uint32_t timeMs) {
    PiezoSensor* sensor = getSensor(id);
    if (sensor) {
        sensor->debounceTime = timeMs;
    }
}

PiezoDriver::PiezoSensor* PiezoDriver::getSensor(uint8_t id) {
    if (id >= MAX_PIEZO_SENSORS) {
        return nullptr;
    }
    return &sensors[id];
}

} // namespace Sensors
} // namespace BITS
