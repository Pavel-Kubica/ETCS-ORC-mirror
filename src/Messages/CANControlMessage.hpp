/** @file CANControlMessage.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Message with control board state from CAN
 *
 *  ###Contributors
 *  cajantom
 */

#pragma once

#include "Message.hpp"
#include "DrivingLeverPosition.hpp"
#include "DirectionLeverPosition.hpp"

class CANControlMessage : public Message {
public:
    CANControlMessage();
    CANControlMessage(DrivingLeverPosition drivingLever,
                      DirectionLeverPosition directionLever,
                      bool cab);
    void from_json(const nlohmann::json& j) override;
    nlohmann::json to_json() const override;
    void FromBitstring(const Bitstring& bits) override;
    Bitstring ToBitstring() const override;
    DrivingLeverPosition GetDrivingLever() const noexcept;
    DirectionLeverPosition GetDirectionLever() const noexcept;
	bool GetCabControl() const noexcept;

private:
    DrivingLeverPosition drivingLever;
    DirectionLeverPosition directionLever;
	bool cab;
};
