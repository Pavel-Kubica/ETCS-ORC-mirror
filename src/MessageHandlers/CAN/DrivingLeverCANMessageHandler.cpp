/** @file DrivingLeverCANMessageHandler.cpp
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

#include "DrivingLeverCANMessage.hpp"
#include "DrivingLeverCANMessageHandler.hpp"

DrivingLeverCANMessageHandler::DrivingLeverCANMessageHandler(
    ServiceContainer& services
) : MessageHandler(services) {
    jruLoggerService = services.FetchService<JRULoggerService>().get();
    humanControlDataService = services.FetchService<IHumanControlDataService>().get();
    trainControlUpdateService = services.FetchService<ITrainControlUpdateService>().get();
}

void DrivingLeverCANMessageHandler::HandleMessageBody(const Message& message) {
    const auto& msg = static_cast<const DrivingLeverCANMessage&>(message);

    jruLoggerService->Log(true, MessageType::Note, "[CAN] ----> [ORC] || Driving lever: %position%",
                          msg.GetDrivingLeverPosition());
    humanControlDataService->SetDrivingLever(msg.GetDrivingLeverPosition());
    trainControlUpdateService->Update();
}

std::unique_ptr<Message> DrivingLeverCANMessageHandler::GetEmptyMessage() const {
    return std::make_unique<DrivingLeverCANMessage>();
}
