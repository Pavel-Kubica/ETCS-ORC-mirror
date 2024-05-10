/** @file MessageHandlersService.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Service that is responsible for giving handlers for topic worker creation
 *
 *  ### Contributors
 *  veselo21
 *  zimaluk1
 *  pavlian5
 *  rehorja8
 */

#include "MessageHandlersService.hpp"
#include "TiuMessageHandler.hpp"
#include "SimulationStatusMessageHandler.hpp"
#include "ODOConfigMessageHandler.hpp"
#include "ODOInitializationMessageHandler.hpp"
#include "CANControlMessageHandler.hpp"

void MessageHandlersService::Initialize(ServiceContainer& container) {
    messageHandlers[MessageID::SimulationStatusLPCtoORC] = std::make_shared<SimulationStatusMessageHandler>(container);
    messageHandlers[MessageID::ODOConfig] = std::make_shared<ODOConfigMessageHandler>(container);
    messageHandlers[MessageID::ODOInitialization] = std::make_shared<ODOInitializationMessageHandler>(container);
    messageHandlers[MessageID::ToTIU] = std::make_shared<TiuMessageHandler>(container);
    
    canMessageHandlers[Topic::CANtoORC] = std::make_shared<CANControlMessageHandler>(container);
}

std::map<MessageID, std::shared_ptr<MessageHandler>> MessageHandlersService::GetAllHandlers() const {
    return messageHandlers;
}

std::optional<std::shared_ptr<MessageHandler>> MessageHandlersService::TryToGetCANMessageHandler(Topic topic) const {
    auto handlerIt = this->canMessageHandlers.find(topic);
    if (handlerIt == this->canMessageHandlers.end()) {
        return {}; // Empty optional
    }
    
    std::shared_ptr<MessageHandler> handler = handlerIt->second;
    return {std::move(handler)};
}
