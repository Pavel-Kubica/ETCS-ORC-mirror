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
 */

#include "MqttPublisherService.hpp"

MqttPublisherService::~MqttPublisherService() {
    mosquitto_disconnect(komar);
    mosquitto_destroy(komar);
}

void MqttPublisherService::Publish(std::shared_ptr<Message> msg) {
    nlohmann::json j = msg->to_json();
    msg->GetMessageID();
    int res = mosquitto_publish(komar, NULL, ConvertTopicToString(ConvertMessageIDToTopic(msg->GetMessageID())).c_str(),
                                j.dump().size(), j.dump().c_str(), 0, false);
    if (res != MOSQ_ERR_SUCCESS) {
        mosquitto_reconnect_async(komar);
        res = mosquitto_publish(komar, NULL, ConvertTopicToString(ConvertMessageIDToTopic(msg->GetMessageID())).c_str(),
                                j.dump().size(), j.dump().c_str(), 0, false);
        if (res != MOSQ_ERR_SUCCESS) {
            throw std::runtime_error(std::string("MqttPublisher could not publish message: ") +
                                     mosquitto_strerror(res));
        }
    }
}

void MqttPublisherService::Initialize(ServiceContainer& container) {
    jruLoggerService = container.FetchService<JRULoggerService>().get();
    ConnectToBroker();
    jruLoggerService->Log(true, MessageType::Info, "MqttPublisher successfully connected to MQTT broker.");
}

void MqttPublisherService::ConnectToBroker() {
    if (!komar) {
        komar = mosquitto_new("OrcPublisher", true, this);
        auto res = mosquitto_connect_async(komar, mqttHostname.c_str(), mqttPort, 60);
        if (res != MOSQ_ERR_SUCCESS) {
            throw std::runtime_error(std::string("MqttPublisher could not connect to MQTT broker.") +
                                     mosquitto_strerror(res));
        }
        mosquitto_loop_start(komar);
    }
}