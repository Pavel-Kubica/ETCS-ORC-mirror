/** @file TopicWorker.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Abstract class that is resposible for one topic and serves all trafic on that topic
 *
 *  ### Contributors
 *  veselo21
 *  zimaluk1
 *  hamaljan
 *  cajantom
 */

#include "TopicWorker.hpp"
#include <iostream>

void TopicWorker::AddToQueue(const std::string& data) {
    messageQueue.Push(data);
}

void TopicWorker::MessageLoop() {
    while (true) {
        const std::string message = messageQueue.Pop();

        if(message.empty()){
            if(lpcSaidStop){
                jruLoggerService->Log(true, MessageType::Info, "TopicWorker on topic: %topic% is done working.",
                                      ConvertTopicToString(topic));
                return;
            }
            jruLoggerService->Log(true, MessageType::Warning, "TopicWorker on topic: %topic% received empty string.",
                                  ConvertTopicToString(topic));
            continue;
        }

        ProcessMessage(message);
    }
}

TopicWorker::TopicWorker(JRULoggerService* jru, Topic _topic) {
    topic = _topic;
    workThread = std::thread(&TopicWorker::MessageLoop, this);
    jruLoggerService = jru;
    jruLoggerService->Log(true, MessageType::Info, "Started listening on %topic%.", ConvertTopicToString(topic));
}

void TopicWorker::Stop() {
    lpcSaidStop = true;
    messageQueue.Push("");
    workThread.join();
}
