#ifndef BITS_CORE_WATCHDOG_H
#define BITS_CORE_WATCHDOG_H

namespace BITS {
namespace Core {

class Watchdog {
public:
    static void init(uint32_t timeoutMs = 10000);
    static void feed();
    static void disable();
    static bool isEnabled();
    static uint32_t getTimeout();

private:
    static bool enabled;
    static uint32_t timeout;
};

} // namespace Core
} // namespace BITS

#endif // BITS_CORE_WATCHDOG_H
