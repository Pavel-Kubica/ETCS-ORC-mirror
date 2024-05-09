/** @file BatteryControlCANMessageHandler.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Message handler for battery switch state messages from CAN
 *
 *  ### Contributors
 *  cajantom
 */

#pragma once

#include "MessageHandler.hpp"
#include "JRULoggerService.hpp"
#include "IHumanControlDataService.hpp"
#include "ITrainControlUpdateService.hpp"

class BatteryControlCANMessageHandler : public MessageHandler {
public:
    explicit BatteryControlCANMessageHandler(ServiceContainer& services);

private:
    void HandleMessageBody(const Message& message) override;
    std::unique_ptr<Message> GetEmptyMessage() const override;

    JRULoggerService* jruLoggerService;
    IHumanControlDataService* humanControlDataService;
    ITrainControlUpdateService* trainControlUpdateService;
};
