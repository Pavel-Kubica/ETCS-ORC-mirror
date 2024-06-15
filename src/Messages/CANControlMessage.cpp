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

static const std::map<unsigned long long, EngineBrakeLeverPosition> engineBrakeSerializations = {
    {0x11, EngineBrakeLeverPosition::FullRelease},
    {0x01, EngineBrakeLeverPosition::Release},
    {0x02, EngineBrakeLeverPosition::Neutral},
    {0x0a, EngineBrakeLeverPosition::Engage},
    {0x0c, EngineBrakeLeverPosition::FullPower},
};

static const std::map<EngineBrakeLeverPosition, unsigned long long> engineBrakeToBits = {
    {EngineBrakeLeverPosition::FullRelease, 0x11},
    {EngineBrakeLeverPosition::Release, 0x01},
    {EngineBrakeLeverPosition::Neutral, 0x02},
    {EngineBrakeLeverPosition::Engage, 0x0a},
    {EngineBrakeLeverPosition::FullPower, 0x0c},
};

static const std::map<unsigned long long, PantographPosition> pantographSerializations = {
    {1, PantographPosition::Down},
    {2, PantographPosition::Up},
    {4, PantographPosition::UpConnected},
};

static const std::map<PantographPosition, unsigned long long> pantographToBits = {
    {PantographPosition::Down, 1},
    {PantographPosition::Up, 2},
    {PantographPosition::UpConnected, 4},
};

CANControlMessage::CANControlMessage() {
    NID_MESSAGE = MessageID::CANControls;
}

CANControlMessage::CANControlMessage(DrivingLeverPosition drivingLever,
                                     DirectionLeverPosition directionLever,
                                     EngineBrakeLeverPosition engineBrake,
                                     PantographPosition pantograph,
                                     bool generalStop,
                                     bool leftTimeout,
                                     bool rightTimeout,
                                     bool lights,
                                     bool farLights,
                                     bool openLeftDoor,
                                     bool openRightDoor,
                                     bool horn,
                                     bool sander,
                                     bool emergencyStop,
                                     bool cab) :
    drivingLever(drivingLever),
    directionLever(directionLever),
    engineBrake(engineBrake),
    pantograph(pantograph),
    generalStop(generalStop),
    leftTimeout(leftTimeout),
    rightTimeout(rightTimeout),
    lights(lights),
    farLights(farLights),
    openLeftDoor(openLeftDoor),
    openRightDoor(openRightDoor),
    horn(horn),
    sander(sander),
    emergencyStop(emergencyStop),
    cab(cab) {
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
    unsigned long long engineBrakeBits = engineBrakeToBits.at(GetEngineBrakeLever());
    bits.Write(GetRightDoor(), 8, 1);
    bits.Write(GetLeftDoor(), 9, 1);
    bits.Write(GetGeneralStop(), 10, 1);
    bits.Write(GetCabControl(), 11, 1);
    bits.Write(GetLights(), 24, 1);
    bits.Write(GetRightTimeout(), 25, 1);
    bits.Write(GetFarLights(), 26, 1);
    bits.Write(GetEmergencyStop(), 27, 1);
    bits.Write(GetLeftTimeout(), 28, 1);
    bits.Write(GetHorn(), 29, 1);
    bits.Write(GetSander(), 30, 1);
    bits.Write(engineBrakeBits & 3, 40, 2);
    bits.Write(directionsToBits.at(GetDirectionLever()), 42, 2);
    bits.Write(pantographToBits.at(GetPantograph()), 44, 3);
    bits.Write(drivingLeverToBits.at(GetDrivingLever()), 56, 5);
    bits.Write(engineBrakeBits >> 2, 61, 3);
    return bits;
}

void CANControlMessage::FromBitstring(const Bitstring& bits) {
    try {
        openRightDoor = bits.At(8, 1);
        openLeftDoor = bits.At(9, 1);
        generalStop = bits.At(10, 1);
        cab = bits.At(11, 1);
        lights = bits.At(24, 1);
        rightTimeout = bits.At(25, 1);
        farLights = bits.At(26, 1);
        emergencyStop = bits.At(27, 1);
        leftTimeout = bits.At(28, 1);
        horn = bits.At(29, 1);
        sander = bits.At(30, 1);
        directionLever = directionLeverSerializations.at(bits.At(42, 2));
        pantograph = pantographSerializations.at(bits.At(44, 3));
        drivingLever = drivingLeverSerializations.at(bits.At(56, 5));
        engineBrake = engineBrakeSerializations.at((bits.At(61, 3) << 2) | bits.At(40, 2));
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

EngineBrakeLeverPosition CANControlMessage::GetEngineBrakeLever() const noexcept {
    return engineBrake;
}

PantographPosition CANControlMessage::GetPantograph() const noexcept {
    return pantograph;
}

bool CANControlMessage::GetGeneralStop() const noexcept {
    return generalStop;
}

bool CANControlMessage::GetLeftTimeout() const noexcept {
    return leftTimeout;
}

bool CANControlMessage::GetRightTimeout() const noexcept {
    return rightTimeout;
}

bool CANControlMessage::GetLights() const noexcept {
    return lights;
}

bool CANControlMessage::GetFarLights() const noexcept {
    return farLights;
}

bool CANControlMessage::GetLeftDoor() const noexcept {
    return openLeftDoor;
}

bool CANControlMessage::GetRightDoor() const noexcept {
    return openRightDoor;
}

bool CANControlMessage::GetHorn() const noexcept {
    return horn;
}

bool CANControlMessage::GetSander() const noexcept {
    return sander;
}

bool CANControlMessage::GetEmergencyStop() const noexcept {
    return emergencyStop;
}

bool CANControlMessage::GetCabControl() const noexcept {
	return cab;
}
