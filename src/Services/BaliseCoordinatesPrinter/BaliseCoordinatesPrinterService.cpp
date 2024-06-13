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
    configurationService = container.FetchService<ConfigurationService>().get();
    filename = configurationService->FetchConfiguration<BaliseCoordinatesPrinterConfiguration>().baliseCoordinatesFilename;
    simulationStateDataService = container.FetchService<ISimulationStateDataService>().get();
    file = std::ofstream(filename);
    if (!file) {
        logger->Log(MessageType::Warning, "Balise coordinates output file was not created");
    }
}

void BaliseCoordinatesPrinterService::PrintBalisesOnCurrentPosition(Distance currentDistance, uint32_t baliseCnt) {
    SimulationState simulationState = simulationStateDataService->GetSimulationState();

    file << "balise-count:{" << baliseCnt << "} | world-location:" << simulationState.worldLocationString
           << " | world-position:" << simulationState.worldPositionString << std::endl;
}
