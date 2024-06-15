/** @file CANControlMessage.cpp
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

#include "CANControlMessage.hpp"

static const std::map<unsigned long long, DirectionLeverPosition> directionLeverSerializations = {
    {0, DirectionLeverPosition::Neutral},
    {1, DirectionLeverPosition::Forwards},
    {2, DirectionLeverPosition::Backwards},
};

static const std::map<DirectionLeverPosition, unsigned long long> directionsToBits = {
    {DirectionLeverPosition::Neutral, 0},
    {DirectionLeverPosition::Forwards, 1},
    {DirectionLeverPosition::Backwards, 2},
};

static const std::map<unsigned long long, DrivingLeverPosition> drivingLeverSerializations = {
    {0x08, DrivingLeverPosition::Continue},
    {0x0c, DrivingLeverPosition::Accelerate},
    {0x10, DrivingLeverPosition::Neutral},
    {0x11, DrivingLeverPosition::QuickBrake},
    {0x12, DrivingLeverPosition::ElectrodynamicBrake},
    {0x13, DrivingLeverPosition::PneumaticBrake},
};

static const std::map<DrivingLeverPosition, unsigned long long> drivingLeverToBits = {
    {DrivingLeverPosition::Continue, 0x08},
    {DrivingLeverPosition::Accelerate, 0x0c},
    {DrivingLeverPosition::Neutral, 0x10},
    {DrivingLeverPosition::QuickBrake, 0x11},
    {DrivingLeverPosition::ElectrodynamicBrake, 0x12},
    {DrivingLeverPosition::PneumaticBrake, 0x13},
};

CANControlMessage::CANControlMessage() {
    NID_MESSAGE = MessageID::CANControls;
}

CANControlMessage::CANControlMessage(DrivingLeverPosition drivingLever,
                                     DirectionLeverPosition directionLever,
                                     bool cab) :
    drivingLever(drivingLever),
    directionLever(directionLever),
    cab(cab)
{
    NID_MESSAGE = MessageID::CANControls;
}

nlohmann::json CANControlMessage::to_json() const {
    UnsupportedSerializationFormat(MessageSerializationFormat::Json);
}

void CANControlMessage::from_json(const nlohmann::json& j) {
    UnsupportedSerializationFormat(MessageSerializationFormat::Json);
}

Bitstring CANControlMessage::ToBitstring() const {
    Bitstring bits = Bitstring::FromHex("ff00ff00ff00ff00");
	bits.Write(cab, 11, 1);
	bits.Write(directionsToBits.at(GetDirectionLever()), 42, 2);
	bits.Write(drivingLeverToBits.at(GetDrivingLever()), 56, 5);
    return bits;
}

void CANControlMessage::FromBitstring(const Bitstring& bits) {
    try {
		cab = bits.At(11, 1);
		directionLever = directionLeverSerializations.at(bits.At(42, 2));
		drivingLever = drivingLeverSerializations.at(bits.At(56, 5));
    }
    catch (const std::out_of_range&) {
        throw std::invalid_argument(__FUNCTION__);
    }
}

DrivingLeverPosition CANControlMessage::GetDrivingLever() const noexcept {
    return drivingLever;
}

DirectionLeverPosition CANControlMessage::GetDirectionLever() const noexcept {
    return directionLever;
}

bool CANControlMessage::GetCabControl() const noexcept {
	return cab;
}
