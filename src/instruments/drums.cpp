#include "instruments/drums.h"
#include "audio/audio_manager.h"
#include "sensors/sensor_manager.h"
#include "core/logger.h"
#include <Arduino.h>

namespace BITS {
namespace Instruments {

Drums::Drums() 
    : BaseInstrument(InstrumentType::DRUMS), padCount(8) {
    for (uint8_t i = 0; i < MAX_PADS; i++) {
        padSensors[i] = 0;
        padNotes[i] = 36 + i; // MIDI notes starting from C2
    }
}

void Drums::init() {
    Logger::info("Initializing Drums");
    
    // Register piezo sensors for drum pads
    for (uint8_t i = 0; i < padCount; i++) {
        uint8_t gpio = A0 + i; // Default GPIOs
        SensorManager::registerSensor(SensorType::PIEZO, i, gpio);
        padSensors[i] = i;
    }
    
    // Register MPU6050 for strike velocity and angle
    SensorManager::registerSensor(SensorType::MPU6050, padCount, 0);
    
    // Set pad notes (standard MIDI drum mapping)
    padNotes[static_cast<uint8_t>(DrumPad::KICK)] = 36;    // C2
    padNotes[static_cast<uint8_t>(DrumPad::SNARE)] = 38;   // D2
    padNotes[static_cast<uint8_t>(DrumPad::HI_HAT)] = 42;  // F#2
    padNotes[static_cast<uint8_t>(DrumPad::TOM1)] = 48;    // C3
    padNotes[static_cast<uint8_t>(DrumPad::TOM2)] = 45;    // A2
    padNotes[static_cast<uint8_t>(DrumPad::TOM3)] = 41;    // F2
    padNotes[static_cast<uint8_t>(DrumPad::CRASH)] = 49;   // C#3
    padNotes[static_cast<uint8_t>(DrumPad::RIDE)] = 51;    // D#3
    
    active = true;
    Logger::info("Drums initialized with %d pads", padCount);
}

void Drums::update() {
    if (!active) {
        return;
    }
    
    // Check for triggered sensors
    for (uint8_t i = 0; i < padCount; i++) {
        if (SensorManager::isSensorTriggered(padSensors[i])) {
            SensorData data = SensorManager::getSensorData(padSensors[i]);
            handleSensorInput(padSensors[i], data.value, data.velocity);
        }
    }
}

void Drums::handleSensorInput(uint8_t sensorId, float value, float velocity) {
    // Map sensor to pad
    uint8_t padIndex = 255;
    for (uint8_t i = 0; i < padCount; i++) {
        if (padSensors[i] == sensorId) {
            padIndex = i;
            break;
        }
    }
    
    if (padIndex >= padCount) {
        return;
    }
    
    // Get note for this pad
    uint8_t noteId = padNotes[padIndex];
    
    // Calculate velocity from sensor value
    float normalizedVelocity = constrain(velocity / 2000.0f, 0.0f, 1.0f);
    
    // Play note (drums are typically one-shot samples)
    AudioManager::playNote(trackId, noteId, normalizedVelocity);
}

void Drums::setPadCount(uint8_t count) {
    padCount = constrain(count, 1, MAX_PADS);
}

} // namespace Instruments
} // namespace BITS
