#include "ai/tempo_detector.h"
#include "ai/ai_acceleration.h"
#include "core/logger.h"
#include <Arduino.h>

namespace BITS {
namespace AI {

bool TempoDetector::initialized = false;
float TempoDetector::currentTempo = 120.0f; // BPM
uint32_t TempoDetector::beatTimes[16];
uint8_t TempoDetector::beatCount = 0;
uint32_t TempoDetector::lastBeatTime = 0;

void TempoDetector::init() {
    initialized = true;
    currentTempo = 120.0f;
    beatCount = 0;
    lastBeatTime = 0;
    Logger::info("Tempo detector initialized");
}

void TempoDetector::update() {
    // Tempo detection runs on beat events
}

float TempoDetector::detectTempo() {
    if (beatCount < 2) {
        return currentTempo;
    }
    
    currentTempo = calculateTempo();
    return currentTempo;
}

float TempoDetector::getCurrentTempo() {
    return currentTempo;
}

uint32_t TempoDetector::getBeatInterval() {
    if (currentTempo == 0.0f) {
        return 0;
    }
    // Convert BPM to milliseconds
    return static_cast<uint32_t>(60000.0f / currentTempo);
}

void TempoDetector::recordBeat() {
    uint32_t now = millis();
    
    if (beatCount >= 16) {
        // Shift array
        for (uint8_t i = 0; i < 15; i++) {
            beatTimes[i] = beatTimes[i + 1];
        }
        beatCount = 15;
    }
    
    beatTimes[beatCount++] = now;
    lastBeatTime = now;
    
    // Update tempo
    detectTempo();
}

void TempoDetector::reset() {
    beatCount = 0;
    lastBeatTime = 0;
    currentTempo = 120.0f;
}

float TempoDetector::calculateTempo() {
    if (beatCount < 2) {
        return currentTempo;
    }
    
    // Calculate average interval between beats
    uint32_t totalInterval = 0;
    uint8_t intervals = 0;
    
    for (uint8_t i = 1; i < beatCount; i++) {
        uint32_t interval = beatTimes[i] - beatTimes[i - 1];
        if (interval > 200 && interval < 2000) { // Valid range: 30-300 BPM
            totalInterval += interval;
            intervals++;
        }
    }
    
    if (intervals == 0) {
        return currentTempo;
    }
    
    float avgInterval = static_cast<float>(totalInterval) / intervals;
    float tempo = 60000.0f / avgInterval; // Convert to BPM
    
    return tempo;
}

} // namespace AI
} // namespace BITS
