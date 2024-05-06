/** @file OdoToEvcSenderService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ###Description
 *  Service for sending Odo messages to Evc
 *
 *  ###Contributors
 *  nguyem12
 */

#pragma once


#include "IOdoToEvcSenderService.hpp"
#include "IInitializable.hpp"
#include "ServiceContainer.hpp"
#include "IMqttPublisherService.hpp"


class OdoToEvcSenderService : public IOdoToEvcSenderService,
                              public IInitializable {
public:
    void Initialize(ServiceContainer& container) override;

    void SendSimulationState(const SimulationState& simulationState) override;

private:
    IMqttPublisherService* mqttPublisher{};

    static constexpr uint16_t Q_CONTROL = 0b100000000;
    static constexpr uint16_t NID_C = 0;
    static constexpr uint32_t NID_BG = 16383;
    static constexpr uint16_t DL_DOUBTOVER = 100;
    static constexpr uint16_t DL_DOUBTUNDER = 100;
    static constexpr uint16_t V_DOUBTPOS = 0;
    static constexpr uint16_t V_DOUBTNEG = 0;
};