/** @file ITrainControlDataService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Interface for TrainControlDataService
 *
 *  ###Contributors
 *  kubicpa3
 */

#pragma once

#include "IService.hpp"
#include "DirectionLeverPosition.hpp"
#include "DrivingLeverPosition.hpp"

class ITrainControlDataService : public IService {
public:
    [[nodiscard]] virtual bool GetBattery() const = 0;

    virtual void SetBattery(bool battery) = 0;

    [[nodiscard]] virtual bool GetCab() const = 0;

    virtual void SetCab(bool cab) = 0;

    [[nodiscard]] virtual DirectionLeverPosition GetTrainDirection() const = 0;

    virtual void SetTrainDirection(DirectionLeverPosition position) = 0;

    [[nodiscard]] virtual DrivingLeverPosition GetDrivingLever() const = 0;

    virtual void SetDrivingLever(DrivingLeverPosition position) = 0;

    static constexpr ServiceType Type = ServiceType::TrainControlData;
};