/** @file ILocalCabControlsDataService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Interface for LocalCabControlsDataService
 *
 *  ###Usage
 *  First set a throttle and engine brake step, and then use the Increase or Decrease methods
 *  to change the appropriate variables
 *
 *  ###Contributors
 *  kubicpa3
 */

#pragma once

#include "IService.hpp"
#include "TrainBrake.hpp"

class ILocalCabControlsDataService : public IService
{
public:
    // Sets by how much throttle should be increased/decreased with each incremental change
    virtual void SetThrottleStep(double step) = 0;
    [[nodiscard]] virtual double GetThrottleStep() const = 0;

    // Sets by how much the dynamic brake should be increased/decreased with each incremental change
    virtual void SetDynamicBrakeStep(double step) = 0;
    [[nodiscard]] virtual double GetDynamicBrakeStep() const = 0;

    // Sets by how much the engine brake should be increased/decreased with each incremental change
    virtual void SetEngineBrakeStep(double step) = 0;
    [[nodiscard]] virtual double GetEngineBrakeStep() const = 0;

    virtual void SetThrottle(double newValue) = 0;
    [[nodiscard]] virtual double GetThrottle() const = 0;
    // Returns true if throttle is not yet at its maximum value
    [[nodiscard]] virtual bool IncreaseThrottle() = 0;
    // Returns true if throttle is still above 0
    [[nodiscard]] virtual bool DecreaseThrottle() = 0;

    virtual void SetDynamicBrake(double newValue) = 0;
    [[nodiscard]] virtual double GetDynamicBrake() const = 0;
    // Returns true if dynamic brake is not yet at its maximum value
    [[nodiscard]] virtual bool IncreaseDynamicBrake() = 0;
    // Returns true if dynamic brake is still above 0
    [[nodiscard]] virtual bool DecreaseDynamicBrake() = 0;

    virtual void SetEngineBrake(double newValue) = 0;
    [[nodiscard]] virtual double GetEngineBrake() const = 0;
    // Returns true if engine brake is not yet at its maximum value
    [[nodiscard]] virtual bool IncreaseEngineBrake() = 0;
    // Returns true if engine brake is still above 0
    [[nodiscard]] virtual bool DecreaseEngineBrake() = 0;

    virtual void SetTrainBrake(TrainBrake newState) = 0;
    [[nodiscard]] virtual TrainBrake GetTrainBrake() const = 0;

    static constexpr ServiceType Type = ServiceType::LocalCabControlsData;
};