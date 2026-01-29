#include "ai/chord_predictor.h"
#include "core/logger.h"

namespace BITS {
namespace AI {

bool ChordPredictor::initialized = false;
Chord ChordPredictor::currentChord = {0, 0, 0.0f};
uint8_t ChordPredictor::playedNotes[16];
uint8_t ChordPredictor::playedNoteCount = 0;

void ChordPredictor::init() {
    initialized = true;
    currentChord = {0, 0, 0.0f};
    playedNoteCount = 0;
    Logger::info("Chord predictor initialized");
}

void ChordPredictor::update() {
    if (!initialized || playedNoteCount == 0) {
        return;
    }
    
    // Predict chord from played notes
    currentChord = predictChord(playedNotes, playedNoteCount);
}

Chord ChordPredictor::predictChord(const uint8_t* notes, uint8_t noteCount) {
    if (noteCount == 0) {
        return {0, 0, 0.0f};
    }
    
    // Detect key
    uint8_t key = detectKey(notes, noteCount);
    
    // Find chord in key
    return findChordInKey(key, notes, noteCount);
}

Chord ChordPredictor::getCurrentChord() {
    return currentChord;
}

void ChordPredictor::addPlayedNote(uint8_t note) {
    if (playedNoteCount >= 16) {
        // Remove oldest note
        for (uint8_t i = 0; i < 15; i++) {
            playedNotes[i] = playedNotes[i + 1];
        }
        playedNoteCount = 15;
    }
    
    playedNotes[playedNoteCount++] = note % 12; // Convert to note class
}

void ChordPredictor::clearPlayedNotes() {
    playedNoteCount = 0;
}

uint8_t ChordPredictor::detectKey(const uint8_t* notes, uint8_t noteCount) {
    // Simplified key detection
    // In production, use music theory algorithms
    
    // Count note occurrences
    uint8_t noteCounts[12] = {0};
    for (uint8_t i = 0; i < noteCount; i++) {
        noteCounts[notes[i] % 12]++;
    }
    
    // Find most common note (simplified)
    uint8_t maxCount = 0;
    uint8_t key = 0;
    for (uint8_t i = 0; i < 12; i++) {
        if (noteCounts[i] > maxCount) {
            maxCount = noteCounts[i];
            key = i;
        }
    }
    
    return key;
}

Chord ChordPredictor::findChordInKey(uint8_t key, const uint8_t* notes, uint8_t noteCount) {
    // Simplified chord detection
    // In production, use proper music theory
    
    Chord chord;
    chord.root = key;
    chord.quality = 0; // Major
    chord.confidence = 0.7f;
    
    // Check for minor third
    bool hasMinorThird = false;
    for (uint8_t i = 0; i < noteCount; i++) {
        if ((notes[i] - key + 12) % 12 == 3) {
            hasMinorThird = true;
            break;
        }
    }
    
    if (hasMinorThird) {
        chord.quality = 1; // Minor
    }
    
    return chord;
}

} // namespace AI
} // namespace BITS
