/** @file LocalCabControlsDataService.hpp
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

#pragma once

#include <atomic>
#include "AsyncProperty.hpp"
#include "ILocalCabControlsDataService.hpp"
#include "TrainBrake.hpp"

class LocalCabControlsDataService : public ILocalCabControlsDataService
{
public:
    void SetThrottleIncStep(double step) override;
    [[nodiscard]] double GetThrottleIncStep() const override;

    void SetThrottleDecStep(double step) override;

    double GetThrottleDecStep() const override;

    void SetDynamicBrakeDecStep(double step) override;

    double GetDynamicBrakeDecStep() const override;

    void SetEngineBrakeDecStep(double step) override;

    double GetEngineBrakeDecStep() const override;

    void SetDynamicBrakeIncStep(double step) override;
    [[nodiscard]] double GetDynamicBrakeIncStep() const override;

    void SetEngineBrakeIncStep(double step) override;
    [[nodiscard]] double GetEngineBrakeIncStep() const override;

    void SetThrottle(double newThrottle) override;
    [[nodiscard]] double GetThrottle() const override;
    [[nodiscard]] bool IncreaseThrottle() override;
    [[nodiscard]] bool DecreaseThrottle() override;

    void SetDynamicBrake(double newValue) override;
    [[nodiscard]] double GetDynamicBrake() const override;
    [[nodiscard]] bool IncreaseDynamicBrake() override;
    [[nodiscard]] bool DecreaseDynamicBrake() override;
    
    void SetTrainBrake(TrainBrake newState) override;
    [[nodiscard]] TrainBrake GetTrainBrake() const override;

    void SetEngineBrake(double newValue) override;
    [[nodiscard]] double GetEngineBrake() const override;
    [[nodiscard]] bool IncreaseEngineBrake() override;
    [[nodiscard]] bool DecreaseEngineBrake() override;

private:
    std::atomic<double> throttleIncStep;
    std::atomic<double> dynamicBrakeIncStep;
    std::atomic<double> engineBrakeIncStep;
    std::atomic<double> throttleDecStep;
    std::atomic<double> dynamicBrakeDecStep;
    std::atomic<double> engineBrakeDecStep;
    std::atomic<double> throttle;
    std::atomic<double> dynamicBrake;
    std::atomic<double> engineBrake;
    std::atomic<TrainBrake> trainBrakeState;
};