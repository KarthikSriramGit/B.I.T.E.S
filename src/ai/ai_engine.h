#ifndef BITS_AI_AI_ENGINE_H
#define BITS_AI_AI_ENGINE_H

#include <stdint.h>

namespace BITS {
namespace AI {

class AIEngine {
public:
    static void init();
    static void update();
    
    static void enableGestureRecognition(bool enable);
    static void enableChordPrediction(bool enable);
    static void enableTempoDetection(bool enable);
    static void enablePitchCorrection(bool enable);
    static void enablePatternLearning(bool enable);
    
    static bool isGestureRecognitionEnabled();
    static bool isChordPredictionEnabled();
    static bool isTempoDetectionEnabled();
    static bool isPitchCorrectionEnabled();
    static bool isPatternLearningEnabled();

private:
    static bool initialized;
    static bool gestureRecognitionEnabled;
    static bool chordPredictionEnabled;
    static bool tempoDetectionEnabled;
    static bool pitchCorrectionEnabled;
    static bool patternLearningEnabled;
};

} // namespace AI
} // namespace BITS

#endif // BITS_AI_AI_ENGINE_H
