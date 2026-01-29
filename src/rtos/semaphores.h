#ifndef BITS_RTOS_SEMAPHORES_H
#define BITS_RTOS_SEMAPHORES_H

#include <FreeRTOS.h>
#include <semphr.h>

namespace BITS {
namespace RTOS {

// Semaphore handles
extern SemaphoreHandle_t audioMutex;
extern SemaphoreHandle_t sensorMutex;
extern SemaphoreHandle_t aiMutex;
extern SemaphoreHandle_t configMutex;
extern SemaphoreHandle_t i2cMutex;

// Binary semaphores
extern SemaphoreHandle_t sensorDataReady;
extern SemaphoreHandle_t audioBufferReady;
extern SemaphoreHandle_t aiInferenceReady;

// Semaphore management
void createSemaphores();
void deleteSemaphores();

} // namespace RTOS
} // namespace BITS

#endif // BITS_RTOS_SEMAPHORES_H
