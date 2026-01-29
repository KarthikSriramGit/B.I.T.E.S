#include "network/wifi_manager.h"
#include "core/config_manager.h"
#include "core/logger.h"
#include "config.h"
#include <WiFi.h>
#include <Arduino.h>

namespace BITS {
namespace Network {

bool WiFiManager::initialized = false;
bool WiFiManager::connected = false;
WiFiServer* WiFiManager::webServer = nullptr;
uint32_t WiFiManager::lastConnectAttempt = 0;

void WiFiManager::init() {
    if (initialized) {
        Logger::warning("WiFi manager already initialized");
        return;
    }
    
    WiFi.mode(WIFI_STA);
    initialized = true;
    Logger::info("WiFi manager initialized");
    
    // Try to connect if credentials are stored
    String ssid = ConfigManager::getWiFiSSID();
    String password = ConfigManager::getWiFiPassword();
    if (ssid.length() > 0) {
        connect(ssid.c_str(), password.c_str());
    }
}

void WiFiManager::update() {
    if (!initialized) {
        return;
    }
    
    // Check connection status
    if (WiFi.status() == WL_CONNECTED) {
        if (!connected) {
            connected = true;
            Logger::info("WiFi connected: %s", getIP().c_str());
        }
        
        // Handle web server requests
        if (webServer != nullptr) {
            WiFiClient client = webServer->available();
            if (client) {
                handleWebRequest(client);
            }
        }
    } else {
        if (connected) {
            connected = false;
            Logger::warning("WiFi disconnected");
        }
    }
}

bool WiFiManager::connect(const char* ssid, const char* password) {
    if (!initialized) {
        return false;
    }
    
    Logger::info("Connecting to WiFi: %s", ssid);
    WiFi.begin(ssid, password);
    
    lastConnectAttempt = millis();
    uint32_t timeout = CONNECT_TIMEOUT_MS;
    
    while (WiFi.status() != WL_CONNECTED && (millis() - lastConnectAttempt) < timeout) {
        delay(500);
        Serial.print(".");
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        connected = true;
        Logger::info("WiFi connected!");
        Logger::info("IP address: %s", WiFi.localIP().toString().c_str());
        return true;
    } else {
        connected = false;
        Logger::error("WiFi connection failed");
        return false;
    }
}

void WiFiManager::disconnect() {
    WiFi.disconnect();
    connected = false;
    Logger::info("WiFi disconnected");
}

bool WiFiManager::isConnected() {
    return initialized && WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getIP() {
    if (isConnected()) {
        return WiFi.localIP().toString();
    }
    return String("");
}

int WiFiManager::getRSSI() {
    if (isConnected()) {
        return WiFi.RSSI();
    }
    return 0;
}

void WiFiManager::startWebServer() {
    if (webServer == nullptr) {
        webServer = new WiFiServer(WEB_SERVER_PORT);
        webServer->begin();
        Logger::info("Web server started on port %d", WEB_SERVER_PORT);
    }
}

void WiFiManager::stopWebServer() {
    if (webServer != nullptr) {
        webServer->stop();
        delete webServer;
        webServer = nullptr;
        Logger::info("Web server stopped");
    }
}

void WiFiManager::handleWebRequest(WiFiClient& client) {
    // Simplified web request handler
    // In production, implement full HTTP server
    String request = client.readStringUntil('\r');
    client.flush();
    
    // Send simple response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<h1>B.I.T.E.S Web Interface</h1>");
    client.println("<p>System Status: Running</p>");
    client.stop();
}

} // namespace Network
} // namespace BITS
