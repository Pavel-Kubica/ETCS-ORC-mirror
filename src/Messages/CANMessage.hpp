/** @file CANMessage.hpp
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

#pragma once

#include "Message.hpp"

class CANMessage : public Message {
public:
    CANMessage();
	nlohmann::json to_json() const override;
    void from_json(const nlohmann::json& j) override;
};
