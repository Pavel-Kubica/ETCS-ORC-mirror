/** @file ODOConfigMessageHandler.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Message handler handling ODOConfigMessage from EVC
 *
 *  ### Contributors
 *  veselo21
 *  zimaluk1
 *  hamaljan
 *  pavlian5
 */

#include "ODOConfigMessageHandler.hpp"
#include "ODOConfigMessage.hpp"
#include <iostream>

ODOConfigMessageHandler::ODOConfigMessageHandler(ServiceContainer& container)
    : MessageHandler(container) {
    mqttListener = container.FetchService<IMqttListenerService>().get();
    jruLoggerService = container.FetchService<JRULoggerService>().get();
    simulationStateApiService = container.FetchService<ISimulationStateApiService>().get();
}
void ODOConfigMessageHandler::HandleMessageBody(const Message& message) {
    const auto& msg = static_cast<const ODOConfigMessage&>(message);

    // Log that the message has been received
    jruLoggerService->Log(true, MessageType::Note, "[EVC] ----> [ODO] |401| ODO Config");

    simulationStateApiService->StartSendingOdoMessages(std::chrono::milliseconds(msg.T_ODO_CYC));
}

std::unique_ptr<Message> ODOConfigMessageHandler::GetEmptyMessage() const {
    return std::make_unique<ODOConfigMessage>();
}
