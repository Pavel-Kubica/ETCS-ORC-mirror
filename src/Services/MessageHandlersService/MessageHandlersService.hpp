/** @file MessageHandlersService.hpp
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
 *  rehorja8
 */

#pragma once

#include <map>
 #include "IInitializable.hpp"
#include "IMessageHandlersService.hpp"
#include "MessageHandlers/MessageHandler.hpp"

/*
 * Service that is responsible for collecting and giving handlers for topic worker creation
 */
class MessageHandlersService : public IMessageHandlersService, public IInitializable {
public:
    void Initialize(ServiceContainer& container) override;
    std::map<MessageID, std::shared_ptr<MessageHandler>> GetAllHandlers() const override;
    [[nodiscard]] std::optional<std::shared_ptr<MessageHandler>> TryToGetCANMessageHandler(Topic topic) const override;
private:
    std::map<MessageID, std::shared_ptr<MessageHandler>> messageHandlers;
    
    std::map<Topic, std::shared_ptr<MessageHandler>> canMessageHandlers;
};
