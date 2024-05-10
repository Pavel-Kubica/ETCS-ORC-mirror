/** @file DrivingLeverCANMessage.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  CAN message that updates the driving lever
 *
 *  ###Contributors
 *  cajantom
 */

#pragma once

#include "CANMessage.hpp"
#include "DrivingLeverPosition.hpp"

class DrivingLeverCANMessage : public CANMessage {
public:
    DrivingLeverCANMessage() = default;
    DrivingLeverCANMessage(DrivingLeverPosition position);
    DrivingLeverPosition GetDrivingLeverPosition() const noexcept;
    void FromBitstring(const Bitstring& bits) override;
    Bitstring ToBitstring() const override;

private:
    DrivingLeverPosition drivingLeverPos;
};
