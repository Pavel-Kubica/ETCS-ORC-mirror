/** @file GuiSimulationStateSenderService.hpp
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

#pragma once

#include "IGuiSimulationStateSenderService.hpp"
#include "ServiceContainer.hpp"
#include "IInitializable.hpp"
#include "IMqttPublisherService.hpp"


class GuiSimulationStateSenderService : public IGuiSimulationStateSenderService,
                                        public IInitializable {
public:
    void Initialize(ServiceContainer&) override;

    void SendSimulationState(const SimulationState&) override;

private:
    IMqttPublisherService* mqttPublisher;
};