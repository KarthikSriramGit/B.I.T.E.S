#ifndef BITS_AUDIO_AUDIO_MANAGER_H
#define BITS_AUDIO_AUDIO_MANAGER_H

#include <Audio.h>
#include <stdint.h>

namespace BITS {
namespace Audio {

class AudioManager {
public:
    static void init();
    static void update();
    
    static bool playNote(uint8_t trackId, uint8_t noteId, float velocity = 1.0f);
    static void stopNote(uint8_t trackId, uint8_t noteId);
    static void stopAll();
    
    static void setVolume(float volume);
    static float getVolume();
    static void setTrackVolume(uint8_t trackId, float volume);
    
    static uint32_t getMemoryUsage();
    static uint32_t getProcessorUsage();

private:
    static bool initialized;
    static float masterVolume;
    static AudioControlSGTL5000 codec;
};

} // namespace Audio
} // namespace BITS

#endif // BITS_AUDIO_AUDIO_MANAGER_H
