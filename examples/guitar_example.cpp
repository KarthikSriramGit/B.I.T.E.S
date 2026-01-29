/*
 * B.I.T.E.S - Guitar Example
 * 
 * This example demonstrates how to use the B.I.T.E.S system
 * with a standard guitar instrument.
 */

#include "bits.h"
#include "instruments/guitar.h"
#include "core/system_manager.h"
#include "core/logger.h"

using namespace BITS;
using namespace BITS::Instruments;
using namespace BITS::Core;

Guitar guitar;

void setup() {
    // Initialize system
    SystemManager::init();
    
    // Initialize guitar
    guitar.setStringCount(6);
    guitar.init();
    
    Logger::info("Guitar example ready!");
}

void loop() {
    // System runs in RTOS tasks
    // Guitar updates automatically via sensor task
    delay(100);
}
