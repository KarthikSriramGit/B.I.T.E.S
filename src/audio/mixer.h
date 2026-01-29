#ifndef BITS_AUDIO_MIXER_H
#define BITS_AUDIO_MIXER_H

#include <Audio.h>
#include <stdint.h>

namespace BITS {
namespace Audio {

class Mixer {
public:
    static void init();
    static void update();
    
    static void setTrackVolume(uint8_t trackId, float volume);
    static float getTrackVolume(uint8_t trackId);
    static void setMasterVolume(float volume);
    static float getMasterVolume();

private:
    static constexpr uint8_t MAX_TRACKS = 8;
    static AudioMixer4 mixers[MAX_TRACKS / 4];
    static AudioMixer4 finalMixer;
    static float trackVolumes[MAX_TRACKS];
    static float masterVolume;
    static bool initialized;
};

} // namespace Audio
} // namespace BITS

#endif // BITS_AUDIO_MIXER_H
