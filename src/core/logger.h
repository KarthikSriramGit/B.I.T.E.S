#ifndef BITS_CORE_LOGGER_H
#define BITS_CORE_LOGGER_H

#include <Arduino.h>

namespace BITS {
namespace Core {

enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
    NONE = 4
};

class Logger {
public:
    static void init(uint32_t baudRate = 115200);
    static void setLevel(LogLevel level);
    static LogLevel getLevel();
    
    static void debug(const char* format, ...);
    static void info(const char* format, ...);
    static void warning(const char* format, ...);
    static void error(const char* format, ...);
    
    static void debug(const String& message);
    static void info(const String& message);
    static void warning(const String& message);
    static void error(const String& message);

private:
    static LogLevel currentLevel;
    static void log(LogLevel level, const char* format, va_list args);
    static const char* levelToString(LogLevel level);
};

} // namespace Core
} // namespace BITS

// Convenience macros
#define LOG_DEBUG(...) BITS::Core::Logger::debug(__VA_ARGS__)
#define LOG_INFO(...) BITS::Core::Logger::info(__VA_ARGS__)
#define LOG_WARNING(...) BITS::Core::Logger::warning(__VA_ARGS__)
#define LOG_ERROR(...) BITS::Core::Logger::error(__VA_ARGS__)

#endif // BITS_CORE_LOGGER_H
