#include <Arduino.h>
#include "config.h"
#include "core/system_manager.h"
#include "core/logger.h"

using namespace BITS;
using namespace BITS::Core;

void setup() {
    // Initialize system
    SystemManager::init();
    
    // Additional initialization can go here
    Logger::info("B.I.T.E.S ready!");
}

void loop() {
    // Main loop is mostly empty - system runs in RTOS tasks
    // This loop can be used for non-critical tasks or serial command processing
    delay(100);
}
