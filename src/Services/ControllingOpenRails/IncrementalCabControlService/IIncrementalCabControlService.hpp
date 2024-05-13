/** @file IIncrementalCabControlService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Interface for IncrementalCabControlService
 *
 *  ###Contributors
 *  kubicpa3
 *  rehorja8
 */

#pragma once
#include "IService.hpp"

class IIncrementalCabControlService : public IService
{
public:
    virtual void StartIncreasingThrottle() = 0;
    virtual void StopChangingThrottle() = 0;
    virtual void StartDecreasingThrottle() = 0;
    virtual void SetThrottleTo(double value) = 0;

    virtual void StartIncreasingDynamicBrake() = 0;
    virtual void StopChangingDynamicBrake() = 0;
    virtual void StartDecreasingDynamicBrake() = 0;
    virtual void SetDynamicBrakeTo(double value) = 0;

    static constexpr ServiceType Type = ServiceType::IncrementalCabControl;
};