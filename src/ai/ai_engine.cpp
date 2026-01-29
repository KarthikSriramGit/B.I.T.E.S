#include "ai/ai_engine.h"
#include "ai/ai_acceleration.h"
#include "ai/gesture_recognition.h"
#include "ai/chord_predictor.h"
#include "ai/tempo_detector.h"
#include "ai/pitch_corrector.h"
#include "ai/pattern_learner.h"
#include "core/logger.h"

namespace BITS {
namespace AI {

bool AIEngine::initialized = false;
bool AIEngine::gestureRecognitionEnabled = false;
bool AIEngine::chordPredictionEnabled = false;
bool AIEngine::tempoDetectionEnabled = false;
bool AIEngine::pitchCorrectionEnabled = false;
bool AIEngine::patternLearningEnabled = false;

void AIEngine::init() {
    if (initialized) {
        Logger::warning("AI engine already initialized");
        return;
    }
    
    // Initialize AI acceleration (hardware optimizations)
    AIAcceleration::init();
    
    // Initialize AI components
    GestureRecognition::init();
    ChordPredictor::init();
    TempoDetector::init();
    PitchCorrector::init();
    PatternLearner::init();
    
    initialized = true;
    Logger::info("AI engine initialized with hardware acceleration");
}

void AIEngine::update() {
    if (!initialized) {
        return;
    }
    
    if (gestureRecognitionEnabled) {
        GestureRecognition::update();
    }
    
    if (chordPredictionEnabled) {
        ChordPredictor::update();
    }
    
    if (tempoDetectionEnabled) {
        TempoDetector::update();
    }
    
    if (pitchCorrectionEnabled) {
        PitchCorrector::update();
    }
    
    if (patternLearningEnabled) {
        PatternLearner::update();
    }
}

void AIEngine::enableGestureRecognition(bool enable) {
    gestureRecognitionEnabled = enable;
    Logger::info("Gesture recognition %s", enable ? "enabled" : "disabled");
}

void AIEngine::enableChordPrediction(bool enable) {
    chordPredictionEnabled = enable;
    Logger::info("Chord prediction %s", enable ? "enabled" : "disabled");
}

void AIEngine::enableTempoDetection(bool enable) {
    tempoDetectionEnabled = enable;
    Logger::info("Tempo detection %s", enable ? "enabled" : "disabled");
}

void AIEngine::enablePitchCorrection(bool enable) {
    pitchCorrectionEnabled = enable;
    Logger::info("Pitch correction %s", enable ? "enabled" : "disabled");
}

void AIEngine::enablePatternLearning(bool enable) {
    patternLearningEnabled = enable;
    Logger::info("Pattern learning %s", enable ? "enabled" : "disabled");
}

bool AIEngine::isGestureRecognitionEnabled() {
    return gestureRecognitionEnabled;
}

bool AIEngine::isChordPredictionEnabled() {
    return chordPredictionEnabled;
}

bool AIEngine::isTempoDetectionEnabled() {
    return tempoDetectionEnabled;
}

bool AIEngine::isPitchCorrectionEnabled() {
    return pitchCorrectionEnabled;
}

bool AIEngine::isPatternLearningEnabled() {
    return patternLearningEnabled;
}

} // namespace AI
} // namespace BITS
