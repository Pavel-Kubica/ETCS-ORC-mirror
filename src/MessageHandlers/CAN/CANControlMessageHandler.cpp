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
 *  kubicpa3
 */

#include "CANControlMessageHandler.hpp"
#include "CANControlMessage.hpp"

CANControlMessageHandler::CANControlMessageHandler(ServiceContainer& services) : MessageHandler(services) {
    jruLoggerService = services.FetchService<JRULoggerService>().get();
    humanControlDataService = services.FetchService<IHumanControlDataService>().get();
    trainControlUpdateService = services.FetchService<ITrainControlUpdateService>().get();
}

void CANControlMessageHandler::HandleMessageBody(const Message& message) {
    const auto& msg = static_cast<const CANControlMessage&>(message);
    DrivingLeverPosition drivingLeverPosition = msg.GetDrivingLever();
    DirectionLeverPosition directionLeverPosition = msg.GetDirectionLever();
    EngineBrakeLeverPosition engineBrakeLeverPosition = msg.GetEngineBrakeLever();
    // TODO is this pantograph behavior correct?
    bool pantograph = msg.GetPantograph() != PantographPosition::Down;
    bool horn = msg.GetHorn();
    bool sander = msg.GetSander();
    bool emergencyBrake = msg.GetGeneralStop();

    if (drivingLeverPosition == humanControlDataService->GetDrivingLever() &&
        directionLeverPosition == humanControlDataService->GetTrainDirection() &&
        engineBrakeLeverPosition == humanControlDataService->GetEngineBrake() &&
        pantograph == humanControlDataService->GetPantograph() &&
        horn == humanControlDataService->GetHorn() &&
        sander == humanControlDataService->GetSander() &&
        emergencyBrake == humanControlDataService->GetEmergencyBrake()) {
        jruLoggerService->Log(MessageType::Note,
                              "[CAN] ----> [ORC] || CAN control WITH NO CHANGE. "
                              "[Main lever: %drivingLeverPosition%] "
                              "[Direction lever:%directionLever%]",
                              drivingLeverPosition, directionLeverPosition);
        return;
    }

    jruLoggerService->Log(true, MessageType::Info,
                          "[CAN] ----> [ORC] || CAN control"
                          "[Main lever: %drivingLeverPosition%] "
                          "[Direction lever:%directionLever%]",
                          drivingLeverPosition, directionLeverPosition);

    humanControlDataService->SetDrivingLever(drivingLeverPosition);
    humanControlDataService->SetTrainDirection(directionLeverPosition);
    humanControlDataService->SetEngineBrake(engineBrakeLeverPosition);
    humanControlDataService->SetPantograph(pantograph);
    humanControlDataService->SetHorn(horn);
    humanControlDataService->SetSander(sander);
    humanControlDataService->SetEmergencyBrake(emergencyBrake);
    trainControlUpdateService->Update();
}

std::unique_ptr<Message> CANControlMessageHandler::GetEmptyMessage() const {
    return std::make_unique<CANControlMessage>();
}
