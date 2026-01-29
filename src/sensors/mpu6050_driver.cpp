#include "sensors/mpu6050_driver.h"
#include "core/logger.h"
#include "config.h"
#include <Wire.h>
#include <math.h>
#include <Arduino.h>

namespace BITS {
namespace Sensors {

bool MPU6050Driver::initialized = false;
uint8_t MPU6050Driver::deviceAddress = 0x68;
MPU6050Data MPU6050Driver::lastReading = {0};

bool MPU6050Driver::init(uint8_t address) {
    deviceAddress = address;
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C
    
    // Wake up MPU6050
    writeRegister(0x6B, 0x00);
    delay(100);
    
    // Check connection
    if (!isConnected()) {
        Logger::error("MPU6050 not connected");
        return false;
    }
    
    // Configure accelerometer range (±2g)
    setAccelRange(0);
    
    // Configure gyroscope range (±250°/s)
    setGyroRange(0);
    
    initialized = true;
    Logger::info("MPU6050 initialized");
    return true;
}

void MPU6050Driver::calibrate() {
    Logger::info("Calibrating MPU6050...");
    
    // Read multiple samples and average
    float accelXSum = 0, accelYSum = 0, accelZSum = 0;
    float gyroXSum = 0, gyroYSum = 0, gyroZSum = 0;
    const int samples = 100;
    
    for (int i = 0; i < samples; i++) {
        update();
        accelXSum += lastReading.accelX;
        accelYSum += lastReading.accelY;
        accelZSum += lastReading.accelZ;
        gyroXSum += lastReading.gyroX;
        gyroYSum += lastReading.gyroY;
        gyroZSum += lastReading.gyroZ;
        delay(10);
    }
    
    // Store offsets (simplified - in production use proper calibration)
    Logger::info("MPU6050 calibration complete");
}

bool MPU6050Driver::isConnected() {
    Wire.beginTransmission(deviceAddress);
    return Wire.endTransmission() == 0;
}

float MPU6050Driver::readAccel() {
    update();
    // Return magnitude of acceleration
    return sqrt(lastReading.accelX * lastReading.accelX +
                lastReading.accelY * lastReading.accelY +
                lastReading.accelZ * lastReading.accelZ);
}

float MPU6050Driver::readGyro() {
    update();
    // Return magnitude of angular velocity
    return sqrt(lastReading.gyroX * lastReading.gyroX +
                lastReading.gyroY * lastReading.gyroY +
                lastReading.gyroZ * lastReading.gyroZ);
}

MPU6050Data MPU6050Driver::readAll() {
    update();
    return lastReading;
}

float MPU6050Driver::getRoll() {
    update();
    return lastReading.roll;
}

float MPU6050Driver::getPitch() {
    update();
    return lastReading.pitch;
}

float MPU6050Driver::getYaw() {
    update();
    return lastReading.yaw;
}

void MPU6050Driver::setAccelRange(uint8_t range) {
    uint8_t value = readRegister(0x1C) & 0xE7;
    writeRegister(0x1C, value | (range << 3));
}

void MPU6050Driver::setGyroRange(uint8_t range) {
    uint8_t value = readRegister(0x1B) & 0xE7;
    writeRegister(0x1B, value | (range << 3));
}

void MPU6050Driver::update() {
    Wire.beginTransmission(deviceAddress);
    Wire.write(0x3B); // Start register
    Wire.endTransmission(false);
    Wire.requestFrom(deviceAddress, 14, true);
    
    // Read accelerometer
    int16_t accelX = Wire.read() << 8 | Wire.read();
    int16_t accelY = Wire.read() << 8 | Wire.read();
    int16_t accelZ = Wire.read() << 8 | Wire.read();
    
    // Read temperature
    int16_t temp = Wire.read() << 8 | Wire.read();
    
    // Read gyroscope
    int16_t gyroX = Wire.read() << 8 | Wire.read();
    int16_t gyroY = Wire.read() << 8 | Wire.read();
    int16_t gyroZ = Wire.read() << 8 | Wire.read();
    
    // Convert to physical units (simplified - assumes ±2g and ±250°/s)
    lastReading.accelX = accelX / 16384.0f;
    lastReading.accelY = accelY / 16384.0f;
    lastReading.accelZ = accelZ / 16384.0f;
    lastReading.temperature = temp / 340.0f + 36.53f;
    lastReading.gyroX = gyroX / 131.0f;
    lastReading.gyroY = gyroY / 131.0f;
    lastReading.gyroZ = gyroZ / 131.0f;
    
    // Calculate roll and pitch
    lastReading.roll = atan2(lastReading.accelY, lastReading.accelZ) * 180.0f / PI;
    lastReading.pitch = atan2(-lastReading.accelX, 
                              sqrt(lastReading.accelY * lastReading.accelY + 
                                   lastReading.accelZ * lastReading.accelZ)) * 180.0f / PI;
}

int16_t MPU6050Driver::readRegister(uint8_t reg) {
    Wire.beginTransmission(deviceAddress);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(deviceAddress, 1, true);
    return Wire.read();
}

void MPU6050Driver::writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(deviceAddress);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

} // namespace Sensors
} // namespace BITS
