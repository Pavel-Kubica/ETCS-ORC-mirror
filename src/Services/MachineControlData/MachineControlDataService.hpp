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

    [[nodiscard]] bool getEmergencyBrake() const override;

    void setEmergencyBrake(bool emergencyBrake) override;

    [[nodiscard]] bool getServiceBrake() const override;

    void setServiceBrake(bool serviceBrake) override;

private:
    bool emergencyBrake;
    bool serviceBrake;
};