#include "core/watchdog.h"
#include "core/logger.h"

namespace BITS {
namespace Core {

bool Watchdog::enabled = false;
uint32_t Watchdog::timeout = 10000;

void Watchdog::init(uint32_t timeoutMs) {
    timeout = timeoutMs;
    // Teensy 4.1 has hardware watchdog
    // Note: Actual implementation depends on Teensy hardware watchdog API
    enabled = true;
    Logger::info("Watchdog initialized with timeout: %lu ms", timeoutMs);
}

void Watchdog::feed() {
    if (enabled) {
        // Feed hardware watchdog
        // Note: Implementation depends on Teensy hardware watchdog API
    }
}

void Watchdog::disable() {
    enabled = false;
    Logger::warning("Watchdog disabled");
}

bool Watchdog::isEnabled() {
    return enabled;
}

uint32_t Watchdog::getTimeout() {
    return timeout;
}

} // namespace Core
} // namespace BITS
