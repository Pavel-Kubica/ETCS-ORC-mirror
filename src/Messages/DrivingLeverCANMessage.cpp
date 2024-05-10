/** @file DrivingLeverCANMessage.cpp
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

#include "DrivingLeverCANMessage.hpp"
#include <map>

static const std::map<unsigned long long, DrivingLeverPosition> drivingLeverSerializations = {
    {0xf7, DrivingLeverPosition::PneumaticBrake},
    {0xbd, DrivingLeverPosition::ElectrodynamicBrake},
    {0x77, DrivingLeverPosition::Neutral},
    {0xdb, DrivingLeverPosition::Continue},
    {0xeb, DrivingLeverPosition::Accelerate},
    {0xc00000, DrivingLeverPosition::QuickBrake},
};

static const std::map<DrivingLeverPosition, unsigned long long> drivingLeverToBits = {
    {DrivingLeverPosition::PneumaticBrake, 0xf7},
    {DrivingLeverPosition::ElectrodynamicBrake, 0xbd},
    {DrivingLeverPosition::Neutral, 0x77},
    {DrivingLeverPosition::Continue, 0xdb},
    {DrivingLeverPosition::Accelerate, 0xeb},
    {DrivingLeverPosition::QuickBrake, 0xc00000},
};

DrivingLeverCANMessage::DrivingLeverCANMessage(DrivingLeverPosition position) :
    drivingLeverPos(position)
{}

DrivingLeverPosition DrivingLeverCANMessage::GetDrivingLeverPosition() const noexcept {
    return drivingLeverPos;
}

void DrivingLeverCANMessage::FromBitstring(const Bitstring& bits) {
    try {
        drivingLeverPos = drivingLeverSerializations.at(bits.At(0, 24));
    }
    catch (const std::out_of_range&) {
        throw std::invalid_argument(__FUNCTION__);
    }
}

Bitstring DrivingLeverCANMessage::ToBitstring() const {
    Bitstring bits(64);
    bits.Write(drivingLeverToBits.at(drivingLeverPos), 0, 24);
    return bits;
}
