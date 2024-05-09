#include "TiuMessageHandler.hpp"

std::unique_ptr<Message> TiuMessageHandler::GetEmptyMessage() const {
    return std::make_unique<TIUMessage>();
}

void TiuMessageHandler::HandleMessageBody(const Message& message) {
    const auto& msg = static_cast<const TIUMessage&>(message);
    
    this->machineControlDataService->SetServiceBrake(msg.GetServiceBrake());
    
    // TODO: mark GetEmergencyBrake() as const in CEM
    bool emergencyBrake = const_cast<TIUMessage&>(msg).GetEmergencyBrake();
    this->machineControlDataService->SetEmergencyBrake(emergencyBrake);
    
    // Log that the message has been received
    jruLoggerService->Log(true,
                          MessageType::Note,
                          "[EVC] ----> [TIU] |601| Received TIUMessage {serviceBrake:"
                          + std::to_string(msg.GetServiceBrake())
                          + "|emergencyBrake:"
                          + std::to_string(emergencyBrake)
                          + "}"
    );
    
    this->trainService->Update();
}

TiuMessageHandler::TiuMessageHandler(ServiceContainer& container)
        : MessageHandler(container) {
    this->jruLoggerService = container.FetchService<JRULoggerService>().get();
    this->machineControlDataService = container.FetchService<IMachineControlDataService>().get();
    this->trainService = container.FetchService<ITrainControlUpdateService>().get();
}
