#ifndef BITS_NETWORK_MQTT_CLIENT_H
#define BITS_NETWORK_MQTT_CLIENT_H

#include <stdint.h>

namespace BITS {
namespace Network {

class MQTTClient {
public:
    static void init();
    static void update();
    
    static bool connect(const char* broker, uint16_t port = 1883);
    static void disconnect();
    static bool isConnected();
    
    static bool publish(const char* topic, const char* payload);
    static void subscribe(const char* topic);

private:
    static bool initialized;
    static bool connected;
    // Note: Would use PubSubClient library in production
};

} // namespace Network
} // namespace BITS

#endif // BITS_NETWORK_MQTT_CLIENT_H
