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
 *  ### Contributors
 *  rehorja8
 */


#pragma once


#include "IService.hpp"
#include "DirectionLeverPosition.hpp"
#include "ControllingOpenRails/CabControlApiService/CabControlRequest/CabControlRequest.hpp"

class ICabControlApiService : public IService {
public:
    virtual void Send(const CabControlRequest& request) = 0;
    
    static constexpr ServiceType Type = ServiceType::CabControlApi;
};
