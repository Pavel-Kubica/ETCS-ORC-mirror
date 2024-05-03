/** @file SimulationStateApiService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Service responsible for getting simulation state from Open Rails
 *
 *  ###Contributors
 *  pavlian5
 */

#pragma once


#include <atomic>
#include "SimulationStateDataService.hpp"

class SimulationStateApiService {
public:

private:
    SimulationStateDataService* simulationStateDataService;
//    BtmService* btmService;
    std::atomic_bool shouldStop;
};
