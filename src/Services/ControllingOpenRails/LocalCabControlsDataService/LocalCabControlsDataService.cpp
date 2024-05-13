/** @file LocalCabControlsDataService.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  This service is used as a storage of member variables of IncrementalCabControlService.
 *
 *  This data service stores the local state of cab controls.
 *  It does not represent the current numbers in Open Rails, only what we think they should be.
 *  At the beginning, these values are zero initialised and then we "blindly" increase/decrease them
 *  (meaning we do not lookup these values from the Open Rails).
 *
 *  ###Contributors
 *  kubicpa3
 *  rehorja8
 */

#include "LocalCabControlsDataService.hpp"
#include <stdexcept>

void LocalCabControlsDataService::SetThrottleStep(double step) {
    if (step < 0 || step > 1)
        throw std::invalid_argument("Attempting to set invalid throttle step size");
    throttleStep = step;
}

double LocalCabControlsDataService::GetThrottleStep() const {
    return throttleStep;
}

void LocalCabControlsDataService::SetDynamicBrakeStep(double step) {
    if (step < 0 || step > 1)
        throw std::invalid_argument("Attempting to set invalid engine brake step size");
    dynamicBrakeStep = step;
}

double LocalCabControlsDataService::GetDynamicBrakeStep() const {
    return dynamicBrakeStep;
}

void LocalCabControlsDataService::SetThrottle(double newThrottle) {
    throttle = newThrottle;
}

double LocalCabControlsDataService::GetThrottle() const {
    return throttle;
}

bool LocalCabControlsDataService::IncreaseThrottle() {
    throttle += throttleStep;
    if (throttle > 1.0)
        throttle = 1.0;
    return throttle < 1.0;
}

bool LocalCabControlsDataService::DecreaseThrottle() {
    throttle -= throttleStep;
    if (throttle < 0)
        throttle = 0;
    return throttle > 0;
}

void LocalCabControlsDataService::SetDynamicBrake(double newValue) {
    dynamicBrake = newValue;
}

double LocalCabControlsDataService::GetDynamicBrake() const {
    return dynamicBrake;
}

bool LocalCabControlsDataService::IncreaseDynamicBrake() {
    dynamicBrake += dynamicBrakeStep;
    if (dynamicBrake > 1.0)
        dynamicBrake = 1.0;
    return dynamicBrake < 1.0;
}

bool LocalCabControlsDataService::DecreaseDynamicBrake() {
    dynamicBrake -= dynamicBrakeStep;
    if (dynamicBrake < 0.0)
        dynamicBrake = 0.0;
    return dynamicBrake > 0.0;
}

void LocalCabControlsDataService::SetTrainBrake(TrainBrake newState) {
    trainBrakeState = newState;
}

TrainBrake LocalCabControlsDataService::GetTrainBrake() const {
    return trainBrakeState;
}

