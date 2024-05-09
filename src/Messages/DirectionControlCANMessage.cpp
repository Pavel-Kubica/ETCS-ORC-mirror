/** @file DirectionControlCANMessage.cpp
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

#include "DirectionControlCANMessage.hpp"
#include <map>

static const std::map<unsigned long long, bool> controlSerializations = {
    {1, false}, {2, true}
};

static const std::map<unsigned long long, DirectionLeverPosition> directionLeverSerializations = {
    {1, DirectionLeverPosition::Neutral},
    {2, DirectionLeverPosition::Forwards},
    {0x80, DirectionLeverPosition::Backwards},
};

static const std::map<DirectionLeverPosition, unsigned long long> directionsToBits = {
    {DirectionLeverPosition::Neutral, 1},
    {DirectionLeverPosition::Forwards, 2},
    {DirectionLeverPosition::Backwards, 0x80},
};

DirectionControlCANMessage::DirectionControlCANMessage(
    DirectionLeverPosition direction,
    bool control) :
    directionLeverPos(direction),
    control(control)
{}

DirectionLeverPosition DirectionControlCANMessage::GetDirectionLeverPosition() const noexcept {
    return directionLeverPos;
}

bool DirectionControlCANMessage::GetControl() const noexcept {
    return control;
}

void DirectionControlCANMessage::FromBitstring(const Bitstring& bits) {
    try {
        control = controlSerializations.at(bits.At(21, 2));
        directionLeverPos = directionLeverSerializations.at(bits.At(16, 8) & 0x83);
    }
    catch (const std::out_of_range&) {
        throw std::invalid_argument(__FUNCTION__);
    }
}

Bitstring DirectionControlCANMessage::ToBitstring() const {
    Bitstring bits(64);
    bits.Write(directionsToBits.at(directionLeverPos), 16, 8);
    bits.Write(1 + static_cast<unsigned long long>(control), 21, 2);
    return bits;
}
