/** @file SimulationStateApiService.cpp
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

#include "SimulationStateApiService.hpp"
#include "cpr/response.h"
#include "cpr/api.h"
#include "OpenRailsApiConfiguration.hpp"
#include "IGuiSimulationStateSenderService.hpp"
#include "IOdoToEvcSenderService.hpp"


void SimulationStateApiService::Initialize(ServiceContainer& container) {
    simulationStateDataService = container.FetchService<ISimulationStateDataService>().get();
    btmService = container.FetchService<IBtmService>().get();
    jruLoggerService = container.FetchService<JRULoggerService>().get();
    configurationService = container.FetchService<ConfigurationService>().get();
    orcToGuiSender = container.FetchService<IGuiSimulationStateSenderService>().get();
    odoToEvcSender = container.FetchService<IOdoToEvcSenderService>().get();

    configurationService->FetchConfiguration<OpenRailsApiConfiguration>();
}

bool SimulationStateApiService::LpcSaidStart() {
    shouldStop = false;

    auto configuration = configurationService->FetchConfiguration<OpenRailsApiConfiguration>();
    url = configuration.orcUrl;
    apiCallingInterval = configuration.apiGetOrcCallingInterval;
    httpRequestTimeout = configuration.orcTimeout;

    simulationStateGettingThread = std::thread(&SimulationStateApiService::CallApiInALoop, this);
    orcToGuiSenderThread = std::thread(&SimulationStateApiService::SendMessagesToAComponent, this, orcToGuiSender, std::chrono::milliseconds(400));
    return true;
}

bool SimulationStateApiService::LpcSaidStop() {
    shouldStop = true;
    simulationStateGettingThread.join();
    orcToGuiSenderThread.join();
    if (odoToEvcSenderThread.get_id() != std::thread::id()) {   // thread is running
        odoToEvcSenderThread.join();
    }
    return true;
}

bool SimulationStateApiService::LpcSaidRestart() {
    return LpcSaidStop() && LpcSaidStart();
}

void SimulationStateApiService::CallApiInALoop() {
    while (!shouldStop) {
        auto start = std::chrono::steady_clock::now();
        cpr::Response response = cpr::Get(cpr::Url{url}, cpr::Timeout(httpRequestTimeout));

        if (response.status_code / 100 != 2) {
            // Detected error
            std::ostringstream error;
            error << "SimulationStateApiService: OpenRails API returned non 2xx response code. "
                  << "[status code: " << response.status_code << ']'
                  << "[error: " << response.error.message << ']';
            jruLoggerService->Log(true, MessageType::Error, error.str());
        } else {
            try {
                nlohmann::json j = nlohmann::json::parse(response.text);
                jruLoggerService->Log(true, MessageType::Debug, "SimulationStateApiService: Response from Open Rails: " + j.dump());
                SimulationState currentState = SimulationStateFromJson(j);
                currentState.distanceTravelledInMetres += startingPoint.GetMeters();    // add the starting offset received from EVC
                SimulationState previousState = simulationStateDataService->GetSimulationState();
                simulationStateDataService->SetSimulationState(currentState);
                btmService->CheckIfBaliseWasPassed(previousState.distanceTravelledInMetres, currentState.distanceTravelledInMetres);
            } catch (const std::exception& e) {
                std::ostringstream error;
                error << "SimulationStateApiService: Error while parsing json response from OpenRails";
                if (e.what() != nullptr && e.what()[0] != '\0') {
                    error << ": " << e.what();
                }
                jruLoggerService->Log(true, MessageType::Error, error.str());
            }
        }
        std::this_thread::sleep_until(start + apiCallingInterval);
    }
}

SimulationState SimulationStateApiService::SimulationStateFromJson(const nlohmann::json& json) {
    SimulationState state;
    state.distanceTravelledInMetres = json.at("DistanceTravelledInMeters");
    state.speedInMetresPerSecond = json.at("SpeedInMetersPerSecond");
    state.accelerationInMetersPerSecondSquared = json.at("AccelerationInMeteresPerSecondSquared");
    state.throttleLeverPercentage = json.at("ThrottleLeverPercentage");
    state.direction = json.at("Direction");
    state.motiveForceInNewtons = json.at("MotiveForceInNewtons");
    state.brakeCylinderPressureInPsi = json.at("BrakeCylinderPreasureInPsi");
    state.mainPipeBrakePressureInPsi = json.at("MainPipeBrakePreasureInPsi");

    return state;
}

void SimulationStateApiService::SendMessagesToAComponent(ISimulationStateSender* sender, std::chrono::milliseconds interval) {
    auto start = std::chrono::steady_clock::now();
    while(!shouldStop) {
        std::this_thread::sleep_until(start + interval);
        start = std::chrono::steady_clock::now();
        sender->SendSimulationState(simulationStateDataService->GetSimulationState());
    }
}

void SimulationStateApiService::SetStartingPoint(const Distance& _startingPoint) {
    startingPoint = _startingPoint;
}

void SimulationStateApiService::StartSendingOdoMessages(const std::chrono::milliseconds& interval) {
    if (odoToEvcSenderThread.get_id() == std::thread::id()) {   // thread is not running yet
        odoToEvcSenderThread = std::thread(&SimulationStateApiService::SendMessagesToAComponent, this, odoToEvcSender, interval);
    }
}
