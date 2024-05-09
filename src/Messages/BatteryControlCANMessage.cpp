/** @file BatteryControlCANMessage.cpp
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

#include "BatteryControlCANMessage.hpp"
#include <map>

static const std::map<unsigned long long, bool> batterySerializations = {
    {1, true}, {2, false}
};

BatteryControlCANMessage::BatteryControlCANMessage(bool battery) :
    battery(battery)
{}

bool BatteryControlCANMessage::GetBattery() const noexcept {
    return battery;
}

void BatteryControlCANMessage::FromBitstring(const Bitstring& bits) {
    try {
        battery = batterySerializations.at(bits.At(20, 2));
    }
    catch (const std::out_of_range&) {
        throw std::invalid_argument(__FUNCTION__);
    }
}

Bitstring BatteryControlCANMessage::ToBitstring() const {
    Bitstring bits(64);
    bits.Write(2 - static_cast<unsigned long long>(battery), 20, 2);
    return bits;
}
