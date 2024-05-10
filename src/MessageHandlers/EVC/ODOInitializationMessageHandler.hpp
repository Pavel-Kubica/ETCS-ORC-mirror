/** @file ODOInitializationMessageHandler.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Message handler handling ODOInitializationMessage from EVC
 *
 *  ### Contributors
 *  pavlian5
 */

#pragma once

#include "JRULoggerService.hpp"
#include "IMqttListenerService.hpp"
#include "MessageHandler.hpp"
#include "ISimulationStateApiService.hpp"

/**
 * Handles message 406: OdoInitialization
 */
class ODOInitializationMessageHandler : public MessageHandler {
public:
    explicit ODOInitializationMessageHandler(ServiceContainer& container);

protected:
    void HandleMessageBody(const Message& data) override;

    [[nodiscard]] std::unique_ptr<Message> GetEmptyMessage() const override;

private:
    IMqttListenerService* mqttListener;
    JRULoggerService* jruLoggerService;
    ISimulationStateApiService* simulationStateApiService;
};

