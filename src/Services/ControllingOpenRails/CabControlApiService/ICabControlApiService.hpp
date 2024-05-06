/** @file ICabControlApiService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Service for POSTing to the OpenRails API in order to manage
 *  the train's controls (brakes, throttle, ...).
 *
 *  ### Usage
 *  The service should send multiple controls in a single request.
 *  For setting of individual controls, you use the SetXxx() methods. Then you call the SendAndClear() method.
 *  This POSTs the controls and clears the internal state of the service (the state is controls set by the SetXxx() methods).
 *  You can also clear the state by calling Clear().
 *
 *  ### Contributors
 *  rehorja8
 */


#pragma once


#include "IService.hpp"
#include "DirectionLeverPosition.hpp"

class ICabControlApiService : public IService {
public:
    virtual void Clear() = 0;
    virtual void SendAndClear() = 0;
    virtual void SetThrottle(double percentage) = 0;
    virtual void SetBrake(double percentage) = 0;
    virtual void SetDirection(DirectionLeverPosition position) = 0;
    
    static constexpr ServiceType Type = ServiceType::CabControlApi;
};
