#ifndef BITS_AUDIO_SAMPLE_MANAGER_H
#define BITS_AUDIO_SAMPLE_MANAGER_H

#include <Audio.h>
#include <stdint.h>

namespace BITS {
namespace Audio {

class SampleManager {
public:
    static void init();
    static bool loadSample(uint8_t trackId, uint8_t noteId, const int16_t* data, uint32_t length);
    static bool playNote(uint8_t trackId, uint8_t noteId, float velocity = 1.0f);
    static void stopNote(uint8_t trackId, uint8_t noteId);
    static void stopAll();
    
    static bool isNotePlaying(uint8_t trackId, uint8_t noteId);
    static uint8_t getActiveVoices(uint8_t trackId);

private:
    static constexpr uint8_t MAX_TRACKS = 8;
    static constexpr uint8_t MAX_NOTES_PER_TRACK = 128;
    static constexpr uint8_t MAX_VOICES = 16;
    
    struct Voice {
        AudioPlayMemory* player;
        bool active;
        uint8_t trackId;
        uint8_t noteId;
        uint32_t startTime;
    };
    
    static Voice voices[MAX_VOICES];
    static uint8_t voiceCount;
    static bool initialized;
    
    static Voice* allocateVoice(uint8_t trackId, uint8_t noteId);
    static void freeVoice(Voice* voice);
    static Voice* findVoice(uint8_t trackId, uint8_t noteId);
};

} // namespace Audio
} // namespace BITS

#endif // BITS_AUDIO_SAMPLE_MANAGER_H
