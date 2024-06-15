/** @file HumanControlDataService.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Data service storing current train control data
 *
 *  ###Contributors
 *  kubicpa3
 *  rehorja8
 */

#include "HumanControlDataService.hpp"

HumanControlDataService::HumanControlDataService()
    : battery(false),
      cab(false),
      trainDirection(DirectionLeverPosition::Neutral),
      drivingLeverPosition(DrivingLeverPosition::Neutral),
      touchedRelease(false) {}

bool HumanControlDataService::GetBattery() const {
    return battery;
}

void HumanControlDataService::SetBattery(bool b) {
    battery = b;
}

bool HumanControlDataService::GetCab() const {
    return cab;
}

void HumanControlDataService::SetCab(bool c) {
    cab = battery = c;
}

DirectionLeverPosition HumanControlDataService::GetTrainDirection() const {
    return trainDirection;
}

void HumanControlDataService::SetTrainDirection(DirectionLeverPosition position) {
    trainDirection = position;
}

DrivingLeverPosition HumanControlDataService::GetDrivingLever() const {
    std::lock_guard l(leverPositionMutex);
    return drivingLeverPosition;
}

void HumanControlDataService::SetDrivingLever(DrivingLeverPosition position) {
    std::lock_guard l(leverPositionMutex);
    drivingLeverPosition = position;

    using enum DrivingLeverPosition;
    
    if (this->drivingLeverPosition == Accelerate || this->drivingLeverPosition == Continue) {
        this->touchedRelease = true;
    } else if (this->drivingLeverPosition == PneumaticBrake || this->drivingLeverPosition == QuickBrake) {
        this->touchedRelease = false;
    }
}
bool HumanControlDataService::HasTouchedRelease() const {
    std::lock_guard l(leverPositionMutex);
    return this->touchedRelease;
}
