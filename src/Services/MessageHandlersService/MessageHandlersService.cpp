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
 */

#include "MessageHandlersService.hpp"
#include "TiuMessageHandler.hpp"
#include "SimulationStatusMessageHandler.hpp"
#include "ODOConfigMessageHandler.hpp"
#include "ODOInitializationMessageHandler.hpp"

void MessageHandlersService::Initialize(ServiceContainer& container) {
    messageHandlers[MessageID::SimulationStatusLPCtoORC] = std::make_shared<SimulationStatusMessageHandler>(container);
    messageHandlers[MessageID::ODOConfig] = std::make_shared<ODOConfigMessageHandler>(container);
    messageHandlers[MessageID::ODOInitialization] = std::make_shared<ODOInitializationMessageHandler>(container);
    messageHandlers[MessageID::ToTIU] = std::make_shared<TiuMessageHandler>(container);
}

std::map<MessageID, std::shared_ptr<MessageHandler>> MessageHandlersService::GetAllHandlers() const {
    return messageHandlers;
}
