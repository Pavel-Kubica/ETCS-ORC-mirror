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

class ISimulationStateApiService: public IService, public ILpcManageable {

};
