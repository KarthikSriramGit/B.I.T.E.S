#ifndef BITS_NETWORK_OTA_UPDATER_H
#define BITS_NETWORK_OTA_UPDATER_H

#include <stdint.h>

namespace BITS {
namespace Network {

class OTAUpdater {
public:
    static void init();
    static void update();
    
    static bool checkForUpdate(const char* url);
    static bool performUpdate(const char* url);
    static float getProgress();

private:
    static bool initialized;
    static bool updating;
    static float progress;
};

} // namespace Network
} // namespace BITS

#endif // BITS_NETWORK_OTA_UPDATER_H
