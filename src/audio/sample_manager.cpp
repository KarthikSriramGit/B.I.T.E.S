#include "audio/sample_manager.h"
#include "core/logger.h"
#include "config.h"
#include <Arduino.h>

namespace BITS {
namespace Audio {

SampleManager::Voice SampleManager::voices[MAX_VOICES];
uint8_t SampleManager::voiceCount = 0;
bool SampleManager::initialized = false;

void SampleManager::init() {
    // Initialize voices
    for (uint8_t i = 0; i < MAX_VOICES; i++) {
        voices[i].player = new AudioPlayMemory();
        voices[i].active = false;
        voices[i].trackId = 0;
        voices[i].noteId = 0;
        voices[i].startTime = 0;
    }
    
    voiceCount = 0;
    initialized = true;
    Logger::info("Sample manager initialized");
}

bool SampleManager::loadSample(uint8_t trackId, uint8_t noteId, const int16_t* data, uint32_t length) {
    // Note: AudioPlayMemory uses PROGMEM data
    // In production, implement proper sample loading from flash/SD
    return true;
}

bool SampleManager::playNote(uint8_t trackId, uint8_t noteId, float velocity) {
    if (!initialized) {
        return false;
    }
    
    // Check if note is already playing
    Voice* existing = findVoice(trackId, noteId);
    if (existing != nullptr) {
        // Restart note
        existing->startTime = millis();
        return true;
    }
    
    // Allocate new voice
    Voice* voice = allocateVoice(trackId, noteId);
    if (voice == nullptr) {
        Logger::warning("No free voices available");
        return false;
    }
    
    // Play note (simplified - in production, load actual sample data)
    // voice->player->play(...);
    voice->active = true;
    voice->startTime = millis();
    
    return true;
}

void SampleManager::stopNote(uint8_t trackId, uint8_t noteId) {
    Voice* voice = findVoice(trackId, noteId);
    if (voice != nullptr) {
        freeVoice(voice);
    }
}

void SampleManager::stopAll() {
    for (uint8_t i = 0; i < MAX_VOICES; i++) {
        if (voices[i].active) {
            freeVoice(&voices[i]);
        }
    }
}

bool SampleManager::isNotePlaying(uint8_t trackId, uint8_t noteId) {
    Voice* voice = findVoice(trackId, noteId);
    return voice != nullptr && voice->active;
}

uint8_t SampleManager::getActiveVoices(uint8_t trackId) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < MAX_VOICES; i++) {
        if (voices[i].active && voices[i].trackId == trackId) {
            count++;
        }
    }
    return count;
}

SampleManager::Voice* SampleManager::allocateVoice(uint8_t trackId, uint8_t noteId) {
    for (uint8_t i = 0; i < MAX_VOICES; i++) {
        if (!voices[i].active) {
            voices[i].trackId = trackId;
            voices[i].noteId = noteId;
            voices[i].active = true;
            voiceCount++;
            return &voices[i];
        }
    }
    return nullptr;
}

void SampleManager::freeVoice(Voice* voice) {
    if (voice != nullptr && voice->active) {
        voice->active = false;
        voice->trackId = 0;
        voice->noteId = 0;
        voice->startTime = 0;
        voiceCount--;
    }
}

SampleManager::Voice* SampleManager::findVoice(uint8_t trackId, uint8_t noteId) {
    for (uint8_t i = 0; i < MAX_VOICES; i++) {
        if (voices[i].active && 
            voices[i].trackId == trackId && 
            voices[i].noteId == noteId) {
            return &voices[i];
        }
    }
    return nullptr;
}

} // namespace Audio
} // namespace BITS
