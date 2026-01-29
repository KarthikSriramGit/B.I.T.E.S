#ifndef BITS_AI_GESTURE_RECOGNITION_H
#define BITS_AI_GESTURE_RECOGNITION_H

#include <stdint.h>

namespace BITS {
namespace AI {

enum class GestureType {
    NONE = 0,
    STRUM_DOWN = 1,
    STRUM_UP = 2,
    PICK = 3,
    STRIKE = 4
};

class GestureRecognition {
public:
    static void init();
    static void update();
    
    static GestureType recognizeGesture();
    static float getConfidence();
    
    static void startTraining();
    static void stopTraining();
    static bool isTraining();

private:
    static bool initialized;
    static bool training;
    static GestureType lastGesture;
    static float lastConfidence;
    
    // Feature extraction
    static void extractFeatures(float* features);
    static GestureType classifyGesture(const float* features);
};

} // namespace AI
} // namespace BITS

#endif // BITS_AI_GESTURE_RECOGNITION_H
