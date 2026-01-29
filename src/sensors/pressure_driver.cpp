#include "sensors/pressure_driver.h"
#include "core/logger.h"
#include <Arduino.h>

namespace BITS {
namespace Sensors {

PressureDriver::PressureSensor PressureDriver::sensors[MAX_PRESSURE_SENSORS];
uint8_t PressureDriver::sensorCount = 0;
bool PressureDriver::initialized = false;

void PressureDriver::init() {
    initialized = true;
    sensorCount = 0;
    Logger::info("Pressure driver initialized");
}

bool PressureDriver::initSensor(uint8_t id, uint8_t gpio) {
    if (id >= MAX_PRESSURE_SENSORS) {
        return false;
    }
    
    PressureSensor* sensor = &sensors[id];
    sensor->gpio = gpio;
    sensor->threshold = 100.0f; // Default threshold
    sensor->lastValue = 0.0f;
    sensor->velocity = 0.0f;
    sensor->triggered = false;
    
    pinMode(gpio, INPUT);
    sensorCount++;
    
    Logger::info("Pressure sensor %d initialized on GPIO %d", id, gpio);
    return true;
}

void PressureDriver::calibrate(uint8_t id) {
    PressureSensor* sensor = getSensor(id);
    if (!sensor) return;
    
    // Read baseline value
    float sum = 0.0f;
    const int samples = 100;
    for (int i = 0; i < samples; i++) {
        sum += analogRead(sensor->gpio);
        delay(10);
    }
    float baseline = (sum / samples) * (5.0f / 4095.0f); // Convert to volts
    
    // Set threshold slightly above baseline
    sensor->threshold = baseline + 0.1f;
    Logger::info("Pressure sensor %d calibrated: threshold = %.2f V", id, sensor->threshold);
}

float PressureDriver::read(uint8_t id) {
    PressureSensor* sensor = getSensor(id);
    if (!sensor) return 0.0f;
    
    int raw = analogRead(sensor->gpio);
    float voltage = (raw / 4095.0f) * 5.0f; // Convert to volts
    
    // Calculate velocity (rate of change)
    sensor->velocity = abs(voltage - sensor->lastValue);
    sensor->lastValue = voltage;
    
    return voltage;
}

float PressureDriver::getVelocity(uint8_t id) {
    PressureSensor* sensor = getSensor(id);
    if (!sensor) return 0.0f;
    return sensor->velocity;
}

bool PressureDriver::isTriggered(uint8_t id) {
    PressureSensor* sensor = getSensor(id);
    if (!sensor) return false;
    
    float value = read(id);
    sensor->triggered = (value > sensor->threshold);
    return sensor->triggered;
}

void PressureDriver::setThreshold(uint8_t id, float threshold) {
    PressureSensor* sensor = getSensor(id);
    if (sensor) {
        sensor->threshold = threshold;
    }
}

float PressureDriver::getThreshold(uint8_t id) {
    PressureSensor* sensor = getSensor(id);
    if (!sensor) return 0.0f;
    return sensor->threshold;
}

PressureDriver::PressureSensor* PressureDriver::getSensor(uint8_t id) {
    if (id >= MAX_PRESSURE_SENSORS) {
        return nullptr;
    }
    return &sensors[id];
}

} // namespace Sensors
} // namespace BITS
