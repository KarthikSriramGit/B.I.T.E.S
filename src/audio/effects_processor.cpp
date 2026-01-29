#include "audio/effects_processor.h"
#include "core/logger.h"

namespace BITS {
namespace Audio {

AudioEffectReverb EffectsProcessor::reverb;
AudioEffectDelay EffectsProcessor::delay(16000); // 16k samples delay
AudioEffectFreeverb EffectsProcessor::freeverb;
EffectType EffectsProcessor::trackEffects[MAX_TRACKS];
bool EffectsProcessor::initialized = false;

void EffectsProcessor::init() {
    // Initialize effects
    reverb.reverbTime(0.5f);
    freeverb.roomsize(0.5f);
    freeverb.damping(0.5f);
    
    // Initialize track effects
    for (uint8_t i = 0; i < MAX_TRACKS; i++) {
        trackEffects[i] = EffectType::NONE;
    }
    
    initialized = true;
    Logger::info("Effects processor initialized");
}

void EffectsProcessor::update() {
    // Effects update automatically via Audio library
}

void EffectsProcessor::setEffect(uint8_t trackId, EffectType type) {
    if (trackId >= MAX_TRACKS) {
        return;
    }
    trackEffects[trackId] = type;
}

EffectType EffectsProcessor::getEffect(uint8_t trackId) {
    if (trackId >= MAX_TRACKS) {
        return EffectType::NONE;
    }
    return trackEffects[trackId];
}

void EffectsProcessor::setReverbRoomSize(float size) {
    freeverb.roomsize(constrain(size, 0.0f, 1.0f));
}

void EffectsProcessor::setDelayTime(float timeMs) {
    // Convert ms to samples (at 44.1kHz)
    uint32_t samples = static_cast<uint32_t>(timeMs * 44.1f);
    delay.delay(0, constrain(samples, 0u, 16000u));
}

void EffectsProcessor::setDistortionAmount(float amount) {
    // Distortion implementation depends on Audio library version
    // Simplified for now
}

void EffectsProcessor::setEQBand(uint8_t band, float gain) {
    // EQ implementation depends on Audio library version
    // Simplified for now
}

} // namespace Audio
} // namespace BITS
