/** @file SimulationStateDataService.cpp
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

#include "SimulationStateDataService.hpp"

SimulationState SimulationStateDataService::GetSimulationState() {
    return state.GetValue();
}

void SimulationStateDataService::SetSimulationState(const SimulationState& simulationState) {
    state = simulationState;
}
