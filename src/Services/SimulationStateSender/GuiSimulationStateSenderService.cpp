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

void GuiSimulationStateSenderService::Initialize(ServiceContainer& container) {
    jruLoggerService = container.FetchService<JRULoggerService>().get();
    mqttPublisher = container.FetchService<IMqttPublisherService>().get();
}

void GuiSimulationStateSenderService::SendSimulationState(const SimulationState& simState) {
    OrcToGuiMessage orcToGuiMessage(simState.speedInMetresPerSecond, simState.motiveForceInNewtons,
                                    simState.brakeCylinderPressureInPsi, simState.mainPipeBrakePressureInPsi);

    jruLoggerService->Log(true, MessageType::Note,
                          "GuiSimulationSenderService: sending "
                          "[speed: %speed% m/s]"
                          "[motive-force:%force% N] "
                          "[brake-cylinder-pressure: %b_pressure% psi]"
                          "[main-pipe-pressure: %main_pressure% psi]",
                          simState.speedInMetresPerSecond, simState.motiveForceInNewtons,
                          simState.brakeCylinderPressureInPsi, simState.mainPipeBrakePressureInPsi);

    mqttPublisher->Publish(std::make_shared<OrcToGuiMessage>(orcToGuiMessage));
}