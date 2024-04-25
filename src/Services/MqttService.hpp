/** @file MqttService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Parent class for MQTT services
 *
 *  ###Contributors
 *  moravp15
 *  maresj39
 */

#pragma once

#include <string>

#include "IInitializable.hpp"
#include "IStartable.hpp"
/**
 * Parent class for MQTT services
 */
class MqttService {
public:
    void SetMqttAddress(const std::string& hostname, int port = 1883);

protected:
    std::string mqttHostname;
    int mqttPort;
};
