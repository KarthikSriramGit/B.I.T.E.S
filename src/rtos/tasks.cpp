#include "rtos/tasks.h"
#include "core/task_manager.h"
#include "sensors/sensor_manager.h"
#include "audio/audio_manager.h"
#include "ai/ai_engine.h"
#include "network/wifi_manager.h"
#include "network/bluetooth_manager.h"
#include "core/watchdog.h"
#include "core/logger.h"

namespace BITS {
namespace RTOS {

// Task handles
TaskHandle_t sensorTaskHandle = nullptr;
TaskHandle_t audioTaskHandle = nullptr;
TaskHandle_t aiTaskHandle = nullptr;
TaskHandle_t networkTaskHandle = nullptr;
TaskHandle_t systemTaskHandle = nullptr;

// Sensor Task - High priority, polls sensors at 1kHz
void sensorTask(void* parameters) {
    const TickType_t xFrequency = pdMS_TO_TICKS(1); // 1ms = 1kHz
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    Logger::info("Sensor task started");
    
    while (true) {
        // Poll all sensors
        SensorManager::update();
        
        // Yield CPU
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

// Audio Task - High priority, processes audio samples
void audioTask(void* parameters) {
    const TickType_t xFrequency = pdMS_TO_TICKS(2); // 2ms for audio processing
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    Logger::info("Audio task started");
    
    while (true) {
        // Process audio samples
        AudioManager::update();
        
        // Yield CPU
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

// AI Task - Medium priority, runs ML inference
void aiTask(void* parameters) {
    const TickType_t xFrequency = pdMS_TO_TICKS(10); // 10ms for AI processing
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    Logger::info("AI task started");
    
    while (true) {
        // Run AI inference
        AIEngine::update();
        
        // Yield CPU
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

// Network Task - Low priority, handles WiFi/Bluetooth
void networkTask(void* parameters) {
    const TickType_t xFrequency = pdMS_TO_TICKS(100); // 100ms for network
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    Logger::info("Network task started");
    
    while (true) {
        // Handle WiFi
        WiFiManager::update();
        
        // Handle Bluetooth
        BluetoothManager::update();
        
        // Yield CPU
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

// System Task - Low priority, watchdog and health monitoring
void systemTask(void* parameters) {
    const TickType_t xFrequency = pdMS_TO_TICKS(1000); // 1s for system tasks
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    Logger::info("System task started");
    
    while (true) {
        // Feed watchdog
        Watchdog::feed();
        
        // System health monitoring
        TaskManager::monitorHealth();
        
        // Yield CPU
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

// Create all tasks
void createTasks() {
    // Sensor Task (High Priority)
    xTaskCreate(
        sensorTask,
        "SensorTask",
        TASK_STACK_SENSOR,
        nullptr,
        static_cast<UBaseType_t>(TaskPriority::HIGH),
        &sensorTaskHandle
    );
    
    // Audio Task (High Priority)
    xTaskCreate(
        audioTask,
        "AudioTask",
        TASK_STACK_AUDIO,
        nullptr,
        static_cast<UBaseType_t>(TaskPriority::HIGH),
        &audioTaskHandle
    );
    
    // AI Task (Medium Priority)
    xTaskCreate(
        aiTask,
        "AITask",
        TASK_STACK_AI,
        nullptr,
        static_cast<UBaseType_t>(TaskPriority::MEDIUM),
        &aiTaskHandle
    );
    
    // Network Task (Low Priority)
    xTaskCreate(
        networkTask,
        "NetworkTask",
        TASK_STACK_NETWORK,
        nullptr,
        static_cast<UBaseType_t>(TaskPriority::LOW),
        &networkTaskHandle
    );
    
    // System Task (Low Priority)
    xTaskCreate(
        systemTask,
        "SystemTask",
        TASK_STACK_SYSTEM,
        nullptr,
        static_cast<UBaseType_t>(TaskPriority::LOW),
        &systemTaskHandle
    );
    
    Logger::info("All RTOS tasks created");
}

// Suspend all tasks
void suspendTasks() {
    if (sensorTaskHandle) vTaskSuspend(sensorTaskHandle);
    if (audioTaskHandle) vTaskSuspend(audioTaskHandle);
    if (aiTaskHandle) vTaskSuspend(aiTaskHandle);
    if (networkTaskHandle) vTaskSuspend(networkTaskHandle);
    if (systemTaskHandle) vTaskSuspend(systemTaskHandle);
}

// Resume all tasks
void resumeTasks() {
    if (sensorTaskHandle) vTaskResume(sensorTaskHandle);
    if (audioTaskHandle) vTaskResume(audioTaskHandle);
    if (aiTaskHandle) vTaskResume(aiTaskHandle);
    if (networkTaskHandle) vTaskResume(networkTaskHandle);
    if (systemTaskHandle) vTaskResume(systemTaskHandle);
}

// Delete all tasks
void deleteTasks() {
    if (sensorTaskHandle) vTaskDelete(sensorTaskHandle);
    if (audioTaskHandle) vTaskDelete(audioTaskHandle);
    if (aiTaskHandle) vTaskDelete(aiTaskHandle);
    if (networkTaskHandle) vTaskDelete(networkTaskHandle);
    if (systemTaskHandle) vTaskDelete(systemTaskHandle);
}

} // namespace RTOS
} // namespace BITS
