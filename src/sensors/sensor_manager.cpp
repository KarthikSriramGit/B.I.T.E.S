#include "sensors/sensor_manager.h"
#include "sensors/mpu6050_driver.h"
#include "sensors/piezo_driver.h"
#include "sensors/ir_driver.h"
#include "sensors/pressure_driver.h"
#include "sensors/flex_driver.h"
#include "core/logger.h"
#include "rtos/semaphores.h"

namespace BITS {
namespace Sensors {

SensorData SensorManager::sensors[MAX_SENSORS];
uint8_t SensorManager::sensorCount = 0;
bool SensorManager::initialized = false;

void SensorManager::init() {
    if (initialized) {
        Logger::warning("Sensor manager already initialized");
        return;
    }
    
    // Initialize sensor drivers
    MPU6050Driver::init();
    PiezoDriver::init();
    IRDriver::init();
    PressureDriver::init();
    FlexDriver::init();
    
    sensorCount = 0;
    initialized = true;
    Logger::info("Sensor manager initialized");
}

void SensorManager::update() {
    if (!initialized) {
        return;
    }
    
    // Take mutex for sensor access
    if (RTOS::sensorMutex && xSemaphoreTake(RTOS::sensorMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        // Poll all registered sensors
        for (uint8_t i = 0; i < sensorCount; i++) {
            pollSensor(i);
        }
        
        xSemaphoreGive(RTOS::sensorMutex);
    }
}

void SensorManager::calibrate() {
    Logger::info("Calibrating sensors...");
    
    // Calibrate MPU6050
    MPU6050Driver::calibrate();
    
    // Calibrate other sensors
    for (uint8_t i = 0; i < sensorCount; i++) {
        switch (sensors[i].type) {
            case SensorType::PIEZO:
                PiezoDriver::calibrate(sensors[i].id);
                break;
            case SensorType::PRESSURE:
                PressureDriver::calibrate(sensors[i].id);
                break;
            case SensorType::FLEX:
                FlexDriver::calibrate(sensors[i].id);
                break;
            default:
                break;
        }
    }
    
    Logger::info("Sensor calibration complete");
}

bool SensorManager::registerSensor(SensorType type, uint8_t id, uint8_t gpio) {
    if (sensorCount >= MAX_SENSORS) {
        Logger::error("Maximum sensor count reached");
        return false;
    }
    
    if (id >= MAX_SENSORS) {
        Logger::error("Invalid sensor ID");
        return false;
    }
    
    sensors[sensorCount].type = type;
    sensors[sensorCount].id = id;
    sensors[sensorCount].value = 0.0f;
    sensors[sensorCount].velocity = 0.0f;
    sensors[sensorCount].timestamp = millis();
    sensors[sensorCount].triggered = false;
    
    // Initialize sensor driver based on type
    bool success = false;
    switch (type) {
        case SensorType::MPU6050:
            success = MPU6050Driver::init();
            break;
        case SensorType::PIEZO:
            success = PiezoDriver::initSensor(id, gpio);
            break;
        case SensorType::IR:
            success = IRDriver::initSensor(id, gpio);
            break;
        case SensorType::PRESSURE:
            success = PressureDriver::initSensor(id, gpio);
            break;
        case SensorType::FLEX:
            success = FlexDriver::initSensor(id, gpio);
            break;
    }
    
    if (success) {
        sensorCount++;
        Logger::info("Sensor %d registered (type: %d, GPIO: %d)", id, static_cast<int>(type), gpio);
    }
    
    return success;
}

bool SensorManager::unregisterSensor(uint8_t id) {
    for (uint8_t i = 0; i < sensorCount; i++) {
        if (sensors[i].id == id) {
            // Shift remaining sensors
            for (uint8_t j = i; j < sensorCount - 1; j++) {
                sensors[j] = sensors[j + 1];
            }
            sensorCount--;
            Logger::info("Sensor %d unregistered", id);
            return true;
        }
    }
    return false;
}

SensorData SensorManager::getSensorData(uint8_t id) {
    for (uint8_t i = 0; i < sensorCount; i++) {
        if (sensors[i].id == id) {
            return sensors[i];
        }
    }
    
    SensorData empty;
    empty.type = SensorType::MPU6050;
    empty.id = 255;
    empty.value = 0.0f;
    empty.velocity = 0.0f;
    empty.timestamp = 0;
    empty.triggered = false;
    return empty;
}

bool SensorManager::isSensorTriggered(uint8_t id) {
    SensorData data = getSensorData(id);
    return data.triggered;
}

void SensorManager::setThreshold(uint8_t id, float threshold) {
    for (uint8_t i = 0; i < sensorCount; i++) {
        if (sensors[i].id == id) {
            // Threshold stored per sensor type driver
            switch (sensors[i].type) {
                case SensorType::PIEZO:
                    PiezoDriver::setThreshold(id, threshold);
                    break;
                case SensorType::IR:
                    IRDriver::setThreshold(id, threshold);
                    break;
                case SensorType::PRESSURE:
                    PressureDriver::setThreshold(id, threshold);
                    break;
                case SensorType::FLEX:
                    FlexDriver::setThreshold(id, threshold);
                    break;
                default:
                    break;
            }
            break;
        }
    }
}

float SensorManager::getThreshold(uint8_t id) {
    SensorData data = getSensorData(id);
    switch (data.type) {
        case SensorType::PIEZO:
            return PiezoDriver::getThreshold(id);
        case SensorType::IR:
            return IRDriver::getThreshold(id);
        case SensorType::PRESSURE:
            return PressureDriver::getThreshold(id);
        case SensorType::FLEX:
            return FlexDriver::getThreshold(id);
        default:
            return 0.0f;
    }
}

uint8_t SensorManager::getSensorCount() {
    return sensorCount;
}

SensorData* SensorManager::getAllSensorData() {
    return sensors;
}

void SensorManager::pollSensor(uint8_t index) {
    if (index >= sensorCount) {
        return;
    }
    
    SensorData& sensor = sensors[index];
    sensor.timestamp = millis();
    
    switch (sensor.type) {
        case SensorType::MPU6050:
            sensor.value = MPU6050Driver::readAccel();
            sensor.velocity = MPU6050Driver::readGyro();
            break;
        case SensorType::PIEZO:
            sensor.value = PiezoDriver::read(sensor.id);
            sensor.velocity = PiezoDriver::getVelocity(sensor.id);
            sensor.triggered = PiezoDriver::isTriggered(sensor.id);
            break;
        case SensorType::IR:
            sensor.value = IRDriver::read(sensor.id) ? 1.0f : 0.0f;
            sensor.triggered = IRDriver::isTriggered(sensor.id);
            break;
        case SensorType::PRESSURE:
            sensor.value = PressureDriver::read(sensor.id);
            sensor.velocity = PressureDriver::getVelocity(sensor.id);
            sensor.triggered = PressureDriver::isTriggered(sensor.id);
            break;
        case SensorType::FLEX:
            sensor.value = FlexDriver::read(sensor.id);
            sensor.triggered = FlexDriver::isTriggered(sensor.id);
            break;
    }
}

} // namespace Sensors
} // namespace BITS
