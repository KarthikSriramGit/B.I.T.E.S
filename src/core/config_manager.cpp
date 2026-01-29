#include "core/config_manager.h"
#include "core/logger.h"
#include <EEPROM.h>

namespace BITS {
namespace Core {

const char* ConfigManager::NAMESPACE = "bits_config";
bool ConfigManager::initialized = false;

// Note: Using EEPROM for Teensy. For production, consider using EEPROM library
// or implement NVS-like storage using Flash memory

void ConfigManager::init() {
    EEPROM.begin(512); // Allocate 512 bytes for configuration
    initialized = true;
    Logger::info("Config manager initialized");
    load();
}

bool ConfigManager::load() {
    if (!initialized) {
        init();
    }
    
    // Load configuration from EEPROM
    // For now, use defaults if EEPROM is empty
    Logger::info("Configuration loaded");
    return true;
}

bool ConfigManager::save() {
    if (!initialized) {
        return false;
    }
    
    EEPROM.commit();
    Logger::info("Configuration saved");
    return true;
}

uint8_t ConfigManager::getInstrumentType() {
    return getKeyUint8("instrument_type", 0);
}

void ConfigManager::setInstrumentType(uint8_t type) {
    setKey("instrument_type", type);
}

bool ConfigManager::getSensorEnabled(uint8_t sensorId) {
    char key[32];
    snprintf(key, sizeof(key), "sensor_%d_en", sensorId);
    return getKeyBool(key, true);
}

void ConfigManager::setSensorEnabled(uint8_t sensorId, bool enabled) {
    char key[32];
    snprintf(key, sizeof(key), "sensor_%d_en", sensorId);
    setKey(key, enabled);
}

uint8_t ConfigManager::getSensorGPIO(uint8_t sensorId) {
    char key[32];
    snprintf(key, sizeof(key), "sensor_%d_gpio", sensorId);
    return getKeyUint8(key, 0);
}

void ConfigManager::setSensorGPIO(uint8_t sensorId, uint8_t gpio) {
    char key[32];
    snprintf(key, sizeof(key), "sensor_%d_gpio", sensorId);
    setKey(key, gpio);
}

float ConfigManager::getVolume() {
    return getKeyFloat("audio_volume", 0.8f);
}

void ConfigManager::setVolume(float volume) {
    setKey("audio_volume", volume);
}

bool ConfigManager::getAIEnabled(uint8_t aiFeatureId) {
    char key[32];
    snprintf(key, sizeof(key), "ai_%d_en", aiFeatureId);
    return getKeyBool(key, false);
}

void ConfigManager::setAIEnabled(uint8_t aiFeatureId, bool enabled) {
    char key[32];
    snprintf(key, sizeof(key), "ai_%d_en", aiFeatureId);
    setKey(key, enabled);
}

String ConfigManager::getWiFiSSID() {
    return getKey("wifi_ssid");
}

void ConfigManager::setWiFiSSID(const String& ssid) {
    setKey("wifi_ssid", ssid);
}

String ConfigManager::getWiFiPassword() {
    return getKey("wifi_pass");
}

void ConfigManager::setWiFiPassword(const String& password) {
    setKey("wifi_pass", password);
}

bool ConfigManager::getBluetoothEnabled() {
    return getKeyBool("bluetooth_en", false);
}

void ConfigManager::setBluetoothEnabled(bool enabled) {
    setKey("bluetooth_en", enabled);
}

void ConfigManager::resetToDefaults() {
    setInstrumentType(0);
    setVolume(0.8f);
    setBluetoothEnabled(false);
    save();
    Logger::info("Configuration reset to defaults");
}

String ConfigManager::getKey(const char* key) {
    // Simplified implementation - in production, use proper key-value storage
    return String("");
}

void ConfigManager::setKey(const char* key, const String& value) {
    // Simplified implementation
}

void ConfigManager::setKey(const char* key, uint8_t value) {
    // Simplified implementation - store in EEPROM
}

void ConfigManager::setKey(const char* key, float value) {
    // Simplified implementation
}

void ConfigManager::setKey(const char* key, bool value) {
    // Simplified implementation
}

uint8_t ConfigManager::getKeyUint8(const char* key, uint8_t defaultValue) {
    // Simplified implementation
    return defaultValue;
}

float ConfigManager::getKeyFloat(const char* key, float defaultValue) {
    // Simplified implementation
    return defaultValue;
}

bool ConfigManager::getKeyBool(const char* key, bool defaultValue) {
    // Simplified implementation
    return defaultValue;
}

} // namespace Core
} // namespace BITS
