#include "core/logger.h"
#include <stdarg.h>

namespace BITS {
namespace Core {

LogLevel Logger::currentLevel = LogLevel::INFO;

void Logger::init(uint32_t baudRate) {
    Serial.begin(baudRate);
    while (!Serial && millis() < 5000) {
        delay(10);
    }
    info("Logger initialized at %lu baud", baudRate);
}

void Logger::setLevel(LogLevel level) {
    currentLevel = level;
}

LogLevel Logger::getLevel() {
    return currentLevel;
}

void Logger::log(LogLevel level, const char* format, va_list args) {
    if (level < currentLevel) {
        return;
    }
    
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    
    Serial.print("[");
    Serial.print(levelToString(level));
    Serial.print("] ");
    Serial.print(millis());
    Serial.print("ms: ");
    Serial.println(buffer);
}

const char* Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void Logger::debug(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LogLevel::DEBUG, format, args);
    va_end(args);
}

void Logger::info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LogLevel::INFO, format, args);
    va_end(args);
}

void Logger::warning(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LogLevel::WARNING, format, args);
    va_end(args);
}

void Logger::error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LogLevel::ERROR, format, args);
    va_end(args);
}

void Logger::debug(const String& message) {
    debug(message.c_str());
}

void Logger::info(const String& message) {
    info(message.c_str());
}

void Logger::warning(const String& message) {
    warning(message.c_str());
}

void Logger::error(const String& message) {
    error(message.c_str());
}

} // namespace Core
} // namespace BITS
