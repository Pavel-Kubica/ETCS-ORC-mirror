/** @file ITrainControlDataService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Data service storing current machine control data
 *
 *  ###Contributors
 *  kubicpa3
 */

#pragma once

#include "IMachineControlDataService.hpp"

class MachineControlDataService : public IMachineControlDataService {
public:
    MachineControlDataService();

    [[nodiscard]] bool GetEmergencyBrake() const override;

    void SetEmergencyBrake(bool emergencyBrake) override;

    [[nodiscard]] bool GetServiceBrake() const override;

    void SetServiceBrake(bool serviceBrake) override;

private:
    bool emergencyBrake;
    bool serviceBrake;
};