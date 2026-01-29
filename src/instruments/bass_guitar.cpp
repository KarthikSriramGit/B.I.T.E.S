#include "instruments/bass_guitar.h"
#include "audio/audio_manager.h"
#include "sensors/sensor_manager.h"
#include "core/logger.h"
#include <Arduino.h>

namespace BITS {
namespace Instruments {

BassGuitar::BassGuitar() 
    : BaseInstrument(InstrumentType::BASS_GUITAR), stringCount(4) {
    for (uint8_t i = 0; i < MAX_STRINGS; i++) {
        stringSensors[i] = 0;
        lastVelocities[i] = 0.0f;
    }
}

void BassGuitar::init() {
    Logger::info("Initializing Bass Guitar");
    
    // Register piezo sensors for strings
    for (uint8_t i = 0; i < stringCount; i++) {
        uint8_t gpio = A0 + i; // Default GPIOs
        SensorManager::registerSensor(SensorType::PIEZO, i, gpio);
        stringSensors[i] = i;
    }
    
    // Register MPU6050 for gesture detection
    SensorManager::registerSensor(SensorType::MPU6050, stringCount, 0);
    
    active = true;
    Logger::info("Bass Guitar initialized with %d strings", stringCount);
}

void BassGuitar::update() {
    if (!active) {
        return;
    }
    
    // Check for triggered sensors
    for (uint8_t i = 0; i < stringCount; i++) {
        if (SensorManager::isSensorTriggered(stringSensors[i])) {
            SensorData data = SensorManager::getSensorData(stringSensors[i]);
            handleSensorInput(stringSensors[i], data.value, data.velocity);
        }
    }
}

void BassGuitar::handleSensorInput(uint8_t sensorId, float value, float velocity) {
    // Map sensor to string
    uint8_t stringIndex = 255;
    for (uint8_t i = 0; i < stringCount; i++) {
        if (stringSensors[i] == sensorId) {
            stringIndex = i;
            break;
        }
    }
    
    if (stringIndex >= stringCount) {
        return;
    }
    
    // Map string to note (E0, A0, D1, G1 for 4-string bass)
    uint8_t notes[] = {40, 45, 50, 55}; // MIDI note numbers
    uint8_t noteId = notes[stringIndex];
    
    // Calculate velocity from sensor value
    float normalizedVelocity = constrain(velocity / 1000.0f, 0.0f, 1.0f);
    
    // Play note
    AudioManager::playNote(trackId, noteId, normalizedVelocity);
    
    lastVelocities[stringIndex] = velocity;
}

void BassGuitar::setStringCount(uint8_t count) {
    stringCount = constrain(count, 1, MAX_STRINGS);
}

} // namespace Instruments
} // namespace BITS
