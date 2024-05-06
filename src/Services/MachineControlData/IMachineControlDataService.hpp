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
    [[nodiscard]] virtual bool getEmergencyBrake() const = 0;

    virtual void setEmergencyBrake(bool brake) = 0;

    [[nodiscard]] virtual bool getServiceBrake() const = 0;

    virtual void setServiceBrake(bool brake) = 0;

    static constexpr ServiceType Type = ServiceType::MachineControlData;
};