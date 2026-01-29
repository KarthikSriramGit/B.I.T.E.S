#include "core/task_manager.h"
#include "core/logger.h"
#include "rtos/tasks.h"

namespace BITS {
namespace Core {

uint32_t TaskManager::lastIdleTime = 0;
uint32_t TaskManager::lastTotalTime = 0;

void TaskManager::init() {
    Logger::info("Task manager initialized");
}

void TaskManager::monitorHealth() {
    // Check stack usage
    TaskHandle_t handles[] = {
        RTOS::sensorTaskHandle,
        RTOS::audioTaskHandle,
        RTOS::aiTaskHandle,
        RTOS::networkTaskHandle,
        RTOS::systemTaskHandle
    };
    
    for (auto handle : handles) {
        if (handle != nullptr) {
            uint32_t stackHighWaterMark = uxTaskGetStackHighWaterMark(handle);
            if (stackHighWaterMark < 100) {
                Logger::warning("Task %s low on stack: %lu bytes remaining", 
                               pcTaskGetName(handle), stackHighWaterMark * 4);
            }
        }
    }
    
    // Check heap
    uint32_t freeHeap = getFreeHeap();
    if (freeHeap < 10000) {
        Logger::warning("Low heap memory: %lu bytes free", freeHeap);
    }
}

TaskStats TaskManager::getTaskStats(TaskHandle_t handle) {
    TaskStats stats;
    if (handle == nullptr) {
        stats.name = "NULL";
        return stats;
    }
    
    stats.name = pcTaskGetName(handle);
    stats.handle = handle;
    stats.priority = uxTaskPriorityGet(handle);
    stats.stackHighWaterMark = uxTaskGetStackHighWaterMark(handle);
    stats.state = eTaskGetState(handle);
    stats.runtime = 0; // Runtime stats require configUSE_STATS_FORMATTING_FUNCTIONS
    
    return stats;
}

void TaskManager::printAllTaskStats() {
    Logger::info("=== Task Statistics ===");
    Logger::info("Free Heap: %lu bytes", getFreeHeap());
    Logger::info("Min Free Heap: %lu bytes", getMinFreeHeap());
    
    TaskHandle_t handles[] = {
        RTOS::sensorTaskHandle,
        RTOS::audioTaskHandle,
        RTOS::aiTaskHandle,
        RTOS::networkTaskHandle,
        RTOS::systemTaskHandle
    };
    
    for (auto handle : handles) {
        if (handle != nullptr) {
            TaskStats stats = getTaskStats(handle);
            Logger::info("Task: %s, Priority: %d, Stack: %lu bytes, State: %d",
                        stats.name, stats.priority, stats.stackHighWaterMark * 4, stats.state);
        }
    }
}

uint32_t TaskManager::getFreeHeap() {
    return xPortGetFreeHeapSize();
}

uint32_t TaskManager::getMinFreeHeap() {
    return xPortGetMinimumEverFreeHeapSize();
}

float TaskManager::getCPUUsage() {
    // Simplified CPU usage calculation
    // In production, use proper runtime statistics
    return 0.0f;
}

} // namespace Core
} // namespace BITS
