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
 */

// TODO: change LPCtoEVC to LPCtoORC in the whole file

#include "MqttListenerService.hpp"
#include "ListenerConfiguration.hpp"


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

    mosquitto_subscribe(komar, NULL, ConvertTopicToString(Topic::LPCtoEVC).c_str(), 0);
    topicWorkers.insert({Topic::LPCtoEVC, std::make_shared<TopicWorker>(messageHandlersService->GetAllHandlers(),
                                                                        jruLoggerService, Topic::LPCtoEVC)});
}

void MqttListenerService::Start(ServiceContainer& container) {
    work_thread = std::thread(&MqttListenerService::Listen, this);
}

void MqttListenerService::MQTTCallback(struct mosquitto* mosquitto, const struct mosquitto_message* message) {
    nlohmann::json data = nlohmann::json::parse(static_cast<char*>(message->payload));
    try {
        data = nlohmann::json::parse(static_cast<char*>(message->payload));
    } catch (nlohmann::json::parse_error e) {
        jruLoggerService->Log(true, MessageType::Error, "Received message with invalid json format.");
        return;
    }
    std::string topic(message->topic);
    std::shared_ptr<TopicWorker> worker = topicWorkers[ConvertStringToTopic(topic)];
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

void MqttListenerService::LpcSaidStart() {
    ListenerConfiguration config = configurationService->FetchConfiguration<ListenerConfiguration>();

    for (Topic t : config.topics) {
        mosquitto_subscribe(komar, NULL, ConvertTopicToString(t).c_str(), 0);
        topicWorkers.insert(
            {t, std::make_shared<TopicWorker>(messageHandlersService->GetAllHandlers(), jruLoggerService, t)});
    }

    started = true;
}

bool MqttListenerService::GetStarted() const {
    return started;
}

void MqttListenerService::LpcSaidStop() {
    if(!started) return;
    std::shared_ptr<ListenerConfiguration> config = std::make_shared<ListenerConfiguration>();
    config->from_json(configurationService->FetchConfiguration<ListenerConfiguration>().to_json());
    for (Topic t : config->topics) {
        topicWorkers.at(t)->Stop();
        mosquitto_unsubscribe(komar, NULL, ConvertTopicToString(t).c_str());
        topicWorkers.erase(t);
    }
    started = false;
}

void MqttListenerService::LpcSaidRestart() {
    // Does nothing
}
void MqttListenerService::AppExit() {
    topicWorkers[Topic::LPCtoEVC]->Stop();
    mosquitto_unsubscribe(komar, NULL, ConvertTopicToString(Topic::LPCtoEVC).c_str());  // TODO: change LPCtoEVC to LPCtoORC
    topicWorkers.erase(Topic::LPCtoEVC);
    mosquitto_disconnect(komar);
}
