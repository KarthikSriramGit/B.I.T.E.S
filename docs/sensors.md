# Sensor Integration Guide

## Supported Sensors

1. **MPU6050**: Accelerometer/Gyroscope
2. **Piezoelectric**: Impact detection
3. **IR Sensors**: Non-contact detection
4. **Pressure Sensors**: Force measurement
5. **Flex Sensors**: Bending detection

## Sensor Registration

```cpp
SensorManager::registerSensor(SensorType::PIEZO, sensorId, gpio);
```

## Reading Sensor Data

```cpp
SensorData data = SensorManager::getSensorData(sensorId);
float value = data.value;
float velocity = data.velocity;
bool triggered = data.triggered;
```

## Sensor Fusion

Kalman filtering is applied automatically for MPU6050 data:
```cpp
float roll = SensorFusion::getRoll();
float pitch = SensorFusion::getPitch();
```

## Calibration

Each sensor type has calibration procedures. See individual driver documentation.
