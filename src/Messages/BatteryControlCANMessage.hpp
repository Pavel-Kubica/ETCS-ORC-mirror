/** @file BatteryControlCANMessage.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  CAN message that updates battery control
 *
 *  ###Contributors
 *  cajantom
 */

#pragma once

#include "CANMessage.hpp"

class BatteryControlCANMessage : public CANMessage {
public:
    BatteryControlCANMessage() = default;
    BatteryControlCANMessage(bool battery);
    bool GetBattery() const noexcept;
    void FromBitstring(const Bitstring& bits) override;
    Bitstring ToBitstring() const override;

private:
    bool battery;
};
