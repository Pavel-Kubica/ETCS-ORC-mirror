/** @file MqttPublisherService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Class for publishing messages
 *
 *  ###Contributors
 *  veselo21
 *  moravp15
 *  maresj39
 *  zimaluk1
 *  hamaljan
 */

#pragma once

 #include "ServiceContainer.hpp"
 #include "IInitializable.hpp"
 #include "JRULoggerService.hpp"
 #include "MqttService.hpp"
#include "IMqttPublisherService.hpp"

#include <mosquitto.h>
#include <memory>
#include <vector>
/**
 * Service responsible for publishing messages via MQTT
 */
class MqttPublisherService : public MqttService, public IMqttPublisherService, public IInitializable {
public:
    ~MqttPublisherService();
    /**
     * Publishes message via mosquitto
     * @param msg message to be published
     */
    void Publish(std::shared_ptr<Message> msg) override;
    void Initialize(ServiceContainer& container) override;

private:
    struct mosquitto* komar;
    JRULoggerService* jruLoggerService;
};
