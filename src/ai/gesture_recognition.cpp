#include "ai/gesture_recognition.h"
#include "ai/ai_acceleration.h"
#include "sensors/mpu6050_driver.h"
#include "sensors/sensor_fusion.h"
#include "core/logger.h"
#include <math.h>
#include <arm_math.h>

namespace BITS {
namespace AI {

bool GestureRecognition::initialized = false;
bool GestureRecognition::training = false;
GestureType GestureRecognition::lastGesture = GestureType::NONE;
float GestureRecognition::lastConfidence = 0.0f;

void GestureRecognition::init() {
    initialized = true;
    training = false;
    lastGesture = GestureType::NONE;
    lastConfidence = 0.0f;
    Logger::info("Gesture recognition initialized");
}

void GestureRecognition::update() {
    if (!initialized) {
        return;
    }
    
    // Extract features from sensor data
    float features[10];
    extractFeatures(features);
    
    // Classify gesture
    lastGesture = classifyGesture(features);
    
    // Calculate confidence (simplified)
    lastConfidence = 0.8f; // Placeholder
}

GestureType GestureRecognition::recognizeGesture() {
    return lastGesture;
}

float GestureRecognition::getConfidence() {
    return lastConfidence;
}

void GestureRecognition::startTraining() {
    training = true;
    Logger::info("Gesture training started");
}

void GestureRecognition::stopTraining() {
    training = false;
    Logger::info("Gesture training stopped");
}

bool GestureRecognition::isTraining() {
    return training;
}

void GestureRecognition::extractFeatures(float* features) {
    // Get sensor data
    MPU6050Data data = MPU6050Driver::readAll();
    
    // Extract features:
    // 0-2: Acceleration (X, Y, Z)
    features[0] = data.accelX;
    features[1] = data.accelY;
    features[2] = data.accelZ;
    
    // 3-5: Angular velocity (X, Y, Z)
    features[3] = data.gyroX;
    features[4] = data.gyroY;
    features[5] = data.gyroZ;
    
    // 6-7: Orientation (roll, pitch)
    features[6] = data.roll;
    features[7] = data.pitch;
    
    // 8: Acceleration magnitude (using optimized vector magnitude)
    float accelVec[3] = {data.accelX, data.accelY, data.accelZ};
    arm_power_f32(accelVec, 3, &features[8]);
    features[8] = sqrtf(features[8]);
    
    // 9: Angular velocity magnitude (using optimized vector magnitude)
    float gyroVec[3] = {data.gyroX, data.gyroY, data.gyroZ};
    arm_power_f32(gyroVec, 3, &features[9]);
    features[9] = sqrtf(features[9]);
}

GestureType GestureRecognition::classifyGesture(const float* features) {
    // Simplified gesture classification
    // In production, use TensorFlow Lite Micro model
    
    float accelMag = features[8];
    float gyroMag = features[9];
    
    // Simple threshold-based classification
    if (accelMag > 2.0f && gyroMag > 100.0f) {
        // High acceleration and rotation = strum
        if (features[1] > 0) {
            return GestureType::STRUM_DOWN;
        } else {
            return GestureType::STRUM_UP;
        }
    } else if (accelMag > 1.5f && gyroMag < 50.0f) {
        // High acceleration, low rotation = strike
        return GestureType::STRIKE;
    } else if (accelMag > 0.5f && gyroMag < 20.0f) {
        // Moderate acceleration, very low rotation = pick
        return GestureType::PICK;
    }
    
    return GestureType::NONE;
}

} // namespace AI
} // namespace BITS
