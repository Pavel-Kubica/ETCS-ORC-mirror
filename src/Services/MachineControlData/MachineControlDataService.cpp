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

#include "MachineControlDataService.hpp"

bool MachineControlDataService::getEmergencyBrake() const {
    return emergencyBrake;
}

void MachineControlDataService::setEmergencyBrake(bool brake) {
    emergencyBrake = brake;
}

bool MachineControlDataService::getServiceBrake() const {
    return serviceBrake;
}

void MachineControlDataService::setServiceBrake(bool brake) {
    serviceBrake = brake;
}

MachineControlDataService::MachineControlDataService() : emergencyBrake(false), serviceBrake(false)
{}
