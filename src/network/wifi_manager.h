#ifndef BITS_NETWORK_WIFI_MANAGER_H
#define BITS_NETWORK_WIFI_MANAGER_H

#include <WiFi.h>
#include <stdint.h>

namespace BITS {
namespace Network {

class WiFiManager {
public:
    static void init();
    static void update();
    
    static bool connect(const char* ssid, const char* password);
    static void disconnect();
    static bool isConnected();
    static String getIP();
    static int getRSSI();
    
    static void startWebServer();
    static void stopWebServer();

private:
    static bool initialized;
    static bool connected;
    static WiFiServer* webServer;
    static uint32_t lastConnectAttempt;
    static constexpr uint32_t CONNECT_TIMEOUT_MS = 30000;
};

} // namespace Network
} // namespace BITS

#endif // BITS_NETWORK_WIFI_MANAGER_H
