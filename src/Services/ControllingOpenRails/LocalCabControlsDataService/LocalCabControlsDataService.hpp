/** @file LocalCabControlsDataService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  This service is used as a storage of member variables of ThrottleAndDynBrakeControlService.
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
    void SetThrottleStep(double step) override;
    [[nodiscard]] double GetThrottleStep() const override;

    void SetDynamicBrakeStep(double step) override;
    [[nodiscard]] double GetDynamicBrakeStep() const override;

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

private:
    std::atomic<double> throttleStep;
    std::atomic<double> dynamicBrakeStep;
    std::atomic<double> throttle;
    std::atomic<double> dynamicBrake;
    std::atomic<TrainBrake> trainBrakeState;
};