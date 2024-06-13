/** @file IBaliseCoordinatesPrinterService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Interface for BaliseCoordinatesPrinterService
 *
 *  ### Contributors
 *  pavlian5
 */

#pragma once

#include "IService.hpp"
#include "Distance.hpp"


class IBaliseCoordinatesPrinterService : public IService {
public:
    virtual void PrintBalisesOnCurrentPosition(Distance currentDistance, uint32_t baliseCnt) = 0;
    
    static constexpr ServiceType Type = ServiceType::BaliseCoordinatesPrinter;
};
