#ifndef BITS_AI_PITCH_CORRECTOR_H
#define BITS_AI_PITCH_CORRECTOR_H

#include <stdint.h>

namespace BITS {
namespace AI {

class PitchCorrector {
public:
    static void init();
    static void update();
    
    static float detectPitch(const float* audioBuffer, uint32_t length);
    static float correctPitch(float detectedPitch, float targetPitch);
    static float getCorrectionFactor();

private:
    static bool initialized;
    static float correctionFactor;
    
    static float autocorrelation(const float* buffer, uint32_t length, uint32_t lag);
};

} // namespace AI
} // namespace BITS

#endif // BITS_AI_PITCH_CORRECTOR_H
