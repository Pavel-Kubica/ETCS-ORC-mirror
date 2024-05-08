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

#pragma once

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
    AsyncProperty<double> throttleStep;
    AsyncProperty<double> engineBrakeStep;

    AsyncProperty<double> throttle;
    AsyncProperty<double> engineBrake;
    AsyncProperty<OpenRailsTrainBrakeState> trainBrakeState;
};