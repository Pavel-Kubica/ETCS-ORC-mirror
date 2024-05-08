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


void GuiSimulationStateSenderService::Initialize(ServiceContainer &container) {
    mqttPublisher = container.FetchService<IMqttPublisherService>().get();
}

void GuiSimulationStateSenderService::SendSimulationState(const SimulationState& simState) {
    OrcToGuiMessage orcToGuiMessage(
        simState.speedInMetresPerSecond,
        simState.motiveForceInNewtons,
        simState.brakeCylinderPressureInPsi,
        simState.mainPipeBrakePressureInPsi
    );

    mqttPublisher->Publish(std::make_shared<OrcToGuiMessage>(orcToGuiMessage));
}