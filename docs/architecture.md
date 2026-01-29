# System Architecture

See [TECHNICAL_ARCHITECTURE.md](TECHNICAL_ARCHITECTURE.md) for comprehensive details.

## Quick Overview

B.I.T.E.S uses a layered architecture:

1. **Application Layer**: Instrument implementations
2. **AI Layer**: Machine learning features
3. **RTOS Layer**: FreeRTOS task management
4. **Hardware Layer**: Sensor and audio drivers

## Key Components

- **System Manager**: Initializes and coordinates all components
- **Sensor Manager**: Unified interface for all sensors
- **Audio Manager**: Handles audio playback and effects
- **AI Engine**: Coordinates ML features
- **Network Manager**: WiFi and Bluetooth connectivity

## Data Flow

```
Sensors → Sensor Manager → Instrument → Audio Manager → Output
                ↓
            AI Engine → Gesture/Chord/Tempo Detection
```
