#include "audio/audio_manager.h"
#include "audio/sample_manager.h"
#include "audio/mixer.h"
#include "core/logger.h"
#include "config.h"

namespace BITS {
namespace Audio {

bool AudioManager::initialized = false;
float AudioManager::masterVolume = 0.8f;
AudioControlSGTL5000 AudioManager::codec;

void AudioManager::init() {
    if (initialized) {
        Logger::warning("Audio manager already initialized");
        return;
    }
    
    // Allocate audio memory
    AudioMemory(AUDIO_BUFFER_SIZE);
    
    // Initialize codec
    codec.enable();
    codec.volume(masterVolume);
    
    // Initialize sample manager
    SampleManager::init();
    
    // Initialize mixer
    Mixer::init();
    
    initialized = true;
    Logger::info("Audio manager initialized");
    Logger::info("Audio memory: %lu blocks", AudioMemoryUsageMax());
}

void AudioManager::update() {
    if (!initialized) {
        return;
    }
    
    // Update mixer
    Mixer::update();
    
    // Check for audio underruns
    if (AudioProcessorUsageMax() > 90.0f) {
        Logger::warning("High audio processor usage: %.1f%%", AudioProcessorUsageMax());
    }
}

bool AudioManager::playNote(uint8_t trackId, uint8_t noteId, float velocity) {
    if (!initialized) {
        return false;
    }
    
    return SampleManager::playNote(trackId, noteId, velocity);
}

void AudioManager::stopNote(uint8_t trackId, uint8_t noteId) {
    if (!initialized) {
        return;
    }
    
    SampleManager::stopNote(trackId, noteId);
}

void AudioManager::stopAll() {
    if (!initialized) {
        return;
    }
    
    SampleManager::stopAll();
}

void AudioManager::setVolume(float volume) {
    masterVolume = constrain(volume, 0.0f, 1.0f);
    codec.volume(masterVolume);
}

float AudioManager::getVolume() {
    return masterVolume;
}

void AudioManager::setTrackVolume(uint8_t trackId, float volume) {
    Mixer::setTrackVolume(trackId, volume);
}

uint32_t AudioManager::getMemoryUsage() {
    return AudioMemoryUsageMax();
}

uint32_t AudioManager::getProcessorUsage() {
    return static_cast<uint32_t>(AudioProcessorUsageMax());
}

} // namespace Audio
} // namespace BITS
