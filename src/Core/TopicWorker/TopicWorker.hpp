/** @file TopicWorker.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Abstract class that is responsible for one topic and serves all traffic on that topic
 *
 *  ### Contributors
 *  veselo21
 *  zimaluk1
 *  maresj39
 *  hamaljan
 *  cajantom
 */

#pragma once

#include <map>
#include <thread>
#include <string>

#include "MessageID.hpp"
#include "Message.hpp"
#include "MessageHandler.hpp"
#include "JRULoggerService.hpp"
#include "AsyncQueue.hpp"

class TopicWorker {
public:
    /**
     * @param jru for logging purposes
     * @param topic to later know which topic-workers are done working (mostly for clarity)
     */
    explicit TopicWorker(JRULoggerService* jru, Topic topic);
    /**
     * Adds a message in to queue for processing on the TopicWorker's thread
     * @param message Content of received message that is to be passed to message handlers
     */
    void AddToQueue(const std::string& message);
    /**
     * Stops the working process by adding empty string to queue and setting exit to true
     * @todo Allow closing the message queue in a non-janky way
     */
    void Stop();

protected:
    /**
     * Parses and dispatches a message
     * How this is done depends on the format of the message
     */
    virtual void ProcessMessage(const std::string& message) = 0;

    JRULoggerService* jruLoggerService;
    Topic topic;

private:
    /**
     * The work loop of the topic worker
     * It takes messages from the messageQueue and processes them by
     * sending them to the relevant messageHandlers
     */
    void MessageLoop();

    AsyncQueue<std::string> messageQueue;
    std::thread workThread;
    bool lpcSaidStop = false;
};
