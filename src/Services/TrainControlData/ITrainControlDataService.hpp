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
    [[nodiscard]] virtual bool getBattery() const = 0;

    virtual void setBattery(bool battery) = 0;

    [[nodiscard]] virtual bool getCab() const = 0;

    virtual void setCab(bool cab) = 0;

    [[nodiscard]] virtual DirectionLeverPosition getTrainDirection() const = 0;

    virtual void setTrainDirection(DirectionLeverPosition position) = 0;

    [[nodiscard]] virtual DrivingLeverPosition getDrivingLever() const = 0;

    virtual void setDrivingLever(DrivingLeverPosition position) = 0;

    static constexpr ServiceType Type() { return ServiceType::TrainControlData; }
};