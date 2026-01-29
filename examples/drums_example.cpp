/*
 * B.I.T.E.S - Drums Example
 * 
 * This example demonstrates how to use the B.I.T.E.S system
 * with a drums instrument.
 */

#include "bits.h"
#include "instruments/drums.h"
#include "core/system_manager.h"
#include "core/logger.h"

using namespace BITS;
using namespace BITS::Instruments;
using namespace BITS::Core;

Drums drums;

void setup() {
    // Initialize system
    SystemManager::init();
    
    // Initialize drums
    drums.setPadCount(8);
    drums.init();
    
    Logger::info("Drums example ready!");
}

void loop() {
    // System runs in RTOS tasks
    // Drums updates automatically via sensor task
    delay(100);
}
