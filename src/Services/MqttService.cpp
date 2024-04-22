/** @file MqttService.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Super class for MQTT services
 *
 *  ###Contributors
 *  moravp15
 *  maresj39
 */

#include "MqttService.hpp"

void MqttService::SetMqttAddress(const std::string& hostname, int port) {
    mqttHostname = hostname;
    mqttPort = port;
}
