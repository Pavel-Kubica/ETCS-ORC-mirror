/** @file SimulationStatusMessageHandler.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Message handler handling TiuMessage from EVC
 *
 *  ### Contributors
 *  rehorja8
 */
 
#pragma once


#include "MessageHandler.hpp"
#include "TIUMessage.hpp"
#include "JRULoggerService.hpp"
#include "ITrainControlUpdateService.hpp"
#include "IMachineControlDataService.hpp"

class TiuMessageHandler : public MessageHandler {
public:
    explicit TiuMessageHandler(ServiceContainer& container);
    [[nodiscard]] std::unique_ptr<Message> GetEmptyMessage() const override;
protected:
    void HandleMessageBody(const Message& data) override;
    
    IMachineControlDataService* machineControlDataService;
    ITrainControlUpdateService* trainService;
    JRULoggerService* jruLoggerService;
};
