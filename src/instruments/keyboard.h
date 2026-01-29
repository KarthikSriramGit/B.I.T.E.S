#ifndef BITS_INSTRUMENTS_KEYBOARD_H
#define BITS_INSTRUMENTS_KEYBOARD_H

#include "instruments/base_instrument.h"
#include <stdint.h>

namespace BITS {
namespace Instruments {

class Keyboard : public BaseInstrument {
public:
    Keyboard();
    virtual void init() override;
    virtual void update() override;
    virtual void handleSensorInput(uint8_t sensorId, float value, float velocity) override;
    
    void setKeyCount(uint8_t count);
    uint8_t getKeyCount() const { return keyCount; }

private:
    static constexpr uint8_t MAX_KEYS = 88;
    uint8_t keyCount;
    uint8_t keySensors[MAX_KEYS];
    bool keyStates[MAX_KEYS];
};

} // namespace Instruments
} // namespace BITS

#endif // BITS_INSTRUMENTS_KEYBOARD_H
