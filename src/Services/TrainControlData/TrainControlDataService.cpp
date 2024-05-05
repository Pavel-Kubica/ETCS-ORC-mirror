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

bool TrainControlDataService::getBattery() const {
    return battery;
}

void TrainControlDataService::setBattery(bool b) {
    battery = b;
}

bool TrainControlDataService::getCab() const {
    return cab;
}

void TrainControlDataService::setCab(bool c) {
    cab = c;
}

DirectionLeverPosition TrainControlDataService::getTrainDirection() const {
    return trainDirection;
}

void TrainControlDataService::setTrainDirection(DirectionLeverPosition position) {
    trainDirection = position;
}

DrivingLeverPosition TrainControlDataService::getDrivingLever() const {
    return drivingLeverPosition;
}

void TrainControlDataService::setDrivingLever(DrivingLeverPosition position) {
    drivingLeverPosition = position;
}
