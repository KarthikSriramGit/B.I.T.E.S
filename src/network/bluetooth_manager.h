#ifndef BITS_NETWORK_BLUETOOTH_MANAGER_H
#define BITS_NETWORK_BLUETOOTH_MANAGER_H

#include <stdint.h>

namespace BITS {
namespace Network {

class BluetoothManager {
public:
    static void init();
    static void update();
    
    static bool enable();
    static void disable();
    static bool isEnabled();
    
    static void sendMIDI(uint8_t status, uint8_t data1, uint8_t data2);
    static void sendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    static void sendNoteOff(uint8_t channel, uint8_t note);

private:
    static bool initialized;
    static bool enabled;
    
    // Note: Teensy 4.1 doesn't have built-in Bluetooth
    // This is a placeholder for future Bluetooth module integration
};

} // namespace Network
} // namespace BITS

#endif // BITS_NETWORK_BLUETOOTH_MANAGER_H
