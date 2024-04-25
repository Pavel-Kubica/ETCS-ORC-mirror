/** @file IMqttPublisherService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Interface for publisher service
 *
 *  ###Contributors
 *  veselo21
 *  maresj39
 *  zimaluk1
 */

#pragma once

#include <memory>
#include <string>

 #include "Message.hpp"
 #include "IService.hpp"
/**
 * Interface for service responsible for publishing messages via MQTT
 */
class IMqttPublisherService : public IService {
public:
    explicit IMqttPublisherService() : IService() {}

    virtual void Publish(std::shared_ptr<Message> msg) = 0;

    static constexpr ServiceType Type = ServiceType::Publisher;
};
