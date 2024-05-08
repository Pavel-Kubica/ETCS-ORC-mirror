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
#include "ISimulationStateApiService.hpp"
#include "Btm/BtmService.hpp"
#include "SimulationStateDataService.hpp"
#include "ServiceContainer.hpp"
#include "ConfigurationService.hpp"

class SimulationStateApiService: public ISimulationStateApiService, public IInitializable, public ILpcManageable {
public:
    void Initialize(ServiceContainer& container) override;

    bool LpcSaidStart() override;

    bool LpcSaidStop() override;

    bool LpcSaidRestart() override;

private:
    void CallApiInALoop();

    SimulationState SimulationStateFromJson(const nlohmann::json& json);

    ISimulationStateDataService* simulationStateDataService;
    IBtmService* btmService;
    JRULoggerService* jruLoggerService;
    ConfigurationService* configurationService;
    std::atomic_bool shouldStop;
    std::string url;
    std::chrono::milliseconds httpRequestTimeout;
    std::chrono::milliseconds apiCallingInterval;
};
