#ifndef BITS_INSTRUMENTS_DRUMS_H
#define BITS_INSTRUMENTS_DRUMS_H

#include "instruments/base_instrument.h"
#include <stdint.h>

namespace BITS {
namespace Instruments {

enum class DrumPad {
    KICK = 0,
    SNARE = 1,
    HI_HAT = 2,
    TOM1 = 3,
    TOM2 = 4,
    TOM3 = 5,
    CRASH = 6,
    RIDE = 7
};

class Drums : public BaseInstrument {
public:
    Drums();
    virtual void init() override;
    virtual void update() override;
    virtual void handleSensorInput(uint8_t sensorId, float value, float velocity) override;
    
    void setPadCount(uint8_t count);
    uint8_t getPadCount() const { return padCount; }

private:
    static constexpr uint8_t MAX_PADS = 8;
    uint8_t padCount;
    uint8_t padSensors[MAX_PADS];
    uint8_t padNotes[MAX_PADS];
};

} // namespace Instruments
} // namespace BITS

#endif // BITS_INSTRUMENTS_DRUMS_H
