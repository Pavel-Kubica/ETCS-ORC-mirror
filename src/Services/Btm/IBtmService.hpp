/** @file IBtmService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Interface for BtmService
 *
 *  ### Contributors
 *  pavlian5
 */

#pragma once


#include "IService.hpp"
#include "IInitializable.hpp"
#include "Distance.hpp"

class IBtmService: public IService {
public:
    virtual void CheckIfBaliseWasPassed(Distance previousDistance, Distance currentDistance) = 0;

    static constexpr ServiceType Type = ServiceType::Btm;
};
