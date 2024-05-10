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

#include "CANControlMessage.hpp"
#include "CANControlMessageHandler.hpp"

CANControlMessageHandler::CANControlMessageHandler(
    ServiceContainer& services
) : MessageHandler(services) {
    jruLoggerService = services.FetchService<JRULoggerService>().get();
    humanControlDataService = services.FetchService<IHumanControlDataService>().get();
    trainControlUpdateService = services.FetchService<ITrainControlUpdateService>().get();
}

void CANControlMessageHandler::HandleMessageBody(const Message& message) {
    const auto& msg = static_cast<const CANControlMessage&>(message);

    jruLoggerService->Log(true, MessageType::Note, "[CAN] ----> [ORC] || CAN control");

    humanControlDataService->SetDrivingLever(msg.GetDrivingLever());
    humanControlDataService->SetTrainDirection(msg.GetDirectionLever());
    trainControlUpdateService->Update();
}

std::unique_ptr<Message> CANControlMessageHandler::GetEmptyMessage() const {
    return std::make_unique<CANControlMessage>();
}
