#include "core/event_queue.h"
#include "core/logger.h"

namespace BITS {
namespace Core {

QueueHandle_t EventQueue::queue = nullptr;

void EventQueue::init() {
    queue = xQueueCreate(QUEUE_SIZE, sizeof(Event));
    if (queue == nullptr) {
        Logger::error("Failed to create event queue");
    } else {
        Logger::info("Event queue initialized");
    }
}

bool EventQueue::publish(const Event& event) {
    if (queue == nullptr) {
        return false;
    }
    
    BaseType_t result = xQueueSend(queue, &event, 0);
    return result == pdTRUE;
}

bool EventQueue::subscribe(Event& event, uint32_t timeoutMs) {
    if (queue == nullptr) {
        return false;
    }
    
    TickType_t timeout = timeoutMs == portMAX_DELAY ? portMAX_DELAY : pdMS_TO_TICKS(timeoutMs);
    BaseType_t result = xQueueReceive(queue, &event, timeout);
    return result == pdTRUE;
}

uint32_t EventQueue::getQueueSize() {
    if (queue == nullptr) {
        return 0;
    }
    return uxQueueMessagesWaiting(queue);
}

void EventQueue::clear() {
    if (queue != nullptr) {
        xQueueReset(queue);
    }
}

} // namespace Core
} // namespace BITS
