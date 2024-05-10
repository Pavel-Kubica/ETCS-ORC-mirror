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
 */

#include "HumanControlDataService.hpp"

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
    cab = c;
}

DirectionLeverPosition HumanControlDataService::GetTrainDirection() const {
    return trainDirection;
}

void HumanControlDataService::SetTrainDirection(DirectionLeverPosition position) {
    trainDirection = position;
}

DrivingLeverPosition HumanControlDataService::GetDrivingLever() const {
    return drivingLeverPosition;
}

void HumanControlDataService::SetDrivingLever(DrivingLeverPosition position) {
    drivingLeverPosition = position;
}

HumanControlDataService::HumanControlDataService() : battery(false), cab(false),
                                                     trainDirection(DirectionLeverPosition::Neutral),
                                                     drivingLeverPosition(DrivingLeverPosition::Neutral) {}
