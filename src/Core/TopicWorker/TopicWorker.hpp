/** @file TopicWorker.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Class that is responsible for one topic and serve all traffic on that topic
 *
 *  ### Contributors
 *  veselo21
 *  zimaluk1
 *  maresj39
 *  hamaljan
 */

#pragma once

#include <map>
#include <thread>

#include "MessageID.hpp"
#include "Message.hpp"
#include "MessageHandler.hpp"
#include "JRULoggerService.hpp"
#include "AsyncQueue.hpp"

class TopicWorker {
public:
    /**
     * @param handlers map of handlers <MessageId, shared_ptr<AMessageHandler>> that the topic worker can call
     * @param jru for logging purposes
     * @param topic to later know which topic-workers are done working (mostly for clarity)
     */
    explicit TopicWorker(const std::map<MessageID, std::shared_ptr<MessageHandler>>& handlers,
                         JRULoggerService* jru,
                         Topic topic);
    ~TopicWorker() = default;
    /**
     * Adds a message in json format to queue for processing
     * @param data json format of received message that is to be passed to message handlers
     */
    void AddToQueue(const nlohmann::json& data);
    /**
     * Stops the working process by adding empty json to queue and setting exit to true
     */
    void Stop();

private:
    /**
     * The work loop of the topic worker
     * It takes messages from the messageQueue and processes them by
     * sending them to the relevant messageHandlers
     */
    void ProcessMessage();
    AsyncQueue<nlohmann::json> messageQueue;
    std::map<MessageID, std::shared_ptr<MessageHandler>> messageHandlers;
    std::thread workThread;
    JRULoggerService* jruLoggerService;
    bool lpcSaidStop = false;
    Topic topic;
};
