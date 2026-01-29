#ifndef BITS_SENSORS_SENSOR_FUSION_H
#define BITS_SENSORS_SENSOR_FUSION_H

#include "sensors/mpu6050_driver.h"

namespace BITS {
namespace Sensors {

// Kalman filter for sensor fusion
class KalmanFilter {
public:
    KalmanFilter();
    void init(float processNoise, float measurementNoise);
    float update(float measurement);
    void reset();

private:
    float Q; // Process noise covariance
    float R; // Measurement noise covariance
    float P; // Estimation error covariance
    float X; // State estimate
    float K; // Kalman gain
};

// Sensor fusion manager
class SensorFusion {
public:
    static void init();
    static void update();
    
    // Get fused orientation
    static float getRoll();
    static float getPitch();
    static float getYaw();
    
    // Get fused acceleration
    static float getAccelMagnitude();
    
    // Get fused angular velocity
    static float getGyroMagnitude();

private:
    static KalmanFilter rollFilter;
    static KalmanFilter pitchFilter;
    static KalmanFilter yawFilter;
    static KalmanFilter accelFilter;
    static KalmanFilter gyroFilter;
    static bool initialized;
};

} // namespace Sensors
} // namespace BITS

#endif // BITS_SENSORS_SENSOR_FUSION_H
