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
      touchedRelease(false),
      engineBrake(EngineBrakeLeverPosition::FullRelease),
      quickRelease(false),
      pantograph(true),
      horn(false),
      sander(false),
      emergencyBrake(false),
      forwardLight(ForwardLight::Day) {}

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

EngineBrakeLeverPosition HumanControlDataService::GetEngineBrake() const {
    return engineBrake;
}

void HumanControlDataService::SetEngineBrake(EngineBrakeLeverPosition position) {
    engineBrake = position;
}

bool HumanControlDataService::GetQuickRelease() const {
    return quickRelease;
}

void HumanControlDataService::SetQuickRelease(bool on) {
    quickRelease = on;
}

bool HumanControlDataService::GetHorn() const {
    return horn;
}

void HumanControlDataService::SetHorn(bool on) {
    horn = on;
}

bool HumanControlDataService::GetSander() const {
    return sander;
}

void HumanControlDataService::SetSander(bool on) {
    sander = on;
}

bool HumanControlDataService::GetPantograph() const {
    return pantograph;
}

void HumanControlDataService::SetPantograph(bool up) {
    pantograph = up;
}

bool HumanControlDataService::GetEmergencyBrake() const {
    return emergencyBrake;
}

void HumanControlDataService::SetEmergencyBrake(bool on) {
    emergencyBrake = on;
}

ForwardLight HumanControlDataService::GetLight() const {
    std::lock_guard lk(lightMutex);
    return forwardLight;
}

void HumanControlDataService::SetLight(ForwardLight light) {
    std::lock_guard lk(lightMutex);
    if ((forwardLight == ForwardLight::Off && light == ForwardLight::Far) ||
         forwardLight == ForwardLight::Far && light == ForwardLight::Off) {
        lightSkipped = true;
    }
    else {
        lightSkipped = false;
    }
    forwardLight = light;
}

bool HumanControlDataService::LightSkipped() const {
    std::lock_guard lk(lightMutex);
    return lightSkipped;
}

void HumanControlDataService::ClearLightSkipped() {
    std::lock_guard lk(lightMutex);
    lightSkipped = false;
}
