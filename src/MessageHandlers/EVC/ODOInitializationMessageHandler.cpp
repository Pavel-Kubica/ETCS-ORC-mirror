/** @file ODOInitializationMessageHandler.cpp
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

#include "ODOInitializationMessageHandler.hpp"
#include <iostream>
#include "ODOInitializationMessage.hpp"

ODOInitializationMessageHandler::ODOInitializationMessageHandler(ServiceContainer& container)
    : MessageHandler(container) {
    mqttListener = container.FetchService<IMqttListenerService>().get();
    jruLoggerService = container.FetchService<JRULoggerService>().get();
    simulationStateApiService = container.FetchService<ISimulationStateApiService>().get();
}
void ODOInitializationMessageHandler::HandleMessageBody(const Message& message) {
    const auto& msg = static_cast<const ODOInitializationMessage&>(message);

    // Log that the message has been received
    jruLoggerService->Log(true, MessageType::Note, "[EVC] ----> [ODO] |406| ODO Initialization");

    Distance distance;
    distance.SetDistanceFromCm(msg.D_TRAIN);

    simulationStateApiService->SetStartingPoint(distance);
}

std::unique_ptr<Message> ODOInitializationMessageHandler::GetEmptyMessage() const {
    return std::make_unique<ODOInitializationMessage>();
}
