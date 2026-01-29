#include "rtos/queues.h"
#include "core/logger.h"

namespace BITS {
namespace RTOS {

// Queue handles
QueueHandle_t sensorQueue = nullptr;
QueueHandle_t audioQueue = nullptr;
QueueHandle_t aiQueue = nullptr;
QueueHandle_t networkQueue = nullptr;

// Create all queues
void createQueues() {
    sensorQueue = xQueueCreate(QUEUE_SIZE_SENSOR, sizeof(SensorEvent));
    audioQueue = xQueueCreate(QUEUE_SIZE_AUDIO, sizeof(AudioCommand));
    aiQueue = xQueueCreate(QUEUE_SIZE_AI, sizeof(AIResult));
    networkQueue = xQueueCreate(QUEUE_SIZE_NETWORK, sizeof(NetworkMessage));
    
    if (sensorQueue && audioQueue && aiQueue && networkQueue) {
        Logger::info("All RTOS queues created");
    } else {
        Logger::error("Failed to create RTOS queues");
    }
}

// Delete all queues
void deleteQueues() {
    if (sensorQueue) vQueueDelete(sensorQueue);
    if (audioQueue) vQueueDelete(audioQueue);
    if (aiQueue) vQueueDelete(aiQueue);
    if (networkQueue) vQueueDelete(networkQueue);
}

} // namespace RTOS
} // namespace BITS
