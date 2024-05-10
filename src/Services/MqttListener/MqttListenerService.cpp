/** @file MqttListenerService.cpp
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
 *  pavlian5
 */


#include "MqttListenerService.hpp"
#include "ListenerConfiguration.hpp"
#include "JsonTopicWorker.hpp"
#include "CANTopicWorker.hpp"
#include "BatteryControlCANMessageHandler.hpp"
#include "DirectionControlCANMessageHandler.hpp"
#include "DrivingLeverCANMessageHandler.hpp"


MqttListenerService::~MqttListenerService() {
    mosquitto_disconnect(komar);
    mosquitto_destroy(komar);
}

void MqttListenerService::Initialize(ServiceContainer& container) {
    jruLoggerService = container.FetchService<JRULoggerService>().get();
    configurationService = container.FetchService<ConfigurationService>().get();
    
    komar = mosquitto_new("OrcListener", true, this);
    mosquitto_threaded_set(komar, true);
    mosquitto_message_callback_set(komar, CallbackForwarder);
    
    if (mosquitto_connect(komar, mqttHostname.c_str(), mqttPort, 60) != MOSQ_ERR_SUCCESS) {
        jruLoggerService->Log(true, MessageType::Fatal, "MqttListener could not connect to MQTT broker.");
        throw std::runtime_error("MqttListener could not connect to MQTT broker.");
    }
    jruLoggerService->Log(true, MessageType::Info, "MqttListener successfully connected to MQTT broker.");
    
    messageHandlersService = container.FetchService<IMessageHandlersService>().get();
    
    mosquitto_subscribe(komar, NULL, ConvertTopicToString(Topic::LPCtoORC).c_str(), 0);
    topicWorkers.insert({Topic::LPCtoORC, std::make_shared<JsonTopicWorker>(messageHandlersService->GetAllHandlers(),
                                                                            jruLoggerService, Topic::LPCtoORC)});
}

void MqttListenerService::Start(ServiceContainer& container) {
    work_thread = std::thread(&MqttListenerService::Listen, this);
}

void MqttListenerService::MQTTCallback(struct mosquitto* mosquitto, const struct mosquitto_message* message) {
    std::string data(static_cast<const char*>(message->payload), message->payloadlen);
    std::string topic(message->topic);
    std::shared_ptr<TopicWorker> worker = topicWorkers.at(ConvertStringToTopic(topic));
    worker->AddToQueue(data);
}

void MqttListenerService::CallbackForwarder(mosquitto* mosquitto,
                                            void* user_callback_obj,
                                            const mosquitto_message* message) {
    MqttListenerService* listener = static_cast<MqttListenerService*>(user_callback_obj);
    listener->MQTTCallback(mosquitto, message);
}

void MqttListenerService::Listen() {
    mosquitto_loop_forever(komar, -1, 1);
}

void MqttListenerService::Wait() {
    work_thread.join();
}

bool MqttListenerService::LpcSaidStart() {
    if (started) return false; // If we are started why are we staring again ?
    
    ListenerConfiguration config = configurationService->FetchConfiguration<ListenerConfiguration>();
    
    for (Topic t : config.topics) {
        if (mosquitto_subscribe(komar, NULL, ConvertTopicToString(t).c_str(), 0) != MOSQ_ERR_SUCCESS) {
            throw std::runtime_error("Could not subscribe to a topic.");
        }
        
        std::shared_ptr<TopicWorker> newWorker;
        
        // Check if the Topic `t` is a CAN topic and therefore the corresponding message handler should be put in a CANTopicWorker
        auto optionalCANHandler = messageHandlersService->TryToGetCANMessageHandler(t);
        
        if (optionalCANHandler.has_value()) {
            std::shared_ptr<MessageHandler> canMessageHandler = std::move(optionalCANHandler).value();
            newWorker = std::make_shared<CANTopicWorker>(std::move(canMessageHandler), jruLoggerService, t);
        } else {
            newWorker = std::make_shared<JsonTopicWorker>(messageHandlersService->GetAllHandlers(), jruLoggerService, t);
        }
        
        topicWorkers.insert({t, std::move(newWorker)});
    }

    started = true;
    return true;
}

bool MqttListenerService::GetStarted() const {
    return started;
}

bool MqttListenerService::LpcSaidStop() {
    if (!started) return false;
    
    std::shared_ptr<ListenerConfiguration> config = std::make_shared<ListenerConfiguration>();
    config->from_json(configurationService->FetchConfiguration<ListenerConfiguration>().to_json());
    for (Topic t : config->topics) {
        topicWorkers.at(t)->Stop();
        if (mosquitto_unsubscribe(komar, NULL, ConvertTopicToString(t).c_str()) != MOSQ_ERR_SUCCESS) {
            throw std::runtime_error("Could not unsubscribe from a topic.");
        }
        topicWorkers.erase(t);
    }
    started = false;
    return true;
}

bool MqttListenerService::LpcSaidRestart() {
    // Does nothing
    return true;
}

void MqttListenerService::AppExit() {
    topicWorkers[Topic::LPCtoORC]->Stop();
    mosquitto_unsubscribe(komar, NULL, ConvertTopicToString(Topic::LPCtoORC).c_str());
    topicWorkers.erase(Topic::LPCtoORC);
    mosquitto_disconnect(komar);
}
