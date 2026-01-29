#include "ai/pitch_corrector.h"
#include "ai/ai_acceleration.h"
#include "core/logger.h"
#include <math.h>

namespace BITS {
namespace AI {

bool PitchCorrector::initialized = false;
float PitchCorrector::correctionFactor = 1.0f;

void PitchCorrector::init() {
    initialized = true;
    correctionFactor = 1.0f;
    Logger::info("Pitch corrector initialized");
}

void PitchCorrector::update() {
    // Pitch correction runs on audio processing
}

float PitchCorrector::detectPitch(const float* audioBuffer, uint32_t length) {
    if (length < 2) {
        return 0.0f;
    }
    
    // Find fundamental frequency using autocorrelation
    float maxCorrelation = 0.0f;
    uint32_t bestLag = 0;
    
    // Search for period (lag) with highest autocorrelation
    for (uint32_t lag = 20; lag < length / 2; lag++) {
        float correlation = autocorrelation(audioBuffer, length, lag);
        if (correlation > maxCorrelation) {
            maxCorrelation = correlation;
            bestLag = lag;
        }
    }
    
    if (bestLag == 0) {
        return 0.0f;
    }
    
    // Convert lag to frequency (assuming 44.1kHz sample rate)
    float frequency = 44100.0f / bestLag;
    return frequency;
}

float PitchCorrector::correctPitch(float detectedPitch, float targetPitch) {
    if (detectedPitch == 0.0f || targetPitch == 0.0f) {
        correctionFactor = 1.0f;
        return correctionFactor;
    }
    
    // Calculate correction factor
    correctionFactor = targetPitch / detectedPitch;
    
    // Limit correction range
    if (correctionFactor < 0.5f) correctionFactor = 0.5f;
    if (correctionFactor > 2.0f) correctionFactor = 2.0f;
    
    return correctionFactor;
}

float PitchCorrector::getCorrectionFactor() {
    return correctionFactor;
}

float PitchCorrector::autocorrelation(const float* buffer, uint32_t length, uint32_t lag) {
    if (lag >= length) {
        return 0.0f;
    }
    
    // Use hardware-accelerated dot product
    float sum;
    uint32_t count = length - lag;
    arm_dot_prod_f32(buffer, buffer + lag, count, &sum);
    
    return sum / count;
}

} // namespace AI
} // namespace BITS
