#include "instruments/keyboard.h"
#include "audio/audio_manager.h"
#include "sensors/sensor_manager.h"
#include "core/logger.h"
#include <Arduino.h>

namespace BITS {
namespace Instruments {

Keyboard::Keyboard() 
    : BaseInstrument(InstrumentType::KEYBOARD), keyCount(61) {
    for (uint8_t i = 0; i < MAX_KEYS; i++) {
        keySensors[i] = 0;
        keyStates[i] = false;
    }
}

void Keyboard::init() {
    Logger::info("Initializing Keyboard");
    
    // Register pressure sensors for keys
    for (uint8_t i = 0; i < keyCount; i++) {
        uint8_t gpio = A10 + i; // Default GPIOs (simplified)
        SensorManager::registerSensor(SensorType::PRESSURE, i, gpio);
        keySensors[i] = i;
    }
    
    // Register MPU6050 for velocity and aftertouch
    SensorManager::registerSensor(SensorType::MPU6050, keyCount, 0);
    
    active = true;
    Logger::info("Keyboard initialized with %d keys", keyCount);
}

void Keyboard::update() {
    if (!active) {
        return;
    }
    
    // Check for triggered sensors
    for (uint8_t i = 0; i < keyCount; i++) {
        bool triggered = SensorManager::isSensorTriggered(keySensors[i]);
        
        if (triggered && !keyStates[i]) {
            // Key pressed
            SensorData data = SensorManager::getSensorData(keySensors[i]);
            handleSensorInput(keySensors[i], data.value, data.velocity);
            keyStates[i] = true;
        } else if (!triggered && keyStates[i]) {
            // Key released
            AudioManager::stopNote(trackId, i + 21); // MIDI note 21 = A0
            keyStates[i] = false;
        }
    }
}

void Keyboard::handleSensorInput(uint8_t sensorId, float value, float velocity) {
    // Map sensor to key
    uint8_t keyIndex = 255;
    for (uint8_t i = 0; i < keyCount; i++) {
        if (keySensors[i] == sensorId) {
            keyIndex = i;
            break;
        }
    }
    
    if (keyIndex >= keyCount) {
        return;
    }
    
    // Map key to MIDI note (A0 = 21, C8 = 108)
    uint8_t noteId = keyIndex + 21;
    
    // Calculate velocity from pressure
    float normalizedVelocity = constrain(value / 5.0f, 0.0f, 1.0f);
    
    // Play note
    AudioManager::playNote(trackId, noteId, normalizedVelocity);
}

void Keyboard::setKeyCount(uint8_t count) {
    keyCount = constrain(count, 1, MAX_KEYS);
}

} // namespace Instruments
} // namespace BITS
