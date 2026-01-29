#ifndef BITS_RTOS_TIMERS_H
#define BITS_RTOS_TIMERS_H

#include <FreeRTOS.h>
#include <timers.h>

namespace BITS {
namespace RTOS {

// Timer periods (in milliseconds)
constexpr uint32_t TIMER_PERIOD_HEARTBEAT = 5000;    // 5 seconds
constexpr uint32_t TIMER_PERIOD_WATCHDOG = 1000;      // 1 second
constexpr uint32_t TIMER_PERIOD_CALIBRATION = 10000;  // 10 seconds

// Timer handles
extern TimerHandle_t heartbeatTimer;
extern TimerHandle_t watchdogTimer;
extern TimerHandle_t calibrationTimer;

// Timer callback functions
void heartbeatTimerCallback(TimerHandle_t xTimer);
void watchdogTimerCallback(TimerHandle_t xTimer);
void calibrationTimerCallback(TimerHandle_t xTimer);

// Timer management
void createTimers();
void deleteTimers();

} // namespace RTOS
} // namespace BITS

#endif // BITS_RTOS_TIMERS_H
