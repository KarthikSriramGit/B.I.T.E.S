#ifndef BITS_AI_TEMPO_DETECTOR_H
#define BITS_AI_TEMPO_DETECTOR_H

#include <stdint.h>

namespace BITS {
namespace AI {

class TempoDetector {
public:
    static void init();
    static void update();
    
    static float detectTempo();
    static float getCurrentTempo();
    static uint32_t getBeatInterval();
    
    static void recordBeat();
    static void reset();

private:
    static bool initialized;
    static float currentTempo;
    static uint32_t beatTimes[16];
    static uint8_t beatCount;
    static uint32_t lastBeatTime;
    
    static float calculateTempo();
};

} // namespace AI
} // namespace BITS

#endif // BITS_AI_TEMPO_DETECTOR_H
