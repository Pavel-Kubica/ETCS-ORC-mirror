/** @file MqttListenerService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Service that is responsible for listening and giving workOrders to topic workers
 *
 *  ### Contributors
 *  veselo21
 *  moravp15
 *  zimaluk1
 *  maresj39
 *  hamaljan
 */

#pragma once

 #include "Topic.hpp"
 #include "TopicWorker.hpp"
 #include "ConfigurationService.hpp"
 #include "IInitializable.hpp"
 #include "IStartable.hpp"
 #include "JRULoggerService.hpp"
 #include "IMessageHandlersService.hpp"
 #include "MqttService.hpp"
#include "IMqttListenerService.hpp"
#include "mosquitto.h"
/**
 * Service that is responsible for listening and giving workOrders to topic workers
 */
class MqttListenerService : public MqttService,
                            public IMqttListenerService,
                            public IStartable,
                            public IInitializable,
                            public ILpcManagable {
public:
    ~MqttListenerService();
    void Initialize(ServiceContainer& container) override;
    /**
     * Starts the work_thread
     */
    void Start(ServiceContainer& container) override;
    /**
     * Tells all the topic Workers to wait until they finish execution and then joins them
     */
    void Wait() override;
    /**
     * Configures itself after receiving Start from LPC
     */
    void LpcSaidStart() override;
    void LpcSaidStop() override;
    void LpcSaidRestart() override;
    bool GetStarted() const override;
    void AppExit() override;

private:
    /**
     * Deserializes the message and sends it to get processed by the correct Topic Worker
     * @param mosquitto MQTT mosquitto struct for that callback
     * @param message The message that was received by mosquitto
     */
    virtual void MQTTCallback(struct mosquitto* mosquitto, const struct mosquitto_message* message);
    /**
     * Forwards the callback to user_callback_obj, which is a ListenerService*
     * @param mosquitto MQTT mosquitto struct for that callback
     * @param user_callback_obj ListenerService*
     * @param message The message that was received by mosquitto
     */
    static void CallbackForwarder(mosquitto* mosquitto, void* user_callback_obj, const mosquitto_message* message);
    struct mosquitto* komar;
    std::map<Topic, std::shared_ptr<TopicWorker>> topicWorkers;
    IMessageHandlersService* messageHandlersService;
    ConfigurationService* configurationService;
    JRULoggerService* jruLoggerService;
    bool started;
    /**
     * mosquitto listens in a loop
     */
    void Listen();
    std::thread work_thread;
};
