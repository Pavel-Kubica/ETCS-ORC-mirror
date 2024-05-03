/** @file ICabControlApiService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Interface for the ICabControlApiService
 *
 *  ### Contributors
 *  rehorja8
 */


#pragma once


#include "ILpcManageable.hpp"
#include "DirectionLeverPosition.hpp"

class ICabControlApiService : public ILpcManageable {
public:
    virtual void Clear() = 0;
    virtual void SendAndClear() = 0;
    virtual void SetThrottle(double percentage) = 0;
    virtual void SetBrake(double percentage) = 0;
    virtual void SetDirection(DirectionLevelPosition position) = 0;
};
