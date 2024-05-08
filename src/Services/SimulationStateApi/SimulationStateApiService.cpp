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


void SimulationStateApiService::Initialize(ServiceContainer& container) {
    simulationStateDataService = container.FetchService<SimulationStateDataService>().get();
    btmService = container.FetchService<BtmService>().get();
    jruLoggerService = container.FetchService<JRULoggerService>().get();
    configurationService = container.FetchService<ConfigurationService>().get();

    configurationService->FetchConfiguration<OpenRailsApiConfiguration>();
}

bool SimulationStateApiService::LpcSaidStart() {
    shouldStop = false;

    auto configuration = configurationService->FetchConfiguration<OpenRailsApiConfiguration>();
    url = configuration.orcUrl;
    apiCallingInterval = configuration.apiGetOrcCallingInterval;
    httpRequestTimeout = configuration.orcTimeout;

    std::thread(&SimulationStateApiService::CallApiInALoop, this);
    return true;
}

bool SimulationStateApiService::LpcSaidStop() {
    shouldStop = true;
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
                SimulationState currentState = SimulationStateFromJson(j);
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
    state.distanceTravelledInMetres = json.at("distanceTravelledInMetres");
    state.speedInMetresPerSecond = json.at("speedInMetresPerSecond");
    state.accelerationInMetersPerSecondSquared = json.at("accelerationInMetersPerSecondSquared");
    state.throttleLeverPercentage = json.at("throttleLeverPercentage");
    state.direction = json.at("direction");
    state.motiveForceInNewtons = json.at("motiveForceInNewtons");
    state.brakeCylinderPressureInPsi = json.at("brakeCylinderPressureInPsi");
    state.mainPipeBrakePressureInPsi = json.at("mainPipeBrakePressureInPsi");

    return state;
}
