/** @file BaliseCoordinatesPrinterService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Service responsible for printing balise coordinates to a file
 *
 *  ### Contributors
 *  pavlian5
 */

#pragma once

#include "IInitializable.hpp"
#include "IBaliseCoordinatesPrinterService.hpp"
#include "ConfigurationService.hpp"
#include "ISimulationStateDataService.hpp"
#include "JRULoggerService.hpp"

class BaliseCoordinatesPrinterService: public IBaliseCoordinatesPrinterService, public IInitializable {
public:
protected:
    void Initialize(ServiceContainer& container) override;

private:
    void PrintCurrentPosition(Distance currentDistance) override;

private:
    ConfigurationService* configurationService;
    ISimulationStateDataService* simulationStateDataService;
    JRULoggerService* logger;
    std::string filename;
    std::ofstream file;
};
