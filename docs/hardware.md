# Hardware Setup Guide

## Required Hardware

- Teensy 4.1 development board
- Audio Shield (optional but recommended)
- MPU6050 accelerometer/gyroscope
- Piezoelectric sensors (for bass/drums)
- IR sensors (for guitar)
- Pressure sensors (for keyboard)
- Flex sensors (optional)

## Wiring Guide

### MPU6050
- VCC → 3.3V
- GND → GND
- SDA → Pin 18
- SCL → Pin 19

### Piezoelectric Sensors
- Positive → Analog pin (A0-A17)
- Negative → GND
- 10kΩ resistor recommended for voltage divider

### IR Sensors
- VCC → 3.3V or 5V
- GND → GND
- OUT → Digital pin (with pull-up)

### Pressure Sensors
- VCC → 5V
- GND → GND
- OUT → Analog pin

## Default GPIO Assignments

See `src/config.h` for default GPIO assignments.

## Calibration

Run sensor calibration after wiring:
```cpp
SensorManager::calibrate();
```
