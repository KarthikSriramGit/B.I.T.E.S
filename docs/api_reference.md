# API Reference

## Core System

### SystemManager
```cpp
void init();
void shutdown();
uint32_t getUptime();
```

### Logger
```cpp
void init(uint32_t baudRate = 115200);
void info(const char* format, ...);
void error(const char* format, ...);
```

### ConfigManager
```cpp
void init();
uint8_t getInstrumentType();
void setInstrumentType(uint8_t type);
float getVolume();
void setVolume(float volume);
```

## Instruments

### BaseInstrument
```cpp
virtual void init() = 0;
virtual void update() = 0;
virtual void handleSensorInput(uint8_t sensorId, float value, float velocity) = 0;
```

### BassGuitar, Guitar, Keyboard, Drums
All inherit from BaseInstrument and implement instrument-specific logic.

## Sensors

### SensorManager
```cpp
void init();
void update();
bool registerSensor(SensorType type, uint8_t id, uint8_t gpio);
SensorData getSensorData(uint8_t id);
```

## Audio

### AudioManager
```cpp
void init();
bool playNote(uint8_t trackId, uint8_t noteId, float velocity);
void setVolume(float volume);
```

## AI

### AIEngine
```cpp
void init();
void enableGestureRecognition(bool enable);
void enableChordPrediction(bool enable);
```

## Network

### WiFiManager
```cpp
void init();
bool connect(const char* ssid, const char* password);
bool isConnected();
```

### BluetoothManager
```cpp
void init();
void sendMIDI(uint8_t status, uint8_t data1, uint8_t data2);
```
