/** @file IHumanControlDataService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Interface for HumanControlDataService
 *
 *  ###Contributors
 *  kubicpa3
 */

#pragma once

#include "IService.hpp"
#include "DirectionLeverPosition.hpp"
#include "DrivingLeverPosition.hpp"
#include "EngineBrakeLeverPosition.hpp"
#include "ForwardLight.hpp"

class IHumanControlDataService : public IService {
public:
    [[nodiscard]] virtual bool GetBattery() const = 0;

    virtual void SetBattery(bool battery) = 0;

    [[nodiscard]] virtual bool GetCab() const = 0;

    virtual void SetCab(bool cab) = 0;

    [[nodiscard]] virtual DirectionLeverPosition GetTrainDirection() const = 0;

    virtual void SetTrainDirection(DirectionLeverPosition position) = 0;

    [[nodiscard]] virtual DrivingLeverPosition GetDrivingLever() const = 0;

    virtual void SetDrivingLever(DrivingLeverPosition position) = 0;
    
    [[nodiscard]] virtual bool HasTouchedRelease() const = 0;

    [[nodiscard]] virtual EngineBrakeLeverPosition GetEngineBrake() const = 0;

    virtual void SetEngineBrake(EngineBrakeLeverPosition percentage) = 0;

    [[nodiscard]] virtual bool GetQuickRelease() const = 0;

    virtual void SetQuickRelease(bool on) = 0;

    [[nodiscard]] virtual bool GetHorn() const = 0;

    virtual void SetHorn(bool on) = 0;

    [[nodiscard]] virtual bool GetSander() const = 0;

    virtual void SetSander(bool sander) = 0;

    [[nodiscard]] virtual bool GetPantograph() const = 0;

    virtual void SetPantograph(bool up) = 0;

    [[nodiscard]] virtual bool GetEmergencyBrake() const = 0;

    virtual void SetEmergencyBrake(bool on) = 0;

    [[nodiscard]] virtual ForwardLight GetLight() const = 0;

    virtual void SetLight(ForwardLight light) = 0;

    // We cannot send an OR api request that wants to go directly from far lights to off, or from off to far
    virtual bool LightSkipped() const = 0;

    static constexpr ServiceType Type = ServiceType::TrainControlData;
};
