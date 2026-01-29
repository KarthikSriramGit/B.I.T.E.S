#ifndef BITS_SENSORS_MPU6050_DRIVER_H
#define BITS_SENSORS_MPU6050_DRIVER_H

#include <Wire.h>

namespace BITS {
namespace Sensors {

struct MPU6050Data {
    float accelX, accelY, accelZ;
    float gyroX, gyroY, gyroZ;
    float temperature;
    float roll, pitch, yaw;
};

class MPU6050Driver {
public:
    static bool init(uint8_t address = 0x68);
    static void calibrate();
    static bool isConnected();
    
    static float readAccel();
    static float readGyro();
    static MPU6050Data readAll();
    
    static float getRoll();
    static float getPitch();
    static float getYaw();
    
    static void setAccelRange(uint8_t range);
    static void setGyroRange(uint8_t range);

private:
    static bool initialized;
    static uint8_t deviceAddress;
    static MPU6050Data lastReading;
    
    static void update();
    static int16_t readRegister(uint8_t reg);
    static void writeRegister(uint8_t reg, uint8_t value);
};

} // namespace Sensors
} // namespace BITS

#endif // BITS_SENSORS_MPU6050_DRIVER_H
