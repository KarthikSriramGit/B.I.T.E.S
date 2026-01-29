#ifndef BITS_RTOS_TASKS_H
#define BITS_RTOS_TASKS_H

#include <FreeRTOS.h>
#include <task.h>

namespace BITS {
namespace RTOS {

// Task priorities
enum class TaskPriority {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3,
    CRITICAL = 4
};

// Task stack sizes (in words, 4 bytes each)
constexpr uint32_t TASK_STACK_SENSOR = 1024;      // 4KB
constexpr uint32_t TASK_STACK_AUDIO = 2048;       // 8KB
constexpr uint32_t TASK_STACK_AI = 4096;          // 16KB
constexpr uint32_t TASK_STACK_NETWORK = 1536;     // 6KB
constexpr uint32_t TASK_STACK_SYSTEM = 512;       // 2KB

// Task handles
extern TaskHandle_t sensorTaskHandle;
extern TaskHandle_t audioTaskHandle;
extern TaskHandle_t aiTaskHandle;
extern TaskHandle_t networkTaskHandle;
extern TaskHandle_t systemTaskHandle;

// Task function prototypes
void sensorTask(void* parameters);
void audioTask(void* parameters);
void aiTask(void* parameters);
void networkTask(void* parameters);
void systemTask(void* parameters);

// Task management
void createTasks();
void suspendTasks();
void resumeTasks();
void deleteTasks();

} // namespace RTOS
} // namespace BITS

#endif // BITS_RTOS_TASKS_H
