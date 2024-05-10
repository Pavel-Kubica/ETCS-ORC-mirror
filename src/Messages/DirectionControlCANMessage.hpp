/** @file DirectionControlCANMessage.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  CAN message that updates direction and control switches
 *
 *  ###Contributors
 *  cajantom
 */

#pragma once

#include "CANMessage.hpp"
#include "DirectionLeverPosition.hpp"

class DirectionControlCANMessage : public CANMessage {
public:
    DirectionControlCANMessage() = default;
    DirectionControlCANMessage(DirectionLeverPosition direction,
                               bool control);
    DirectionLeverPosition GetDirectionLeverPosition() const noexcept;
    bool GetControl() const noexcept;
    void FromBitstring(const Bitstring& bits) override;
    Bitstring ToBitstring() const override;

private:
    DirectionLeverPosition directionLeverPos;
    bool control;
};
