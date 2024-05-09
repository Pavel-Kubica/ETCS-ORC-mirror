/** @file DrivingLeverCANMessageHandler.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Message handler for driving lever state messages from CAN
 *
 *  ### Contributors
 *  cajantom
 */

#pragma once

#include "JRULoggerService.hpp"
#include "MessageHandler.hpp"

class DrivingLeverCANMessageHandler : public MessageHandler {
public:
    explicit DrivingLeverCANMessageHandler(ServiceContainer& services);

private:
    void HandleMessageBody(const Message& message) override;
    std::unique_ptr<Message> GetEmptyMessage() const override;

    JRULoggerService* jruLoggerService;
};
