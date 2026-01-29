/*
 * B.I.T.E.S - Sensor Tests
 * 
 * Unit tests for sensor drivers and sensor manager
 */

#include <Arduino.h>
#include "sensors/sensor_manager.h"
#include "sensors/mpu6050_driver.h"
#include "sensors/piezo_driver.h"
#include "core/logger.h"

using namespace BITS::Sensors;
using namespace BITS::Core;

void testMPU6050() {
    Logger::info("Testing MPU6050...");
    
    if (!MPU6050Driver::init()) {
        Logger::error("MPU6050 initialization failed");
        return;
    }
    
    delay(100);
    
    if (!MPU6050Driver::isConnected()) {
        Logger::error("MPU6050 not connected");
        return;
    }
    
    MPU6050Data data = MPU6050Driver::readAll();
    Logger::info("MPU6050 data: Accel(%.2f, %.2f, %.2f) Gyro(%.2f, %.2f, %.2f)",
                data.accelX, data.accelY, data.accelZ,
                data.gyroX, data.gyroY, data.gyroZ);
    
    Logger::info("MPU6050 test passed");
}

void testPiezoSensor() {
    Logger::info("Testing Piezo sensor...");
    
    PiezoDriver::init();
    if (!PiezoDriver::initSensor(0, A0)) {
        Logger::error("Piezo sensor initialization failed");
        return;
    }
    
    delay(100);
    
    float value = PiezoDriver::read(0);
    Logger::info("Piezo sensor value: %.2f mV", value);
    
    Logger::info("Piezo sensor test passed");
}

void testSensorManager() {
    Logger::info("Testing Sensor Manager...");
    
    SensorManager::init();
    
    // Register sensors
    SensorManager::registerSensor(SensorType::MPU6050, 0, 0);
    SensorManager::registerSensor(SensorType::PIEZO, 1, A0);
    
    delay(100);
    
    // Update sensors
    SensorManager::update();
    
    // Get sensor data
    SensorData data = SensorManager::getSensorData(1);
    Logger::info("Sensor data: value=%.2f, velocity=%.2f", data.value, data.velocity);
    
    Logger::info("Sensor Manager test passed");
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Logger::init();
    Logger::info("=== Sensor Tests ===");
    
    testMPU6050();
    delay(1000);
    
    testPiezoSensor();
    delay(1000);
    
    testSensorManager();
    
    Logger::info("=== All Tests Complete ===");
}

void loop() {
    delay(1000);
}
