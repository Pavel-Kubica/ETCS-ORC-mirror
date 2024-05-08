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
#include "ILocalCabControlsDataService.hpp"
#include "AsyncProperty.hpp"

class LocalCabControlsDataService : public ILocalCabControlsDataService
{
public:
    void SetThrottleStep(double step) override;
    [[nodiscard]] double GetThrottleStep() const override;

    void SetEngineBrakeStep(double step) override;
    [[nodiscard]] double GetEngineBrakeStep() const override;

    void SetThrottle(double newThrottle) override;
    [[nodiscard]] double GetThrottle() const override;
    [[nodiscard]] bool IncreaseThrottle() override;
    [[nodiscard]] bool DecreaseThrottle() override;

    void SetEngineBrake(double newValue) override;
    [[nodiscard]] double GetEngineBrake() const override;
    [[nodiscard]] bool IncreaseEngineBrake() override;
    [[nodiscard]] bool DecreaseEngineBrake() override;

    void SetTrainBrake(OpenRailsTrainBrakeState newState) override;
    [[nodiscard]] OpenRailsTrainBrakeState GetTrainBrake() const override;

private:
    std::atomic<double> throttleStep;
    std::atomic<double> engineBrakeStep;
    std::atomic<double> throttle;
    std::atomic<double> engineBrake;
    std::atomic<OpenRailsTrainBrakeState> trainBrakeState;
};