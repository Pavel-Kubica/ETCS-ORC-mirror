/** @file BatteryControlCANMessageHandler.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Message handler for battery switch state messages from CAN
 *
 *  ### Contributors
 *  cajantom
 */

#include "BatteryControlCANMessageHandler.hpp"
#include "BatteryControlCANMessage.hpp"

BatteryControlCANMessageHandler::BatteryControlCANMessageHandler(
    ServiceContainer& services
) : MessageHandler(services) {
    jruLoggerService = services.FetchService<JRULoggerService>().get();
}

void BatteryControlCANMessageHandler::HandleMessageBody(const Message& message) {
    const auto& msg = static_cast<const BatteryControlCANMessage&>(message);

    jruLoggerService->Log(true, MessageType::Note, "[CAN] ----> [ORC] || Battery: %battery%",
                          msg.GetBattery());
}

std::unique_ptr<Message> BatteryControlCANMessageHandler::GetEmptyMessage() const {
    return std::make_unique<BatteryControlCANMessage>();
}
