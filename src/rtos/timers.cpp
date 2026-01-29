#include "rtos/timers.h"
#include "core/logger.h"
#include "core/watchdog.h"
#include "sensors/sensor_manager.h"

namespace BITS {
namespace RTOS {

// Timer handles
TimerHandle_t heartbeatTimer = nullptr;
TimerHandle_t watchdogTimer = nullptr;
TimerHandle_t calibrationTimer = nullptr;

// Heartbeat timer callback
void heartbeatTimerCallback(TimerHandle_t xTimer) {
    Logger::info("System heartbeat");
    // Can be used for system health monitoring
}

// Watchdog timer callback
void watchdogTimerCallback(TimerHandle_t xTimer) {
    Watchdog::feed();
}

// Calibration timer callback
void calibrationTimerCallback(TimerHandle_t xTimer) {
    SensorManager::calibrate();
}

// Create all timers
void createTimers() {
    heartbeatTimer = xTimerCreate(
        "Heartbeat",
        pdMS_TO_TICKS(TIMER_PERIOD_HEARTBEAT),
        pdTRUE,  // Auto-reload
        nullptr,
        heartbeatTimerCallback
    );
    
    watchdogTimer = xTimerCreate(
        "Watchdog",
        pdMS_TO_TICKS(TIMER_PERIOD_WATCHDOG),
        pdTRUE,  // Auto-reload
        nullptr,
        watchdogTimerCallback
    );
    
    calibrationTimer = xTimerCreate(
        "Calibration",
        pdMS_TO_TICKS(TIMER_PERIOD_CALIBRATION),
        pdTRUE,  // Auto-reload
        nullptr,
        calibrationTimerCallback
    );
    
    if (heartbeatTimer && watchdogTimer && calibrationTimer) {
        xTimerStart(heartbeatTimer, 0);
        xTimerStart(watchdogTimer, 0);
        Logger::info("All RTOS timers created");
    } else {
        Logger::error("Failed to create RTOS timers");
    }
}

// Delete all timers
void deleteTimers() {
    if (heartbeatTimer) xTimerDelete(heartbeatTimer, 0);
    if (watchdogTimer) xTimerDelete(watchdogTimer, 0);
    if (calibrationTimer) xTimerDelete(calibrationTimer, 0);
}

} // namespace RTOS
} // namespace BITS
