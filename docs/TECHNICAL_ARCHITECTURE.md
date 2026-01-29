# B.I.T.E.S Technical Architecture

**Comprehensive Technical Deep-Dive Documentation**

This document provides a complete technical reference for understanding every aspect of the B.I.T.E.S (Beats, Instruments, Technology, Embedded Systems) platform implementation.

## Table of Contents

1. [System Architecture](#1-system-architecture)
2. [RTOS Implementation Details](#2-rtos-implementation-details)
3. [Sensor Processing Pipeline](#3-sensor-processing-pipeline)
4. [Audio Processing System](#4-audio-processing-system)
5. [AI/ML Implementation](#5-aiml-implementation)
6. [Network Protocols](#6-network-protocols)
7. [Real-Time Constraints](#7-real-time-constraints)
8. [Performance Optimizations](#8-performance-optimizations)
9. [Hardware Interactions](#9-hardware-interactions)
10. [Algorithms and Mathematics](#10-algorithms-and-mathematics)
11. [Error Handling and Reliability](#11-error-handling-and-reliability)
12. [Scalability and Extensibility](#12-scalability-and-extensibility)

---

## 1. System Architecture

### 1.1 Layered Architecture

B.I.T.E.S follows a layered architecture pattern with clear separation of concerns:

```
┌─────────────────────────────────────────────────────────┐
│              Application Layer (Instruments)            │
│  Provides instrument-specific interfaces and logic      │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│                  AI Engine Layer                         │
│  Gesture recognition, chord prediction, tempo, pitch    │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│                  RTOS Task Layer                         │
│  FreeRTOS tasks: Sensor, Audio, AI, Network, System      │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│                  Hardware Abstraction Layer              │
│  Sensor drivers, audio codec, GPIO, I2C, SPI, ADC       │
└─────────────────────────────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────┐
│                    Hardware Layer                        │
│  Teensy 4.1, MPU6050, Piezo, IR, Pressure, Flex        │
└─────────────────────────────────────────────────────────┘
```

### 1.2 Component Interaction

**Data Flow:**
1. Sensors → Sensor Manager → Sensor Task → Event Queue
2. Event Queue → Instrument Handler → Audio Manager → Audio Task
3. Sensor Data → AI Engine → Gesture/Chord/Tempo Detection
4. AI Results → Instrument Handler → Audio Effects

**Control Flow:**
- System Manager initializes all components
- RTOS scheduler manages task execution
- Event Queue provides asynchronous communication
- Configuration Manager handles persistent settings

### 1.3 Memory Map

```
Teensy 4.1 Memory Layout:
┌─────────────────────────────────────┐
│  Flash (7.75 MB)                    │
│  ├─ Code: ~500 KB                   │
│  ├─ Audio Samples: ~2 MB             │
│  ├─ ML Models: ~1 MB                │
│  └─ Free: ~4.25 MB                  │
├─────────────────────────────────────┤
│  RAM (512 KB)                        │
│  ├─ RTOS: ~50 KB                    │
│  ├─ Audio Buffers: ~200 KB          │
│  ├─ Sensor Data: ~20 KB             │
│  ├─ AI Buffers: ~100 KB             │
│  └─ Free: ~140 KB                   │
└─────────────────────────────────────┘
```

### 1.4 State Machine

**System States:**
- **INIT**: System initialization
- **READY**: Ready for operation
- **PLAYING**: Active instrument playback
- **CONFIGURING**: Configuration mode
- **UPDATING**: OTA firmware update
- **ERROR**: Error state

**State Transitions:**
```
INIT → READY → PLAYING
  ↓       ↓        ↓
ERROR ← ERROR ← ERROR
  ↓
READY
```

---

## 2. RTOS Implementation Details

### 2.1 FreeRTOS Task Scheduling

**Scheduler Type:** Preemptive, priority-based

**Task Priorities:**
- **Sensor Task**: Priority 3 (High) - 1kHz polling rate
- **Audio Task**: Priority 3 (High) - Real-time audio processing
- **AI Task**: Priority 2 (Medium) - ML inference
- **Network Task**: Priority 1 (Low) - WiFi/Bluetooth
- **System Task**: Priority 1 (Low) - Watchdog, health monitoring

**Scheduling Algorithm:**
```cpp
// FreeRTOS uses fixed-priority preemptive scheduling
// Higher priority tasks always preempt lower priority tasks
// Round-robin scheduling for tasks of equal priority
```

**Context Switching:**
- Overhead: ~5-10 microseconds per switch
- Frequency: ~1000 switches/second (typical)
- Worst-case: 20 microseconds

### 2.2 Inter-Task Communication

**Queues:**
- **Sensor Queue**: 32 events, 1ms timeout
- **Audio Queue**: 16 commands, no timeout
- **AI Queue**: 16 results, no timeout
- **Network Queue**: 8 messages, 100ms timeout

**Semaphores:**
- **Audio Mutex**: Protects audio buffer access
- **Sensor Mutex**: Protects sensor data structures
- **AI Mutex**: Protects ML model inference
- **Config Mutex**: Protects configuration data
- **I2C Mutex**: Serializes I2C bus access

**Event Groups:**
- WiFi connected event
- Audio ready event
- Sensor calibration complete event

### 2.3 Task Synchronization

**Critical Sections:**
```cpp
// Using mutex for resource protection
if (xSemaphoreTake(audioMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
    // Critical section
    audioBuffer->write(data);
    xSemaphoreGive(audioMutex);
}
```

**Priority Inversion Prevention:**
- Mutex priority inheritance enabled
- Critical sections kept minimal (<100μs)
- No nested mutex locks

### 2.4 Stack Management

**Stack Sizes:**
- Sensor Task: 4KB (1024 words × 4 bytes)
- Audio Task: 8KB (2048 words)
- AI Task: 16KB (4096 words)
- Network Task: 6KB (1536 words)
- System Task: 2KB (512 words)

**Stack Overflow Protection:**
- FreeRTOS stack overflow hook enabled
- Watermark monitoring via `uxTaskGetStackHighWaterMark()`
- Alert threshold: <100 bytes remaining

### 2.5 Watchdog Timer

**Implementation:**
- Hardware watchdog: 10 second timeout
- System task feeds watchdog every 1 second
- Task watchdog monitors individual tasks

**Watchdog Feeding:**
```cpp
void systemTask(void* parameters) {
    while (true) {
        Watchdog::feed();  // Feed hardware watchdog
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

### 2.6 Memory Management

**Heap Allocation:**
- FreeRTOS heap: 50KB
- Dynamic allocation: Minimal (prefer static)
- Memory pools: Audio buffers, sensor data

**Memory Monitoring:**
```cpp
uint32_t freeHeap = xPortGetFreeHeapSize();
uint32_t minFreeHeap = xPortGetMinimumEverFreeHeapSize();
```

---

## 3. Sensor Processing Pipeline

### 3.1 Sampling Rates and Nyquist

**Sensor Polling Rate:** 1kHz (1ms period)

**Nyquist Considerations:**
- Maximum detectable frequency: 500 Hz
- Gesture frequencies: 0-50 Hz (well within Nyquist)
- Audio frequencies: 20-20kHz (handled separately)

**ADC Sampling:**
- Teensy 4.1 ADC: 12-bit, up to 2MHz
- Effective rate: 1kHz per sensor
- Conversion time: ~1μs per sample

### 3.2 Sensor Fusion (Kalman Filter)

**Mathematical Model:**

**State Vector:**
```
x = [roll, pitch, yaw, accel_mag, gyro_mag]ᵀ
```

**Process Model:**
```
xₖ = Fₖxₖ₋₁ + wₖ
```
where:
- `Fₖ` = State transition matrix (identity for static estimation)
- `wₖ` = Process noise (Q = 0.1)

**Measurement Model:**
```
zₖ = Hₖxₖ + vₖ
```
where:
- `Hₖ` = Observation matrix (identity)
- `vₖ` = Measurement noise (R = 0.1)

**Kalman Filter Equations:**

1. **Prediction Step:**
```
Pₖ|ₖ₋₁ = Pₖ₋₁|ₖ₋₁ + Q
x̂ₖ|ₖ₋₁ = Fₖx̂ₖ₋₁|ₖ₋₁
```

2. **Update Step:**
```
Kₖ = Pₖ|ₖ₋₁ / (Pₖ|ₖ₋₁ + R)
x̂ₖ|ₖ = x̂ₖ|ₖ₋₁ + Kₖ(zₖ - x̂ₖ|ₖ₋₁)
Pₖ|ₖ = (1 - Kₖ)Pₖ|ₖ₋₁
```

**Implementation:**
```cpp
float KalmanFilter::update(float measurement) {
    // Prediction
    P = P + Q;
    
    // Update
    K = P / (P + R);
    X = X + K * (measurement - X);
    P = (1 - K) * P;
    
    return X;
}
```

### 3.3 Gesture Detection State Machine

**States:**
- **IDLE**: No gesture detected
- **DETECTING**: Potential gesture in progress
- **CONFIRMED**: Gesture confirmed
- **COMPLETE**: Gesture completed

**State Transitions:**
```
IDLE → (accel > threshold) → DETECTING
DETECTING → (pattern match) → CONFIRMED
CONFIRMED → (timeout) → COMPLETE
COMPLETE → IDLE
```

### 3.4 Debouncing Algorithms

**Piezoelectric Sensors:**
- Threshold: 100mV (configurable)
- Debounce time: 50ms
- Velocity calculation: Rate of change

**IR Sensors:**
- Digital input with pull-up
- Debounce time: 10ms
- Edge detection: Rising/falling

**Algorithm:**
```cpp
bool isTriggered(uint32_t now) {
    bool state = readSensor();
    if (state == triggerState) {
        if (now - lastTriggerTime > debounceTime) {
            lastTriggerTime = now;
            return true;
        }
    }
    return false;
}
```

### 3.5 Calibration Procedures

**MPU6050 Calibration:**
1. Collect 100 samples at rest
2. Calculate mean for each axis
3. Store offsets
4. Apply offsets to subsequent readings

**Piezoelectric Calibration:**
1. Read baseline (no impact)
2. Set threshold = baseline + 50mV
3. Test with known impacts
4. Adjust threshold if needed

**Pressure Sensor Calibration:**
1. Read minimum (no pressure)
2. Read maximum (full pressure)
3. Map to 0.0-1.0 range
4. Store calibration values

---

## 4. Audio Processing System

### 4.1 Audio Buffer Management

**Buffer Structure:**
- Block size: 128 samples
- Block duration: 2.9ms at 44.1kHz
- Total buffers: 15 blocks (configurable)
- Memory per block: 256 bytes (128 samples × 2 bytes)

**Buffer Allocation:**
```cpp
AudioMemory(15);  // Allocate 15 blocks
```

**Buffer Flow:**
```
Sensor → Instrument → Sample Manager → Audio Buffer → Mixer → Codec → Output
```

### 4.2 Sample Rate Conversion

**Teensy Audio Library:**
- Fixed sample rate: 44.1kHz
- 16-bit samples
- Stereo output

**Sample Playback:**
- Samples stored in PROGMEM (flash)
- Streamed to audio buffer
- Zero-copy when possible

### 4.3 Polyphonic Voice Allocation

**Voice Allocation Algorithm:**
```cpp
Voice* allocateVoice(uint8_t trackId, uint8_t noteId) {
    // First-fit allocation
    for (uint8_t i = 0; i < MAX_VOICES; i++) {
        if (!voices[i].active) {
            voices[i].trackId = trackId;
            voices[i].noteId = noteId;
            voices[i].active = true;
            return &voices[i];
        }
    }
    // Steal oldest voice if full
    return stealOldestVoice();
}
```

**Voice Limits:**
- Maximum voices: 16
- Per-track limit: 4 voices
- Priority: Newest note wins

### 4.4 Audio Effects Algorithms

**Reverb:**
- Algorithm: Freeverb (Schroeder reverb)
- Room size: 0.0-1.0
- Damping: 0.0-1.0
- Delay: 16000 samples (~363ms)

**Delay:**
- Delay time: 0-500ms
- Feedback: 0.0-1.0
- Wet/dry mix: 0.5

**Distortion:**
- Algorithm: Soft clipping
- Formula: `y = tanh(x * gain)`
- Gain: 1.0-10.0

**EQ:**
- 3-band parametric EQ
- Low: 20-200 Hz
- Mid: 200-2000 Hz
- High: 2000-20000 Hz

### 4.5 Mixing Mathematics

**Gain Staging:**
```
Track Volume → Mixer Channel → Master Mixer → Codec Volume → Output
   0.0-1.0        0.0-1.0         0.0-1.0        0.0-1.0
```

**Mixing Formula:**
```cpp
output = Σ(track[i] * trackVolume[i] * masterVolume)
```

**Headroom Management:**
- Individual tracks: -6dB default
- Master mix: -3dB headroom
- Prevents clipping

### 4.6 Latency Analysis

**End-to-End Latency:**
```
Sensor Poll:        1ms
Processing:         2ms
Audio Buffer:        3ms (1 block)
Codec Output:        1ms
─────────────────────────
Total:              ~7ms
```

**Optimization:**
- Direct sensor-to-audio path
- Minimal buffering
- Interrupt-driven audio

---

## 5. AI/ML Implementation

### 5.1 TensorFlow Lite Micro Integration

**Model Format:**
- Quantized INT8 model
- Model size: ~50-100KB
- Inference time: <5ms

**Model Loading:**
```cpp
// Model stored in PROGMEM
const unsigned char gesture_model[] PROGMEM = {
    // Model weights...
};
```

**Inference:**
```cpp
tflite::MicroInterpreter interpreter(&model, resolver, tensor_arena, ...);
interpreter.Invoke();
```

### 5.2 Gesture Recognition Feature Extraction

**Features (10-dimensional):**
1. Acceleration X, Y, Z
2. Angular velocity X, Y, Z
3. Roll, Pitch angles
4. Acceleration magnitude
5. Angular velocity magnitude

**Feature Normalization:**
```cpp
features[i] = (raw[i] - mean[i]) / std[i]
```

### 5.3 Chord Prediction Algorithm

**Music Theory Basis:**
- 12-tone equal temperament
- Chord detection via note classes (0-11)
- Key detection via note frequency analysis

**Algorithm:**
```cpp
Chord detectChord(uint8_t* notes, uint8_t count) {
    // Count note classes
    uint8_t classes[12] = {0};
    for (uint8_t i = 0; i < count; i++) {
        classes[notes[i] % 12]++;
    }
    
    // Find root (most common note)
    uint8_t root = argmax(classes);
    
    // Determine quality
    bool hasMinorThird = classes[(root + 3) % 12] > 0;
    uint8_t quality = hasMinorThird ? MINOR : MAJOR;
    
    return {root, quality, confidence};
}
```

### 5.4 Tempo Detection (FFT Analysis)

**Beat Detection:**
1. Collect sensor data over 2 seconds
2. Apply FFT to detect periodic patterns
3. Find dominant frequency
4. Convert to BPM: `BPM = frequency × 60`

**Autocorrelation Method:**
```cpp
float autocorrelation(float* buffer, uint32_t length, uint32_t lag) {
    float sum = 0.0f;
    for (uint32_t i = 0; i < length - lag; i++) {
        sum += buffer[i] * buffer[i + lag];
    }
    return sum / (length - lag);
}
```

### 5.5 Pitch Detection (Autocorrelation)

**Fundamental Frequency Detection:**
```cpp
float detectPitch(float* audioBuffer, uint32_t length) {
    float maxCorr = 0.0f;
    uint32_t bestLag = 0;
    
    for (uint32_t lag = 20; lag < length / 2; lag++) {
        float corr = autocorrelation(audioBuffer, length, lag);
        if (corr > maxCorr) {
            maxCorr = corr;
            bestLag = lag;
        }
    }
    
    return 44100.0f / bestLag;  // Convert to frequency
}
```

### 5.6 Pattern Learning Storage

**Pattern Structure:**
```cpp
struct Pattern {
    uint8_t notes[32];
    uint32_t timings[32];
    uint8_t noteCount;
    float confidence;
};
```

**Similarity Calculation:**
```cpp
float similarity(Pattern& p1, Pattern& p2) {
    uint8_t matches = 0;
    uint8_t minCount = min(p1.noteCount, p2.noteCount);
    
    for (uint8_t i = 0; i < minCount; i++) {
        if (p1.notes[i] == p2.notes[i]) matches++;
    }
    
    return (float)matches / minCount;
}
```

---

## 6. Network Protocols

### 6.1 WiFi Connection State Machine

**States:**
- **DISCONNECTED**: Not connected
- **CONNECTING**: Connection in progress
- **CONNECTED**: Connected and ready
- **RECONNECTING**: Attempting reconnection

**State Transitions:**
```
DISCONNECTED → (connect()) → CONNECTING
CONNECTING → (success) → CONNECTED
CONNECTING → (timeout) → DISCONNECTED
CONNECTED → (disconnect) → DISCONNECTED
CONNECTED → (lost) → RECONNECTING
```

### 6.2 OTA Update Protocol

**Update Process:**
1. Check update URL via HTTPS
2. Download firmware binary
3. Verify signature (optional)
4. Write to flash (OTA partition)
5. Reboot to new firmware

**Security:**
- HTTPS only
- Signature verification
- Rollback on failure

### 6.3 MQTT Message Format

**Topic Structure:**
```
bits/{device_id}/{instrument}/{event}
```

**Message Format (JSON):**
```json
{
    "timestamp": 1234567890,
    "instrument": "bass",
    "event": "note_played",
    "data": {
        "note": 40,
        "velocity": 0.8
    }
}
```

**QoS Levels:**
- QoS 0: At most once (sensor data)
- QoS 1: At least once (commands)
- QoS 2: Exactly once (critical)

### 6.4 Bluetooth MIDI Protocol

**MIDI Message Format:**
```
Status Byte | Data Byte 1 | Data Byte 2
```

**Note On:**
```
0x9n | Note (0-127) | Velocity (0-127)
where n = channel (0-15)
```

**Note Off:**
```
0x8n | Note (0-127) | Velocity (0-127)
```

**Implementation:**
```cpp
void sendMIDI(uint8_t status, uint8_t data1, uint8_t data2) {
    bluetooth.write(status);
    bluetooth.write(data1);
    bluetooth.write(data2);
}
```

### 6.5 Web Server Architecture

**HTTP Server:**
- Port: 80
- Protocol: HTTP/1.1
- Endpoints:
  - `/` - Status page
  - `/config` - Configuration
  - `/sensors` - Sensor data (JSON)
  - `/audio` - Audio controls

**Request Handling:**
```cpp
void handleRequest(WiFiClient& client) {
    String request = client.readStringUntil('\r');
    
    if (request.indexOf("GET /") == 0) {
        sendResponse(client, "200 OK", htmlContent);
    }
}
```

---

## 7. Real-Time Constraints

### 7.1 Worst-Case Execution Time (WCET)

**Task WCET Analysis:**
- Sensor Task: 500μs (sensor polling + fusion)
- Audio Task: 2ms (buffer processing)
- AI Task: 5ms (ML inference)
- Network Task: 50ms (WiFi operations)
- System Task: 100μs (watchdog feed)

**Total CPU Usage:**
```
Sensor:  500μs / 1000μs = 50%
Audio:   2000μs / 2000μs = 100% (worst case)
AI:      5000μs / 10000μs = 50%
Network: 50ms / 100ms = 50%
System:  100μs / 1000ms = 0.01%
─────────────────────────────────
Total:   ~250% (multicore would help)
```

**Note:** Audio task runs at higher priority, ensuring real-time performance.

### 7.2 Deadline Guarantees

**Hard Deadlines:**
- Audio buffer: 2.9ms (must not underrun)
- Sensor polling: 1ms (must complete)

**Soft Deadlines:**
- AI inference: 10ms (can be delayed)
- Network operations: 100ms (best effort)

### 7.3 Interrupt Latency

**Measured Latencies:**
- GPIO interrupt: <1μs
- I2C interrupt: <2μs
- Audio interrupt: <5μs
- Timer interrupt: <1μs

**Interrupt Service Routines:**
- Keep ISRs minimal (<10μs)
- Defer work to tasks
- Use queues for data transfer

### 7.4 Audio Buffer Underrun Prevention

**Strategies:**
1. Pre-allocate buffers
2. Monitor buffer levels
3. Reduce processing if needed
4. Priority scheduling for audio task

**Buffer Monitoring:**
```cpp
if (AudioMemoryUsageMax() > 90%) {
    // Reduce effects processing
    // Skip non-critical operations
}
```

---

## 8. Performance Optimizations

### 8.1 Cache Optimization

**Teensy 4.1 Cache:**
- Instruction cache: 32KB
- Data cache: 32KB
- Cache line: 32 bytes

**Optimization Strategies:**
- Align data structures to cache lines
- Keep hot data in cache
- Minimize cache misses

### 8.2 DMA Usage

**DMA Channels:**
- Audio: DMA for I2S transfer
- Sensors: DMA for ADC (if available)
- Network: DMA for Ethernet (if available)

**Benefits:**
- Reduces CPU load
- Enables parallel processing
- Lower latency

### 8.3 SIMD Instructions

**ARM Cortex-M7 SIMD:**
- NEON instructions available
- 128-bit SIMD operations
- 4× float operations per cycle

**Audio Processing:**
```cpp
// Vectorized mixing (pseudo-code)
float32x4_t samples = vld1q_f32(buffer);
float32x4_t gains = vld1q_f32(gainArray);
float32x4_t result = vmulq_f32(samples, gains);
vst1q_f32(output, result);
```

### 8.4 Lookup Tables

**Mathematical Functions:**
- Sine/cosine: 256-entry LUT
- Note frequencies: 128-entry LUT (MIDI notes)
- ADC calibration: Per-sensor LUT

**Memory vs. Speed Trade-off:**
- LUT: Fast, uses memory
- Calculation: Slow, saves memory
- Choose based on frequency of use

### 8.5 Memory Alignment

**Alignment Requirements:**
- Audio buffers: 4-byte aligned
- DMA buffers: 32-byte aligned (cache line)
- Structs: Natural alignment

**Alignment Macros:**
```cpp
#define ALIGN_4 __attribute__((aligned(4)))
#define ALIGN_32 __attribute__((aligned(32)))
```

### 8.6 CPU Usage Profiling

**Profiling Methods:**
1. FreeRTOS run-time stats
2. GPIO toggles for timing
3. Serial logging with timestamps
4. External logic analyzer

**Optimization Process:**
1. Profile to find bottlenecks
2. Optimize hot paths
3. Re-profile to verify
4. Iterate

---

## 9. Hardware Interactions

### 9.1 GPIO Configuration

**GPIO Modes:**
- Input: Floating, pull-up, pull-down
- Output: Push-pull, open-drain
- Analog: ADC input
- Special: I2C, SPI, PWM

**Configuration:**
```cpp
pinMode(pin, INPUT_PULLUP);
pinMode(pin, OUTPUT);
analogReadResolution(12);  // 12-bit ADC
```

### 9.2 I2C Communication

**MPU6050 I2C:**
- Speed: 400kHz (fast mode)
- Address: 0x68 (7-bit)
- Protocol: Standard I2C

**Transaction:**
```cpp
Wire.beginTransmission(0x68);
Wire.write(0x3B);  // Register address
Wire.endTransmission(false);
Wire.requestFrom(0x68, 14, true);
```

### 9.3 ADC Sampling Strategies

**ADC Configuration:**
- Resolution: 12-bit (0-4095)
- Reference: 3.3V
- Sampling: Single-shot or continuous

**Averaging:**
```cpp
// Reduce noise via averaging
uint32_t sum = 0;
for (uint8_t i = 0; i < 8; i++) {
    sum += analogRead(pin);
}
uint16_t value = sum / 8;
```

### 9.4 PWM Generation

**PWM Configuration:**
- Frequency: 1kHz - 20kHz
- Resolution: 8-bit or 16-bit
- Channels: Multiple independent channels

**Usage:**
```cpp
analogWriteFrequency(pin, 1000);  // 1kHz
analogWrite(pin, 128);  // 50% duty cycle
```

### 9.5 Interrupt Service Routines

**ISR Best Practices:**
- Keep ISRs short (<10μs)
- No blocking operations
- Use queues for data transfer
- Disable interrupts only when necessary

**Example:**
```cpp
void sensorISR() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(sensorQueue, &data, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
```

### 9.6 Hardware Abstraction Layer (HAL)

**HAL Design:**
- Abstract hardware details
- Provide consistent interface
- Enable portability

**Example:**
```cpp
class SensorHAL {
public:
    virtual float read() = 0;
    virtual void calibrate() = 0;
};

class MPU6050HAL : public SensorHAL {
    // MPU6050-specific implementation
};
```

---

## 10. Algorithms and Mathematics

### 10.1 Kalman Filter Equations

**Complete Kalman Filter:**

**State Prediction:**
```
x̂ₖ|ₖ₋₁ = Fₖx̂ₖ₋₁|ₖ₋₁ + Bₖuₖ
Pₖ|ₖ₋₁ = FₖPₖ₋₁|ₖ₋₁Fₖᵀ + Qₖ
```

**Measurement Update:**
```
Kₖ = Pₖ|ₖ₋₁Hₖᵀ(HₖPₖ|ₖ₋₁Hₖᵀ + Rₖ)⁻¹
x̂ₖ|ₖ = x̂ₖ|ₖ₋₁ + Kₖ(zₖ - Hₖx̂ₖ|ₖ₋₁)
Pₖ|ₖ = (I - KₖHₖ)Pₖ|ₖ₋₁
```

**Where:**
- `x̂` = State estimate
- `P` = Error covariance
- `F` = State transition matrix
- `H` = Observation matrix
- `Q` = Process noise covariance
- `R` = Measurement noise covariance
- `K` = Kalman gain
- `z` = Measurement

### 10.2 FFT for Frequency Analysis

**Discrete Fourier Transform:**
```
X[k] = Σ(n=0 to N-1) x[n]e^(-j2πkn/N)
```

**FFT Algorithm:**
- Radix-2 Cooley-Tukey
- Complexity: O(N log N)
- N must be power of 2

**Implementation:**
```cpp
// Use ARM CMSIS DSP library
arm_cfft_f32(&S, fftBuffer, 0, 1);
arm_cmplx_mag_f32(fftBuffer, magnitude, fftSize);
```

### 10.3 Autocorrelation for Pitch Detection

**Autocorrelation Function:**
```
R(τ) = Σ(t=0 to T-τ) x(t)x(t+τ)
```

**Normalized Autocorrelation:**
```
R_norm(τ) = R(τ) / √(R(0) × R(τ))
```

**Pitch Detection:**
```cpp
float detectPitch(float* buffer, uint32_t length) {
    float maxCorr = 0.0f;
    uint32_t bestLag = 0;
    
    for (uint32_t lag = minLag; lag < maxLag; lag++) {
        float corr = autocorrelation(buffer, length, lag);
        if (corr > maxCorr) {
            maxCorr = corr;
            bestLag = lag;
        }
    }
    
    return sampleRate / bestLag;
}
```

### 10.4 Music Theory Algorithms

**Note to Frequency:**
```
f = 440 × 2^((n - 69) / 12)
where n = MIDI note number (0-127)
```

**Chord Detection:**
```cpp
bool isMajorChord(uint8_t root, uint8_t* notes, uint8_t count) {
    bool hasThird = false, hasFifth = false;
    for (uint8_t i = 0; i < count; i++) {
        uint8_t interval = (notes[i] - root + 12) % 12;
        if (interval == 4) hasThird = true;   // Major third
        if (interval == 7) hasFifth = true;   // Perfect fifth
    }
    return hasThird && hasFifth;
}
```

**Key Detection:**
- Count note occurrences
- Match to key signatures
- Major keys: 0, 2, 4, 5, 7, 9, 11 (C, D, E, F, G, A, B)
- Minor keys: 0, 2, 3, 5, 7, 8, 10 (A, B, C, D, E, F, G)

### 10.5 Gesture Classification

**Feature Extraction:**
```cpp
void extractFeatures(float* features, MPU6050Data& data) {
    features[0] = data.accelX;
    features[1] = data.accelY;
    features[2] = data.accelZ;
    features[3] = data.gyroX;
    features[4] = data.gyroY;
    features[5] = data.gyroZ;
    features[6] = sqrt(data.accelX² + data.accelY² + data.accelZ²);
    features[7] = sqrt(data.gyroX² + data.gyroY² + data.gyroZ²);
    features[8] = atan2(data.accelY, data.accelZ);  // Roll
    features[9] = atan2(-data.accelX, sqrt(data.accelY² + data.accelZ²));  // Pitch
}
```

**Classification:**
- Neural network: 10 inputs → 5 outputs (gesture types)
- Softmax activation for probabilities
- Threshold: 0.7 confidence

### 10.6 Pattern Matching

**Dynamic Time Warping (DTW):**
```
DTW(i,j) = distance(i,j) + min(
    DTW(i-1, j),
    DTW(i, j-1),
    DTW(i-1, j-1)
)
```

**Simplified Pattern Matching:**
```cpp
float patternSimilarity(Pattern& p1, Pattern& p2) {
    uint8_t matches = 0;
    uint8_t minLen = min(p1.noteCount, p2.noteCount);
    
    for (uint8_t i = 0; i < minLen; i++) {
        if (p1.notes[i] == p2.notes[i]) {
            matches++;
        }
    }
    
    return (float)matches / minLen;
}
```

---

## 11. Error Handling and Reliability

### 11.1 Error Detection Mechanisms

**Sensor Errors:**
- I2C communication failure
- ADC out of range
- Sensor disconnected detection

**Audio Errors:**
- Buffer underrun
- Codec initialization failure
- Sample loading failure

**Network Errors:**
- WiFi connection timeout
- MQTT disconnection
- OTA update failure

### 11.2 Fault Tolerance Strategies

**Redundancy:**
- Multiple sensor readings
- Averaging to reduce noise
- Fallback to defaults

**Recovery:**
- Automatic reconnection (WiFi)
- Sensor recalibration
- System reset on critical errors

### 11.3 Recovery Procedures

**Error Recovery Flow:**
```
Error Detected → Log Error → Attempt Recovery → 
  Success → Continue
  Failure → Escalate → System Reset (if critical)
```

**Recovery Strategies:**
- Retry with exponential backoff
- Fallback to safe mode
- Graceful degradation

### 11.4 Logging and Diagnostics

**Log Levels:**
- DEBUG: Detailed debugging info
- INFO: General information
- WARNING: Recoverable errors
- ERROR: Critical errors

**Log Format:**
```
[LEVEL] timestamp_ms: message
```

**Diagnostic Data:**
- System uptime
- Task statistics
- Memory usage
- CPU usage
- Error counts

### 11.5 Watchdog Timer Integration

**Hardware Watchdog:**
- Timeout: 10 seconds
- Fed by system task
- Reset on timeout

**Task Watchdog:**
- Monitor individual tasks
- Alert on task starvation
- Reset if critical task fails

### 11.6 Memory Leak Prevention

**Strategies:**
- Prefer static allocation
- Use memory pools
- Monitor heap usage
- Regular garbage collection (if applicable)

**Monitoring:**
```cpp
uint32_t freeHeap = xPortGetFreeHeapSize();
if (freeHeap < threshold) {
    Logger::warning("Low heap memory: %lu bytes", freeHeap);
}
```

---

## 12. Scalability and Extensibility

### 12.1 Plugin Architecture

**Instrument Plugin Interface:**
```cpp
class InstrumentPlugin {
public:
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void handleSensor(uint8_t id, float value) = 0;
};
```

**Plugin Registration:**
```cpp
void registerInstrument(InstrumentPlugin* plugin) {
    instruments[instrumentCount++] = plugin;
}
```

### 12.2 Sensor Driver Abstraction

**Unified Sensor Interface:**
```cpp
class SensorDriver {
public:
    virtual float read() = 0;
    virtual void calibrate() = 0;
    virtual bool isTriggered() = 0;
};
```

**Driver Registration:**
```cpp
void registerSensor(SensorType type, SensorDriver* driver) {
    drivers[type] = driver;
}
```

### 12.3 AI Model Hot-Swapping

**Model Loading:**
```cpp
bool loadModel(const uint8_t* modelData, size_t size) {
    // Unload current model
    unloadModel();
    
    // Load new model
    return tflite::LoadModel(modelData, size);
}
```

**Runtime Model Update:**
- Download new model via OTA
- Validate model
- Swap models atomically
- Rollback on failure

### 12.4 Configuration System Design

**Configuration Storage:**
- NVS (Non-Volatile Storage)
- Key-value pairs
- Namespace: "bits_config"

**Configuration API:**
```cpp
void setConfig(const char* key, const char* value);
String getConfig(const char* key, const char* defaultValue);
```

**Runtime Updates:**
- Serial commands
- Web interface
- MQTT messages

### 12.5 API Design Principles

**Principles:**
1. **Consistency**: Similar functions work similarly
2. **Simplicity**: Easy to use, hard to misuse
3. **Composability**: Functions can be combined
4. **Extensibility**: Easy to add features

**Example API:**
```cpp
// Simple, consistent interface
BITS::SystemManager::init();
BITS::InstrumentManager::setInstrument(BITS::InstrumentType::BASS_GUITAR);
BITS::AIManager::enableGestureRecognition(true);
```

---

## Conclusion

This technical architecture document provides a comprehensive reference for understanding the B.I.T.E.S system implementation. Each component is designed with:

- **Real-time performance** in mind
- **Modularity** for easy extension
- **Reliability** through error handling
- **Efficiency** via optimization

For implementation details, refer to the source code in the `src/` directory. For usage examples, see the `examples/` directory.

---

**Document Version:** 1.0  
**Last Updated:** 2026-01-28  
**Author:** B.I.T.E.S Development Team
