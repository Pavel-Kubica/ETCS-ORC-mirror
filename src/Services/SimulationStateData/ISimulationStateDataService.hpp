/** @file ISimulationStateDataService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Interface for SimulationStateDataService
 *
 *  ###Contributors
 *  pavlian5
 */

#pragma once

#include "IService.hpp"
#include "SimulationState.hpp"

class ISimulationStateDataService: public IService {
public:
    virtual SimulationState GetSimulationState() = 0;

    /**
     * @return true if any change to simulationState was made, otherwise false
     */
    virtual void SetSimulationState(const SimulationState& simulationState) = 0;

    static constexpr ServiceType Type = ServiceType::SimulationStateData;
};
