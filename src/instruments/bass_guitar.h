#ifndef BITS_INSTRUMENTS_BASS_GUITAR_H
#define BITS_INSTRUMENTS_BASS_GUITAR_H

#include "instruments/base_instrument.h"
#include <stdint.h>

namespace BITS {
namespace Instruments {

class BassGuitar : public BaseInstrument {
public:
    BassGuitar();
    virtual void init() override;
    virtual void update() override;
    virtual void handleSensorInput(uint8_t sensorId, float value, float velocity) override;
    
    void setStringCount(uint8_t count);
    uint8_t getStringCount() const { return stringCount; }

private:
    static constexpr uint8_t MAX_STRINGS = 6;
    uint8_t stringCount;
    uint8_t stringSensors[MAX_STRINGS];
    float lastVelocities[MAX_STRINGS];
};

} // namespace Instruments
} // namespace BITS

#endif // BITS_INSTRUMENTS_BASS_GUITAR_H
