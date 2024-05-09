/** @file JsonTopicWorker.hpp
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

#pragma once

#include "TopicWorker.hpp"

class JsonTopicWorker : public TopicWorker {
public:
    /**
     * @param handlers map of handlers <MessageId, shared_ptr<AMessageHandler>> that the topic worker can call
     * @param jru for logging purposes
     * @param topic to later know which topic-workers are done working (mostly for clarity)
     */
    JsonTopicWorker(const std::map<MessageID, std::shared_ptr<MessageHandler>>& handlers,
                    JRULoggerService* jru,
                    Topic topic);

private:
    void ProcessMessage(const std::string& message) override;

    std::map<MessageID, std::shared_ptr<MessageHandler>> messageHandlers;
};
