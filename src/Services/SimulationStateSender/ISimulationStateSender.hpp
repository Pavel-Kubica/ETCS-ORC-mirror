/** @file ISimulationStateSender.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Parent interface for simulation state sending interfaces
 *
 *  ###Contributors
 *  nguyem12
 */

#pragma once

#include "SimulationState.hpp"

class ISimulationStateSender {
public:
    virtual void SendSimulationState(const SimulationState& simulationState) = 0;
};