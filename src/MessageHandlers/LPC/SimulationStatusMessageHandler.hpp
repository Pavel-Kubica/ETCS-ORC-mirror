/** @file SimulationStatusMessageHandler.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Message handler handling start/stop/restart messages from LPC
 *
 *  ### Contributors
 *  veselo21
 *  zimaluk1
 *  maresj39
 *  hamaljan
 *  pavlian5
 */

#pragma once

 #include "JRULoggerService.hpp"
 #include "IMqttListenerService.hpp"
 #include "MessageHandler.hpp"
/**
 * Handles message 1150: SimulationStatusLPCtoORC
 */
class SimulationStatusMessageHandler : public MessageHandler {
public:
    explicit SimulationStatusMessageHandler(ServiceContainer& container);

protected:
    void HandleMessageBody(const Message& data) override;

    [[nodiscard]] std::unique_ptr<Message> GetEmptyMessage() const override;

private:
    IMqttListenerService* mqttListener;
    JRULoggerService* jruLoggerService;
    ServiceContainer& serviceContainer;
};
