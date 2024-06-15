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

void LocalCabControlsDataService::SetThrottleIncStep(double step) {
    if (step < 0 || step > 1)
        throw std::invalid_argument("Attempting to set invalid throttle inc step size");
    throttleIncStep = step;
}

double LocalCabControlsDataService::GetThrottleIncStep() const {
    return throttleIncStep;
}

void LocalCabControlsDataService::SetThrottleDecStep(double step) {
    if (step < 0 || step > 1)
        throw std::invalid_argument("Attempting to set invalid throttle dec step size");
    throttleDecStep = step;
}

double LocalCabControlsDataService::GetThrottleDecStep() const {
    return throttleDecStep;
}

void LocalCabControlsDataService::SetDynamicBrakeIncStep(double step) {
    if (step < 0 || step > 1)
        throw std::invalid_argument("Attempting to set invalid dynamic brake inc step size");
    dynamicBrakeIncStep = step;
}

double LocalCabControlsDataService::GetDynamicBrakeIncStep() const {
    return dynamicBrakeIncStep;
}

void LocalCabControlsDataService::SetDynamicBrakeDecStep(double step) {
    if (step < 0 || step > 1)
        throw std::invalid_argument("Attempting to set invalid dynamic brake dec step size");
    dynamicBrakeDecStep = step;
}

double LocalCabControlsDataService::GetDynamicBrakeDecStep() const {
    return dynamicBrakeDecStep;
}

void LocalCabControlsDataService::SetEngineBrakeIncStep(double step) {
    if (step < 0 || step > 1)
        throw std::invalid_argument("Attempting to set invalid engine brake inc step size");
    engineBrakeIncStep = step;
}

double LocalCabControlsDataService::GetEngineBrakeIncStep() const {
    return engineBrakeIncStep;
}

void LocalCabControlsDataService::SetEngineBrakeDecStep(double step) {
    if (step < 0 || step > 1)
        throw std::invalid_argument("Attempting to set invalid engine brake dec step size");
    engineBrakeDecStep = step;
}

double LocalCabControlsDataService::GetEngineBrakeDecStep() const {
    return engineBrakeDecStep;
}

void LocalCabControlsDataService::SetThrottle(double newThrottle) {
    throttle = newThrottle;
}

double LocalCabControlsDataService::GetThrottle() const {
    return throttle;
}

bool LocalCabControlsDataService::IncreaseThrottle() {
    throttle += throttleIncStep;
    if (throttle > 1.0)
        throttle = 1.0;
    return throttle < 1.0;
}

bool LocalCabControlsDataService::DecreaseThrottle() {
    throttle -= throttleDecStep;
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
    dynamicBrake += dynamicBrakeIncStep;
    if (dynamicBrake > 1.0)
        dynamicBrake = 1.0;
    return dynamicBrake < 1.0;
}

bool LocalCabControlsDataService::DecreaseDynamicBrake() {
    dynamicBrake -= dynamicBrakeDecStep;
    if (dynamicBrake < 0.0)
        dynamicBrake = 0.0;
    return dynamicBrake > 0.0;
}

void LocalCabControlsDataService::SetEngineBrake(double newValue) {
    engineBrake = newValue;
}

double LocalCabControlsDataService::GetEngineBrake() const {
    return engineBrake;
}

bool LocalCabControlsDataService::IncreaseEngineBrake() {
    engineBrake += engineBrakeIncStep;
    if (engineBrake > 1.0)
        engineBrake = 1.0;
    return engineBrake < 1.0;
}

bool LocalCabControlsDataService::DecreaseEngineBrake() {
    engineBrake -= engineBrakeDecStep;
    if (engineBrake < 0.0)
        engineBrake = 0.0;
    return engineBrake > 0.0;
}

void LocalCabControlsDataService::SetTrainBrake(TrainBrake newState) {
    trainBrakeState = newState;
}

TrainBrake LocalCabControlsDataService::GetTrainBrake() const {
    return trainBrakeState;
}
