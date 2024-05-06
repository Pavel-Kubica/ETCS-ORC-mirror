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


void GuiSimulationStateSenderService::Initialize(ServiceContainer &container) {
    this->mqttPublisher = container.FetchService<IMqttPublisherService>().get();
}

void GuiSimulationStateSenderService::SendSimulationState(const SimulationState& simulationState) {

}