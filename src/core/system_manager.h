#ifndef BITS_CORE_SYSTEM_MANAGER_H
#define BITS_CORE_SYSTEM_MANAGER_H

namespace BITS {
namespace Core {

class SystemManager {
public:
    static void init();
    static void shutdown();
    static bool isInitialized();
    static void restart();
    static uint32_t getUptime();
    static float getCPUUsage();
    static uint32_t getFreeMemory();

private:
    static bool initialized;
    static uint32_t startTime;
};

} // namespace Core
} // namespace BITS

#endif // BITS_CORE_SYSTEM_MANAGER_H
