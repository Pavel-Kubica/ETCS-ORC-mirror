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
 *  pavlian5
 *  cajantom
 */

#pragma once

#include "ServiceContainer.hpp"
#include "IInitializable.hpp"
#include "IStartable.hpp"
#include "JRULoggerService.hpp"
#include "MqttService.hpp"
#include "IMqttPublisherService.hpp"
#include "AsyncQueue.hpp"

#include <mosquitto.h>
#include <memory>
#include <vector>
#include <thread>

/**
 * Service responsible for publishing messages via MQTT
 */
class MqttPublisherService : public MqttService, public IMqttPublisherService, public IInitializable, public IStartable {
public:
    /**
     * Publishes message via mosquitto
     * @param msg message to be published
     */
    void Publish(std::shared_ptr<Message> msg) override;
    void Initialize(ServiceContainer& container) override;
    void Start(ServiceContainer& container) override;
    void Wait() override;
    void AppExit() override;

private:
    void PublisherThreadEntryPoint();

    JRULoggerService* jruLoggerService;
    std::thread publishThread;
    AsyncQueue<std::shared_ptr<Message>> publishQueue;
};
