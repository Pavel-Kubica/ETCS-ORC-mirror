/** @file ISimulationStateApiService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Interface for SimulationStateApiService
 *
 *  ###Contributors
 *  pavlian5
 */

#pragma once


#include "ILpcManageable.hpp"
#include "IService.hpp"
#include "Distance.hpp"

class ISimulationStateApiService: public IService {
public:
    static constexpr ServiceType Type = ServiceType::SimulationStateApi;

    virtual void SetStartingPoint(const Distance& startingPoint) = 0;

    virtual void StartSendingOdoMessages(const std::chrono::milliseconds& interval) = 0;
};
