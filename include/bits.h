#ifndef BITS_H
#define BITS_H

/*
 * B.I.T.E.S - Main API Header
 * 
 * Include this header to use the B.I.T.E.S system
 */

// Core system
#include "core/system_manager.h"
#include "core/logger.h"
#include "core/config_manager.h"

// Instruments
#include "instruments/base_instrument.h"
#include "instruments/bass_guitar.h"
#include "instruments/guitar.h"
#include "instruments/keyboard.h"
#include "instruments/drums.h"

// Sensors
#include "sensors/sensor_manager.h"

// Audio
#include "audio/audio_manager.h"

// AI
#include "ai/ai_engine.h"

// Network
#include "network/wifi_manager.h"
#include "network/bluetooth_manager.h"

#endif // BITS_H
