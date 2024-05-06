/** @file ITrainControlDataService.hpp
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

#include "TrainControlDataService.hpp"

bool TrainControlDataService::GetBattery() const {
    return battery;
}

void TrainControlDataService::SetBattery(bool b) {
    battery = b;
}

bool TrainControlDataService::GetCab() const {
    return cab;
}

void TrainControlDataService::SetCab(bool c) {
    cab = c;
}

DirectionLeverPosition TrainControlDataService::GetTrainDirection() const {
    return trainDirection;
}

void TrainControlDataService::SetTrainDirection(DirectionLeverPosition position) {
    trainDirection = position;
}

DrivingLeverPosition TrainControlDataService::GetDrivingLever() const {
    return drivingLeverPosition;
}

void TrainControlDataService::SetDrivingLever(DrivingLeverPosition position) {
    drivingLeverPosition = position;
}

TrainControlDataService::TrainControlDataService() : battery(false), cab(false),
                                                     trainDirection(DirectionLeverPosition::Neutral),
                                                     drivingLeverPosition(DrivingLeverPosition::Neutral) {}
