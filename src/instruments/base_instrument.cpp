#include "instruments/base_instrument.h"

namespace BITS {
namespace Instruments {

BaseInstrument::BaseInstrument(InstrumentType type) 
    : type(type), active(false), trackId(0) {
}

} // namespace Instruments
} // namespace BITS
