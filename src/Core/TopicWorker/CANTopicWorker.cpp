/** @file CANTopicWorker.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Implementation of topic worker for handling CAN hexstring messages
 *
 *  ### Contributors
 *  cajantom
 */

#include <sstream>
#include <bitset>
#include <algorithm>

#include "CANTopicWorker.hpp"
#include "Bitstring.hpp"

CANTopicWorker::CANTopicWorker(std::shared_ptr<MessageHandler> handler,
                               JRULoggerService* jru,
                               Topic topic) :
    TopicWorker(jru, topic),
    messageHandler(std::move(handler))
{}

void CANTopicWorker::ProcessMessage(const std::string& message) {
    std::string messageLittleEndian = message;
    std::reverse(messageLittleEndian.begin(), messageLittleEndian.end());

    Bitstring bits;
    try {
        bits = Bitstring::FromHex(messageLittleEndian);
    }
    catch (const std::invalid_argument&) {
        jruLoggerService->Log(true, MessageType::Error, "CAN message contains a non-hexadecimal character");
        return;
    }

    std::unique_ptr<Message> msg = messageHandler->GetEmptyMessage();
    try {
        msg.get()->FromBitstring(bits);
    }
    catch (const std::invalid_argument& e) {
        jruLoggerService->Log(true, MessageType::Error, "Message received was in wrong format. e: %e%", e.what());
        return;
    }
    messageHandler->HandleMessage(*msg);
}
