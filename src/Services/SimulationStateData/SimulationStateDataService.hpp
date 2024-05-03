/** @file SimulationStateDataService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Service responsible for data about the state of Open Rails simulation
 *
 *  ###Contributors
 *  pavlian5
 */

#pragma once


#include "SimulationState.hpp"
#include "ISimulationStateDataService.hpp"
#include "AsyncProperty.hpp"

class SimulationStateDataService: public ISimulationStateDataService {
public:
    [[nodiscard]] SimulationState GetSimulationState() override;

    void SetSimulationState(const SimulationState& simulationState) override;

private:
    AsyncProperty<SimulationState> state;
};
