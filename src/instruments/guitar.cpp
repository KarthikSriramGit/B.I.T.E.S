#include "instruments/guitar.h"
#include "audio/audio_manager.h"
#include "sensors/sensor_manager.h"
#include "core/logger.h"
#include <Arduino.h>

namespace BITS {
namespace Instruments {

Guitar::Guitar() 
    : BaseInstrument(InstrumentType::GUITAR), stringCount(6) {
    for (uint8_t i = 0; i < MAX_STRINGS; i++) {
        stringSensors[i] = 0;
        fretSensors[i] = 0;
    }
}

void Guitar::init() {
    Logger::info("Initializing Guitar");
    
    // Register IR sensors for strings
    for (uint8_t i = 0; i < stringCount; i++) {
        uint8_t gpio = 2 + i; // Default GPIOs
        SensorManager::registerSensor(SensorType::IR, i, gpio);
        stringSensors[i] = i;
    }
    
    // Register pressure sensors for frets (simplified)
    // In production, implement proper fret detection
    
    // Register MPU6050 for picking/strumming detection
    SensorManager::registerSensor(SensorType::MPU6050, stringCount, 0);
    
    active = true;
    Logger::info("Guitar initialized with %d strings", stringCount);
}

void Guitar::update() {
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

void Guitar::handleSensorInput(uint8_t sensorId, float value, float velocity) {
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
    
    // Map string to note (E2, A2, D3, G3, B3, E4 for standard tuning)
    uint8_t notes[] = {40, 45, 50, 55, 59, 64}; // MIDI note numbers
    uint8_t noteId = notes[stringIndex];
    
    // Add fret offset (simplified - in production, read from pressure sensors)
    noteId += 0; // Fret offset
    
    // Play note
    AudioManager::playNote(trackId, noteId, 1.0f);
}

void Guitar::setStringCount(uint8_t count) {
    stringCount = constrain(count, 1, MAX_STRINGS);
}

} // namespace Instruments
} // namespace BITS
