#include "audio/mixer.h"
#include "core/logger.h"
#include "config.h"

namespace BITS {
namespace Audio {

AudioMixer4 Mixer::mixers[MAX_TRACKS / 4];
AudioMixer4 Mixer::finalMixer;
float Mixer::trackVolumes[MAX_TRACKS];
float Mixer::masterVolume = 1.0f;
bool Mixer::initialized = false;

void Mixer::init() {
    // Initialize track volumes
    for (uint8_t i = 0; i < MAX_TRACKS; i++) {
        trackVolumes[i] = 0.5f;
    }
    
    // Set mixer gains
    for (uint8_t i = 0; i < MAX_TRACKS / 4; i++) {
        mixers[i].gain(0, 0.25f);
        mixers[i].gain(1, 0.25f);
        mixers[i].gain(2, 0.25f);
        mixers[i].gain(3, 0.25f);
    }
    
    finalMixer.gain(0, masterVolume);
    
    initialized = true;
    Logger::info("Mixer initialized");
}

void Mixer::update() {
    // Mixer updates automatically via Audio library
}

void Mixer::setTrackVolume(uint8_t trackId, float volume) {
    if (trackId >= MAX_TRACKS) {
        return;
    }
    
    trackVolumes[trackId] = constrain(volume, 0.0f, 1.0f);
    
    // Update mixer gain
    uint8_t mixerIndex = trackId / 4;
    uint8_t channel = trackId % 4;
    mixers[mixerIndex].gain(channel, trackVolumes[trackId] * 0.25f);
}

float Mixer::getTrackVolume(uint8_t trackId) {
    if (trackId >= MAX_TRACKS) {
        return 0.0f;
    }
    return trackVolumes[trackId];
}

void Mixer::setMasterVolume(float volume) {
    masterVolume = constrain(volume, 0.0f, 1.0f);
    finalMixer.gain(0, masterVolume);
}

float Mixer::getMasterVolume() {
    return masterVolume;
}

} // namespace Audio
} // namespace BITS
