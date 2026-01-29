/*
 * B.I.T.E.S - Bass Guitar Example
 * 
 * This example demonstrates how to use the B.I.T.E.S system
 * with a bass guitar instrument.
 */

#include "bits.h"
#include "instruments/bass_guitar.h"
#include "core/system_manager.h"
#include "core/logger.h"

using namespace BITS;
using namespace BITS::Instruments;
using namespace BITS::Core;

BassGuitar bassGuitar;

void setup() {
    // Initialize system
    SystemManager::init();
    
    // Initialize bass guitar
    bassGuitar.setStringCount(4);
    bassGuitar.init();
    
    Logger::info("Bass Guitar example ready!");
}

void loop() {
    // System runs in RTOS tasks
    // Bass guitar updates automatically via sensor task
    delay(100);
}
