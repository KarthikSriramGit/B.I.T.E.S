#ifndef BITS_AUDIO_EFFECTS_PROCESSOR_H
#define BITS_AUDIO_EFFECTS_PROCESSOR_H

#include <Audio.h>
#include <stdint.h>

namespace BITS {
namespace Audio {

enum class EffectType {
    NONE = 0,
    REVERB = 1,
    DELAY = 2,
    DISTORTION = 3,
    EQ = 4
};

class EffectsProcessor {
public:
    static void init();
    static void update();
    
    static void setEffect(uint8_t trackId, EffectType type);
    static EffectType getEffect(uint8_t trackId);
    
    static void setReverbRoomSize(float size);
    static void setDelayTime(float timeMs);
    static void setDistortionAmount(float amount);
    static void setEQBand(uint8_t band, float gain);

private:
    static constexpr uint8_t MAX_TRACKS = 8;
    static AudioEffectReverb reverb;
    static AudioEffectDelay delay;
    static AudioEffectFreeverb freeverb;
    static EffectType trackEffects[MAX_TRACKS];
    static bool initialized;
};

} // namespace Audio
} // namespace BITS

#endif // BITS_AUDIO_EFFECTS_PROCESSOR_H
