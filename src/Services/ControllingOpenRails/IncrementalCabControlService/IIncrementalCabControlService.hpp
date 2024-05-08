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
 */

#pragma once
#include "IService.hpp"

class IIncrementalCabControlService : public IService
{
public:
    virtual void StartIncreasingThrottle() = 0;
    virtual void StopChangingThrottle() = 0;
    virtual void StartDecreasingThrottle() = 0;

    virtual void StartIncreasingEngineBrake() = 0;
    virtual void StopChangingEngineBrake() = 0;
    virtual void StartDecreasingEngineBrake() = 0;

    static constexpr ServiceType Type = ServiceType::IncrementalCabControl;
};