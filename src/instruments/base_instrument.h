#ifndef BITS_INSTRUMENTS_BASE_INSTRUMENT_H
#define BITS_INSTRUMENTS_BASE_INSTRUMENT_H

#include <stdint.h>
#include "config.h"

namespace BITS {
namespace Instruments {

class BaseInstrument {
public:
    BaseInstrument(InstrumentType type);
    virtual ~BaseInstrument() = default;
    
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void handleSensorInput(uint8_t sensorId, float value, float velocity) = 0;
    
    InstrumentType getType() const { return type; }
    bool isActive() const { return active; }
    void setActive(bool active) { this->active = active; }
    
protected:
    InstrumentType type;
    bool active;
    uint8_t trackId;
};

} // namespace Instruments
} // namespace BITS

#endif // BITS_INSTRUMENTS_BASE_INSTRUMENT_H
