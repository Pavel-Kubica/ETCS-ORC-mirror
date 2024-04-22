/** @file IMessageHandlersService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Interface for MessageHandlersService
 *
 *  ### Contributors
 *  veselo21
 *  zimaluk1
 */

#pragma once

 #include "MessageID.hpp"
 #include "MessageHandler.hpp"
 #include "IService.hpp"
/**
 * Interface for service collecting Message handlers
 */
class IMessageHandlersService : public IService {
public:
    explicit IMessageHandlersService() : IService() {}

    virtual std::map<MessageID, std::shared_ptr<MessageHandler>> GetAllHandlers() const = 0;

    static constexpr ServiceType Type = ServiceType::MessageHandlers;
};
