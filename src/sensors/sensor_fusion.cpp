#include "sensors/sensor_fusion.h"
#include "core/logger.h"
#include <math.h>

namespace BITS {
namespace Sensors {

KalmanFilter::KalmanFilter() : Q(0.1f), R(0.1f), P(1.0f), X(0.0f), K(0.0f) {}

void KalmanFilter::init(float processNoise, float measurementNoise) {
    Q = processNoise;
    R = measurementNoise;
    P = 1.0f;
    X = 0.0f;
    K = 0.0f;
}

float KalmanFilter::update(float measurement) {
    // Prediction step
    P = P + Q;
    
    // Update step
    K = P / (P + R);
    X = X + K * (measurement - X);
    P = (1 - K) * P;
    
    return X;
}

void KalmanFilter::reset() {
    P = 1.0f;
    X = 0.0f;
    K = 0.0f;
}

// Sensor fusion static members
KalmanFilter SensorFusion::rollFilter;
KalmanFilter SensorFusion::pitchFilter;
KalmanFilter SensorFusion::yawFilter;
KalmanFilter SensorFusion::accelFilter;
KalmanFilter SensorFusion::gyroFilter;
bool SensorFusion::initialized = false;

void SensorFusion::init() {
    rollFilter.init(0.1f, 0.1f);
    pitchFilter.init(0.1f, 0.1f);
    yawFilter.init(0.1f, 0.1f);
    accelFilter.init(0.1f, 0.1f);
    gyroFilter.init(0.1f, 0.1f);
    initialized = true;
    Logger::info("Sensor fusion initialized");
}

void SensorFusion::update() {
    if (!initialized) {
        init();
    }
    
    MPU6050Data data = MPU6050Driver::readAll();
    
    // Update filters
    rollFilter.update(data.roll);
    pitchFilter.update(data.pitch);
    yawFilter.update(data.yaw);
    
    float accelMag = sqrt(data.accelX * data.accelX +
                          data.accelY * data.accelY +
                          data.accelZ * data.accelZ);
    accelFilter.update(accelMag);
    
    float gyroMag = sqrt(data.gyroX * data.gyroX +
                         data.gyroY * data.gyroY +
                         data.gyroZ * data.gyroZ);
    gyroFilter.update(gyroMag);
}

float SensorFusion::getRoll() {
    return rollFilter.update(MPU6050Driver::getRoll());
}

float SensorFusion::getPitch() {
    return pitchFilter.update(MPU6050Driver::getPitch());
}

float SensorFusion::getYaw() {
    return yawFilter.update(MPU6050Driver::getYaw());
}

float SensorFusion::getAccelMagnitude() {
    return accelFilter.update(MPU6050Driver::readAccel());
}

float SensorFusion::getGyroMagnitude() {
    return gyroFilter.update(MPU6050Driver::readGyro());
}

} // namespace Sensors
} // namespace BITS
