/** @file DirectionControlCANMessageHandler.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Message handler for direction and control switch state messages from CAN
 *
 *  ### Contributors
 *  cajantom
 */

#pragma once

#include "JRULoggerService.hpp"
#include "MessageHandler.hpp"

class DirectionControlCANMessageHandler : public MessageHandler {
public:
    explicit DirectionControlCANMessageHandler(ServiceContainer& services);

private:
    void HandleMessageBody(const Message& message) override;
    std::unique_ptr<Message> GetEmptyMessage() const override;

    JRULoggerService* jruLoggerService;
};
