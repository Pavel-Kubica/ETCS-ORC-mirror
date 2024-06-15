/** @file MqttPublisherService.cpp
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

#include "MqttPublisherService.hpp"

void MqttPublisherService::Publish(std::shared_ptr<Message> msg) {
    // Null messages must not be sent from outside, publisher thread treats them as termination command
    if (msg != nullptr) {
        publishQueue.Push(std::move(msg));
    }
}

void MqttPublisherService::Initialize(ServiceContainer& container) {
    jruLoggerService = container.FetchService<JRULoggerService>().get();
}

void MqttPublisherService::Start(ServiceContainer& container) {
    publishThread = std::thread(&MqttPublisherService::PublisherThreadEntryPoint, this);
}

void MqttPublisherService::Wait() {
    publishThread.join();
}

void MqttPublisherService::AppExit() {
    // Sending a null message terminates the publisher thread
    publishQueue.Push(nullptr);
}

void MqttPublisherService::PublisherThreadEntryPoint() {
    mosquitto* komar = mosquitto_new("OrcPublisher", true, this);
    auto res = mosquitto_connect(komar, mqttHostname.c_str(), mqttPort, 60);
    if (res != MOSQ_ERR_SUCCESS) {
        throw std::runtime_error(std::string("MqttPublisher could not connect to MQTT broker.") +
                                 mosquitto_strerror(res));
    }
    jruLoggerService->Log(true, MessageType::Info, "MqttPublisher successfully connected to MQTT broker.");
    mosquitto_loop_start(komar);

    while (auto msg = publishQueue.Pop()) {
        nlohmann::json j = msg->to_json();
        msg->GetMessageID();
        int res = mosquitto_publish(komar, NULL, ConvertTopicToString(ConvertMessageIDToTopic(msg->GetMessageID())).c_str(),
                                    j.dump().size(), j.dump().c_str(), 0, false);
        if (res != MOSQ_ERR_SUCCESS) {
            mosquitto_reconnect(komar);
            res = mosquitto_publish(komar, NULL, ConvertTopicToString(ConvertMessageIDToTopic(msg->GetMessageID())).c_str(),
                                    j.dump().size(), j.dump().c_str(), 0, false);
            if (res != MOSQ_ERR_SUCCESS) {
                throw std::runtime_error(std::string("MqttPublisher could not publish message: ") +
                                         mosquitto_strerror(res));
            }
        }
    }

    mosquitto_disconnect(komar);
    mosquitto_destroy(komar);
}
