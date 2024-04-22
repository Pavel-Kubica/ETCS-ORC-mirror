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
 */

#include "MessageHandlersService.hpp"

void MessageHandlersService::Initialize(ServiceContainer& container) {
}

std::map<MessageID, std::shared_ptr<MessageHandler>> MessageHandlersService::GetAllHandlers() const {
    return messageHandlers;
}
