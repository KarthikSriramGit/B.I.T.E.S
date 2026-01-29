#ifndef BITS_AI_CHORD_PREDICTOR_H
#define BITS_AI_CHORD_PREDICTOR_H

#include <stdint.h>

namespace BITS {
namespace AI {

struct Chord {
    uint8_t root;      // MIDI note (0-11)
    uint8_t quality;   // 0=major, 1=minor, 2=diminished, 3=augmented
    float confidence;
};

class ChordPredictor {
public:
    static void init();
    static void update();
    
    static Chord predictChord(const uint8_t* notes, uint8_t noteCount);
    static Chord getCurrentChord();
    static void addPlayedNote(uint8_t note);
    static void clearPlayedNotes();

private:
    static bool initialized;
    static Chord currentChord;
    static uint8_t playedNotes[16];
    static uint8_t playedNoteCount;
    
    static uint8_t detectKey(const uint8_t* notes, uint8_t noteCount);
    static Chord findChordInKey(uint8_t key, const uint8_t* notes, uint8_t noteCount);
};

} // namespace AI
} // namespace BITS

#endif // BITS_AI_CHORD_PREDICTOR_H
