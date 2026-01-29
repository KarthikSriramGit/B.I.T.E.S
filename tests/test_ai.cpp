/*
 * B.I.T.E.S - AI Tests
 * 
 * Unit tests for AI features
 */

#include <Arduino.h>
#include "ai/ai_engine.h"
#include "ai/gesture_recognition.h"
#include "ai/chord_predictor.h"
#include "ai/tempo_detector.h"
#include "core/logger.h"

using namespace BITS::AI;
using namespace BITS::Core;

void testGestureRecognition() {
    Logger::info("Testing Gesture Recognition...");
    
    GestureRecognition::init();
    GestureRecognition::update();
    
    GestureType gesture = GestureRecognition::recognizeGesture();
    float confidence = GestureRecognition::getConfidence();
    
    Logger::info("Gesture: %d, Confidence: %.2f", static_cast<int>(gesture), confidence);
    Logger::info("Gesture Recognition test passed");
}

void testChordPredictor() {
    Logger::info("Testing Chord Predictor...");
    
    ChordPredictor::init();
    
    // Add some notes
    ChordPredictor::addPlayedNote(60); // C
    ChordPredictor::addPlayedNote(64); // E
    ChordPredictor::addPlayedNote(67); // G
    
    ChordPredictor::update();
    Chord chord = ChordPredictor::getCurrentChord();
    
    Logger::info("Predicted chord: root=%d, quality=%d, confidence=%.2f",
                chord.root, chord.quality, chord.confidence);
    Logger::info("Chord Predictor test passed");
}

void testTempoDetector() {
    Logger::info("Testing Tempo Detector...");
    
    TempoDetector::init();
    
    // Simulate beats
    for (int i = 0; i < 4; i++) {
        TempoDetector::recordBeat();
        delay(500); // 120 BPM
    }
    
    float tempo = TempoDetector::getCurrentTempo();
    Logger::info("Detected tempo: %.1f BPM", tempo);
    Logger::info("Tempo Detector test passed");
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Logger::init();
    Logger::info("=== AI Tests ===");
    
    AIEngine::init();
    
    testGestureRecognition();
    delay(1000);
    
    testChordPredictor();
    delay(1000);
    
    testTempoDetector();
    
    Logger::info("=== All Tests Complete ===");
}

void loop() {
    delay(1000);
}
