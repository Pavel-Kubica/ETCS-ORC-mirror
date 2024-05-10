/** @file CANControlMessageHandler.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Message handler for control board state messages from CAN
 *
 *  ### Contributors
 *  cajantom
 *  rehorja8
 */

#pragma once

#include "JRULoggerService.hpp"
#include "MessageHandler.hpp"
#include "IHumanControlDataService.hpp"
#include "ITrainControlUpdateService.hpp"

class CANControlMessageHandler : public MessageHandler {
public:
    explicit CANControlMessageHandler(ServiceContainer& services);

private:
    void HandleMessageBody(const Message& message) override;
    std::unique_ptr<Message> GetEmptyMessage() const override;

    JRULoggerService* jruLoggerService;
    IHumanControlDataService* humanControlDataService;
    ITrainControlUpdateService* trainControlUpdateService;
};
