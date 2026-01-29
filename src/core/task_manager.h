#ifndef BITS_CORE_TASK_MANAGER_H
#define BITS_CORE_TASK_MANAGER_H

#include <FreeRTOS.h>
#include <task.h>

namespace BITS {
namespace Core {

struct TaskStats {
    const char* name;
    TaskHandle_t handle;
    UBaseType_t priority;
    uint32_t stackHighWaterMark;
    eTaskState state;
    uint32_t runtime;
};

class TaskManager {
public:
    static void init();
    static void monitorHealth();
    static TaskStats getTaskStats(TaskHandle_t handle);
    static void printAllTaskStats();
    static uint32_t getFreeHeap();
    static uint32_t getMinFreeHeap();
    static float getCPUUsage();

private:
    static uint32_t lastIdleTime;
    static uint32_t lastTotalTime;
};

} // namespace Core
} // namespace BITS

#endif // BITS_CORE_TASK_MANAGER_H
