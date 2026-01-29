#include "rtos/semaphores.h"
#include "core/logger.h"

namespace BITS {
namespace RTOS {

// Semaphore handles
SemaphoreHandle_t audioMutex = nullptr;
SemaphoreHandle_t sensorMutex = nullptr;
SemaphoreHandle_t aiMutex = nullptr;
SemaphoreHandle_t configMutex = nullptr;
SemaphoreHandle_t i2cMutex = nullptr;

// Binary semaphores
SemaphoreHandle_t sensorDataReady = nullptr;
SemaphoreHandle_t audioBufferReady = nullptr;
SemaphoreHandle_t aiInferenceReady = nullptr;

// Create all semaphores
void createSemaphores() {
    // Mutexes
    audioMutex = xSemaphoreCreateMutex();
    sensorMutex = xSemaphoreCreateMutex();
    aiMutex = xSemaphoreCreateMutex();
    configMutex = xSemaphoreCreateMutex();
    i2cMutex = xSemaphoreCreateMutex();
    
    // Binary semaphores
    sensorDataReady = xSemaphoreCreateBinary();
    audioBufferReady = xSemaphoreCreateBinary();
    aiInferenceReady = xSemaphoreCreateBinary();
    
    if (audioMutex && sensorMutex && aiMutex && configMutex && i2cMutex &&
        sensorDataReady && audioBufferReady && aiInferenceReady) {
        Logger::info("All RTOS semaphores created");
    } else {
        Logger::error("Failed to create RTOS semaphores");
    }
}

// Delete all semaphores
void deleteSemaphores() {
    if (audioMutex) vSemaphoreDelete(audioMutex);
    if (sensorMutex) vSemaphoreDelete(sensorMutex);
    if (aiMutex) vSemaphoreDelete(aiMutex);
    if (configMutex) vSemaphoreDelete(configMutex);
    if (i2cMutex) vSemaphoreDelete(i2cMutex);
    if (sensorDataReady) vSemaphoreDelete(sensorDataReady);
    if (audioBufferReady) vSemaphoreDelete(audioBufferReady);
    if (aiInferenceReady) vSemaphoreDelete(aiInferenceReady);
}

} // namespace RTOS
} // namespace BITS
