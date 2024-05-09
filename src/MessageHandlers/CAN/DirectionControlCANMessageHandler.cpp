/** @file DirectionControlCANMessageHandler.cpp
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

#include "DirectionControlCANMessageHandler.hpp"
#include "DirectionControlCANMessage.hpp"

DirectionControlCANMessageHandler::DirectionControlCANMessageHandler(
    ServiceContainer& services
) : MessageHandler(services) {
    jruLoggerService = services.FetchService<JRULoggerService>().get();
    humanControlDataService = services.FetchService<IHumanControlDataService>().get();
    trainControlUpdateService = services.FetchService<ITrainControlUpdateService>().get();
}

void DirectionControlCANMessageHandler::HandleMessageBody(const Message& message) {
    const auto& msg = static_cast<const DirectionControlCANMessage&>(message);

    jruLoggerService->Log(true, MessageType::Note, "[CAN] ----> [ORC] || Control: %battery%, Direction: %direction%",
                          msg.GetControl(), msg.GetDirectionLeverPosition());
    humanControlDataService->SetTrainDirection(msg.GetDirectionLeverPosition());
    humanControlDataService->SetCab(msg.GetControl());
    trainControlUpdateService->Update();
}

std::unique_ptr<Message> DirectionControlCANMessageHandler::GetEmptyMessage() const {
    return std::make_unique<DirectionControlCANMessage>();
}
