#ifndef BITS_RTOS_QUEUES_H
#define BITS_RTOS_QUEUES_H

#include <FreeRTOS.h>
#include <queue.h>

namespace BITS {
namespace RTOS {

// Queue sizes
constexpr UBaseType_t QUEUE_SIZE_SENSOR = 32;
constexpr UBaseType_t QUEUE_SIZE_AUDIO = 16;
constexpr UBaseType_t QUEUE_SIZE_AI = 16;
constexpr UBaseType_t QUEUE_SIZE_NETWORK = 8;

// Sensor event structure
struct SensorEvent {
    uint8_t sensorId;
    uint8_t sensorType;
    float value;
    uint32_t timestamp;
};

// Audio command structure
struct AudioCommand {
    enum Type {
        PLAY_NOTE,
        STOP_NOTE,
        SET_VOLUME,
        APPLY_EFFECT
    };
    Type type;
    uint8_t instrumentId;
    uint8_t noteId;
    float parameter;
};

// AI result structure
struct AIResult {
    enum Type {
        GESTURE_RECOGNIZED,
        CHORD_PREDICTED,
        TEMPO_DETECTED,
        PITCH_CORRECTED
    };
    Type type;
    uint8_t data[16];
    float confidence;
};

// Network message structure
struct NetworkMessage {
    enum Type {
        MQTT_PUBLISH,
        MQTT_SUBSCRIBE,
        OTA_UPDATE,
        WEB_REQUEST
    };
    Type type;
    char data[128];
};

// Queue handles
extern QueueHandle_t sensorQueue;
extern QueueHandle_t audioQueue;
extern QueueHandle_t aiQueue;
extern QueueHandle_t networkQueue;

// Queue management
void createQueues();
void deleteQueues();

} // namespace RTOS
} // namespace BITS

#endif // BITS_RTOS_QUEUES_H
