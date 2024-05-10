/** @file FromTIUMessageHandler.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Abstract class responsible for basic message handling behaviour
 *
 *  ### Contributors
 *  kaufmlu1
 *  zimaluk1
 *  veselo21
 *  hamaljan
 *  cajantom
 */

#pragma once

#include <memory>

#include "ServiceContainer.hpp"
#include "ITimeService.hpp"

#include "Message.hpp"
#include "MessageID.hpp"
#include "json.hpp"

/**
 * Abstract class responsible for basic message handling behaviour
 */
class MessageHandler {
public:
    explicit MessageHandler(ServiceContainer& container) {
        timeService = container.FetchService<ITimeService>().get();
    }
    /**
     * Handles the message
     * @param message The message. Must be the same runtime type as the message returned by GetEmptyMessage
     */
    void HandleMessage(const Message& data) { HandleMessageBody(data); }
    /**
     * Creates an empty message of the type accepted by the handler
     */
    virtual std::unique_ptr<Message> GetEmptyMessage() const = 0;

protected:
    virtual void HandleMessageBody(const Message& data) = 0;

    ITimeService* timeService;
};
