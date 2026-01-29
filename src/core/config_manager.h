#ifndef BITS_CORE_CONFIG_MANAGER_H
#define BITS_CORE_CONFIG_MANAGER_H

#include <Arduino.h>

namespace BITS {
namespace Core {

class ConfigManager {
public:
    static void init();
    static bool load();
    static bool save();
    
    // Instrument configuration
    static uint8_t getInstrumentType();
    static void setInstrumentType(uint8_t type);
    
    // Sensor configuration
    static bool getSensorEnabled(uint8_t sensorId);
    static void setSensorEnabled(uint8_t sensorId, bool enabled);
    static uint8_t getSensorGPIO(uint8_t sensorId);
    static void setSensorGPIO(uint8_t sensorId, uint8_t gpio);
    
    // Audio configuration
    static float getVolume();
    static void setVolume(float volume);
    
    // AI configuration
    static bool getAIEnabled(uint8_t aiFeatureId);
    static void setAIEnabled(uint8_t aiFeatureId, bool enabled);
    
    // WiFi configuration
    static String getWiFiSSID();
    static void setWiFiSSID(const String& ssid);
    static String getWiFiPassword();
    static void setWiFiPassword(const String& password);
    
    // Bluetooth configuration
    static bool getBluetoothEnabled();
    static void setBluetoothEnabled(bool enabled);
    
    // Reset to defaults
    static void resetToDefaults();

private:
    static const char* NAMESPACE;
    static bool initialized;
    
    static String getKey(const char* key);
    static void setKey(const char* key, const String& value);
    static void setKey(const char* key, uint8_t value);
    static void setKey(const char* key, float value);
    static void setKey(const char* key, bool value);
    static uint8_t getKeyUint8(const char* key, uint8_t defaultValue);
    static float getKeyFloat(const char* key, float defaultValue);
    static bool getKeyBool(const char* key, bool defaultValue);
};

} // namespace Core
} // namespace BITS

#endif // BITS_CORE_CONFIG_MANAGER_H
