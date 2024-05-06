/** @file ISimulationStateSender.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Interface for simulation state sending services
 *
 *  ###Contributors
 *  nguyem12
 */

#pragma once

#include "SimulationState.hpp"

class ISimulationStateSender {
public:
    virtual void SendSimulationState(const SimulationState&) = 0;
};