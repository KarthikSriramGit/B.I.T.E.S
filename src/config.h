#ifndef BITS_CONFIG_H
#define BITS_CONFIG_H

// System configuration
#define BITS_VERSION_MAJOR 1
#define BITS_VERSION_MINOR 0
#define BITS_VERSION_PATCH 0

// Hardware configuration
#define TEENSY_BOARD_TEENSY41
#define CPU_FREQ_MHZ 600
#define SERIAL_BAUD_RATE 115200

// RTOS configuration
#define RTOS_TICK_RATE_HZ 1000
#define RTOS_MAX_TASKS 10

// Sensor configuration
#define MAX_SENSORS 16
#define SENSOR_POLL_RATE_HZ 1000
#define MPU6050_I2C_ADDRESS 0x68
#define MPU6050_SDA_PIN 18
#define MPU6050_SCL_PIN 19

// Audio configuration
#define AUDIO_SAMPLE_RATE_HZ 44100
#define AUDIO_BITS_PER_SAMPLE 16
#define AUDIO_CHANNELS 2
#define AUDIO_BUFFER_SIZE 128
#define MAX_AUDIO_TRACKS 8
#define MAX_POLYPHONY 16

// AI configuration
#define AI_GESTURE_ENABLED 1
#define AI_CHORD_ENABLED 1
#define AI_TEMPO_ENABLED 1
#define AI_PITCH_ENABLED 1
#define AI_PATTERN_ENABLED 1

// Network configuration
#define WIFI_MAX_RETRY 5
#define WIFI_CONNECT_TIMEOUT_MS 30000
#define MQTT_MAX_MESSAGE_SIZE 256
#define WEB_SERVER_PORT 80

// Instrument types
enum class InstrumentType {
    BASS_GUITAR = 0,
    GUITAR = 1,
    KEYBOARD = 2,
    DRUMS = 3
};

#endif // BITS_CONFIG_H
