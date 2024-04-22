/** @file TopicWorker.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Class that is resposible for one topic and serve all trafic on that topic
 *
 *  ### Contributors
 *  veselo21
 *  zimaluk1
 *  hamaljan
 */

#include "TopicWorker.hpp"
#include <iostream>
void TopicWorker::AddToQueue(const nlohmann::json& data) {
    messageQueue.Push(data);
}

void TopicWorker::ProcessMessage() {
    while (true) {
        nlohmann::json j = messageQueue.Pop();

        if(j.empty()){
            if(lpcSaidStop){
                jruLoggerService->Log(true, MessageType::Info, "TopicWorker on topic: %topic% is done working.",
                                      ConvertTopicToString(topic));
                return;
            }
            jruLoggerService->Log(true, MessageType::Warning, "TopicWorker on topic: %topic% received empty json.",
                                  ConvertTopicToString(topic));
            continue;
        }

        MessageID messageId;
        try {
            messageId = j.at("NID_MESSAGE").get<MessageID>();
        } catch (nlohmann::detail::out_of_range e) {
            jruLoggerService->Log(true, MessageType::Error,
                                  "Topic worker did not find \"NID_MESSAGE\" in the received message");
            continue;
        }
        if (!messageHandlers.contains(messageId)) {
            jruLoggerService->Log(true, MessageType::Error,
                                  "There is no message handler registred for messageID = %id%", (int) messageId);
            continue;
        }

        std::shared_ptr<MessageHandler> handler = messageHandlers[messageId];
        handler->HandleMessage(j);
    }
}

TopicWorker::TopicWorker(const std::map<MessageID, std::shared_ptr<MessageHandler>>& handlers,
                         JRULoggerService* jru,
                         Topic _topic) {
    messageHandlers = handlers;
    topic = _topic;
    workThread = std::thread(&TopicWorker::ProcessMessage, this);
    jruLoggerService = jru;
    jruLoggerService->Log(true, MessageType::Info, "Started listening on %topic%.", ConvertTopicToString(topic));
}
void TopicWorker::Stop() {
    lpcSaidStop = true;
    messageQueue.Push(nlohmann::json());
    workThread.join();
}
