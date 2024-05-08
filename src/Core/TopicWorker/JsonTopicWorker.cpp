/** @file JsonTopicWorker.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Implementation of topic worker for handling messages in JSON format
 *
 *  ### Contributors
 *  veselo21
 *  zimaluk1
 *  hamaljan
 *  cajantom
 */

#include "JsonTopicWorker.hpp"

JsonTopicWorker::JsonTopicWorker(const std::map<MessageID, std::shared_ptr<MessageHandler>>& handlers,
                                 JRULoggerService* jru,
                                 Topic topic) :
    TopicWorker(jru, topic),
    messageHandlers(handlers)
{}

void JsonTopicWorker::ProcessMessage(const std::string& message) {
    nlohmann::json data;
    try {
        data = nlohmann::json::parse(message);
    }
    catch (nlohmann::json::parse_error e) {
        jruLoggerService->Log(true, MessageType::Error, "Received message with invalid json format.");
        return;
    }

    MessageID messageId;
    try {
        messageId = data.at("NID_MESSAGE").get<MessageID>();
    }
    catch (nlohmann::detail::out_of_range e) {
        jruLoggerService->Log(true, MessageType::Error,
            "Topic worker did not find \"NID_MESSAGE\" in the received message");
        return;
    }
    if (!messageHandlers.contains(messageId)) {
        jruLoggerService->Log(true, MessageType::Error,
            "There is no message handler registred for messageID = %id%", (int)messageId);
        return;
    }

    MessageHandler* handler = messageHandlers[messageId].get();
    std::unique_ptr<Message> msg = handler->GetEmptyMessage();
    try {
        msg->from_json(data);
    }
    catch (nlohmann::json::out_of_range& e) {
        jruLoggerService->Log(true, MessageType::Error, "Message received was in wrong format. e: %e%", e.what());
        return;
    }
    handler->HandleMessage(*msg);
}
