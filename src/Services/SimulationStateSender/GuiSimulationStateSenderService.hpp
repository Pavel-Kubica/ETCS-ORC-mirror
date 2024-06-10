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
#include "IInitializable.hpp"
#include "IMqttPublisherService.hpp"
#include "JRULoggerService.hpp"
#include "ServiceContainer.hpp"

class GuiSimulationStateSenderService : public IGuiSimulationStateSenderService,
                                        public IInitializable {
public:
    void Initialize(ServiceContainer& container) override;

    void SendSimulationState(const SimulationState& simulationState) override;

private:
    JRULoggerService* jruLoggerService;
    IMqttPublisherService* mqttPublisher{};
};