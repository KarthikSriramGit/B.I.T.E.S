#ifndef BITS_AI_PATTERN_LEARNER_H
#define BITS_AI_PATTERN_LEARNER_H

#include <stdint.h>

namespace BITS {
namespace AI {

struct Pattern {
    uint8_t notes[32];
    uint32_t timings[32];
    uint8_t noteCount;
    float confidence;
};

class PatternLearner {
public:
    static void init();
    static void update();
    
    static void recordNote(uint8_t note, uint32_t timing);
    static Pattern recognizePattern();
    static Pattern getLearnedPattern(uint8_t patternId);
    
    static void startLearning();
    static void stopLearning();
    static bool isLearning();

private:
    static bool initialized;
    static bool learning;
    static Pattern currentPattern;
    static Pattern learnedPatterns[8];
    static uint8_t learnedPatternCount;
    
    static float patternSimilarity(const Pattern& p1, const Pattern& p2);
};

} // namespace AI
} // namespace BITS

#endif // BITS_AI_PATTERN_LEARNER_H
