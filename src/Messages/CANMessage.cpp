/** @file CANMessage.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Base class for messages that come from CAN
 *
 *  ###Contributors
 *  cajantom
 */

#include "CANMessage.hpp"

CANMessage::CANMessage() {
    NID_MESSAGE = MessageID::Invalid;
}

nlohmann::json CANMessage::to_json() const {
    UnsupportedSerializationFormat(MessageSerializationFormat::Json);
}

void CANMessage::from_json(const nlohmann::json& j) {
    UnsupportedSerializationFormat(MessageSerializationFormat::Json);
}
