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
#include "EngineBrakeLeverPosition.hpp"
#include "PantographPosition.hpp"

class CANControlMessage : public Message {
public:
    CANControlMessage();
    CANControlMessage(DrivingLeverPosition drivingLever,
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
                      bool cab);
    void from_json(const nlohmann::json& j) override;
    nlohmann::json to_json() const override;
    void FromBitstring(const Bitstring& bits) override;
    Bitstring ToBitstring() const override;
    DrivingLeverPosition GetDrivingLever() const noexcept;
    DirectionLeverPosition GetDirectionLever() const noexcept;
    EngineBrakeLeverPosition GetEngineBrakeLever() const noexcept;
    PantographPosition GetPantograph() const noexcept;
    bool GetGeneralStop() const noexcept;
    bool GetLeftTimeout() const noexcept;
    bool GetRightTimeout() const noexcept;
    bool GetLights() const noexcept;
    bool GetFarLights() const noexcept;
    bool GetLeftDoor() const noexcept;
    bool GetRightDoor() const noexcept;
    bool GetHorn() const noexcept;
    bool GetSander() const noexcept;
    bool GetEmergencyStop() const noexcept;
    bool GetCabControl() const noexcept;

private:
    DrivingLeverPosition drivingLever;
    DirectionLeverPosition directionLever;
    EngineBrakeLeverPosition engineBrake;
    PantographPosition pantograph;
    bool generalStop;
    bool leftTimeout;
    bool rightTimeout;
    bool lights;
    bool farLights;
    bool openLeftDoor;
    bool openRightDoor;
    bool horn;
    bool sander;
    bool emergencyStop;
    bool cab;
};
