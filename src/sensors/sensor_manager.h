#ifndef BITS_SENSORS_SENSOR_MANAGER_H
#define BITS_SENSORS_SENSOR_MANAGER_H

#include <stdint.h>

namespace BITS {
namespace Sensors {

enum class SensorType {
    MPU6050 = 0,
    PIEZO = 1,
    IR = 2,
    PRESSURE = 3,
    FLEX = 4
};

struct SensorData {
    SensorType type;
    uint8_t id;
    float value;
    float velocity;
    uint32_t timestamp;
    bool triggered;
};

class SensorManager {
public:
    static void init();
    static void update();
    static void calibrate();
    
    static bool registerSensor(SensorType type, uint8_t id, uint8_t gpio);
    static bool unregisterSensor(uint8_t id);
    static SensorData getSensorData(uint8_t id);
    static bool isSensorTriggered(uint8_t id);
    
    static void setThreshold(uint8_t id, float threshold);
    static float getThreshold(uint8_t id);
    
    static uint8_t getSensorCount();
    static SensorData* getAllSensorData();

private:
    static constexpr uint8_t MAX_SENSORS = 16;
    static SensorData sensors[MAX_SENSORS];
    static uint8_t sensorCount;
    static bool initialized;
    
    static void pollSensor(uint8_t id);
};

} // namespace Sensors
} // namespace BITS

#endif // BITS_SENSORS_SENSOR_MANAGER_H
