#include "ai/pattern_learner.h"
#include "core/logger.h"
#include <Arduino.h>

namespace BITS {
namespace AI {

bool PatternLearner::initialized = false;
bool PatternLearner::learning = false;
Pattern PatternLearner::currentPattern = {{0}, {0}, 0, 0.0f};
Pattern PatternLearner::learnedPatterns[8];
uint8_t PatternLearner::learnedPatternCount = 0;

void PatternLearner::init() {
    initialized = true;
    learning = false;
    currentPattern.noteCount = 0;
    learnedPatternCount = 0;
    Logger::info("Pattern learner initialized");
}

void PatternLearner::update() {
    // Pattern learning runs on note events
}

void PatternLearner::recordNote(uint8_t note, uint32_t timing) {
    if (!learning) {
        return;
    }
    
    if (currentPattern.noteCount >= 32) {
        // Pattern full, save and start new
        learnedPatterns[learnedPatternCount % 8] = currentPattern;
        learnedPatternCount++;
        currentPattern.noteCount = 0;
    }
    
    currentPattern.notes[currentPattern.noteCount] = note;
    currentPattern.timings[currentPattern.noteCount] = timing;
    currentPattern.noteCount++;
}

Pattern PatternLearner::recognizePattern() {
    if (currentPattern.noteCount == 0) {
        return {{0}, {0}, 0, 0.0f};
    }
    
    // Find most similar learned pattern
    float maxSimilarity = 0.0f;
    Pattern bestPattern = currentPattern;
    
    for (uint8_t i = 0; i < learnedPatternCount && i < 8; i++) {
        float similarity = patternSimilarity(currentPattern, learnedPatterns[i]);
        if (similarity > maxSimilarity) {
            maxSimilarity = similarity;
            bestPattern = learnedPatterns[i];
            bestPattern.confidence = similarity;
        }
    }
    
    return bestPattern;
}

Pattern PatternLearner::getLearnedPattern(uint8_t patternId) {
    if (patternId >= learnedPatternCount || patternId >= 8) {
        return {{0}, {0}, 0, 0.0f};
    }
    return learnedPatterns[patternId];
}

void PatternLearner::startLearning() {
    learning = true;
    currentPattern.noteCount = 0;
    Logger::info("Pattern learning started");
}

void PatternLearner::stopLearning() {
    learning = false;
    // Save current pattern
    if (currentPattern.noteCount > 0) {
        learnedPatterns[learnedPatternCount % 8] = currentPattern;
        learnedPatternCount++;
    }
    Logger::info("Pattern learning stopped");
}

bool PatternLearner::isLearning() {
    return learning;
}

float PatternLearner::patternSimilarity(const Pattern& p1, const Pattern& p2) {
    if (p1.noteCount == 0 || p2.noteCount == 0) {
        return 0.0f;
    }
    
    // Simple similarity: count matching notes
    uint8_t matches = 0;
    uint8_t maxCount = p1.noteCount < p2.noteCount ? p1.noteCount : p2.noteCount;
    
    for (uint8_t i = 0; i < maxCount; i++) {
        if (p1.notes[i] == p2.notes[i]) {
            matches++;
        }
    }
    
    return static_cast<float>(matches) / maxCount;
}

} // namespace AI
} // namespace BITS
