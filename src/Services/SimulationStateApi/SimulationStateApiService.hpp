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
#include <thread>
#include "ISimulationStateApiService.hpp"
#include "Btm/IBtmService.hpp"
#include "ISimulationStateDataService.hpp"
#include "ServiceContainer.hpp"
#include "ConfigurationService.hpp"
#include "ISimulationStateSender.hpp"
#include "JRULoggerService.hpp"
#include "Distance.hpp"

class SimulationStateApiService: public ISimulationStateApiService, public IInitializable, public ILpcManageable {
public:
    void Initialize(ServiceContainer& container) override;

    bool LpcSaidStart() override;

    bool LpcSaidStop() override;

    bool LpcSaidRestart() override;

    void SetStartingPoint(const Distance& startingPoint) override;

    void StartSendingOdoMessages(const std::chrono::milliseconds& interval) override;

private:
    void CallApiInALoop();

    void SendMessagesToAComponent(ISimulationStateSender* sender, std::chrono::milliseconds interval);

    SimulationState SimulationStateFromJson(const nlohmann::json& json);

    ISimulationStateDataService* simulationStateDataService;
    IBtmService* btmService;
    JRULoggerService* jruLoggerService;
    ConfigurationService* configurationService;
    ISimulationStateSender* orcToGuiSender;
    ISimulationStateSender* odoToEvcSender;
    std::atomic_bool shouldStop;
    std::string url;
    std::chrono::milliseconds httpRequestTimeout;
    std::chrono::milliseconds apiCallingInterval;
    std::thread simulationStateGettingThread;
    std::thread orcToGuiSenderThread;
    std::thread odoToEvcSenderThread;
    Distance startingPoint = 0;
};
