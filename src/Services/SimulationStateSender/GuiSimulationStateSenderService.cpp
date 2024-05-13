/** @file GuiSimulationStateSenderService.cpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Service responsible for sending messages to Gui applications
 *
 *  ###Contributors
 *  nguyem12
 */

#include "GuiSimulationStateSenderService.hpp"
#include "OrcToGuiMessage.hpp"

static constexpr double PSI_TO_BAR = 0.06894744825494;

void GuiSimulationStateSenderService::Initialize(ServiceContainer& container) {
    jruLoggerService = container.FetchService<JRULoggerService>().get();
    mqttPublisher = container.FetchService<IMqttPublisherService>().get();
}

void GuiSimulationStateSenderService::SendSimulationState(const SimulationState& simState) {
    OrcToGuiMessage orcToGuiMessage(
            simState.speedInMetresPerSecond,
            simState.motiveForceInNewtons,
            simState.brakeCylinderPressureInPsi * PSI_TO_BAR,
            simState.mainPipeBrakePressureInPsi * PSI_TO_BAR,
            simState.mainReservoirPressureInPsi * PSI_TO_BAR);

    jruLoggerService->Log(true, MessageType::Note,
                          "GuiSimulationSenderService: sending "
                          "[speed: %speed% m/s]"
                          "[motive-force:%force% N] "
                          "[brake-cylinder-pressure: %b_pressure% bar]"
                          "[main-pipe-pressure: %main_pressure% bar]",
                          "[main-reservoir-pressure: %main_reservoir% bar]",
                          simState.speedInMetresPerSecond,
                          simState.motiveForceInNewtons,
                          simState.brakeCylinderPressureInPsi * PSI_TO_BAR,
                          simState.mainPipeBrakePressureInPsi * PSI_TO_BAR,
                          simState.mainReservoirPressureInPsi * PSI_TO_BAR);

    mqttPublisher->Publish(std::make_shared<OrcToGuiMessage>(orcToGuiMessage));
}