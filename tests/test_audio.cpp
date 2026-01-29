/*
 * B.I.T.E.S - Audio Tests
 * 
 * Unit tests for audio system
 */

#include <Arduino.h>
#include "audio/audio_manager.h"
#include "core/logger.h"

using namespace BITS::Audio;
using namespace BITS::Core;

void testAudioManager() {
    Logger::info("Testing Audio Manager...");
    
    AudioManager::init();
    
    delay(100);
    
    // Test volume control
    AudioManager::setVolume(0.5f);
    float volume = AudioManager::getVolume();
    if (abs(volume - 0.5f) > 0.01f) {
        Logger::error("Volume test failed: expected 0.5, got %.2f", volume);
        return;
    }
    
    Logger::info("Audio Manager test passed");
}

void testSampleManager() {
    Logger::info("Testing Sample Manager...");
    
    // Note: Would test sample loading and playback
    // Requires actual audio samples
    
    Logger::info("Sample Manager test skipped (requires samples)");
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Logger::init();
    Logger::info("=== Audio Tests ===");
    
    testAudioManager();
    delay(1000);
    
    testSampleManager();
    
    Logger::info("=== All Tests Complete ===");
}

void loop() {
    delay(1000);
}
