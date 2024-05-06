/** @file ITrainControlDataService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Interface for MachineControlDataService
 *
 *  ###Contributors
 *  kubicpa3
 */

#pragma once

#include "IService.hpp"

class IMachineControlDataService : public IService {
public:
    [[nodiscard]] virtual bool GetEmergencyBrake() const = 0;

    virtual void SetEmergencyBrake(bool brake) = 0;

    [[nodiscard]] virtual bool GetServiceBrake() const = 0;

    virtual void SetServiceBrake(bool brake) = 0;

    static constexpr ServiceType Type = ServiceType::MachineControlData;
};