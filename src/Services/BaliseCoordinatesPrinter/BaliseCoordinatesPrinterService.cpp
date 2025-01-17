/** @file BaliseCoordinatesPrinterService.cpp
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

#include "BaliseCoordinatesPrinterService.hpp"
#include "BaliseCoordinatesPrinterConfiguration.hpp"

void BaliseCoordinatesPrinterService::Initialize(ServiceContainer& container) {
    this->logger = container.FetchService<JRULoggerService>().get();
    configurationService = container.FetchService<ConfigurationService>().get();
    auto config = configurationService->FetchConfiguration<BaliseCoordinatesPrinterConfiguration>();
    filename = config.baliseCoordinatesFilename;
    shouldPrintBalises = config.shouldPrintBalises;
    simulationStateDataService = container.FetchService<ISimulationStateDataService>().get();
    file = std::ofstream(filename);
    if (!file && shouldPrintBalises) {
        logger->Log(true, MessageType::Warning,
                    "Balise coordinates output file could not be created but it is wanted by the config file: %file%",
                    BaliseCoordinatesPrinterConfiguration::FileName);
    }
    if (!shouldPrintBalises) {
        logger->Log(true, MessageType::Note,
                    "Logging of train coordinates when passing a balise is turned off (this is the default) - you can "
                    "turn this on in the config file: %file%",
                    BaliseCoordinatesPrinterConfiguration::FileName);
    }
}

void BaliseCoordinatesPrinterService::PrintBalisesOnCurrentPosition(Distance currentDistance, uint32_t baliseCnt) {
    if (!file || !shouldPrintBalises) {
        return;
    }
    SimulationState simulationState = simulationStateDataService->GetSimulationState();
    file << "balise-count:{" << baliseCnt << "} | world-location:" << simulationState.worldLocationString
         << " | world-position:" << simulationState.worldPositionString << std::endl;
}
