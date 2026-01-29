#ifndef BITS_CORE_EVENT_QUEUE_H
#define BITS_CORE_EVENT_QUEUE_H

#include <FreeRTOS.h>
#include <queue.h>
#include <stdint.h>

namespace BITS {
namespace Core {

enum class EventType {
    SENSOR_TRIGGERED,
    NOTE_PLAYED,
    NOTE_STOPPED,
    GESTURE_RECOGNIZED,
    CHORD_DETECTED,
    TEMPO_DETECTED,
    CONFIG_CHANGED,
    SYSTEM_ERROR
};

struct Event {
    EventType type;
    uint32_t timestamp;
    uint8_t sourceId;
    uint8_t data[16];
};

class EventQueue {
public:
    static void init();
    static bool publish(const Event& event);
    static bool subscribe(Event& event, uint32_t timeoutMs = portMAX_DELAY);
    static uint32_t getQueueSize();
    static void clear();

private:
    static QueueHandle_t queue;
    static constexpr UBaseType_t QUEUE_SIZE = 32;
};

} // namespace Core
} // namespace BITS

#endif // BITS_CORE_EVENT_QUEUE_H
