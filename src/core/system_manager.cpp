#include "core/system_manager.h"
#include "core/logger.h"
#include "core/watchdog.h"
#include "core/config_manager.h"
#include "core/event_queue.h"
#include "core/task_manager.h"
#include "rtos/tasks.h"
#include "rtos/queues.h"
#include "rtos/semaphores.h"
#include "rtos/timers.h"

namespace BITS {
namespace Core {

bool SystemManager::initialized = false;
uint32_t SystemManager::startTime = 0;

void SystemManager::init() {
    if (initialized) {
        Logger::warning("System already initialized");
        return;
    }
    
    startTime = millis();
    
    // Initialize core components
    Logger::init();
    Logger::info("=== B.I.T.E.S System Initialization ===");
    
    // Initialize configuration
    ConfigManager::init();
    
    // Initialize watchdog
    Watchdog::init();
    
    // Initialize RTOS components
    RTOS::createQueues();
    RTOS::createSemaphores();
    RTOS::createTimers();
    
    // Initialize task manager
    TaskManager::init();
    
    // Initialize event queue
    EventQueue::init();
    
    // Create RTOS tasks
    RTOS::createTasks();
    
    initialized = true;
    Logger::info("System initialization complete");
    Logger::info("Free memory: %lu bytes", getFreeMemory());
}

void SystemManager::shutdown() {
    if (!initialized) {
        return;
    }
    
    Logger::info("Shutting down system...");
    
    // Delete RTOS tasks
    RTOS::deleteTasks();
    
    // Delete RTOS resources
    RTOS::deleteTimers();
    RTOS::deleteSemaphores();
    RTOS::deleteQueues();
    
    // Disable watchdog
    Watchdog::disable();
    
    initialized = false;
    Logger::info("System shutdown complete");
}

bool SystemManager::isInitialized() {
    return initialized;
}

void SystemManager::restart() {
    Logger::warning("System restart requested");
    shutdown();
    delay(1000);
    // Note: Actual restart depends on hardware
    // For Teensy, may need to use watchdog reset or software reset
}

uint32_t SystemManager::getUptime() {
    return millis() - startTime;
}

float SystemManager::getCPUUsage() {
    return TaskManager::getCPUUsage();
}

uint32_t SystemManager::getFreeMemory() {
    return TaskManager::getFreeHeap();
}

} // namespace Core
} // namespace BITS
