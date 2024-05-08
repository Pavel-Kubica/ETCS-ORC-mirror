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
#include "SimulationStatusMessageHandler.hpp"

void MessageHandlersService::Initialize(ServiceContainer& container) {
    messageHandlers[MessageID::SimulationStatusLPCtoORC] = std::make_shared<SimulationStatusMessageHandler>(container);
}

std::map<MessageID, std::shared_ptr<MessageHandler>> MessageHandlersService::GetAllHandlers() const {
    return messageHandlers;
}
