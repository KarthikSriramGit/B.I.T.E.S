/*
 * B.I.T.E.S - Keyboard Example
 * 
 * This example demonstrates how to use the B.I.T.E.S system
 * with a keyboard instrument.
 */

#include "bits.h"
#include "instruments/keyboard.h"
#include "core/system_manager.h"
#include "core/logger.h"

using namespace BITS;
using namespace BITS::Instruments;
using namespace BITS::Core;

Keyboard keyboard;

void setup() {
    // Initialize system
    SystemManager::init();
    
    // Initialize keyboard
    keyboard.setKeyCount(61); // 5-octave keyboard
    keyboard.init();
    
    Logger::info("Keyboard example ready!");
}

void loop() {
    // System runs in RTOS tasks
    // Keyboard updates automatically via sensor task
    delay(100);
}
