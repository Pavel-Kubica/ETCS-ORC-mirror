/** @file IHumanControlDataService.cpp
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

#include "MachineControlDataService.hpp"

bool MachineControlDataService::GetEmergencyBrake() const {
    return emergencyBrake;
}

void MachineControlDataService::SetEmergencyBrake(bool brake) {
    emergencyBrake = brake;
}

bool MachineControlDataService::GetServiceBrake() const {
    return serviceBrake;
}

void MachineControlDataService::SetServiceBrake(bool brake) {
    serviceBrake = brake;
}

MachineControlDataService::MachineControlDataService() : emergencyBrake(false), serviceBrake(false) {}
