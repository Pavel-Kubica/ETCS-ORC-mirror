/** @file LocalCabControlsDataService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  This data service stores the local state of cab controls.
 *  It does not represent the current numbers in Open Rails, only what we think they should be
 *
 *  ###Contributors
 *  kubicpa3
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

void LocalCabControlsDataService::SetEngineBrakeStep(double step) {
    if (step < 0 || step > 1)
        throw std::invalid_argument("Attempting to set invalid engine brake step size");
    engineBrakeStep = step;
}

double LocalCabControlsDataService::GetEngineBrakeStep() const {
    return engineBrakeStep;
}

void LocalCabControlsDataService::SetThrottle(double newThrottle) {
    throttle = newThrottle;
}

double LocalCabControlsDataService::GetThrottle() const {
    return throttle;
}

bool LocalCabControlsDataService::IncreaseThrottle() {
    throttle = throttle.GetValue() + throttleStep;
    if (throttle > 1.0)
        throttle = 1.0;
    return throttle < 1.0;
}

bool LocalCabControlsDataService::DecreaseThrottle() {
    throttle = throttle.GetValue() - throttleStep;
    if (throttle < 0)
        throttle = 0;
    return throttle > 0;
}

void LocalCabControlsDataService::SetEngineBrake(double newValue) {
    engineBrake = newValue;
}

double LocalCabControlsDataService::GetEngineBrake() const {
    return engineBrake;
}

bool LocalCabControlsDataService::IncreaseEngineBrake() {
    engineBrake = engineBrake.GetValue() + engineBrakeStep;
    if (engineBrake > 1.0)
        engineBrake = 1.0;
    return engineBrake < 1.0;
}

bool LocalCabControlsDataService::DecreaseEngineBrake() {
    engineBrake = engineBrake.GetValue() - engineBrakeStep;
    if (engineBrake < 0.0)
        engineBrake = 0.0;
    return engineBrake > 0.0;
}

void LocalCabControlsDataService::SetTrainBrake(OpenRailsTrainBrakeState newState) {
    trainBrakeState = newState;
}

OpenRailsTrainBrakeState LocalCabControlsDataService::GetTrainBrake() const {
    return trainBrakeState;
}

