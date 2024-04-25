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
 */

#pragma once

 #include "ServiceContainer.hpp"
 #include "ITimeService.hpp"

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
     * Deserializes the json into it's corresponding message and handles the message
     * @param data Message in nlohmann:json data
     */
    void HandleMessage(const nlohmann::json& data) { HandleMessageBody(data); }

protected:
    virtual void HandleMessageBody(const nlohmann::json& data) = 0;

    ITimeService* timeService;
};
